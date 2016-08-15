// Copyright (C) 2013, Intel Corporation, all rights reserved.

#include <iostream>
#include <fstream>

#include "umf/umf.hpp"

#define MSEC_IN_SEC                     1000

#define SKI_RESORT_SCHEMA               "ski-resort"
#define SKI_RESORT_GPS_DESC             "gps-data"
#define SKI_RESORT_GPS_FIELD_X          "x"
#define SKI_RESORT_GPS_FIELD_Y          "y"
#define SKI_RESORT_GPS_FIELD_TIME       "time"
#define SKI_RESORT_SPEED_DESC           "speed"
#define SKI_RESORT_SPEED_FIELD_SPEED    "speed"
#define SKI_RESORT_SPEED_FIELD_TIME     "time"

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cerr << "Error: Wrong number of arguments." << std::endl;
        std::cerr << "Usage: " << argv[0] << " <coordinates_file> <video_file> " << std::endl;
        return -1;
    }

    umf::Log::setVerbosityLevel(umf::LOG_NO_MESSAGE);
    umf::initialize();

    umf::umf_string coordFile(argv[1]);
    umf::umf_string videoFile(argv[2]);

    umf::MetadataStream stream;
    
    if (!stream.open(videoFile, umf::MetadataStream::Update))
    {
        std::cerr << "Can't open file '" + videoFile + "'" << std::endl;
        exit(1);
    }

    if (!stream.load(SKI_RESORT_SCHEMA))
    {
        std::cout << "Create ski resort schema..." << std::endl;

        std::vector<umf::FieldDesc> gpsFields;
        gpsFields.push_back(umf::FieldDesc(SKI_RESORT_GPS_FIELD_X, umf::Variant::type_real));
        gpsFields.push_back(umf::FieldDesc(SKI_RESORT_GPS_FIELD_Y, umf::Variant::type_real));
        gpsFields.push_back(umf::FieldDesc(SKI_RESORT_GPS_FIELD_TIME, umf::Variant::type_integer));

        std::shared_ptr<umf::MetadataDesc> gpsDesc(new umf::MetadataDesc(SKI_RESORT_GPS_DESC, gpsFields));

        std::vector<umf::FieldDesc> speedFields;
        speedFields.push_back(umf::FieldDesc(SKI_RESORT_SPEED_FIELD_SPEED, umf::Variant::type_real));
        speedFields.push_back(umf::FieldDesc(SKI_RESORT_SPEED_FIELD_TIME, umf::Variant::type_integer));
        std::shared_ptr<umf::MetadataDesc> speedDesc(new umf::MetadataDesc(SKI_RESORT_SPEED_DESC, speedFields));

        std::shared_ptr<umf::MetadataSchema> appSchema(new umf::MetadataSchema(SKI_RESORT_SCHEMA));
        appSchema->add(gpsDesc);
        appSchema->add(speedDesc);

        stream.addSchema(appSchema);
    }
    else
    {
        std::cout << "Ski resort schema already created. Clear it." << std::endl;

        auto items = stream.queryByName(SKI_RESORT_GPS_DESC);
        stream.remove(items);

        items = stream.queryByName(SKI_RESORT_SPEED_DESC);
        stream.remove(items);
    }

    std::ifstream coordinatesStream;
    coordinatesStream.open(coordFile.c_str());
    if (!coordinatesStream.is_open())
    {
        std::clog << "Could not open file '" << coordFile <<"' for reading of coordinates.";
        return -1;
    }

    auto schema = stream.getSchema(SKI_RESORT_SCHEMA);
    if (!schema)
    {
        std::cerr << "Can't get GPS schema." << std::endl;
        exit(1);
    }

    auto gpsDesc = schema->findMetadataDesc(SKI_RESORT_GPS_DESC);

    umf::umf_integer frameTime = 0;
    while (true)
    {
        umf::umf_real latitude, longitude;
        coordinatesStream >> latitude >> longitude;
        if (coordinatesStream.eof())
            break;

        std::shared_ptr<umf::Metadata> frameCoordinates(new umf::Metadata(gpsDesc));

        frameCoordinates->setFieldValue(SKI_RESORT_GPS_FIELD_X, latitude);
        frameCoordinates->setFieldValue(SKI_RESORT_GPS_FIELD_Y, longitude);
        frameCoordinates->setFieldValue(SKI_RESORT_GPS_FIELD_TIME, frameTime);

        stream.add(frameCoordinates);

        frameTime += MSEC_IN_SEC;
    }

    stream.save();
    stream.close();

    umf::terminate();

    return 0;
}
