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
#define SCHEMA_TEST_FILE tempPath + "schema_test_file.avi"
#else
#define SCHEMA_TEST_FILE "schema_test_file.avi"
#endif /* TARGET_OS_IPHONE */

class TestSaveLoadSchema : public ::testing::Test
{
protected:
    void SetUp()
    {
        TEST_SCHEMA_NAME = "TEST_SCHEMA_NAME";
        TEST_DESC_NAME = "TEST_DESC_NAME";
        TEST_FIELD_NAME = "TEST_FIELD_NAME";
        TEST_VALUE_1 = "TEST_VALUE_1";
        TEST_VALUE_2 = "TEST_VALUE_2";

        copyFile(VIDEO_FILE, SCHEMA_TEST_FILE);

        //umf::initialize();

        schema = std::shared_ptr<umf::MetadataSchema>(new umf::MetadataSchema(TEST_SCHEMA_NAME));
        desc = std::shared_ptr<umf::MetadataDesc>(new umf::MetadataDesc(TEST_DESC_NAME, umf::Variant::type_string));
        schema->add(desc);
    }

    void TearDown()
    {
        //umf::terminate();
    }

    std::shared_ptr<umf::MetadataSchema> schema;
    std::shared_ptr<umf::MetadataDesc> desc;

    umf::umf_string TEST_SCHEMA_NAME;
    umf::umf_string TEST_DESC_NAME;
    umf::umf_string TEST_FIELD_NAME;
    umf::umf_string TEST_VALUE_1;
    umf::umf_string TEST_VALUE_2;
};

TEST_F(TestSaveLoadSchema, OneField)
{
    {
        std::vector<umf::FieldDesc> fields;
        fields.push_back(umf::FieldDesc(TEST_FIELD_NAME,umf::Variant::type_string));
        desc = std::shared_ptr<umf::MetadataDesc> (new umf::MetadataDesc(TEST_DESC_NAME, fields));
        schema = std::shared_ptr<umf::MetadataSchema>(new umf::MetadataSchema(TEST_SCHEMA_NAME));
        schema->add(desc);

        umf::MetadataStream stream;
        stream.addSchema(schema);
        stream.saveTo(SCHEMA_TEST_FILE);
    }

    {
        umf::MetadataStream stream;
        stream.open(SCHEMA_TEST_FILE, umf::MetadataStream::ReadOnly);
        auto schema = stream.getSchema(TEST_SCHEMA_NAME);

        ASSERT_EQ(TEST_SCHEMA_NAME, schema->getName());
        ASSERT_EQ(1u, schema->size());
        std::shared_ptr<umf::MetadataDesc> desc = schema->findMetadataDesc(TEST_DESC_NAME);
        ASSERT_TRUE(desc != NULL);
        ASSERT_EQ(TEST_SCHEMA_NAME, desc->getSchemaName());
        ASSERT_EQ(TEST_DESC_NAME, desc->getMetadataName());
        auto fields = desc->getFields();
        ASSERT_EQ(1u, fields.size());
        umf::FieldDesc fieldDescr;
        ASSERT_TRUE(desc->getFieldDesc(fieldDescr, TEST_FIELD_NAME));
        ASSERT_EQ(umf::Variant::type_string, fieldDescr.type);
        ASSERT_EQ(TEST_FIELD_NAME, fieldDescr.name);
    }
}

TEST_F(TestSaveLoadSchema, Array)
{
    {
        umf::MetadataStream stream;
        stream.addSchema(schema);
        stream.saveTo(SCHEMA_TEST_FILE);
    }

    {
        umf::MetadataStream stream;
        stream.open(SCHEMA_TEST_FILE, umf::MetadataStream::ReadOnly);

        auto schema = stream.getSchema(TEST_SCHEMA_NAME);

        ASSERT_EQ(1u, schema->size());
        std::shared_ptr<umf::MetadataDesc> desc = schema->findMetadataDesc(TEST_DESC_NAME);
        ASSERT_TRUE(desc.get() != NULL);
        ASSERT_EQ(TEST_SCHEMA_NAME, desc->getSchemaName());
        ASSERT_EQ(TEST_DESC_NAME, desc->getMetadataName());
        std::vector<umf::FieldDesc> fields = desc->getFields();
        ASSERT_EQ(1u, fields.size());
        umf::FieldDesc field = fields.at(0);
        ASSERT_EQ(umf::Variant::type_string, field.type);
        ASSERT_EQ("", field.name);
    }
}

TEST_F(TestSaveLoadSchema, GetAllNames)
{
    {
        umf::MetadataStream stream;
        stream.addSchema(schema);
        stream.saveTo(SCHEMA_TEST_FILE);
    }

    {
        umf::MetadataStream stream;
        stream.open(SCHEMA_TEST_FILE, umf::MetadataStream::ReadOnly);

        auto schemaNames = stream.getAllSchemaNames();
        ASSERT_EQ(1u, schemaNames.size());
        ASSERT_EQ(TEST_SCHEMA_NAME, schemaNames[0]);
    }
}

TEST_F(TestSaveLoadSchema, SchemaAuthor)
{
    const umf::umf_string TEST_SCHEMA_WITH_AUTHOR_NAME = "Schema with author";
    const umf::umf_string TEST_AUTHOR_NAME = "< the \"Author\" >";

    {
        umf::MetadataStream stream;

        auto schemaWithAuthor = std::shared_ptr<umf::MetadataSchema>(new umf::MetadataSchema(TEST_SCHEMA_WITH_AUTHOR_NAME, TEST_AUTHOR_NAME));
        auto desc1 = std::shared_ptr<umf::MetadataDesc>(new umf::MetadataDesc(TEST_DESC_NAME, umf::Variant::type_string));
        schemaWithAuthor->add(desc1);

        stream.addSchema(schema);
        stream.addSchema(schemaWithAuthor);
        stream.saveTo(SCHEMA_TEST_FILE);
	stream.close();
    }

    {
        umf::MetadataStream stream;
        stream.open(SCHEMA_TEST_FILE, umf::MetadataStream::ReadOnly);

        auto schemaNames = stream.getAllSchemaNames();
        ASSERT_EQ(2u, schemaNames.size());
        for (auto schema = schemaNames.begin(); schema != schemaNames.end(); schema++)
        {
            auto desc = stream.getSchema(*schema);
            auto author = desc->getAuthor();
            if(*schema == TEST_SCHEMA_WITH_AUTHOR_NAME) ASSERT_EQ(author, TEST_AUTHOR_NAME);
            else ASSERT_EQ(author, "");
        }
    }
}
