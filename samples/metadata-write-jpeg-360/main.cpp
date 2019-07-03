/* 
 * Copyright 2019 Intel(r) Corporation
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
 * to store data structures such as jpeg coordinates with associative time.
 */

#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cassert>

#include "umf/umf.hpp"


using namespace umf;
using namespace std;

string workingPath;

#define VIDEO_FILE (workingPath + "BlueSquare.avi")

void copyFile(const string& srcName, const char *dstName)
{
    ifstream src(srcName, ios::binary);
    ofstream dst(dstName, ios::binary);
    if (src && dst)
        dst << src.rdbuf();
    else
        UMF_EXCEPTION(IncorrectParamException, "Error copying '" + srcName + "' to '" + dstName + "'");
    //src.close();
    //dst.close();
}

int main(int argc, char *argv[])
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

    cout << "UMF sample: write of JPEG 360 metadata" << endl << endl;

    const string FILE_NAME = "global_test.avi";
    const string META_SOURCE_NAME = "test-id";
	const string jpeg_SCHEMA_NAME = "JPEG360Metadata";

	// Image metadata setup
    const string jpeg_DESC = "JPEG360ImageMetadata";
	const string jpeg_IMAGE_VERSION_FIELD = "JPEG360version";
	const string jpeg_IMAGE_MEDIA_TYPE_FIELD = "MediaType";
	const string jpeg_IMAGE_PROJECTION_TYPE_FIELD = "ProjectionType";
	const string jpeg_IMAGE_PHI_MIN_FIELD = "PhiMin";
	const string jpeg_IMAGE_PHI_MAX_FIELD = "PhiMax";
	const string jpeg_IMAGE_THETA_MAX_FIELD = "ThetaMax";
	const string jpeg_IMAGE_THETA_MIN_FIELD = "ThetaMin";
	const string jpeg_IMAGE_PHI_GRAVITY_FIELD = "PhiGravity";
	const string jpeg_IMAGE_THETA_GRAVITY_FIELD = "ThetaGravity";
	const string jpeg_IMAGE_COMPASS_PHI_FIELD = "CompassPhi";
	const string jpeg_IMAGE_BOX_REFERENCE_FIELD = "BoxReference";
	
	const int jpeg_METADATA_IMAGE_VERSION_ITEM1 = 1;
	const string jpeg_METADATA_IMAGE_MEDIA_TYPE_ITEM1 = "image/jpeg";
	const string jpeg_METADATA_IMAGE_PROJECTION_TYPE_ITEM1 = "Equirectangular";
	const double jpeg_METADATA_IMAGE_PHI_MIN_ITEM1 = -180.0;
	const double jpeg_METADATA_IMAGE_PHI_MAX_ITEM1 = 180.0;
	const double jpeg_METADATA_IMAGE_THETA_MAX_ITEM1 = 90.0;
	const double jpeg_METADATA_IMAGE_THETA_MIN_ITEM1 = -90.0;
	const double jpeg_METADATA_IMAGE_PHI_GRAVITY_ITEM1 = 0.0;
	const double jpeg_METADATA_IMAGE_THETA_GRAVITY_ITEM1 = -90.0;
	const double jpeg_IMAGE_COMPASS_PHI_ITEM1 = 0.0;
	const string jpeg_METADATA_IMAGE_BOX_REFERENCE_ITEM1 = "Legacy";

	// Viewport metadata setup
	const string jpeg_DESC2 = "JPEG360ViewportMetadata";
    const string jpeg_VIEWPORT_NUMBER_FIELD = "JPEG360ViewportNumber";
	const string jpeg_VIEWPORT_PHI_FIELD = "ViewportPhi";
	const string jpeg_VIEWPORT_THETA_FIELD = "ViewportTheta";
	const string jpeg_VIEWPORT_PHI_FOV_FIELD = "ViewportPhiFOV";
	const string jpeg_VIEWPORT_THETA_FOV_FIELD = "ViewportThetaFOV";
	const string jpeg_VIEWPORT_ROLL_FIELD = "ViewportRoll";
	
	
    const int jpeg_METADATA_VIEWPORT_NUMBER_ITEM1 = 0;
    const int jpeg_METADATA_VIEWPORT_NUMBER_ITEM2 = 1;
	const double jpeg_METADATA_VIEWPORT_PHI_ITEM1 = 0.0;
	const double jpeg_METADATA_VIEWPORT_PHI_ITEM2 = 90.0;
	const double jpeg_METADATA_VIEWPORT_THETA_ITEM1 = 0.0;
	const double jpeg_METADATA_VIEWPORT_THETA_ITEM2 = 0.0;
	const double jpeg_METADATA_VIEWPORT_PHI_FOV_ITEM1 = 90.0;
	const double jpeg_METADATA_VIEWPORT_PHI_FOV_ITEM2 = 45.0;
	const double jpeg_METADATA_VIEWPORT_THETA_FOV_ITEM1 = 45.0;
	const double jpeg_METADATA_VIEWPORT_THETA_FOV_ITEM2 = 45.0;
	const double jpeg_METADATA_VIEWPORT_ROLL_ITEM1 = 0.0;
	const double jpeg_METADATA_VIEWPORT_ROLL_ITEM2 = 0.0;

    cout << "Adding metadata..." << endl;
    cout << "Opening file name '" << FILE_NAME << "'" << endl;

    // Open metadata stream
    MetadataStream mdStream;
    if (!mdStream.open(FILE_NAME, MetadataStream::Update))
    {
        cerr << "Can't open file " << FILE_NAME << endl;
        exit(1);
    }

    // Create a jpeg metadata field descriptions
    vector<FieldDesc> fieldDesc;
	vector<FieldDesc> fieldDesc2;

	// Field descriptors for Image
	fieldDesc.push_back(FieldDesc(jpeg_IMAGE_VERSION_FIELD, umf::Variant::type_integer));
	fieldDesc.push_back(FieldDesc(jpeg_IMAGE_MEDIA_TYPE_FIELD, umf::Variant::type_string));
	fieldDesc.push_back(FieldDesc(jpeg_IMAGE_PROJECTION_TYPE_FIELD, umf::Variant::type_string));
	fieldDesc.push_back(FieldDesc(jpeg_IMAGE_PHI_MIN_FIELD, umf::Variant::type_real));
	fieldDesc.push_back(FieldDesc(jpeg_IMAGE_PHI_MAX_FIELD, umf::Variant::type_real));
	fieldDesc.push_back(FieldDesc(jpeg_IMAGE_THETA_MAX_FIELD, umf::Variant::type_real));
	fieldDesc.push_back(FieldDesc(jpeg_IMAGE_THETA_MIN_FIELD, umf::Variant::type_real));
	fieldDesc.push_back(FieldDesc(jpeg_IMAGE_PHI_GRAVITY_FIELD, umf::Variant::type_real));
	fieldDesc.push_back(FieldDesc(jpeg_IMAGE_THETA_GRAVITY_FIELD, umf::Variant::type_real));
	fieldDesc.push_back(FieldDesc(jpeg_IMAGE_COMPASS_PHI_FIELD, umf::Variant::type_real));
	fieldDesc.push_back(FieldDesc(jpeg_IMAGE_BOX_REFERENCE_FIELD, umf::Variant::type_string));

	// Field descriptors for Viewport 
    fieldDesc2.push_back(FieldDesc(jpeg_VIEWPORT_NUMBER_FIELD, umf::Variant::type_integer));
	fieldDesc2.push_back(FieldDesc(jpeg_VIEWPORT_PHI_FIELD, umf::Variant::type_real));
	fieldDesc2.push_back(FieldDesc(jpeg_VIEWPORT_THETA_FIELD, umf::Variant::type_real));
	fieldDesc2.push_back(FieldDesc(jpeg_VIEWPORT_PHI_FOV_FIELD, umf::Variant::type_real));
	fieldDesc2.push_back(FieldDesc(jpeg_VIEWPORT_THETA_FOV_FIELD, umf::Variant::type_real));
	fieldDesc2.push_back(FieldDesc(jpeg_VIEWPORT_ROLL_FIELD, umf::Variant::type_real));
   
    // Create jpeg metadata description
    shared_ptr<MetadataDesc> jpegDesc(new MetadataDesc(jpeg_DESC, fieldDesc));
	shared_ptr<MetadataDesc> jpegDesc2(new MetadataDesc(jpeg_DESC2, fieldDesc2));

    // Create jpeg metadata schema
    shared_ptr<MetadataSchema> jpegSchema(new MetadataSchema(jpeg_SCHEMA_NAME));

    // Add description to the schema
    jpegSchema->add(jpegDesc);
	jpegSchema->add(jpegDesc2);

    // Add schema to metadata stream
    mdStream.addSchema(jpegSchema);

    shared_ptr<Metadata> jpegMetadata;
	shared_ptr<Metadata> jpegMetadata2;

    // Create first metadata items for both sets
    jpegMetadata = shared_ptr<Metadata>(new Metadata(jpegDesc));
	jpegMetadata->push_back(FieldValue(jpeg_IMAGE_VERSION_FIELD, jpeg_METADATA_IMAGE_VERSION_ITEM1));
	jpegMetadata->push_back(FieldValue(jpeg_IMAGE_MEDIA_TYPE_FIELD, jpeg_METADATA_IMAGE_MEDIA_TYPE_ITEM1));
	jpegMetadata->push_back(FieldValue(jpeg_IMAGE_PROJECTION_TYPE_FIELD, jpeg_METADATA_IMAGE_PROJECTION_TYPE_ITEM1));
	jpegMetadata->push_back(FieldValue(jpeg_IMAGE_PHI_MIN_FIELD, jpeg_METADATA_IMAGE_PHI_MIN_ITEM1));
	jpegMetadata->push_back(FieldValue(jpeg_IMAGE_PHI_MAX_FIELD, jpeg_METADATA_IMAGE_PHI_MAX_ITEM1));
	jpegMetadata->push_back(FieldValue(jpeg_IMAGE_THETA_MAX_FIELD, jpeg_METADATA_IMAGE_THETA_MAX_ITEM1));
	jpegMetadata->push_back(FieldValue(jpeg_IMAGE_THETA_MIN_FIELD, jpeg_METADATA_IMAGE_THETA_MIN_ITEM1));
	jpegMetadata->push_back(FieldValue(jpeg_IMAGE_PHI_GRAVITY_FIELD, jpeg_METADATA_IMAGE_PHI_GRAVITY_ITEM1));
	jpegMetadata->push_back(FieldValue(jpeg_IMAGE_THETA_GRAVITY_FIELD, jpeg_METADATA_IMAGE_THETA_GRAVITY_ITEM1));
	jpegMetadata->push_back(FieldValue(jpeg_IMAGE_COMPASS_PHI_FIELD, jpeg_IMAGE_COMPASS_PHI_ITEM1));
	jpegMetadata->push_back(FieldValue(jpeg_IMAGE_BOX_REFERENCE_FIELD, jpeg_METADATA_IMAGE_BOX_REFERENCE_ITEM1));

	mdStream.add(jpegMetadata);

	jpegMetadata2 = shared_ptr<Metadata>(new Metadata(jpegDesc2));
    jpegMetadata2->push_back(FieldValue(jpeg_VIEWPORT_NUMBER_FIELD, jpeg_METADATA_VIEWPORT_NUMBER_ITEM1));
	jpegMetadata2->push_back(FieldValue(jpeg_VIEWPORT_PHI_FIELD, jpeg_METADATA_VIEWPORT_PHI_ITEM1));
	jpegMetadata2->push_back(FieldValue(jpeg_VIEWPORT_THETA_FIELD, jpeg_METADATA_VIEWPORT_THETA_ITEM1));
	jpegMetadata2->push_back(FieldValue(jpeg_VIEWPORT_PHI_FOV_FIELD, jpeg_METADATA_VIEWPORT_PHI_FOV_ITEM1));
	jpegMetadata2->push_back(FieldValue(jpeg_VIEWPORT_THETA_FOV_FIELD, jpeg_METADATA_VIEWPORT_THETA_FOV_ITEM1));
	jpegMetadata2->push_back(FieldValue(jpeg_VIEWPORT_ROLL_FIELD, jpeg_METADATA_VIEWPORT_ROLL_ITEM1));

	mdStream.add(jpegMetadata2);

	jpegMetadata2 = shared_ptr<Metadata>(new Metadata(jpegDesc2));
    jpegMetadata2->push_back(FieldValue(jpeg_VIEWPORT_NUMBER_FIELD, jpeg_METADATA_VIEWPORT_NUMBER_ITEM2));
	jpegMetadata2->push_back(FieldValue(jpeg_VIEWPORT_PHI_FIELD, jpeg_METADATA_VIEWPORT_PHI_ITEM2));
	jpegMetadata2->push_back(FieldValue(jpeg_VIEWPORT_THETA_FIELD, jpeg_METADATA_VIEWPORT_THETA_ITEM2));
	jpegMetadata2->push_back(FieldValue(jpeg_VIEWPORT_PHI_FOV_FIELD, jpeg_METADATA_VIEWPORT_PHI_FOV_ITEM2));
	jpegMetadata2->push_back(FieldValue(jpeg_VIEWPORT_THETA_FOV_FIELD, jpeg_METADATA_VIEWPORT_THETA_FOV_ITEM2));
	jpegMetadata2->push_back(FieldValue(jpeg_VIEWPORT_ROLL_FIELD, jpeg_METADATA_VIEWPORT_ROLL_ITEM2));

	mdStream.add(jpegMetadata2);

    cout << "Save metadata" << endl << endl;

    // Save metadata to video file and close metadata stream
    mdStream.save();
    mdStream.close();

    return 0;
}
