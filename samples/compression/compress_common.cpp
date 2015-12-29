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

#include "compress_common.hpp"

#include <cmath>
#include <iostream>
#include <fstream>

using namespace std;
using namespace vmf;

const string GPS_DESC = "gps";
const string GPS_COORD_LAT_FIELD = "lat";
const string GPS_COORD_LNG_FIELD = "lng";
const string GPS_SCHEMA_NAME = "gps_coords_schema";
const double PI = 3.14159265358979323846;

void writeMetadataStream(MetadataStream& mdStream, const vmf_string& videoFile)
{
    // Open metadata stream
    if (!mdStream.open(videoFile, vmf::MetadataStream::ReadWrite))
    {
        std::cerr << "Can't open file '" + videoFile + "'" << std::endl;
        exit(1);
    }

    // Create a GPS metadata field descriptions
    shared_ptr<MetadataSchema> gpsSchema(new MetadataSchema(GPS_SCHEMA_NAME));

    VMF_METADATA_BEGIN(GPS_DESC);
        VMF_FIELD_REAL(GPS_COORD_LAT_FIELD);
        VMF_FIELD_REAL(GPS_COORD_LNG_FIELD);
    VMF_METADATA_END(gpsSchema);

    cout << "Adding metadata schema '" << GPS_SCHEMA_NAME << "'..." << endl;

    // Add schema to metadata stream
    mdStream.addSchema(gpsSchema);

    shared_ptr<Metadata> gpsMetadata;

    // Let there be an UFO moving around some point on Earth
    const int nPoints = 32768;
    for(int i = 0; i < nPoints; i++)
    {
        float lat =   37.235 + cos(i/25.0*2.0*PI) * 0.001;
        float lng = -115.811 + sin(i/25.0*2.0*PI) * 0.001;
        long long time = i;
        cout << "Adding metadata item 'lat " << lat << " lng " << lng << "'";
        cout << " with associated time " << time << endl;

        // Create a metadata item
        gpsMetadata = shared_ptr<Metadata>(new Metadata(gpsSchema->findMetadataDesc(GPS_DESC)));

        // Fill item fields
        gpsMetadata->push_back(FieldValue(GPS_COORD_LAT_FIELD, lat));
        gpsMetadata->push_back(FieldValue(GPS_COORD_LNG_FIELD, lng));
        gpsMetadata->setTimestamp(time);

        // Add to metadata a new item
        mdStream.add(gpsMetadata);
    }
}

void readVideoFile(const vmf_string& videoFile)
{
    cout << "Opening file name '" << videoFile << "'" << endl;

    // Open new metadata stream to load and print saved metadata
    MetadataStream loadStream;
    if (!loadStream.open(videoFile, MetadataStream::ReadOnly))
    {
        cerr << "Can't open file " << videoFile << endl;
        exit(1);
    }

    // When loading decompression is performed automatically
    // for registered and built-in compressors
    cout << "Loading schema '" << GPS_SCHEMA_NAME << "'" << endl;
    if (!loadStream.load(GPS_SCHEMA_NAME))
    {
        cerr << "Can't load schema " << GPS_SCHEMA_NAME << endl;
        exit(1);
    }

    // Select all metadata items from loaded schema
    auto dataSet = loadStream.queryBySchema(GPS_SCHEMA_NAME);

    // and print them to console
    for (size_t i = 0; i < dataSet.size(); i++)
    {
        cout << "Getting item " << i << endl;
        auto metadataItem = dataSet[i];

        vmf_real lat = metadataItem->getFieldValue(GPS_COORD_LAT_FIELD);
        vmf_real lng = metadataItem->getFieldValue(GPS_COORD_LNG_FIELD);
        cout << "\tGPS coordinates are: lat " << lat << " lng " << lng << endl;

        long long time = metadataItem->getTime();
        cout << "\tAssociated time is: " << time << endl;
    }

    // Close metadata stream
    loadStream.close();
}

