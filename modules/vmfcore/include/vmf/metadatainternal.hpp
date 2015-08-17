// Copyright (C) 2014, Intel Corporation, all rights reserved.

/*!
* \file metadatainternal.hpp
* \brief %MetadataInternal class header file
*/

#ifndef __VMF_METADATAINTERNAL_H__
#define __VMF_METADATAINTERNAL_H__

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

#include "metadata.hpp"

namespace vmf
{
/*!
* class MetadataInternal
* \brief %MetadataInternal is a %Metadata sub-class intended for incremental Metadata items deserialization (streaming case).
* Unlike the %Metadata parent it allows setting internal item ID and keeps unresolved yet references (when the object isn't in a stream).
* Please don't use %MetadataInternal objects unless you develop custom %IReader implementation, use %Metadata instances.
*/
class VMF_EXPORT MetadataInternal : public Metadata
{
public:
    /*!
    * \brief Class constructor
    * \param spDescription [in] metadata item description
    * \throw NullPointerException if metadata description pointer is null
    */
    MetadataInternal( const std::shared_ptr< MetadataDesc >& spDescription );

    /*!
    * \brief Class copy constructor
    * \param that [in] MetadataInternal object
    */
    MetadataInternal( const MetadataInternal& that );

    /*!
    * \brief Class constructor
    * \param base [in] Metadata object
    */
    MetadataInternal( const Metadata& base );

    /*!
    * \brief Class destructor
    */
    virtual ~MetadataInternal();

    /*!
    * \brief Making internal ID setter public
    */
    using Metadata::setId;

    /*!
    * \brief vector of references of the object
    */
    std::vector<std::pair<IdType, std::string>> vRefs;
};

}//vmf

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif //__VMF_METADATAINTERNAL_H__
