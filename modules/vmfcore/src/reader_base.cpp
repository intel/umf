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

#include "vmf/reader_base.hpp"

namespace vmf
{

std::string ReaderBase::decompress(const std::string& input, const std::string algo)
{
    if(algo.empty())
    {
        VMF_EXCEPTION(vmf::InternalErrorException, "Algorithm name isn't specified");
        return std::string();
    }

    std::string encoded(input);
    std::string decompressed;
    std::shared_ptr<Compressor> decompressor = Compressor::create(algo);
    // Compressed binary data should be represented in base64
    // because of '\0' symbols
    vmf_rawbuffer compressed = Variant::base64decode(encoded);
    decompressor->decompress(compressed, decompressed);

    return decompressed;
}

}
