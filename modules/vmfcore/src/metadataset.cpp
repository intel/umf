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
#include "vmf/metadataset.hpp"
#include <algorithm>

namespace vmf
{
MetadataSet::MetadataSet(void)
{
}
    
MetadataSet::MetadataSet( MetadataSet&& rhs ) 
: std::vector< std::shared_ptr< Metadata >>( std::move( rhs ))
{
}
    
MetadataSet::MetadataSet( const MetadataSet& rhs ) 
    : std::vector< std::shared_ptr< Metadata >>( rhs.begin(), rhs.end())
{
}

MetadataSet::~MetadataSet(void)
{
}

MetadataSet& MetadataSet::operator = ( MetadataSet&& rhs )
{
    std::vector< std::shared_ptr< Metadata >>::operator = ( std::move( rhs ));
    return *this;
}

MetadataSet MetadataSet::query( std::function< bool( const std::shared_ptr<Metadata>& )> filter ) const
{
    MetadataSet set;
    std::for_each( this->begin(), this->end(), [&]( const std::shared_ptr< Metadata >& spItem )
    {
        if( filter( spItem ) )
            set.push_back( spItem );
    });
    return set;
}

MetadataSet MetadataSet::queryByReference( std::function< bool( const std::shared_ptr<Metadata>& spMetadata, const std::shared_ptr<Metadata>& spReference )> filter ) const
{
    MetadataSet set;
    std::for_each( this->begin(), this->end(), [&]( const std::shared_ptr< Metadata >& spItem )
    {
        auto it = std::find_if( spItem->m_vReferences.begin(), spItem->m_vReferences.end(), [&]( const Reference& wpItemRef )->bool
        {
            const std::shared_ptr<Metadata> spItemRef = wpItemRef.getReferenceMetadata().lock();
            if( spItemRef != nullptr )
                return filter( spItem, spItemRef );
            return false;
        });
        if( it != spItem->m_vReferences.end() )
            set.push_back( spItem );
    });
    return set;
}

MetadataSet MetadataSet::queryByName( const std::string& sName ) const
{
    MetadataSet set = query( [&]( const std::shared_ptr< Metadata >& spItem )->bool
    {
        return ( spItem->getName() == sName );
    });

    return set;
}
MetadataSet MetadataSet::queryByNameAndValue( const std::string& sMetadataName, const vmf::FieldValue& value ) const
{
    MetadataSet set = query([&](const std::shared_ptr< Metadata >& spItem)->bool
    {
        if ((spItem->getName() != sMetadataName) || (spItem->size() == 0))
            return false;

        auto it = spItem->findField(value.getName());
        return ((it != spItem->end()) && (*it == value));
    });

    return set;
}

MetadataSet MetadataSet::queryByNameAndFields( const std::string& sMetadataName, const std::vector< vmf::FieldValue>& vFields ) const
{
    MetadataSet set = query( [&]( const std::shared_ptr< Metadata >& spItem )->bool
    {
        if( spItem->getName() == sMetadataName && spItem->size() > 0 )
        {
            auto itFailed = std::find_if( vFields.begin(), vFields.end(), [&]( const vmf::FieldValue& value )->bool
            {
                std::string sFieldName = value.getName();
                auto it = spItem->findField( sFieldName );
                if( it == spItem->end() || *it != value )
                {
                    // Found a field that does not exist, or the value is not the same
                    return true;
                }
                return false;
            });

            // Succeed if there is no field failed
            if( itFailed == vFields.end() )
            {
                return true;
            }
        }
        return false;
    });

    return set;
}

MetadataSet MetadataSet::queryBySchema( const std::string& sSchemaName ) const
{
    MetadataSet set = query( [&]( const std::shared_ptr< Metadata >& spItem )->bool
    {
        return ( spItem->getSchemaName() == sSchemaName );
    });

    return set;
}

MetadataSet MetadataSet::queryByFrameIndex( size_t index ) const
{
    MetadataSet set = query( [&]( const std::shared_ptr< Metadata >& spItem )->bool
    {
        return ( index >= (size_t)spItem->getFrameIndex() && index < (size_t)spItem->getFrameIndex() + (size_t)spItem->getNumOfFrames()  );
    });

    return set;
}

MetadataSet MetadataSet::queryByTime( long long startTime, long long endTime ) const
{
    MetadataSet set = query( [&]( const std::shared_ptr< Metadata >& spItem )->bool
    {
        long long itemStart = spItem->getTime(), itemEnd = itemStart + spItem->getDuration();
        return (itemStart >= 0) && (itemEnd >= startTime) && (itemStart <= endTime);
    });

    return set;
}

MetadataSet MetadataSet::queryByReference( const std::string& sReferenceName ) const
{
    MetadataSet set = query( [&]( const std::shared_ptr< Metadata >& spItem )->bool
    {
        MetadataSet setReference = spItem->getReferencesByMetadata( sReferenceName );
        return ( setReference.size() > 0 );
    });

    return set;
}

MetadataSet MetadataSet::queryByReference( const std::string& sReferenceName, const vmf::FieldValue& value ) const
{
    std::string sFieldName = value.getName();
    MetadataSet set;

    // For anonymous field, check the first field only
    if( sFieldName.empty() )
    {
        set = query( [&]( const std::shared_ptr< Metadata >& spItem )->bool
        {
            MetadataSet referenceSet = spItem->getReferencesByMetadata(sReferenceName);
            if(  referenceSet.size() > 0 )
            {
                auto itReference = std::find_if( referenceSet.begin(), referenceSet.end(), [&]( const std::shared_ptr< Metadata >& spReference )->bool
                {
                    // Just compare the first field, since it has no field
                    if( spReference->size() > 0 && spReference->at(0) == value )
                    {
                        return true;
                    }

                    return false;
                });

                return itReference != referenceSet.end();
            }
            return false;
        });
    }
    // For named field
    else
    {
        set = query( [&]( const std::shared_ptr< Metadata >& spItem )->bool
        {
            MetadataSet referenceSet = spItem->getReferencesByMetadata(sReferenceName);
            if( referenceSet.size() > 0 )
            {
                auto itReference = std::find_if( referenceSet.begin(), referenceSet.end(), [&]( const std::shared_ptr< Metadata >& spReference )->bool
                {
                    auto it = spReference->findField( sFieldName );
                    if( it != spReference->end() && *it == value )
                    {
                        return true;
                    }

                    return false;
                });

                return itReference != referenceSet.end();
            }
            return false;
        });
    }

    return set;
}

MetadataSet MetadataSet::queryByReference( const std::string& sReferenceName, const std::vector< vmf::FieldValue>& vFields ) const
{
    MetadataSet set;
    if( vFields.size() > 0 )
    {
        set = query( [&]( const std::shared_ptr< Metadata >& spItem )->bool
        {
            MetadataSet referenceSet = spItem->getReferencesByMetadata(sReferenceName);
            if( referenceSet.size() > 0 )
            {
                auto itReference = std::find_if( referenceSet.begin(), referenceSet.end(), [&]( const std::shared_ptr< Metadata >& spReference )->bool
                {
                    auto itFailed = std::find_if( vFields.begin(), vFields.end(), [&]( const vmf::FieldValue& value )->bool
                    {
                        std::string sFieldName = value.getName();
                        auto it = spReference->findField( sFieldName );
                        if( it == spReference->end() || *it != value )
                        {
                            // Found a field that does not exist, or the value is not the same
                            return true;
                        }
                        return false;
                    });

                    // Succeed if there is no field failed
                    if( itFailed == vFields.end() )
                    {
                        return true;
                    }

                    return false;
                });

                return itReference != referenceSet.end();
            }
            return false;
        });
    }

    return set;
}

size_t MetadataSet::shift( long long nTarFrameIndex, long long nSrcFrameIndex, long long nNumOfFrames, MetadataSet* pSetFailure )
{
	if( pSetFailure != NULL )
		pSetFailure->clear();

    // Shift all metadata
    MetadataSet set;
    std::for_each( this->begin(), this->end(), [&]( std::shared_ptr<Metadata>& spMetadata )
    {
        if( spMetadata->shiftFrameIndex(nTarFrameIndex, nSrcFrameIndex, nNumOfFrames ))
        {
            set.push_back( spMetadata );
        }
        else if( pSetFailure != NULL )
        {
            pSetFailure->push_back( spMetadata );
        }
    });

    // Remove invalid references
    std::for_each( set.begin(), set.end(), [&]( std::shared_ptr<Metadata>& spMetadata )
    {
        spMetadata->removeInvalidReferences();
    });

    // Update the container with the new set
    std::swap( *this, set );

    return this->size();
}
}
