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

int compress_custom(const string& videoFile)
{
    cout << "Custom compression usage sample:" << endl;

    MetadataStream mdStream;
    if (!mdStream.open(videoFile, MetadataStream::Update | MetadataStream::IgnoreUnknownCompressor))
    {
        cerr << "Can't open file '" + videoFile + "'" << endl;
        exit(1);
    }

    cout << "Generating metadata..." << endl;
    generateMetadata(mdStream);

    cout << "Registering custom compressor..." << endl;
    shared_ptr<Compressor> compressor = make_shared<MyLZWCompressor>();
    Compressor::registerNew(compressor);

    cout << "Trying to save metadata using unregister compressor..." << endl;
    if (mdStream.save("unknown_compressor_ID"))
        cout << "VMF error: it shouldn't allow saving with unregistered compressor" << endl;
    else
        cout << "VMF is right: it can't use unregistered compressor!" << endl;

    cout << "Saving metadata using custom just registered compressor (" << compressor->getId() << ")..." << endl;
    mdStream.save(compressor->getId());


    mdStream.close();

    cout << "Loading metadata back..." << endl;
    readAndDumpMetadata(videoFile);

    cout << "Unregistering custom '" << compressor->getId() << "' compressor (it's optional here)..." << endl;
    vmf::Compressor::unregister(compressor->getId());

    cout << "That's all!" << endl;
    return 0;
}
