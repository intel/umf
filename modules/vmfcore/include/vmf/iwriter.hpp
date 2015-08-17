// Copyright (C) 2014, Intel Corporation, all rights reserved.

/*!
* \file iwriter.hpp
* \brief %IWriter interface declaration
* \details The file contains declaration of %IWriter interface.
*/

#pragma once

#ifndef __VMF_IWRITER_H__
#define __VMF_IWRITER_H__

#include "metadatastream.hpp"

namespace vmf
{
/*!
* \class IWriter
* \brief %IWriter is an interface for metadata and schemas serialization to various formats.
*/
class VMF_EXPORT IWriter
{
public:
    /*!
    * \brief Export a schemas vector to a string representation
    */
    virtual std::string store(const std::vector<std::shared_ptr<MetadataSchema>>& schemas) = 0;

    /*!
    * \brief Export a MetadataSet to a string representation
    */
    virtual std::string store(const MetadataSet& set) = 0;

    /*!
    * \brief Export all the stream metadata including schemas to a string representation. Used for "stream.serialize()" implementation.
    */
    virtual std::string store( const IdType& nextId,
                               const std::string& filepath,
                               const std::string& checksum,
			       const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
			       const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                               const MetadataSet& set ) = 0;

    /*!
    * \brief Export a video segment to a string representation
    */
    virtual std::string store(const std::shared_ptr<MetadataStream::VideoSegment>& spSegment) = 0;

    /*!
    * \brief Export a video segments vector to a string representation.
    */
    virtual std::string store(const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments) = 0;

};

}//vmf

#endif /* __VMF_IWRITER_H__ */
