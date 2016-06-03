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
#define ANOTHER_TEST_FILE (tempPath + "another_global_test.avi")
#else
#define TEST_FILE "global_test.avi"
#define ANOTHER_TEST_FILE "another_global_test.avi"
#endif /* TARGET_OS_IPHONE */

#define NO_FILE "i_hope_this_file_will_never_be_created_by_user.avi"

class TestGlobal : public ::testing::Test
{
protected:
    void SetUp()
    {
        //vmf::initialize();
        copyFile(VIDEO_FILE, TEST_FILE);

        fields.push_back(vmf::FieldDesc("name", vmf::Variant::type_string));
        fields.push_back(vmf::FieldDesc("last name", vmf::Variant::type_string));
        fields.push_back(vmf::FieldDesc("age", vmf::Variant::type_integer));
        desc = std::make_shared<vmf::MetadataDesc>("people", fields);
        schema = std::make_shared<vmf::MetadataSchema>("my schema");
        schema->add(desc);
    }

    void TearDown()
    {
        //vmf::terminate();
    }

    vmf::MetadataStream stream;
    std::shared_ptr<vmf::MetadataSchema> schema;
    std::shared_ptr<vmf::MetadataDesc> desc;
    std::vector<vmf::FieldDesc> fields;
};

TEST_F(TestGlobal, GetTimeStamp)
{
    ASSERT_THROW(vmf::getTimestamp(1965, 1, 1), vmf::IncorrectParamException);
    ASSERT_THROW(vmf::getTimestamp(1989, 13, 1), vmf::IncorrectParamException);
    ASSERT_THROW(vmf::getTimestamp(1989, 1, 32), vmf::IncorrectParamException);
    ASSERT_THROW(vmf::getTimestamp(1989, 1, 14, 26), vmf::IncorrectParamException);
    ASSERT_THROW(vmf::getTimestamp(1989, 1, 14, 9, 65), vmf::IncorrectParamException);
    ASSERT_THROW(vmf::getTimestamp(1989, 1, 14, 9, 30, 65), vmf::IncorrectParamException);
    ASSERT_THROW(vmf::getTimestamp(1989, 1, 14, 9, 30, 40, 1500), vmf::IncorrectParamException);
}

TEST_F(TestGlobal, Open)
{
    ASSERT_FALSE(stream.open(NO_FILE, vmf::MetadataStream::Update));
    ASSERT_TRUE(stream.open(TEST_FILE, vmf::MetadataStream::Update));
}

TEST_F(TestGlobal, Reopen)
{
    stream.open(TEST_FILE, vmf::MetadataStream::Update);
    stream.addSchema(schema);
    auto meta = std::make_shared<vmf::Metadata>(desc);
    meta->setFieldValue("name", "Dmitry");
    meta->setFieldValue("last name", "Bogdanov");
    meta->setFieldValue("age", (vmf::vmf_integer) 21);
    stream.add(meta);
    ASSERT_TRUE(stream.save());
    stream.close();

    ASSERT_TRUE(stream.reopen(vmf::MetadataStream::Update));
    stream.load();
    ASSERT_EQ(1u, stream.getAll().size());
    ASSERT_TRUE(stream.save());
    stream.close();
}

TEST_F(TestGlobal, SaveTo)
{
    copyFile(VIDEO_FILE, ANOTHER_TEST_FILE);

    {   // open ANOTHER_TEST_FILE and add metadata
        auto another_schema = std::make_shared<vmf::MetadataSchema>("schema for another file");
        auto another_desc = std::make_shared<vmf::MetadataDesc>("property for another file", vmf::Variant::type_string);
        another_schema->add(another_desc);
        auto metadata = std::make_shared<vmf::Metadata>(another_desc);
        metadata->addValue("test value for another file");

        stream.open(ANOTHER_TEST_FILE, vmf::MetadataStream::Update);
        stream.addSchema(another_schema);
        stream.add(metadata);
        stream.save();
        stream.close();
        stream.clear();
    }
    {   // check content of ANOTHER_TEST_FILE
        stream.open(ANOTHER_TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load();
        vmf::MetadataSet all = stream.getAll();
        ASSERT_EQ(1u, all.size());
        std::shared_ptr<vmf::Metadata> metadata = all.front();
        ASSERT_EQ("test value for another file", (vmf::vmf_string) metadata->front());
        ASSERT_EQ("schema for another file", metadata->getSchemaName());
        ASSERT_EQ("property for another file", metadata->getName());
        stream.clear();
    }
    {   // open TEST_FILE, add metadata and save to ANOTHER_TEST_FILE
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.close();
        stream.addSchema(schema);
        auto meta = std::make_shared<vmf::Metadata>(desc);
        meta->setFieldValue("name", "Dmitry");
        meta->setFieldValue("last name", "Bogdanov");
        meta->setFieldValue("age", (vmf::vmf_integer) 21);
        stream.add(meta);
        stream.saveTo(ANOTHER_TEST_FILE);
        stream.clear();
    }
    {   // check content of ANOTHER_TEST_FILE
        stream.open(ANOTHER_TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load();
        vmf::MetadataSet all = stream.getAll();
        ASSERT_EQ(1u, all.size());
        std::shared_ptr<vmf::Metadata> me = all.front();
        ASSERT_EQ("Dmitry", (vmf::vmf_string) me->getFieldValue("name"));
        ASSERT_EQ("Bogdanov", (vmf::vmf_string) me->getFieldValue("last name"));
        ASSERT_EQ(21, (vmf::vmf_integer) me->getFieldValue("age"));
    }
}

TEST_F(TestGlobal, AddBeforeOpen)
{
    {
        stream.addSchema(schema);
        auto meta = std::make_shared<vmf::Metadata>(desc);
        meta->setFieldValue("name", "Dmitry");
        meta->setFieldValue("last name", "Bogdanov");
        meta->setFieldValue("age", (vmf::vmf_integer) 21);
        stream.add(meta);
        stream.saveTo(TEST_FILE);
        stream.clear();
    }
    {
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load();
        vmf::MetadataSet all = stream.getAll();
        ASSERT_EQ(1u, all.size());
        std::shared_ptr<vmf::Metadata> metadata = all.front();
        ASSERT_NE(vmf::INVALID_ID, metadata->getId());
        ASSERT_EQ("Dmitry", (vmf::vmf_string) metadata->getFieldValue("name"));
        ASSERT_EQ("Bogdanov", (vmf::vmf_string) metadata->getFieldValue("last name"));
        ASSERT_EQ(21, (vmf::vmf_integer) metadata->getFieldValue("age"));
    }
}

TEST_F(TestGlobal, Clear)
{
    stream.addSchema(schema);
    auto metadata = std::make_shared<vmf::Metadata>(desc);
    metadata->setFieldValue("name", "Dmitry");
    metadata->setFieldValue("last name", "Bogdanov");
    metadata->setFieldValue("age", (vmf::vmf_integer) 21);
    stream.add(metadata);

    ASSERT_EQ(1u, stream.getAll().size());
    stream.clear();
    ASSERT_EQ(0u, stream.getAll().size());
}

/*!
* \test Checks that it's impossible to add reference to metadata that has not
* been associated to stream yet and that user may save the result after it.
*/
TEST_F(TestGlobal, NoDestinationReference)
{
    stream.addSchema(schema);

    auto metadata1 = std::make_shared<vmf::Metadata>(desc);
    metadata1->setFieldValue("name", "Dmitry");
    metadata1->setFieldValue("last name", "Bogdanov");
    metadata1->setFieldValue("age", (vmf::vmf_integer) 21);
    stream.add(metadata1);

    {
        auto metadata2 = std::make_shared<vmf::Metadata>(desc);
        metadata2->setFieldValue("name", "Konstantin");
        metadata2->setFieldValue("last name", "Matskevich");
        metadata2->setFieldValue("age", (vmf::vmf_integer) 22);
        EXPECT_THROW(metadata1->addReference(metadata2), vmf::IncorrectParamException);
    }
    EXPECT_TRUE(stream.saveTo(TEST_FILE));

    vmf::MetadataStream stream2;
    stream2.open(TEST_FILE);
    stream2.load();
    vmf::MetadataSet all = stream2.getAll();
    ASSERT_EQ(1u, all.size());
    auto metadata3 = all.at(0);
    ASSERT_EQ("Dmitry", (vmf::vmf_string) metadata3->getFieldValue("name"));
    ASSERT_EQ("Bogdanov", (vmf::vmf_string) metadata3->getFieldValue("last name"));
    ASSERT_EQ(21, (vmf::vmf_integer) metadata3->getFieldValue("age"));
}
