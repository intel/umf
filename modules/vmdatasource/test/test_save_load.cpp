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
#include <memory>

#if TARGET_OS_IPHONE
extern std::string tempPath;

#define TEST_FILE (tempPath + "global_test.avi")
#define TEST_FILE2 (tempPath + "global_test2.avi")
#else
#define TEST_FILE "global_test.avi"
#define TEST_FILE2 "global_test2.avi"
#endif /* TARGET_OS_IPHONE */

#define TEST_FILE_SRC VIDEO_FILE

TEST(TestSaveLoadMetadataTyped, String)
{
    const vmf::vmf_string atom(TEST_SCHEMA_NAME);
    const vmf::vmf_string atom(TEST_PROPERTY_NAME);
    const vmf::vmf_string atom(TEST_VALUE);
    const vmf::vmf_string atom(TEST_FIELD_NAME);

    //vmf::initialize();

    {
        std::shared_ptr<vmf::MetadataSchema> schema(new vmf::MetadataSchema(TEST_SCHEMA_NAME));
        std::vector<vmf::FieldDesc> fields;
        fields.push_back(vmf::FieldDesc(TEST_FIELD_NAME,vmf::Variant::type_string));
        std::shared_ptr<vmf::MetadataDesc> descr(new vmf::MetadataDesc(TEST_PROPERTY_NAME, fields));
        schema->add(descr);

        copyFile(TEST_FILE_SRC, TEST_FILE);
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.addSchema(schema);

        std::shared_ptr<vmf::Metadata> md(new vmf::Metadata(descr));
        md->setFieldValue(TEST_FIELD_NAME, TEST_VALUE);
        stream.add(md);
        stream.save();
        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load(TEST_SCHEMA_NAME);
        stream.close();

        vmf::MetadataSet schema = stream.queryBySchema(TEST_SCHEMA_NAME);
        ASSERT_EQ(1u, schema.size());
        vmf::MetadataSet property = schema.queryByName(TEST_PROPERTY_NAME);
        ASSERT_EQ(1u, property.size());
        std::shared_ptr<vmf::Metadata> md = property.at(0);
        ASSERT_EQ(1u, md->getFieldNames().size());
        vmf::Variant value = md->getFieldValue(TEST_FIELD_NAME);
        ASSERT_EQ(vmf::Variant::type_string, value.getType());
        ASSERT_EQ(TEST_VALUE, (vmf::vmf_string) value);
    }

    //vmf::terminate();
}

TEST(TestSaveLoadMetadataTyped, TwoStrings)
{
    const vmf::vmf_string atom(TEST_SCHEMA_NAME);
    const vmf::vmf_string atom(TEST_PROPERTY_NAME);
    const vmf::vmf_string atom(TEST_VALUE);
    const vmf::vmf_string atom(TEST_FIELD_NAME);

    //vmf::initialize();

    {
        std::shared_ptr<vmf::MetadataSchema> schema(new vmf::MetadataSchema(TEST_SCHEMA_NAME));
        std::vector<vmf::FieldDesc> fields;
        fields.push_back(vmf::FieldDesc(TEST_FIELD_NAME,vmf::Variant::type_string));
        std::shared_ptr<vmf::MetadataDesc> descr(new vmf::MetadataDesc(TEST_PROPERTY_NAME, fields));
        schema->add(descr);

        copyFile(TEST_FILE_SRC, TEST_FILE);
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);

        stream.addSchema(schema);

        std::shared_ptr<vmf::Metadata> md(new vmf::Metadata(descr));
        md->setFieldValue(TEST_FIELD_NAME, TEST_VALUE);
        stream.add(md);
        stream.save();
        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load(TEST_SCHEMA_NAME);
        stream.close();

        vmf::MetadataSet schema = stream.queryBySchema(TEST_SCHEMA_NAME);
        ASSERT_EQ(1u, schema.size());
        vmf::MetadataSet property = schema.queryByName(TEST_PROPERTY_NAME);
        ASSERT_EQ(1u, property.size());
        std::shared_ptr<vmf::Metadata> md = property.at(0);
        ASSERT_EQ(1u, md->getFieldNames().size());
        vmf::Variant value = md->getFieldValue(TEST_FIELD_NAME);
        ASSERT_EQ(vmf::Variant::type_string, value.getType());
        ASSERT_EQ(TEST_VALUE, (vmf::vmf_string) value);
    }

    //vmf::terminate();
}

#define DECLARE_INT_TEST(subname, Type, testValue)\
    TEST(TestSaveLoadMetadataTyped, subname)\
{\
    const vmf::vmf_string atom(TEST_SCHEMA_NAME);\
    const vmf::vmf_string atom(TEST_PROPERTY_NAME);\
    const vmf::vmf_string atom(TEST_FIELD_NAME);\
    const vmf::vmf_##Type TEST_VALUE(testValue);\
    \
    std::shared_ptr<vmf::MetadataSchema> schema(new vmf::MetadataSchema(TEST_SCHEMA_NAME));\
    std::vector<vmf::FieldDesc> fields;\
    fields.push_back(vmf::FieldDesc(TEST_FIELD_NAME,vmf::Variant::type_##Type));\
    std::shared_ptr<vmf::MetadataDesc> descr(new vmf::MetadataDesc(TEST_PROPERTY_NAME, fields));\
    schema->add(descr);\
{\
    copyFile(TEST_FILE_SRC, TEST_FILE);\
    vmf::MetadataStream stream;\
    stream.open(TEST_FILE, vmf::MetadataStream::Update);\
    stream.addSchema(schema);\
    std::shared_ptr<vmf::Metadata> md(new vmf::Metadata(descr));\
    md->setFieldValue(TEST_FIELD_NAME, TEST_VALUE);\
    stream.add(md);\
    stream.save();\
    stream.close();\
}\
{\
    vmf::MetadataStream stream;\
    stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);\
    stream.load(TEST_SCHEMA_NAME);\
    stream.close();\
    vmf::MetadataSet schema = stream.queryBySchema(TEST_SCHEMA_NAME);\
    ASSERT_EQ(1u, schema.size());\
    vmf::MetadataSet property = schema.queryByName(TEST_PROPERTY_NAME);\
    ASSERT_EQ(1u, property.size());\
    std::shared_ptr<vmf::Metadata> md = property.at(0);\
    ASSERT_EQ(1u, md->getFieldNames().size());\
    vmf::Variant value = md->getFieldValue(TEST_FIELD_NAME);\
    ASSERT_EQ(vmf::Variant::type_##Type, value.getType());\
    ASSERT_EQ(TEST_VALUE, (vmf::vmf_##Type) value);\
}\
\
}

DECLARE_INT_TEST(Max_integer, integer, (vmf::vmf_integer) 0x7FFFFFFFFFFFFFFF)
DECLARE_INT_TEST(Min_integer, integer, (vmf::vmf_integer) 0xFFFFFFFFFFFFFFFF)


TEST(TestSaveLoadMetadataTyped, DoublePos)
{
    const vmf::vmf_string atom(TEST_SCHEMA_NAME);
    const vmf::vmf_string atom(TEST_PROPERTY_NAME);
    const vmf::vmf_real TEST_VALUE(100.500);
    const vmf::vmf_string atom(TEST_FIELD_NAME);

    //vmf::initialize();

    std::shared_ptr<vmf::MetadataSchema> schema(new vmf::MetadataSchema(TEST_SCHEMA_NAME));
    std::vector<vmf::FieldDesc> fields;
    fields.push_back(vmf::FieldDesc(TEST_FIELD_NAME,vmf::Variant::type_real));
    std::shared_ptr<vmf::MetadataDesc> descr(new vmf::MetadataDesc(TEST_PROPERTY_NAME, fields));
    schema->add(descr);

    {
        copyFile(TEST_FILE_SRC, TEST_FILE);
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.addSchema(schema);

        std::shared_ptr<vmf::Metadata> md(new vmf::Metadata(descr));
        md->setFieldValue(TEST_FIELD_NAME, TEST_VALUE);
        stream.add(md);

        stream.save();
        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load(TEST_SCHEMA_NAME);
        stream.close();

        vmf::MetadataSet schema = stream.queryBySchema(TEST_SCHEMA_NAME);
        ASSERT_EQ(1u, schema.size());
        vmf::MetadataSet property = schema.queryByName(TEST_PROPERTY_NAME);
        ASSERT_EQ(1u, property.size());
        std::shared_ptr<vmf::Metadata> md = property.at(0);
        ASSERT_EQ(1u, md->getFieldNames().size());
        vmf::Variant value = md->getFieldValue(TEST_FIELD_NAME);
        ASSERT_EQ(vmf::Variant::type_real, value.getType());
        ASSERT_DOUBLE_EQ(TEST_VALUE, (vmf::vmf_real) value);
    }

    //vmf::terminate();
}

TEST(TestSaveLoadMetadataTyped, DoubleNeg)
{
    const vmf::vmf_string atom(TEST_SCHEMA_NAME);
    const vmf::vmf_string atom(TEST_PROPERTY_NAME);
    const vmf::vmf_real TEST_VALUE(-100.500);
    const vmf::vmf_string atom(TEST_FIELD_NAME);

    //vmf::initialize();

    std::shared_ptr<vmf::MetadataSchema> schema(new vmf::MetadataSchema(TEST_SCHEMA_NAME));
    std::vector<vmf::FieldDesc> fields;
    fields.push_back(vmf::FieldDesc(TEST_FIELD_NAME,vmf::Variant::type_real));
    std::shared_ptr<vmf::MetadataDesc> descr(new vmf::MetadataDesc(TEST_PROPERTY_NAME, fields));
    schema->add(descr);

    {
        copyFile(TEST_FILE_SRC, TEST_FILE);
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.addSchema(schema);

        std::shared_ptr<vmf::Metadata> md(new vmf::Metadata(descr));
        md->setFieldValue(TEST_FIELD_NAME, TEST_VALUE);
        stream.add(md);

        stream.save();
        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load(TEST_SCHEMA_NAME);
        stream.close();

        vmf::MetadataSet schema = stream.queryBySchema(TEST_SCHEMA_NAME);
        ASSERT_EQ(1u, schema.size());
        vmf::MetadataSet property = schema.queryByName(TEST_PROPERTY_NAME);
        ASSERT_EQ(1u, property.size());
        std::shared_ptr<vmf::Metadata> md = property.at(0);
        ASSERT_EQ(1u, md->getFieldNames().size());
        vmf::Variant value = md->getFieldValue(TEST_FIELD_NAME);
        ASSERT_EQ(vmf::Variant::type_real, value.getType());
        ASSERT_DOUBLE_EQ(TEST_VALUE, (vmf::vmf_real) value);
    }

    //vmf::terminate();
}

TEST(TestSaveLoadMetadataTyped, Rawbuffer)
{
    const vmf::vmf_string atom(TEST_SCHEMA_NAME);
    const vmf::vmf_string atom(TEST_PROPERTY_NAME);
    const vmf::vmf_string atom(TEST_FIELD_NAME);

    const vmf::vmf_rawbuffer TEST_VALUE("Raw <buffer \0 content>\n&", 25);

    //vmf::initialize();

    std::shared_ptr<vmf::MetadataSchema> schema(new vmf::MetadataSchema(TEST_SCHEMA_NAME));
    std::vector<vmf::FieldDesc> fields;
    fields.push_back(vmf::FieldDesc(TEST_FIELD_NAME,vmf::Variant::type_rawbuffer));
    std::shared_ptr<vmf::MetadataDesc> descr(new vmf::MetadataDesc(TEST_PROPERTY_NAME, fields));
    schema->add(descr);

    {
        copyFile(TEST_FILE_SRC, TEST_FILE);
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.addSchema(schema);

        std::shared_ptr<vmf::Metadata> md(new vmf::Metadata(descr));
        md->setFieldValue(TEST_FIELD_NAME, TEST_VALUE);
        stream.add(md);

        stream.save();
        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load(TEST_SCHEMA_NAME);
        stream.close();

        vmf::MetadataSet schema = stream.queryBySchema(TEST_SCHEMA_NAME);
        ASSERT_EQ(1u, schema.size());
        vmf::MetadataSet property = schema.queryByName(TEST_PROPERTY_NAME);
        ASSERT_EQ(1u, property.size());
        std::shared_ptr<vmf::Metadata> md = property.at(0);
        ASSERT_EQ(1u, md->getFieldNames().size());
        vmf::Variant value = md->getFieldValue(TEST_FIELD_NAME);
        ASSERT_EQ(vmf::Variant::type_rawbuffer, value.getType());
        bool result = TEST_VALUE == value.get_rawbuffer();
        ASSERT_TRUE(result);
    }

    //vmf::terminate();
}

TEST(TestSaveLoadMetadataTyped, Vectors)
{
    std::vector<vmf::vmf_integer> test_integer_value;
    test_integer_value.push_back(42);
    test_integer_value.push_back(24);
    test_integer_value.push_back(1024);
    test_integer_value.push_back(-1024);

    std::vector<vmf::vmf_real> test_real_value;
    test_real_value.push_back(42.42);
    test_real_value.push_back(24.24);
    test_real_value.push_back(1024.1024);
    test_real_value.push_back(-1024.6725);

    std::vector<vmf::vmf_string> test_string_value;
    test_string_value.push_back("test");
    test_string_value.push_back("string");
    test_string_value.push_back("value");

    std::vector<vmf::vmf_vec2d> test_vec2d_value;
    test_vec2d_value.push_back(vmf::vmf_vec2d(42, 42));
    test_vec2d_value.push_back(vmf::vmf_vec2d(33.33, 57.75));
    test_vec2d_value.push_back(vmf::vmf_vec2d(-72, 15.67));

    std::vector<vmf::vmf_vec3d> test_vec3d_value;
    test_vec3d_value.push_back(vmf::vmf_vec3d(42, 42, 42));
    test_vec3d_value.push_back(vmf::vmf_vec3d(33.33, 57.75, 99.12));
    test_vec3d_value.push_back(vmf::vmf_vec3d(-72, 15.67, 0));

    std::vector<vmf::vmf_vec4d> test_vec4d_value;
    test_vec4d_value.push_back(vmf::vmf_vec4d(42, 42, 42, 42));
    test_vec4d_value.push_back(vmf::vmf_vec4d(33.33, 57.75, 99.12, 45.14));
    test_vec4d_value.push_back(vmf::vmf_vec4d(-72, 15.67, 0, 16.79));

    //vmf::initialize();

    std::shared_ptr<vmf::MetadataSchema> schema(new vmf::MetadataSchema("test_schema"));
    std::vector<vmf::FieldDesc> fields;
    fields.push_back(vmf::FieldDesc("test_integer", vmf::Variant::type_integer_vector));
    fields.push_back(vmf::FieldDesc("test_real", vmf::Variant::type_real_vector));
    fields.push_back(vmf::FieldDesc("test_string", vmf::Variant::type_string_vector));
    fields.push_back(vmf::FieldDesc("test_vec2d", vmf::Variant::type_vec2d_vector));
    fields.push_back(vmf::FieldDesc("test_vec3d", vmf::Variant::type_vec3d_vector));
    fields.push_back(vmf::FieldDesc("test_vec4d", vmf::Variant::type_vec4d_vector));
    std::shared_ptr<vmf::MetadataDesc> descr(new vmf::MetadataDesc("test_desc", fields));
    schema->add(descr);

    {
        copyFile(TEST_FILE_SRC, TEST_FILE);
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.addSchema(schema);

        std::shared_ptr<vmf::Metadata> md(new vmf::Metadata(descr));
        md->setFieldValue("test_integer", test_integer_value);
        md->setFieldValue("test_real", test_real_value);
        md->setFieldValue("test_string", test_string_value);
        md->setFieldValue("test_vec2d", test_vec2d_value);
        md->setFieldValue("test_vec3d", test_vec3d_value);
        md->setFieldValue("test_vec4d", test_vec4d_value);
        stream.add(md);

        stream.save();
        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load("test_schema");
        stream.close();

        vmf::MetadataSet schema = stream.queryBySchema("test_schema");
        ASSERT_EQ(1u, schema.size());
        vmf::MetadataSet property = schema.queryByName("test_desc");
        ASSERT_EQ(1u, property.size());
        std::shared_ptr<vmf::Metadata> md = property.at(0);
        ASSERT_EQ(6u, md->getFieldNames().size());

        vmf::Variant value;
        bool result;

        value = md->getFieldValue("test_integer");
        ASSERT_EQ(vmf::Variant::type_integer_vector, value.getType());
        result = std::equal(test_integer_value.begin(), test_integer_value.end(), value.get_integer_vector().begin());
        ASSERT_TRUE(result);

        value = md->getFieldValue("test_real");
        ASSERT_EQ(vmf::Variant::type_real_vector, value.getType());
        auto testRealVec = value.get_real_vector();
        result = std::equal(test_real_value.begin(), test_real_value.end(), value.get_real_vector().begin());
        ASSERT_TRUE(result);

        value = md->getFieldValue("test_string");
        ASSERT_EQ(vmf::Variant::type_string_vector, value.getType());
        result = std::equal(test_string_value.begin(), test_string_value.end(), value.get_string_vector().begin());
        ASSERT_TRUE(result);

        value = md->getFieldValue("test_vec2d");
        ASSERT_EQ(vmf::Variant::type_vec2d_vector, value.getType());
        result = std::equal(test_vec2d_value.begin(), test_vec2d_value.end(), value.get_vec2d_vector().begin());
        ASSERT_TRUE(result);

        value = md->getFieldValue("test_vec3d");
        ASSERT_EQ(vmf::Variant::type_vec3d_vector, value.getType());
        result = std::equal(test_vec3d_value.begin(), test_vec3d_value.end(), value.get_vec3d_vector().begin());
        ASSERT_TRUE(result);

        value = md->getFieldValue("test_vec4d");
        ASSERT_EQ(vmf::Variant::type_vec4d_vector, value.getType());
        result = std::equal(test_vec4d_value.begin(), test_vec4d_value.end(), value.get_vec4d_vector().begin());
        ASSERT_TRUE(result);
    }

    //vmf::terminate();
}

class TestSaveLoadMetadataTypeVec : public ::testing::Test
{
protected:
    void SetUp()
    {
        TEST_SCHEMA_NAME = "TEST_SCHEMA_NAME";
        TEST_PROPERTY_NAME = "TEST_PROPERTY_NAME";
        TEST_FIELD_NAME_2D = "TEST_FIELD_NAME_2D";
        TEST_FIELD_NAME_3D = "TEST_FIELD_NAME_3D";
        TEST_FIELD_NAME_4D = "TEST_FIELD_NAME_4D";
        TEST_VALUE = vmf::vmf_vec4d(42.42, 24.24, 42.24, 24.42);

        //vmf::initialize();

        std::shared_ptr<vmf::MetadataSchema> schema(new vmf::MetadataSchema(TEST_SCHEMA_NAME));
        std::vector<vmf::FieldDesc> fields;
        fields.push_back(vmf::FieldDesc(TEST_FIELD_NAME_2D, vmf::Variant::type_vec2d));
        fields.push_back(vmf::FieldDesc(TEST_FIELD_NAME_3D, vmf::Variant::type_vec3d));
        fields.push_back(vmf::FieldDesc(TEST_FIELD_NAME_4D, vmf::Variant::type_vec4d));
        std::shared_ptr<vmf::MetadataDesc> descr(new vmf::MetadataDesc(TEST_PROPERTY_NAME, fields));
        schema->add(descr);

        {
            copyFile(TEST_FILE_SRC, TEST_FILE);

            vmf::MetadataStream stream;
            stream.open(TEST_FILE, vmf::MetadataStream::Update);
            stream.addSchema(schema);

            std::shared_ptr<vmf::Metadata> md(new vmf::Metadata(descr));
            md->setFieldValue(TEST_FIELD_NAME_2D, (vmf::vmf_vec2d)TEST_VALUE);
            md->setFieldValue(TEST_FIELD_NAME_3D, (vmf::vmf_vec3d)TEST_VALUE);
            md->setFieldValue(TEST_FIELD_NAME_4D, TEST_VALUE);
            stream.add(md);

            stream.save();
            stream.close();
        }
    }

    void TearDown()
    {
        //vmf::terminate();
    }

    vmf::vmf_string TEST_SCHEMA_NAME;
    vmf::vmf_string TEST_PROPERTY_NAME;
    vmf::vmf_string TEST_FIELD_NAME_2D;
    vmf::vmf_string TEST_FIELD_NAME_3D;
    vmf::vmf_string TEST_FIELD_NAME_4D;
    vmf::vmf_vec4d TEST_VALUE;

};

TEST_F(TestSaveLoadMetadataTypeVec, Vec2D)
{
    vmf::MetadataStream stream;
    stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
    stream.load(TEST_SCHEMA_NAME);
    stream.close();

    vmf::MetadataSet schema = stream.queryBySchema(TEST_SCHEMA_NAME);
    ASSERT_EQ(1u, schema.size());
    vmf::MetadataSet property = schema.queryByName(TEST_PROPERTY_NAME);
    ASSERT_EQ(1u, property.size());
    std::shared_ptr<vmf::Metadata> md = property.at(0);
    ASSERT_EQ(3u, md->getFieldNames().size());
    vmf::Variant value = md->getFieldValue(TEST_FIELD_NAME_2D);
    ASSERT_EQ(vmf::Variant::type_vec2d, value.getType());
    bool result = (vmf::vmf_vec2d)TEST_VALUE == (vmf::vmf_vec2d)value;
    ASSERT_TRUE(result);
}

TEST_F(TestSaveLoadMetadataTypeVec, Vec3D)
{
    vmf::MetadataStream stream;
    stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
    stream.load(TEST_SCHEMA_NAME);
    stream.close();

    vmf::MetadataSet schema = stream.queryBySchema(TEST_SCHEMA_NAME);
    ASSERT_EQ(1u, schema.size());
    vmf::MetadataSet property = schema.queryByName(TEST_PROPERTY_NAME);
    ASSERT_EQ(1u, property.size());
    std::shared_ptr<vmf::Metadata> md = property.at(0);
    ASSERT_EQ(3u, md->getFieldNames().size());
    vmf::Variant value = md->getFieldValue(TEST_FIELD_NAME_3D);
    ASSERT_EQ(vmf::Variant::type_vec3d, value.getType());
    bool result = (vmf::vmf_vec3d)TEST_VALUE == (vmf::vmf_vec3d)value;
    ASSERT_TRUE(result);
}

TEST_F(TestSaveLoadMetadataTypeVec, Vec4D)
{
    vmf::MetadataStream stream;
    stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
    stream.load(TEST_SCHEMA_NAME);
    stream.close();

    vmf::MetadataSet schema = stream.queryBySchema(TEST_SCHEMA_NAME);
    ASSERT_EQ(1u, schema.size());
    vmf::MetadataSet property = schema.queryByName(TEST_PROPERTY_NAME);
    ASSERT_EQ(1u, property.size());
    std::shared_ptr<vmf::Metadata> md = property.at(0);
    ASSERT_EQ(3u, md->getFieldNames().size());
    vmf::Variant value = md->getFieldValue(TEST_FIELD_NAME_4D);
    ASSERT_EQ(vmf::Variant::type_vec4d, value.getType());
    bool result = TEST_VALUE == (vmf::vmf_vec4d)value;
    ASSERT_TRUE(result);
}

class TestSaveLoadMetadata : public ::testing::Test
{
protected:
    void SetUp()
    {
        TEST_SCHEMA_NAME = "TEST_SCHEMA_NAME";
        TEST_DESC_NAME = "TEST_DESC_NAME";
        TEST_VALUE_1 = "TEST_VALUE_1";
        TEST_VALUE_2 = "TEST_VALUE_2";

        copyFile(TEST_FILE_SRC, TEST_FILE);

        //vmf::initialize();

        schema = std::shared_ptr<vmf::MetadataSchema>(new vmf::MetadataSchema(TEST_SCHEMA_NAME));
        desc = std::shared_ptr<vmf::MetadataDesc>(new vmf::MetadataDesc(TEST_DESC_NAME, vmf::Variant::type_string));
        schema->add(desc);
    }

    void TearDown()
    {
        //vmf::terminate();
    }

    std::shared_ptr<vmf::MetadataSchema> schema;
    std::shared_ptr<vmf::MetadataDesc> desc;

    vmf::vmf_string TEST_SCHEMA_NAME;
    vmf::vmf_string TEST_DESC_NAME;
    vmf::vmf_string TEST_VALUE_1;
    vmf::vmf_string TEST_VALUE_2;
};

TEST_F(TestSaveLoadMetadata, AddMetadataBeforeAddingSchema)
{
    vmf::MetadataStream stream;
    stream.open(TEST_FILE, vmf::MetadataStream::Update);

    std::shared_ptr<vmf::Metadata> md(new vmf::Metadata(desc));
    md->addValue(vmf::Variant(TEST_VALUE_1));
    ASSERT_THROW(stream.add(md), vmf::NotFoundException);

    stream.addSchema(schema);
    ASSERT_NO_THROW(stream.add(md));

    stream.close();
}

TEST_F(TestSaveLoadMetadata, SimpleProperty)
{
    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.addSchema(schema);
        std::shared_ptr<vmf::Metadata> md(new vmf::Metadata(desc));
        md->addValue(vmf::Variant(TEST_VALUE_1));
        stream.add(md);
        stream.save();
        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load(TEST_SCHEMA_NAME);
        stream.close();

        vmf::MetadataSet schemaSet = stream.queryBySchema(TEST_SCHEMA_NAME);
        ASSERT_EQ(1u, schemaSet.size());
        vmf::MetadataSet descSet = schemaSet.queryByName(TEST_DESC_NAME);
        ASSERT_EQ(1u, descSet.size());
        std::shared_ptr<vmf::Metadata> md = descSet.at(0);
        ASSERT_EQ(0u, md->getFieldNames().size());
        vmf::Variant value = md->at(0);
        ASSERT_EQ(vmf::Variant::type_string, value.getType());
        ASSERT_EQ(TEST_VALUE_1, (vmf::vmf_string) value);
    }
}

TEST_F(TestSaveLoadMetadata, Array)
{
    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.addSchema(schema);

        std::shared_ptr<vmf::Metadata> md(new vmf::Metadata(desc));
        md->addValue(vmf::Variant(TEST_VALUE_1));
        md->addValue(vmf::Variant(TEST_VALUE_2));
        stream.add(md);

        stream.save();
        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load(TEST_SCHEMA_NAME);
        stream.close();

        vmf::MetadataSet schemaSet = stream.queryBySchema(TEST_SCHEMA_NAME);
        ASSERT_EQ(1u, schemaSet.size());
        vmf::MetadataSet descSet = schemaSet.queryByName(TEST_DESC_NAME);
        ASSERT_EQ(1u, descSet.size());
        std::shared_ptr<vmf::Metadata> md = descSet.at(0);
        ASSERT_EQ(0u, md->getFieldNames().size());
        vmf::Variant value1 = md->at(0);
        ASSERT_EQ(vmf::Variant::type_string, value1.getType());
        ASSERT_EQ(TEST_VALUE_1, (vmf::vmf_string) value1);
        vmf::Variant value2 = md->at(1);
        ASSERT_EQ(vmf::Variant::type_string, value2.getType());
        ASSERT_EQ(TEST_VALUE_2, (vmf::vmf_string) value2);
    }
}

TEST_F(TestSaveLoadMetadata, DeleteItem)
{
    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.addSchema(schema);
        std::shared_ptr<vmf::Metadata> md(new vmf::Metadata(desc));
        md->addValue(vmf::Variant(TEST_VALUE_1));
        md->addValue(vmf::Variant(TEST_VALUE_2));
        stream.add(md);
        stream.save();
        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.load(TEST_SCHEMA_NAME);
        vmf::MetadataSet schemaSet = stream.queryBySchema(TEST_SCHEMA_NAME);
        vmf::MetadataSet descSet = schemaSet.queryByName(TEST_DESC_NAME);
        std::shared_ptr<vmf::Metadata> md = descSet.at(0);
        ASSERT_EQ(2u, md->size());
        md->pop_back();
        stream.save();
        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load(TEST_SCHEMA_NAME);
        stream.close();
        vmf::MetadataSet schemaSet = stream.queryBySchema(TEST_SCHEMA_NAME);
        ASSERT_EQ(1u, schemaSet.size());
        vmf::MetadataSet descSet = schemaSet.queryByName(TEST_DESC_NAME);
        ASSERT_EQ(1u, descSet.size());
        std::shared_ptr<vmf::Metadata> md = descSet.at(0);
        ASSERT_EQ(1u, md->size());
        ASSERT_EQ(0u, md->getFieldNames().size());
        vmf::Variant value1 = md->at(0);
        ASSERT_EQ(vmf::Variant::type_string, value1.getType());
        ASSERT_EQ(TEST_VALUE_1, (vmf::vmf_string) value1);
    }
}

TEST_F(TestSaveLoadMetadata, DeleteProperty)
{
    const vmf::vmf_string atom(ANOTHER_TEST_DESC_NAME);
    {
        std::shared_ptr<vmf::MetadataDesc> anotherDesc(new vmf::MetadataDesc(ANOTHER_TEST_DESC_NAME, vmf::Variant::type_string));
        schema->add(anotherDesc);

        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.addSchema(schema);

        std::shared_ptr<vmf::Metadata> md(new vmf::Metadata(desc));
        md->addValue(vmf::Variant(TEST_VALUE_1));
        md->addValue(vmf::Variant(TEST_VALUE_2));
        std::shared_ptr<vmf::Metadata> anotherMd(new vmf::Metadata(anotherDesc));
        anotherMd->addValue(vmf::Variant(TEST_VALUE_2));
        stream.add(md);
        stream.add(anotherMd);
        ASSERT_TRUE(stream.save());
        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.load(TEST_SCHEMA_NAME);
        vmf::MetadataSet schemaSet = stream.queryBySchema(TEST_SCHEMA_NAME);
        ASSERT_EQ(2u, schemaSet.size());
        vmf::MetadataSet descSet = schemaSet.queryByName(TEST_DESC_NAME);
        ASSERT_EQ(1u, descSet.size());
        stream.remove(descSet);
        ASSERT_TRUE(stream.save());
        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load(TEST_SCHEMA_NAME);
        stream.close();

        vmf::MetadataSet schemaSet = stream.queryBySchema(TEST_SCHEMA_NAME);
        ASSERT_EQ(1u, schemaSet.size());
        vmf::MetadataSet descSet = schemaSet.queryByName(TEST_DESC_NAME);
        ASSERT_EQ(0u, descSet.size());
    }
}

TEST_F(TestSaveLoadMetadata, FrameIndex)
{
    const long long TEST_FRAME_INDEX = 101;
    const long long TEST_NUMBER_OF_FRAMES = 265;

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.addSchema(schema);

        std::shared_ptr<vmf::Metadata> md(new vmf::Metadata(desc));
        md->setFrameIndex(TEST_FRAME_INDEX, TEST_NUMBER_OF_FRAMES);
        md->addValue("value");
        ASSERT_EQ(TEST_FRAME_INDEX, md->getFrameIndex());
        ASSERT_EQ(TEST_NUMBER_OF_FRAMES, md->getNumOfFrames());

        std::shared_ptr<vmf::Metadata> md2(new vmf::Metadata(desc));
        md2->setFrameIndex(TEST_FRAME_INDEX);
        md2->addValue("value");
        ASSERT_EQ(TEST_FRAME_INDEX, md2->getFrameIndex());
        ASSERT_EQ(1, md2->getNumOfFrames());

        std::shared_ptr<vmf::Metadata> md3(new vmf::Metadata(desc));
        md3->addValue("value");
        ASSERT_THROW(md3->setFrameIndex(TEST_FRAME_INDEX, -10), vmf::IncorrectParamException);

        std::shared_ptr<vmf::Metadata> md4(new vmf::Metadata(desc));
        md4->addValue("value");
        ASSERT_THROW( md4->setFrameIndex(-10, TEST_NUMBER_OF_FRAMES), vmf::IncorrectParamException);

        std::shared_ptr<vmf::Metadata> md5(new vmf::Metadata(desc));
        md5->addValue("value");
        ASSERT_THROW( md5->setFrameIndex(-10, -10), vmf::IncorrectParamException);

        std::shared_ptr<vmf::Metadata> md6(new vmf::Metadata(desc));
        md6->addValue("value");
        ASSERT_EQ(vmf::Metadata::UNDEFINED_FRAME_INDEX, md6->getFrameIndex());
        ASSERT_EQ(vmf::Metadata::UNDEFINED_FRAMES_NUMBER, md6->getNumOfFrames());

        stream.add(md);
        stream.add(md2);
        stream.add(md3);
        stream.add(md4);
        stream.add(md5);
        stream.add(md6);

        stream.save();
        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load(TEST_SCHEMA_NAME);
        stream.close();

        vmf::MetadataSet descSet = stream.queryByName(TEST_DESC_NAME);
        ASSERT_EQ(6u, descSet.size());

        ASSERT_EQ(TEST_FRAME_INDEX, descSet.at(0)->getFrameIndex());
        ASSERT_EQ(TEST_NUMBER_OF_FRAMES, descSet.at(0)->getNumOfFrames());

        ASSERT_EQ(TEST_FRAME_INDEX, descSet.at(1)->getFrameIndex());
        ASSERT_EQ(1, descSet.at(1)->getNumOfFrames());

        ASSERT_EQ(vmf::Metadata::UNDEFINED_FRAME_INDEX, descSet.at(2)->getFrameIndex());
        ASSERT_EQ(vmf::Metadata::UNDEFINED_FRAMES_NUMBER, descSet.at(2)->getNumOfFrames());

        ASSERT_EQ(vmf::Metadata::UNDEFINED_FRAME_INDEX, descSet.at(3)->getFrameIndex());
        ASSERT_EQ(vmf::Metadata::UNDEFINED_FRAMES_NUMBER, descSet.at(3)->getNumOfFrames());

        ASSERT_EQ(vmf::Metadata::UNDEFINED_FRAME_INDEX, descSet.at(4)->getFrameIndex());
        ASSERT_EQ(vmf::Metadata::UNDEFINED_FRAMES_NUMBER, descSet.at(4)->getNumOfFrames());

        ASSERT_EQ(vmf::Metadata::UNDEFINED_FRAME_INDEX, descSet.at(5)->getFrameIndex());
        ASSERT_EQ(vmf::Metadata::UNDEFINED_FRAMES_NUMBER, descSet.at(5)->getNumOfFrames());
    }
}

TEST_F(TestSaveLoadMetadata, Timestamp)
{
    const long long TEST_TIME = vmf::getTimestamp();
    ASSERT_GT(TEST_TIME, 0);
    const long long TEST_TIME_2 = vmf::getTimestamp(1970, 1, 2, 0, 0, 1);
    ASSERT_GT(TEST_TIME_2, 0);
    const long long TEST_DURATION = 1000;

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.addSchema(schema);

        std::shared_ptr<vmf::Metadata> md(new vmf::Metadata(desc));
        md->setTimestamp(TEST_TIME, TEST_DURATION);
        md->addValue("value");
        ASSERT_EQ(TEST_TIME, md->getTime());
        ASSERT_EQ(TEST_DURATION, md->getDuration());

        std::shared_ptr<vmf::Metadata> md2(new vmf::Metadata(desc));
        md2->setTimestamp(TEST_TIME_2);
        md2->addValue("value");
        ASSERT_EQ(TEST_TIME_2, md2->getTime());
        ASSERT_EQ(vmf::Metadata::UNDEFINED_DURATION, md2->getDuration());

        std::shared_ptr<vmf::Metadata> md3(new vmf::Metadata(desc));
        md3->addValue("value");
        ASSERT_THROW(md3->setTimestamp(TEST_TIME, -10), vmf::IncorrectParamException);

        std::shared_ptr<vmf::Metadata> md4(new vmf::Metadata(desc));
        md4->addValue("value");
        ASSERT_THROW( md4->setTimestamp(-10, TEST_DURATION), vmf::IncorrectParamException);

        std::shared_ptr<vmf::Metadata> md5(new vmf::Metadata(desc));
        md5->addValue("value");
        ASSERT_THROW( md5->setTimestamp(-10, -10), vmf::IncorrectParamException);

        std::shared_ptr<vmf::Metadata> md6(new vmf::Metadata(desc));
        md6->addValue("value");
        ASSERT_EQ(vmf::Metadata::UNDEFINED_TIMESTAMP, md6->getTime());
        ASSERT_EQ(vmf::Metadata::UNDEFINED_DURATION, md6->getDuration());

        stream.add(md);
        stream.add(md2);
        stream.add(md3);
        stream.add(md4);
        stream.add(md5);
        stream.add(md6);

        stream.save();
        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load(TEST_SCHEMA_NAME);
        stream.close();

        vmf::MetadataSet descSet = stream.queryByName(TEST_DESC_NAME);
        ASSERT_EQ(6u, descSet.size());

        ASSERT_EQ(TEST_TIME, descSet.at(0)->getTime());
        ASSERT_EQ(TEST_DURATION, descSet.at(0)->getDuration());

        ASSERT_EQ(TEST_TIME_2, descSet.at(1)->getTime());
        ASSERT_EQ(vmf::Metadata::UNDEFINED_DURATION, descSet.at(1)->getDuration());

        ASSERT_EQ(vmf::Metadata::UNDEFINED_TIMESTAMP, descSet.at(2)->getTime());
        ASSERT_EQ(vmf::Metadata::UNDEFINED_DURATION, descSet.at(2)->getDuration());

        ASSERT_EQ(vmf::Metadata::UNDEFINED_TIMESTAMP, descSet.at(3)->getTime());
        ASSERT_EQ(vmf::Metadata::UNDEFINED_DURATION, descSet.at(3)->getDuration());

        ASSERT_EQ(vmf::Metadata::UNDEFINED_TIMESTAMP, descSet.at(4)->getTime());
        ASSERT_EQ(vmf::Metadata::UNDEFINED_DURATION, descSet.at(4)->getDuration());

        ASSERT_EQ(vmf::Metadata::UNDEFINED_TIMESTAMP, descSet.at(5)->getTime());
        ASSERT_EQ(vmf::Metadata::UNDEFINED_DURATION, descSet.at(5)->getDuration());

        ASSERT_EQ(descSet.queryByTime(0, TEST_TIME + TEST_DURATION).size(), 2u);
        ASSERT_EQ(descSet.queryByTime(0, TEST_TIME_2 + TEST_DURATION).size(), 1u);
    }
}

TEST_F(TestSaveLoadMetadata, LoadByProperty)
{
    const vmf::vmf_string atom(TEST_DESC_NAME_1);
    {
        std::shared_ptr<vmf::MetadataDesc> desc1;
        desc1 = std::make_shared<vmf::MetadataDesc>(TEST_DESC_NAME_1, vmf::Variant::type_string);
        schema->add(desc1);

        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        auto md1 = std::make_shared<vmf::Metadata>(desc);
        auto md2 = std::make_shared<vmf::Metadata>(desc1);
        md1->addValue(TEST_VALUE_1);
        md2->addValue(TEST_VALUE_2);
        stream.addSchema(schema);
        stream.add(md1);
        stream.add(md2);
        stream.save();
        stream.close();
    }
    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load(TEST_SCHEMA_NAME, TEST_DESC_NAME);
        auto descSet0 = stream.queryByName(TEST_DESC_NAME);
        auto descSet1 = stream.queryByName(TEST_DESC_NAME_1);
        ASSERT_EQ(1u, descSet0.size());
        ASSERT_TRUE(descSet1.empty());
        stream.close();
    }
    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load(TEST_SCHEMA_NAME, TEST_DESC_NAME_1);
        auto descSet0 = stream.queryByName(TEST_DESC_NAME);
        auto descSet1 = stream.queryByName(TEST_DESC_NAME_1);
        ASSERT_TRUE(descSet0.empty());
        ASSERT_EQ(1u, descSet1.size());
        stream.close();
    }
}

TEST_F(TestSaveLoadMetadata, TwoMetadata)
{
    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.addSchema(schema);
        auto meta1 = std::make_shared<vmf::Metadata>(desc);
        meta1->addValue(TEST_VALUE_1);
        auto meta2 = std::make_shared<vmf::Metadata>(desc);
        meta2->addValue(TEST_VALUE_2);
        stream.add(meta1);
        stream.add(meta2);
        stream.save();
        stream.close();

    }
    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load();
        vmf::MetadataSet metaSet = stream.queryByName(TEST_DESC_NAME);
        ASSERT_EQ(2u, metaSet.size());
        stream.close();
    }
}

TEST_F(TestSaveLoadMetadata, EmptyFields)
{
    const vmf::vmf_string atom(TEST_FIELD_NAME);
    {
        vmf::MetadataStream stream;
        std::vector<vmf::FieldDesc> fields;
        fields.push_back(vmf::FieldDesc(TEST_FIELD_NAME, vmf::Variant::type_string));
        desc = std::shared_ptr<vmf::MetadataDesc> (new vmf::MetadataDesc(TEST_DESC_NAME, fields));
        schema = std::shared_ptr<vmf::MetadataSchema>(new vmf::MetadataSchema(TEST_SCHEMA_NAME));
        schema->add(desc);

        copyFile(TEST_FILE_SRC, TEST_FILE);
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.addSchema(schema);

        std::shared_ptr<vmf::Metadata> md(new vmf::Metadata(desc));
        md->setFieldValue(TEST_FIELD_NAME, "");
        stream.add(md);

        std::shared_ptr<vmf::Metadata> md2(new vmf::Metadata(desc));
        md2->setFieldValue(TEST_FIELD_NAME, TEST_FIELD_NAME);
        stream.add(md2);

        stream.save();
        stream.close();
    }

    {
        vmf::MetadataStream stream2;
        stream2.open(TEST_FILE, vmf::MetadataStream::ReadOnly);

        stream2.load();
        auto all = stream2.getAll();
        ASSERT_EQ(2u, all.size());
        stream2.close();
    }
}

TEST_F(TestSaveLoadMetadata, RemoveFromOneProperty)
{
    vmf::IdType idToRemove;
    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.addSchema(schema);
        auto meta1 = std::make_shared<vmf::Metadata>(desc);
        meta1->addValue(TEST_VALUE_1);
        auto meta2 = std::make_shared<vmf::Metadata>(desc);
        meta2->addValue(TEST_VALUE_2);
        stream.add(meta1);
        stream.add(meta2);
        idToRemove = meta1->getId();
        stream.save();
        stream.close();
    }
    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.load();
        std::shared_ptr<vmf::Metadata> md = stream.getById(idToRemove);
        ASSERT_TRUE(md != nullptr);
        vmf::MetadataSet descSet = stream.queryByName(TEST_DESC_NAME);
        ASSERT_EQ(2u, descSet.size());
        stream.remove(idToRemove);
        stream.save();
        stream.close();
    }
    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.load();
        std::shared_ptr<vmf::Metadata> md = stream.getById(idToRemove);
        ASSERT_TRUE(md == nullptr);
        vmf::MetadataSet descSet = stream.queryByName(TEST_DESC_NAME);
        ASSERT_EQ(1u, descSet.size());
        ASSERT_EQ(TEST_VALUE_2, (vmf::vmf_string) descSet.at(0)->at(0));
        stream.close();
    }
}
TEST_F(TestSaveLoadMetadata, shift)
{
    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.addSchema(schema);
        auto meta1 = std::make_shared<vmf::Metadata>(desc);
        meta1->setFrameIndex(4, 8);
        meta1->addValue(TEST_VALUE_1);
        auto meta2 = std::make_shared<vmf::Metadata>(desc);
        meta2->addValue(TEST_VALUE_2);
        meta2->setFrameIndex(5, 10);
        stream.add(meta1);
        stream.add(meta2);

        meta1->addReference(meta2);
        meta2->addReference(meta1);

        vmf::MetadataSet set = stream.queryByFrameIndex(6);
        ASSERT_EQ(set.size(), 2u);

        set.shift(25, 6, 2);

        ASSERT_EQ(meta1->getFrameIndex(), 25);
        ASSERT_EQ(meta1->getNumOfFrames(), 2);
        ASSERT_EQ(meta2->getFrameIndex(), 25);
        ASSERT_EQ(meta2->getNumOfFrames(), 2);

        vmf::MetadataSet newSet;
        auto newMd = std::make_shared<vmf::Metadata>(desc);
        newSet.push_back(newMd);
        ASSERT_EQ(1, newSet.shift(3, 2, 2));
        newMd->setFrameIndex(0, 0);
        ASSERT_EQ(0, newSet.shift(2, -1, 2));
    }
}

TEST_F(TestSaveLoadMetadata, Import)
{
    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.addSchema(schema);
        auto meta1 = std::make_shared<vmf::Metadata>(desc);
        meta1->setFrameIndex(4, 8);
        meta1->addValue(TEST_VALUE_1);
        auto meta2 = std::make_shared<vmf::Metadata>(desc);
        meta2->addValue(TEST_VALUE_2);
        meta2->setFrameIndex(5, 10);
        stream.add(meta1);
        stream.add(meta2);
        vmf::MetadataSet set = stream.queryByName(TEST_DESC_NAME);

        copyFile(TEST_FILE_SRC, TEST_FILE2);
        vmf::MetadataStream importStream;
        importStream.open(TEST_FILE2, vmf::MetadataStream::Update);
        importStream.import(stream, set, 6, 2, 3);

        stream.close();
        stream.clear();

        vmf::MetadataSet importSet = importStream.queryByName(TEST_DESC_NAME);
        ASSERT_EQ(importSet.size(), 1u);
        ASSERT_EQ((std::string) importSet[0]->at(0), TEST_VALUE_1);

        auto importedSchema = importStream.getSchema(TEST_SCHEMA_NAME);
        ASSERT_NE(importedSchema, nullptr);
        auto importedDesc = importedSchema->findMetadataDesc(TEST_DESC_NAME);

        ASSERT_EQ(desc->getFields(), importedDesc->getFields());
    }
}

TEST_F(TestSaveLoadMetadata, Structure)
{
    {
        std::vector<vmf::FieldDesc> fields;
        fields.push_back(vmf::FieldDesc("name", vmf::Variant::type_string));
        fields.push_back(vmf::FieldDesc("last name", vmf::Variant::type_string));
        fields.push_back(vmf::FieldDesc("age", vmf::Variant::type_integer));
        desc = std::make_shared<vmf::MetadataDesc>("people", fields);
        schema = std::shared_ptr<vmf::MetadataSchema>(new vmf::MetadataSchema(TEST_SCHEMA_NAME));
        schema->add(desc);

        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.addSchema(schema);
        auto meta = std::make_shared<vmf::Metadata>(desc);
        meta->setFieldValue("name", "Dmitry");
        meta->setFieldValue("last name", "Bogdanov");
        meta->setFieldValue("age", (vmf::vmf_integer) 21);
        stream.add(meta);
        stream.save();
        stream.close();
    }
    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load();
        stream.close();
        vmf::MetadataSet people = stream.queryByName("people");
        std::shared_ptr<vmf::Metadata> me = people.at(0);
        ASSERT_EQ("Dmitry", (vmf::vmf_string) me->getFieldValue("name"));
        ASSERT_EQ("Bogdanov", (vmf::vmf_string) me->getFieldValue("last name"));
        ASSERT_EQ(21, (vmf::vmf_integer) me->getFieldValue("age"));
    }
}


class TestSaveLoadFields : public ::testing::Test
{
protected:
    void SetUp()
    {
        //vmf::initialize();

        TEST_SCHEMA_NAME = "TEST_SCHEMA_NAME";
        TEST_DESC_NAME = "TEST_DESC_NAME";
        TEST_FIELD_NAME = "name";
        TEST_FIELD_AGE = "age";
        TEST_FIELD_ADDRESS = "address";

        schema = std::shared_ptr<vmf::MetadataSchema>(new vmf::MetadataSchema(TEST_SCHEMA_NAME));

        std::vector<vmf::FieldDesc> fields;
        fields.push_back(vmf::FieldDesc(TEST_FIELD_NAME, vmf::Variant::type_string));
        fields.push_back(vmf::FieldDesc(TEST_FIELD_AGE, vmf::Variant::type_integer));
        fields.push_back(vmf::FieldDesc(TEST_FIELD_ADDRESS, vmf::Variant::type_string, true));
        desc = std::shared_ptr<vmf::MetadataDesc>(new vmf::MetadataDesc(TEST_DESC_NAME, fields));

        schema->add(desc);

        copyFile(TEST_FILE_SRC, TEST_FILE);

        vmf::MetadataStream stream;
        if(!stream.open(TEST_FILE, vmf::MetadataStream::Update))
            std::cout << "Can't open stream!" << std::endl;

        stream.addSchema(schema);

        stream.save();
        stream.close();
    }

    void TearDown()
    {
        //vmf::terminate();
    }

    std::shared_ptr<vmf::MetadataSchema> schema;
    std::shared_ptr<vmf::MetadataDesc> desc;

    vmf::vmf_string TEST_SCHEMA_NAME;
    vmf::vmf_string TEST_DESC_NAME;
    vmf::vmf_string TEST_FIELD_NAME;
    vmf::vmf_string TEST_FIELD_AGE;
    vmf::vmf_string TEST_FIELD_ADDRESS;
};

TEST_F(TestSaveLoadFields, EmptyNonOptionalFields)
{
    vmf::MetadataStream stream;
    stream.open(TEST_FILE, vmf::MetadataStream::Update);

    std::shared_ptr<vmf::Metadata> md(new vmf::Metadata(desc));
    md->setFieldValue(TEST_FIELD_NAME, "Konstantin");
    ASSERT_THROW(stream.add(md), vmf::ValidateException);

    stream.save();
    stream.close();
}

TEST_F(TestSaveLoadFields, EmptyOptionalFields)
{
    vmf::MetadataStream stream;
    stream.open(TEST_FILE, vmf::MetadataStream::Update);

    std::shared_ptr<vmf::Metadata> md(new vmf::Metadata(desc));
    md->setFieldValue(TEST_FIELD_NAME, "Konstantin");
    md->setFieldValue(TEST_FIELD_AGE, 22);
    ASSERT_NO_THROW(stream.add(md));

    stream.save();
    stream.close();
}

TEST_F(TestSaveLoadFields, Attributes)
{
    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);

        std::shared_ptr<vmf::Metadata> md(new vmf::Metadata(desc));
        md->setFieldValue(TEST_FIELD_NAME, "Konstantin");
        md->setFieldValue(TEST_FIELD_AGE, 22);

        std::shared_ptr<vmf::Metadata> md2(new vmf::Metadata(desc));
        md2->setFieldValue(TEST_FIELD_NAME, "Vasya Pupkin");
        md2->setFieldValue(TEST_FIELD_AGE, 33);
        md2->setFieldValue(TEST_FIELD_ADDRESS, "Karaganda");

        stream.add(md);
        stream.add(md2);

        stream.save();
        stream.close();
    }
    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load();

        auto people = stream.queryByName(TEST_DESC_NAME);

        ASSERT_EQ(2u, people.size());
        ASSERT_TRUE(people[0]->hasField(TEST_FIELD_NAME));
        ASSERT_EQ("Konstantin", (vmf::vmf_string)people[0]->getFieldValue(TEST_FIELD_NAME));
        ASSERT_EQ(22, (vmf::vmf_integer)people[0]->getFieldValue(TEST_FIELD_AGE));
        vmf::Variant addressValue;
        ASSERT_FALSE(people[0]->hasField(TEST_FIELD_ADDRESS));
        ASSERT_NO_THROW(addressValue = people[0]->getFieldValue(TEST_FIELD_ADDRESS));
        ASSERT_EQ(vmf::Variant::type_empty, addressValue.getType());
        ASSERT_TRUE(addressValue.isEmpty());

        ASSERT_EQ("Vasya Pupkin", (vmf::vmf_string)people[1]->getFieldValue(TEST_FIELD_NAME) );
        ASSERT_EQ(33, (vmf::vmf_integer)people[1]->getFieldValue(TEST_FIELD_AGE));
        ASSERT_EQ("Karaganda", (vmf::vmf_string)people[1]->getFieldValue(TEST_FIELD_ADDRESS) );

        stream.close();
    }
}

class TestSaveLoadUtils : public ::testing::Test
{
protected:
    void SetUp()
    {
        copyFile(TEST_FILE_SRC, TEST_FILE);
        //vmf::initialize();
    }

    void TearDown()
    {
        //vmf::terminate();
    }
};

TEST_F(TestSaveLoadUtils, Checksum)
{
    std::string checksum1, checksum2;
    {
        vmf::MetadataStream stream;

        ASSERT_THROW(stream.computeChecksum(), vmf::InternalErrorException);

        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        checksum1 = stream.computeChecksum();
        stream.setChecksum(checksum1);
        stream.save();
        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        checksum2 = stream.getChecksum();
        stream.close();
    }

    ASSERT_EQ(checksum1, checksum2);
}


//Bad compressor that increases the size of input data
class BloatingCompressor : public vmf::Compressor
{
public:
    static const int factor = 5;

    virtual void compress(const vmf::vmf_string& input, vmf::vmf_rawbuffer& output)
    {
        output.clear();
        for(auto it = input.begin(); it != input.end(); ++it)
        {
            for(int i = 0; i < factor; i++)
                output.push_back(*it);
        }
    }

    virtual void decompress(const vmf::vmf_rawbuffer& input, vmf::vmf_string &output)
    {
        output.clear();
        for(auto it = input.begin(); it != input.end();)
        {
            char c = *it++;
            for(int i = 1; i < factor; i++)
            {
                char cc = *it++;
                if(cc != c)
                {
                    throw std::runtime_error("Incorrect input to bloating compressor");
                }
            }
            output.push_back(c);
        }
    }

    std::shared_ptr<Compressor> createNewInstance() const
    {
        return std::shared_ptr<Compressor>(new BloatingCompressor);
    }

    virtual vmf::vmf_string getId()
    {
        return "com.intel.vmf.compressor.test.bloating";
    }
};


class TestSaveLoadCompressionEncryption : public ::testing::TestWithParam< std::tuple<std::string,
                                                                                      vmf::CryptAlgo> >
{
protected:
    void SetUp()
    {
        copyFile(TEST_FILE_SRC, TEST_FILE);
        //vmf::initialize();
        //register bloating compressor
        std::shared_ptr<vmf::Compressor> bloating = std::make_shared<BloatingCompressor>();
        vmf::Compressor::registerNew(bloating);
    }

    void TearDown()
    {
        vmf::Compressor::unregister("com.intel.vmf.compressor.test.bloating");
        //vmf::terminate();
    }
};


TEST_P(TestSaveLoadCompressionEncryption, Checksum)
{
    std::string name = std::get<0>(GetParam());
    std::shared_ptr<vmf::Encryptor> encryptor = vmf::getEncryptor(std::get<1>(GetParam()));

    std::string checksum1, checksum2;
    {
        vmf::MetadataStream stream;
        stream.setEncryptor(encryptor);
        stream.setUseEncryption(true);

        ASSERT_THROW(stream.computeChecksum(), vmf::InternalErrorException);

        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        checksum1 = stream.computeChecksum();
        stream.setChecksum(checksum1);
        if(name == "unregistered")
        {
            ASSERT_FALSE(stream.save(name));
        }
        else
        {
            if(encryptor)
            {
                ASSERT_TRUE(stream.save(name));
            }
            else
            {
                ASSERT_FALSE(stream.save(name));
                stream.setUseEncryption(false);
                ASSERT_TRUE(stream.save(name));
            }
        }
        stream.close();
    }

    if(name != "unregistered")
    {
        vmf::MetadataStream stream;
        stream.setEncryptor(encryptor);

        ASSERT_TRUE(stream.open(TEST_FILE, vmf::MetadataStream::Update));
        checksum2 = stream.getChecksum();
        stream.close();

        ASSERT_EQ(checksum1, checksum2);
    }
}


TEST_P(TestSaveLoadCompressionEncryption, CheckIgnoreUnknownCompressor)
{
    std::string name = std::get<0>(GetParam());
    std::shared_ptr<vmf::Encryptor> encryptor = vmf::getEncryptor(std::get<1>(GetParam()));

    if(name == "com.intel.vmf.compressor.test.bloating")
    {
        {
            vmf::MetadataStream stream;
            stream.setEncryptor(encryptor);
            stream.setUseEncryption(true);

            stream.open(TEST_FILE, vmf::MetadataStream::Update);

            //arbitrary content
            std::string checksum1 = stream.computeChecksum();
            stream.setChecksum(checksum1);

            if(encryptor)
            {
                ASSERT_TRUE(stream.save(name));
            }
            else
            {
                ASSERT_FALSE(stream.save(name));
                stream.setUseEncryption(false);
                ASSERT_TRUE(stream.save(name));
            }
            stream.close();
        }

        vmf::Compressor::unregister("com.intel.vmf.compressor.test.bloating");

        {
            vmf::MetadataStream stream;
            stream.setEncryptor(encryptor);
            ASSERT_TRUE(stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly |
                                               vmf::MetadataStream::IgnoreUnknownCompressor));
            std::vector<std::string> schemaNames = stream.getAllSchemaNames();
            ASSERT_EQ(schemaNames.size(), 1u);
            ASSERT_EQ(schemaNames[0], "com.intel.vmf.compressed-metadata");
        }

        //restore as it was before
        std::shared_ptr<vmf::Compressor> bloating = std::make_shared<BloatingCompressor>();
        vmf::Compressor::registerNew(bloating);
    }
}


TEST_P(TestSaveLoadCompressionEncryption, CheckIgnoreUnknownEncryption)
{
    std::string name = std::get<0>(GetParam());
    std::shared_ptr<vmf::Encryptor> encryptor = vmf::getEncryptor(std::get<1>(GetParam()));

    std::string checksum1, checksum2;
    {
        vmf::MetadataStream stream;
        stream.setEncryptor(encryptor);
        stream.setUseEncryption(true);

        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        checksum1 = stream.computeChecksum();
        stream.setChecksum(checksum1);
        if(name == "unregistered")
        {
            ASSERT_FALSE(stream.save(name));
        }
        else
        {
            if(encryptor)
            {
                ASSERT_TRUE(stream.save(name));
            }
            else
            {
                ASSERT_FALSE(stream.save(name));
                stream.setUseEncryption(false);
                ASSERT_TRUE(stream.save(name));
            }
        }
        stream.close();
    }

    if(name != "unregistered" && encryptor)
    {
        vmf::MetadataStream stream;

        ASSERT_TRUE(stream.open(TEST_FILE, vmf::MetadataStream::Update |
                                           vmf::MetadataStream::IgnoreUnknownEncryptor));
        std::vector<std::string> schemaNames = stream.getAllSchemaNames();
        ASSERT_EQ(schemaNames.size(), 1);
        ASSERT_EQ(schemaNames[0], "com.intel.vmf.encrypted-metadata");
        ASSERT_TRUE(stream.load(schemaNames[0]));
        vmf::MetadataSet eSet = stream.queryBySchema(schemaNames[0]);
        ASSERT_FALSE(eSet.empty());
        std::shared_ptr<vmf::Metadata> eItem = eSet[0];
        ASSERT_EQ((vmf::vmf_string)eItem->getFieldValue("hint"), encryptor->getHint());
    }
}

INSTANTIATE_TEST_CASE_P(UnitTest, TestSaveLoadCompressionEncryption,
                        ::testing::Combine(
                            ::testing::Values(vmf::Compressor::builtinId(),
                                              "unregistered",
                                              "com.intel.vmf.compressor.test.bloating"),
                            ::testing::Values(vmf::CryptAlgo::DEFAULT, vmf::CryptAlgo::WEAK, vmf::CryptAlgo::NONE)
                            ));


class TestSaveLoadEncryptionSubsets : public ::testing::TestWithParam<vmf::CryptAlgo>
{
protected:
    void SetUp()
    {
        copyFile(TEST_FILE_SRC, TEST_FILE);
        vmf::initialize();
        TEST_SCHEMA_NAME = "TEST_SCHEMA_NAME";
        TEST_DESC_NAME = "TEST_DESC_NAME";
        TEST_FIELD_NAME = "name";
        TEST_VALUE_NAME = "Sherlock";
        TEST_FIELD_AGE = "age";
        TEST_VALUE_AGE = 33;
        TEST_FIELD_ADDRESS = "address";
        TEST_VALUE_ADDRESS = "221b Baker St, London NW1 6XE, UK";
    }

    void TearDown()
    {
        vmf::terminate();
    }

    vmf::vmf_string  TEST_SCHEMA_NAME;
    vmf::vmf_string  TEST_DESC_NAME;
    vmf::vmf_string  TEST_FIELD_NAME;
    vmf::vmf_string  TEST_VALUE_NAME;
    vmf::vmf_string  TEST_FIELD_AGE;
    vmf::vmf_integer TEST_VALUE_AGE;
    vmf::vmf_string  TEST_FIELD_ADDRESS;
    vmf::vmf_string  TEST_VALUE_ADDRESS;
};


TEST_P(TestSaveLoadEncryptionSubsets, OneField)
{
    std::shared_ptr<vmf::Encryptor> encryptor = vmf::getEncryptor(GetParam());

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.setEncryptor(encryptor);
        stream.setUseEncryption(false);

        std::shared_ptr<vmf::MetadataSchema> schema;
        std::shared_ptr<vmf::MetadataDesc> desc;

        schema = std::make_shared<vmf::MetadataSchema>(TEST_SCHEMA_NAME);

        std::vector<vmf::FieldDesc> fields;
        fields.push_back(vmf::FieldDesc(TEST_FIELD_NAME, vmf::Variant::type_string));
        fields.push_back(vmf::FieldDesc(TEST_FIELD_AGE, vmf::Variant::type_integer));
        fields.push_back(vmf::FieldDesc(TEST_FIELD_ADDRESS, vmf::Variant::type_string, true));
        desc = std::make_shared<vmf::MetadataDesc>(TEST_DESC_NAME, fields);

        schema->add(desc);
        stream.addSchema(schema);

        std::shared_ptr<vmf::Metadata> address(new vmf::Metadata(desc));
        address->setFieldValue(TEST_FIELD_NAME, TEST_VALUE_NAME);
        address->setFieldValue(TEST_FIELD_AGE, TEST_VALUE_AGE);
        address->setFieldValue(TEST_FIELD_ADDRESS, TEST_VALUE_ADDRESS);
        address->findField(TEST_FIELD_ADDRESS)->setUseEncryption(true);
        address->setTimestamp(vmf::getTimestamp());

        stream.add(address);

        if(encryptor)
        {
            ASSERT_TRUE(stream.save());
        }
        else
        {
            ASSERT_FALSE(stream.save());
            address->findField(TEST_FIELD_ADDRESS)->setUseEncryption(false);
            ASSERT_TRUE(stream.save());
        }

        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.setEncryptor(encryptor);

        ASSERT_TRUE(stream.open(TEST_FILE, vmf::MetadataStream::OpenModeFlags::ReadOnly));
        ASSERT_TRUE(stream.load(TEST_SCHEMA_NAME));
        vmf::MetadataSet mSet = stream.queryBySchema(TEST_SCHEMA_NAME);
        ASSERT_EQ(mSet.size(), 1);

        ASSERT_EQ(mSet[0]->findField(TEST_FIELD_ADDRESS)->getUseEncryption(), (bool)encryptor);

        vmf::vmf_string gotAddress = mSet[0]->getFieldValue(TEST_FIELD_ADDRESS);
        ASSERT_EQ(gotAddress, TEST_VALUE_ADDRESS);
    }
}


TEST_P(TestSaveLoadEncryptionSubsets, OneRecord)
{
    std::shared_ptr<vmf::Encryptor> encryptor = vmf::getEncryptor(GetParam());

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.setEncryptor(encryptor);
        stream.setUseEncryption(false);

        std::shared_ptr<vmf::MetadataSchema> schema;
        std::shared_ptr<vmf::MetadataDesc> desc;

        schema = std::make_shared<vmf::MetadataSchema>(TEST_SCHEMA_NAME);

        std::vector<vmf::FieldDesc> fields;
        fields.push_back(vmf::FieldDesc(TEST_FIELD_NAME, vmf::Variant::type_string));
        fields.push_back(vmf::FieldDesc(TEST_FIELD_AGE, vmf::Variant::type_integer));
        fields.push_back(vmf::FieldDesc(TEST_FIELD_ADDRESS, vmf::Variant::type_string, true));
        desc = std::make_shared<vmf::MetadataDesc>(TEST_DESC_NAME, fields);

        schema->add(desc);
        stream.addSchema(schema);

        std::shared_ptr<vmf::Metadata> address(new vmf::Metadata(desc));
        address->setFieldValue(TEST_FIELD_NAME, TEST_VALUE_NAME);
        address->setFieldValue(TEST_FIELD_AGE, TEST_VALUE_AGE);
        address->setFieldValue(TEST_FIELD_ADDRESS, TEST_VALUE_ADDRESS);
        address->setTimestamp(vmf::getTimestamp());
        address->setUseEncryption(true);

        stream.add(address);

        if(encryptor)
        {
            ASSERT_TRUE(stream.save());
        }
        else
        {
            ASSERT_FALSE(stream.save());
            address->setUseEncryption(false);
            ASSERT_TRUE(stream.save());
        }
        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.setEncryptor(encryptor);

        ASSERT_TRUE(stream.open(TEST_FILE, vmf::MetadataStream::OpenModeFlags::ReadOnly));
        ASSERT_TRUE(stream.load(TEST_SCHEMA_NAME));
        vmf::MetadataSet mSet = stream.queryBySchema(TEST_SCHEMA_NAME);
        ASSERT_EQ(mSet.size(), 1);

        ASSERT_EQ(mSet[0]->getUseEncryption(), (bool)encryptor);

        vmf::vmf_string gotAddress = mSet[0]->getFieldValue(TEST_FIELD_ADDRESS);
        ASSERT_EQ(gotAddress, TEST_VALUE_ADDRESS);
    }
}


TEST_P(TestSaveLoadEncryptionSubsets, FieldDesc)
{
    std::shared_ptr<vmf::Encryptor> encryptor = vmf::getEncryptor(GetParam());

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.setEncryptor(encryptor);
        stream.setUseEncryption(false);

        std::shared_ptr<vmf::MetadataSchema> schema;
        std::shared_ptr<vmf::MetadataDesc> desc;

        schema = std::make_shared<vmf::MetadataSchema>(TEST_SCHEMA_NAME);

        std::vector<vmf::FieldDesc> fields;
        fields.push_back(vmf::FieldDesc(TEST_FIELD_NAME, vmf::Variant::type_string));
        fields.push_back(vmf::FieldDesc(TEST_FIELD_AGE, vmf::Variant::type_integer));
        fields.push_back(vmf::FieldDesc(TEST_FIELD_ADDRESS, vmf::Variant::type_string, true, true));
        desc = std::make_shared<vmf::MetadataDesc>(TEST_DESC_NAME, fields);

        schema->add(desc);
        stream.addSchema(schema);

        std::shared_ptr<vmf::Metadata> address(new vmf::Metadata(desc));
        address->setFieldValue(TEST_FIELD_NAME, TEST_VALUE_NAME);
        address->setFieldValue(TEST_FIELD_AGE, TEST_VALUE_AGE);
        address->setFieldValue(TEST_FIELD_ADDRESS, TEST_VALUE_ADDRESS);
        address->setTimestamp(vmf::getTimestamp());

        stream.add(address);

        if(encryptor)
        {
            ASSERT_TRUE(stream.save());
        }
        else
        {
            ASSERT_FALSE(stream.save());
            desc->getFieldDesc(TEST_FIELD_ADDRESS).useEncryption = false;
            ASSERT_TRUE(stream.save());
        }
        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.setEncryptor(encryptor);

        ASSERT_TRUE(stream.open(TEST_FILE, vmf::MetadataStream::OpenModeFlags::ReadOnly));

        std::shared_ptr<vmf::MetadataSchema> schema;
        std::shared_ptr<vmf::MetadataDesc> desc;

        schema = stream.getSchema(TEST_SCHEMA_NAME);
        desc = schema->findMetadataDesc(TEST_DESC_NAME);
        vmf::FieldDesc& field = desc->getFieldDesc(TEST_FIELD_ADDRESS);
        ASSERT_EQ(field.useEncryption, (bool)encryptor);

        ASSERT_TRUE(stream.load(TEST_SCHEMA_NAME));
        vmf::MetadataSet mSet = stream.queryBySchema(TEST_SCHEMA_NAME);
        ASSERT_EQ(mSet.size(), 1);
        vmf::vmf_string gotAddress = mSet[0]->getFieldValue(TEST_FIELD_ADDRESS);
        ASSERT_EQ(gotAddress, TEST_VALUE_ADDRESS);
    }
}


TEST_P(TestSaveLoadEncryptionSubsets, MetaDesc)
{
    std::shared_ptr<vmf::Encryptor> encryptor = vmf::getEncryptor(GetParam());

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.setEncryptor(encryptor);
        stream.setUseEncryption(false);

        std::shared_ptr<vmf::MetadataSchema> schema;
        std::shared_ptr<vmf::MetadataDesc> desc;

        schema = std::make_shared<vmf::MetadataSchema>(TEST_SCHEMA_NAME);

        std::vector<vmf::FieldDesc> fields;
        fields.push_back(vmf::FieldDesc(TEST_FIELD_NAME, vmf::Variant::type_string));
        fields.push_back(vmf::FieldDesc(TEST_FIELD_AGE, vmf::Variant::type_integer));
        fields.push_back(vmf::FieldDesc(TEST_FIELD_ADDRESS, vmf::Variant::type_string, true));
        desc = std::make_shared<vmf::MetadataDesc>(TEST_DESC_NAME, fields, true);

        schema->add(desc);
        stream.addSchema(schema);

        std::shared_ptr<vmf::Metadata> address(new vmf::Metadata(desc));
        address->setFieldValue(TEST_FIELD_NAME, TEST_VALUE_NAME);
        address->setFieldValue(TEST_FIELD_AGE, TEST_VALUE_AGE);
        address->setFieldValue(TEST_FIELD_ADDRESS, TEST_VALUE_ADDRESS);
        address->setTimestamp(vmf::getTimestamp());

        stream.add(address);

        if(encryptor)
        {
            ASSERT_TRUE(stream.save());
        }
        else
        {
            ASSERT_FALSE(stream.save());
            desc->setUseEncryption(false);
            ASSERT_TRUE(stream.save());
        }
        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.setEncryptor(encryptor);

        ASSERT_TRUE(stream.open(TEST_FILE, vmf::MetadataStream::OpenModeFlags::ReadOnly));

        std::shared_ptr<vmf::MetadataSchema> schema;
        std::shared_ptr<vmf::MetadataDesc> desc;

        schema = stream.getSchema(TEST_SCHEMA_NAME);
        desc = schema->findMetadataDesc(TEST_DESC_NAME);
        ASSERT_EQ(desc->getUseEncryption(), (bool)encryptor);

        ASSERT_TRUE(stream.load(TEST_SCHEMA_NAME));
        vmf::MetadataSet mSet = stream.queryBySchema(TEST_SCHEMA_NAME);
        ASSERT_EQ(mSet.size(), 1);
        vmf::vmf_string gotAddress = mSet[0]->getFieldValue(TEST_FIELD_ADDRESS);
        ASSERT_EQ(gotAddress, TEST_VALUE_ADDRESS);
    }
}


TEST_P(TestSaveLoadEncryptionSubsets, Schema)
{
    std::shared_ptr<vmf::Encryptor> encryptor = vmf::getEncryptor(GetParam());

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.setEncryptor(encryptor);
        stream.setUseEncryption(false);

        std::shared_ptr<vmf::MetadataSchema> schema;
        std::shared_ptr<vmf::MetadataDesc> desc;

        schema = std::make_shared<vmf::MetadataSchema>(TEST_SCHEMA_NAME, true);

        std::vector<vmf::FieldDesc> fields;
        fields.push_back(vmf::FieldDesc(TEST_FIELD_NAME, vmf::Variant::type_string));
        fields.push_back(vmf::FieldDesc(TEST_FIELD_AGE, vmf::Variant::type_integer));
        fields.push_back(vmf::FieldDesc(TEST_FIELD_ADDRESS, vmf::Variant::type_string, true));
        desc = std::make_shared<vmf::MetadataDesc>(TEST_DESC_NAME, fields);

        schema->add(desc);
        stream.addSchema(schema);

        std::shared_ptr<vmf::Metadata> address(new vmf::Metadata(desc));
        address->setFieldValue(TEST_FIELD_NAME, TEST_VALUE_NAME);
        address->setFieldValue(TEST_FIELD_AGE, TEST_VALUE_AGE);
        address->setFieldValue(TEST_FIELD_ADDRESS, TEST_VALUE_ADDRESS);
        address->setTimestamp(vmf::getTimestamp());

        stream.add(address);

        if(encryptor)
        {
            ASSERT_TRUE(stream.save());
        }
        else
        {
            ASSERT_FALSE(stream.save());
            schema->setUseEncryption(false);
            ASSERT_TRUE(stream.save());
        }
        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.setEncryptor(encryptor);

        ASSERT_TRUE(stream.open(TEST_FILE, vmf::MetadataStream::OpenModeFlags::ReadOnly));

        std::shared_ptr<vmf::MetadataSchema> schema;

        schema = stream.getSchema(TEST_SCHEMA_NAME);
        ASSERT_EQ(schema->getUseEncryption(), (bool)encryptor);

        ASSERT_TRUE(stream.load(TEST_SCHEMA_NAME));
        vmf::MetadataSet mSet = stream.queryBySchema(TEST_SCHEMA_NAME);
        ASSERT_EQ(mSet.size(), 1);
        vmf::vmf_string gotAddress = mSet[0]->getFieldValue(TEST_FIELD_ADDRESS);
        ASSERT_EQ(gotAddress, TEST_VALUE_ADDRESS);
    }
}


INSTANTIATE_TEST_CASE_P(UnitTest, TestSaveLoadEncryptionSubsets,
                        ::testing::Values(vmf::CryptAlgo::DEFAULT, vmf::CryptAlgo::WEAK, vmf::CryptAlgo::NONE));


