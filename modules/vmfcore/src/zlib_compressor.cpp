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

#include "vmf/zlib_compressor.hpp"
#include "zlib.h"

namespace vmf {

void ZLibCompressor::compress(const MetaString& input, vmf_rawbuffer& output)
{
    //TODO: (length of string)*(size of character)
    size_t srcLen = input.length()*sizeof(MetaString::value_type);
    size_t destBound = compressBound(srcLen);
    size_t destLength = destBound;
    //should also keep the size of source data
    //for further decompression
    u_char* destBuf = (u_char*)malloc(destBound+sizeof(size_t));
    if(!destBuf)
    {
        VMF_EXCEPTION(InternalErrorException, "Out of memory");
    }

    *((size_t*)destBuf) = srcLen;
    u_char* toCompress = destBuf + sizeof(size_t);

    //level should be default or from 0 to 9 (regulates speed/size ratio)
    int level = Z_DEFAULT_COMPRESSION;
    int rcode = compress2(toCompress, &destLength, (const Bytef*)input.c_str(), srcLen, level);
    if(rcode != Z_OK)
    {
        if(rcode == Z_MEM_ERROR)
        {
            VMF_EXCEPTION(InternalErrorException, "Out of memory");
        }
        else
        {
            VMF_EXCEPTION(InternalErrorException, "Compressing error occured");
            //Z_BUF_ERROR if there was not enough room in the output buffer,
            //Z_STREAM_ERROR if the level parameter is invalid.
        }
    }

    output = std::move(vmf_rawbuffer((const char*)destBuf, destLength));
    free(destBuf);
}

void ZLibCompressor::decompress(const vmf_rawbuffer& input, MetaString& output)
{
    //input data also keeps the size of source data
    //since zlib doesn't save it at compression time
    size_t  compressedSize = input.size-sizeof(size_t);
    u_char* compressedBuf = (u_char*)input.data.get();
    size_t decompressedSize = *((size_t*)compressedBuf);
    compressedBuf += sizeof(size_t);
    size_t gotDecompressedSize = decompressedSize;
    u_char* decompressedBuf = (u_char*)malloc(decompressedSize);
    int rcode = uncompress(decompressedBuf, &gotDecompressedSize, compressedBuf, compressedSize);
    if(rcode != Z_OK)
    {
        if(rcode == Z_MEM_ERROR)
        {
            VMF_EXCEPTION(InternalErrorException, "Out of memory");
        }
        else
        {
            VMF_EXCEPTION(InternalErrorException, "Decompressing error occured");
            //Z_BUF_ERROR if there was not enough room in the output buffer,
            //Z_DATA_ERROR if the input data was corrupted or incomplete
        }
    }

    if(gotDecompressedSize != decompressedSize)
    {
        VMF_EXCEPTION(InternalErrorException,
                      "The size of decompressed data doesn't match to ")
    }

    output = std::move(MetaString((const char*)decompressedBuf, decompressedSize));
    free(decompressedBuf);
}

const MetaString& ZLibCompressor::getId()
{
    return id;
}

class ZLibCompressorRegistrator
{
public:
    ZLibCompressorRegistrator()
    {
        std::shared_ptr<ZLibCompressor> dc = std::make_shared<ZLibCompressor>();
        registerCompressor(std::dynamic_pointer_cast<ICompressor>(dc));
    }
};

const MetaString ZLibCompressor::id = "zlib";
static ZLibCompressorRegistrator zcr;

} /* vmf */
