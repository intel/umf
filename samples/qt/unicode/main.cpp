// Copyright (C) 2013, Intel Corporation, all rights reserved.
#include <iostream>
#include <cstdio>

#include <QApplication>
#include <QMessageBox>
#include <QFile>

#include "vmf/vmf.hpp"

using namespace vmf;
using namespace std;

std::string workingPath;
 
#define VIDEO_FILE (workingPath + "/BlueSquare.avi")
#define TEST_FILE "global_test.avi"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    vmf::initialize();

    std::string appPath = argv[0];
#ifdef WIN32
    char delim = '\\';
#else
    char delim = '/';
#endif
    size_t pos = appPath.find_last_of(delim);

    if (pos != std::string::npos)
    {
        workingPath = appPath.substr(0, pos);
    }

    if (QFile::exists(TEST_FILE))
        QFile::remove(TEST_FILE);

    QFile::copy(VIDEO_FILE.c_str(), "global_test.avi");

    const vmf_string FILE_NAME = "global_test.avi";
    const vmf_string SCHEMA_NAME = "demo-unicode-schema";
    const vmf_string STRING_METADATA = "unicode-string-metadata";

    {
        auto schema = make_shared<MetadataSchema>(SCHEMA_NAME);
        auto desc = make_shared<MetadataDesc>(STRING_METADATA, Variant::type_string);
        schema->add(desc);
        MetadataStream stream;
        if (!stream.open(FILE_NAME, MetadataStream::Update))
        {
            cerr << "Failed to open file " << FILE_NAME << endl;
            return -1;
        }
        stream.addSchema(schema);
        auto metadata = make_shared<Metadata>(desc);
        // UTF-8 string with Russian text
        std::string str("\xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82\x21");
        metadata->addValue(str);
        stream.add(metadata);
        if (!stream.save())
        {
            cerr << "Failed to save file " << FILE_NAME << endl;
            return -1;
        }
        stream.close();
    }

    {
        MetadataStream stream;
        if (!stream.open(FILE_NAME, MetadataStream::ReadOnly))
        {
            cerr << "Failed to open file " << FILE_NAME << endl;
            return -1;
        }
        if (!stream.load(SCHEMA_NAME))
        {
            cerr << "Failed to load schema " << SCHEMA_NAME << endl;
            return -1;
        }
        auto stringMetadata = stream.queryByName(STRING_METADATA);
        auto savedMetadata = stringMetadata.at(0);
        std::string savedString = savedMetadata->at(0);
        QMessageBox::information(NULL, "VMF Demo Unicode", QString::fromUtf8(savedString.c_str()));
    }

    vmf::terminate();
    return 0;
}
