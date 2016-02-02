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

void generateMetadata(MetadataStream& mdStream)
{
    mdStream.remove();
    mdStream.save();
    mdStream.close();
    mdStream.reopen(MetadataStream::OpenModeFlags::Update);

    // Create a GPS metadata field descriptions
    shared_ptr<MetadataSchema> gpsSchema(new MetadataSchema(GPS_SCHEMA_NAME));

    VMF_METADATA_BEGIN(GPS_DESC);
        VMF_FIELD_REAL(GPS_COORD_LAT_FIELD);
        VMF_FIELD_REAL(GPS_COORD_LNG_FIELD);
    VMF_METADATA_END(gpsSchema);

    cout << "Adding metadata schema '" << GPS_SCHEMA_NAME << "':" << endl;

    // Add schema to metadata stream
    mdStream.addSchema(gpsSchema);

    shared_ptr<Metadata> gpsMetadata;

    // Let there be an UFO moving around some point on Earth
    const int nPoints = 1<<10;
    for(int i = 0; i < nPoints; i++)
    {
        float lat = float(  37.235 + cos(i/25.0*2.0*PI) * 0.001);
        float lng = float(-115.811 + sin(i/25.0*2.0*PI) * 0.001);
        long long time = i;
        if (i<3) cout << "\t[" << i << "] 'lat " << lat << " lng " << lng << '\'' << " time " << time << endl;
        else cout << '.';

        // Create a metadata item
        gpsMetadata = shared_ptr<Metadata>(new Metadata(gpsSchema->findMetadataDesc(GPS_DESC)));

        // Fill item fields
        gpsMetadata->push_back(FieldValue(GPS_COORD_LAT_FIELD, lat));
        gpsMetadata->push_back(FieldValue(GPS_COORD_LNG_FIELD, lng));
        gpsMetadata->setTimestamp(time);

        // Add to metadata a new item
        mdStream.add(gpsMetadata);
    }
    cout << "\n\t" << nPoints << " items." << endl;
}

void readAndDumpMetadata(const vmf_string& videoFile)
{
    cout << "Opening file name '" << videoFile << "'" << endl;

    // Open new metadata stream to load and print saved metadata
    MetadataStream loadStream;
    if (!loadStream.open(videoFile, MetadataStream::ReadOnly))
    {
        cerr << "Can't open file " << videoFile << endl;
        exit(1);
    }

    // Get all schemas
    vector<string> schemas = loadStream.getAllSchemaNames();
    // and dump all the related data to console
    for(size_t sNum = 0; sNum < schemas.size(); sNum++)
    {
        string sName = schemas[sNum];
        cout << "* (" << sNum << ") [schema]: " << sName << endl;
        if(!loadStream.load(sName))
        {
            cerr << "Error loading schema " << sName << endl;
            exit(1);
        }
        vector< shared_ptr<MetadataDesc> > mDescs;
        mDescs = loadStream.getSchema(sName)->getAll();
        for(size_t setNum = 0; setNum < mDescs.size(); setNum++)
        {
            auto mDesc = mDescs[setNum];
            string setName = mDesc->getMetadataName();
            MetadataSet mdSet = loadStream.queryByName(setName);
            cout << "\t* (" << sNum << "." << setNum << ") [set]: ";
            cout << setName << "(" << mdSet.size() << " items)" << endl;
            if(mdSet.empty()) continue;
            vector<string> fields(mdSet[0]->getFieldNames());
            int itemNum = 0;
            for(const auto& item : mdSet)
            {
                if (itemNum++ <= 3)
                {
                    cout << "\t\t* (" << sNum << "." << setNum << "." << itemNum << ") { ";
                    const char * separator = "";
                    for (const auto& f : fields)
                    {
                        cout << separator << f << "=";
                        try { cout << item->getFieldValue(f).toString(); }
                        catch (vmf::Exception& e) { cout << '<' << e.what() << '>'; }
                        separator = ", ";
                    }
                    cout << " }";
                    long long t = item->getTime();
                    if (t >= 0) cout << ", time " << t;
                    cout << endl;
                }
                else
                    cout << '.';
            }
            cout << "\n\t\t" << itemNum << " items." << endl;
        }
    }

    // Close metadata stream
    loadStream.close();
}

