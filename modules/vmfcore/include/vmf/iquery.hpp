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
* \file iquery.hpp
* \brief %IQuery interface declaration
* \details The file contains declaration of %IQuery interface.
*/

#ifndef __VMF_IQUERY_H__
#define __VMF_IQUERY_H__

#include <string>
#include <vector>
#include <functional>

#include "metadata.hpp"
#include "global.hpp"

namespace vmf
{
class MetadataSet;

/*!
    * \class IQuery
    * \brief %IQuery is an interface for build different queries on metadata.
    */
class VMF_EXPORT IQuery
{
public:
    /*!
    * \brief Default constructor
    */
    IQuery(void) {};

    /*!
    * \brief Class destructor
    */
    virtual ~IQuery(void){};

    /*!
    * \brief Make metadata query by specified filter
    * \param filter [in] filter function
    * \return filtered set of metadata
    */
    virtual MetadataSet query( std::function< bool( const std::shared_ptr<Metadata>& spMetadata )> filter ) const = 0;

    /*!
    * \brief Make metadata query by reference
    * \param filter [in] filter function
    * \return filtered set of metadata
    */
    virtual MetadataSet queryByReference( std::function< bool( const std::shared_ptr<Metadata>& spMetadata, const std::shared_ptr<Metadata>& spReference )> filter ) const = 0;

    /*!
    * \brief Make metadata query by frame index
    * \param index [in] index of frame
    * \return filtered set of metadata
    */
    virtual MetadataSet queryByFrameIndex( size_t index ) const = 0;

    /*!
    * \brief Make metadata query by schema name
    * \param sSchemaName [in] name of schema
    * \return filtered set of metadata
    */
    virtual MetadataSet queryBySchema( const std::string& sSchemaName ) const = 0;

    /*!
    * \brief Make metadata query by metadata name
    * \param sName [in] name of metadata
    * \return filtered set of metadata
    */
    virtual MetadataSet queryByName( const std::string& sName ) const = 0;

    /*!
    * \brief Make metadata query by metadata item name and value
    * \param sMetadataName [in] name of metadata item
    * \param value [in] metadata value
    * \return filtered set of metadata
    */
    virtual MetadataSet queryByNameAndValue( const std::string& sMetadataName, const vmf::FieldValue& value ) const = 0;

    /*!
    * \brief Make metadata query by metadata item name and multiple values
    * \param sMetadataName [in] name of metadata item
    * \param vFields [in] metadata values
    * \return filtered set of metadata
    */
    virtual MetadataSet queryByNameAndFields( const std::string& sMetadataName, const std::vector< vmf::FieldValue>& vFields ) const = 0;

    /*!
    * \brief Make metadata query by reference name
    * \param sReferenceName [in] name of reference
    * \return filtered set of metadata
    */
    virtual MetadataSet queryByReference( const std::string& sReferenceName ) const = 0;

    /*!
    * \brief Make metadata query by reference name and metadata item value
    * \param sReferenceName [in] name of reference
    * \param value [in] metadata value
    * \return filtered set of metadata
    */
    virtual MetadataSet queryByReference( const std::string& sReferenceName, const vmf::FieldValue& value ) const = 0;

    /*!
    * \brief Make metadata query by reference name and multiple metadata item values
    * \param sReferenceName [in] name of reference
    * \param vFields [in] possible metadata values
    * \return filtered set of metadata
    */
    virtual MetadataSet queryByReference( const std::string& sReferenceName, const std::vector< vmf::FieldValue>& vFields ) const = 0;
};
}

#endif /* __VMF_IQUERY_H__ */
