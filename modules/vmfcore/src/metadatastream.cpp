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
#include "vmf/metadatastream.hpp"
#include "vmf/ireader.hpp"
#include "vmf/iwriter.hpp"
#include "datasource.hpp"
#include "object_factory.hpp"
#include <algorithm>
#include <stdexcept>

#include <iostream>

namespace vmf
{
MetadataStream::MetadataStream(void)
    : m_eMode( InMemory ), dataSource(nullptr), nextId(0), m_sChecksumMedia("")
{
}

MetadataStream::~MetadataStream(void)
{
    close();
    clear();
}

bool MetadataStream::open( const std::string& sFilePath, MetadataStream::OpenMode eMode )
{
    try
    {
        if (m_eMode != InMemory)
            return false;
        dataSource = ObjectFactory::getInstance()->getDataSource();
        if (!dataSource)
        {
            VMF_EXCEPTION(InternalErrorException, "Failed to get datasource instance. Possible, call of vmf::initialize is missed");
        }
        clear();
        m_sFilePath = sFilePath;
        dataSource->openFile(m_sFilePath, eMode);
        dataSource->loadVideoSegments(videoSegments);
        dataSource->load(m_mapSchemas);
        m_eMode = eMode;
        m_sFilePath = sFilePath;
        nextId = dataSource->loadId();
        m_sChecksumMedia = dataSource->loadChecksum();

        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool MetadataStream::load( const std::string& sSchemaName )
{
    dataSourceCheck();
    try
    {
        if (sSchemaName.empty())
        {
            for (auto it = m_mapSchemas.begin(); it != m_mapSchemas.end(); ++it)
            {
                dataSource->loadSchema(it->first, *this);
            }
        }
        else
        {
            dataSource->loadSchema(sSchemaName, *this);
        }
        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool MetadataStream::load(const std::string& sSchemaName, const std::string& sMetadataName)
{
    dataSourceCheck();
    try
    {
        dataSource->loadProperty(sSchemaName, sMetadataName, *this);
        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool MetadataStream::save()
{
    dataSourceCheck();
    try
    {
        if( m_eMode == ReadWrite && !m_sFilePath.empty() )
        {
            dataSource->remove(removedIds);
            removedIds.clear();

            for(auto p = m_mapSchemas.begin(); p != m_mapSchemas.end(); ++p)
            {
                dataSource->saveSchema(p->first, *this);
                dataSource->save(p->second);
            }

	        dataSource->saveVideoSegments(videoSegments);

            dataSource->save(nextId);

            if(!m_sChecksumMedia.empty())
                dataSource->saveChecksum(m_sChecksumMedia);

            addedIds.clear();

            for(auto schemaPtr = removedSchemas.begin(); schemaPtr != removedSchemas.end(); schemaPtr++)
            {
                if(schemaPtr->first == "")
                {
                    dataSource->removeSchema("");
                    break;
                }
                dataSource->removeSchema(schemaPtr->first);
            }

            return true;
        }
        else
        {
            return false;
        }
    }
    catch (...)
    {
        return false;
    }
}

bool MetadataStream::reopen( OpenMode eMode )
{
    dataSourceCheck();
    if( m_eMode != InMemory )
        VMF_EXCEPTION(vmf::IncorrectParamException, "The previous file has not been closed!");

    if( m_sFilePath.empty())
        VMF_EXCEPTION(vmf::IncorrectParamException, "The file path is emtpy!");
    try
    {
        dataSource->openFile(m_sFilePath, eMode);
        m_eMode = eMode;
        return true;
    }
    catch(...)
    {
        // do nothing
    }
    return false;
}

bool MetadataStream::saveTo( const std::string& sFilePath )
{
    if( m_eMode != InMemory )
        throw std::runtime_error("The previous file has not been closed!");
    try
    {
        std::shared_ptr<IDataSource> oldDataSource = dataSource;
        dataSource = ObjectFactory::getInstance()->getDataSource();
        if (!dataSource)
        {
            dataSource = oldDataSource;
            return false;
        }

        // Change file path to make reopen() happy
        m_sFilePath = sFilePath;

        bool bRet = false;

        // Do not load anything from the file by calling reopen()
        if( this->reopen( ReadWrite ) )
        {
            dataSource->clear();
            bRet = this->save();
        }
        dataSource->closeFile();

        dataSource = oldDataSource;
        return bRet;
    }
    catch (const DataStorageException& /* unused */)
    {
        return false;
    }
}

void MetadataStream::close()
{
    try
    {
        m_eMode = InMemory;
        if (dataSource)
            dataSource->closeFile();
    }
    catch(...)
    {
        // do nothing
    }
}

std::shared_ptr< Metadata > MetadataStream::getById( const IdType& id ) const
{
    MetadataSet::const_iterator it = std::find_if( m_oMetadataSet.begin(), m_oMetadataSet.end(), [id]( const std::shared_ptr< Metadata >& spItem )->bool 
    {
        return spItem->getId() == id; 
    });

    if( it != m_oMetadataSet.end() )
        return *it;

    return nullptr;
}

IdType MetadataStream::add( std::shared_ptr< Metadata >& spMetadata )
{
    if( !this->getSchema(spMetadata->getDesc()->getSchemaName()) )
        VMF_EXCEPTION(vmf::NotFoundException, "Metadata schema is not in the stream");

    IdType id = nextId++;
    spMetadata->setId(id);
    internalAdd(spMetadata);
    addedIds.push_back(id);
    return id;
}

IdType MetadataStream::add( std::shared_ptr< MetadataInternal >& spMetadataInternal)
{
    if( !this->getSchema(spMetadataInternal->getDesc()->getSchemaName()) )
	VMF_EXCEPTION(vmf::NotFoundException, "Metadata schema is not in the stream");

    IdType id = spMetadataInternal->getId();
    if(id != INVALID_ID)
    {
        if(this->getById(id) == nullptr)
        {
            if(nextId < id)
                nextId = id + 1;
        }
        else
            VMF_EXCEPTION(IncorrectParamException, "Metadata with such id is already in the stream");
    }
    else
    {
        id = nextId++;
        spMetadataInternal->setId(id);
    }
    internalAdd(spMetadataInternal);
    addedIds.push_back(id);

    if(!spMetadataInternal->vRefs.empty())
    {
        auto spItem = getById(id);
        for(auto ref = spMetadataInternal->vRefs.begin(); ref != spMetadataInternal->vRefs.end(); ref++)
        {
            auto referencedItem = getById(ref->first);
            if(referencedItem != nullptr)
                spItem->addReference(referencedItem, ref->second);
            else
                m_pendingReferences[ref->first].push_back(std::make_pair(id, ref->second));
        }
    }
    auto pendingReferences = m_pendingReferences[id];
    if(!pendingReferences.empty())
    {
        for(auto pendingId = pendingReferences.begin(); pendingId != pendingReferences.end(); pendingId++)
            getById(pendingId->first)->addReference(spMetadataInternal, pendingId->second);
        m_pendingReferences[id].clear();
        m_pendingReferences.erase(id);
    }

    return id;
}

void MetadataStream::internalAdd(const std::shared_ptr<Metadata>& spMetadata)
{
    spMetadata->validate();
    spMetadata->setStreamRef(this);

    // Make sure all referenced metadata are from the same stream
    auto vRefSet = spMetadata->getAllReferences();
    std::for_each( vRefSet.begin(), vRefSet.end(), [&]( Reference& spRef )
    {
        if(spRef.getReferenceMetadata().lock()->m_pStream != this)
        {
            VMF_EXCEPTION(IncorrectParamException, "Referenced metadata is from different metadata stream.");
        }
    });
    m_oMetadataSet.push_back(spMetadata);

    // update statistics
    handleStatistics( IStatisticsOperation::Add, spMetadata );
}

bool MetadataStream::remove( const IdType& id )
{
    bool bRet = false;

    // Locate the item in the vector
    MetadataSet::iterator it = std::find_if( m_oMetadataSet.begin(), m_oMetadataSet.end(), [id]( const std::shared_ptr< Metadata >& spItem )->bool 
    { 
        return spItem->getId() == id; 
    });

    // Found it, let's remove it
    if( it != m_oMetadataSet.end() )
    {
        std::shared_ptr< Metadata > spMetadata = *it;

        // update statistics
        handleStatistics( IStatisticsOperation::Remove, spMetadata );

        spMetadata->setStreamRef(nullptr);
        m_oMetadataSet.erase( it );

        // Also remove any reference to it. There might be other shared pointers pointing to this object, so that
        // we cannot rely on weak_ptr being nullptr.
        std::for_each( m_oMetadataSet.begin(), m_oMetadataSet.end(), [id]( std::shared_ptr< Metadata >& spItem )
        {
            spItem->removeReference( id );
        });

        auto addedItr = std::find(addedIds.begin(), addedIds.end(), id);

        if (addedItr == addedIds.end())
        {
            removedIds.push_back(id);
        }
        else
        {
            addedIds.erase(addedItr);
        }

        bRet = true;
    }

    return bRet;
}

void MetadataStream::remove( const MetadataSet& set )
{
    std::for_each( set.begin(), set.end(), [&]( const std::shared_ptr<Metadata>& spMetadata )
    {
        this->remove( spMetadata->getId() );
    });
}

void MetadataStream::remove(const std::shared_ptr< MetadataSchema >& spSchema)
{
    if( spSchema == nullptr )
    {
        VMF_EXCEPTION(NullPointerException, "Metadata Schema is null." );
    }

    std::string sSchemaName = spSchema->getName();
    if( sSchemaName.empty() )
    {
        VMF_EXCEPTION(IncorrectParamException, "Metadata Schema name cannot be empty!" );
    }

    auto it = m_mapSchemas.find( sSchemaName );
    if( it == m_mapSchemas.end())
    {
        VMF_EXCEPTION(IncorrectParamException, "Metadata Schema doesn't exist!" );
    }

    auto items = this->queryBySchema(sSchemaName);
    remove(items);

    auto removedItr = m_mapSchemas.find(sSchemaName);

    if (removedItr != m_mapSchemas.end())
        m_mapSchemas.erase(removedItr);

    removedSchemas[sSchemaName] = spSchema;
}

void MetadataStream::remove()
{
    std::shared_ptr<vmf::MetadataSchema> emptySchema;
    removedSchemas[""] = emptySchema;
    this->remove(this->getAll());
    m_mapSchemas.clear();
}

void MetadataStream::addSchema( std::shared_ptr< MetadataSchema >& spSchema )
{
    if( spSchema == nullptr )
    {
        VMF_EXCEPTION(NullPointerException, "Metadata Schema is null." );
    }

    std::string sSchemaName = spSchema->getName();
    if( sSchemaName.empty() )
    {
        VMF_EXCEPTION(IncorrectParamException, "Metadata Schema name cannot be empty!" );
    }

    auto it = m_mapSchemas.find( sSchemaName );
    if( it != m_mapSchemas.end())
    {
        VMF_EXCEPTION(IncorrectParamException, "Metadata Schema already exists!" );
    }

    m_mapSchemas[ sSchemaName ] = spSchema;
}

const std::shared_ptr< MetadataSchema > MetadataStream::getSchema( const std::string& sSchemaName ) const
{
    auto it = m_mapSchemas.find( sSchemaName );
    if( it != m_mapSchemas.end())
        return it->second;

    return nullptr;
}

std::vector< std::string > MetadataStream::getAllSchemaNames() const
{
    std::vector< std::string > vAllSchemaNames;

    std::for_each( m_mapSchemas.begin(), m_mapSchemas.end(), [&]( const std::pair< std::string, std::shared_ptr< MetadataSchema >>& pair )
    {
        vAllSchemaNames.push_back( pair.first );
    });

    return vAllSchemaNames;
}

std::shared_ptr<Metadata> MetadataStream::import( MetadataStream& srcStream, std::shared_ptr< Metadata >& spMetadata, std::map< IdType, IdType >& mapIds, long long nTarFrameIndex, long long nSrcFrameIndex, long long nNumOfFrames )
{
    auto nSrcMetadataId = spMetadata->getId();

    // Check to make sure the metadata belongs to the source stream
    if( nSrcMetadataId < 0 || srcStream.getById( nSrcMetadataId ) != spMetadata )
    {
        VMF_EXCEPTION(IncorrectParamException, "The input metadata does not belong to the source stream!" );
    }

    // Skip if it has already been imported
    if( mapIds.find( nSrcMetadataId ) != mapIds.end() )
    {
        auto spNewMetadata = this->getById( mapIds[ nSrcMetadataId ] );
        if( spNewMetadata == nullptr )
        {
            VMF_EXCEPTION(InternalErrorException, "Unexpected exception!" );
        }

        return spNewMetadata;
    }

    // Make a deep copy, add to the new stream, and add to the map
    std::shared_ptr< Metadata > spNewMetadata( new Metadata( *spMetadata ));
    if( !spNewMetadata->shiftFrameIndex( nTarFrameIndex, nSrcFrameIndex, nNumOfFrames ))
    {
        return nullptr;
    }

    auto nNewMetadataId = this->add( spNewMetadata );
    mapIds[ nSrcMetadataId ] = nNewMetadataId;

    // Wire to the correct description
    auto spNewSchema = this->getSchema( spMetadata->getSchemaName() );
    auto spNewDescriptor = spNewSchema == nullptr ? nullptr : spNewSchema->findMetadataDesc( spMetadata->getName() );
    if( spNewDescriptor == nullptr )
    {
        VMF_EXCEPTION(InternalErrorException, "Metadata schema or description was not found!" );
    }
    spNewMetadata->setDescriptor( spNewDescriptor );

    // Import all references recursively
    spNewMetadata->removeAllReferences();
    auto vReferences = spMetadata->getAllReferences();
    std::for_each( vReferences.begin(), vReferences.end(), [&]( Reference& reference )
    {
        // Import the reference to the new stream
        std::shared_ptr<Metadata> md = reference.getReferenceMetadata().lock();
        auto spNewReference = import( srcStream, md, mapIds, nTarFrameIndex, nSrcFrameIndex, nNumOfFrames );

        // Add as reference
        if( spNewReference != nullptr )
            spNewMetadata->addReference( spNewReference );
    });

    return spNewMetadata;
}

bool MetadataStream::import( MetadataStream& srcStream, MetadataSet& srcSet, long long nTarFrameIndex, long long nSrcFrameIndex, long long nNumOfFrames, MetadataSet* pSetFailure )
{
    // Find all schemes used by the source metadata set
    std::vector< std::string > vSchemaNames;
    std::for_each( srcSet.begin(), srcSet.end(), [&vSchemaNames]( std::shared_ptr<Metadata>& spMetadata )
    {
        vSchemaNames.push_back( spMetadata->getSchemaName() );
    });

    // Remove redundant schema names
    std::sort( vSchemaNames.begin(), vSchemaNames.end() );
    vSchemaNames.erase( std::unique( vSchemaNames.begin(), vSchemaNames.end() ), vSchemaNames.end());

    // Import all schemes used
    std::for_each( vSchemaNames.begin(), vSchemaNames.end(), [&]( std::string& sAppName )
    {
        if( this->getSchema( sAppName ) == nullptr )
        {
            auto it = srcStream.m_mapSchemas.find( sAppName );
            if( it != srcStream.m_mapSchemas.end())
                this->addSchema( it->second );
            else
            {
                VMF_EXCEPTION(InternalErrorException, "Metadata schema missing in the source stream!" );
            }
        }
    });

    // This map stores Id pairs<old, new> from the srcSet and this set.
    std::map< IdType, IdType > mapIds;

    // Import metadata one by one. 
    std::for_each( srcSet.begin(), srcSet.end(), [&]( std::shared_ptr<Metadata>& spMetadata )
    {
        if( import( srcStream, spMetadata, mapIds, nTarFrameIndex, nSrcFrameIndex, nNumOfFrames ) == nullptr &&
            pSetFailure != NULL )
        {
            pSetFailure->push_back( spMetadata );
        }
    });

    return mapIds.size() > 0;
}

void MetadataStream::clear()
{
    m_eMode = InMemory;
    m_sFilePath = "";
    m_oMetadataSet.clear();
    m_mapSchemas.clear();
    removedIds.clear();
    addedIds.clear();
    videoSegments.clear();
}

void MetadataStream::dataSourceCheck()
{
    if (!dataSource)
    {
        VMF_EXCEPTION(InternalErrorException, "No files has been assosiated with this stream");
    }
}

MetadataSet MetadataStream::getAll() const
{
    return m_oMetadataSet;
}

MetadataSet MetadataStream::query( std::function< bool( const std::shared_ptr<Metadata>& spMetadata )> filter ) const
{
    return m_oMetadataSet.query( filter );
}

MetadataSet MetadataStream::queryByReference( std::function< bool( const std::shared_ptr<Metadata>& spMetadata, const std::shared_ptr<Metadata>& spReference )> filter ) const
{
    return m_oMetadataSet.queryByReference( filter );
}
MetadataSet MetadataStream::queryByName( const std::string& sName ) const
{
    return m_oMetadataSet.queryByName( sName );
}

MetadataSet MetadataStream::queryBySchema( const std::string& sSchemaName ) const
{
    return m_oMetadataSet.queryBySchema( sSchemaName );
}

MetadataSet MetadataStream::queryByFrameIndex( size_t index ) const
{
    return m_oMetadataSet.queryByFrameIndex( index );
}

MetadataSet MetadataStream::queryByTime( long long startTime, long long endTime ) const
{
    return m_oMetadataSet.queryByTime(startTime, endTime);
}

MetadataSet MetadataStream::queryByNameAndValue( const std::string& sMetadataName, const vmf::FieldValue& value ) const
{
    return m_oMetadataSet.queryByNameAndValue( sMetadataName, value );
}

MetadataSet MetadataStream::queryByNameAndFields( const std::string& sMetadataName, const std::vector< vmf::FieldValue>& vFields ) const
{
    return m_oMetadataSet.queryByNameAndFields( sMetadataName, vFields );
}

MetadataSet MetadataStream::queryByReference( const std::string& sReferenceName ) const
{
    return m_oMetadataSet.queryByReference( sReferenceName );
}

MetadataSet MetadataStream::queryByReference( const std::string& sReferenceName, const vmf::FieldValue& value ) const
{
    return m_oMetadataSet.queryByReference( sReferenceName, value );
}

MetadataSet MetadataStream::queryByReference( const std::string& sReferenceName, const std::vector< vmf::FieldValue>& vFields ) const
{
    return m_oMetadataSet.queryByReference( sReferenceName, vFields );
}

std::string MetadataStream::serialize(IWriter& writer)
{
    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    for(auto spMetadataIter = m_mapSchemas.begin(); spMetadataIter != m_mapSchemas.end(); spMetadataIter++)
        schemas.push_back(spMetadataIter->second);
    return writer.store(nextId, m_sFilePath, m_sChecksumMedia, videoSegments, schemas, m_oMetadataSet);
}

void MetadataStream::deserialize(const std::string& text, IReader& reader)
{
    std::vector<std::shared_ptr<VideoSegment>> segments;
    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    std::vector<std::shared_ptr<MetadataInternal>> metadata;
    std::string filePath;
    reader.parseAll(text, nextId, filePath, m_sChecksumMedia, segments, schemas, metadata);
    if(m_sFilePath.empty())
        m_sFilePath = filePath;
    std::for_each( segments.begin(), segments.end(), [&]( std::shared_ptr< VideoSegment >& spSegment )
    {
	addVideoSegment(spSegment);
    });
    std::for_each( schemas.begin(), schemas.end(), [&]( std::shared_ptr< MetadataSchema >& spSchema )
    {
        addSchema(spSchema);
    });
    std::for_each( metadata.begin(), metadata.end(), [&]( std::shared_ptr< MetadataInternal >& spMetadata )
    {
        add(spMetadata);
    });
}

std::string MetadataStream::computeChecksum()
{
    long long size, offset;
    return this->computeChecksum(size, offset);
}

std::string MetadataStream::computeChecksum(long long& XMPPacketSize, long long& XMPPacketOffset)
{
    dataSourceCheck();
    return dataSource->computeChecksum(XMPPacketSize, XMPPacketOffset);
}

std::string MetadataStream::getChecksum() const
{
    return m_sChecksumMedia;
}

void MetadataStream::setChecksum(const std::string &digestStr)
{
    m_sChecksumMedia = digestStr;
}

void MetadataStream::addVideoSegment(const std::shared_ptr<VideoSegment>& newSegment)
{
    if (!newSegment)
        VMF_EXCEPTION(NullPointerException, "Pointer to new segment is NULL");

    if (newSegment->getTitle().empty())
        VMF_EXCEPTION(IncorrectParamException, "Segment contains empty 'title' value");

    if (newSegment->getDuration() < 0)
        VMF_EXCEPTION(IncorrectParamException, "Segment contains invalid 'duration' value");

    if (newSegment->getTime() < 0)
        VMF_EXCEPTION(IncorrectParamException, "Segment contains invalid 'timeStart' value");

    if (newSegment->getFPS() <= 0)
        VMF_EXCEPTION(IncorrectParamException, "Segment contains invalid 'FPS' value");

    long height = 0, width = 0;
    newSegment->getResolution(height, width);

    if ((height < 0) || (width < 0))
        VMF_EXCEPTION(IncorrectParamException, "Segment contains invalid resolution");

    std::for_each(videoSegments.begin(), videoSegments.end(), [&](const std::shared_ptr<VideoSegment>& segment)
    {   
	    long long newTime = newSegment->getTime(), segmentTime = segment->getTime();
	    
        if (((segmentTime <= newTime) && ((segmentTime + segment->getDuration() - 1) >= newTime)) ||
		    ((newTime <= segmentTime) && ((newTime + newSegment->getDuration() - 1) >= segmentTime)))
		    VMF_EXCEPTION(IncorrectParamException, "Input segment intersects a one of the already created segments");
    });

    videoSegments.push_back(newSegment);
}

std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& MetadataStream::getAllVideoSegments()
{
    return videoSegments;
}

MetadataStream::VideoSegment::VideoSegment()
    : title(""), fps(0.), timeStart(-1), duration(0), width(0), height(0) {}

MetadataStream::VideoSegment::VideoSegment(
            const std::string& _title, double _fps, long long _timeStart,
            long long _duration, long _width, long _height )
    :
    title(_title), fps(_fps), timeStart(_timeStart), duration(_duration), width(_width), height(_height)
{
    if (_title.empty())
        VMF_EXCEPTION(IncorrectParamException, "Segment title can't be empty");
    if (_fps <= 0)
        VMF_EXCEPTION(IncorrectParamException, "Segment fps must be positive");
    if (_timeStart < 0)
        VMF_EXCEPTION(IncorrectParamException, "Segment start time must be positive");
    if (_duration < 0)
        VMF_EXCEPTION(IncorrectParamException, "Segment duration must be positive");
    if (_width < 0 || _height < 0)
        VMF_EXCEPTION(IncorrectParamException, "Segment resoulution width and height must be positive");
}

MetadataStream::VideoSegment::~VideoSegment() {}

std::string MetadataStream::VideoSegment::getTitle() const { return title; }

void MetadataStream::VideoSegment::setTitle(const std::string& _title)
{
    if(_title.empty())
        VMF_EXCEPTION(IncorrectParamException, "Segment title can't be empty");
    title = _title;
}

double MetadataStream::VideoSegment::getFPS() const { return fps; }

void MetadataStream::VideoSegment::setFPS(double _fps)
{
    if(_fps <= 0)
        VMF_EXCEPTION(IncorrectParamException, "Segment fps must be positive");
    fps = _fps;
}

long long MetadataStream::VideoSegment::getDuration() const { return duration; }

void MetadataStream::VideoSegment::setDuration(long long _duration)
{
    if(_duration <= 0)
        VMF_EXCEPTION(IncorrectParamException, "Segment duration must positive");
    duration = _duration;
}

long long MetadataStream::VideoSegment::getTime() const { return timeStart; }

void MetadataStream::VideoSegment::setTime(long long _timeStart)
{
    if(_timeStart < 0)
        VMF_EXCEPTION(IncorrectParamException, "Segment start time must be positive");
    timeStart = _timeStart;
}

void MetadataStream::VideoSegment::getResolution(long& _width, long& _height) const { _width = width; _height = height; }

void MetadataStream::VideoSegment::setResolution(long _width, long _height)
{
    if(_width <= 0 || _height <= 0)
        VMF_EXCEPTION(IncorrectParamException, "Segment resoulution width and height must be positive");
    width = _width;
    height = _height;
}

void MetadataStream::convertTimestampToFrameIndex(
    long long timestamp, long long duration,
    long long& frameIndex, long long& numOfFrames )
{
    frameIndex = 0, numOfFrames = 0;
    unsigned int i = 0;
    for(; i < videoSegments.size(); i++)
    {
        auto segment = videoSegments[i];
        if (segment->getTime() <= timestamp && segment->getTime() + segment->getDuration() >= timestamp)
        {
            frameIndex += (long long)((timestamp - segment->getTime()) * segment->getFPS() / 1000);
            if (segment->getTime() + segment->getDuration() >= timestamp + duration)
                numOfFrames = (long long)(duration * segment->getFPS() / 1000);
            else
                numOfFrames = (long long)((segment->getTime() + segment->getDuration() - timestamp) * segment->getFPS() / 1000);
            break;
        }
        frameIndex += (long long)(segment->getDuration() * segment->getFPS() / 1000);
    }
    if (i == videoSegments.size())
        frameIndex = Metadata::UNDEFINED_FRAME_INDEX, numOfFrames = Metadata::UNDEFINED_FRAMES_NUMBER;
}

void MetadataStream::convertFrameIndexToTimestamp(
    long long frameIndex, long long numOfFrames,
    long long& timestamp, long long& duration )
{
    long long localFrameIndex = 0;
    timestamp = 0, duration = 0;
    unsigned int i = 0;
    for (; i < videoSegments.size(); i++)
    {
        auto segment = videoSegments[i];
        if (localFrameIndex + (long long)(segment->getDuration() * segment->getFPS() / 1000) > frameIndex)
        {
            timestamp = (long long)(segment->getTime() + (frameIndex - localFrameIndex) / segment->getFPS() * 1000);
            if (localFrameIndex + segment->getDuration() * segment->getFPS() / 1000 > frameIndex + numOfFrames)
                duration = (long long)(numOfFrames / segment->getFPS() * 1000);
            else
                duration = (long long)(segment->getTime() + segment->getDuration() - timestamp);
            break;
        }
        localFrameIndex += (long long)(segment->getDuration() * segment->getFPS() / 1000);
    }
    if (i == videoSegments.size())
        timestamp = Metadata::UNDEFINED_TIMESTAMP, duration = Metadata::UNDEFINED_DURATION;
}

void MetadataStream::handleStatistics(
    IStatisticsOperation::Mode mode,
    const IdType& id,
    const std::string& fieldName ) const
{
    const std::shared_ptr< Metadata > spMetadata = getById( id );
    if( !spMetadata ) return;

    handleStatistics( mode, spMetadata, fieldName );
}

void MetadataStream::handleStatistics(
    IStatisticsOperation::Mode mode,
    const std::shared_ptr< Metadata > spMetadata,
    const std::string& fieldName ) const
{
    if( !spMetadata ) return;

    const std::shared_ptr< MetadataDesc > spDesc = spMetadata->getDesc();
    if( !spDesc ) return;

    const std::shared_ptr< MetadataSchema > spSchema = getSchema( spDesc->getSchemaName() );
    if( !spSchema ) return;

    std::shared_ptr< Statistics > statistics = spSchema->getStatistics();
    switch( mode )
    {
    case IStatisticsOperation::Add:
    case IStatisticsOperation::Remove:
        if( statistics )
        {
            statistics->handleMetadata( mode, spMetadata );
        }
        return;
    case IStatisticsOperation::Change:
        if( statistics )
        {
            statistics->handleMetadataField( mode, spMetadata, fieldName );
        }
        return;
    default:
        VMF_EXCEPTION(InternalErrorException, "Unknown statistics operation type: only Add/Remove/Change are accepted");
    }
}

void MetadataStream::setStatistics( std::shared_ptr< Statistics > statistics, const std::string& schemaName )
{
    // TODO: on schema/stream add/remove/load events, set also these schema statistics params accordingly

    if( statistics == nullptr )
    {
        VMF_EXCEPTION(NullPointerException, "Statistics object pointer is empty!" );
    }

    const std::shared_ptr< MetadataSchema > spSchema = getSchema( schemaName );
    if( spSchema == nullptr )
    {
        VMF_EXCEPTION(NotFoundException, "Metadata schema is not in the stream");
    }

    statistics->setMetadataStream( this );
    spSchema->setStatistics( statistics );
}

unsigned MetadataStream::registerUserOperation( std::shared_ptr< IStatisticsOperation > operation )
{
    const unsigned userBegin = unsigned(IStatisticsOperation::_UserBegin);

    if( operation == nullptr )
    {
        VMF_EXCEPTION(NullPointerException, "Operation pointer is empty!" );
    }

    auto it = std::find_if( m_userOperations.begin(), m_userOperations.end(),
                            [&]( const std::shared_ptr< IStatisticsOperation >& spOperation )->bool
    {
        return spOperation == operation;
    });

    if( it != m_userOperations.end() )
    {
        return userBegin + std::distance( m_userOperations.begin(), it );
    }
    else
    {
        unsigned type = userBegin + m_userOperations.size();
        m_userOperations.push_back( operation );
        return type;
    }
}

std::shared_ptr< IStatisticsOperation > MetadataStream::findUserOperation( unsigned operationType /*, unsigned dataType*/ )
{
    const unsigned userBegin = unsigned(IStatisticsOperation::_UserBegin);

    unsigned index = 0;
    if( (operationType < userBegin) || ((index = operationType-userBegin) >= m_userOperations.size()) )
    {
        VMF_EXCEPTION(OutOfRangeException, "Invalid user operation type!" );
    }

    return m_userOperations[index];
}

std::vector< std::shared_ptr< IStatisticsOperation >> MetadataStream::m_userOperations;

}//namespace vmf

