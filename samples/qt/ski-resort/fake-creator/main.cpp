// Copyright (C) 2013, Intel Corporation, all rights reserved.

#include <iostream>
#include <fstream>

#include "vmf/vmf.hpp"

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

    vmf::Log::setVerbosityLevel(vmf::LOG_NO_MESSAGE);
    vmf::initialize();

    vmf::vmf_string coordFile(argv[1]);
    vmf::vmf_string videoFile(argv[2]);

    vmf::MetadataStream stream;
    
    if (!stream.open(videoFile, vmf::MetadataStream::ReadWrite))
    {
        std::cerr << "Can't open file '" + videoFile + "'" << std::endl;
        exit(1);
    }

    if (!stream.load(SKI_RESORT_SCHEMA))
    {
        std::cout << "Create ski resort schema..." << std::endl;

        std::vector<vmf::FieldDesc> gpsFields;
        gpsFields.push_back(vmf::FieldDesc(SKI_RESORT_GPS_FIELD_X, vmf::Variant::type_real));
        gpsFields.push_back(vmf::FieldDesc(SKI_RESORT_GPS_FIELD_Y, vmf::Variant::type_real));
        gpsFields.push_back(vmf::FieldDesc(SKI_RESORT_GPS_FIELD_TIME, vmf::Variant::type_integer));

        std::shared_ptr<vmf::MetadataDesc> gpsDesc(new vmf::MetadataDesc(SKI_RESORT_GPS_DESC, gpsFields));

        std::vector<vmf::FieldDesc> speedFields;
        speedFields.push_back(vmf::FieldDesc(SKI_RESORT_SPEED_FIELD_SPEED, vmf::Variant::type_real));
        speedFields.push_back(vmf::FieldDesc(SKI_RESORT_SPEED_FIELD_TIME, vmf::Variant::type_integer));
        std::shared_ptr<vmf::MetadataDesc> speedDesc(new vmf::MetadataDesc(SKI_RESORT_SPEED_DESC, speedFields));

        std::shared_ptr<vmf::MetadataSchema> appSchema(new vmf::MetadataSchema(SKI_RESORT_SCHEMA));
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

    vmf::vmf_integer frameTime = 0;
    while (true)
    {
        vmf::vmf_real latitude, longitude;
        coordinatesStream >> latitude >> longitude;
        if (coordinatesStream.eof())
            break;

        std::shared_ptr<vmf::Metadata> frameCoordinates(new vmf::Metadata(gpsDesc));

        frameCoordinates->setFieldValue(SKI_RESORT_GPS_FIELD_X, latitude);
        frameCoordinates->setFieldValue(SKI_RESORT_GPS_FIELD_Y, longitude);
        frameCoordinates->setFieldValue(SKI_RESORT_GPS_FIELD_TIME, frameTime);

        stream.add(frameCoordinates);

        frameTime += MSEC_IN_SEC;
    }

    std::shared_ptr<vmf::ICompressor> compressor = std::make_shared<vmf::ZLibCompressor>();
    stream.save(compressor);
    stream.close();

    vmf::terminate();

    return 0;
}
