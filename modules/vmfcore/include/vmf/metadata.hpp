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
* \file metadata.hpp
* \brief %Metadata class header file
*/

#pragma once
#ifndef __VMF_METADATA_H__
#define __VMF_METADATA_H__

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <stdexcept>
#include <limits>
#include "vmf/fieldvalue.hpp"
#include "vmf/metadatadesc.hpp"
#include "vmf/metadatareference.hpp"

namespace vmf
{
class MetadataSet;
class MetadataStream;
class Reference;

#define FRAME_COUNT_ALL		std::numeric_limits<long long>::max()

/*!
* \class Metadata
* \brief The class contains values of metadata items
*/
class VMF_EXPORT Metadata : public std::vector< vmf::FieldValue >
{
    friend class MetadataSet;
    friend class MetadataStream;

public:
    /*!
    * \brief Class constructor
    * \param spDescription [in] metadata item description
    * \throw NullPointerException if metadata description pointer is null
    */
    Metadata( const std::shared_ptr< MetadataDesc >& spDescription );

    /*!
    * \brief Class copy constructor
    * \param oMetadata [in] based metadata object
    */
    Metadata( const Metadata& oMetadata );

    /*!
    * \brief Class destructor
    */
    virtual ~Metadata(void);

    /*!
    * \brief Get metadata item identifier
    * \return metadata numeric identifier
    */
    IdType getId() const;

    /*!
    * \brief Get metadata item frame index
    * \return frame index
    */
    long long getFrameIndex() const;

    /*!
    * \brief Get number of frames
    * \return number of frames
    */
    long long getNumOfFrames() const; 

    /*!
    * \brief Set frame index of metadata item
    * \param nFrameIndex [in] frame index
    * \param nNumOfFrames [in] frames count
    * \note: Be aware that unspecified 'nNumOfFrames' value is interpreted as '1', not as 'UNDEFINED_FRAMES_NUMBER'!
    */
    void setFrameIndex( long long nFrameIndex, long long nNumOfFrames = 1 );

    /*
    * \brief Set Timestamp of metadata item
    * \param time [in] time in Unix time format in miliseconds
    * \param duration [in] duration of the metadata item in miliseconds (cool moment or event for example)
    */
    void setTimestamp(long long time, long long duration = UNDEFINED_DURATION);

    /*
    * \brief Get metadata item Unix time in miliseconds
    * return Unix time
    */
    long long getTime() const;

    /*
    * \brief Get metadata item duration in miliseconds
    * \return duration
    */
    long long getDuration() const;

    /*!
    * \brief Get metadata item name
    * \return metadata item name
    */
    std::string getName() const;

    /*!
    * \brief Get current schema name
    * \return Schema name
    * \details Schema name may be empty if metadata 
    * has not been added to metadata stream
    */
    std::string getSchemaName() const;

    /*!
    * \brief Returns list of field names
    * \return list of field names
    */
    std::vector< std::string > getFieldNames() const;

    /*!
    * \brief Get value of specified field
    * \param sName [in] field name
    * \return field value
    * \throw IncorrectParamException if field with specified name not found
    * \note For missing optional fields returns an empty Variant value
    *       that can be checked via 'Variant::isEmpty()' call.
    */
    vmf::Variant getFieldValue( const std::string& sName = std::string()) const;

    /*!
    * \brief Find field by name
    * \param sFieldName [in] field name
    * \return Iterator to the specified field
    */
    iterator findField( const std::string& sFieldName );

    /*!
    * \brief Const version of finding field by name
    * \param sFieldName [in] field name
    * \return Const_tierator to the specified field
    */
    const_iterator findField(const std::string& sFieldName) const;
    /*!
    * \brief Checks if the field is present (i.e. has a value) in the metadata
    */
    bool hasField(const std::string& sFieldName) const
    {
        return this->end() != findField(sFieldName);
    }

    /*!
    * \brief Compare two metadata objects
    * \param oMetadata [in] Metadata object to compare
    * \return comparsion result
    * \throw IncorrectParamException if metadata has not been added to metadata stream
    */
    bool operator == ( const Metadata& oMetadata ) const;

    /*!
    * \brief Check that current matadata object is less than another
    * \param oMetadata [in] metadata object to compare
    * \throw IncorrectParamException if metadata has not been added to metadata stream
    */
    bool operator < ( const Metadata& oMetadata ) const;

    /*!
    * \brief Get first referenced metadata
    * \param sMetadataName [in]  metadata object name
    * \return pointer to metadata object or null if metadata with specified name not found
    */
    std::shared_ptr<Metadata> getFirstReference( const std::string& sMetadataName ) const;

    MetadataSet getReferencesByMetadata(const std::string& mdName) const;

    MetadataSet getReferencesByName(const std::string& refName) const;

    /*!
    * \brief Get all referenced metadata
    * \return set of referenced metadata objects
    */
    const std::vector<Reference>& getAllReferences() const;

    /*!
    * \brief Check that metadata object with specified id has been added as
    * reference
    * \param id [in] metadata object identifier
    * \return Checking result
    */
    bool isReference(const IdType& id, const std::string& refName = "") const;

    bool isReference(const std::shared_ptr<Metadata>& spMetadata, const std::string& refName = "") const;

    /*!
    * \brief Add reference to othe metadata object
    * \throw IncorrectParamException if reference is not unique
    * \param sRefName [in] name of reference
    * \param spMetadata [in] pointer to metadata object
    * \param bEnsureUnique [in] flag to check that reference is unique
    */
    void addReference(const std::shared_ptr<Metadata>& spMetadata, const std::string& refName = "");

    /*!
    * \brief Remove reference to metadata item with specified identifier
    * \param id [in] identifier of referenced metadata object
    */
    void removeReference(const IdType& id, const std::string& refName = "");

    void removeReference(const std::shared_ptr<Metadata>& md, const std::string& refName = "");

    /*!
    * \brief Set value of specified field
    * \param sFieldName [in] name of field
    * \param value [in] new field value
    * \throw NullPointerException when metadata field description object is null
    * \throw IncorrectParamException when field description not found
    */
    void setFieldValue( const std::string& sFieldName, const vmf::Variant& value );

    /*!
    * \brief Add new value with empty name to metadata item
    * \param value [in] value to add
    * \throw NullPointerException when metadata field description object is null
    * \throw InternalErrorException when field description not found
    * \throw TypeCastException if field type is incompatible with type of value
    */
    void addValue( const vmf::Variant& value );


    /*!
    * \brief Get the descriptor of the metadata
    * \return the descriptor of the metadata.
    */
    std::shared_ptr< MetadataDesc > getDesc() const;


    /*!
    * \brief Validate metadata item
    * \throw ValidateException throws if data content not compatible with metadata description
    */
    void validate() const;

    /*!
    * \brief Check that metadata is valid
    * \return checking result
    */
    bool isValid() const;

    enum {
        UNDEFINED_FRAME_INDEX = -1, UNDEFINED_FRAMES_NUMBER = 0,
        UNDEFINED_TIMESTAMP = -1, UNDEFINED_DURATION = 0,
    };

protected:
    void setId( const IdType& id );
    bool shiftFrameIndex( long long nTarFrameIndex, long long nSrcFrameIndex, long long nNumOfFrames = FRAME_COUNT_ALL );
    void removeInvalidReferences();
    bool operator > ( const Metadata& ) const { throw std::runtime_error("Operator not available!"); }

    void removeAllReferences();
    void setDescriptor( const std::shared_ptr< MetadataDesc >& spDescriptor );
    void setStreamRef(const MetadataStream* streamPtr);

private:
    IdType			m_Id;
    long long		m_nFrameIndex;
    long long		m_nNumOfFrames;
    long long		m_nTimestamp;
    long long		m_nDuration;
    std::string		m_sName;
    std::string		m_sSchemaName;

    std::vector<Reference> m_vReferences;
    std::shared_ptr< MetadataDesc >	m_spDesc;
    const MetadataStream *m_pStream;
};
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif /* __VMF_METADATA_H__ */
