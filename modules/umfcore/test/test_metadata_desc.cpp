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
        vFields.emplace_back( umf::FieldDesc( "name", umf::Variant::type_string ));
        vFields.emplace_back( umf::FieldDesc( "age", umf::Variant::type_integer ));
        vFields.emplace_back( umf::FieldDesc( "sex", umf::Variant::type_integer ));
        vFields.emplace_back( umf::FieldDesc( "email", umf::Variant::type_string ));
    }

    std::vector< umf::FieldDesc > vFields;
    std::shared_ptr< umf::MetadataDesc > spDesc;
};

TEST_F(TestMetadataDesc, CreateWithFields)
{
    EXPECT_NO_THROW(spDesc = std::shared_ptr< umf::MetadataDesc >(new umf::MetadataDesc( "people", vFields )));
    auto descFields = spDesc->getFields();
    ASSERT_EQ(descFields.size(), vFields.size());
    ASSERT_EQ(spDesc->getMetadataName(), "people");
    ASSERT_EQ(spDesc->getSchemaName(), umf::umf_string());
}

TEST_F(TestMetadataDesc, CreateWithFieldsIncorrect)
{
    vFields.emplace_back( umf::FieldDesc( "sex", umf::Variant::type_string ));
    EXPECT_THROW(spDesc = std::shared_ptr< umf::MetadataDesc >(new umf::MetadataDesc( "people", vFields )), umf::ValidateException);
    vFields.pop_back();
    vFields.emplace_back(umf::FieldDesc("", umf::Variant::type_integer));
    EXPECT_THROW(spDesc = std::shared_ptr< umf::MetadataDesc >(new umf::MetadataDesc("people", vFields)), umf::ValidateException);
}

TEST_F(TestMetadataDesc, CreateWithFieldsEmptyName)
{
    EXPECT_THROW(spDesc = std::shared_ptr< umf::MetadataDesc >(new umf::MetadataDesc( umf::umf_string(), vFields )), umf::ValidateException);
}

TEST_F(TestMetadataDesc, FindDesc)
{
    spDesc = std::shared_ptr< umf::MetadataDesc >(new umf::MetadataDesc( "people", vFields ));
    umf::FieldDesc nameDesc;
    ASSERT_TRUE(spDesc->getFieldDesc(nameDesc, "name"));
    ASSERT_EQ(nameDesc.name, "name");
    ASSERT_EQ(nameDesc.type, umf::Variant::type_string);
}

TEST_F(TestMetadataDesc, FindDescInvalidName)
{
    spDesc = std::shared_ptr< umf::MetadataDesc >(new umf::MetadataDesc( "people", vFields ));
    umf::FieldDesc nameDesc;
    ASSERT_FALSE(spDesc->getFieldDesc(nameDesc, "name11111"));
}

TEST_F(TestMetadataDesc, FindDescEmpty)
{
    std::vector< umf::FieldDesc > emptyFields;
    std::shared_ptr< umf::MetadataDesc > spDesc = std::shared_ptr< umf::MetadataDesc >(new umf::MetadataDesc( "people", emptyFields ));
    umf::FieldDesc nameDesc;
    ASSERT_FALSE(spDesc->getFieldDesc(nameDesc, "name11111"));
}

TEST_F(TestMetadataDesc, GetFields)
{
    spDesc = std::shared_ptr< umf::MetadataDesc >(new umf::MetadataDesc( "people", vFields ));

    std::vector<umf::FieldDesc> fields = spDesc->getFields();
    ASSERT_EQ(fields.size(), vFields.size());

    for (size_t i = 0; i < fields.size(); i++)
    {
        ASSERT_TRUE(fields[i] == vFields[i]);
    }
}

TEST_F(TestMetadataDesc, CreateEvent)
{
    spDesc = std::shared_ptr< umf::MetadataDesc >( new umf::MetadataDesc( "people", umf::Variant::type_string ));
    std::vector<umf::FieldDesc> fields = spDesc->getFields();
    ASSERT_EQ(fields.size(), (size_t) 1);
    ASSERT_EQ(fields[0].name, umf::umf_string());
    ASSERT_EQ(fields[0].type, umf::Variant::type_string);
}

TEST_F(TestMetadataDesc, CreateEventUnknownType)
{
    EXPECT_THROW(spDesc = std::shared_ptr< umf::MetadataDesc >(new umf::MetadataDesc( "people", umf::Variant::type_empty )), umf::IncorrectParamException);
}
