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
        defUpdateMode = vmf::Stat::UpdateMode::Manual;
        stat = std::make_shared< vmf::Stat >( statName, emptyFields, defUpdateMode );

        defUpdateTimeout = 0;
        defStatState = vmf::Stat::State::UpToDate;
        inexistingFieldName = "AnyNameYouWish";
    }

    std::string statName;
    vmf::Stat::UpdateMode::Type defUpdateMode;
    unsigned defUpdateTimeout;
    vmf::Stat::State::Type defStatState;
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
    vmf::Stat::UpdateMode::Type updateMode = vmf::Stat::UpdateMode::OnAdd;
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
        defUpdateMode = vmf::Stat::UpdateMode::Manual;

        initNameData();
        for( auto& n : nameData )
            statFields.emplace_back( n.name, n.schemaName, n.metadataName, n.fieldName, n.opName );

        stat = std::make_shared< vmf::Stat >( statName, statFields, defUpdateMode );
    }

    std::string statName;
    vmf::Stat::UpdateMode::Type defUpdateMode;
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

        //ASSERT_EQ( testField->getMetadataDesc(), nullptr );
        vmf::FieldDesc emptyDesc;
        //ASSERT_EQ( testField->getFieldDesc(), emptyDesc );

        ASSERT_NE( it, nameData.end() );
        ++it;
    }
}

class TestStatOperations : public ::testing::Test
{
protected:
    enum Flags // operation capability flags
    {
        InputMask  = 0x000f, // input mask
        InputInt   = 0x0001, //   accepts integer
        InputReal  = 0x0002, //   accepts real
        InputAny   = 0x0004, //   accepts any type (types can be mixed)
        OutputMask = 0x00f0, // output mask
        OutputInt  = 0x0010, //   produces integer
        OutputReal = 0x0020, //   produces real
        OutputSame = 0x0040, //   produces the same type as input
        ResetMask  = 0x0f00, // reset mask
        ResetInt   = 0x0100, //   reset to integer 0
        ResetReal  = 0x0200, //   reset to real 0
        ResetEmpty = 0x0400  //   reset to empty value (type_empty)
    };

    void testBuiltin( const std::string& name, unsigned flags )
    {
        bool status;
        std::string str;
        vmf::Variant val1,val2,val3,bad,res;
        vmf::StatOpBase* op = nullptr;

        // test if operation is registered
        status = false;
        EXPECT_NO_THROW( status = vmf::StatOpFactory::isRegistered( name ));
        ASSERT_EQ( status, true );

        // created op by fabric call
        EXPECT_NO_THROW( op = vmf::StatOpFactory::create( name ));
        ASSERT_NE( op, nullptr );

        // check name consistency
        EXPECT_NO_THROW( str = op->name() );
        ASSERT_EQ( str, name );

        do
        {
            // check reset result
            EXPECT_NO_THROW( op->reset() );
            EXPECT_NO_THROW( res = op->value() );

            ASSERT_NE( flags & ResetMask, 0 );
            vmf::Variant::Type resetType = res.getType();
            if( flags & ResetInt )
                ASSERT_EQ( resetType, vmf::Variant::type_integer );
            else if( flags & ResetReal )
                ASSERT_EQ( resetType, vmf::Variant::type_real );
            else /*if( flags & ResetEmpty )*/
                ASSERT_EQ( resetType, vmf::Variant::type_empty );

            // provide right output type
            ASSERT_NE( flags & OutputMask, 0 );
            vmf::Variant::Type outputType = vmf::Variant::type_empty;
            if( flags & OutputInt )
                outputType = vmf::Variant::type_integer;
            else if( flags & OutputReal )
                outputType = vmf::Variant::type_real;
            else /*if( flags & OutputSame )*/
                outputType = vmf::Variant::type_empty; // depends on input type, see below

            // provide consistent test inputs
            ASSERT_NE( flags & InputMask, 0 );
            if( flags & InputInt )
            {
                val1 = vmf::Variant( (vmf::vmf_integer)131 );
                val2 = vmf::Variant( (vmf::vmf_integer)-13 );
                val3 = vmf::Variant( (vmf::vmf_integer) 75 );
                bad = vmf::Variant( (vmf::vmf_real)77.13 );
                if( flags & OutputSame )
                    outputType = vmf::Variant::type_integer; // depends on input type, fixed
            }
            else if( flags & InputReal )
            {
                val1 = vmf::Variant( (vmf::vmf_real) 36.6 );
                val2 = vmf::Variant( (vmf::vmf_real)307.1 );
                val3 = vmf::Variant( (vmf::vmf_real)-3.14 );
                bad = vmf::Variant( (vmf::vmf_integer)77 );
                if( flags & OutputSame )
                    outputType = vmf::Variant::type_real; // depends on input type, fixed
            }
            else /*if( flags & InputAny )*/
            {
                val1 = vmf::Variant( (vmf::vmf_integer) 352 );
                val2 = vmf::Variant( (vmf::vmf_real)   13.7 );
                val3 = vmf::Variant( (vmf::vmf_string)"any" );
                bad = vmf::Variant(); // op accepts any type, so there's no bad
                // output depends on input type of individual value, must be checked individually
            }

            // Add first value
            EXPECT_NO_THROW( op->handle( val1 ));

            EXPECT_NO_THROW( res = op->value() );
            if( outputType == vmf::Variant::type_empty )
                ASSERT_EQ( res.getType(), val1.getType() );
            else
                ASSERT_EQ( res.getType(), outputType );

            // Add second value
            EXPECT_NO_THROW( op->handle( val2 ));

            EXPECT_NO_THROW( res = op->value() );
            if( outputType == vmf::Variant::type_empty )
                ASSERT_EQ( res.getType(), val2.getType() );
            else
                ASSERT_EQ( res.getType(), outputType );

            // Add third value
            EXPECT_NO_THROW( op->handle( val3 ));

            EXPECT_NO_THROW( res = op->value() );
            if( outputType == vmf::Variant::type_empty )
                ASSERT_EQ( res.getType(), val3.getType() );
            else
                ASSERT_EQ( res.getType(), outputType );

            // try to handle bad input
            if( bad.getType() == vmf::Variant::type_empty )
                EXPECT_NO_THROW( op->handle( bad ));
            else
                EXPECT_THROW( op->handle( bad ), vmf::TypeCastException );

            if( flags & InputInt )
                flags &= ~InputInt;
            else if( flags & InputReal )
                flags &= ~InputReal;
            else /*if( flags & InputAny )*/
                flags &= ~InputMask;
        }
        while( (flags & InputMask) != 0 );

        delete op;
    }

    void testStatOpFactory()
    {
        bool status;
        std::string name,str;
        vmf::StatOpBase* op = nullptr;

        str = "AnyUnknownStringYouWant";
        EXPECT_THROW( op = vmf::StatOpFactory::create( str ), vmf::NotFoundException );
        ASSERT_EQ( op, nullptr );

        str = "YetAnotherUnknownString";
        EXPECT_THROW( op = vmf::StatOpFactory::create( str ), vmf::NotFoundException );
        ASSERT_EQ( op, nullptr );

        EXPECT_NO_THROW( op = UserOp::createInstance() );
        ASSERT_NE( op, nullptr );
        EXPECT_NO_THROW( name = op->name() );
        ASSERT_EQ( name, UserOp::userOpName );
        delete op; op = nullptr;

        status = true;
        EXPECT_NO_THROW( status = vmf::StatOpFactory::isRegistered( name ));
        ASSERT_EQ( status, false );

        EXPECT_THROW( vmf::StatOpFactory::registerUserOp( nullptr ), vmf::NullPointerException );
        EXPECT_NO_THROW( vmf::StatOpFactory::registerUserOp( UserOp::createInstance ));
        EXPECT_THROW( vmf::StatOpFactory::registerUserOp( UserOp::createInstance ), vmf::IncorrectParamException );
        EXPECT_THROW( vmf::StatOpFactory::registerUserOp( UserOp::createInstance2 ), vmf::IncorrectParamException );

        status = false;
        EXPECT_NO_THROW( status = vmf::StatOpFactory::isRegistered( name ));
        ASSERT_EQ( status, true );

        ASSERT_EQ( op, nullptr );
        EXPECT_NO_THROW( op = vmf::StatOpFactory::create( name ));
        ASSERT_NE( op, nullptr );
        EXPECT_NO_THROW( str = op->name() );
        ASSERT_EQ( str, name );
        delete op; op = nullptr;
    }

    class UserOp: public vmf::StatOpBase
    {
    public:
        UserOp() {}
        virtual ~UserOp() {}
    public:
        virtual std::string name() const { return userOpName; }
        virtual void reset() { m_value = vmf::Variant(); }
        virtual void handle( const vmf::Variant& /*fieldValue*/ ) {}
        virtual vmf::Variant value() const { return m_value; }
    private:
        vmf::Variant m_value;
    public:
        static vmf::StatOpBase* createInstance() { return new UserOp(); }
        static vmf::StatOpBase* createInstance2() { return new UserOp(); }
        static const std::string userOpName;
    };
};

/*static*/ const std::string TestStatOperations::UserOp::userOpName = "***TestStatOperations::UserOp::userOpName***";

TEST_F( TestStatOperations, BuiltinMin )
{
    testBuiltin( vmf::StatOpFactory::builtinName( vmf::StatOpFactory::BuiltinOp::Min ),
                 InputInt | InputReal | OutputSame | ResetEmpty );
}

TEST_F( TestStatOperations, BuiltinMax )
{
    testBuiltin( vmf::StatOpFactory::builtinName( vmf::StatOpFactory::BuiltinOp::Max ),
                 InputInt | InputReal | OutputSame | ResetEmpty );
}

TEST_F( TestStatOperations, BuiltinAverage )
{
    testBuiltin( vmf::StatOpFactory::builtinName( vmf::StatOpFactory::BuiltinOp::Average ),
                 InputInt | InputReal | OutputReal | ResetEmpty );
}

TEST_F( TestStatOperations, BuiltinCount )
{
    testBuiltin( vmf::StatOpFactory::builtinName( vmf::StatOpFactory::BuiltinOp::Count ),
                 InputAny | OutputInt | ResetInt );
}

TEST_F( TestStatOperations, BuiltinSum )
{
    testBuiltin( vmf::StatOpFactory::builtinName( vmf::StatOpFactory::BuiltinOp::Sum ),
                 InputInt | InputReal | OutputSame | ResetEmpty );
}

TEST_F( TestStatOperations, BuiltinLast )
{
    testBuiltin( vmf::StatOpFactory::builtinName( vmf::StatOpFactory::BuiltinOp::Last ),
                 InputAny | OutputSame | ResetEmpty );
}

TEST_F( TestStatOperations, StatOpFactory )
{
    testStatOpFactory();
}

class TestStatistics : public ::testing::TestWithParam< vmf::Stat::UpdateMode::Type >
{
protected:
    void SetUp()
    {
        fnWorkingPath = getWorkingPath();
        fnInputName   = "BlueSquare.avi";

        mcSchemaName = "PersonSchema";
        mcDescName   = "Person";
        mcPersonName = "Name";
        mcAgeName    = "Age";
        mcGrowthName = "Growth";
        mcSalaryName = "Salary";

        scStatName            = "PersonStatistics";
        scPersonNameCount     = "PersonNameCount";
        scPersonNameLast      = "PersonNameLast";
        scPersonAgeMin        = "PersonAgeMin";
        scPersonAgeMax        = "PersonAgeMax";
        scPersonGrowthAverage = "PersonGrowthAverage";
        scPersonSalarySum     = "PersonSalarySum";
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
        scMetadataSchema = std::make_shared< vmf::MetadataSchema >( mcSchemaName );

        VMF_METADATA_BEGIN( mcDescName );
            VMF_FIELD_STR( mcPersonName );
            VMF_FIELD_INT( mcAgeName );
            VMF_FIELD_INT( mcGrowthName );
            VMF_FIELD_INT( mcSalaryName );
        VMF_METADATA_END( scMetadataSchema );

        stream.addSchema( scMetadataSchema );

        scMetadataDesc = scMetadataSchema->findMetadataDesc( mcDescName );

        vmf::FieldDesc field;
        scMetadataDesc->getFieldDesc( field, mcPersonName ); scFieldDesc.push_back( field );
        scMetadataDesc->getFieldDesc( field, mcAgeName    ); scFieldDesc.push_back( field );
        scMetadataDesc->getFieldDesc( field, mcGrowthName ); scFieldDesc.push_back( field );
        scMetadataDesc->getFieldDesc( field, mcSalaryName ); scFieldDesc.push_back( field );
    }

    void configureStatistics( vmf::MetadataStream& stream )
    {
        std::vector< vmf::StatField > fields;
        fields.emplace_back( scPersonNameCount, mcSchemaName, mcDescName, mcPersonName, vmf::StatOpFactory::builtinName( vmf::StatOpFactory::BuiltinOp::Count ));
        fields.emplace_back( scPersonNameLast, mcSchemaName, mcDescName, mcPersonName, vmf::StatOpFactory::builtinName( vmf::StatOpFactory::BuiltinOp::Last ));
        fields.emplace_back( scPersonAgeMin, mcSchemaName, mcDescName, mcAgeName, vmf::StatOpFactory::builtinName( vmf::StatOpFactory::BuiltinOp::Min ));
        fields.emplace_back( scPersonAgeMax, mcSchemaName, mcDescName, mcAgeName, vmf::StatOpFactory::builtinName( vmf::StatOpFactory::BuiltinOp::Max ));
        fields.emplace_back( scPersonGrowthAverage, mcSchemaName, mcDescName, mcGrowthName, vmf::StatOpFactory::builtinName( vmf::StatOpFactory::BuiltinOp::Average ));
        fields.emplace_back( scPersonSalarySum, mcSchemaName, mcDescName, mcSalaryName, vmf::StatOpFactory::builtinName( vmf::StatOpFactory::BuiltinOp::Sum ));
        stream.addStat( std::make_shared<vmf::Stat>( scStatName, fields, vmf::Stat::UpdateMode::Disabled ));
    }

    void initStatistics()
    {
        stNameCount = 0;
        stNameLast = "";
        stAgeMin = 0;
        stAgeMax = 0;
        stGrowthAverage = 0.0; stGrowthAverageSum = 0; stGrowthAverageCount = 0;
        stSalarySum = 0;
        stFirstTimeOnce = true;
    }

    void finalizeStatistics()
    {
        if( stGrowthAverageCount != 0 )
            stGrowthAverage = (vmf::vmf_real)stGrowthAverageSum / (vmf::vmf_real)stGrowthAverageCount;
        else
            stGrowthAverage = 0.0;
    }

    void addMetadata( vmf::MetadataStream& stream, const vmf::vmf_string& name, vmf::vmf_integer age, vmf::vmf_integer growth, vmf::vmf_integer salary, bool doStatistics )
    {
        std::shared_ptr<vmf::Metadata> metadata = std::make_shared< vmf::Metadata >( scMetadataDesc );

        metadata->emplace_back( mcPersonName, name );
        if( doStatistics )
        {
            ++stNameCount;
            stNameLast = name;
        }

        metadata->emplace_back( mcAgeName, age );
        if( doStatistics )
        {
            stAgeMin = (stFirstTimeOnce ? age : std::min( stAgeMin, age ));
            stAgeMax = (stFirstTimeOnce ? age : std::max( stAgeMax, age ));
        }

        metadata->emplace_back( mcGrowthName, growth );
        if( doStatistics )
        {
            stGrowthAverageSum += growth;
            ++stGrowthAverageCount;
        }

        metadata->emplace_back( mcSalaryName, salary );
        if( doStatistics )
        {
            stSalarySum += salary;
        }

        stream.add( metadata );

        if( doStatistics )
        {
            stFirstTimeOnce = false;
        }
    }

    void putMetadata( vmf::MetadataStream& stream, bool doStatistics )
    {
        if( doStatistics )
            initStatistics();

        addMetadata( stream, "Peter", 53, 185, 5000, doStatistics );
        addMetadata( stream, "Jessica", 31, 176, 3000, doStatistics );
        addMetadata( stream, "Matthias", 41, 192, 7000, doStatistics );
        addMetadata( stream, "John", 29, 180, 5500, doStatistics );

        if( doStatistics )
            finalizeStatistics();
    }

    void checkStatistics( const vmf::Stat& stat, vmf::Stat::UpdateMode::Type updateMode, bool doCompareValues )
    {
        vmf::Variant nameCount     = stat[scPersonNameCount];
        vmf::Variant nameLast      = stat[scPersonNameLast];
        vmf::Variant ageMin        = stat[scPersonAgeMin];
        vmf::Variant ageMax        = stat[scPersonAgeMax];
        vmf::Variant growthAverage = stat[scPersonGrowthAverage];
        vmf::Variant salarySum     = stat[scPersonSalarySum];

        if( updateMode != vmf::Stat::UpdateMode::Disabled )
        {
            ASSERT_EQ( nameCount.getType(), vmf::Variant::type_integer );
            ASSERT_EQ( nameLast.getType(), vmf::Variant::type_string );
            ASSERT_EQ( ageMin.getType(), vmf::Variant::type_integer );
            ASSERT_EQ( ageMax.getType(), vmf::Variant::type_integer );
            ASSERT_EQ( growthAverage.getType(), vmf::Variant::type_real );
            ASSERT_EQ( salarySum.getType(), vmf::Variant::type_integer );

            if( doCompareValues )
            {
                ASSERT_EQ( nameCount.get_integer(), stNameCount );
                ASSERT_EQ( nameLast.get_string(), stNameLast );
                ASSERT_EQ( ageMin.get_integer(), stAgeMin );
                ASSERT_EQ( ageMax.get_integer(), stAgeMax );
                ASSERT_EQ( growthAverage.get_real(), stGrowthAverage );
                ASSERT_EQ( salarySum.get_integer(), stSalarySum );
            }
        }
    }

    std::string fnWorkingPath;
    std::string fnInputName;

    std::string mcSchemaName;
    std::string mcDescName;
    std::string mcPersonName;
    std::string mcAgeName;
    std::string mcGrowthName;
    std::string mcSalaryName;

    std::vector< vmf::FieldDesc > scFieldDesc;
    std::shared_ptr< vmf::MetadataDesc > scMetadataDesc;
    std::shared_ptr< vmf::MetadataSchema > scMetadataSchema;

    std::string scStatName;
    std::string scPersonNameCount;
    std::string scPersonNameLast;
    std::string scPersonAgeMin;
    std::string scPersonAgeMax;
    std::string scPersonGrowthAverage;
    std::string scPersonSalarySum;

    vmf::vmf_integer stNameCount;
    vmf::vmf_string stNameLast;
    vmf::vmf_integer stAgeMin;
    vmf::vmf_integer stAgeMax;
    vmf::vmf_real stGrowthAverage; vmf::vmf_integer stGrowthAverageSum,stGrowthAverageCount;
    vmf::vmf_integer stSalarySum;
    bool stFirstTimeOnce;
};

TEST_P( TestStatistics, Gathering )
{
    vmf::Stat::UpdateMode::Type updateMode = GetParam();
    unsigned updateTimeout = 100;
    const bool doCompareValues = true;

    std::string fileName = "test_statistics.avi";
    createFile( fileName );

    vmf::MetadataStream stream;
    ASSERT_EQ( stream.open( fileName, vmf::MetadataStream::Update ), true );

    configureSchema( stream );
    configureStatistics( stream );

    std::shared_ptr<vmf::Stat> stat = stream.getStat(scStatName);
    stat->setUpdateTimeout( updateTimeout );
    stat->setUpdateMode( updateMode );
    putMetadata( stream, doCompareValues );
    stat->update( true );

    checkStatistics( *stat, updateMode, doCompareValues );

    stream.save();
    stream.close();
}

TEST_P( TestStatistics, SaveLoad )
{
    vmf::Stat::UpdateMode::Type updateMode = GetParam();
    unsigned updateTimeout = 100;
    const bool doCompareValues = true;

    std::string fileName = "test_statistics.avi";
    createFile( fileName );

    vmf::MetadataStream saveStream;
    ASSERT_EQ( saveStream.open( fileName, vmf::MetadataStream::Update ), true );

    configureSchema( saveStream );
    configureStatistics( saveStream );
    putMetadata( saveStream, doCompareValues );

    saveStream.save();
    saveStream.close();

    vmf::MetadataStream loadStream;
    ASSERT_EQ( loadStream.open( fileName, vmf::MetadataStream::ReadOnly ), true );
    ASSERT_EQ( loadStream.load( mcSchemaName ), true );

    std::shared_ptr<vmf::Stat> stat = loadStream.getStat(scStatName);
    stat->setUpdateTimeout( updateTimeout );
    stat->setUpdateMode( updateMode );

    loadStream.recalcStat();
    stat->update( true );

    checkStatistics( *stat, updateMode, doCompareValues );

    loadStream.close();
}

TEST_P( TestStatistics, ExportImportXML )
{
    vmf::Stat::UpdateMode::Type updateMode = GetParam();
    unsigned updateTimeout = 100;
    const bool doCompareValues = true;
    vmf::FormatXML format;

    vmf::MetadataStream saveStream;

    configureSchema( saveStream );
    configureStatistics( saveStream );
    putMetadata( saveStream, doCompareValues );

    std::string data = saveStream.serialize( format );

    saveStream.close();

    vmf::MetadataStream loadStream;

    loadStream.deserialize( data, format );

    std::shared_ptr<vmf::Stat> stat = loadStream.getStat(scStatName);
    stat->setUpdateTimeout( updateTimeout );
    stat->setUpdateMode( updateMode );

    loadStream.recalcStat();
    stat->update( true );

    checkStatistics( *stat, updateMode, doCompareValues );

    loadStream.close();
}

TEST_P( TestStatistics, ExportImportJSON )
{
    vmf::Stat::UpdateMode::Type updateMode = GetParam();
    unsigned updateTimeout = 100;
    const bool doCompareValues = true;
    vmf::FormatJSON format;

    vmf::MetadataStream saveStream;

    configureSchema( saveStream );
    configureStatistics( saveStream );
    putMetadata( saveStream, doCompareValues );

    std::string data = saveStream.serialize( format );

    saveStream.close();

    vmf::MetadataStream loadStream;

    loadStream.deserialize( data, format );

    std::shared_ptr<vmf::Stat> stat = loadStream.getStat(scStatName);
    stat->setUpdateTimeout( updateTimeout );
    stat->setUpdateMode( updateMode );

    loadStream.recalcStat();
    stat->update( true );

    checkStatistics( *stat, updateMode, doCompareValues );

    loadStream.close();
}

INSTANTIATE_TEST_CASE_P(UnitTest, TestStatistics,
                        ::testing::Values(
                            vmf::Stat::UpdateMode::Disabled ,vmf::Stat::UpdateMode::Manual
                            ,vmf::Stat::UpdateMode::OnAdd, vmf::Stat::UpdateMode::OnTimer
                            ));

