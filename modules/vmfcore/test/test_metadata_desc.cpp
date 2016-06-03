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

class TestMetadataDesc : public ::testing::Test
{
protected:
    void SetUp()
    {
        vFields.emplace_back( vmf::FieldDesc( "name", vmf::Variant::type_string ));
        vFields.emplace_back( vmf::FieldDesc( "age", vmf::Variant::type_integer ));
        vFields.emplace_back( vmf::FieldDesc( "sex", vmf::Variant::type_integer ));
        vFields.emplace_back( vmf::FieldDesc( "email", vmf::Variant::type_string ));
    }

    std::vector< vmf::FieldDesc > vFields;
    std::shared_ptr< vmf::MetadataDesc > spDesc;
};

TEST_F(TestMetadataDesc, CreateWithFields)
{
    EXPECT_NO_THROW(spDesc = std::shared_ptr< vmf::MetadataDesc >(new vmf::MetadataDesc( "people", vFields )));
    auto descFields = spDesc->getFields();
    ASSERT_EQ(descFields.size(), vFields.size());
    ASSERT_EQ(spDesc->getMetadataName(), "people");
    ASSERT_EQ(spDesc->getSchemaName(), vmf::vmf_string());
}

TEST_F(TestMetadataDesc, CreateWithFieldsIncorrect)
{
    vFields.emplace_back( vmf::FieldDesc( "sex", vmf::Variant::type_string ));
    EXPECT_THROW(spDesc = std::shared_ptr< vmf::MetadataDesc >(new vmf::MetadataDesc( "people", vFields )), vmf::ValidateException);
    vFields.pop_back();
    vFields.emplace_back(vmf::FieldDesc("", vmf::Variant::type_integer));
    EXPECT_THROW(spDesc = std::shared_ptr< vmf::MetadataDesc >(new vmf::MetadataDesc("people", vFields)), vmf::ValidateException);
}

TEST_F(TestMetadataDesc, CreateWithFieldsEmptyName)
{
    EXPECT_THROW(spDesc = std::shared_ptr< vmf::MetadataDesc >(new vmf::MetadataDesc( vmf::vmf_string(), vFields )), vmf::ValidateException);
}

TEST_F(TestMetadataDesc, FindDesc)
{
    spDesc = std::shared_ptr< vmf::MetadataDesc >(new vmf::MetadataDesc( "people", vFields ));
    vmf::FieldDesc nameDesc;
    ASSERT_TRUE(spDesc->getFieldDesc(nameDesc, "name"));
    ASSERT_EQ(nameDesc.name, "name");
    ASSERT_EQ(nameDesc.type, vmf::Variant::type_string);
}

TEST_F(TestMetadataDesc, FindDescInvalidName)
{
    spDesc = std::shared_ptr< vmf::MetadataDesc >(new vmf::MetadataDesc( "people", vFields ));
    vmf::FieldDesc nameDesc;
    ASSERT_FALSE(spDesc->getFieldDesc(nameDesc, "name11111"));
}

TEST_F(TestMetadataDesc, FindDescEmpty)
{
    std::vector< vmf::FieldDesc > emptyFields;
    std::shared_ptr< vmf::MetadataDesc > spDesc = std::shared_ptr< vmf::MetadataDesc >(new vmf::MetadataDesc( "people", emptyFields ));
    vmf::FieldDesc nameDesc;
    ASSERT_FALSE(spDesc->getFieldDesc(nameDesc, "name11111"));
}

TEST_F(TestMetadataDesc, GetFields)
{
    spDesc = std::shared_ptr< vmf::MetadataDesc >(new vmf::MetadataDesc( "people", vFields ));

    std::vector<vmf::FieldDesc> fields = spDesc->getFields();
    ASSERT_EQ(fields.size(), vFields.size());

    for (size_t i = 0; i < fields.size(); i++)
    {
        ASSERT_TRUE(fields[i] == vFields[i]);
    }
}

TEST_F(TestMetadataDesc, CreateEvent)
{
    spDesc = std::shared_ptr< vmf::MetadataDesc >( new vmf::MetadataDesc( "people", vmf::Variant::type_string ));
    std::vector<vmf::FieldDesc> fields = spDesc->getFields();
    ASSERT_EQ(fields.size(), (size_t) 1);
    ASSERT_EQ(fields[0].name, vmf::vmf_string());
    ASSERT_EQ(fields[0].type, vmf::Variant::type_string);
}

TEST_F(TestMetadataDesc, CreateEventUnknownType)
{
    EXPECT_THROW(spDesc = std::shared_ptr< vmf::MetadataDesc >(new vmf::MetadataDesc( "people", vmf::Variant::type_empty )), vmf::IncorrectParamException);
}
