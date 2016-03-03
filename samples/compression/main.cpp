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

#include "compress_common.hpp"

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

    string videoFile(argv[1]);

    MetadataStream mds;

    // cleanup existing metadata
    if (mds.open(videoFile, MetadataStream::Update | MetadataStream::IgnoreUnknownCompressor))
    {
        mds.load();
        mds.remove();
        mds.save();
        mds.close();
    }

    compress_builtin(videoFile);

    cout << endl << endl;

    // cleanup existing metadata
    if (mds.open(videoFile, MetadataStream::Update | MetadataStream::IgnoreUnknownCompressor))
    {
        mds.load();
        mds.remove();
        mds.save();
        mds.close();
    }

    compress_custom(videoFile);

    return 0;
}
