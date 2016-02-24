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

#include "vmf/writer_compressed.hpp"
#include "vmf/rwconst.hpp"

namespace vmf
{

std::string WriterCompressed::store(const std::vector<std::shared_ptr<MetadataSchema>>& schemas)
{
    std::string text = writer->store(schemas);
    return compress(text);
}

std::string WriterCompressed::store(const MetadataSet& set)
{
    std::string text = writer->store(set);
    return compress(text);
}

std::string WriterCompressed::store(const IdType& nextId, const std::string& filepath,
                                    const std::string& checksum,
                                    const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
                                    const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                                    const MetadataSet& set,
                                    const std::vector< Stat >& stats)
{
    std::string text = writer->store(nextId, filepath, checksum, segments, schemas, set, stats);
    return compress(text);
}

std::string WriterCompressed::store(const std::shared_ptr<MetadataStream::VideoSegment>& spSegment)
{
    std::string text = writer->store(spSegment);
    return compress(text);
}

std::string WriterCompressed::store(const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments)
{
    std::string text = writer->store(segments);
    return compress(text);
}

std::string WriterCompressed::store(const Stat& stat)
{
    std::string text = writer->store(stat);
    return compress(text);
}

std::string WriterCompressed::store(const std::vector<Stat>& stats)
{
    std::string text = writer->store(stats);
    return compress(text);
}

class MetadataAccessor: public Metadata
{
public:
    MetadataAccessor( const std::shared_ptr< MetadataDesc >& spDescription )
      : Metadata(spDescription) { }
    MetadataAccessor( const Metadata& oMetadata )
      : Metadata(oMetadata) { }
    using Metadata::setId;
    virtual ~MetadataAccessor() {}
};


std::string WriterCompressed::compress(const std::string& input)
{
    if(!compressorId.empty())
    {
        std::shared_ptr<Compressor> compressor = Compressor::create(compressorId);
        vmf_rawbuffer compressedBuf;
        compressor->compress(input, compressedBuf);
        // Compressed binary data should be represented in base64
        // because of '\0' symbols
        std::string compressed = Variant::base64encode(compressedBuf);

        //Store compressed data in a format of current implementation
        std::shared_ptr<vmf::MetadataSchema> cSchema;
        cSchema = std::make_shared<vmf::MetadataSchema>(COMPRESSED_DATA_SCHEMA_NAME);
        VMF_METADATA_BEGIN(COMPRESSED_DATA_DESC_NAME);
            VMF_FIELD_STR(COMPRESSION_ALGO_PROP_NAME);
            VMF_FIELD_STR(COMPRESSED_DATA_PROP_NAME);
        VMF_METADATA_END(cSchema);

        std::shared_ptr<Metadata> cMetadata;
        cMetadata = std::make_shared<Metadata>(cSchema->findMetadataDesc(COMPRESSED_DATA_DESC_NAME));
        cMetadata->push_back(FieldValue(COMPRESSION_ALGO_PROP_NAME, compressor->getId()));
        cMetadata->push_back(FieldValue(COMPRESSED_DATA_PROP_NAME,  compressed));

        MetadataAccessor metadataAccessor(*cMetadata);
        metadataAccessor.setId(0);
        cMetadata = std::make_shared<Metadata>(metadataAccessor);

        MetadataSet cSet;
        cSet.push_back(cMetadata);
        std::vector< std::shared_ptr<MetadataSchema> > cSchemas;
        cSchemas.push_back(cSchema);

        const IdType nextId = 1;
        const std::string filePath = "";
        const std::string checksum = "";
        std::vector<std::shared_ptr<MetadataStream::VideoSegment>> segments;
        std::vector<Stat> stats;

        //create writer with no compression enabled
        std::string outputString;
        outputString = writer->store(nextId, filePath, checksum, segments, cSchemas, cSet, stats);

        return outputString;
    }
    else
    {
        return input;
    }
}

}
