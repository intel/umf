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
/*!
* \file metadatastream.hpp
* \brief %MetadataStream class header file
*/
#pragma once
#ifndef __METADATA_STREAM_H__
#define __METADATA_STREAM_H__

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

#include "vmf/global.hpp"
#include "vmf/metadatainternal.hpp"
#include "vmf/metadataset.hpp"
#include "vmf/metadataschema.hpp"
#include "vmf/compressor.hpp"
#include "vmf/iquery.hpp"

#include <map>
#include <memory>
#include <vector>
#include <algorithm>

namespace vmf
{
class IDataSource;
class IReader;
class IWriter;
class Format;

/*!
* \class MetadataStream
* \brief %MetadataStream provides functionality to read/write
* metadata to media file
*/
class VMF_EXPORT MetadataStream : public IQuery
{
public:
    /*!
    * \brief File open mode flags
    */
    enum OpenModeFlags
    {
        InMemory  = 0, /**< Stream data are in-memory */
        ReadOnly  = 1, /**< Open file for read only */
        Update = 2, /**< Open file for read and write */
        IgnoreUnknownCompressor = 4 /**< Represent compressed data as VMF metadata if decompressor is unknown*/
    };
    typedef int OpenMode;

    class VMF_EXPORT VideoSegment
    {
    public:
        VideoSegment();

        VideoSegment( const std::string& title, double fps, long long timeStart,
                      long long duration = 0, long width = 0, long height = 0 );

        ~VideoSegment();

        std::string getTitle() const;
        void setTitle(const std::string& _title);

        double getFPS() const;
        void setFPS(double fps);

        long long getDuration() const;
        void setDuration(long long duration);

        long long getTime() const;
        void setTime(long long timeStart);

        void getResolution(long& width, long& height) const;
        void setResolution(long width, long height);

    private:
        std::string title;
        double fps;
        long long timeStart;
        long long duration;
        long width;
        long height;
    };

    /*!
    * \brief Default class constructor
    * \throw NotInitializedException if Video metadata framework library is no initialized
    */
    MetadataStream(void);

    /*!
    * \brief Class destructor
    */
    virtual ~MetadataStream(void);

    /*!
    * \brief Open metadata stream
    * \param sFilePath [in] path to media file
    * \param eMode [in] open mode
    * \return Open result
    */
    bool open( const std::string& sFilePath, OpenMode eMode = ReadOnly );

    /*!
    * \brief Reopen a previously closed metadata stream. 
    * \param eMode [in] open mode
    * \return Open result
    */
    bool reopen( OpenMode eMode = ReadOnly );

    /*!
    * \brief Load metadata with specified scheme
    * \param sSchemaName [in] schema name
    * \return operation result
    * \details The function loads metadata with specified scheme
    * or all metadata if schema name is empty string
    */
    bool load( const std::string& sSchemaName = "" );

    /*!
    * \brief Load metadata with specified name from selected schema
    * \param sSchemaName [in] schema name
    * \param sMetadataName [in] metadata item name
    */
    bool load( const std::string& sSchemaName, const std::string& sMetadataName );

    /*!
    * \brief Save loaded data to media file
    * \param compressorId String identifying compression to be used at saving
    * (empty string means no compression)
    * \return Save operation result
    */
    bool save(const vmf_string& compressorId = vmf_string());


    /*!
    * \brief Save the in-memory metadata to a different video file.
    * \param sFilePath the path of the new file.
    * \param compressorId String identifying compression to be used at saving
    * (empty string means no compression)
    * \return true if succeed.
    */
    bool saveTo(const std::string& sFilePath, const vmf_string& compressorId = vmf_string() );


    /*!
    * \brief Close previously opened media file
    */
    void close();

    /*!
    * \brief Get metadata by its identifier
    * \param id [in] metadata identifier
    * \details The function return pointer to metadata with specified id or null pointer if identifier not found
    */
    std::shared_ptr< Metadata > getById( const IdType& id ) const;

    /*!
    * \brief Add new metadata item
    * \param spMetadata [in] pointer to metadata object
    * \return identifier of added metadata object
    * \throw ValidateException if metadata is not valid to selected scheme or description
    */
    IdType add( std::shared_ptr< Metadata >& spMetadata);

    /*!
    * \brief Add new metadata item
    * \param spMetadataInternal [in] pointer to metadataInternal object
    * \return identifier of added metadata object
    * \throw ValidateException if metadata is not valid to selected scheme or description
    * \throw IncorrectParamException if metadata with such id is already exists
    */
    IdType add( std::shared_ptr< MetadataInternal >& spMetadataInternal);

    /*!
    * \brief Remove metadata by their id
    * \param id [in] metadata identifier
    * \return operation result
    */
    bool remove( const IdType& id );

    /*!
    * \brief Remove set of metadata objects
    * \param set [in] set of metadata objects
    */
    void remove( const MetadataSet& set );

    /*!
    * \brief Remove schema and all objects in it.
    */
    void remove( const std::shared_ptr< MetadataSchema >& schema );

    /*!
    * \brief Remove all metadata.
    */
    void remove();

    /*!
    * \brief Add new schema
    * \throw NullPointerException if schema pointer is null
    * \throw IncorrectParamException if schema name is empty or
    * schema already exists
    */
    void addSchema( const std::shared_ptr< MetadataSchema >& spSchema );

    /*!
    * \brief Get metadata schema by its name
    * \param sSchemaName [in] schema name
    * \return pointer to schema o9bject or null if schema not found
    */
    const std::shared_ptr< MetadataSchema > getSchema( const std::string& sSchemaName ) const;

    /*!
    * \brief Get the names of all schemas available in the stream
    * \return vector of names of all schemas
    */
    std::vector< std::string > getAllSchemaNames() const;

    /*!
    * \brief Import a set of metadata from a source stream.
    * \param srcStream [in] The source stream that contains the source metadata
    * \param srcSet [in] The source metadata set to be imported
    * \param nTarFrameIndex [in] The frame index in the new video referenced by nSrcFrameIndex
    * \param nSrcFrameIndex [in] The frame index of the first video frame to be imported from the source video.
    * \param nNumOfFrames [in] Number of video frames that has been imported. If this is FRAME_COUNT_ALL, metadata associated to all video 
    * frames since nSrcFrameIndex will be imported
    * \param pSetFailure [out] Set to receive metadata that failed to import, due to losing of associated frame. If this is null, no
    * information about failed metadata will be returned.
    * \details This routine should be called after importing a video sequence from a source video. The nTarFrameIndex, nSrcFrameIndex 
    * and nNumOfFrames parameters should reflect the way how the video was imported.
    */
    bool import( MetadataStream& srcStream, MetadataSet& srcSet, long long nTarFrameIndex, long long nSrcFrameIndex, long long nNumOfFrames = FRAME_COUNT_ALL, MetadataSet* pSetFailure = NULL );

    /*!
    * \brief Get all metadata
    * \return set of metadata objects
    */
    MetadataSet getAll() const;

    /*!
     * \brief Unload all metadata from the stream
     */
    void clear();

    // Assume C++ 11, IQuery interface, seal the interface at this level
    MetadataSet query( std::function< bool( const std::shared_ptr<Metadata>& spMetadata )> filter ) const;
    MetadataSet queryByReference( std::function< bool( const std::shared_ptr<Metadata>& spMetadata, const std::shared_ptr<Metadata>& spReference )> filter ) const;

    MetadataSet queryByFrameIndex( size_t index ) const;
    MetadataSet queryByTime( long long startTime, long long endTime ) const;
    MetadataSet queryBySchema( const std::string& sSchemaName ) const;
    MetadataSet queryByName( const std::string& sName ) const;

    MetadataSet queryByNameAndValue( const std::string& sMetadataName, const vmf::FieldValue& value ) const;
    MetadataSet queryByNameAndFields( const std::string& sMetadataName, const std::vector< vmf::FieldValue>& vFields ) const;

    MetadataSet queryByReference( const std::string& sReferenceName ) const;
    MetadataSet queryByReference( const std::string& sReferenceName, const vmf::FieldValue& value ) const;
    MetadataSet queryByReference( const std::string& sReferenceName, const std::vector< vmf::FieldValue>& vFields ) const;

    void sortById()
    {
        std::sort(m_oMetadataSet.begin(), m_oMetadataSet.end(),
            [](const std::shared_ptr<Metadata> &a, const std::shared_ptr<Metadata>& b){ return a->getId() < b->getId(); });
    }


    /*
    * \brief serialized stream in std::string in selected format
    */
    std::string serialize(Format& format);

    /*
    * \brief deserialized stream from std::string in selected format
    */
    void deserialize(const std::string& text, Format& format);

    /*!
    * \brief Compute MD5 digest of media part of the opened file
    * \return MD5 checksum in std::string format
    */
    std::string computeChecksum();

    /*!
    * \brief Compute MD5 digest of media part of a file
    * This functuion was created for developers purpose and it's a subject to remove,
    * so the function is not recommended for using.
    * \return MD5 checksum in std::string format
    */
    std::string computeChecksum(long long& XMPPacketSize, long long& XMPPacketOffset);

    /*!
    * \brief Get MD5 digest of media part of opened file
    * \return MD5 digest of opened file.
    */
    std::string getChecksum() const;

    /*!
    * \brief Set new MD5 digest to MetadataStream field
    * \param checksum [in] MD5 digest in string format
    */
    void setChecksum(const std::string& checksum);

    /*!
    * \brief Add new video segment
    * \throw IncorrectParamException when input segment intersected with anyone of already created segments.
    */
    void addVideoSegment(const std::shared_ptr<VideoSegment>& newSegment);

    /*!
    * \brief Get vector of video segments that were set for the video
    */
    std::vector<std::shared_ptr<VideoSegment>>& getAllVideoSegments();

    /*!
    * \brief Convert a pair timestamp - duration to a pair frame index - number of frames
    */
    void convertTimestampToFrameIndex(
        long long timestamp, long long duration,
        long long& frameIndex, long long& numOfFrames );

    /*!
    * \brief Convert a pair frame index - number of frames to a pair timestamp - duration
    */
    void convertFrameIndexToTimestamp(
        long long frameIndex, long long numOfFrames,
        long long& timestamp, long long& duration );

protected:
    void dataSourceCheck();
    std::shared_ptr<Metadata> import( MetadataStream& srcStream, std::shared_ptr< Metadata >& spMetadata, std::map< IdType, IdType >& mapIds, 
        long long nTarFrameIndex, long long nSrcFrameIndex, long long nNumOfFrames = FRAME_COUNT_ALL );
    void internalAdd(const std::shared_ptr< Metadata >& spMetadata);

private:
    OpenMode m_eMode;
    std::string m_sFilePath;
    MetadataSet m_oMetadataSet;

    std::map<IdType, std::vector<std::pair<IdType, std::string>>> m_pendingReferences;
    std::map< std::string, std::shared_ptr< MetadataSchema > > m_mapSchemas;
    std::map< std::string, std::shared_ptr< MetadataSchema > > removedSchemas;
    std::vector<std::shared_ptr<VideoSegment>> videoSegments;
    std::vector<IdType> removedIds;
    std::vector<IdType> addedIds;
    std::shared_ptr<IDataSource> dataSource;
    vmf::IdType nextId;
    std::string m_sChecksumMedia;
};

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif /* __METADATA_STREAM_H__ */
