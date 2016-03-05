/*
 * Copyright 2016 Intel(r) Corporation
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

#include <fstream>

#if defined(WIN32)
  const char delim = '\\';
#else
  const char delim = '/';
#endif

class TestStat : public ::testing::Test
{
protected:
    void SetUp()
    {
        statName = "SimpleStat";
        defUpdateMode = vmf::StatUpdateMode::Manual;
        stat = std::make_shared< vmf::Stat >( statName, emptyFields, defUpdateMode );

        defUpdateTimeout = 0;
        defStatState = vmf::StatState::UpToDate;
        inexistingFieldName = "AnyNameYouWish";
    }

    std::string statName;
    vmf::StatUpdateMode::Type defUpdateMode;
    unsigned defUpdateTimeout;
    vmf::StatState::Type defStatState;
    std::vector< vmf::StatField > emptyFields;
    std::string inexistingFieldName;
    std::shared_ptr< vmf::Stat > stat;
};

TEST_F( TestStat, Creation )
{
    ASSERT_EQ( stat->getName(), statName );
    ASSERT_EQ( stat->getState(), defStatState );
}

TEST_F( TestStat, UpdateMode )
{
    vmf::StatUpdateMode::Type updateMode = vmf::StatUpdateMode::OnAdd;
    ASSERT_NE( updateMode, defUpdateMode );

    ASSERT_EQ( stat->getUpdateMode(), defUpdateMode );

    EXPECT_NO_THROW( stat->setUpdateMode( updateMode ));
    ASSERT_EQ( stat->getUpdateMode(), updateMode );

    EXPECT_NO_THROW( stat->setUpdateMode( defUpdateMode ));
    ASSERT_EQ( stat->getUpdateMode(), defUpdateMode );
}

TEST_F( TestStat, UpdateTimeout )
{
    unsigned updateTimeout = defUpdateTimeout + 100;
    ASSERT_NE( updateTimeout, defUpdateTimeout );

    ASSERT_EQ( stat->getUpdateTimeout(), defUpdateTimeout );

    EXPECT_NO_THROW( stat->setUpdateTimeout( updateTimeout ));
    ASSERT_EQ( stat->getUpdateTimeout(), updateTimeout );

    EXPECT_NO_THROW( stat->setUpdateTimeout( defUpdateTimeout ));
    ASSERT_EQ( stat->getUpdateTimeout(), defUpdateTimeout );
}

TEST_F( TestStat, EmptyFields )
{
    std::vector< std::string > fieldNames;

    EXPECT_NO_THROW( fieldNames = stat->getAllFieldNames() );
    ASSERT_EQ( fieldNames.size(), emptyFields.size() );

    vmf::StatField field;
    EXPECT_THROW( field = stat->getField( inexistingFieldName ), vmf::NotFoundException );

    vmf::Variant value;
    EXPECT_THROW( value = (*stat)[ inexistingFieldName ], vmf::NotFoundException );
}

class TestStatFields : public ::testing::Test
{
protected:
    struct NameData
    {
        NameData( const std::string& n, const std::string& sn, const std::string& mn,
                  const std::string& fn, const std::string& on )
            : name( n ), schemaName( sn ), metadataName( mn ), fieldName( fn ), opName ( on ) {}
        NameData() {}
        ~NameData() {}
        const std::string name, schemaName, metadataName, fieldName, opName;
    };

    void initNameData()
    {
        nameData.emplace_back( "MinField", "MetadataSchema", "MetadataName", "FieldName", vmf::StatOpFactory::builtinName( vmf::StatOpFactory::BuiltinOp::Min ));
        nameData.emplace_back( "MaxField", "MetadataSchema", "MetadataName", "FieldName", vmf::StatOpFactory::builtinName( vmf::StatOpFactory::BuiltinOp::Max ));
        nameData.emplace_back( "AverageField", "MetadataSchema", "MetadataName", "FieldName", vmf::StatOpFactory::builtinName( vmf::StatOpFactory::BuiltinOp::Average ));
        nameData.emplace_back( "CountField", "MetadataSchema", "MetadataName", "FieldName", vmf::StatOpFactory::builtinName( vmf::StatOpFactory::BuiltinOp::Count ));
        nameData.emplace_back( "SumField", "MetadataSchema", "MetadataName", "FieldName", vmf::StatOpFactory::builtinName( vmf::StatOpFactory::BuiltinOp::Sum ));
        nameData.emplace_back( "LastField", "MetadataSchema", "MetadataName", "FieldName", vmf::StatOpFactory::builtinName( vmf::StatOpFactory::BuiltinOp::Last ));
    }

    void SetUp()
    {
        statName = "SimpleStat";
        defUpdateMode = vmf::StatUpdateMode::Manual;

        initNameData();
        for( auto& n : nameData )
            statFields.emplace_back( n.name, n.schemaName, n.metadataName, n.fieldName, n.opName );

        stat = std::make_shared< vmf::Stat >( statName, statFields, defUpdateMode );
    }

    std::string statName;
    vmf::StatUpdateMode::Type defUpdateMode;
    std::vector< NameData > nameData;
    std::vector< vmf::StatField > statFields;
    std::shared_ptr< vmf::Stat > stat;
};

TEST_F( TestStatFields, Creation )
{
    std::vector< std::string > fieldNames;

    EXPECT_NO_THROW( fieldNames = stat->getAllFieldNames() );
    ASSERT_EQ( fieldNames.size(), statFields.size() );

    auto it = nameData.begin();
    for( auto& fieldName : fieldNames )
    {
        const NameData& n = *it;

        const vmf::StatField* testField = 0;
        EXPECT_NO_THROW( testField = &(stat->getField( fieldName )));

        ASSERT_EQ( testField->getName(), n.name );
        ASSERT_EQ( testField->getSchemaName(), n.schemaName );
        ASSERT_EQ( testField->getMetadataName(), n.metadataName );
        ASSERT_EQ( testField->getFieldName(), n.fieldName );
        ASSERT_EQ( testField->getOpName(), n.opName );

        ASSERT_EQ( testField->getMetadataDesc(), nullptr );
        vmf::FieldDesc emptyDesc;
        ASSERT_EQ( testField->getFieldDesc(), emptyDesc );

        ASSERT_NE( it, nameData.end() );
        ++it;
    }
}

class TestStatistics : public ::testing::TestWithParam< vmf::StatUpdateMode::Type >
{
protected:
    void SetUp()
    {
        vmf::initialize();

        fnWorkingPath = getWorkingPath();
        fnInputName   = "BlueSquare.avi";

        scSchemaName = "PersonSchema";
        scDescName   = "Person";
        scPersonName = "Name";
        scAgeName    = "Age";
        scGrowthName = "Growth";

        stStatName            = "PersonStatistics";
        stPersonNameCount     = "PersonNameCount";
        stPersonNameLast      = "PersonNameLast";
        stPersonAgeMin        = "PersonAgeMin";
        stPersonAgeMax        = "PersonAgeMax";
        stPersonGrowthAverage = "PersonGrowthAverage";
    }

    void TearDown()
    {
        vmf::terminate();
    }

    std::string getWorkingPath() const
    {
        std::string path = ::testing::internal::g_argvs[0];
        size_t pos = path.find_last_of( delim );
        if( pos != std::string::npos )
            path = path.substr( 0, pos + 1 );
		else
			path = "";
        return path;
    }

    void createFile( const std::string& dstName )
    {
        std::string srcName = fnWorkingPath + fnInputName;

        std::ifstream source( srcName, std::ios::binary );
        if( !source )
            VMF_EXCEPTION( vmf::IncorrectParamException, "Error opening input file: "  + srcName );

        std::ofstream dest( dstName, std::ios::binary );
        if( !dest )
            VMF_EXCEPTION( vmf::IncorrectParamException, "Error opening output file: " + dstName );

        dest << source.rdbuf();
    }

    void configureSchema( vmf::MetadataStream& stream )
    {
        scFieldDesc.emplace_back( scPersonName, vmf::Variant::type_string );
        scFieldDesc.emplace_back( scAgeName, vmf::Variant::type_integer );
        scFieldDesc.emplace_back( scGrowthName, vmf::Variant::type_integer );

        scMetadataDesc = std::make_shared< vmf::MetadataDesc >( scDescName, scFieldDesc );
        scMetadataSchema = std::make_shared< vmf::MetadataSchema >( scSchemaName );

        scMetadataSchema->add( scMetadataDesc );
        stream.addSchema( scMetadataSchema );
    }

    void configureStatistics( vmf::MetadataStream& stream )
    {
        std::vector< vmf::StatField > fields;
        fields.emplace_back( stPersonNameCount, scSchemaName, scDescName, scPersonName, vmf::StatOpFactory::builtinName( vmf::StatOpFactory::BuiltinOp::Count ));
        fields.emplace_back( stPersonNameLast, scSchemaName, scDescName, scPersonName, vmf::StatOpFactory::builtinName( vmf::StatOpFactory::BuiltinOp::Last ));
        fields.emplace_back( stPersonAgeMin, scSchemaName, scDescName, scAgeName, vmf::StatOpFactory::builtinName( vmf::StatOpFactory::BuiltinOp::Min ));
        fields.emplace_back( stPersonAgeMax, scSchemaName, scDescName, scAgeName, vmf::StatOpFactory::builtinName( vmf::StatOpFactory::BuiltinOp::Max ));
        fields.emplace_back( stPersonGrowthAverage, scSchemaName, scDescName, scGrowthName, vmf::StatOpFactory::builtinName( vmf::StatOpFactory::BuiltinOp::Average ));
        stream.addStat( stStatName, fields, vmf::StatUpdateMode::Disabled );
    }

    void addMetadata( vmf::MetadataStream& stream )
    {
        std::shared_ptr<vmf::Metadata> metadata;

        metadata = std::make_shared< vmf::Metadata >( scMetadataDesc );
        metadata->emplace_back( scPersonName, "Peter" );
        metadata->emplace_back( scAgeName, 53 );
        metadata->emplace_back( scGrowthName, 185 );
        stream.add( metadata );

        metadata = std::make_shared< vmf::Metadata >( scMetadataDesc );
        metadata->emplace_back( scPersonName, "Jessica" );
        metadata->emplace_back( scAgeName, 31 );
        metadata->emplace_back( scGrowthName, 176 );
        stream.add( metadata );
    }

    std::string fnWorkingPath;
    std::string fnInputName;

    std::string scSchemaName;
    std::string scDescName;
    std::string scPersonName;
    std::string scAgeName;
    std::string scGrowthName;

    std::vector< vmf::FieldDesc > scFieldDesc;
    std::shared_ptr< vmf::MetadataDesc > scMetadataDesc;
    std::shared_ptr< vmf::MetadataSchema > scMetadataSchema;

    std::string stStatName;
    std::string stPersonNameCount;
    std::string stPersonNameLast;
    std::string stPersonAgeMin;
    std::string stPersonAgeMax;
    std::string stPersonGrowthAverage;
};

TEST_P( TestStatistics, Gathering )
{
    vmf::StatUpdateMode::Type updateMode = GetParam();
    unsigned updateTimeout = 100;

    std::string fileName = "test_statistics.avi";
    createFile( fileName );

    vmf::MetadataStream stream;
    ASSERT_EQ( stream.open( fileName, vmf::MetadataStream::Update ), true );

    configureSchema( stream );
    configureStatistics( stream );

    vmf::Stat& stat = stream.getStat( stStatName );
    stat.setUpdateTimeout( updateTimeout );
    stat.setUpdateMode( updateMode );
    addMetadata( stream );
    stat.update( true, true );

    vmf::Variant nameCount     = stat[stPersonNameCount];
    vmf::Variant nameLast      = stat[stPersonNameLast];
    vmf::Variant ageMin        = stat[stPersonAgeMin];
    vmf::Variant ageMax        = stat[stPersonAgeMax];
    vmf::Variant growthAverage = stat[stPersonGrowthAverage];

    if( updateMode != vmf::StatUpdateMode::Disabled )
    {
        ASSERT_EQ( nameCount.getType(), vmf::Variant::type_integer );
        ASSERT_EQ( nameLast.getType(), vmf::Variant::type_string );
        ASSERT_EQ( ageMin.getType(), vmf::Variant::type_integer );
        ASSERT_EQ( ageMax.getType(), vmf::Variant::type_integer );
        ASSERT_EQ( growthAverage.getType(), vmf::Variant::type_real );

//        std::cout << "nameCount     = " << nameCount.get_integer() << std::endl;
//        std::cout << "nameLast      = " << nameLast.get_string() << std::endl;
//        std::cout << "ageMin        = " << ageMin.get_integer() << std::endl;
//        std::cout << "ageMax        = " << ageMax.get_integer() << std::endl;
//        std::cout << "growthAverage = " << growthAverage.get_real() << std::endl;
    }

    stream.save();
    stream.close();
}

INSTANTIATE_TEST_CASE_P(UnitTest, TestStatistics,
                        ::testing::Values(
                            vmf::StatUpdateMode::Disabled,
                            vmf::StatUpdateMode::Manual,
                            vmf::StatUpdateMode::OnAdd,
                            vmf::StatUpdateMode::OnTimer
                            ));

