// Copyright (C) 2014, Intel Corporation, all rights reserved.

#include "test_precomp.hpp"


TEST(TestStdSchema, GetName)
{
    std::string sn = vmf::MetadataSchema::getStdSchemaName();
    ASSERT_EQ(sn, "vmf://ns.intel.com/vmf/std-dst-schema-1.0");
}

TEST(TestStdSchema, GetSchema)
{
    auto stdSchema = vmf::MetadataSchema::getStdSchema();
    ASSERT_TRUE( (bool)stdSchema );

    EXPECT_EQ(stdSchema->getName(), vmf::MetadataSchema::getStdSchemaName());
    EXPECT_EQ(stdSchema->getAuthor(), "Intel Corporation");

    auto channels = stdSchema->getAll();
    EXPECT_EQ(channels.size(), 23);
    EXPECT_EQ(channels[0]->getMetadataName(), "person");
    EXPECT_EQ(channels[0]->getFields().size(), 8);
}

