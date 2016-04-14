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
#include "vmf/format_compressed.hpp"
#include "vmf/format_const.hpp"

namespace vmf
{

FormatCompressed::FormatCompressed(std::shared_ptr<Format> format,
                                   const std::string& compressorId,
                                   bool _ignoreUnknownCompressor)
    : format(format), compressorId(compressorId), ignoreUnknownCompressor(_ignoreUnknownCompressor)
{
    cSchema = std::make_shared<vmf::MetadataSchema>(COMPRESSED_DATA_SCHEMA_NAME);
    VMF_METADATA_BEGIN(COMPRESSED_DATA_DESC_NAME);
        VMF_FIELD_STR(COMPRESSION_ALGO_PROP_NAME);
        VMF_FIELD_STR(COMPRESSED_DATA_PROP_NAME);
    VMF_METADATA_END(cSchema);
}


FormatCompressed::~FormatCompressed()
{}


std::string FormatCompressed::store(
    const MetadataSet& set,
    const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
    const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
    const std::vector<std::shared_ptr<Stat>>& stats,
    const AttribMap& attribs
)
{
    std::string text = format->store(set, schemas, segments, stats, attribs);
    return compress(text);
}


Format::ParseCounters FormatCompressed::parse(
    const std::string& text,
    std::vector<MetadataInternal>& metadata,
    std::vector<std::shared_ptr<MetadataSchema>>& schemas,
    std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
    std::vector<std::shared_ptr<Stat>>& stats,
    AttribMap& attribs // nextId, checksum, etc
    )
{
    std::string decompressed = decompress(text);
    return format->parse(decompressed, metadata, schemas, segments, stats, attribs);
}


//used to set ID of metadata record
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


std::string FormatCompressed::compress(const std::string& input)
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
        std::vector<std::shared_ptr<MetadataStream::VideoSegment>> segments;
        std::vector<std::shared_ptr<Stat>> stats;
        AttribMap attribs{ {"nextId", to_string(nextId)}, };

        //create writer with no wrapping (like compression or encryption) enabled
        std::string outputString;
        outputString = getBackendFormat()->store(cSet, cSchemas, segments, stats, attribs);

        return outputString;
    }
    else
    {
        return input;
    }
}


std::string FormatCompressed::decompress(const std::string& input)
{
    //parse it as usual serialized VMF data, search for specific schemas
    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    std::vector<MetadataInternal> metadata;
    std::vector<std::shared_ptr<MetadataStream::VideoSegment>> segments;
    std::vector<std::shared_ptr<Stat>> stats;
    AttribMap attribs;

    //any exceptions thrown inside will be passed further
    Format::ParseCounters counter;
    counter = getBackendFormat()->parse(input, metadata, schemas, segments, stats, attribs);

    if(counter.schemas == 1 && schemas.size() == 1 && schemas[0]->getName() == COMPRESSED_DATA_SCHEMA_NAME)
    {
        auto algoIter = metadata[0].fields.find(COMPRESSION_ALGO_PROP_NAME),
             dataIter = metadata[0].fields.find(COMPRESSED_DATA_PROP_NAME),
             mapEnd   = metadata[0].fields.end();
        vmf_string algo = algoIter == mapEnd ? "" : algoIter->second.value;
        vmf_string data = dataIter == mapEnd ? "" : dataIter->second.value;

        if(algo.empty())
            VMF_EXCEPTION(vmf::InternalErrorException, "Algorithm name isn't specified");

        try
        {
            std::string decompressed;
            std::shared_ptr<Compressor> decompressor = Compressor::create(algo);
            // Compressed binary data should be represented in base64
            // because of '\0' symbols
            vmf_rawbuffer compressed = Variant::base64decode(data);
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

}//vmf
