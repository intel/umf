// Copyright (C) 2013, Intel Corporation, all rights reserved.

#include "test_precomp.hpp"

class TestMetadata : public ::testing::Test
{
protected:
    void SetUp()
    {
        vFields.emplace_back( vmf::FieldDesc( "name", vmf::Variant::type_string ));
        vFields.emplace_back( vmf::FieldDesc( "age", vmf::Variant::type_integer ));
        vFields.emplace_back( vmf::FieldDesc( "sex", vmf::Variant::type_string ));
        vFields.emplace_back( vmf::FieldDesc( "email", vmf::Variant::type_string ));
        spDesc = std::shared_ptr< vmf::MetadataDesc >(new vmf::MetadataDesc( "people", vFields ));
        spJessica = std::shared_ptr< vmf::Metadata >(new vmf::Metadata(spDesc));
    }

    std::vector< vmf::FieldDesc > vFields;
    std::shared_ptr< vmf::MetadataDesc > spDesc;
    std::shared_ptr< vmf::Metadata > spJessica;
};

TEST_F(TestMetadata, CreateFromDesc)
{
    spJessica->setFieldValue( "name", "Jessica" );
    spJessica->setFieldValue( "age", (vmf::vmf_integer) 12 );
    spJessica->setFieldValue( "sex", "F" );
    spJessica->setFieldValue( "email", "jessica@kidsmail.com" );

    EXPECT_NO_THROW(spJessica->validate());
    ASSERT_EQ(spJessica->getName(), "people");
    ASSERT_EQ(spJessica->getSchemaName(), vmf::vmf_string());
    ASSERT_EQ(spJessica->getId(), vmf::INVALID_ID);
}

TEST_F(TestMetadata, CreateNullPtr)
{
    EXPECT_THROW(std::shared_ptr< vmf::Metadata > spJessica( new vmf::Metadata( nullptr )), vmf::NullPointerException);
}

TEST_F(TestMetadata, SetInvalidField)
{
    spJessica->setFieldValue( "name", "Jessica" );
    spJessica->setFieldValue( "age", (vmf::vmf_integer) 12 );
    spJessica->setFieldValue( "sex", "F" );
    EXPECT_THROW(spJessica->setFieldValue( "position", "manager" ), vmf::IncorrectParamException);
}

TEST_F(TestMetadata, SetInvalidType)
{
    spJessica->setFieldValue( "name", "Jessica" );
    EXPECT_THROW(spJessica->setFieldValue( "age", "xxx" ), vmf::TypeCastException);
}

TEST_F(TestMetadata, AddValue)
{
    spDesc = std::shared_ptr< vmf::MetadataDesc >( new vmf::MetadataDesc( "event", vmf::Variant::type_string ));
    std::shared_ptr< vmf::Metadata > spSki( new vmf::Metadata( spDesc ));
    spSki->addValue("string 1");
    spSki->addValue("string 2");
    ASSERT_EQ(spSki->size(), (size_t) 2);
    EXPECT_NO_THROW(spSki->validate());
}

TEST_F(TestMetadata, AddValueIncorrectType)
{
    spDesc = std::shared_ptr< vmf::MetadataDesc >( new vmf::MetadataDesc( "event", vmf::Variant::type_string ));
    std::shared_ptr< vmf::Metadata > spSki( new vmf::Metadata( spDesc ));
    EXPECT_THROW(spSki->addValue((vmf::vmf_integer) 42), vmf::TypeCastException);
}
