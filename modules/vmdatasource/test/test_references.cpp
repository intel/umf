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
#include "test_precomp.hpp"
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
using namespace std;

class TestNamedReferences : public ::testing::Test
{
protected:

    virtual void SetUp()
    {
        copyFile(TEST_FILE_SRC, TEST_FILE);

        initialize();

        spSchema = make_shared<MetadataSchema>("TEST_SCHEMA_NAME");

        vFieldDescs1.emplace_back(FieldDesc("AGE", Variant::type_integer));
        vFieldDescs1.emplace_back(FieldDesc("HEIGHT", Variant::type_real));
        vFieldDescs1.emplace_back(FieldDesc("SURNAME", Variant::type_string));
        vFieldDescs1.emplace_back(FieldDesc("NAME", Variant::type_string));

        vspReferenceDescs1.emplace_back(make_shared<ReferenceDesc>("FRIEND"));
        vspReferenceDescs1.emplace_back(make_shared<ReferenceDesc>("COLLEAGUE", false, true));
        vspReferenceDescs1.emplace_back(make_shared<ReferenceDesc>("SPOUSE", true));
        vspReferenceDescs1.emplace_back(make_shared<ReferenceDesc>("CAR"));

        spMDesc1 = make_shared<MetadataDesc>("PERSON", vFieldDescs1, vspReferenceDescs1);
        spSchema->add(spMDesc1);

        vFieldDescs2.emplace_back(FieldDesc("AGE", Variant::type_integer));
        vFieldDescs2.emplace_back(FieldDesc("NUMBER", Variant::type_string));
        vFieldDescs2.emplace_back(FieldDesc("MODEL", Variant::type_string));

        vspReferenceDescs2.emplace_back(make_shared<ReferenceDesc>("OWNER", true));
        vspReferenceDescs2.emplace_back(make_shared<ReferenceDesc>("OWNERS_SPOUSE", true));
        vspReferenceDescs2.emplace_back(make_shared<ReferenceDesc>("MODEL"));

        spMDesc2 = make_shared<MetadataDesc>("CAR", vFieldDescs2, vspReferenceDescs2);
        spSchema->add(spMDesc2);
       
        if (!stream.open(TEST_FILE, vmf::MetadataStream::Update))
            cout << "Cann't open stream!" << endl;

        stream.addSchema(spSchema);

        std::shared_ptr<vmf::Metadata> md1(new Metadata(spSchema->findMetadataDesc("PERSON")));
        std::shared_ptr<vmf::Metadata> md2(new Metadata(spSchema->findMetadataDesc("PERSON")));
        std::shared_ptr<vmf::Metadata> md3(new Metadata(spSchema->findMetadataDesc("CAR")));

        md1->setFieldValue("AGE", 25);
        md1->setFieldValue("HEIGHT", 180.5);
        md1->setFieldValue("SURNAME", "Smith");
        md1->setFieldValue("NAME", "Denis");
        stream.add(md1);
        
        md2->setFieldValue("AGE", 20);
        md2->setFieldValue("HEIGHT", 160.0);
        md2->setFieldValue("SURNAME", "Smith");
        md2->setFieldValue("NAME", "Anna");
        stream.add(md2);

        md3->setFieldValue("AGE", 3);
        md3->setFieldValue("NUMBER", "a333aa");
        md3->setFieldValue("MODEL", "magic");
        
        stream.add(md3);

        md2->addReference(md2);
        md2->addReference(md1, "SPOUSE");
        md2->addReference(md1, "COLLEAGUE");

        md3->addReference(md3);
        md3->addReference(md1, "OWNER");
        md3->addReference(md2, "OWNERS_SPOUSE");

        stream.save();
        stream.close();
        stream.clear();
    }

    virtual void TearDown()
    {
        //vmf::terminate();
    }

    MetadataStream stream;
    shared_ptr<MetadataSchema> spSchema;
    shared_ptr<MetadataDesc> spMDesc1, spMDesc2, spMDesc3;
    vector<std::shared_ptr<vmf::ReferenceDesc>> vspReferenceDescs1, vspReferenceDescs2, vspReferenceDescs3;
    vector<vmf::FieldDesc> vFieldDescs1, vFieldDescs2, vFieldDescs3;
};

TEST_F(TestNamedReferences, SaveLoad)
{
    vmf::MetadataStream newStream;
    newStream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
    newStream.load();

    auto md = newStream.getAll();
    EXPECT_EQ(3u, md.size());

    auto refs = md[0]->getAllReferences();
    EXPECT_EQ(0u, refs.size());

    refs = md[1]->getAllReferences();
    EXPECT_EQ(3u, refs.size());

    refs = md[2]->getAllReferences();
    EXPECT_EQ(3u, refs.size());

    md[0]->addReference(md[2], "CAR");
    md[0]->addReference(md[0]);

    refs = md[0]->getAllReferences();
    EXPECT_EQ(2u, refs.size());

    newStream.save();
    newStream.load();

    refs = md[0]->getAllReferences();
    EXPECT_EQ(2u, refs.size());

    auto mdSet = md[1]->getReferencesByName("COLLEAGUE");
    EXPECT_EQ(1u, mdSet.size());

    newStream.close();
}

TEST_F(TestNamedReferences, AddReferences)
{
    vmf::MetadataStream newStream;
    newStream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
    newStream.load();

    auto md = newStream.getAll();
    EXPECT_EQ(3u, md.size());

    auto refs = md[0]->getAllReferences();
    EXPECT_EQ(0u, refs.size());

    EXPECT_THROW(md[0]->addReference(nullptr, "CAR"), NullPointerException);

    md[0]->addReference(md[2], "CAR");
    md[0]->addReference(md[1], "SPOUSE");
    md[0]->addReference(md[0]);
    md[0]->addReference(md[1], "CAR");
    md[0]->addReference(md[1], "FRIEND");
    md[0]->addReference(md[0], "FRIEND");
    EXPECT_THROW(md[0]->addReference(md[1], "SPOUSE"), vmf::IncorrectParamException);
    EXPECT_THROW(md[0]->addReference(md[2], "MODEL"), vmf::IncorrectParamException);
    
    refs = md[0]->getAllReferences();
    EXPECT_EQ(6u, refs.size());

    auto mdSet = md[0]->getReferencesByName("");
    EXPECT_EQ(1u, mdSet.size());
    
    mdSet = md[0]->getReferencesByName("SPOUSE");
    EXPECT_EQ(1u, mdSet.size());

    mdSet = md[0]->getReferencesByName("CAR");
    EXPECT_EQ(2u, mdSet.size());

    mdSet = md[0]->getReferencesByName("MODEL");
    EXPECT_EQ(0u, mdSet.size());

    mdSet = md[0]->getReferencesByName("FRIEND");
    EXPECT_EQ(2u, mdSet.size());

    mdSet = md[0]->getReferencesByMetadata("PERSON");
    EXPECT_EQ(5u, mdSet.size());

    EXPECT_THROW(md[0]->addReference(md[2], "SPOUSE"), vmf::IncorrectParamException);

    mdSet = md[0]->getReferencesByMetadata("PERSON");
    EXPECT_EQ(5u, mdSet.size());

    mdSet = md[0]->getReferencesByMetadata("CAR");
    EXPECT_EQ(1u, mdSet.size());

    newStream.save();
    newStream.load();
    refs = md[0]->getAllReferences();
    EXPECT_EQ(6u, refs.size());

    newStream.close();
}

TEST_F(TestNamedReferences, QueryByReference)
{
    vmf::MetadataStream newStream;
    newStream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
    newStream.load();

    vmf::MetadataSet set = newStream.queryByReference([&](const std::shared_ptr< vmf::Metadata >& spItem, const std::shared_ptr< vmf::Metadata >& spReferenceMd)->bool
    {
        return (spItem->getName() == spReferenceMd->getName());
    });

    ASSERT_EQ(set.size(), 2);
    newStream.close();
}

TEST_F(TestNamedReferences, RemoveReferences)
{
    vmf::MetadataStream newStream;
    newStream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
    newStream.load();

    auto md = newStream.getAll();
    EXPECT_EQ(3u, md.size());

    ASSERT_FALSE(md[0]->isReference(2, "SPOUSE"));

    ASSERT_TRUE(md[1]->isReference(md[0], "SPOUSE"));
    ASSERT_TRUE(md[1]->isReference(0, "SPOUSE"));
    ASSERT_TRUE(md[1]->isReference(1));
    ASSERT_TRUE(md[1]->isReference(md[1]));

    md[1]->removeReference(1);
    md[1]->removeReference(md[0], "SPOUSE");

    ASSERT_FALSE(md[1]->isReference(1));
    ASSERT_FALSE(md[1]->isReference(md[0], "SPOUSE"));

    auto mdSet = md[1]->getAllReferences();
    EXPECT_EQ(1u, mdSet.size());

    ASSERT_TRUE(md[2]->isReference(md[2]));
    ASSERT_TRUE(md[2]->isReference(md[0], "OWNER"));
    ASSERT_TRUE(md[2]->isReference(1, "OWNERS_SPOUSE"));

    md[2]->removeReference(0, "OWNER");
    md[2]->removeReference(md[2]);

    ASSERT_FALSE(md[2]->isReference(md[2]));
    ASSERT_FALSE(md[2]->isReference(md[0], "OWNER"));

    mdSet = md[2]->getAllReferences();
    EXPECT_EQ(1u, mdSet.size());

    newStream.close();
}


class TestSaveLoadReference : public ::testing::Test
{
protected:
    void SetUp()
    {
        TEST_SCHEMA_NAME_0 = "TEST_SCHEMA_NAME_0";
        TEST_SCHEMA_NAME_1 = "TEST_SCHEMA_NAME_1";
        TEST_DESC_NAME_0 = "TEST_DESC_NAME_0";
        TEST_DESC_NAME_1 = "TEST_DESC_NAME_1";
        TEST_STRING_VAL = "TEST_STRING_VAL";
        TEST_INTEGER_VAL = 0xDEADBEEF;

        copyFile(TEST_FILE_SRC, TEST_FILE);

        //vmf::initialize();

        schema[0] = std::shared_ptr<vmf::MetadataSchema>(new vmf::MetadataSchema(TEST_SCHEMA_NAME_0));
        schema[1] = std::shared_ptr<vmf::MetadataSchema>(new vmf::MetadataSchema(TEST_SCHEMA_NAME_1));

        desc[0] = std::shared_ptr<vmf::MetadataDesc>(new vmf::MetadataDesc(TEST_DESC_NAME_0, vmf::Variant::type_string));
        desc[1] = std::shared_ptr<vmf::MetadataDesc>(new vmf::MetadataDesc(TEST_DESC_NAME_1, vmf::Variant::type_integer));

        schema[0]->add(desc[0]);
        schema[1]->add(desc[1]);
    }

    void TearDown()
    {
        //vmf::terminate();
    }

    std::shared_ptr<vmf::MetadataSchema> schema[2];
    std::shared_ptr<vmf::MetadataDesc> desc[2];
    vmf::IdType id[2];

    vmf::vmf_string TEST_SCHEMA_NAME_0;
    vmf::vmf_string TEST_SCHEMA_NAME_1;
    vmf::vmf_string TEST_DESC_NAME_0;
    vmf::vmf_string TEST_DESC_NAME_1;
    vmf::vmf_string TEST_STRING_VAL;
    vmf::vmf_integer TEST_INTEGER_VAL;
};

TEST_F(TestSaveLoadReference, SimpleReference)
{
    {
        std::shared_ptr<vmf::Metadata> metadata[2];
        metadata[0] = std::shared_ptr<vmf::Metadata>(new vmf::Metadata(desc[0]));
        metadata[1] = std::shared_ptr<vmf::Metadata>(new vmf::Metadata(desc[1]));
        metadata[0]->addValue(TEST_STRING_VAL);
        metadata[1]->addValue(TEST_INTEGER_VAL);

        vmf::MetadataStream stream;
        stream.addSchema(schema[0]);
        stream.addSchema(schema[1]);
        stream.add(metadata[0]);
        stream.add(metadata[1]);
        metadata[0]->addReference(metadata[1]);
        stream.saveTo(TEST_FILE);

        id[0] = metadata[0]->getId();
        id[1] = metadata[1]->getId();
    }

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load(TEST_SCHEMA_NAME_0);
        stream.close();

        vmf::MetadataSet schemaSet[2];
        schemaSet[0] = stream.queryBySchema(TEST_SCHEMA_NAME_0);
        schemaSet[1] = stream.queryBySchema(TEST_SCHEMA_NAME_1);
        vmf::MetadataSet descSet[2];
        descSet[0] = schemaSet[0].queryByName(TEST_DESC_NAME_0);
        descSet[1] = schemaSet[1].queryByName(TEST_DESC_NAME_1);
        std::shared_ptr< vmf::Metadata > metadata[2];
        metadata[0] = descSet[0].at(0);
        metadata[1] = descSet[1].at(0);
        ASSERT_TRUE(metadata[0]->isReference(id[1]));
        ASSERT_FALSE(metadata[1]->isReference(id[0]));
        ASSERT_FALSE(metadata[1]->isReference(id[1]));
        ASSERT_FALSE(metadata[0]->isReference(id[0]));
    }
}

TEST_F(TestSaveLoadReference, RecursiveReference)
{
    {
        std::shared_ptr<vmf::Metadata> metadata[2];
        metadata[0] = std::shared_ptr<vmf::Metadata>(new vmf::Metadata(desc[0]));
        metadata[1] = std::shared_ptr<vmf::Metadata>(new vmf::Metadata(desc[1]));
        metadata[0]->addValue(TEST_STRING_VAL);
        metadata[1]->addValue(TEST_INTEGER_VAL);

        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.addSchema(schema[0]);
        stream.addSchema(schema[1]);
        stream.add(metadata[0]);
        stream.add(metadata[1]);

        metadata[0]->addReference(metadata[1]);
        metadata[1]->addReference(metadata[0]);
        id[0] = metadata[0]->getId();
        id[1] = metadata[1]->getId();

        stream.save();
        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load(TEST_SCHEMA_NAME_0);
        stream.close();

        vmf::MetadataSet schemaSet[2];
        schemaSet[0] = stream.queryBySchema(TEST_SCHEMA_NAME_0);
        schemaSet[1] = stream.queryBySchema(TEST_SCHEMA_NAME_1);
        vmf::MetadataSet descSet[2];
        descSet[0] = schemaSet[0].queryByName(TEST_DESC_NAME_0);
        descSet[1] = schemaSet[1].queryByName(TEST_DESC_NAME_1);
        std::shared_ptr< vmf::Metadata > metadata[2];
        metadata[0] = descSet[0].at(0);
        metadata[1] = descSet[1].at(0);
        ASSERT_TRUE(metadata[0]->isReference(id[1]));
        ASSERT_TRUE(metadata[1]->isReference(id[0]));
        ASSERT_FALSE(metadata[1]->isReference(id[1]));
        ASSERT_FALSE(metadata[0]->isReference(id[0]));
    }
}

TEST_F(TestSaveLoadReference, ToItselfReference)
{
    {
        std::shared_ptr<vmf::Metadata> metadata[2];
        metadata[0] = std::shared_ptr<vmf::Metadata>(new vmf::Metadata(desc[0]));
        metadata[1] = std::shared_ptr<vmf::Metadata>(new vmf::Metadata(desc[1]));
        metadata[0]->addValue(TEST_STRING_VAL);
        metadata[1]->addValue(TEST_INTEGER_VAL);

        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.addSchema(schema[0]);
        stream.addSchema(schema[1]);
        stream.add(metadata[0]);
        stream.add(metadata[1]);

        metadata[0]->addReference(metadata[0]);
        metadata[1]->addReference(metadata[1]);
        id[0] = metadata[0]->getId();
        id[1] = metadata[1]->getId();

        stream.save();
        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load();
        stream.close();

        vmf::MetadataSet schemaSet[2];
        schemaSet[0] = stream.queryBySchema(TEST_SCHEMA_NAME_0);
        schemaSet[1] = stream.queryBySchema(TEST_SCHEMA_NAME_1);
        vmf::MetadataSet descSet[2];
        descSet[0] = schemaSet[0].queryByName(TEST_DESC_NAME_0);
        descSet[1] = schemaSet[1].queryByName(TEST_DESC_NAME_1);
        std::shared_ptr< vmf::Metadata > metadata[2];
        metadata[0] = descSet[0].at(0);
        metadata[1] = descSet[1].at(0);
        ASSERT_FALSE(metadata[0]->isReference(id[1]));
        ASSERT_FALSE(metadata[1]->isReference(id[0]));
        ASSERT_TRUE(metadata[1]->isReference(id[1]));
        ASSERT_TRUE(metadata[0]->isReference(id[0]));
    }
}

TEST_F(TestSaveLoadReference, InsideOnePropertyReference)
{
    const vmf::vmf_integer TEST_INTEGER_VAL_0(0xDEADBEEF);
    const vmf::vmf_integer TEST_INTEGER_VAL_1(0xC0FFEE);
    {
        std::shared_ptr<vmf::Metadata> metadata[2];
        metadata[0] = std::shared_ptr<vmf::Metadata>(new vmf::Metadata(desc[1]));
        metadata[1] = std::shared_ptr<vmf::Metadata>(new vmf::Metadata(desc[1]));
        metadata[0]->addValue(TEST_INTEGER_VAL_0);
        metadata[1]->addValue(TEST_INTEGER_VAL_1);

        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.addSchema(schema[1]);
        stream.add(metadata[0]);
        stream.add(metadata[1]);

        metadata[0]->addReference(metadata[1]);
        metadata[1]->addReference(metadata[0]);
        id[0] = metadata[0]->getId();
        id[1] = metadata[1]->getId();

        stream.save();
        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load(TEST_SCHEMA_NAME_1, TEST_DESC_NAME_1);
        stream.close();

        std::shared_ptr<vmf::Metadata> metadata[2];
        metadata[0] = stream.getById(id[0]);
        metadata[1] = stream.getById(id[1]);
        ASSERT_TRUE(metadata[0]->isReference(id[1]));
        ASSERT_TRUE(metadata[1]->isReference(id[0]));
        ASSERT_FALSE(metadata[1]->isReference(id[1]));
        ASSERT_FALSE(metadata[0]->isReference(id[0]));
    }
}

TEST_F(TestSaveLoadReference, SeveralReferences)
{
    {
        std::shared_ptr<vmf::Metadata> metadata[2];
        metadata[0] = std::shared_ptr<vmf::Metadata>(new vmf::Metadata(desc[0]));
        metadata[1] = std::shared_ptr<vmf::Metadata>(new vmf::Metadata(desc[1]));
        metadata[0]->addValue(TEST_STRING_VAL);
        metadata[1]->addValue(TEST_INTEGER_VAL);

        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.addSchema(schema[0]);
        stream.addSchema(schema[1]);
        stream.add(metadata[0]);
        stream.add(metadata[1]);

        metadata[0]->addReference(metadata[0]);
        metadata[0]->addReference(metadata[1]);
        id[0] = metadata[0]->getId();
        id[1] = metadata[1]->getId();

        stream.save();
        stream.close();
    }

    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::ReadOnly);
        stream.load(TEST_SCHEMA_NAME_0);
        stream.close();

        vmf::MetadataSet schemaSet[2];
        schemaSet[0] = stream.queryBySchema(TEST_SCHEMA_NAME_0);
        schemaSet[1] = stream.queryBySchema(TEST_SCHEMA_NAME_1);
        vmf::MetadataSet descSet[2];
        descSet[0] = schemaSet[0].queryByName(TEST_DESC_NAME_0);
        descSet[1] = schemaSet[1].queryByName(TEST_DESC_NAME_1);
        std::shared_ptr< vmf::Metadata > metadata[2];
        metadata[0] = descSet[0].at(0);
        metadata[1] = descSet[1].at(0);
        ASSERT_TRUE(metadata[0]->isReference(id[1]));
        ASSERT_FALSE(metadata[1]->isReference(id[0]));
        ASSERT_FALSE(metadata[1]->isReference(id[1]));
        ASSERT_TRUE(metadata[0]->isReference(id[0]));
    }
}

TEST_F(TestSaveLoadReference, OneToOneReference)
{
    const vmf::vmf_string atom(TEST_STRING_VAL_1);
    const vmf::vmf_string atom(TEST_STRING_VAL_2);
    {
        schema[0]->add(desc[1]);

        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.addSchema(schema[0]);

        std::shared_ptr<vmf::Metadata> metadata, intMetadata;

        intMetadata.reset(new vmf::Metadata(desc[1]));
        intMetadata->addValue(TEST_INTEGER_VAL);
        stream.add(intMetadata);

        metadata.reset(new vmf::Metadata(desc[0]));
        metadata->addValue(TEST_STRING_VAL);
        stream.add(metadata);

        metadata.reset(new vmf::Metadata(desc[0]));
        metadata->addValue(TEST_STRING_VAL_1);
        stream.add(metadata);

        metadata.reset(new vmf::Metadata(desc[0]));
        metadata->addValue(TEST_STRING_VAL_2);
        stream.add(metadata);
        intMetadata->addReference(metadata);

        stream.save();
        stream.close();
    }
    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.load(TEST_SCHEMA_NAME_0, TEST_DESC_NAME_1);
        stream.save();
        stream.close();
    }
    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.load();
        stream.close();
        EXPECT_EQ(1u, stream.queryByName(TEST_DESC_NAME_1).size());
        EXPECT_EQ(3u, stream.queryByName(TEST_DESC_NAME_0).size());
    }
}

TEST_F(TestSaveLoadReference, CheckReferenceStream)
{
    const vmf::vmf_string atom(TEST_STRING_VAL_1);
    {
        std::shared_ptr<vmf::Metadata> metadata[2];
        metadata[0] = std::shared_ptr<vmf::Metadata>(new vmf::Metadata(desc[0]));
        metadata[1] = std::shared_ptr<vmf::Metadata>(new vmf::Metadata(desc[0]));
        metadata[0]->addValue(TEST_STRING_VAL);
        metadata[1]->addValue(TEST_STRING_VAL_1);

        vmf::MetadataStream stream1, stream2;
        stream1.addSchema(schema[0]);
        stream2.addSchema(schema[0]);
        stream1.add(metadata[0]);
        stream2.add(metadata[1]);
        metadata[0]->addReference(metadata[0], "");
        EXPECT_THROW(metadata[0]->addReference(metadata[1], ""), vmf::IncorrectParamException);
    }
}

TEST_F(TestSaveLoadReference, NonSimpleReferencesOrder)
{
    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);

        stream.addSchema(schema[1]);
        for (int i = 0; i < 10; i++)
        {
            std::shared_ptr<vmf::Metadata> md(new vmf::Metadata(desc[1]));
            md->addValue(i);
            stream.add(md);
        }

        auto item2 = stream.getById(2);
        auto item4 = stream.getById(4);
        auto item5 = stream.getById(5);

        item2->addReference(item5);
        item5->addReference(item4);

        stream.save();
        stream.close();
    }
    {
        vmf::MetadataStream stream;
        stream.open(TEST_FILE, vmf::MetadataStream::Update);
        stream.load(TEST_SCHEMA_NAME_1);

        auto item2 = stream.getById(2);
        auto item5 = stream.getById(5);

        ASSERT_EQ(item2->getFirstReference(TEST_DESC_NAME_1)->getId(), 5);
        ASSERT_EQ(item5->getFirstReference(TEST_DESC_NAME_1)->getId(), 4);

        ASSERT_TRUE(stream.save());

        stream.close();
    }
}
