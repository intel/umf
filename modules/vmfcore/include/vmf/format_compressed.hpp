/*
* Copyright 2016 Intel(r) Corporation
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

/*!
* \file format_compressed.hpp
* \brief Serialization and deserialization based on instances of Compressor and Format classes
*/

#ifndef VMF_FORMAT_COMPRESSED_H
#define VMF_FORMAT_COMPRESSED_H

#include "vmf/format.hpp"

namespace vmf
{
/*!
* \class FormatCompressed
* \brief FormatCompressed class is a %Format interface extension for %Compressor use
*/
class VMF_EXPORT FormatCompressed : public Format
{
public:
    /*!
     * \brief Constructor taking compressor ID and format to be used at data reading/writing
     * \note This class can be used for reading w/o specifying the compressor ID
     * \note To use both compression and encryption
     * pass this class to FormatEncrypted constructor as the format argument
     * \param format Shared pointer to instance of Format class
     * \param compressorId ID of compression algorithm
     * \param _ignoreUnknownCompressor Flag specifying what to do with unknown compressor:
     * throw an exception (false) or pass compressed data as VMF metadata
     */
    FormatCompressed(std::shared_ptr<Format> format,
                     const std::string& compressorId,
                     bool _ignoreUnknownCompressor = false);

    /*!
    * \brief Class destructor
    */
    virtual ~FormatCompressed();

    /*!
     * \brief Serialize input metadata and related stuff to string
     */
    virtual std::string store(
        const MetadataSet& set,
        const std::vector<std::shared_ptr<MetadataSchema>>& schemas = {},
        const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments = {},
        const std::vector<std::shared_ptr<Stat>>& stats = {},
        const AttribMap& attribs = AttribMap() // nextId, checksum, etc
        );

    /*!
     * \brief Deserialize input string to metadata and related stuff
     * \param text input string
     * \param metadata Metadata records
     * \param schemas Schemas of the metadata
     * \param segments Video segments
     * \param stats Statistical object
     * \param attribs Attributes like nextId, checksum, etc.
     * \return Numbers of items read by categories
     */
    virtual ParseCounters parse(
        const std::string& text,
        std::vector<MetadataInternal>& metadata,
        std::vector<std::shared_ptr<MetadataSchema>>& schemas,
        std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
        std::vector<std::shared_ptr<Stat>>& stats,
        AttribMap& attribs // nextId, checksum, etc
        );

    virtual std::shared_ptr<Format> getBackendFormat()
    {
        return format ? format->getBackendFormat() : nullptr;
    }

protected:
    /*!
    * \brief Performs compression of text data
    * \param input Input string
    * \return Compressed string
    */
    virtual std::string compress(const std::string& input);

    /*!
    * \brief Performs decompression of previously compressed data
    * \param input Input string
    * \return Decompressed string
    */
    virtual std::string decompress(const std::string& input);

    std::shared_ptr<Format> format;
    std::shared_ptr<vmf::MetadataSchema> cSchema;
    std::string compressorId;
    bool ignoreUnknownCompressor;
};

}//vmf

#endif //VMF_FORMAT_COMPRESSED_H
