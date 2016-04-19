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
#include "test_precomp.hpp"
#include <fstream>
#include "vmf/format_const.hpp"

#define TO_VECTOR(x) std::vector<int>(std::begin(x), std::end(x))

enum SerializerType
{
    TypeXML = 0,
    TypeJson = 1
};

using namespace vmf;

class TestSerialization : public ::testing::TestWithParam< std::tuple<SerializerType, vmf_string, CryptAlgo> >
{
protected:
    void SetUp()
    {
        n_schemaPeople = "people";
        n_schemaFrames = "frames";

        spSchemaFrames = std::make_shared<MetadataSchema>(n_schemaFrames);
        vFieldsFrames.push_back(FieldDesc("frameIdx", Variant::type_integer));
        vFieldsFrames.push_back(FieldDesc("score", Variant::type_real, true));
        vRefDescsFrames.emplace_back(std::make_shared<ReferenceDesc>("index1"));
        vRefDescsFrames.emplace_back(std::make_shared<ReferenceDesc>("index2"));
        spDescFrames = std::make_shared<MetadataDesc>("frame", vFieldsFrames, vRefDescsFrames);
        spSchemaFrames->add(spDescFrames);

        spSchemaPeople = std::make_shared<MetadataSchema>(n_schemaPeople);
        vFieldsPeople.push_back(FieldDesc("name", Variant::type_string));
        vFieldsPeople.push_back(FieldDesc("address", Variant::type_string, true));
        vRefDescsPeople.emplace_back(std::make_shared<ReferenceDesc>("friend"));
        vRefDescsPeople.emplace_back(std::make_shared<ReferenceDesc>("colleague"));
        spDescPeople = std::make_shared<MetadataDesc>("person", vFieldsPeople, vRefDescsPeople);
        spSchemaPeople->add(spDescPeople);

        stream.addSchema(spSchemaFrames);
        stream.addSchema(spSchemaPeople);

        std::shared_ptr<Metadata> person(new Metadata(spDescPeople));
        person->setFieldValue("name", "TestPersonName");
        person->setTimestamp(getTimestamp());
        set.push_back(person);
        stream.add(person);

        for(int i = 0; i < 10; i++)
        {
            std::shared_ptr<Metadata> md(new Metadata(spDescFrames));
            md->setFieldValue("frameIdx", i);
            md->setFrameIndex((long long)i << 33);
            set.push_back(md);
            stream.add(md);

            if(i%2 == 0)
            {
                md->addReference(person, (i%4? "index1" : ""));
                person->addReference(md, (i%4? "friend" : ""));
            }
        }

        segments.push_back(std::make_shared<MetadataStream::VideoSegment>("segment1", 30, 0, 1000, 800, 600));
        segments.push_back(std::make_shared<MetadataStream::VideoSegment>("segment2", 25, 5000, 1000));
    }

    void initFormat(SerializerType type, const std::string& compressorId, CryptAlgo algo, bool ignoreUnknownEncryptor = false)
    {
        encryptor = getEncryptor(algo);

        std::shared_ptr<Format> f, cf;
        switch (type)
        {
            case TypeXML:  f = std::make_shared<FormatXML>();  break;
            case TypeJson: f = std::make_shared<FormatJSON>(); break;
            default: VMF_EXCEPTION(IncorrectParamException, "Wrong serialization format type value: " + to_string(type));
        }
        cf.reset(new FormatCompressed(f, compressorId));
        format.reset(new FormatEncrypted(cf, encryptor, ignoreUnknownEncryptor));
    }

    void compareSchemas(const std::shared_ptr<MetadataSchema>& goldSchema, const std::shared_ptr<MetadataSchema>& testSchema, bool compareRefs = true)
    {
        ASSERT_EQ(goldSchema->getName(), testSchema->getName());
        ASSERT_EQ(goldSchema->getAuthor(), testSchema->getAuthor());

        auto goldDescs = goldSchema->getAll();
        auto testDescs = testSchema->getAll();
        ASSERT_EQ(goldDescs.size(), testDescs.size());

        for(unsigned int i = 0; i < testDescs.size(); i++)
        {
            ASSERT_EQ(goldDescs[i]->getMetadataName(), testDescs[i]->getMetadataName());

            auto goldFields = goldDescs[i]->getFields();
            auto testFileds = testDescs[i]->getFields();
            ASSERT_EQ(goldFields.size(), testFileds.size());

            for(unsigned int j = 0; j < testFileds.size(); j++)
            {
                ASSERT_EQ(goldFields[j].name, testFileds[j].name);
                ASSERT_EQ(goldFields[j].type, testFileds[j].type);
                ASSERT_EQ(goldFields[j].optional, testFileds[j].optional);
            }

            if (compareRefs)
            {
                auto goldRefDescs = goldDescs[i]->getAllReferenceDescs();
                auto testRefDescs = testDescs[i]->getAllReferenceDescs();
                ASSERT_EQ(goldRefDescs.size(), testRefDescs.size());

                for (unsigned int k = 0; k < testRefDescs.size(); k++)
                {
                    ASSERT_EQ(goldRefDescs[k]->name, testRefDescs[k]->name);
                    ASSERT_EQ(goldRefDescs[k]->isUnique, testRefDescs[k]->isUnique);
                    ASSERT_EQ(goldRefDescs[k]->isCustom, testRefDescs[k]->isCustom);
                }
            }
        }
    }

    void compareMetadata(const std::shared_ptr<Metadata>& goldMd, const std::shared_ptr<Metadata>& testMd, bool compareRefs = true)
    {
        ASSERT_EQ(goldMd->size(), testMd->size());
        ASSERT_EQ(goldMd->getSchemaName(), testMd->getSchemaName());
        ASSERT_EQ(goldMd->getName(), testMd->getName());
        ASSERT_EQ(goldMd->getFrameIndex(), testMd->getFrameIndex());
        ASSERT_EQ(goldMd->getNumOfFrames(), testMd->getNumOfFrames());
        ASSERT_EQ(goldMd->getTime(), testMd->getTime());
        ASSERT_EQ(goldMd->getDuration(), testMd->getDuration());
        ASSERT_EQ(goldMd->getId(), testMd->getId());
        ASSERT_EQ(goldMd->getFieldNames().size(), testMd->getFieldNames().size());

        auto fieldNames = goldMd->getFieldNames(), testNames = testMd->getFieldNames();
        for(auto goldName = fieldNames.begin(), testName = testNames.begin(); goldName != fieldNames.end() && testName !=  testNames.end(); goldName++, testName++)
        {
            ASSERT_EQ(*goldName, *testName);
            ASSERT_TRUE(goldMd->getFieldValue(*goldName) == testMd->getFieldValue(*testName));
        }
        if(compareRefs)
        {
            auto goldRefs = goldMd->getAllReferences(), testRefs = testMd->getAllReferences();
            ASSERT_EQ(goldRefs.size(), testRefs.size());

            std::sort(goldRefs.begin(), goldRefs.end(), [](Reference a, Reference b){return b.getReferenceMetadata().lock()->getId() < a.getReferenceMetadata().lock()->getId(); });
            std::sort(testRefs.begin(), testRefs.end(), [](Reference a, Reference b){return b.getReferenceMetadata().lock()->getId() < a.getReferenceMetadata().lock()->getId(); });

            for(unsigned int i = 0; i < testRefs.size(); i++)
            {
                ASSERT_EQ(goldRefs[i].getReferenceMetadata().lock()->getId(), testRefs[i].getReferenceMetadata().lock()->getId());
                ASSERT_EQ(goldRefs[i].getReferenceDescription()->name, testRefs[i].getReferenceDescription()->name);
            }
        }
    }

    void compareSegments(const std::shared_ptr<MetadataStream::VideoSegment>& idealSegment, const std::shared_ptr<MetadataStream::VideoSegment>& s)
    {
        ASSERT_EQ(idealSegment->getTitle(), s->getTitle());
        ASSERT_EQ(idealSegment->getFPS(), s->getFPS());
        ASSERT_EQ(idealSegment->getTime(), s->getTime());
        ASSERT_EQ(idealSegment->getDuration(), s->getDuration());
        long w1, h1, w2, h2;
        idealSegment->getResolution(w1, h1);
        s->getResolution(w2, h2);
        ASSERT_EQ(w1, w2);
        ASSERT_EQ(h1, h2);
    }

    MetadataStream stream;
    MetadataSet set;

    std::unique_ptr<Format> format;
    std::shared_ptr<vmf::Encryptor> encryptor;
    std::shared_ptr< MetadataSchema > spSchemaPeople, spSchemaFrames;
    std::shared_ptr< MetadataDesc > spDescPeople, spDescFrames;
    std::vector< FieldDesc > vFieldsPeople, vFieldsFrames;
    std::vector<std::shared_ptr<ReferenceDesc>> vRefDescsPeople, vRefDescsFrames;
    std::vector< std::shared_ptr<MetadataStream::VideoSegment>> segments;

    vmf_string n_schemaPeople, n_schemaFrames;
};


TEST_P(TestSerialization, StoreAll)
{
    SerializerType type         = std::get<0>(GetParam());
    std::string    compressorId = std::get<1>(GetParam());
    CryptAlgo      crypto       = std::get<2>(GetParam());
    initFormat(type, compressorId, crypto);

    std::vector<std::shared_ptr<MetadataSchema>> schemas;

    schemas.push_back(spSchemaPeople);
    schemas.push_back(spSchemaFrames);

    std::shared_ptr<vmf::MetadataStream::VideoSegment> nullSegment = nullptr;
    segments.push_back(nullSegment);

    ASSERT_THROW(format->store(set, schemas, segments), vmf::NullPointerException);

    segments.pop_back();
    
    std::shared_ptr<Metadata> nullElement = nullptr;
    set.push_back(nullElement);

    ASSERT_THROW(format->store(set, schemas, segments), vmf::IncorrectParamException);

    set.pop_back();

    std::shared_ptr< MetadataSchema > spSchemaNull = nullptr;
    schemas.push_back(spSchemaNull);

    ASSERT_THROW(format->store(set, schemas, segments), vmf::IncorrectParamException);

    set.clear();

    ASSERT_THROW(format->store(set, schemas, segments), vmf::NullPointerException);

    auto spNewDesc = std::make_shared<vmf::MetadataDesc>("new", vFieldsPeople, vRefDescsFrames);
    schemas.pop_back();
    std::string check = "";
    std::shared_ptr<Metadata> md1(new Metadata(spNewDesc));
    std::shared_ptr<Metadata> md2(new Metadata(spNewDesc));
    set.push_back(md1);
    set.push_back(md2);

    ASSERT_THROW(format->store(set, schemas, segments), vmf::IncorrectParamException);

    segments.clear();
    schemas.clear();
    std::vector<MetadataInternal> mdInt;
    std::vector<std::shared_ptr<Stat>> stats;
    Format::AttribMap attribs;

    ASSERT_THROW(format->parse("", mdInt, schemas, segments, stats, attribs), vmf::IncorrectParamException);
}


TEST_P(TestSerialization, Parse_schemasArray)
{
    SerializerType type         = std::get<0>(GetParam());
    std::string    compressorId = std::get<1>(GetParam());
    CryptAlgo      crypto       = std::get<2>(GetParam());
    initFormat(type, compressorId, crypto);

    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    schemas.push_back(spSchemaPeople);
    schemas.push_back(spSchemaFrames);
    std::string result = format->store({}, schemas);

    schemas.clear();

    std::vector<MetadataInternal> metadata;
    std::vector<std::shared_ptr<Stat>> stats;
    Format::AttribMap attribs;
    Format::ParseCounters
        expected{ { 0, 2, 0, 0, 0 } },
        actual = format->parse(result, metadata, schemas, segments, stats, attribs);
    ASSERT_EQ(TO_VECTOR(expected.cnt), TO_VECTOR(actual.cnt));
    for(const auto& spSchema : schemas)
    {
        compareSchemas(stream.getSchema(spSchema->getName()), spSchema);
    }
}


TEST_P(TestSerialization, Parse_schemasAll)
{
    SerializerType type         = std::get<0>(GetParam());
    std::string    compressorId = std::get<1>(GetParam());
    CryptAlgo      crypto       = std::get<2>(GetParam());
    initFormat(type, compressorId, crypto);
    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    schemas.push_back(spSchemaPeople);
    schemas.push_back(spSchemaFrames);
    std::string result = stream.serialize(*format);

    std::vector<MetadataInternal> metadata;
    std::vector<std::shared_ptr<Stat>> stats;
    Format::AttribMap attribs;
    Format::ParseCounters
        expected{ { 11, 2, 0, 0, 4 } },
        actual = format->parse(result, metadata, schemas, segments, stats, attribs);
    ASSERT_EQ(TO_VECTOR(expected.cnt), TO_VECTOR(actual.cnt));
    compareSchemas(stream.getSchema(schemas[0]->getName()), schemas[0]);
    compareSchemas(stream.getSchema(schemas[1]->getName()), schemas[1]);
}


TEST_P(TestSerialization, Parse_metadataArray)
{
    SerializerType type         = std::get<0>(GetParam());
    std::string    compressorId = std::get<1>(GetParam());
    CryptAlgo      crypto       = std::get<2>(GetParam());
    initFormat(type, compressorId, crypto);

    std::string result = format->store(set);

    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    schemas.push_back(spSchemaFrames);
    schemas.push_back(spSchemaPeople);
    
    std::vector<MetadataInternal> md;
    std::vector<std::shared_ptr<Stat>> stats;
    Format::AttribMap attribs;
    Format::ParseCounters
        expected{ { (int)set.size(), 0, 0, 0, 0 } },
        actual = format->parse(result, md, schemas, segments, stats, attribs);
    ASSERT_EQ(TO_VECTOR(expected.cnt), TO_VECTOR(actual.cnt));

    MetadataStream testStream;
    testStream.addSchema(spSchemaFrames);
    testStream.addSchema(spSchemaPeople);
    for(auto& item : md)
        testStream.add(item);

    ASSERT_EQ(set.size(), testStream.getAll().size());
    for (const auto& spItem : set)
        compareMetadata(spItem, testStream.getById(spItem->getId()) );

    std::shared_ptr<Metadata> nullElement = nullptr;
    set.push_back(nullElement);
    ASSERT_THROW(format->store(set), vmf::NullPointerException);
}


TEST_P(TestSerialization, Parse_metadataAll)
{
    SerializerType type         = std::get<0>(GetParam());
    std::string    compressorId = std::get<1>(GetParam());
    CryptAlgo      crypto       = std::get<2>(GetParam());
    initFormat(type, compressorId, crypto);

    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    schemas.push_back(spSchemaPeople);
    schemas.push_back(spSchemaFrames);
    std::string result = stream.serialize(*format);

    std::vector<MetadataInternal> md;
    std::vector<std::shared_ptr<Stat>> stats;
    Format::AttribMap attribs;
    Format::ParseCounters
        expected{ { (int)set.size(), (int)schemas.size(), 0, 0, 4 } },
        actual = format->parse(result, md, schemas, segments, stats, attribs);
    ASSERT_EQ(TO_VECTOR(expected.cnt), TO_VECTOR(actual.cnt));

    MetadataStream testStream;
    testStream.addSchema(spSchemaFrames);
    testStream.addSchema(spSchemaPeople);
    for(auto& item : md)
        testStream.add(item);

    ASSERT_EQ(set.size(), testStream.getAll().size());
    for (const auto& spItem : set)
        compareMetadata(spItem, testStream.getById(spItem->getId()));
}

TEST_P(TestSerialization, Parse_All)
{
    SerializerType type         = std::get<0>(GetParam());
    std::string    compressorId = std::get<1>(GetParam());
    CryptAlgo      crypto       = std::get<2>(GetParam());
    initFormat(type, compressorId, crypto);

    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    schemas.push_back(spSchemaPeople);
    schemas.push_back(spSchemaFrames);
    std::string result = stream.serialize(*format);

    MetadataStream testStream;
    testStream.deserialize(result, *format);

    ASSERT_EQ(2u, testStream.getAllSchemaNames().size());
    compareSchemas(spSchemaPeople, testStream.getSchema(n_schemaPeople));
    compareSchemas(spSchemaFrames, testStream.getSchema(n_schemaFrames));

    ASSERT_EQ(set.size(), testStream.getAll().size());
    for (const auto& spItem : set)
        compareMetadata(spItem, testStream.getById(spItem->getId()));
}

TEST_P(TestSerialization, Parse_segmentArray)
{
    SerializerType type         = std::get<0>(GetParam());
    std::string    compressorId = std::get<1>(GetParam());
    CryptAlgo      crypto       = std::get<2>(GetParam());
    initFormat(type, compressorId, crypto);

    std::string result = format->store({}, {}, segments);

    std::vector<std::shared_ptr<MetadataStream::VideoSegment>> loadedSegments;
    std::vector<MetadataInternal> md;
    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    std::vector<std::shared_ptr<Stat>> stats;
    Format::AttribMap attribs;
    Format::ParseCounters
        expected{ { 0, 0, (int)segments.size(), 0, 0 } },
        actual = format->parse(result, md, schemas, loadedSegments, stats, attribs);
    ASSERT_EQ(TO_VECTOR(expected.cnt), TO_VECTOR(actual.cnt));

    ASSERT_EQ(segments.size(), loadedSegments.size());
    for (size_t i = 0; i < segments.size(); i++)
    {
        compareSegments(segments[i], loadedSegments[i]);
    }
}

TEST_P(TestSerialization, CheckIgnoreUnknownCompressor)
{
    auto param = GetParam();

    vmf_string compressorId = "unknown_compressor";
    std::shared_ptr<Compressor> fake = std::make_shared<FakeCompressor>();
    std::dynamic_pointer_cast<FakeCompressor>(fake)->setId(compressorId);
    vmf::Compressor::registerNew(fake);

    SerializerType type = std::get<0>(param);
    CryptAlgo algo      = std::get<2>(param);
    std::shared_ptr<vmf::Encryptor> encryptor = getEncryptor(algo);

    std::shared_ptr<Format> f, cf;
    switch (type)
    {
        case TypeXML:  f = std::make_shared<FormatXML>();  break;
        case TypeJson: f = std::make_shared<FormatJSON>(); break;
        default: VMF_EXCEPTION(IncorrectParamException,
                               "Wrong serialization format type value: " + to_string(type));
    }
    cf.reset(new FormatCompressed(f, compressorId, true));
    bool ignoreUnknownEncryptor = true;
    format.reset(new FormatEncrypted(cf, encryptor, ignoreUnknownEncryptor));

    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    schemas.push_back(spSchemaPeople);
    schemas.push_back(spSchemaFrames);
    std::string result = stream.serialize(*format);

    vmf::Compressor::unregister(compressorId);

    std::vector<MetadataInternal> md;
    std::vector<std::shared_ptr<MetadataSchema>> schemas1;
    std::vector<std::shared_ptr<Stat>> stats;
    Format::AttribMap attribs;
    Format::ParseCounters
        expected{ { 1, 1, 0, 0, 1 } },
        actual = format->parse(result, md, schemas1, segments, stats, attribs);
    ASSERT_EQ(TO_VECTOR(expected.cnt), TO_VECTOR(actual.cnt));

    ASSERT_EQ(1u, schemas1.size());
    ASSERT_EQ(COMPRESSED_DATA_SCHEMA_NAME, schemas1[0]->getName());
}


TEST_P(TestSerialization, CheckIgnoreUnknownEncryptor)
{
    SerializerType type         = std::get<0>(GetParam());
    std::string    compressorId = std::get<1>(GetParam());
    CryptAlgo      algo         = std::get<2>(GetParam());

    if(algo != CryptAlgo::NONE)
    {
        initFormat(type, compressorId, algo);

        std::vector<std::shared_ptr<MetadataSchema>> schemas;
        schemas.push_back(spSchemaPeople);
        schemas.push_back(spSchemaFrames);
        std::string result = stream.serialize(*format);

        initFormat(type, compressorId, CryptAlgo::NONE, true);

        std::vector<MetadataInternal> md;
        std::vector<std::shared_ptr<MetadataSchema>> schemas1;
        std::vector<std::shared_ptr<Stat>> stats;
        Format::AttribMap attribs;
        Format::ParseCounters
            expected{ { 1, 1, 0, 0, 1 } },
            actual = format->parse(result, md, schemas1, segments, stats, attribs);
        ASSERT_EQ(TO_VECTOR(expected.cnt), TO_VECTOR(actual.cnt));

        ASSERT_EQ(1u, schemas1.size());
        ASSERT_EQ(ENCRYPTED_DATA_SCHEMA_NAME, schemas1[0]->getName());
    }
}


TEST_P(TestSerialization, EncryptOneField)
{
    SerializerType type         = std::get<0>(GetParam());
    std::string    compressorId = std::get<1>(GetParam());
    CryptAlgo      algo         = std::get<2>(GetParam());
    initFormat(type, compressorId, algo);

    MetadataSet toEncSet = stream.queryBySchema(n_schemaPeople);
    ASSERT_EQ(toEncSet.size(), 1);
    std::shared_ptr<Metadata> toBeEncrypted  = toEncSet[0];

    if(algo != CryptAlgo::NONE)
    {
        toBeEncrypted->findField("name")->setUseEncryption(true);
        stream.setEncryptor(encryptor);
    }

    std::string result = stream.serialize(*format);

    MetadataStream testStream;

    if(algo != CryptAlgo::NONE)
    {
        testStream.setEncryptor(encryptor);
    }

    testStream.deserialize(result, *format);

    std::shared_ptr<Metadata> encrypted = testStream.getById(toBeEncrypted->getId());
    ASSERT_TRUE((bool)encrypted);
    ASSERT_EQ(encrypted->findField("name")->getUseEncryption(), (algo != CryptAlgo::NONE));
    compareMetadata(toBeEncrypted, encrypted);
}


TEST_P(TestSerialization, EncryptOneRecord)
{
    SerializerType type         = std::get<0>(GetParam());
    std::string    compressorId = std::get<1>(GetParam());
    CryptAlgo      algo         = std::get<2>(GetParam());
    initFormat(type, compressorId, algo);

    MetadataSet toEncSet = stream.queryBySchema(n_schemaPeople);
    ASSERT_EQ(toEncSet.size(), 1);
    std::shared_ptr<Metadata> toBeEncrypted  = toEncSet[0];
    if(algo != CryptAlgo::NONE)
    {
        stream.setEncryptor(encryptor);
        toBeEncrypted->setUseEncryption(true);
    }

    std::string result = stream.serialize(*format);

    MetadataStream testStream;

    if(algo != CryptAlgo::NONE)
    {
        testStream.setEncryptor(encryptor);
    }

    testStream.deserialize(result, *format);

    std::shared_ptr<Metadata> encrypted = testStream.getById(toBeEncrypted->getId());
    ASSERT_TRUE((bool)encrypted);
    ASSERT_EQ(encrypted->getUseEncryption(), (algo != CryptAlgo::NONE));
    compareMetadata(toBeEncrypted, encrypted);
}


TEST_P(TestSerialization, EncryptFieldDesc)
{
    SerializerType type         = std::get<0>(GetParam());
    std::string    compressorId = std::get<1>(GetParam());
    CryptAlgo      algo         = std::get<2>(GetParam());
    initFormat(type, compressorId, algo);

    std::shared_ptr< MetadataSchema > schema = stream.getSchema(n_schemaPeople);
    std::shared_ptr< MetadataDesc > metadesc = schema->findMetadataDesc("person");
    FieldDesc& field = metadesc->getFieldDesc("name");
    if(algo != CryptAlgo::NONE)
    {
        stream.setEncryptor(encryptor);
        field.useEncryption = true;
    }

    MetadataSet toEncSet = stream.queryBySchema(n_schemaPeople);
    ASSERT_EQ(toEncSet.size(), 1);
    std::shared_ptr<Metadata> toBeEncrypted  = toEncSet[0];

    std::string result = stream.serialize(*format);

    MetadataStream testStream;

    if(algo != CryptAlgo::NONE)
    {
        testStream.setEncryptor(encryptor);
    }

    testStream.deserialize(result, *format);

    schema = testStream.getSchema(n_schemaPeople);
    metadesc = schema->findMetadataDesc("person");
    field = metadesc->getFieldDesc("name");
    ASSERT_EQ(field.useEncryption, (algo != CryptAlgo::NONE));

    std::shared_ptr<Metadata> encrypted = testStream.getById(toBeEncrypted->getId());
    ASSERT_TRUE((bool)encrypted);
    compareMetadata(toBeEncrypted, encrypted);
}


TEST_P(TestSerialization, EncryptMetaDesc)
{
    SerializerType type         = std::get<0>(GetParam());
    std::string    compressorId = std::get<1>(GetParam());
    CryptAlgo      algo         = std::get<2>(GetParam());
    initFormat(type, compressorId, algo);

    std::shared_ptr< MetadataSchema > schema = stream.getSchema(n_schemaPeople);
    std::shared_ptr< MetadataDesc > metadesc = schema->findMetadataDesc("person");
    if(algo != CryptAlgo::NONE)
    {
        stream.setEncryptor(encryptor);
        metadesc->setUseEncryption(true);
    }

    MetadataSet toEncSet = stream.queryBySchema(n_schemaPeople);
    ASSERT_EQ(toEncSet.size(), 1);
    std::shared_ptr<Metadata> toBeEncrypted  = toEncSet[0];

    std::string result = stream.serialize(*format);

    MetadataStream testStream;

    if(algo != CryptAlgo::NONE)
    {
        testStream.setEncryptor(encryptor);
    }

    testStream.deserialize(result, *format);

    schema = testStream.getSchema(n_schemaPeople);
    metadesc = schema->findMetadataDesc("person");
    ASSERT_EQ(metadesc->getUseEncryption(), (algo != CryptAlgo::NONE));

    std::shared_ptr<Metadata> encrypted = testStream.getById(toBeEncrypted->getId());
    ASSERT_TRUE((bool)encrypted);
    compareMetadata(toBeEncrypted, encrypted);
}


TEST_P(TestSerialization, EncryptSchema)
{
    SerializerType type         = std::get<0>(GetParam());
    std::string    compressorId = std::get<1>(GetParam());
    CryptAlgo      algo         = std::get<2>(GetParam());
    initFormat(type, compressorId, algo);

    std::shared_ptr< MetadataSchema > schema = stream.getSchema(n_schemaPeople);
    if(algo != CryptAlgo::NONE)
    {
        stream.setEncryptor(encryptor);
        schema->setUseEncryption(true);
    }

    MetadataSet toEncSet = stream.queryBySchema(n_schemaPeople);
    ASSERT_EQ(toEncSet.size(), 1);
    std::shared_ptr<Metadata> toBeEncrypted  = toEncSet[0];

    std::string result = stream.serialize(*format);

    MetadataStream testStream;

    if(algo != CryptAlgo::NONE)
    {
        testStream.setEncryptor(encryptor);
    }

    testStream.deserialize(result, *format);

    schema = testStream.getSchema(n_schemaPeople);

    ASSERT_EQ(schema->getUseEncryption(), (algo != CryptAlgo::NONE));

    std::shared_ptr<Metadata> encrypted = testStream.getById(toBeEncrypted->getId());
    ASSERT_TRUE((bool)encrypted);
    compareMetadata(toBeEncrypted, encrypted);
}


//don't check for incorrect compressors
INSTANTIATE_TEST_CASE_P(UnitTest, TestSerialization,
                        ::testing::Combine(::testing::Values(TypeXML, TypeJson),
                                           ::testing::Values("com.intel.vmf.compressor.zlib", ""),
                                           ::testing::Values(CryptAlgo::DEFAULT,
                                                             CryptAlgo::WEAK,
                                                             CryptAlgo::NONE)));
