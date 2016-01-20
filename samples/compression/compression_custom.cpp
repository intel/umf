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
#include <stdexcept>

#include "vmf/vmf.hpp"

#include "compress_common.hpp"
#include "my_lzw.hpp"

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

    //vmf::Log::setVerbosityLevel(vmf::LOG_NO_MESSAGE);
    vmf::initialize();

    vmf::vmf_string videoFile(argv[1]);

    vmf::MetadataStream mdStream;
    writeMetadataStream(mdStream, videoFile);

    cout << "Saving metadata..." << endl << endl;

    // Save metadata to video file with custom compressor
    shared_ptr<Compressor> compressor = make_shared<MyLZWCompressor>();

    // Register compressor before it could be used in saving and loading
    vmf::Compressor::registerNew(compressor);

    // Trying to load or save file with unregistered compressor
    // doesn't produce exceptions, just returns false
    if(mdStream.save("unknown_compressor_ID"))
    {
        cout << "VMF error: it shouldn't allow saving with unregistered compressor" << endl;
    }
    else
    {
        cout << "VMF is right: it can't use unregistered compressor!" << endl;
    }

    mdStream.save(compressor->getId());


    // Close metadata stream
    mdStream.close();

    cout << "Loading metadata..." << endl;
    readVideoFile(videoFile);

    //The following action is optional
    vmf::Compressor::unregister(compressor->getId());

    vmf::terminate();

    return 0;
}
