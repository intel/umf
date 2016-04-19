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
#include "vmf/format.hpp"
#include "datasource.hpp"
#include "object_factory.hpp"
#include <algorithm>
#include <stdexcept>
#include <set>

#include <iostream>

namespace vmf
{
MetadataStream::MetadataStream(void)
    : m_eMode( InMemory ), dataSource(nullptr), nextId(0), m_sChecksumMedia(""),
      m_useEncryption(false), m_encryptor(nullptr), m_hintEncryption("")
{
}

MetadataStream::~MetadataStream(void)
{
    close();
    clear();
}

bool MetadataStream::open(const std::string& sFilePath, MetadataStream::OpenMode eMode)
{
    try
    {
        if((m_eMode & ReadOnly) || (m_eMode & Update))
            return false;
        dataSource = ObjectFactory::getInstance()->getDataSource();
        if (!dataSource)
        {
            VMF_EXCEPTION(InternalErrorException, "Failed to get datasource instance. Possible, call of vmf::initialize is missed");
        }
        //don't call clear(), reset exactly the things needed to be reset
        m_oMetadataSet.clear();
        m_mapSchemas.clear();
        removedIds.clear();
        addedIds.clear();
        videoSegments.clear();
        for (auto& stat : m_stats) stat->clear();

        m_sFilePath = sFilePath;
        //encryption of all scopes except whole stream should be performed by MetadataStream
        //dataSource should know nothing about that
        dataSource->setEncryptor(m_encryptor);

        dataSource->openFile(m_sFilePath, eMode);
        dataSource->loadVideoSegments(videoSegments);
        dataSource->load(m_mapSchemas);
        dataSource->loadStats(m_stats);
        m_eMode = eMode;
        m_sFilePath = sFilePath;
        nextId = dataSource->loadId();
        m_hintEncryption = dataSource->loadHintEncryption();
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

        //need to decrypt everything, not just loaded schema
        //because the referenced metadata records are also loaded
        decrypt();

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

        //need to decrypt everything, not just loaded schema/description
        //because the referenced metadata records are also loaded
        decrypt();

        return true;
    }
    catch(...)
    {
        return false;
    }
}


bool MetadataStream::save(const vmf_string &compressorId)
{
    dataSourceCheck();
    try
    {
        if( (m_eMode & Update) && !m_sFilePath.empty() )
        {
            //don't copy everything, just things we need for an encryption
            MetadataStream encryptedStream;
            encryptedStream.m_encryptor = m_encryptor;
            encryptedStream.m_oMetadataSet = MetadataSet(m_oMetadataSet);
            encryptedStream.m_mapSchemas = m_mapSchemas;
            encryptedStream.encrypt();

            dataSource->setCompressor(compressorId);
            //encryption of all scopes except whole stream should be performed by MetadataStream
            //dataSource should know nothing about that
            if(m_useEncryption && !m_encryptor)
            {
                VMF_EXCEPTION(vmf::IncorrectParamException, "No encryptor provided while encryption is needed");
            }
            dataSource->setEncryptor(m_useEncryption ? m_encryptor : nullptr);

            dataSource->remove(removedIds);
            removedIds.clear();

            for(auto& schemaPtr : removedSchemas)
            {
                dataSource->removeSchema(schemaPtr.first);
                // Empty schema name is used to delete all schemas in the file
                // That's why there's no need to continue the loop
                if(schemaPtr.first == "")
                {
                    break;
                }
            }
            removedSchemas.clear();

            for(auto& p : m_mapSchemas)
            {
                dataSource->saveSchema(p.second, encryptedStream.getAll());
                dataSource->save(p.second);
            }

            dataSource->saveStats(m_stats);

            dataSource->saveVideoSegments(videoSegments);

            dataSource->save(nextId);

            if(!m_hintEncryption.empty())
                dataSource->saveHintEncryption(m_hintEncryption);

            if(!m_sChecksumMedia.empty())
                dataSource->saveChecksum(m_sChecksumMedia);

            addedIds.clear();

            dataSource->pushChanges();

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
    if((m_eMode & ReadOnly) || (m_eMode & Update))
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


bool MetadataStream::saveTo(const std::string& sFilePath, const vmf_string& compressorId)
{
    if((m_eMode & ReadOnly) || (m_eMode & Update))
        VMF_EXCEPTION(vmf::IncorrectParamException, "The previous file has not been closed!");

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
        if( this->reopen( Update ) )
        {
            dataSource->clear();
            bRet = this->save(compressorId);
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
        m_eMode = (m_eMode & ~Update) & ~ReadOnly;
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

IdType MetadataStream::add( std::shared_ptr< Metadata > spMetadata )
{
    if( !this->getSchema(spMetadata->getDesc()->getSchemaName()) )
        VMF_EXCEPTION(vmf::NotFoundException, "Metadata schema is not in the stream");

    IdType id = nextId++;
    spMetadata->setId(id);
    internalAdd(spMetadata);
    addedIds.push_back(id);
    return id;
}

IdType MetadataStream::add(MetadataInternal& mdi)
{
    auto schema = getSchema(mdi.schemaName);
    if (!schema) VMF_EXCEPTION(vmf::NotFoundException, "Unknown Metadata Schema: " + mdi.schemaName);

    auto desc = schema->findMetadataDesc(mdi.descName);
    if (!desc) VMF_EXCEPTION(vmf::NotFoundException, "Unknown Metadata Description: " + mdi.descName);

    if (mdi.id != INVALID_ID)
        if (!getById(mdi.id)) nextId = std::max(nextId, mdi.id + 1);
        else VMF_EXCEPTION(IncorrectParamException, "Duplicated Metadata ID: " + to_string(mdi.id));
    else
        mdi.id = nextId++;

    auto spMd = std::make_shared<Metadata>(desc);
    spMd->setId(mdi.id);
    FieldDesc fd;
    Variant val;
    for (const auto& f : mdi.fields)
    {
        if (desc->getFieldDesc(fd, f.first))
        {
            val.fromString(fd.type, f.second.value);
            spMd->setFieldValue(f.first, val);
            auto fieldIt = spMd->findField(f.first);
            fieldIt->setUseEncryption(f.second.useEncryption);
            fieldIt->setEncryptedData(f.second.encryptedData);
        }
        else
            VMF_EXCEPTION(IncorrectParamException, "Unknown Metadat field name: " + f.first);
    }
    spMd->setFrameIndex(mdi.frameIndex, mdi.frameNum);
    spMd->setTimestamp(mdi.timestamp, mdi.duration);

    spMd->setUseEncryption(mdi.useEncryption);
    spMd->setEncryptedData(mdi.encryptedData);

    internalAdd(spMd);
    addedIds.push_back(spMd->getId());

    if (!mdi.refs.empty())
    {
        for (const auto& ref : mdi.refs)
        {
            auto referencedItem = getById(ref.first);
            if (referencedItem != nullptr)
                spMd->addReference(referencedItem, ref.second);
            else
                m_pendingReferences[ref.first].push_back(std::make_pair(mdi.id, ref.second));
        }
    }
    for (const auto& pendingId : m_pendingReferences[mdi.id])
        getById(pendingId.first)->addReference(spMd, pendingId.second);

    m_pendingReferences.erase(mdi.id);

    return mdi.id;
}

void MetadataStream::internalAdd(const std::shared_ptr<Metadata>& spMetadata)
{
    spMetadata->validate();
    spMetadata->setStreamRef(this);

    // Make sure all referenced metadata are from the same stream
    auto vRefSet = spMetadata->getAllReferences();
    for (auto& spRef : vRefSet)
    {
        if(spRef.getReferenceMetadata().lock()->m_pStream != this)
            VMF_EXCEPTION(IncorrectParamException, "Referenced metadata is from different metadata stream.");
    }
    m_oMetadataSet.push_back(spMetadata);

    notifyStat(spMetadata);
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

        notifyStat(spMetadata, Stat::Action::Remove);

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

void MetadataStream::remove(std::shared_ptr< MetadataSchema > spSchema)
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

void MetadataStream::addSchema( std::shared_ptr< MetadataSchema > spSchema )
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
    m_useEncryption = false;
    m_oMetadataSet.clear();
    m_mapSchemas.clear();
    removedIds.clear();
    addedIds.clear();
    videoSegments.clear();
    m_encryptor.reset();
    m_hintEncryption.clear();
    for (auto& stat : m_stats) stat->clear();
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

std::string MetadataStream::serialize(Format& format)
{
    MetadataStream encryptedStream(*this);
    encryptedStream.encrypt();

    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    for (const auto& spSchema : m_mapSchemas)
        schemas.push_back(spSchema.second);

    Format::AttribMap attribs{ { "nextId", to_string(nextId) },
                               { "filepath", m_sFilePath },
                               { "checksum", m_sChecksumMedia },
                               { "hint", m_hintEncryption }, };
    return format.store(encryptedStream.m_oMetadataSet, schemas, videoSegments, m_stats, attribs);
}

void MetadataStream::deserialize(const std::string& text, Format& format)
{
    std::vector<std::shared_ptr<VideoSegment>> segments;
    std::vector<std::shared_ptr<MetadataSchema>> schemas;
    std::vector<MetadataInternal> metadata;
    Format::AttribMap attribs;
    format.parse(text, metadata, schemas, segments, m_stats, attribs);
    if(m_sFilePath.empty()) m_sFilePath = attribs["filepath"];
    nextId = from_string<IdType>(attribs["nextId"]);
    m_sChecksumMedia = attribs["checksum"];
    m_hintEncryption = attribs["hint"];
    for (const auto& spSegment : segments) addVideoSegment(spSegment);
    for (const auto& spSchema : schemas) addSchema(spSchema);
    for (auto& mdi : metadata) add(mdi);

    decrypt();
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


bool MetadataStream::getUseEncryption() const
{
    return m_useEncryption;
}

void MetadataStream::setUseEncryption(bool useEncryption)
{
    m_useEncryption = useEncryption;
}

std::shared_ptr<Encryptor> MetadataStream::getEncryptor() const
{
    return m_encryptor;
}

void MetadataStream::setEncryptor(std::shared_ptr<Encryptor> encryptor)
{
    m_encryptor = encryptor;
}


void MetadataStream::encrypt()
{
    //check everything we want to encrypt
    //toEncrypt[tuple(schemaname, descName, fieldName)], descName and fieldName can be ""
    typedef std::tuple<std::string, std::string, std::string> SubsetKey;
    std::map<SubsetKey, bool> toEncrypt;
    for(auto itSchema : m_mapSchemas)
    {
        std::string schemaName = itSchema.second->getName();
        if(itSchema.second->getUseEncryption())
        {
            toEncrypt[SubsetKey(schemaName, "", "")] = true;
        }
        else
        {
            for(auto itDesc : itSchema.second->getAll())
            {
                std::string descName = itDesc->getMetadataName();
                if(itDesc->getUseEncryption())
                {
                    toEncrypt[SubsetKey(schemaName, descName, "")] = true;
                }
                else
                {
                    for(FieldDesc& fd : itDesc->getFields())
                    {
                        if(fd.useEncryption)
                        {
                            toEncrypt[SubsetKey(schemaName, descName, fd.name)] = true;
                        }
                    }
                }
            }
        }
    }

    //do not change useEncryption field
    for(std::shared_ptr<Metadata>& meta : m_oMetadataSet)
    {
        //clone those SPs to MD records which need to be encrypted
        meta = std::make_shared<Metadata>(*meta);
        if(meta->getUseEncryption() ||
           toEncrypt[SubsetKey(meta->getSchemaName(), "", "")] ||
           toEncrypt[SubsetKey(meta->getSchemaName(), meta->getName(), "")])
        {
            //serialize and kill fields
            std::vector<std::string> fvStrings;
            for(std::string fvName : meta->getFieldNames())
            {
                FieldValue& fv = *meta->findField(fvName);
                fvStrings.push_back(fvName);
                fvStrings.push_back(fv.toString());
                fv = FieldValue(fvName, Variant(), fv.getUseEncryption());
            }
            std::string serialized = Variant(fvStrings).toString();

            vmf_rawbuffer encryptedBuf;
            if(m_encryptor)
            {
                m_encryptor->encrypt(serialized, encryptedBuf);
            }
            else
            {
                VMF_EXCEPTION(IncorrectParamException, "No encryptor provided while encryption is needed");
            }
            meta->setEncryptedData(Variant::base64encode(encryptedBuf));
        }
        else
        {
            for(std::string fvName : meta->getFieldNames())
            {
                FieldValue& fv = *meta->findField(fvName);
                if(fv.getUseEncryption() ||
                   toEncrypt[SubsetKey(meta->getSchemaName(), meta->getName(), fv.getName())])
                {
                    vmf_rawbuffer encryptedBuf;
                    if(m_encryptor)
                    {
                        m_encryptor->encrypt(fv.toString(), encryptedBuf);
                    }
                    else
                    {
                        VMF_EXCEPTION(IncorrectParamException,
                                      "No encryptor provided while encryption is needed");
                    }
                    std::string encoded = Variant::base64encode(encryptedBuf);
                    //kill the value
                    fv = FieldValue(fvName, Variant(), fv.getUseEncryption());
                    fv.setEncryptedData(encoded);
                }
            }
        }
    }
}


void MetadataStream::decrypt()
{
    bool ignoreBad = (m_eMode & MetadataStream::OpenModeFlags::IgnoreUnknownEncryptor) != 0;

    //just try to decrypt everything
    //but do not change useEncryption field
    for(std::shared_ptr<Metadata>& meta : m_oMetadataSet)
    {
        const std::string& encryptedData = meta->getEncryptedData();
        if(encryptedData.length() > 0)
        {
            if(!m_encryptor)
            {
                if(!ignoreBad)
                {
                    VMF_EXCEPTION(IncorrectParamException,
                                  "No decryption algorithm provided for encrypted data");
                }
            }
            else
            {
                vmf_rawbuffer encBuf = Variant::base64decode(encryptedData);
                std::string serialized;
                try
                {
                    m_encryptor->decrypt(encBuf, serialized);
                }
                catch(Exception& ee)
                {
                    //if we've failed with decryption (whatever the reason was)
                    //and we're allowed to ignore that
                    if(!ignoreBad)
                    {
                        std::string message = "Decryption failed: " + std::string(ee.what()) +
                                              ", hint: " + m_encryptor->getHint();
                        VMF_EXCEPTION(IncorrectParamException, message);
                    }
                }
                Variant varStrings; varStrings.fromString(Variant::type_string_vector, serialized);
                std::vector<std::string> vStrings = varStrings.get_string_vector();
                std::map<std::string, std::string> fvStrings;
                for(size_t i = 0; i < vStrings.size()/2; i++)
                {
                    std::string& sName = vStrings[i*2];
                    std::string& sVal  = vStrings[i*2+1];
                    fvStrings[sName] = sVal;
                }
                for(FieldDesc fd : meta->getDesc()->getFields())
                {
                    std::string fvName = fd.name;
                    if(fvStrings.find(fvName) != fvStrings.end())
                    {
                        std::string& sVal = fvStrings[fvName];
                        Variant v; v.fromString(fd.type, sVal);
                        meta->setFieldValue(fvName, v);
                        //forget about previous useEncryption status of the field
                    }
                }
                meta->setEncryptedData("");
            }
        }
        else
        {
            if(meta->getUseEncryption())
            {
                VMF_EXCEPTION(IncorrectParamException, "No encrypted metadata presented while the flag is on");
            }
            else
            {
                for(std::string fvName : meta->getFieldNames())
                {
                    FieldValue& fv = *meta->findField(fvName);
                    const std::string& encryptedData = fv.getEncryptedData();
                    if(encryptedData.length() > 0)
                    {
                        if(!m_encryptor)
                        {
                            if(!ignoreBad)
                            {
                                VMF_EXCEPTION(IncorrectParamException,
                                              "No decryption algorithm provided for encrypted data");
                            }
                        }
                        else
                        {
                            vmf_rawbuffer encBuf = Variant::base64decode(encryptedData);
                            std::string decrypted;
                            try
                            {
                                m_encryptor->decrypt(encBuf, decrypted);
                            }
                            catch(Exception& ee)
                            {
                                //if we've failed with decryption (whatever the reason was)
                                //and we're allowed to ignore that
                                if(!ignoreBad)
                                {
                                    std::string message = "Decryption failed: " + std::string(ee.what()) +
                                                          ", hint: " + m_encryptor->getHint();
                                    VMF_EXCEPTION(IncorrectParamException, message);
                                }
                            }
                            Variant v; v.fromString(fv.getType(), decrypted);
                            fv = FieldValue(fvName, v, fv.getUseEncryption());
                            fv.setEncryptedData("");
                        }
                    }
                    else
                    {
                        if(fv.getUseEncryption())
                        {
                            VMF_EXCEPTION(IncorrectParamException,
                                          "No encrypted field data provided while the flag is on");
                        }
                    }
                }
            }
        }
        //validate resulting metadata
        meta->validate();
    }
}


void MetadataStream::addVideoSegment(std::shared_ptr<VideoSegment> newSegment)
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

void MetadataStream::notifyStat(std::shared_ptr< Metadata > spMetadata, Stat::Action::Type action)
{
    for( auto& stat : m_stats )
    {
        stat->notify(spMetadata, action);
    }
}

void MetadataStream::recalcStat()
{
    for (auto& stat : m_stats)
        stat->clear();

    for (const auto& m : m_oMetadataSet)
        notifyStat(m);
}

void MetadataStream::addStat(std::shared_ptr<Stat> stat)
{
    const std::string& name = stat->getName();
    auto it = std::find_if(m_stats.begin(), m_stats.end(), [&name](std::shared_ptr<Stat> s){return s->getName() == name; });
    if (it != m_stats.end()) VMF_EXCEPTION(IncorrectParamException, "Statistics object already exists: " + name);
    m_stats.push_back(stat);
}

std::shared_ptr<Stat> MetadataStream::getStat(const std::string& name) const
{
    auto it = std::find_if(m_stats.begin(), m_stats.end(), [&name](std::shared_ptr<Stat> s){return s->getName() == name; });
    if (it == m_stats.end()) VMF_EXCEPTION(vmf::NotFoundException, "Statistics object not found: " + name);
    return *it;
}

std::vector< std::string > MetadataStream::getAllStatNames() const
{
    std::vector< std::string > names;
    for (auto& stat : m_stats)
        names.push_back(stat->getName());
    return names;
}

}//namespace vmf

