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
* \file metadatadesc.hpp
* \brief %MetadataDesc class header file
*/

#ifndef __VMF_METADATA_DESC_H__
#define __VMF_METADATA_DESC_H__

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

#include <vector>
#include "variant.hpp"
#include "config.hpp"

namespace vmf
{
/*!
* \struct FieldDesc
* \brief Field descrption structure
*/
struct FieldDesc
{
    /*!
    * \brief Structure constructor
    * \param sName [in] field description name
    * \param eType [in] field type
    */
    FieldDesc( const std::string& sName = "", Variant::Type eType = Variant::type_string, bool isOptional = false ) : name( sName ), type( eType ), optional(isOptional) {};

    /*!
    * \brief field name
    */
    std::string		name;

    /*!
    * \brief field type
    */
    Variant::Type	type;

    /*!
    * \brief field optional
    */
    bool        	optional;

    /*!
    * \brief Compare operator
    * \param oth [in] another field description object
    * \return comparsion result
    */
    bool operator==(const FieldDesc& oth) const
    {
        return (name == oth.name) && (type == oth.type);
    }
};

typedef struct ReferenceDesc
{
    std::string name;
    bool isUnique;
    bool isCustom;

    ReferenceDesc() : name(""), isUnique(false), isCustom(false){};
    ReferenceDesc(const std::string& refName, bool isUnique = false, bool isCustom = false) : name(refName), isUnique(isUnique), isCustom(isCustom){};

} ReferenceDesc;

/*!
* \class MetadataDesc
* \brief The class describe metadata content
*/
class VMF_EXPORT MetadataDesc
{
    friend class MetadataSchema;

public:
    
    MetadataDesc();
    /*!
    * \brief Class constructor for constructing struct-type metadata descriptor
    * \param sMetadataName [in] metadata name
    * \param vFields [in] metadata description fields
    * \throw ValidateException if input data is invalid or inconsistent
    */
    MetadataDesc(const std::string& sMetadataName, const std::vector< FieldDesc >& vFields);

    MetadataDesc(const std::string& sMetadataName, const std::vector< FieldDesc >& vFields, const std::vector<std::shared_ptr<ReferenceDesc>>& vRefs);

    /*!
    * \brief Class constructor for single-value descriptor or array type descriptor
    * \param sMetadataName [in] metadata name
    * \param type [in] value type
    */
    MetadataDesc( const std::string& sMetadataName, Variant::Type type );

    /*!
    * \brief Class destructor
    */
    virtual ~MetadataDesc(void);

    /*!
    * \brief Get metadata name
    * \return metadata name
    */
    std::string	getMetadataName() const;

    /*!
    * \brief Get metadata schema name
    * \return metadata schema name
    */
    std::string getSchemaName() const;

    /*!
    * \brief Get metadata description fields
    * \return list of metadata description fields
    */
    std::vector<FieldDesc> getFields() const;

    const std::vector<std::shared_ptr<ReferenceDesc>>& getAllReferenceDescs() const;

    void declareCustomReference(const std::string& refName, bool isUnique = false);

    std::shared_ptr<ReferenceDesc> getReferenceDesc(const std::string& refName) const;

    /*!
    * \brief Get metadata description field by name
    * \param field [out] field object
    * \param sFieldName [in] field name. This should be empty for single value descriptor or array-type descriptor.
    * \retval true if field exists
    * \retval false otherwise
    */
    bool getFieldDesc( FieldDesc& field, const std::string& sFieldName = "" ) const;

protected:
    void validate();
    void setSchemaName( const std::string& sAppName );

private:
    std::string					m_sSchemaName;
    std::string					m_sMetadataName;
    std::vector< FieldDesc >	m_vFields;
    std::vector<std::shared_ptr<ReferenceDesc>>  m_vRefDesc;
};
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif /* __VMF_METADATA_DESC_H__ */
