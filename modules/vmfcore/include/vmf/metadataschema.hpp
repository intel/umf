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
* \file metadataschema.hpp
* \brief %MetadataSchema class header file
*/

#ifndef __VMF_METADATA_SCHEMA_H__
#define __VMF_METADATA_SCHEMA_H__

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

#include <vector>
#include <memory>

#include "metadatadesc.hpp"

namespace vmf
{

/*!
* \brief The class provides functionality to create and manipulate metadata schema
*/
class VMF_EXPORT MetadataSchema : protected std::vector< std::shared_ptr< MetadataDesc > >
{
public:

    /*!
    * \brief Class constructor. Create new schema with specified name
    * \param sName [in] schema name
    * \param sAuthor [in] author of the schema
    * \throw IncorrectParamException if schema name is empty
    */
    MetadataSchema( const std::string& sName, const std::string& sAuthor = "" );

    /*!
    * \brief Class destructor
    */
    virtual ~MetadataSchema(void);

    /*!
    * \brief Get schema name
    * \return name of metadata schema
    */
    std::string getName() const;

    /*!
    * \brief Get schema author
    * \return the author of metadata schema
    */
    std::string getAuthor() const;

    /*!
    * \brief Get count of schema items
    * \return schema items count
    */
    size_t size() const;

    /*!
    * \brief Add new metadata description
    * \param spDesc [in] pointer to metadata description
    * \throw NullPointerException if pointer of metadata description is null
    * \throw IncorrectParamException if metadata description with the same
    * name already exists
    * \throw ValidateException if metadata description object is invalid
    */
    void add( std::shared_ptr< MetadataDesc >& spDesc );

    /*!
    * \brief Find metadata description by its names
    * \param sMetadataName [in] metadata description name
    * \return pointer to metadata description object of null
    * if metadata description not found
    */
    const std::shared_ptr< MetadataDesc > findMetadataDesc( const std::string& sMetadataName ) const;

    std::vector< std::shared_ptr< MetadataDesc >> getAll() const;

    enum StdSchemaKind { STD_DST };

    /*!
    * \brief Get name of standard predefined metadata schema
    * \param kind [in] standard schema kind
    * \return string schema name (e.g. "vmf://ns.intel.com/vmf/std-dst-schema-1.0")
    */
    static std::string getStdSchemaName(StdSchemaKind kind = STD_DST);

    /*!
    * \brief Get description of standard predefined metadata schema
    * \param kind [in] standard schema kind
    * \return pointer to metadata description object
    */
    static std::shared_ptr< MetadataSchema > getStdSchema(StdSchemaKind kind = STD_DST);

protected:
    bool load( const std::string& sSchemaFilePath );

private:
    std::string m_sName;
    std::string m_sAuthor;
};


#define VMF_METADATA_BEGIN( name ) \
{\
    std::vector< vmf::FieldDesc > fields;\
    std::string _name( name );

#define VMF_METADATA_END( schema ) \
    auto desc = std::make_shared< vmf::MetadataDesc >( _name, fields ); \
    schema->add( desc );\
};

#define VMF_FIELD_STR_( name, isOptional ) \
    fields.emplace_back( vmf::FieldDesc( name, vmf::Variant::type_string, isOptional ));
#define VMF_FIELD_STR( name ) VMF_FIELD_STR_( name, false )
#define VMF_FIELD_STR_OPT( name ) VMF_FIELD_STR_( name, true )

#define VMF_FIELD_INT_( name, isOptional ) \
    fields.emplace_back( vmf::FieldDesc( name, vmf::Variant::type_integer, isOptional ));
#define VMF_FIELD_INT( name ) VMF_FIELD_INT_( name, false )
#define VMF_FIELD_INT_OPT( name ) VMF_FIELD_INT_( name, true )

#define VMF_FIELD_REAL_( name, isOptional ) \
    fields.emplace_back( vmf::FieldDesc( name, vmf::Variant::type_real, isOptional ));
#define VMF_FIELD_REAL( name ) VMF_FIELD_REAL_( name, false )
#define VMF_FIELD_REAL_OPT( name ) VMF_FIELD_REAL_( name, true )

#define VMF_FIELD_VEC2D_( name, isOptional ) \
    fields.emplace_back( vmf::FieldDesc( name, vmf::Variant::type_vector2d, isOptional ));
#define VMF_FIELD_VEC2D( name ) VMF_FIELD_VEC2D_( name, false )
#define VMF_FIELD_VEC2D_OPT( name ) VMF_FIELD_VEC2D_( name, true )

#define VMF_FIELD_VEC3D_( name, isOptional ) \
    fields.emplace_back( vmf::FieldDesc( name, vmf::Variant::type_vector3d, isOptional ));
#define VMF_FIELD_VEC3D( name ) VMF_FIELD_VEC3D_( name, false )
#define VMF_FIELD_VEC3D_OPT( name ) VMF_FIELD_VEC3D_( name, true )

#define VMF_FIELD_VEC4D_( name, isOptional ) \
    fields.emplace_back( vmf::FieldDesc( name, vmf::Variant::type_vector4d, isOptional ));
#define VMF_FIELD_VEC4D( name ) VMF_FIELD_VEC4D_( name, false )
#define VMF_FIELD_VEC4D_OPT( name ) VMF_FIELD_VEC4D_( name, true )

#define VMF_FIELD_RAW_( name, isOptional ) \
    fields.emplace_back( vmf::FieldDesc( name, vmf::Variant::type_rawbuffer, isOptional ));
#define VMF_FIELD_RAW( name ) VMF_FIELD_RAW_( name, false )
#define VMF_FIELD_RAW_OPT( name ) VMF_FIELD_RAW_( name, true )

} // namespace vmf

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif /* __VMF_METADATA_SCHEMA_H__ */
