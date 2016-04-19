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
    ASSERT_NO_THROW(vmf::initialize());
    ASSERT_NO_THROW(vmf::terminate());
}

TEST(Integration, fullFunctionality_1)
{
    copyFile(VIDEO_FILE, INTEGRATION_TEST_FILE);
    //vmf::initialize();
    const vmf::vmf_string atom(TEST_SCHEMA_NAME_0);
    const vmf::vmf_string atom(TEST_SCHEMA_NAME_1);
    const vmf::vmf_string atom(TEST_PROPERTY_NAME_0);
    const vmf::vmf_string atom(TEST_PROPERTY_NAME_1);
    const vmf::vmf_string atom(TEST_PROPERTY_NAME_2);
    const vmf::vmf_string atom(TEST_PROPERTY_NAME_3);
    const vmf::vmf_string atom(TEST_FIELD_NAME_0);
    const vmf::vmf_string atom(TEST_FIELD_NAME_1);
    const vmf::vmf_string atom(TEST_FIELD_NAME_2);
    
    {
        std::shared_ptr<vmf::MetadataSchema> schema[2];
        schema[0] = std::shared_ptr<vmf::MetadataSchema>(new vmf::MetadataSchema(TEST_SCHEMA_NAME_0));
        schema[1] = std::shared_ptr<vmf::MetadataSchema>(new vmf::MetadataSchema(TEST_SCHEMA_NAME_1));
        std::shared_ptr<vmf::MetadataDesc> desc[4];
        desc[0] = std::shared_ptr<vmf::MetadataDesc>(new vmf::MetadataDesc(TEST_PROPERTY_NAME_0, vmf::Variant::type_integer));
        desc[2] = std::shared_ptr<vmf::MetadataDesc>(new vmf::MetadataDesc(TEST_PROPERTY_NAME_2, vmf::Variant::type_integer));
        std::vector<vmf::FieldDesc> fields[2];
        fields[0].push_back(vmf::FieldDesc(TEST_FIELD_NAME_0, vmf::Variant::type_string));
        fields[1].push_back(vmf::FieldDesc(TEST_FIELD_NAME_1, vmf::Variant::type_integer));
        fields[1].push_back(vmf::FieldDesc(TEST_FIELD_NAME_2, vmf::Variant::type_string));
        desc[1] = std::shared_ptr<vmf::MetadataDesc>(new vmf::MetadataDesc(TEST_PROPERTY_NAME_1, fields[0]));
        desc[3] = std::shared_ptr<vmf::MetadataDesc>(new vmf::MetadataDesc(TEST_PROPERTY_NAME_3, fields[1]));
        ASSERT_NO_THROW(schema[0]->add(desc[0]));
        ASSERT_NO_THROW(schema[0]->add(desc[1]));
        ASSERT_NO_THROW(schema[1]->add(desc[2]));
        ASSERT_NO_THROW(schema[1]->add(desc[3]));
        vmf::MetadataStream s;
        ASSERT_TRUE(s.open(INTEGRATION_TEST_FILE, vmf::MetadataStream::Update));
        ASSERT_NO_THROW(s.addSchema(schema[0]));
        ASSERT_NO_THROW(s.addSchema(schema[1]));
        ASSERT_TRUE(s.save());
        ASSERT_NO_THROW(s.close());
    }

    const vmf::vmf_integer TEST_INT32_VALUE_1 = 1;
    const vmf::vmf_integer TEST_INT32_VALUE_2 = 2;
    const vmf::vmf_integer TEST_INT32_VALUE_3 = 3;
    const vmf::vmf_integer TEST_INT32_VALUE_4 = 4;
    const vmf::vmf_integer TEST_INT32_VALUE_5 = 5;
    const vmf::vmf_string atom(TEST_STRING_VALUE_1);
    const vmf::vmf_string atom(TEST_STRING_VALUE_2);
    const vmf::vmf_string atom(TEST_STRING_VALUE_3);
    const vmf::vmf_string atom(TEST_STRING_VALUE_4);

    {
        vmf::MetadataStream s;
        ASSERT_TRUE(s.open(INTEGRATION_TEST_FILE, vmf::MetadataStream::Update));
        std::shared_ptr<vmf::MetadataSchema> schema[2];
        schema[0] = s.getSchema(TEST_SCHEMA_NAME_0);
        ASSERT_TRUE(schema[0] != NULL);
        schema[1] = s.getSchema(TEST_SCHEMA_NAME_1);
        ASSERT_TRUE(schema[1] != NULL);
        std::shared_ptr<vmf::MetadataDesc> desc[4];
        ASSERT_NO_THROW(desc[0] = schema[0]->findMetadataDesc(TEST_PROPERTY_NAME_0));
        ASSERT_NO_THROW(desc[1] = schema[0]->findMetadataDesc(TEST_PROPERTY_NAME_1));
        ASSERT_NO_THROW(desc[2] = schema[1]->findMetadataDesc(TEST_PROPERTY_NAME_2));
        ASSERT_NO_THROW(desc[3] = schema[1]->findMetadataDesc(TEST_PROPERTY_NAME_3));
        std::shared_ptr<vmf::Metadata> metadata[4];
        metadata[0] = std::shared_ptr<vmf::Metadata>(new vmf::Metadata(desc[0]));
        metadata[1] = std::shared_ptr<vmf::Metadata>(new vmf::Metadata(desc[1]));
        metadata[2] = std::shared_ptr<vmf::Metadata>(new vmf::Metadata(desc[2]));
        metadata[3] = std::shared_ptr<vmf::Metadata>(new vmf::Metadata(desc[3]));
        metadata[0]->addValue(vmf::Variant(TEST_INT32_VALUE_1));
        metadata[2]->addValue(vmf::Variant(TEST_INT32_VALUE_3));
        metadata[1]->setFieldValue(TEST_FIELD_NAME_0, TEST_STRING_VALUE_1);
        metadata[3]->setFieldValue(TEST_FIELD_NAME_1, TEST_INT32_VALUE_5);
        metadata[3]->setFieldValue(TEST_FIELD_NAME_2, TEST_STRING_VALUE_3);
        for(int i = 0; i < 4; ++i) s.add(metadata[i]);
        ASSERT_TRUE(s.save());
        s.close();
    }

    {
        vmf::MetadataStream s;
        ASSERT_TRUE(s.open(INTEGRATION_TEST_FILE, vmf::MetadataStream::Update));
        vmf::MetadataSet schema[2];
        s.load(TEST_SCHEMA_NAME_0);
        s.load(TEST_SCHEMA_NAME_1);
        schema[0] = s.queryBySchema(TEST_SCHEMA_NAME_0);
        schema[1] = s.queryBySchema(TEST_SCHEMA_NAME_1);
        vmf::MetadataSet prop[4];
        prop[0] = schema[0].queryByName(TEST_PROPERTY_NAME_0);
        prop[1] = schema[0].queryByName(TEST_PROPERTY_NAME_1);
        prop[2] = schema[1].queryByName(TEST_PROPERTY_NAME_2);
        prop[3] = schema[1].queryByName(TEST_PROPERTY_NAME_3);
        std::shared_ptr<vmf::Metadata> metadata[4];
        for(int i = 0; i < 4; ++i)
        {
            ASSERT_NO_THROW(metadata[i] = prop[i].at(0));
        }
        ASSERT_EQ(TEST_INT32_VALUE_1, (vmf::vmf_integer) metadata[0]->at(0));
        ASSERT_EQ(TEST_INT32_VALUE_3, (vmf::vmf_integer) metadata[2]->at(0));
        metadata[0]->addValue(vmf::Variant(TEST_INT32_VALUE_2));
        ASSERT_EQ(TEST_STRING_VALUE_1, (vmf::vmf_string) metadata[1]->getFieldValue(TEST_FIELD_NAME_0));
        ASSERT_EQ(TEST_INT32_VALUE_5, (vmf::vmf_integer) metadata[3]->getFieldValue(TEST_FIELD_NAME_1));
        ASSERT_EQ(TEST_STRING_VALUE_3, (vmf::vmf_string) metadata[3]->getFieldValue(TEST_FIELD_NAME_2));
        metadata[1]->setFieldValue(TEST_FIELD_NAME_0, TEST_STRING_VALUE_2);
        metadata[3]->setFieldValue(TEST_FIELD_NAME_1, TEST_INT32_VALUE_4);
        metadata[3]->setFieldValue(TEST_FIELD_NAME_2, TEST_STRING_VALUE_4);
        s.save();
        s.close();
    }

    {
        vmf::MetadataStream s;
        ASSERT_TRUE(s.open(INTEGRATION_TEST_FILE, vmf::MetadataStream::ReadOnly));
        vmf::MetadataSet schema[2];
        s.load(TEST_SCHEMA_NAME_0);
        s.load(TEST_SCHEMA_NAME_1);
        schema[0] = s.queryBySchema(TEST_SCHEMA_NAME_0);
        schema[1] = s.queryBySchema(TEST_SCHEMA_NAME_1);
        vmf::MetadataSet prop[4];
        prop[0] = schema[0].queryByName(TEST_PROPERTY_NAME_0);
        prop[1] = schema[0].queryByName(TEST_PROPERTY_NAME_1);
        prop[2] = schema[1].queryByName(TEST_PROPERTY_NAME_2);
        prop[3] = schema[1].queryByName(TEST_PROPERTY_NAME_3);
        std::shared_ptr<vmf::Metadata> metadata[4];
        for(int i = 0; i < 4; ++i)
        {
            ASSERT_NO_THROW(metadata[i] = prop[i].at(0));
        }

        ASSERT_EQ(TEST_INT32_VALUE_1, (vmf::vmf_integer) metadata[0]->at(0));
        ASSERT_EQ(TEST_INT32_VALUE_2, (vmf::vmf_integer) metadata[0]->at(1));
        ASSERT_EQ(TEST_INT32_VALUE_3, (vmf::vmf_integer) metadata[2]->at(0));
        ASSERT_EQ(TEST_STRING_VALUE_2, (vmf::vmf_string) metadata[1]->getFieldValue(TEST_FIELD_NAME_0));
        ASSERT_EQ(TEST_INT32_VALUE_4, (vmf::vmf_integer) metadata[3]->getFieldValue(TEST_FIELD_NAME_1));
        ASSERT_EQ(TEST_STRING_VALUE_4, (vmf::vmf_string) metadata[3]->getFieldValue(TEST_FIELD_NAME_2));
        s.close();
    }

    //vmf::terminate();
}

TEST(Integration, Hongwu1)
{
    const vmf::vmf_string atom(TEST_SCHEMA_NAME);
    copyFile(VIDEO_FILE, INTEGRATION_TEST_FILE);
    //vmf::initialize();
    {
        vmf::MetadataStream stream;
        std::shared_ptr<vmf::MetadataSchema> spSchema(new vmf::MetadataSchema(TEST_SCHEMA_NAME));
        stream.open(INTEGRATION_TEST_FILE, vmf::MetadataStream::Update);
        std::shared_ptr< vmf::MetadataDesc > spEventDesc ( new vmf::MetadataDesc( "event", vmf::Variant::type_string ));
        spSchema->add( spEventDesc );
        std::shared_ptr< vmf::Metadata > spBirthday( new vmf::Metadata( spEventDesc ));
        spBirthday->addValue( "Birthday" );
        // 2500 frame starting from frame 4001
        spBirthday->setFrameIndex( 4001, 2500 );
        stream.addSchema(spSchema);
        stream.add( spBirthday );
        stream.save();
        stream.close();
    }
    {
        vmf::MetadataStream stream;
        stream.open(INTEGRATION_TEST_FILE, vmf::MetadataStream::Update);
        stream.load(TEST_SCHEMA_NAME);
        stream.close();
        vmf::MetadataSet set = stream.queryByName("event");
        ASSERT_EQ(1u, set.size());
        std::shared_ptr<vmf::Metadata> birthday = set.at(0);
        ASSERT_EQ(4001, birthday->getFrameIndex());
        ASSERT_EQ(2500, birthday->getNumOfFrames());
        ASSERT_EQ("Birthday", (vmf::vmf_string) birthday->at(0));
    }
    //vmf::terminate();
}

TEST(Integration, Hongwu2)
{
    const vmf::vmf_string atom(TEST_SCHEMA_NAME);
    copyFile(VIDEO_FILE, INTEGRATION_TEST_FILE);
    //vmf::initialize();
    {
        vmf::MetadataStream stream;
        std::shared_ptr<vmf::MetadataSchema> spSchema(new vmf::MetadataSchema(TEST_SCHEMA_NAME));
        stream.open(INTEGRATION_TEST_FILE, vmf::MetadataStream::Update);
        std::shared_ptr< vmf::MetadataDesc > spNumbersDesc ( new vmf::MetadataDesc( "ints", vmf::Variant::type_integer ));
        spSchema->add( spNumbersDesc );
        std::shared_ptr< vmf::Metadata > spNumbers( new vmf::Metadata( spNumbersDesc ));
        spNumbers->addValue( (vmf::vmf_integer) 1);
        spNumbers->addValue( (vmf::vmf_integer) 2);
        spNumbers->addValue( (vmf::vmf_integer) 3);
        spNumbers->addValue( (vmf::vmf_integer) 4);
        spNumbers->addValue( (vmf::vmf_integer) 5);
        stream.addSchema(spSchema);
        stream.add( spNumbers );
        stream.save();
        stream.close();
    }
    {
        vmf::MetadataStream stream;
        stream.open(INTEGRATION_TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load(TEST_SCHEMA_NAME);
        stream.close();
        vmf::MetadataSet set = stream.queryByName("ints");
        ASSERT_EQ(1u, set.size());
        std::shared_ptr<vmf::Metadata> numbers = set.at(0);
        ASSERT_EQ(5u, numbers->size());
        for(int i = 0; i < 5; ++i)
            ASSERT_EQ(i+1, (vmf::vmf_integer) numbers->at(i));
    }
    //vmf::terminate();
}

TEST(Integration, structuresForDoc)
{
    copyFile(VIDEO_FILE, INTEGRATION_TEST_FILE);

    //vmf::initialize();

    std::shared_ptr<vmf::MetadataSchema> schema(new vmf::MetadataSchema("my schema"));
    std::vector<vmf::FieldDesc> fields;
    fields.push_back(vmf::FieldDesc("name", vmf::Variant::type_string));
    fields.push_back(vmf::FieldDesc("last name", vmf::Variant::type_string));
    fields.push_back(vmf::FieldDesc("age", vmf::Variant::type_integer));
    std::shared_ptr<vmf::MetadataDesc> desc(new vmf::MetadataDesc("people", fields));
    schema->add(desc);

    vmf::MetadataStream stream;
    stream.open(INTEGRATION_TEST_FILE, vmf::MetadataStream::Update);
    stream.addSchema(schema);
    std::shared_ptr<vmf::Metadata> dima(new vmf::Metadata(desc)), vasya(new vmf::Metadata(desc));
    dima->setFieldValue("name", "Dmitry");
    dima->setFieldValue("last name", "Bogdanov");
    dima->setFieldValue("age", (vmf::vmf_integer) 21);

    vasya->setFieldValue("name", "Vasya");
    vasya->setFieldValue("last name", "Pupkin");
    vasya->setFieldValue("age", (vmf::vmf_integer) 18);

    stream.add(dima);
    stream.add(vasya);
    stream.save();
    stream.close();

    //vmf::terminate();
}
