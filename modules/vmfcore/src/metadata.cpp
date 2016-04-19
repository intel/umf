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
#include "vmf/metadata.hpp"
#include <algorithm>

namespace vmf
{
Metadata::Metadata(const std::shared_ptr< MetadataDesc >& spDescription , bool useEncryption)
    : m_Id( INVALID_ID )
    , m_nFrameIndex(UNDEFINED_FRAME_INDEX)
    , m_nNumOfFrames(UNDEFINED_FRAMES_NUMBER)
    , m_nTimestamp(UNDEFINED_TIMESTAMP)
    , m_nDuration(UNDEFINED_DURATION)
    , m_sName( "" )
    , m_sSchemaName( "" )
    , m_useEncryption(useEncryption)
    , m_encryptedData("")
    , m_spDesc( spDescription )
    , m_pStream(nullptr)
{
    if (!m_spDesc)
    {
        VMF_EXCEPTION(NullPointerException, "Metadata description is null.");
    }

    m_sName = m_spDesc->getMetadataName();
    m_sSchemaName = m_spDesc->getSchemaName();
}

Metadata::Metadata( const Metadata& oMetadata )
{
    // Use default implementation of operator =.
    *this = oMetadata;
    m_pStream = nullptr;
}

Metadata::~Metadata(void)
{
}

IdType Metadata::getId() const
{
    return m_Id;
}

long long Metadata::getFrameIndex() const
{
    return m_nFrameIndex;
}

long long Metadata::getNumOfFrames() const
{
    return m_nNumOfFrames;
}

void Metadata::setFrameIndex( long long nFrameIndex, long long nNumOfFrames )
{
    if(nFrameIndex < 0 && nFrameIndex != UNDEFINED_FRAME_INDEX)
    {
        VMF_EXCEPTION(IncorrectParamException, "Can't set metadata frame index. Invalid frame index value: " + to_string(nFrameIndex));
    }

    if(nNumOfFrames < 0)
    {
        VMF_EXCEPTION(IncorrectParamException, "Can't set metadata number of frames. Invalid number of frames value: " + to_string(nNumOfFrames));
    }

    m_nFrameIndex = nFrameIndex;
    m_nNumOfFrames = nNumOfFrames;
}

void Metadata::setTimestamp(long long timestamp, long long duration)
{
    if(timestamp < 0 && timestamp != UNDEFINED_TIMESTAMP)
    {
        VMF_EXCEPTION(IncorrectParamException, "Can't set metadata timestamp. Invalid timestamp value: " + to_string(timestamp));
    }

    if(duration < 0)
    {
        VMF_EXCEPTION(IncorrectParamException, "Can't set metadata duration. Invalid duration value: " + to_string(duration));
    }

    m_nTimestamp = timestamp;
    m_nDuration = duration;
}

long long Metadata::getTime() const
{
    return m_nTimestamp;
}

long long Metadata::getDuration() const
{
    return m_nDuration;
}

bool Metadata::shiftFrameIndex( long long nTarFrameIndex, long long nSrcFrameIndex, long long nNumOfFrames )
{
    // Do not change frame index for global metadata
    if(m_nFrameIndex == UNDEFINED_FRAME_INDEX)
        return true;

    // Already invalid frame index. Do nothing to fix it.
    if( m_nNumOfFrames < 0 )
        return false;

    if( m_nFrameIndex + m_nNumOfFrames <= nSrcFrameIndex ||
        m_nFrameIndex >= nSrcFrameIndex + nNumOfFrames ) 
    {
        m_nNumOfFrames = 0;
        return false;
    }

    long long nNewFrameIndex = std::max<long long>( 0, m_nFrameIndex - nSrcFrameIndex ) + nTarFrameIndex;
    long long nNewNumOfFrames = std::min<long long>( m_nNumOfFrames, nTarFrameIndex + nNumOfFrames - nNewFrameIndex );

    if( nNewNumOfFrames < 1 )
    {
        m_nNumOfFrames = 0;
        return false;
    }

    m_nFrameIndex = nNewFrameIndex;
    m_nNumOfFrames = nNewNumOfFrames;

    return true;
}
std::string Metadata::getName() const
{
    return m_sName;
}

std::string Metadata::getSchemaName() const
{
    return m_sSchemaName;
}
std::shared_ptr< MetadataDesc > Metadata::getDesc() const
{
    return m_spDesc;
}
std::vector< std::string > Metadata::getFieldNames() const
{
    std::vector< std::string > vNames;

    std::for_each( this->begin(), this->end(), [ &vNames ]( const vmf::FieldValue& v )
    { 
        // Only add non-empty names
        std::string sName = v.getName();
        if( !sName.empty() )
        {
            vNames.push_back( sName ); 
        }
    });

    return vNames;
}

vmf::Variant Metadata::getFieldValue( const std::string& sName ) const
{
    const_iterator it = const_cast<vmf::Metadata*>(this)->findField( sName );
    if( it != this->end() )
        return *it;

    FieldDesc fieldDesc;
    if( !m_spDesc->getFieldDesc( fieldDesc, sName ) )
    {
        VMF_EXCEPTION(IncorrectParamException, "Metadata field not found in metadata description" );
    }

    if(fieldDesc.optional)
        return vmf::Variant();

    VMF_EXCEPTION(IncorrectParamException, "Field not found!");
}

Metadata::iterator Metadata::findField( const std::string& sFieldName )
{
    return std::find_if( this->begin(), this->end(), [&]( vmf::FieldValue& value )->bool 
    {
        return sFieldName == value.getName();
    });
}

Metadata::const_iterator Metadata::findField(const std::string& sFieldName) const
{
    return std::find_if(this->begin(), this->end(), [&](const vmf::FieldValue& value)->bool
    {
        return sFieldName == value.getName();
    });
}

bool Metadata::operator == ( const Metadata& oMetadata ) const
{
    if( this->m_Id == INVALID_ID && oMetadata.m_Id == INVALID_ID )
    {
        VMF_EXCEPTION(IncorrectParamException, "Cannot compare metadata that has not been added to any stream!" );
    }

    return m_Id == oMetadata.m_Id;
}

bool Metadata::operator < ( const Metadata& oMetadata ) const
{
    if( this->m_Id == INVALID_ID && oMetadata.m_Id == INVALID_ID )
    {
        VMF_EXCEPTION(IncorrectParamException, "Cannot compare metadata that has not been added to any stream!" );
    }

    return m_Id < oMetadata.m_Id;
}

std::shared_ptr<Metadata> Metadata::getFirstReference( const std::string& sMetadataName ) const
{
    for( auto it = m_vReferences.begin(); it != m_vReferences.end(); it ++ )
    {
        auto spReference = it->getReferenceMetadata().lock();
        if (spReference != nullptr)
        {
            if (spReference->getName() == sMetadataName)
            {
                // Found it, add into the list.
                return spReference;
            }
        }
    }

    return nullptr;
}

MetadataSet Metadata::getReferencesByMetadata(const std::string& sMetadataName) const
{
    MetadataSet mdSet;

    if (sMetadataName.empty())
    {
        VMF_EXCEPTION(ValidateException, "MetadataName is empty!");
    }
    else
    {
        std::for_each(m_vReferences.begin(), m_vReferences.end(), [&](const Reference& ref)
        {
            auto spMetadata = ref.getReferenceMetadata().lock();
            if (spMetadata != nullptr)
            {
                if (spMetadata->getName() == sMetadataName)
                {
                    mdSet.emplace_back(spMetadata);
                }
            }
        });
    }

    return mdSet;
}

MetadataSet Metadata::getReferencesByName(const std::string& sRefName) const
{
    MetadataSet mdSet;

    std::for_each(m_vReferences.begin(), m_vReferences.end(), [&](const Reference& ref)
    {
        auto spDesc = ref.getReferenceDescription();
        auto spMetadata = ref.getReferenceMetadata().lock();
        if ((spMetadata != nullptr) || (spDesc != nullptr))
        {
            if (spDesc->name == sRefName)
                mdSet.emplace_back(spMetadata);
        }
    });

    return mdSet;
}

const std::vector<Reference>& Metadata::getAllReferences() const
{
    return m_vReferences;   
}

bool Metadata::isReference(const IdType& id, const std::string& refName) const
{
    for( auto it = m_vReferences.begin(); it != m_vReferences.end(); it ++ )
    {
        auto spMetadata = it->getReferenceMetadata().lock();
        auto spDesc = it->getReferenceDescription();

        if ((spMetadata != nullptr) || (spDesc != nullptr))
        {
            if ((spDesc->name == refName) && (spMetadata->getId() == id))
            {
                // Found reference
                return true;
            }
        }
    }

    return false;
}

bool Metadata::isReference(const std::shared_ptr<Metadata>& md, const std::string& refName) const
{
    for (auto it = m_vReferences.begin(); it != m_vReferences.end(); it++)
    {
        auto spDesc = it->getReferenceDescription();
        auto spMetadata = it->getReferenceMetadata().lock();
        
        if ((spMetadata != nullptr) || (spDesc != nullptr))
        {
            if ((spDesc->name == refName) && (spMetadata == md))
                return true;
        }
    }

    return false;
}

void Metadata::addReference(const std::shared_ptr<Metadata>& md, const std::string& refName)
{
    if (md == nullptr)
        VMF_EXCEPTION(NullPointerException, "Metadata is nullptr!" );

    if (md->getId() == INVALID_ID)
        VMF_EXCEPTION(IncorrectParamException, "Metadata has to be added into stream before being referenced!" );

    if (m_pStream != nullptr && md->m_pStream != m_pStream)
        VMF_EXCEPTION(IncorrectParamException, "Referenced metadata is from different metadata stream." );

    if (isReference(md, refName))
        VMF_EXCEPTION(IncorrectParamException, "This reference already exist.");

    auto spRefDesc = m_spDesc->getReferenceDesc(refName);

    if (!spRefDesc)
        VMF_EXCEPTION(IncorrectParamException, "No such reference description.");

    if (spRefDesc->isUnique)
    {
        auto mdSet = getReferencesByName(refName);
        auto itemsExist = mdSet.size();

        if (itemsExist == 0)
        {
            m_vReferences.emplace_back(Reference(spRefDesc, md));
        }
        else
            VMF_EXCEPTION(IncorrectParamException, "Unique reference with this name already exists");
    }
    else
    {
        m_vReferences.emplace_back(Reference(spRefDesc, md));
    }

    return;
}

void Metadata::removeReference(const IdType& id, const std::string& refName)
{
    for( auto it = m_vReferences.begin(); it != m_vReferences.end(); it ++ )
    {
        auto spDesc = it->getReferenceDescription();
        auto spMetadata = it->getReferenceMetadata().lock();
        
        if ((spMetadata != nullptr) && (spDesc != nullptr))
        {
            if ((spMetadata->getId() == id) && (spDesc->name == refName))
            {
                // Found reference
                m_vReferences.erase(it);
                break;
            }
        }
    }

    return;
}

void Metadata::removeReference(const std::shared_ptr<Metadata>& md, const std::string& refName)
{
    for (auto it = m_vReferences.begin(); it != m_vReferences.end(); it++)
    {
        auto spDesc = it->getReferenceDescription();
        auto spMetadata = it->getReferenceMetadata().lock();

        if ((spMetadata != nullptr) && (spDesc != nullptr))
        {
            if ((spMetadata == md) && (spDesc->name == refName))
            {
                m_vReferences.erase(it);
                break;
            }
        }
    }

    return;
}

void Metadata::addValue( const vmf::Variant& value )
{
    // Check field against description
    if( m_spDesc == nullptr )
    {
        VMF_EXCEPTION(NullPointerException, "Metadata description object is missing!" );
    }

    FieldDesc fieldDesc;
    if( !m_spDesc->getFieldDesc( fieldDesc ) )
    {
        VMF_EXCEPTION(InternalErrorException, "Metadata field not found in metadata description" );
    }

    // Check field type
    if( fieldDesc.type != value.getType() )
    {
        VMF_EXCEPTION(TypeCastException, "Field type does not match!" );
    }

    this->emplace_back( FieldValue( "", value ) );
}

void Metadata::setFieldValue( const std::string& sFieldName, const vmf::Variant& value )
{
    // Check field against description
    if( m_spDesc == nullptr )
    {
        VMF_EXCEPTION(NullPointerException, "Metadata description object is missing!" );
    }

    FieldDesc fieldDesc;
    if( !m_spDesc->getFieldDesc( fieldDesc, sFieldName ) )
    {
        VMF_EXCEPTION(IncorrectParamException, "Metadata field not found in metadata description" );
    }

    iterator it = this->findField( sFieldName );

    // Check field type
    if( fieldDesc.type == value.getType() )
    {
        if( it != this->end() )
        {
            *it = vmf::FieldValue( sFieldName, value );
        }
        else
        {
            this->emplace_back(sFieldName, value);
        }
    }
    // If the field type is not the same, try to convert it to the right type
    else
    {
        vmf::Variant varNew( value );
        // This line may throw exception
        varNew.convertTo( fieldDesc.type );

        if( it != this->end() )
        {
            *it = vmf::FieldValue( sFieldName, varNew );
        }
        else
        {
            this->emplace_back( FieldValue( sFieldName, varNew ) );
        }
    }
}

void Metadata::validate() const
{
    size_t nNumOfValues = this->size();
    if( nNumOfValues < 1 && this->getEncryptedData().empty())
    {
        VMF_EXCEPTION(ValidateException, "The metadata contains neither value nor encrypted data" );
    }

    if ( (m_nFrameIndex < 0 && m_nFrameIndex != UNDEFINED_FRAME_INDEX) || m_nNumOfFrames < 0 )
    {
        VMF_EXCEPTION(ValidateException, "Invalid frame index." );
    }

    if ( (m_nTimestamp < 0 && m_nTimestamp != UNDEFINED_TIMESTAMP) || m_nDuration < 0 )
    {
        VMF_EXCEPTION(ValidateException, "Invalid timestamp." );
    }

    if( this->m_spDesc == nullptr )
        throw std::runtime_error( "Descriptor object was not found!" );

    if(this->getEncryptedData().empty())
    {
        auto fields = this->getDesc()->getFields();
        for(auto f = fields.begin(); f != fields.end(); f++)
            if( !f->optional && findField(f->name) == end() )
                VMF_EXCEPTION(ValidateException,
                              "All non-optional fields in a structure need to have not-empty field value!" );
    }

    if( nNumOfValues > 0 )
    {
        std::vector< std::string > vNames = this->getFieldNames();
        size_t nNumOfFieldNames = vNames.size();

        // Structure item
        if( nNumOfFieldNames > 0 )
        {
            // Make sure the number of non-empty field names matches with the number of values
            if( nNumOfValues != nNumOfFieldNames )
            {
                VMF_EXCEPTION(ValidateException, "All fields in a structure need to have field names!" );
            }

            // Now check to see if there are duplicates in the name vector
            std::vector< std::string > vUniqueNames = vNames;
            std::sort( vUniqueNames.begin(), vUniqueNames.end() );
            vUniqueNames.erase( std::unique( vUniqueNames.begin(), vUniqueNames.end()), vUniqueNames.end() );
            if( vUniqueNames.size() < vNames.size() )
                throw std::runtime_error( "A structure cannot have duplicate field names!" );

            // Check field names and field types against field description property.
            for (const auto& sFieldName : vUniqueNames)
            {
                FieldDesc field;
                if( false == m_spDesc->getFieldDesc( field, sFieldName ))
                {
                    VMF_EXCEPTION(ValidateException, "Field specified[" + sFieldName + "] not found!" );
                }

                vmf::Variant varValue = this->getFieldValue( sFieldName );
                if( field.type != varValue.getType() )
                {
                    VMF_EXCEPTION(ValidateException, "Field type does not match with the descriptor!" );
                }
            }

        }
        // Array and single item value
        else if( nNumOfValues >= 1 )
        {
            FieldDesc field;
            if( false == m_spDesc->getFieldDesc( field ))
                throw std::runtime_error("Field descriptor is not for Array type.");

            vmf::Variant::Type eFirstValueType = this->at( 0 ).getType();
            if( field.type != eFirstValueType )
                throw std::runtime_error( "Value type does not match with type specified in descriptor!" );

            for( size_t i = 1; i < nNumOfValues; i ++ )
            {
                if( this->at( i ).getType() != eFirstValueType )
                {
                    VMF_EXCEPTION(ValidateException, "Metadata without field name should contain values of same type!" );
                }
            }
        }
    }
    for (auto it = m_vReferences.begin(); it != m_vReferences.end(); ++it)
    {
        auto spReference = it->getReferenceMetadata().lock();
        if (spReference == nullptr)
        {
            VMF_EXCEPTION(ValidateException, "At least one of references points unexistent data");
        }
    }
}

void Metadata::setId( const IdType& id )
{
    m_Id = id;
}

bool Metadata::getUseEncryption() const
{
    return m_useEncryption;
}

void Metadata::setUseEncryption(bool useEncryption)
{
    m_useEncryption = useEncryption;
}

const std::string& Metadata::getEncryptedData() const
{
    return m_encryptedData;
}

void Metadata::setEncryptedData(const std::string &encData)
{
    m_encryptedData = encData;
}

bool Metadata::isValid() const
{
    bool bValid = true;
    try
    {
        validate();
    }
    catch( std::exception& )
    {
        bValid = false;
    }

    return bValid;
}

void Metadata::removeInvalidReferences()
{
    std::vector<Reference> setNew;
    std::for_each( m_vReferences.begin(), m_vReferences.end(), [&]( Reference& ref )
    {
        auto spReference = ref.getReferenceMetadata().lock();
        if( spReference != nullptr && spReference->isValid() )
        {
            setNew.push_back(ref);
        }
    });

    if( setNew.size() < m_vReferences.size() )
        std::swap( setNew, m_vReferences );
}

void Metadata::removeAllReferences()
{
    m_vReferences.clear();
}

void Metadata::setDescriptor( const std::shared_ptr< MetadataDesc >& spDescriptor )
{
    m_spDesc = spDescriptor;
}

void Metadata::setStreamRef(const MetadataStream* streamPtr)
{
    m_pStream = streamPtr;
}

} //namespace vmf
