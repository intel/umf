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
#ifndef __VMF_DATA_SOURCE_INTERFACE_H__
#define __VMF_DATA_SOURCE_INTERFACE_H__

/*!
* \file datasource.hpp
* \brief IDataSource interface header file
* \details 
*/

#include <vector>
#include "vmf/global.hpp"
#include "vmf/metadataschema.hpp"
#include "vmf/metadatastream.hpp"

namespace vmf
{
class PropertyImpl;
class MetadataStream;

/*! \class IDataSource
* \brief Interface to interacting with meta data provider
*/
class VMF_EXPORT IDataSource
{
public:
    /*!
     * \brief Open file with specified name
     * \param [in] fileName name of file
     * \param [in] mode file open mode
     * \throw DataStorageException
     */
    virtual void openFile(const MetaString& fileName, MetadataStream::OpenMode mode) = 0;

    /*!
     * \brief Close previously opened file
     * \throw DataStorageException
     */
    virtual void closeFile() = 0;

    /*!
     * \brief Loads all metadata belonging to the specified schema
     * \param [in] schemaName name of the specified schema
     * \param [out] stream stream to be filled by loaded metadata
     * \throw DataStorageException
     */
    virtual void loadSchema(const vmf::MetaString& schemaName, vmf::MetadataStream& stream) = 0;

    /*!
     * \brief Loads all metadata belonging to the specified property
     * \param [in] schemaName name of schema that contains specified property
     * \param [in] propertyName name of property
     * \param [out] stream stream to be filled by loaded metadata
     * \throw DataStorageException
     */
    virtual void loadProperty(const vmf::MetaString& schemaName, const vmf::MetaString& propertyName, vmf::MetadataStream& stream) = 0;

    /*!
     * \brief Saves all metadata belonging to the specified schema
     * \param [in] schemaName name of the specified schema
     * \param [in] stream stream with metadata
     * \throw DataStorageException
     */
    virtual void saveSchema(const MetaString& schemaName, const MetadataStream& stream) = 0;

    /*!
     * \brief Saves schema in the file with specified name
     * \param [in] schema schema to be saved
     * \throw DataStorageException
     */
    virtual void save(const std::shared_ptr<MetadataSchema>& schema) = 0;

    /*!
     * \brief Loads all MetadataSchemas described in current metafile
     * \param schemas [out] map to load schemas
     */
    virtual void load(std::map< MetaString, std::shared_ptr<MetadataSchema> >& schemas) = 0;

    /*!
     * \brief Load saved next identifier for new VMF objects
     * \return new numeric identifier
     */
    virtual IdType loadId() = 0;

    /*!
     * \brief Saved next identifier for new VMF objects
     * \param id that will be saved
     */
    virtual void save(const IdType& id) = 0;

    /*!
     * \brief Removes metadata by ID
     * \throw DataStorageException
     */
    virtual void remove(const std::vector<IdType>& removedIds) = 0;

    /*!
     * \brief Clears the file from VMF metadata
     */
    virtual void clear() = 0;

    /*!
     * \brief Removes Schema
     */
    virtual void removeSchema(const MetaString &schemaName) = 0;

    /*!
     * \brief Load checksum
     * \return checksum in std::string format
     */
    virtual std::string loadChecksum() = 0;
   
    /*!
     * \brief Saves media checksum
     * \param checksum that will be saved
     */
    virtual void saveChecksum(const MetaString& checksum) = 0;

    /*!
    * \brief Computes chechsum for current media data in opened file
    */
    virtual std::string computeChecksum(long long& XMPPacketSize, long long& XMPPacketOffset) = 0;

    /*
    * \brief Saves all video segments
    */
    virtual void saveVideoSegments(const std::vector<std::shared_ptr<MetadataStream::VideoSegment>> &videoSegments) = 0;

    /*
    * \brief Loads stored video segments
    */
    virtual void loadVideoSegments(std::vector<std::shared_ptr<MetadataStream::VideoSegment>> &videoSegments) = 0;

    /*
     * \brief Sets the compressor for compression operations at saving
     */
    virtual void setCompressorId(const vmf_string& id) = 0;
};

} /* vmf */

#endif /* __VMF_DATA_SOURCE_INTERFACE_H__ */
