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
 * This sample demonstrates how to use built-in encryption algorithms for metatada saving.
 */

#include <iostream>
#include <fstream>

#include "vmf/vmf.hpp"

#include "encrypt_common.hpp"

using namespace std;
using namespace vmf;

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        cerr << "Error: Wrong number of arguments." << endl;
        cerr << "Usage: " << argv[0] << " <video_file> " << endl;
        return -1;
    }

    vmf::initialize();

    string videoFile(argv[1]);

    MetadataStream mds;

    for(EncryptionScope scope : {OneField, OneMetadata, AllFieldDesc, AllMetaDesc, AllSchema, Whole})
    {
        cout << "Encrypting ";
        if(scope == OneField)
        {
            cout << "one specific field";
        }
        else if(scope == OneMetadata)
        {
            cout << "one metadata record";
        }
        else if(scope == AllFieldDesc)
        {
            cout << "one field in channel";
        }
        else if(scope == AllMetaDesc)
        {
            cout << "one channel in schema";
        }
        else if(scope == AllSchema)
        {
            cout << "specific schema";
        }
        else if(scope == Whole)
        {
            cout << "whole metadata";
        }
        cout << "..." << endl;

        // cleanup existing metadata
        if (mds.open(videoFile, MetadataStream::Update |
            MetadataStream::IgnoreUnknownCompressor |
            MetadataStream::IgnoreUnknownEncryptor))
        {
            mds.load();
            mds.remove();
            mds.save();
            mds.close();
        }

        encrypt_builtin(videoFile, scope);

        cout << endl;

        // cleanup existing metadata
        if (mds.open(videoFile, MetadataStream::Update |
            MetadataStream::IgnoreUnknownCompressor |
            MetadataStream::IgnoreUnknownEncryptor))
        {
            mds.load();
            mds.remove();
            mds.save();
            mds.close();
        }

        encrypt_custom(videoFile, scope);

        cout << endl << endl;
    }

    vmf::terminate();
    return 0;
}
