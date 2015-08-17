// Copyright (C) 2014, Intel Corporation, all rights reserved.

/*!
* \file ireader.hpp
* \brief %IReader interface declaration
* \details The file contains declaration of %IReader interface.
*/

#pragma once

#ifndef __VMF_IREADER_H__
#define __VMF_IREADER_H__

#include "metadatainternal.hpp"
#include "metadatastream.hpp"

namespace vmf
{
/*!
* \class IReader
* \brief %IReader is an interface for metadata and schemas deserialization from various formats.
*/
class VMF_EXPORT IReader
{
public:
    /*!
    * \brief Deserialize input string to stream metadata and related stuff. Used for "stream.deserialize(text)" implementation.
    */
    virtual bool parseAll(
        const std::string& text,
        IdType& nextId,
        std::string& filepath,
        std::string& checksum,
	std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
        std::vector<std::shared_ptr<MetadataSchema>>& schemas,
        std::vector<std::shared_ptr<MetadataInternal>>& metadata ) = 0;

    /*!
    * \brief Deserialize input string to a schemas vector
    */
    virtual bool parseSchemas(const std::string& text, std::vector<std::shared_ptr<MetadataSchema>>& schemas) = 0;

    /*!
    * \brief Deserialize input string to a metadata vector
    */
    virtual bool parseMetadata(
        const std::string& text,
        const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
        std::vector<std::shared_ptr<MetadataInternal>>& metadata ) = 0;

    virtual bool parseVideoSegments(const std::string& text, std::vector<std::shared_ptr<MetadataStream::VideoSegment> >& segments) = 0;
};

}//vmf

#endif /* __VMF_IREADER_H__ */
