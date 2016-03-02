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
#include "fstream"

enum SerializerType
{
    TypeXML = 0,
    TypeJson = 1
};

using namespace vmf;

class TestSerialization : public ::testing::TestWithParam< std::tuple<SerializerType, vmf_string> >
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

TEST_P(TestSerialization, Parse_schemasArray)
{
    SerializerType type = std::get<0>(GetParam());
    vmf_string compressorId = std::get<1>(GetParam());
    if (type == TypeXML)
    {
        writer.reset(new WriterCompressed(std::make_shared<XMLWriter>(), compressorId));
        reader.reset(new ReaderCompressed(std::make_shared<XMLReader>()));
    }
    else if (type == TypeJson)
    {
        writer.reset(new WriterCompressed(std::make_shared<JSONWriter>(), compressorId));
        reader.reset(new ReaderCompressed(std::make_shared<JSONReader>()));
    }

    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    schemas.push_back(spSchemaPeople);
    schemas.push_back(spSchemaFrames);
    std::string result = writer->store(schemas);

    reader->parseSchemas(result, schemas);
    ASSERT_EQ(2u, schemas.size());
    std::for_each(schemas.begin(), schemas.end(), [&] (const std::shared_ptr<MetadataSchema>& spSchema)
    {
        compareSchemas(stream.getSchema(spSchema->getName()), spSchema);
    });

    schemas.clear();
    std::shared_ptr< MetadataSchema > spSchemaNull = nullptr;
    schemas.emplace_back(spSchemaNull);
    schemas.push_back(spSchemaPeople);
    schemas.push_back(spSchemaFrames);
    ASSERT_THROW(writer->store(schemas), vmf::IncorrectParamException);
}

TEST_P(TestSerialization, Parse_schemasAll)
{
    SerializerType type = std::get<0>(GetParam());
    vmf_string compressorId = std::get<1>(GetParam());
    if (type == TypeXML)
    {
        writer.reset(new WriterCompressed(std::make_shared<XMLWriter>(), compressorId));
        reader.reset(new ReaderCompressed(std::make_shared<XMLReader>()));
    }
    else if (type == TypeJson)
    {
        writer.reset(new WriterCompressed(std::make_shared<JSONWriter>(), compressorId));
        reader.reset(new ReaderCompressed(std::make_shared<JSONReader>()));
    }
    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    schemas.push_back(spSchemaPeople);
    schemas.push_back(spSchemaFrames);
    std::string result = stream.serialize(*writer);

    reader->parseSchemas(result, schemas);

    ASSERT_EQ(2u, schemas.size());
    compareSchemas(stream.getSchema(schemas[0]->getName()), schemas[0]);
    compareSchemas(stream.getSchema(schemas[1]->getName()), schemas[1]);
}

TEST_P(TestSerialization, Parse_metadataArray)
{
    SerializerType type = std::get<0>(GetParam());
    vmf_string compressorId = std::get<1>(GetParam());
    if (type == TypeXML)
    {
        writer.reset(new WriterCompressed(std::make_shared<XMLWriter>(), compressorId));
        reader.reset(new ReaderCompressed(std::make_shared<XMLReader>()));
    }
    else if (type == TypeJson)
    {
        writer.reset(new WriterCompressed(std::make_shared<JSONWriter>(), compressorId));
        reader.reset(new ReaderCompressed(std::make_shared<JSONReader>()));
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
    SerializerType type = std::get<0>(GetParam());
    vmf_string compressorId = std::get<1>(GetParam());
    if (type == TypeXML)
    {
        writer.reset(new WriterCompressed(std::make_shared<XMLWriter>(), compressorId));
        reader.reset(new ReaderCompressed(std::make_shared<XMLReader>()));
    }
    else if (type == TypeJson)
    {
        writer.reset(new WriterCompressed(std::make_shared<JSONWriter>(), compressorId));
        reader.reset(new ReaderCompressed(std::make_shared<JSONReader>()));
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
    SerializerType type     = std::get<0>(GetParam());
    vmf_string compressorId = std::get<1>(GetParam());
    if (type == TypeXML)
    {
        writer.reset(new WriterCompressed(std::make_shared<XMLWriter>(), compressorId));
        reader.reset(new ReaderCompressed(std::make_shared<XMLReader>()));
    }
    else if (type == TypeJson)
    {
        writer.reset(new WriterCompressed(std::make_shared<JSONWriter>(), compressorId));
        reader.reset(new ReaderCompressed(std::make_shared<JSONReader>()));
    }

    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    schemas.push_back(spSchemaPeople);
    schemas.push_back(spSchemaFrames);
    std::string result = stream.serialize(*writer);

    MetadataStream testStream;
    testStream.deserialize(result, *reader);

    ASSERT_EQ(2u, testStream.getAllSchemaNames().size());
    compareSchemas(spSchemaPeople, testStream.getSchema(n_schemaPeople));
    compareSchemas(spSchemaFrames, testStream.getSchema(n_schemaFrames));

    ASSERT_EQ(set.size(), testStream.getAll().size());
    std::for_each(set.begin(), set.end(), [&] (const std::shared_ptr<Metadata>& spItem)
    {
        compareMetadata(spItem, testStream.getById(spItem->getId()) );
    });
}


TEST_P(TestSerialization, CheckIgnoreUnknownCompressor)
{
    SerializerType type = std::get<0>(GetParam());

    vmf_string compressorId = "unknown_compressor";
    std::shared_ptr<Compressor> fake = std::make_shared<FakeCompressor>();
    std::dynamic_pointer_cast<FakeCompressor>(fake)->setId(compressorId);
    vmf::Compressor::registerNew(fake);

    if (type == TypeXML)
    {
        writer.reset(new WriterCompressed(std::make_shared<XMLWriter>(), compressorId));
        reader.reset(new ReaderCompressed(std::make_shared<XMLReader>(), true));
    }
    else if (type == TypeJson)
    {
        writer.reset(new WriterCompressed(std::make_shared<JSONWriter>(), compressorId));
        reader.reset(new ReaderCompressed(std::make_shared<JSONReader>(), true));
    }

    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    schemas.push_back(spSchemaPeople);
    schemas.push_back(spSchemaFrames);
    std::string result = stream.serialize(*writer);

    vmf::Compressor::unregister(compressorId);

    reader->parseSchemas(result, schemas);

    ASSERT_EQ(1u, schemas.size());
    ASSERT_EQ("com.intel.vmf.compressed-metadata", schemas[0]->getName());
}


//don't check for incorrect compressors
INSTANTIATE_TEST_CASE_P(UnitTest, TestSerialization,
                        ::testing::Combine(::testing::Values(TypeXML, TypeJson),
                                           ::testing::Values("com.intel.vmf.compressor.zlib", "")));
