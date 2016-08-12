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


class TestMetadataSchema : public ::testing::Test
{
protected:
    void SetUp()
    {
        SCHEMA_NAME = "vmf:http://ns.intel.com/vmf/1.0";

        EXPECT_NO_THROW(spSchema = std::shared_ptr< umf::MetadataSchema >( new umf::MetadataSchema( SCHEMA_NAME )));

        vFields.emplace_back( umf::FieldDesc( "name", umf::Variant::type_string ));
        vFields.emplace_back( umf::FieldDesc( "age", umf::Variant::type_integer ));
        vFields.emplace_back( umf::FieldDesc( "sex", umf::Variant::type_integer ));
        vFields.emplace_back( umf::FieldDesc( "email", umf::Variant::type_string ));

        spDesc = std::shared_ptr< umf::MetadataDesc >( new umf::MetadataDesc( "people", vFields ));
    }

    std::shared_ptr< umf::MetadataSchema > spSchema;
    std::vector< umf::FieldDesc > vFields;
    std::shared_ptr< umf::MetadataDesc > spDesc;

    umf::umf_string SCHEMA_NAME;
};

TEST_F(TestMetadataSchema, AddSchema)
{
    EXPECT_NO_THROW(spSchema->add( spDesc ));
    ASSERT_EQ(spSchema->size(), (size_t) 1);
    ASSERT_EQ(spSchema->getName(), SCHEMA_NAME);
}

TEST_F(TestMetadataSchema, AddTheSameDesc)
{
    EXPECT_NO_THROW(spSchema->add( spDesc ));
    EXPECT_THROW(spSchema->add(spDesc), std::exception);
}

TEST_F(TestMetadataSchema, AddNullDesc)
{
    std::shared_ptr< umf::MetadataDesc > emptyPtr;
    EXPECT_THROW(spSchema->add(emptyPtr), std::exception);
}

TEST_F(TestMetadataSchema, CreateSchemaEmptyName)
{
    EXPECT_THROW(spSchema = std::shared_ptr< umf::MetadataSchema >( new umf::MetadataSchema( umf::umf_string() )), umf::IncorrectParamException);
}

TEST_F(TestMetadataSchema, FindDesc)
{
    EXPECT_NO_THROW(spSchema->add( spDesc ));
    std::shared_ptr< umf::MetadataDesc > findPtr;
    EXPECT_NO_THROW(findPtr = spSchema->findMetadataDesc("people"));
    ASSERT_NE(findPtr, nullptr);
    ASSERT_EQ(findPtr, spDesc);
}

TEST_F(TestMetadataSchema, FindUnknownDesc)
{
    EXPECT_NO_THROW(spSchema->add( spDesc ));
    std::shared_ptr< umf::MetadataDesc > findPtr;
    EXPECT_NO_THROW(findPtr = spSchema->findMetadataDesc("some desc"));
    ASSERT_EQ(findPtr, nullptr);
}
