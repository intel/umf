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

class TestRemoving : public ::testing::Test
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

        schema = std::shared_ptr<umf::MetadataSchema>(new umf::MetadataSchema(TEST_SCHEMA_NAME));

        std::vector<umf::FieldDesc> fields1;
        fields1.push_back(umf::FieldDesc(TEST_FIELD_NAME, umf::Variant::type_integer));
        descr1 = std::shared_ptr<umf::MetadataDesc>(new umf::MetadataDesc(TEST_PROPERTY_NAME1, fields1));

        std::vector<umf::FieldDesc> fields2;
        fields2.push_back(umf::FieldDesc(TEST_FIELD_NAME, umf::Variant::type_integer));
        descr2 = std::shared_ptr<umf::MetadataDesc>(new umf::MetadataDesc(TEST_PROPERTY_NAME2, fields2));
        
        schema->add(descr1);
        schema->add(descr2);

        copyFile(TEST_FILE_SRC, TEST_FILE);

        if(!stream.open(TEST_FILE, umf::MetadataStream::Update))
            std::cout << "Cann't open stream!" << std::endl;

        stream.addSchema(schema);

        for(unsigned int i = 0; i < n; i++)
        {
            std::shared_ptr<umf::Metadata> md(new umf::Metadata(descr1));
            md->setFieldValue(TEST_FIELD_NAME, i);

            stream.add(md);

            std::shared_ptr<umf::Metadata> md2(new umf::Metadata(descr2));
            md2->setFieldValue(TEST_FIELD_NAME, n + i);
            stream.add(md2);
        }

        someMoreAddings();

        stream.save();
        stream.close();
        stream.clear();
    }

    virtual void TearDown()
    {
        //umf::terminate();
    }

    virtual void someMoreAddings() {}

    unsigned int n;

    umf::MetadataStream stream;
    std::shared_ptr<umf::MetadataSchema> schema;
    std::shared_ptr<umf::MetadataDesc> descr1;
    std::shared_ptr<umf::MetadataDesc> descr2;

    umf::umf_string TEST_SCHEMA_NAME;
    umf::umf_string TEST_PROPERTY_NAME1;
    umf::umf_string TEST_PROPERTY_NAME2;
    umf::umf_string TEST_FIELD_NAME;
};


TEST_F(TestRemoving, RemoveOne)
{
    umf::MetadataStream newStream;
    newStream.open(TEST_FILE, umf::MetadataStream::Update);
    newStream.load(TEST_SCHEMA_NAME);

    auto set = newStream.queryByName(TEST_PROPERTY_NAME1);
    size_t size1 = set.size();
    int removed_item = n/2;
    auto item = set[removed_item];
    newStream.remove(item->getId());

    set = newStream.queryByName(TEST_PROPERTY_NAME2);
    size_t size2 = set.size();
    item = set[removed_item];
    newStream.remove(item->getId());

    newStream.save();
    newStream. close();

    umf::MetadataStream newStream2;
    newStream2.open(TEST_FILE, umf::MetadataStream::ReadOnly);
    newStream2.load(TEST_SCHEMA_NAME);

    set = newStream2.queryByName(TEST_PROPERTY_NAME1);
    ASSERT_EQ(set.size(), size1-1);
    auto query_result = set.query([&](const std::shared_ptr<umf::Metadata> &spItem)->bool
    {
        return (umf::umf_integer)spItem->getFieldValue(TEST_FIELD_NAME) == removed_item;
    });
    ASSERT_EQ(query_result.size(), 0u);

    set = newStream2.queryByName(TEST_PROPERTY_NAME2);
    ASSERT_EQ(set.size(), size2-1);
    query_result = set.query([&](const std::shared_ptr<umf::Metadata> &spItem)->bool
    {
        return (umf::umf_integer)spItem->getFieldValue(TEST_FIELD_NAME) == n + removed_item;
    });
    ASSERT_EQ(query_result.size(), 0u);
    newStream2.close();
}


TEST_F(TestRemoving, RemoveSet)
{
    {
        umf::MetadataStream newStream;
        newStream.open(TEST_FILE, umf::MetadataStream::Update);
        newStream.load(TEST_SCHEMA_NAME);
        auto set = newStream.queryByName(TEST_PROPERTY_NAME1);
        newStream.remove(set);
        set.clear();
        newStream.save();
        newStream. close();
    }
    {
        umf::MetadataStream newStream;
        newStream.open(TEST_FILE, umf::MetadataStream::ReadOnly);
        newStream.load(TEST_SCHEMA_NAME);
        auto set = newStream.queryByName(TEST_PROPERTY_NAME1);
        ASSERT_EQ(set.size(), 0u);
        set = newStream.queryByName(TEST_PROPERTY_NAME2);
        ASSERT_EQ(set.size(), n);
        newStream.close();
    }
}


TEST_F(TestRemoving, RemoveAll)
{
    {
        umf::MetadataStream newStream;
        newStream.open(TEST_FILE, umf::MetadataStream::Update);
        newStream.load();

        auto set = newStream.getAll();
        newStream.remove(set);
        newStream.save();
        newStream. close();
    }
    {
        umf::MetadataStream newStream;
        newStream.open(TEST_FILE, umf::MetadataStream::ReadOnly);
        newStream.load();
        auto set = newStream.getAll();
        ASSERT_EQ(set.size(), 0u);
        newStream.close();
    }
}


TEST_F(TestRemoving, RemoveWithReferences)
{
    {
        umf::MetadataStream newStream;
        newStream.open(TEST_FILE, umf::MetadataStream::Update);
        newStream.load(TEST_SCHEMA_NAME);

        auto set1 = newStream.queryByName(TEST_PROPERTY_NAME1);
        auto set2 = newStream.queryByName(TEST_PROPERTY_NAME2);
        for(unsigned int i = 0; i < set1.size(); i++)
        {
            auto it2 = set1[i];
            auto it1 = set2[i];
            it2->addReference(it1);
            it1->addReference(it2);
        }
        newStream.remove(set2);
        newStream.save();
        newStream. close();
    }
    {
        umf::MetadataStream newStream;
        newStream.open(TEST_FILE, umf::MetadataStream::ReadOnly);
        newStream.load(TEST_SCHEMA_NAME);
        auto refs = newStream.queryByReference(TEST_PROPERTY_NAME1);
        ASSERT_EQ(refs.size(), 0u);
        refs = newStream.queryByReference(TEST_PROPERTY_NAME2);
        ASSERT_EQ(refs.size(), 0u);
        newStream.close();
    }
}


class TestRemovingSchema : public TestRemoving
{
protected:
    void someMoreAddings()
    {
        TEST_SCHEMA_NAME_2 = "TEST_SCHEMA_NAME_2";
        std::vector<umf::FieldDesc> fields;
        fields.push_back(umf::FieldDesc("fields", umf::Variant::type_integer));
        std::shared_ptr<umf::MetadataDesc> newDesc(new umf::MetadataDesc("desc", fields));
        std::shared_ptr<umf::MetadataSchema> newSchema(new umf::MetadataSchema(TEST_SCHEMA_NAME_2));
        newSchema->add(newDesc);
        stream.addSchema(newSchema);
        std::shared_ptr<umf::Metadata> md(new umf::Metadata(newDesc));
        md->setFieldValue("fields", (umf::umf_integer)4242);
        stream.add(md);
    }

    umf::umf_string TEST_SCHEMA_NAME_2;
};


TEST_F(TestRemovingSchema, RemoveOneSchema)
{
    {
        umf::MetadataStream newStream;
        newStream.open(TEST_FILE, umf::MetadataStream::Update);
        newStream.load();
        auto schemaForRemoving = newStream.getSchema(TEST_SCHEMA_NAME);
        newStream.remove(schemaForRemoving);
        std::shared_ptr<umf::MetadataSchema> nullPointerSchema = nullptr;
        ASSERT_THROW(newStream.remove(nullPointerSchema), umf::NullPointerException);
        newStream.save();
        newStream. close();
    }
    {
        umf::MetadataStream newStream;
        newStream.open(TEST_FILE, umf::MetadataStream::ReadOnly);
        bool result = newStream.load(TEST_SCHEMA_NAME);
        ASSERT_EQ( result, false );
        newStream.close();
    }
}


TEST_F(TestRemovingSchema, RemoveUnknownSchema)
{
    umf::MetadataStream newStream;
    newStream.open(TEST_FILE, umf::MetadataStream::Update);
    newStream.load();
    std::shared_ptr<umf::MetadataSchema> schemaForRemoving(new umf::MetadataSchema("Invalid schema") );
    EXPECT_THROW(newStream.remove(schemaForRemoving), umf::Exception);
}


TEST_F(TestRemovingSchema, RemoveAllSchemas)
{
    {
        umf::MetadataStream newStream;
        newStream.open(TEST_FILE, umf::MetadataStream::Update);
        newStream.load();
        auto schemasNames = newStream.getAllSchemaNames();
        for (auto schemaName = schemasNames.begin(); schemaName != schemasNames.end(); schemaName++)
            newStream.remove(newStream.getSchema(*schemaName));
        newStream.save();
        newStream. close();
    }
    {
        umf::MetadataStream newStream;
        newStream.open(TEST_FILE, umf::MetadataStream::ReadOnly);
        bool result = newStream.load(TEST_SCHEMA_NAME);
        ASSERT_EQ( result, false );
        newStream.close();
    }
}


TEST_F(TestRemovingSchema, RemoveAll)
{
    {
        umf::MetadataStream newStream;
        newStream.open(TEST_FILE, umf::MetadataStream::Update);
        newStream.load();
        newStream.remove();
        newStream.save();
        newStream. close();
    }

    {
        umf::MetadataStream newStream;
        newStream.open(TEST_FILE, umf::MetadataStream::ReadOnly);
        bool result = newStream.load(TEST_SCHEMA_NAME);
        ASSERT_FALSE( result );
        result = newStream.load(TEST_SCHEMA_NAME_2);
        ASSERT_FALSE( result );
        result = newStream.load();
        ASSERT_TRUE( result );
        ASSERT_EQ(0u, newStream.getAll().size());
        newStream.close();
    }
}


TEST_F(TestRemovingSchema, RemoveAllAddSchema)
{
    {
        umf::MetadataStream newStream;
        newStream.open(TEST_FILE, umf::MetadataStream::Update);
        newStream.load();
        newStream.remove();
        newStream.save();
        newStream.close();

        std::shared_ptr<umf::MetadataSchema> newSchema(new umf::MetadataSchema(TEST_SCHEMA_NAME));
        VMF_METADATA_BEGIN(TEST_PROPERTY_NAME2);
            VMF_FIELD_INT(TEST_FIELD_NAME);
        VMF_METADATA_END(newSchema);

        newStream.reopen(umf::MetadataStream::Update);
        newStream.addSchema(newSchema);
        newStream.save();
        newStream.close();
    }

    {
        umf::MetadataStream newStream;
        newStream.open(TEST_FILE, umf::MetadataStream::ReadOnly);
        bool result = newStream.load(TEST_SCHEMA_NAME);
        ASSERT_TRUE( result );
        std::shared_ptr<umf::MetadataSchema> schema = newStream.getSchema(TEST_SCHEMA_NAME);
        std::vector< std::shared_ptr<umf::MetadataDesc> > descs = schema->getAll();
        ASSERT_EQ(descs.size(), 1u);
        std::shared_ptr<umf::MetadataDesc> desc = descs[0];
        ASSERT_EQ(desc->getMetadataName(), TEST_PROPERTY_NAME2);
        newStream.close();
    }
}


TEST_F(TestRemovingSchema, RemoveOneAddSchema)
{
    {
        umf::MetadataStream newStream;
        newStream.open(TEST_FILE, umf::MetadataStream::Update);
        newStream.load();
        auto schemaForRemoving = newStream.getSchema(TEST_SCHEMA_NAME);
        newStream.remove(schemaForRemoving);
        newStream.save();
        newStream.close();

        std::shared_ptr<umf::MetadataSchema> newSchema(new umf::MetadataSchema(TEST_SCHEMA_NAME));
        VMF_METADATA_BEGIN(TEST_PROPERTY_NAME2);
            VMF_FIELD_INT(TEST_FIELD_NAME);
        VMF_METADATA_END(newSchema);

        newStream.reopen(umf::MetadataStream::Update);
        newStream.addSchema(newSchema);
        newStream.save();
        newStream.close();
    }

    {
        umf::MetadataStream newStream;
        newStream.open(TEST_FILE, umf::MetadataStream::ReadOnly);
        bool result = newStream.load(TEST_SCHEMA_NAME);
        ASSERT_TRUE( result );
        std::shared_ptr<umf::MetadataSchema> schema = newStream.getSchema(TEST_SCHEMA_NAME);
        std::vector< std::shared_ptr<umf::MetadataDesc> > descs = schema->getAll();
        ASSERT_EQ(descs.size(), 1u);
        std::shared_ptr<umf::MetadataDesc> desc = descs[0];
        ASSERT_EQ(desc->getMetadataName(), TEST_PROPERTY_NAME2);
        newStream.close();
    }
}


TEST_F(TestRemovingSchema, RemoveOneAddSchemaBeforeSaving)
{
    {
        umf::MetadataStream newStream;
        newStream.open(TEST_FILE, umf::MetadataStream::Update);
        newStream.load();
        auto schemaForRemoving = newStream.getSchema(TEST_SCHEMA_NAME);
        newStream.remove(schemaForRemoving);

        std::shared_ptr<umf::MetadataSchema> newSchema(new umf::MetadataSchema(TEST_SCHEMA_NAME));
        VMF_METADATA_BEGIN(TEST_PROPERTY_NAME2);
            VMF_FIELD_INT(TEST_FIELD_NAME);
        VMF_METADATA_END(newSchema);
        newStream.addSchema(newSchema);

        newStream.save();
        newStream.close();
    }

    {
        umf::MetadataStream newStream;
        newStream.open(TEST_FILE, umf::MetadataStream::ReadOnly);
        bool result = newStream.load(TEST_SCHEMA_NAME);
        ASSERT_TRUE( result );
        std::shared_ptr<umf::MetadataSchema> schema = newStream.getSchema(TEST_SCHEMA_NAME);
        std::vector< std::shared_ptr<umf::MetadataDesc> > descs = schema->getAll();
        ASSERT_EQ(descs.size(), 1u);
        std::shared_ptr<umf::MetadataDesc> desc = descs[0];
        ASSERT_EQ(desc->getMetadataName(), TEST_PROPERTY_NAME2);
        newStream.close();
    }
}


TEST_F(TestRemovingSchema, RemoveOneAddSchemaNoSaving)
{
    umf::MetadataStream newStream;
    newStream.open(TEST_FILE, umf::MetadataStream::Update);
    newStream.load();
    auto schemaForRemoving = newStream.getSchema(TEST_SCHEMA_NAME);
    newStream.remove(schemaForRemoving);

    std::shared_ptr<umf::MetadataSchema> newSchema(new umf::MetadataSchema(TEST_SCHEMA_NAME));
    VMF_METADATA_BEGIN(TEST_PROPERTY_NAME2);
        VMF_FIELD_INT(TEST_FIELD_NAME);
    VMF_METADATA_END(newSchema);
    newStream.addSchema(newSchema);

    std::shared_ptr<umf::MetadataSchema> schema = newStream.getSchema(TEST_SCHEMA_NAME);
    ASSERT_NE(schema, nullptr);
    std::vector< std::shared_ptr<umf::MetadataDesc> > descs = schema->getAll();
    ASSERT_EQ(descs.size(), 1u);
    std::shared_ptr<umf::MetadataDesc> desc = descs[0];
    ASSERT_EQ(desc->getMetadataName(), TEST_PROPERTY_NAME2);
    newStream.close();
}


TEST_F(TestRemovingSchema, RemovingWithReferences)
{
    {
        umf::MetadataStream newStream;
        newStream.open(TEST_FILE, umf::MetadataStream::Update);
        newStream.load();
        auto set1 = newStream.queryBySchema(TEST_SCHEMA_NAME);
        auto set2 = newStream.queryBySchema(TEST_SCHEMA_NAME_2);
        ASSERT_TRUE(!set1.empty());
        ASSERT_TRUE(!set2.empty());
        set1.at(0)->addReference(set2.at(0));
        set2.at(0)->addReference(set1.at(0));
        newStream.save();
        newStream.close();
    }

    {
        umf::MetadataStream newStream;
        newStream.open(TEST_FILE, umf::MetadataStream::Update);
        newStream.load();
        auto set1 = newStream.queryBySchema(TEST_SCHEMA_NAME);
        auto set2 = newStream.queryBySchema(TEST_SCHEMA_NAME_2);
        ASSERT_EQ(1u, set2.at(0)->getAllReferences().size());
        ASSERT_EQ(1u, set1.at(0)->getAllReferences().size());
        newStream.remove(schema);
        newStream.save();
        newStream.close();
    }

    {
        umf::MetadataStream newStream;
        newStream.open(TEST_FILE, umf::MetadataStream::ReadOnly);
        newStream.load();
        auto set1 = newStream.queryBySchema(TEST_SCHEMA_NAME);
        auto set2 = newStream.queryBySchema(TEST_SCHEMA_NAME_2);
        ASSERT_TRUE(set1.empty());
        ASSERT_FALSE(set2.empty());
        ASSERT_EQ(0u, set2.at(0)->getAllReferences().size());
        newStream.close();
    }
}
