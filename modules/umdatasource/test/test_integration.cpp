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

#if TARGET_OS_IPHONE
extern std::string tempPath;
#define INTEGRATION_TEST_FILE (tempPath + "integration_test_file.avi")
#else
#define INTEGRATION_TEST_FILE "integration_test_file.avi"
#endif /* TARGET_OS_IPHONE */

TEST(Integration, InitializeAndTerminate)
{
    ASSERT_NO_THROW(umf::initialize());
    ASSERT_NO_THROW(umf::terminate());
}

TEST(Integration, fullFunctionality_1)
{
    copyFile(VIDEO_FILE, INTEGRATION_TEST_FILE);
    //umf::initialize();
    const umf::umf_string atom(TEST_SCHEMA_NAME_0);
    const umf::umf_string atom(TEST_SCHEMA_NAME_1);
    const umf::umf_string atom(TEST_PROPERTY_NAME_0);
    const umf::umf_string atom(TEST_PROPERTY_NAME_1);
    const umf::umf_string atom(TEST_PROPERTY_NAME_2);
    const umf::umf_string atom(TEST_PROPERTY_NAME_3);
    const umf::umf_string atom(TEST_FIELD_NAME_0);
    const umf::umf_string atom(TEST_FIELD_NAME_1);
    const umf::umf_string atom(TEST_FIELD_NAME_2);
    
    {
        std::shared_ptr<umf::MetadataSchema> schema[2];
        schema[0] = std::shared_ptr<umf::MetadataSchema>(new umf::MetadataSchema(TEST_SCHEMA_NAME_0));
        schema[1] = std::shared_ptr<umf::MetadataSchema>(new umf::MetadataSchema(TEST_SCHEMA_NAME_1));
        std::shared_ptr<umf::MetadataDesc> desc[4];
        desc[0] = std::shared_ptr<umf::MetadataDesc>(new umf::MetadataDesc(TEST_PROPERTY_NAME_0, umf::Variant::type_integer));
        desc[2] = std::shared_ptr<umf::MetadataDesc>(new umf::MetadataDesc(TEST_PROPERTY_NAME_2, umf::Variant::type_integer));
        std::vector<umf::FieldDesc> fields[2];
        fields[0].push_back(umf::FieldDesc(TEST_FIELD_NAME_0, umf::Variant::type_string));
        fields[1].push_back(umf::FieldDesc(TEST_FIELD_NAME_1, umf::Variant::type_integer));
        fields[1].push_back(umf::FieldDesc(TEST_FIELD_NAME_2, umf::Variant::type_string));
        desc[1] = std::shared_ptr<umf::MetadataDesc>(new umf::MetadataDesc(TEST_PROPERTY_NAME_1, fields[0]));
        desc[3] = std::shared_ptr<umf::MetadataDesc>(new umf::MetadataDesc(TEST_PROPERTY_NAME_3, fields[1]));
        ASSERT_NO_THROW(schema[0]->add(desc[0]));
        ASSERT_NO_THROW(schema[0]->add(desc[1]));
        ASSERT_NO_THROW(schema[1]->add(desc[2]));
        ASSERT_NO_THROW(schema[1]->add(desc[3]));
        umf::MetadataStream s;
        ASSERT_TRUE(s.open(INTEGRATION_TEST_FILE, umf::MetadataStream::Update));
        ASSERT_NO_THROW(s.addSchema(schema[0]));
        ASSERT_NO_THROW(s.addSchema(schema[1]));
        ASSERT_TRUE(s.save());
        ASSERT_NO_THROW(s.close());
    }

    const umf::umf_integer TEST_INT32_VALUE_1 = 1;
    const umf::umf_integer TEST_INT32_VALUE_2 = 2;
    const umf::umf_integer TEST_INT32_VALUE_3 = 3;
    const umf::umf_integer TEST_INT32_VALUE_4 = 4;
    const umf::umf_integer TEST_INT32_VALUE_5 = 5;
    const umf::umf_string atom(TEST_STRING_VALUE_1);
    const umf::umf_string atom(TEST_STRING_VALUE_2);
    const umf::umf_string atom(TEST_STRING_VALUE_3);
    const umf::umf_string atom(TEST_STRING_VALUE_4);

    {
        umf::MetadataStream s;
        ASSERT_TRUE(s.open(INTEGRATION_TEST_FILE, umf::MetadataStream::Update));
        std::shared_ptr<umf::MetadataSchema> schema[2];
        schema[0] = s.getSchema(TEST_SCHEMA_NAME_0);
        ASSERT_TRUE(schema[0] != NULL);
        schema[1] = s.getSchema(TEST_SCHEMA_NAME_1);
        ASSERT_TRUE(schema[1] != NULL);
        std::shared_ptr<umf::MetadataDesc> desc[4];
        ASSERT_NO_THROW(desc[0] = schema[0]->findMetadataDesc(TEST_PROPERTY_NAME_0));
        ASSERT_NO_THROW(desc[1] = schema[0]->findMetadataDesc(TEST_PROPERTY_NAME_1));
        ASSERT_NO_THROW(desc[2] = schema[1]->findMetadataDesc(TEST_PROPERTY_NAME_2));
        ASSERT_NO_THROW(desc[3] = schema[1]->findMetadataDesc(TEST_PROPERTY_NAME_3));
        std::shared_ptr<umf::Metadata> metadata[4];
        metadata[0] = std::shared_ptr<umf::Metadata>(new umf::Metadata(desc[0]));
        metadata[1] = std::shared_ptr<umf::Metadata>(new umf::Metadata(desc[1]));
        metadata[2] = std::shared_ptr<umf::Metadata>(new umf::Metadata(desc[2]));
        metadata[3] = std::shared_ptr<umf::Metadata>(new umf::Metadata(desc[3]));
        metadata[0]->addValue(umf::Variant(TEST_INT32_VALUE_1));
        metadata[2]->addValue(umf::Variant(TEST_INT32_VALUE_3));
        metadata[1]->setFieldValue(TEST_FIELD_NAME_0, TEST_STRING_VALUE_1);
        metadata[3]->setFieldValue(TEST_FIELD_NAME_1, TEST_INT32_VALUE_5);
        metadata[3]->setFieldValue(TEST_FIELD_NAME_2, TEST_STRING_VALUE_3);
        for(int i = 0; i < 4; ++i) s.add(metadata[i]);
        ASSERT_TRUE(s.save());
        s.close();
    }

    {
        umf::MetadataStream s;
        ASSERT_TRUE(s.open(INTEGRATION_TEST_FILE, umf::MetadataStream::Update));
        umf::MetadataSet schema[2];
        s.load(TEST_SCHEMA_NAME_0);
        s.load(TEST_SCHEMA_NAME_1);
        schema[0] = s.queryBySchema(TEST_SCHEMA_NAME_0);
        schema[1] = s.queryBySchema(TEST_SCHEMA_NAME_1);
        umf::MetadataSet prop[4];
        prop[0] = schema[0].queryByName(TEST_PROPERTY_NAME_0);
        prop[1] = schema[0].queryByName(TEST_PROPERTY_NAME_1);
        prop[2] = schema[1].queryByName(TEST_PROPERTY_NAME_2);
        prop[3] = schema[1].queryByName(TEST_PROPERTY_NAME_3);
        std::shared_ptr<umf::Metadata> metadata[4];
        for(int i = 0; i < 4; ++i)
        {
            ASSERT_NO_THROW(metadata[i] = prop[i].at(0));
        }
        ASSERT_EQ(TEST_INT32_VALUE_1, (umf::umf_integer) metadata[0]->at(0));
        ASSERT_EQ(TEST_INT32_VALUE_3, (umf::umf_integer) metadata[2]->at(0));
        metadata[0]->addValue(umf::Variant(TEST_INT32_VALUE_2));
        ASSERT_EQ(TEST_STRING_VALUE_1, (umf::umf_string) metadata[1]->getFieldValue(TEST_FIELD_NAME_0));
        ASSERT_EQ(TEST_INT32_VALUE_5, (umf::umf_integer) metadata[3]->getFieldValue(TEST_FIELD_NAME_1));
        ASSERT_EQ(TEST_STRING_VALUE_3, (umf::umf_string) metadata[3]->getFieldValue(TEST_FIELD_NAME_2));
        metadata[1]->setFieldValue(TEST_FIELD_NAME_0, TEST_STRING_VALUE_2);
        metadata[3]->setFieldValue(TEST_FIELD_NAME_1, TEST_INT32_VALUE_4);
        metadata[3]->setFieldValue(TEST_FIELD_NAME_2, TEST_STRING_VALUE_4);
        s.save();
        s.close();
    }

    {
        umf::MetadataStream s;
        ASSERT_TRUE(s.open(INTEGRATION_TEST_FILE, umf::MetadataStream::ReadOnly));
        umf::MetadataSet schema[2];
        s.load(TEST_SCHEMA_NAME_0);
        s.load(TEST_SCHEMA_NAME_1);
        schema[0] = s.queryBySchema(TEST_SCHEMA_NAME_0);
        schema[1] = s.queryBySchema(TEST_SCHEMA_NAME_1);
        umf::MetadataSet prop[4];
        prop[0] = schema[0].queryByName(TEST_PROPERTY_NAME_0);
        prop[1] = schema[0].queryByName(TEST_PROPERTY_NAME_1);
        prop[2] = schema[1].queryByName(TEST_PROPERTY_NAME_2);
        prop[3] = schema[1].queryByName(TEST_PROPERTY_NAME_3);
        std::shared_ptr<umf::Metadata> metadata[4];
        for(int i = 0; i < 4; ++i)
        {
            ASSERT_NO_THROW(metadata[i] = prop[i].at(0));
        }

        ASSERT_EQ(TEST_INT32_VALUE_1, (umf::umf_integer) metadata[0]->at(0));
        ASSERT_EQ(TEST_INT32_VALUE_2, (umf::umf_integer) metadata[0]->at(1));
        ASSERT_EQ(TEST_INT32_VALUE_3, (umf::umf_integer) metadata[2]->at(0));
        ASSERT_EQ(TEST_STRING_VALUE_2, (umf::umf_string) metadata[1]->getFieldValue(TEST_FIELD_NAME_0));
        ASSERT_EQ(TEST_INT32_VALUE_4, (umf::umf_integer) metadata[3]->getFieldValue(TEST_FIELD_NAME_1));
        ASSERT_EQ(TEST_STRING_VALUE_4, (umf::umf_string) metadata[3]->getFieldValue(TEST_FIELD_NAME_2));
        s.close();
    }

    //umf::terminate();
}

TEST(Integration, Hongwu1)
{
    const umf::umf_string atom(TEST_SCHEMA_NAME);
    copyFile(VIDEO_FILE, INTEGRATION_TEST_FILE);
    //umf::initialize();
    {
        umf::MetadataStream stream;
        std::shared_ptr<umf::MetadataSchema> spSchema(new umf::MetadataSchema(TEST_SCHEMA_NAME));
        stream.open(INTEGRATION_TEST_FILE, umf::MetadataStream::Update);
        std::shared_ptr< umf::MetadataDesc > spEventDesc ( new umf::MetadataDesc( "event", umf::Variant::type_string ));
        spSchema->add( spEventDesc );
        std::shared_ptr< umf::Metadata > spBirthday( new umf::Metadata( spEventDesc ));
        spBirthday->addValue( "Birthday" );
        // 2500 frame starting from frame 4001
        spBirthday->setFrameIndex( 4001, 2500 );
        stream.addSchema(spSchema);
        stream.add( spBirthday );
        stream.save();
        stream.close();
    }
    {
        umf::MetadataStream stream;
        stream.open(INTEGRATION_TEST_FILE, umf::MetadataStream::Update);
        stream.load(TEST_SCHEMA_NAME);
        stream.close();
        umf::MetadataSet set = stream.queryByName("event");
        ASSERT_EQ(1u, set.size());
        std::shared_ptr<umf::Metadata> birthday = set.at(0);
        ASSERT_EQ(4001, birthday->getFrameIndex());
        ASSERT_EQ(2500, birthday->getNumOfFrames());
        ASSERT_EQ("Birthday", (umf::umf_string) birthday->at(0));
    }
    //umf::terminate();
}

TEST(Integration, Hongwu2)
{
    const umf::umf_string atom(TEST_SCHEMA_NAME);
    copyFile(VIDEO_FILE, INTEGRATION_TEST_FILE);
    //umf::initialize();
    {
        umf::MetadataStream stream;
        std::shared_ptr<umf::MetadataSchema> spSchema(new umf::MetadataSchema(TEST_SCHEMA_NAME));
        stream.open(INTEGRATION_TEST_FILE, umf::MetadataStream::Update);
        std::shared_ptr< umf::MetadataDesc > spNumbersDesc ( new umf::MetadataDesc( "ints", umf::Variant::type_integer ));
        spSchema->add( spNumbersDesc );
        std::shared_ptr< umf::Metadata > spNumbers( new umf::Metadata( spNumbersDesc ));
        spNumbers->addValue( (umf::umf_integer) 1);
        spNumbers->addValue( (umf::umf_integer) 2);
        spNumbers->addValue( (umf::umf_integer) 3);
        spNumbers->addValue( (umf::umf_integer) 4);
        spNumbers->addValue( (umf::umf_integer) 5);
        stream.addSchema(spSchema);
        stream.add( spNumbers );
        stream.save();
        stream.close();
    }
    {
        umf::MetadataStream stream;
        stream.open(INTEGRATION_TEST_FILE, umf::MetadataStream::ReadOnly);
        stream.load(TEST_SCHEMA_NAME);
        stream.close();
        umf::MetadataSet set = stream.queryByName("ints");
        ASSERT_EQ(1u, set.size());
        std::shared_ptr<umf::Metadata> numbers = set.at(0);
        ASSERT_EQ(5u, numbers->size());
        for(int i = 0; i < 5; ++i)
            ASSERT_EQ(i+1, (umf::umf_integer) numbers->at(i));
    }
    //umf::terminate();
}

TEST(Integration, structuresForDoc)
{
    copyFile(VIDEO_FILE, INTEGRATION_TEST_FILE);

    //umf::initialize();

    std::shared_ptr<umf::MetadataSchema> schema(new umf::MetadataSchema("my schema"));
    std::vector<umf::FieldDesc> fields;
    fields.push_back(umf::FieldDesc("name", umf::Variant::type_string));
    fields.push_back(umf::FieldDesc("last name", umf::Variant::type_string));
    fields.push_back(umf::FieldDesc("age", umf::Variant::type_integer));
    std::shared_ptr<umf::MetadataDesc> desc(new umf::MetadataDesc("people", fields));
    schema->add(desc);

    umf::MetadataStream stream;
    stream.open(INTEGRATION_TEST_FILE, umf::MetadataStream::Update);
    stream.addSchema(schema);
    std::shared_ptr<umf::Metadata> dima(new umf::Metadata(desc)), vasya(new umf::Metadata(desc));
    dima->setFieldValue("name", "Dmitry");
    dima->setFieldValue("last name", "Bogdanov");
    dima->setFieldValue("age", (umf::umf_integer) 21);

    vasya->setFieldValue("name", "Vasya");
    vasya->setFieldValue("last name", "Pupkin");
    vasya->setFieldValue("age", (umf::umf_integer) 18);

    stream.add(dima);
    stream.add(vasya);
    stream.save();
    stream.close();

    //umf::terminate();
}
