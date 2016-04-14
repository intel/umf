/* 
 * Copyright 2015 Intel(r) Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http ://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <gtest/gtest.h>

#include <memory>
#include <vmf/vmf.hpp>
#include "utils.hpp"

#if TARGET_OS_IPHONE
extern std::string tempPath;
#define TEST_FILE (tempPath + "global_test.avi")
#else
#define TEST_FILE "global_test.avi"
#endif /* TARGET_OS_IPHONE */

#define TEST_FILE_SRC VIDEO_FILE

using namespace vmf;

enum SerializerType
{
    TypeXML = 0,
    TypeJson = 1
};

class TestVideoSegments : public ::testing::TestWithParam<SerializerType>
{
protected:
    void SetUp()
    {
        copyFile(TEST_FILE_SRC, TEST_FILE);
        //vmf::initialize();

        stream.open(TEST_FILE, MetadataStream::Update);

        spSchema = std::make_shared<MetadataSchema>("schema");
        vFields.push_back(FieldDesc("field", Variant::type_integer));
        spDesc = std::make_shared<MetadataDesc>("desc", vFields);
        spSchema->add(spDesc);
        stream.addSchema(spSchema);

        std::shared_ptr<Metadata> md(new Metadata(spDesc));
        md->setFieldValue("field", 42);
        stream.add(md);

        segments.push_back(std::make_shared<MetadataStream::VideoSegment>("segment1", 30, 0, 1000, 800, 600));
        segments.push_back(std::make_shared<MetadataStream::VideoSegment>("segment2", 25, 5000, 1000));
        stream.addVideoSegment(segments[0]);
        stream.addVideoSegment(segments[1]);

        stream.save();
        stream.close();
    }

    void TearDown()
    {
	//vmf::terminate();
    }

    void compareSegments(const std::shared_ptr<MetadataStream::VideoSegment>& s1, const std::shared_ptr<MetadataStream::VideoSegment>& s2)
    {
        ASSERT_EQ(s1->getTitle(), s2->getTitle());
        ASSERT_EQ(s1->getFPS(), s2->getFPS());
        ASSERT_EQ(s1->getTime(), s2->getTime());
        ASSERT_EQ(s1->getDuration(), s2->getDuration());
        long w1, h1, w2, h2;
        s1->getResolution(w1, h1);
        s2->getResolution(w2, h2);
        ASSERT_EQ(w1, w2);
        ASSERT_EQ(h1, h2);
    }

    MetadataStream stream;

    std::unique_ptr<Format> format;
    std::shared_ptr< MetadataSchema > spSchema;
    std::shared_ptr< MetadataDesc > spDesc;
    std::vector< FieldDesc > vFields;
    std::vector< std::shared_ptr<MetadataStream::VideoSegment> > segments;
    vmf_string n_schema;
};

TEST_F(TestVideoSegments, SaveLoad)
{
    std::shared_ptr<vmf::MetadataStream::VideoSegment> segment1 =
        std::make_shared<vmf::MetadataStream::VideoSegment>("segment1", 30, 0);
    {
        vmf::MetadataStream stream;
        ASSERT_NO_THROW(stream.addVideoSegment(segment1));
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        auto loadedSegments = stream.getAllVideoSegments();
        ASSERT_EQ(segments.size(), loadedSegments.size());
        for (unsigned int i = 0; i < loadedSegments.size(); i++)
            compareSegments(segments[i], loadedSegments[i]);
        ASSERT_THROW(stream.addVideoSegment(segment1), vmf::IncorrectParamException);
        stream.getAllVideoSegments().clear();
        stream.addVideoSegment(segment1);
        stream.save();
        stream.close();
    }
    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        auto loadedSegments = stream.getAllVideoSegments();
        ASSERT_EQ(1u, loadedSegments.size());
        compareSegments(segment1, loadedSegments[0]);

        std::shared_ptr<vmf::MetadataStream::VideoSegment> nullSegment = nullptr;
        ASSERT_THROW(stream.addVideoSegment(nullSegment), vmf::NullPointerException);

        stream.close();
    }
}

TEST_P(TestVideoSegments, ParseSegmentsArray)
{
    SerializerType type = GetParam();
    format.reset(type == TypeXML ? (Format*) new FormatXML() : (Format*) new FormatJSON());

    std::string result = format->store({}, {}, segments);

    std::vector<std::shared_ptr<MetadataStream::VideoSegment>> loadedSegments;
    Format::AttribMap attribs;
    std::vector<MetadataInternal> metadata;
    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    std::vector<std::shared_ptr<Stat>> stats;
    Format::ParseCounters
        expected{ { 1, 1, 2, 0, 3 } },
        actual = format->parse(result, metadata, schemas, loadedSegments, stats, attribs);

    ASSERT_EQ(2u, loadedSegments.size());
    for(unsigned int i = 0; i < loadedSegments.size(); i++)
	compareSegments(segments[i], loadedSegments[i]);

    std::shared_ptr<MetadataStream::VideoSegment> nullSegment = nullptr;
    segments.emplace_back(nullSegment);
    ASSERT_THROW(format->store({}, {}, segments), vmf::NullPointerException);
}

TEST_P(TestVideoSegments, ParseSegmentsAll)
{
    SerializerType type = GetParam();
    format.reset(type == TypeXML ? (Format*) new FormatXML() : (Format*) new FormatJSON());

    std::string result = stream.serialize(*format);

    std::vector<std::shared_ptr<MetadataStream::VideoSegment>> loadedSegments;
    Format::AttribMap attribs;
    std::vector<MetadataInternal> metadata;
    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    std::vector<std::shared_ptr<Stat>> stats;
    Format::ParseCounters
        expected{ { 1, 1, 2, 0, 3 } },
        actual = format->parse(result, metadata, schemas, loadedSegments, stats, attribs);

    ASSERT_EQ(2u, loadedSegments.size());
    for(unsigned int i = 0; i < loadedSegments.size(); i++)
	compareSegments(segments[i], loadedSegments[i]);
}

TEST_P(TestVideoSegments, ParseAll)
{
    SerializerType type = GetParam();
    format.reset(type == TypeXML ? (Format*) new FormatXML() : (Format*) new FormatJSON());

    std::string result = stream.serialize(*format);

    MetadataStream testStream;
    testStream.deserialize(result, *format);

   auto loadedSegments = testStream.getAllVideoSegments();

    ASSERT_EQ(segments.size(), loadedSegments.size());
    for(unsigned int i = 0; i < loadedSegments.size(); i++)
	compareSegments(segments[i], loadedSegments[i]);
}

INSTANTIATE_TEST_CASE_P(UnitTestDS, TestVideoSegments, ::testing::Values(TypeXML, TypeJson) );

TEST(TestVideoSegmentsConvertation, TimestampToFrameIndex)
{
    vmf::MetadataStream stream;
    stream.addVideoSegment(std::make_shared<MetadataStream::VideoSegment>("segment1", 20, 0, 1000));
    stream.addVideoSegment(std::make_shared<MetadataStream::VideoSegment>("segment2", 25, 5000, 1000));
    stream.addVideoSegment(std::make_shared<MetadataStream::VideoSegment>("segment3", 5, 10000, 1000));
    stream.addVideoSegment(std::make_shared<MetadataStream::VideoSegment>("segment4", 10, 1000, 1000));
    stream.addVideoSegment(std::make_shared<MetadataStream::VideoSegment>("segment5", 20, 50000, 1000));

    long long frameIndex, numOfFrames;
    stream.convertTimestampToFrameIndex(1500, 100, frameIndex, numOfFrames);
    ASSERT_EQ(55, frameIndex);
    ASSERT_EQ(1, numOfFrames);

    stream.convertTimestampToFrameIndex(10500, 1000, frameIndex, numOfFrames);
    ASSERT_EQ(47, frameIndex);
    ASSERT_EQ(2, numOfFrames);

    stream.convertTimestampToFrameIndex(3000, 1000, frameIndex, numOfFrames);
    ASSERT_EQ(Metadata::UNDEFINED_FRAME_INDEX, frameIndex);
    ASSERT_EQ(Metadata::UNDEFINED_FRAMES_NUMBER, numOfFrames);
}

TEST(TestVideoSegmentsConvertation, FrameIndexToTimestamp)
{
    vmf::MetadataStream stream;
    stream.addVideoSegment(std::make_shared<MetadataStream::VideoSegment>("segment1", 20, 0, 1000));
    stream.addVideoSegment(std::make_shared<MetadataStream::VideoSegment>("segment2", 25, 5000, 1000));
    stream.addVideoSegment(std::make_shared<MetadataStream::VideoSegment>("segment3", 5, 10000, 1000));
    stream.addVideoSegment(std::make_shared<MetadataStream::VideoSegment>("segment4", 10, 1000, 1000));
    stream.addVideoSegment(std::make_shared<MetadataStream::VideoSegment>("segment5", 20, 50000, 1000));

    long long timestamp, duration;
    stream.convertFrameIndexToTimestamp(27, 10, timestamp, duration);
    ASSERT_EQ(5280, timestamp);
    ASSERT_EQ(400, duration);

    stream.convertFrameIndexToTimestamp(55, 1, timestamp, duration);
    ASSERT_EQ(1500, timestamp);
    ASSERT_EQ(100, duration);

    stream.convertFrameIndexToTimestamp(100, 10, timestamp, duration);
    ASSERT_EQ(Metadata::UNDEFINED_TIMESTAMP, timestamp);
    ASSERT_EQ(Metadata::UNDEFINED_DURATION, duration);
}
