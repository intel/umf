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
#include "vmf/rwconst.hpp"

namespace vmf
{

/*
 * parseXXX() methods:
 * Try to read w/o decompression
 * if failed, decomperess and read once again
 */

bool ReaderBase::parseAll(const std::string& text, IdType& nextId, std::string& filepath, std::string& checksum,
                          std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
                          std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                          std::vector<std::shared_ptr<MetadataInternal>>& metadata)
{
    if(!internalParseAll(text, nextId, filepath, checksum, segments, schemas, metadata) ||
       (schemas.size() == 1 && schemas[0]->getName() == COMPRESSED_DATA_SCHEMA_NAME))
    {
        std::string decompressed = decompress(text);
        return internalParseAll(decompressed, nextId, filepath, checksum, segments, schemas, metadata);
    }
    else
    {
        return true;
    }
}


bool ReaderBase::parseSchemas(const std::string& text, std::vector<std::shared_ptr<MetadataSchema>>& schemas)
{
    if(!internalParseSchemas(text, schemas) ||
       (schemas.size() == 1 && schemas[0]->getName() == COMPRESSED_DATA_SCHEMA_NAME))
    {
        std::string decompressed = decompress(text);
        return internalParseSchemas(decompressed, schemas);
    }
    else
    {
        return true;
    }
}


bool ReaderBase::parseMetadata(const std::string& text,
                              const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                              std::vector<std::shared_ptr<MetadataInternal>>& metadata)
{
    if(!internalParseMetadata(text, schemas, metadata))
    {
        std::string decompressed = decompress(text);
        return internalParseMetadata(decompressed, schemas, metadata);
    }
    else
    {
        return true;
    }
}


bool ReaderBase::parseVideoSegments(const std::string& text, std::vector<std::shared_ptr<MetadataStream::VideoSegment> >& segments)
{
    if(!internalParseVideoSegments(text, segments))
    {
        std::string decompressed = decompress(text);
        return internalParseVideoSegments(decompressed, segments);
    }
    else
    {
        return true;
    }
}


std::string ReaderBase::decompress(const std::string& input)
{
    //parse it as usual serialized VMF XML, search for specific schemas
    std::vector<std::shared_ptr<MetadataStream::VideoSegment>> segments;
    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    std::vector<std::shared_ptr<MetadataInternal>> metadata;
    std::string filePath, checksum;
    IdType nextId;
    if(!internalParseAll(input, nextId, filePath, checksum, segments, schemas, metadata))
    {
        VMF_EXCEPTION(vmf::InternalErrorException, "Failed to parse input data");
    }

    if(schemas.size() == 1 && schemas[0]->getName() == COMPRESSED_DATA_SCHEMA_NAME)
    {
        std::shared_ptr<Metadata> cMetadata = metadata[0];
        vmf_string algo = cMetadata->getFieldValue(COMPRESSION_ALGO_PROP_NAME);
        vmf_string encoded = cMetadata->getFieldValue(COMPRESSED_DATA_PROP_NAME);

        if(algo.empty())
        {
            VMF_EXCEPTION(vmf::InternalErrorException, "Algorithm name isn't specified");
        }

        std::string decompressed;
        std::shared_ptr<Compressor> decompressor = Compressor::create(algo);
        // Compressed binary data should be represented in base64
        // because of '\0' symbols
        vmf_rawbuffer compressed = Variant::base64decode(encoded);
        decompressor->decompress(compressed, decompressed);

        return decompressed;
    }
    else
    {
        return input;
    }
}

}
