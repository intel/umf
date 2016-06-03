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

int encrypt_builtin(const string& videoFile, EncryptionScope scope)
{
    cout << "Builtin encryption usage sample:" << endl;

    MetadataStream mdStream;
    if (!mdStream.open(videoFile, MetadataStream::Update |
        MetadataStream::IgnoreUnknownCompressor |
        MetadataStream::IgnoreUnknownEncryptor))
    {
        cerr << "Can't open file '" + videoFile + "'" << endl;
        exit(1);
    }

    std::shared_ptr<Encryptor> encryptor = std::make_shared<EncryptorDefault>("thereisnospoon");

    cout << "Generating metadata..." << endl;
    generateEncryptedMetadata(mdStream, scope);

    mdStream.setEncryptor(encryptor);

    cout << "Saving metadata with built-in encryption..." << endl;
    if(!mdStream.save())
    {
        cerr << "Can't save file '" + videoFile + "'" << endl;
        exit(1);
    }

    mdStream.close();

    cout << "Loading metadata back..." << endl;
    readAndDumpEncryptedMetadata(videoFile, encryptor);

    cout << "That's all!" << endl;
    return 0;
}
