// Copyright (C) 2014, Intel Corporation, all rights reserved.

#include "test_precomp.hpp"
#include "fstream"

enum SerializerType
{
    TypeXML = 0,
    TypeJson = 1
};

using namespace vmf;

class TestSerialization : public ::testing::TestWithParam<SerializerType>
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
        vRefDescsPeople.emplace_back(std::make_shared<ReferenceDesc>("friend", true));
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

    MetadataStream stream;
    MetadataSet set;

    std::unique_ptr<IWriter> writer;
    std::unique_ptr<IReader> reader;
    std::shared_ptr< MetadataSchema > spSchemaPeople, spSchemaFrames;
    std::shared_ptr< MetadataDesc > spDescPeople, spDescFrames;
    std::vector< FieldDesc > vFieldsPeople, vFieldsFrames;
    std::vector<std::shared_ptr<ReferenceDesc>> vRefDescsPeople, vRefDescsFrames;

    vmf_string n_schemaPeople, n_schemaFrames;
};

/*
TEST_P(TestSerialization, Parse_schema)
{
    SerializerType type = GetParam();
    if(type == TypeXML)
    {
        writer.reset(new XMLWriter());
        reader.reset(new XMLReader());
    }
    else if(type == TypeJson)
    {
        writer.reset(new JSONWriter());
        reader.reset(new JSONReader());
    }

    std::string result = writer->store(spSchemaPeople);

    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    reader->parseSchemas(result, schemas);
    ASSERT_EQ(1, schemas.size());
    compareSchemas(spSchemaPeople, schemas[0]);
}
*/

TEST_P(TestSerialization, Parse_schemasArray)
{
    SerializerType type = GetParam();
    if (type == TypeXML)
    {
        writer.reset(new XMLWriter());
        reader.reset(new XMLReader());
    }
    else if (type == TypeJson)
    {
        writer.reset(new JSONWriter());
        reader.reset(new JSONReader());
    }

    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    schemas.push_back(spSchemaPeople);
    schemas.push_back(spSchemaFrames);
    std::string result = writer->store(schemas);

    reader->parseSchemas(result, schemas);
    ASSERT_EQ(2, schemas.size());
    std::for_each(schemas.begin(), schemas.end(), [&] (const std::shared_ptr<MetadataSchema>& spSchema)
    {
        compareSchemas(stream.getSchema(spSchema->getName()), spSchema);
    });
}

TEST_P(TestSerialization, Parse_schemasAll)
{
    SerializerType type = GetParam();
    if (type == TypeXML)
    {
        writer.reset(new XMLWriter());
        reader.reset(new XMLReader());
    }
    else if (type == TypeJson)
    {
        writer.reset(new JSONWriter());
        reader.reset(new JSONReader());
    }
    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    schemas.push_back(spSchemaPeople);
    schemas.push_back(spSchemaFrames);
    std::string result = stream.serialize(*writer);

    reader->parseSchemas(result, schemas);

    ASSERT_EQ(2, schemas.size());
    compareSchemas(stream.getSchema(schemas[0]->getName()), schemas[0]);
    compareSchemas(stream.getSchema(schemas[1]->getName()), schemas[1]);
}

/*
TEST_P(TestSerialization, Parse_metadata)
{
    SerializerType type = GetParam();
    if (type == TypeXML)
    {
        writer.reset(new XMLWriter());
        reader.reset(new XMLReader());
    }
    else if (type == TypeJson)
    {
        writer.reset(new JSONWriter());
        reader.reset(new JSONReader());
    }

    auto item = stream.getAll()[0];
    std::string result = writer->store(item);

    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    schemas.push_back(spSchemaFrames);
    schemas.push_back(spSchemaPeople);
    std::vector<std::shared_ptr<MetadataInternal>> md;
    reader->parseMetadata(result, schemas, md);
    ASSERT_EQ(1, md.size());
    compareMetadata(item, md[0], false);
    ASSERT_EQ(item->getAllReferences().size(), md[0]->vRefs.size());
}
*/

TEST_P(TestSerialization, Parse_metadataArray)
{
    SerializerType type = GetParam();
    if (type == TypeXML)
    {
        writer.reset(new XMLWriter());
        reader.reset(new XMLReader());
    }
    else if (type == TypeJson)
    {
        writer.reset(new JSONWriter());
        reader.reset(new JSONReader());
    }

    std::string result = writer->store(set);

    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    schemas.push_back(spSchemaFrames);
    schemas.push_back(spSchemaPeople);
    std::vector<std::shared_ptr<MetadataInternal>> md;
    reader->parseMetadata(result, schemas, md);
    ASSERT_EQ(set.size(), md.size());

    MetadataStream testStream;
    testStream.addSchema(spSchemaFrames);
    testStream.addSchema(spSchemaPeople);
    for(auto item = md.begin(); item != md.end(); item++)
        testStream.add(*item);

    ASSERT_EQ(set.size(), testStream.getAll().size());
    std::for_each(set.begin(), set.end(), [&] (const std::shared_ptr<Metadata>& spItem)
    {
        compareMetadata(spItem, testStream.getById(spItem->getId()) );
    });
}

TEST_P(TestSerialization, Parse_metadataAll)
{
    SerializerType type = GetParam();
    if (type == TypeXML)
    {
        writer.reset(new XMLWriter());
        reader.reset(new XMLReader());
    }
    else if (type == TypeJson)
    {
        writer.reset(new JSONWriter());
        reader.reset(new JSONReader());
    }

    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    schemas.push_back(spSchemaPeople);
    schemas.push_back(spSchemaFrames);
    std::string result = stream.serialize(*writer);

    std::vector<std::shared_ptr<MetadataInternal>> md;
    reader->parseMetadata(result, schemas, md);
    ASSERT_EQ(set.size(), md.size());

    MetadataStream testStream;
    testStream.addSchema(spSchemaFrames);
    testStream.addSchema(spSchemaPeople);
    for(auto item = md.begin(); item != md.end(); item++)
        testStream.add(*item);

    ASSERT_EQ(set.size(), testStream.getAll().size());
    std::for_each(set.begin(), set.end(), [&] (const std::shared_ptr<Metadata>& spItem)
    {
        compareMetadata(spItem, testStream.getById(spItem->getId()) );
    });
}

TEST_P(TestSerialization, Parse_All)
{
    SerializerType type = GetParam();
    if (type == TypeXML)
    {
        writer.reset(new XMLWriter());
        reader.reset(new XMLReader());
    }
    else if (type == TypeJson)
    {
        writer.reset(new JSONWriter());
        reader.reset(new JSONReader());
    }

    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    schemas.push_back(spSchemaPeople);
    schemas.push_back(spSchemaFrames);
    std::string result = stream.serialize(*writer);

    MetadataStream testStream;
    testStream.deserialize(result, *reader);

    ASSERT_EQ(2, testStream.getAllSchemaNames().size());
    compareSchemas(spSchemaPeople, testStream.getSchema(n_schemaPeople));
    compareSchemas(spSchemaFrames, testStream.getSchema(n_schemaFrames));

    ASSERT_EQ(set.size(), testStream.getAll().size());
    std::for_each(set.begin(), set.end(), [&] (const std::shared_ptr<Metadata>& spItem)
    {
        compareMetadata(spItem, testStream.getById(spItem->getId()) );
    });
}

INSTANTIATE_TEST_CASE_P(UnitTest, TestSerialization, ::testing::Values(TypeXML, TypeJson) );
