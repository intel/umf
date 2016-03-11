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

    ASSERT_THROW(vmf::MetadataSchema::getStdSchemaName((vmf::MetadataSchema::StdSchemaKind)14), vmf::IncorrectParamException);
    ASSERT_THROW(vmf::MetadataSchema::getStdSchema((vmf::MetadataSchema::StdSchemaKind)14), vmf::IncorrectParamException);

    auto channels = stdSchema->getAll();
    EXPECT_EQ(channels.size(), 23u);
    EXPECT_EQ(channels[0]->getMetadataName(), "person");
    EXPECT_EQ(channels[0]->getFields().size(), 8u);
}

