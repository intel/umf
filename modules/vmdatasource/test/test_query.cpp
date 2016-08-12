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

#define TEST_FILE (tempPath + "global_test.avi")
#else
#define TEST_FILE "global_test.avi"
#endif /* TARGET_OS_IPHONE */

#define TEST_FILE_SRC VIDEO_FILE

class TestQuery : public ::testing::Test
{
protected:

    virtual void SetUp()
    {
        n = 10;

        //umf::initialize();

        TEST_SCHEMA_NAME = "TEST_SCHEMA_NAME";
        TEST_PROPERTY_NAME1 = "TEST_PROPERTY_NAME1";
        TEST_PROPERTY_NAME2 = "TEST_PROPERTY_NAME2";
        TEST_FIELD_NAME = "TEST_FIELD_NAME";
        TEST_FIELD_NAME2 = "TEST_FIELD_NAME2";

        schema = std::shared_ptr<umf::MetadataSchema>(new umf::MetadataSchema(TEST_SCHEMA_NAME));

        VMF_METADATA_BEGIN(TEST_PROPERTY_NAME1)
        VMF_FIELD_INT(TEST_FIELD_NAME)
        VMF_FIELD_INT(TEST_FIELD_NAME2)
        VMF_METADATA_END(schema);
        
        VMF_METADATA_BEGIN(TEST_PROPERTY_NAME2)
        VMF_FIELD_INT(TEST_FIELD_NAME)
        VMF_METADATA_END(schema);

        copyFile(TEST_FILE_SRC, TEST_FILE);

        if (!stream.open(TEST_FILE, umf::MetadataStream::Update))
            std::cout << "Cann't open stream!" << std::endl;

        stream.addSchema(schema);

        for (int i = 0; i < n; i++)
        {
            std::shared_ptr<umf::Metadata> md(new umf::Metadata(schema->findMetadataDesc(TEST_PROPERTY_NAME1)));
            md->setFieldValue(TEST_FIELD_NAME, i);
            md->setFieldValue(TEST_FIELD_NAME2, 2*i);
            stream.add(md);

            std::shared_ptr<umf::Metadata> md2(new umf::Metadata(schema->findMetadataDesc(TEST_PROPERTY_NAME2)));
            md2->setFieldValue(TEST_FIELD_NAME, n + i);
            stream.add(md2);
        }

        stream.save();
        stream.close();
        stream.clear();
    }

    virtual void TearDown()
    {
        //umf::terminate();
    }

    int n;

    umf::MetadataStream stream;
    std::shared_ptr<umf::MetadataSchema> schema;
    std::shared_ptr<umf::MetadataDesc> descr1;
    std::shared_ptr<umf::MetadataDesc> descr2;

    umf::umf_string TEST_SCHEMA_NAME;
    umf::umf_string TEST_PROPERTY_NAME1;
    umf::umf_string TEST_PROPERTY_NAME2;
    umf::umf_string TEST_FIELD_NAME;
    umf::umf_string TEST_FIELD_NAME2;
};

TEST_F(TestQuery, QueryByNameAndValue)
{
    {
        umf::MetadataStream newStream;
        newStream.open(TEST_FILE, umf::MetadataStream::ReadOnly);
        newStream.load(TEST_SCHEMA_NAME);

        for (int i = 0; i < n; i++)
        {
            umf::Variant value(2*i);
            umf::FieldValue fieldValue(TEST_FIELD_NAME2, value);
            auto set = newStream.queryByNameAndValue(TEST_PROPERTY_NAME1, fieldValue);
            ASSERT_EQ(set.size(), 1u);

        }
        newStream.close();
    }
}

TEST_F(TestQuery, Query)
{
    umf::MetadataStream newStream;
    newStream.open(TEST_FILE, umf::MetadataStream::ReadOnly);
    newStream.load(TEST_SCHEMA_NAME);
    
    umf::MetadataSet set = newStream.query([&](const std::shared_ptr< umf::Metadata >& spItem)->bool
    {
        return (spItem->getName() == TEST_PROPERTY_NAME1);
    });

    ASSERT_EQ(set.size(), 10);
    newStream.close();
}
