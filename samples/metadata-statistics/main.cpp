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
/*
 * This demo shows how to use metadata descriptions
 * to store data structures such as GPS coordinates with associative time.
 */

#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cassert>
#include <chrono>
#include <thread>

#include "umf/vmf.hpp"

//using namespace umf;
using namespace std;

string workingPath;

#define VIDEO_FILE (workingPath + "BlueSquare.avi")

void copyFile(const string& srcName, const char *dstName)
{
    std::ifstream src( srcName, ios::binary );
    std::ofstream dst(dstName, ios::binary);
    if (src && dst)
        dst << src.rdbuf();
    else
        VMF_EXCEPTION(umf::IncorrectParamException, "Error copying '" + srcName + "' to '" + dstName + "'");
}

class StrCatOp: public umf::StatOpBase
{
public:
    StrCatOp()
        {}
    virtual ~StrCatOp()
        {}

public:
    virtual std::string name() const
        { return opName(); }
    virtual void reset()
        {
            std::unique_lock< std::mutex > lock( m_lock );
            m_value = "";
        }
    virtual void handle( const umf::Variant& fieldValue )
        {
            std::unique_lock< std::mutex > lock( m_lock );
            if( fieldValue.getType() != umf::Variant::type_string )
                VMF_EXCEPTION( umf::NotImplementedException, "Operation not applicable to this data type" );
            if( !m_value.empty() )
                m_value += " | ";
            m_value += fieldValue.get_string();
        }
    virtual umf::Variant value() const
        {
            std::unique_lock< std::mutex > lock( m_lock );
            return umf::Variant( (umf::umf_string)m_value );
        }

private:
    mutable std::mutex m_lock;
    std::string m_value;

public:
    static StatOpBase* createInstance()
        { return new StrCatOp(); }
    static std::string opName()
        { return "User.StrCatOp"; }
};

inline std::ostream& operator<<( std::ostream& os, const umf::Variant& value )
{
    os << "(" << value.getTypeName() << ") " << value.toString();
    return os;
}

static void dumpStatistics( const umf::MetadataStream& mdStream )
{
    std::cout << std::endl;
    std::cout << "** statistics dump **" << std::endl;

    std::vector< std::string > statNames = mdStream.getAllStatNames();
    if( !statNames.empty() )
    {
        for( auto& statName : statNames )
        {
            auto stat = mdStream.getStat( statName );
            std::cout << "statistics: '" << stat->getName() << "'" << std::endl;

            std::vector< std::string > fieldNames = stat->getAllFieldNames();
            if( !fieldNames.empty() )
            {
                for( auto& fieldName : fieldNames )
                {
                    const umf::StatField& field = stat->getField( fieldName );

                    std::cout << "  name='" << field.getName() << "'"
                                 "  operation='" << field.getOpName() << "'"
                                 "  metadata='" << field.getMetadataName() << "'"
                                 "  field='" << field.getFieldName() << "'"
                                 "  value=" << field.getValue()
                              << std::endl;
                }
            }
            else
            {
                std::cout << "  no field(s) defined" << std::endl;
            }
        }
    }
    else
    {
        std::cout << "There are statistics object(s) found" << std::endl;
    }

    std::cout << "** end of statistics dump **" << std::endl;
    std::cout << std::endl;
}

int sample(int argc, char *argv[])
{
    string appPath = argv[0];
#ifdef WIN32
    char delim = '\\';
#else
    char delim = '/';
#endif
    size_t pos = appPath.find_last_of(delim);

    if (pos != string::npos)
    {
        workingPath = appPath.substr(0, pos + 1);
    }

    // Copy test video file to another location.
    copyFile(VIDEO_FILE, "global_test.avi");

    cout << "VMF sample: read/write GPS location and time" << endl << endl;

    const string FILE_NAME = "global_test.avi";
    const string META_SOURCE_NAME = "test-id";
    const string GPS_DESC = "gps";
    const string GPS_COORD_FIELD = "GPS";
    const string GPS_TIME_FIELD = "Time";
    const string GPS_SCHEMA_NAME = "gps_schema";
    const string GPS_METADATA_ITEM1 = "lat=53.78,lng=132.46";
    const string GPS_METADATA_ITEM2 = "lat=53.28,lng=131.87";
    const string GPS_METADATA_ITEM3 = "lat=52.95,lng=131.41";
    const string GPS_METADATA_ITEM4 = "lat=52.49,lng=130.98";
    const string GPS_STAT_NAME = "gps_stat";
    const string GPS_COUNT_COORD_NAME = "coord count";
    const string GPS_COUNT_TIME_NAME = "time count";
    const string GPS_STRCAT_COORD_NAME = "coord strcat";
    const string GPS_STRCAT_TIME_NAME = "time strcat";

    cout << "Adding metadata..." << endl;
    cout << "Opening file name '" << FILE_NAME << "'" << endl;

    // Open metadata stream
    umf::MetadataStream mdStream;
    if (!mdStream.open(FILE_NAME, umf::MetadataStream::Update))
    {
        cerr << "Can't open file " << FILE_NAME << endl;
        exit(1);
    }

    // Create GPS metadata schema
    std::shared_ptr<umf::MetadataSchema> gpsSchema = std::make_shared<umf::MetadataSchema>(GPS_SCHEMA_NAME);

    // Add description to the schema
    VMF_METADATA_BEGIN( GPS_DESC );
        VMF_FIELD_STR( GPS_COORD_FIELD );
        VMF_FIELD_STR( GPS_TIME_FIELD );
    VMF_METADATA_END( gpsSchema );

    // Get GPS metadata description
    std::shared_ptr<umf::MetadataDesc> gpsDesc = gpsSchema->findMetadataDesc( GPS_DESC );

    string t = "21.02.2013 18:35";
    cout << "Add metadata schema '" << GPS_SCHEMA_NAME << "'" << endl;

    // Add schema to metadata stream
    mdStream.addSchema(gpsSchema);

    // Register user operation: exactly one of two calls must be issued once for each user op
    // umf::StatOpFactory::registerUserOp( StrCatOp::opName(), StrCatOp::createInstance );
    umf::StatOpFactory::registerUserOp( StrCatOp::createInstance );

    // Set up statistics object(s)
    std::vector< umf::StatField > fields;
    fields.emplace_back( GPS_COUNT_COORD_NAME, GPS_SCHEMA_NAME, GPS_DESC, GPS_COORD_FIELD, umf::StatOpFactory::builtinName( umf::StatOpFactory::BuiltinOp::Count ));
    fields.emplace_back( GPS_COUNT_TIME_NAME, GPS_SCHEMA_NAME, GPS_DESC, GPS_TIME_FIELD, umf::StatOpFactory::builtinName( umf::StatOpFactory::BuiltinOp::Count ));
    fields.emplace_back( GPS_STRCAT_COORD_NAME, GPS_SCHEMA_NAME, GPS_DESC, GPS_COORD_FIELD, StrCatOp::opName() );
    fields.emplace_back( GPS_STRCAT_TIME_NAME, GPS_SCHEMA_NAME, GPS_DESC, GPS_TIME_FIELD, StrCatOp::opName() );
    mdStream.addStat( make_shared<umf::Stat>( GPS_STAT_NAME, fields, umf::Stat::UpdateMode::Disabled ));

    mdStream.getStat(GPS_STAT_NAME)->setUpdateTimeout( 50 );
    mdStream.getStat(GPS_STAT_NAME)->setUpdateMode( umf::Stat::UpdateMode::OnTimer );

    std::shared_ptr<umf::Metadata> gpsMetadata;

    t = "21.02.2013 18:45";
    cout << "Adding metadata's item '" << GPS_METADATA_ITEM1 << "' with associated time " << t << endl;
    
    // Create a metadata item
    gpsMetadata = std::make_shared<umf::Metadata>(gpsDesc);

    // Fill item fields
    gpsMetadata->push_back(umf::FieldValue(GPS_COORD_FIELD, GPS_METADATA_ITEM1));
    gpsMetadata->push_back(umf::FieldValue(GPS_TIME_FIELD, t));

    // Add to metadata a new item
    mdStream.add(gpsMetadata);

    t = "21.02.2013 19:28";
    cout << "Adding metadata's item '" << GPS_METADATA_ITEM2 << "' with associated time " << t << endl;

    gpsMetadata = std::make_shared<umf::Metadata>(gpsDesc);
    gpsMetadata->push_back(umf::FieldValue(GPS_COORD_FIELD, GPS_METADATA_ITEM2));
    gpsMetadata->push_back(umf::FieldValue(GPS_TIME_FIELD, t));
    mdStream.add(gpsMetadata);

//    mdStream.getStat(GPS_STAT_NAME).update();

    t = "21.02.2013 21:02";
    cout << "Adding metadata's item '" << GPS_METADATA_ITEM3 << "' with associated time " << t << endl;

    gpsMetadata = std::make_shared<umf::Metadata>(gpsDesc);
    gpsMetadata->push_back(umf::FieldValue(GPS_COORD_FIELD, GPS_METADATA_ITEM3));
    gpsMetadata->push_back(umf::FieldValue(GPS_TIME_FIELD, t));
    mdStream.add(gpsMetadata);

    t = "21.02.2013 23:19";
    cout << "Adding metadata's item '" << GPS_METADATA_ITEM4 << "' with associated time " << t << endl;

    gpsMetadata = std::make_shared<umf::Metadata>(gpsDesc);
    gpsMetadata->push_back(umf::FieldValue(GPS_COORD_FIELD, GPS_METADATA_ITEM4));
    gpsMetadata->push_back(umf::FieldValue(GPS_TIME_FIELD, t));
    mdStream.add(gpsMetadata);

    mdStream.getStat(GPS_STAT_NAME)->update();

    // dump statistics
    dumpStatistics( mdStream );

    cout << "Save metadata" << endl << endl;

    // Save metadata to video file and close metadata stream
    mdStream.save();
    mdStream.close();

    cout << "Loading metadata..." << endl;
    cout << "Opening file name '" << FILE_NAME << "'" << endl;
    
    // Open new metadata stream to load and print saved metadata
    umf::MetadataStream loadStream;
    if (!loadStream.open(FILE_NAME, umf::MetadataStream::ReadOnly))
    {
        cerr << "Can't open file " << FILE_NAME << endl;
        exit(1);
    }

    cout << "Loading schema '" << GPS_SCHEMA_NAME << "'" << endl;
    if (!loadStream.load(GPS_SCHEMA_NAME))
    {
        cerr << "Can't load schema " << GPS_SCHEMA_NAME << endl;
        exit(1);
    }

    loadStream.getStat(GPS_STAT_NAME)->setUpdateMode( umf::Stat::UpdateMode::Manual );
//    loadStream.getStat(GPS_STAT_NAME).update( true );

    // Select all metadata items from loaded schema
    auto dataSet = loadStream.queryBySchema(GPS_SCHEMA_NAME);

    // and print them to console
    for (size_t i = 0; i < dataSet.size(); i++)
    {
        cout << "Getting item " << i << endl;
        auto metadataItem = dataSet[i];

        string coord = metadataItem->getFieldValue(GPS_COORD_FIELD);
        cout << "\tGPS coordinates are: " << coord << endl;

        string time = metadataItem->getFieldValue(GPS_TIME_FIELD);
        cout << "\tAssociated time is: " << time << endl;
    }

    loadStream.getStat(GPS_STAT_NAME)->update( true );

    // dump statistics
    dumpStatistics( loadStream );

    // Close metadata stream
    loadStream.close();

    return 0;
}

int main(int argc, char *argv[])
{
    try
    {
        return sample(argc, argv);
    }
    catch( const umf::Exception& e )
    {
        std::cerr << "EXCEPTION : " << e.getMessage() << std::endl;
        return EXIT_FAILURE;
    }
}

