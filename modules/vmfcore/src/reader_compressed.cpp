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

#include "vmf/reader_compressed.hpp"
#include "vmf/rwconst.hpp"

namespace vmf
{

/*
 * parseXXX() methods:
 * Deserialize -> decompress -> Deserialize
 */

bool ReaderCompressed::parseAll(const std::string& text, IdType& nextId,
                                std::string& filepath, std::string& checksum,
                                std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
                                std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                                std::vector<std::shared_ptr<MetadataInternal>>& metadata,
                                std::vector< Stat >& stats)
{
    std::string decompressed = decompress(text);
    return reader->parseAll(decompressed, nextId, filepath, checksum, segments, schemas, metadata, stats);
}


bool ReaderCompressed::parseSchemas(const std::string& text,
                                    std::vector<std::shared_ptr<MetadataSchema>>& schemas)
{
    std::string decompressed = decompress(text);
    return reader->parseSchemas(decompressed, schemas);
}


bool ReaderCompressed::parseMetadata(const std::string& text,
                                     const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                                     std::vector<std::shared_ptr<MetadataInternal>>& metadata)
{
    std::string decompressed = decompress(text);
    return reader->parseMetadata(decompressed, schemas, metadata);
}


bool ReaderCompressed::parseVideoSegments(const std::string& text,
                                          std::vector<std::shared_ptr<MetadataStream::VideoSegment> >& segments)
{
    std::string decompressed = decompress(text);
    return reader->parseVideoSegments(decompressed, segments);
}

bool ReaderCompressed::parseStats(const std::string& text, std::vector< Stat >& stats)
{
    std::string decompressed = decompress(text);
    return reader->parseStats(decompressed, stats);
}

std::string ReaderCompressed::decompress(const std::string& input)
{
    //parse it as usual serialized VMF XML, search for specific schemas
    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    if(!reader->parseSchemas(input, schemas))
    {
        VMF_EXCEPTION(vmf::InternalErrorException, "Failed to parse schemas in input data");
    }

    if(schemas.size() == 1 && schemas[0]->getName() == COMPRESSED_DATA_SCHEMA_NAME)
    {
        std::vector<std::shared_ptr<MetadataInternal>> metadata;
        if(!reader->parseMetadata(input, schemas, metadata))
        {
            VMF_EXCEPTION(vmf::InternalErrorException, "Failed to parse metadata in input data");
        }

        std::shared_ptr<Metadata> cMetadata = metadata[0];
        vmf_string algo = cMetadata->getFieldValue(COMPRESSION_ALGO_PROP_NAME);
        vmf_string encoded = cMetadata->getFieldValue(COMPRESSED_DATA_PROP_NAME);

        if(algo.empty())
        {
            VMF_EXCEPTION(vmf::InternalErrorException, "Algorithm name isn't specified");
        }

        try
        {
            std::string decompressed;
            std::shared_ptr<Compressor> decompressor = Compressor::create(algo);
            // Compressed binary data should be represented in base64
            // because of '\0' symbols
            vmf_rawbuffer compressed = Variant::base64decode(encoded);
            decompressor->decompress(compressed, decompressed);
            return decompressed;
        }
        catch(IncorrectParamException& ce)
        {
            if(ignoreUnknownCompressor)
            {
                return input;
            }
            else
            {
                VMF_EXCEPTION(IncorrectParamException, ce.what());
            }
        }
    }
    else
    {
        return input;
    }
}

}
