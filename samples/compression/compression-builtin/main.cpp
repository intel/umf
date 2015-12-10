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

/*
 * This sample demonstrates how to use built-in compression algorithms for metatada saving.
 * Compressed metadata loading is performed as usual for built-in compression algorithms.
 */

#include <iostream>
#include <fstream>

#include "vmf/vmf.hpp"

using namespace std;
using namespace vmf;

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Error: Wrong number of arguments." << std::endl;
        std::cerr << "Usage: " << argv[0] << " <video_file> " << std::endl;
        return -1;
    }

    vmf::Log::setVerbosityLevel(vmf::LOG_NO_MESSAGE);
    vmf::initialize();

    vmf::vmf_string videoFile(argv[1]);

    vmf::MetadataStream mdStream;
    // Open metadata stream
    if (!mdStream.open(videoFile, vmf::MetadataStream::ReadWrite))
    {
        std::cerr << "Can't open file '" + videoFile + "'" << std::endl;
        exit(1);
    }

    const string GPS_DESC = "gps";
    const string GPS_COORD_LAT_FIELD = "lat";
    const string GPS_COORD_LNG_FIELD = "lng";
    const string GPS_TIME_FIELD = "Time";
    const string GPS_SCHEMA_NAME = "gps_coords_schema";

    // Create a GPS metadata field descriptions
    vector<FieldDesc> fieldDesc;

    fieldDesc.push_back(FieldDesc(GPS_COORD_LAT_FIELD, Variant::type_real)); // GPS coordinates
    fieldDesc.push_back(FieldDesc(GPS_COORD_LNG_FIELD, Variant::type_real)); // GPS coordinates
    fieldDesc.push_back(FieldDesc(GPS_TIME_FIELD,      Variant::type_real)); // Associated time in msecs

    // Create GPS metadata description
    shared_ptr<MetadataDesc> gpsDesc(new MetadataDesc(GPS_DESC, fieldDesc));

    // Create GPS metadata schema
    shared_ptr<MetadataSchema> gpsSchema(new MetadataSchema(GPS_SCHEMA_NAME));

    // Add description to the schema
    gpsSchema->add(gpsDesc);

    cout << "Adding metadata schema '" << GPS_SCHEMA_NAME << "'..." << endl;

    // Add schema to metadata stream
    mdStream.addSchema(gpsSchema);

    shared_ptr<Metadata> gpsMetadata;

    // Let there be an UFO moving around some point on Earth
    const int nPoints = 32768;
    for(int i = 0; i < nPoints; i++)
    {
        float lat =   37.235 + cos(i/25.0*2.0*M_PI) * 0.001;
        float lng = -115.811 + sin(i/25.0*2.0*M_PI) * 0.001;
        float time = i/25.0;
        cout << "Adding metadata item 'lat " << lat << " lng " << lng << "'";
        cout << " with associated time " << time << endl;

        // Create a metadata item
        gpsMetadata = shared_ptr<Metadata>(new Metadata(gpsDesc));

        // Fill item fields
        gpsMetadata->push_back(FieldValue(GPS_COORD_LAT_FIELD, lat));
        gpsMetadata->push_back(FieldValue(GPS_COORD_LNG_FIELD, lng));
        gpsMetadata->push_back(FieldValue(GPS_TIME_FIELD, time));

        // Add to metadata a new item
        mdStream.add(gpsMetadata);
    }

    cout << "Saving metadata..." << endl << endl;

    // Save metadata to video file with built-in zlib-based compressor
    shared_ptr<ICompressor> compressor = make_shared<ZLibCompressor>();
    mdStream.save(compressor);

    // Close metadata stream
    mdStream.close();

    cout << "Loading metadata..." << endl;
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

        vmf_real time = metadataItem->getFieldValue(GPS_TIME_FIELD);
        cout << "\tAssociated time is: " << time << endl;
    }

    // Close metadata stream
    loadStream.close();

    vmf::terminate();

    return 0;
}
