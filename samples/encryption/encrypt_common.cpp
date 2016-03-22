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

#include "encrypt_common.hpp"

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

void generateEncryptedMetadata(MetadataStream &mdStream, EncryptionScope scope)
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

    //set encryption for all fields/metadata/schema/stream
    std::shared_ptr< MetadataDesc > metadesc = gpsSchema->findMetadataDesc(GPS_DESC);
    FieldDesc& field = metadesc->getFieldDesc(GPS_COORD_LNG_FIELD);
    if(scope == EncryptionScope::Whole)
    {
        mdStream.setUseEncryption(true);
    }
    else if(scope == EncryptionScope::AllSchema)
    {
        gpsSchema->setUseEncryption(true);
    }
    else if(scope == EncryptionScope::AllMetaDesc)
    {
        metadesc->setUseEncryption(true);
    }
    else if(scope == EncryptionScope::AllFieldDesc)
    {
        field.useEncryption = true;
    }

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

        //Encrypt specified fields/records or not
        bool encryptRecord = false, encryptField = false;
        if(i == 0)
        {
            if(scope == EncryptionScope::OneField)
            {
                encryptField = true;
            }
            if(scope == EncryptionScope::OneMetadata)
            {
                encryptRecord = true;
            }
        }

        // Fill item fields
        gpsMetadata->push_back(FieldValue(GPS_COORD_LAT_FIELD, lat, encryptField));
        gpsMetadata->push_back(FieldValue(GPS_COORD_LNG_FIELD, lng));
        gpsMetadata->setTimestamp(time);
        gpsMetadata->setUseEncryption(encryptRecord);

        // Add to metadata a new item
        mdStream.add(gpsMetadata);
    }
    cout << "\n\t" << nPoints << " items." << endl;
}


void readAndDumpEncryptedMetadata(const vmf_string& videoFile, std::shared_ptr<Encryptor> encryptor)
{
    cout << "Opening file name '" << videoFile << "'" << endl;

    // Open new metadata stream to load and print saved metadata
    MetadataStream loadStream;
    loadStream.setEncryptor(encryptor);
    if (!loadStream.open(videoFile, MetadataStream::ReadOnly))
    {
        cerr << "Can't open file " << videoFile << endl;
        exit(1);
    }

    if(loadStream.getUseEncryption())
    {
        cout << "The whole stream is encrypted" << endl;
    }

    // Get all schemas
    vector<string> schemas = loadStream.getAllSchemaNames();
    // and dump all the related data to console
    for(size_t sNum = 0; sNum < schemas.size(); sNum++)
    {
        string sName = schemas[sNum];
        std::shared_ptr<MetadataSchema> schemaPtr = loadStream.getSchema(sName);
        bool schemaEncrypted = schemaPtr->getUseEncryption();
        cout << "* (" << sNum << ") [schema]: " << sName;
        if(schemaEncrypted)
        {
            cout << ", encrypted";
        }
        cout << endl;

        if(!loadStream.load(sName))
        {
            cerr << "Error loading schema " << sName << endl;
            exit(1);
        }

        vector< shared_ptr<MetadataDesc> > mDescs = schemaPtr->getAll();
        for(size_t setNum = 0; setNum < mDescs.size(); setNum++)
        {
            shared_ptr<MetadataDesc> mDesc = mDescs[setNum];
            string setName = mDesc->getMetadataName();
            bool metadescEncrypted = mDesc->getUseEncryption();
            MetadataSet mdSet = loadStream.queryByName(setName);
            cout << "\t* (" << sNum << "." << setNum << ") [set]: ";
            cout << setName << "(" << mdSet.size() << " items)";


            if(mdSet.empty()) continue;
            vector<string> fields(mdSet[0]->getFieldNames());
            cout << " {";
            const char * separator = "";
            for(FieldDesc fDesc : mDesc->getFields())
            {
                cout << separator << fDesc.name;
                if(fDesc.useEncryption)
                {
                    cout << " encrypted";
                }
                separator = ", ";
            }
            cout << "}";
            if(metadescEncrypted)
            {
                cout << ", encrypted";
            }
            cout << endl;
            int itemNum = 0;
            for(const std::shared_ptr<Metadata>& item : mdSet)
            {
                if (itemNum++ <= 3)
                {
                    cout << "\t\t* (" << sNum << "." << setNum << "." << itemNum << ") { ";
                    separator = "";
                    for (const string& f : fields)
                    {
                        cout << separator << f << "=";
                        try { cout << item->getFieldValue(f).toString(); }
                        catch (vmf::Exception& e) { cout << '<' << e.what() << '>'; }
                        if(item->findField(f)->getUseEncryption())
                        {
                            cout << " encrypted";
                        }
                        separator = ", ";
                    }
                    cout << " }";
                    long long t = item->getTime();
                    if (t >= 0) cout << ", time " << t;
                    if(item->getUseEncryption()) cout << ", encrypted";
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

