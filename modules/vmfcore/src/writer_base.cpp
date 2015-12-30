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

#include "vmf/writer_base.hpp"

namespace vmf
{

std::string WriterBase::compress(const std::string& input)
{
    std::string compressed;
    if(!compressorId.empty())
    {
        std::shared_ptr<Compressor> compressor = Compressor::create(compressorId);
        if(compressor)
        {
            vmf_rawbuffer compressedBuf;
            compressor->compress(input, compressedBuf);
            // Compressed binary data should be represented in base64
            // because of '\0' symbols
            compressed = Variant::base64encode(compressedBuf);
        }
        else
        {
            VMF_EXCEPTION(IncorrectParamException,
                          "Unregistered compression algorithm: " + compressorId);
        }
    }
    else
    {
        compressed = input;
    }
    return compressed;
}

}
