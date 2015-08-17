// Copyright (C) 2014, Intel Corporation, all rights reserved.

/*!
* \file jsonwriter.hpp
* \brief %JSONWriter class header file
*/

#ifndef __VMF_JSONWRITER_H__
#define __VMF_JSONWRITER_H__

#include "metadataset.hpp"
#include "metadataschema.hpp"
#include "iwriter.hpp"

namespace vmf
{
/*!
* class JSONWriter
* \brief JSONWriter class is a %IWriter interface implementation for JSON format representation
*/
class VMF_EXPORT JSONWriter : public IWriter
{
public:
    /*!
    * \brief Default class constructor
    */
    JSONWriter();

    /*!
    * \brief Class destructor
    */
    ~JSONWriter();

    // IWriter implementation
    virtual std::string store(const std::vector<std::shared_ptr<MetadataSchema>>& schemas);
    virtual std::string store(const MetadataSet& set);
    virtual std::string store(const IdType& nextId,
                              const std::string& filepath,
                              const std::string& checksum,
			      const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
			      const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                              const MetadataSet& set);
    virtual std::string store(const std::shared_ptr<MetadataStream::VideoSegment>& spSegment);
    virtual std::string store(const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments);

private:
    // hiding API that may be removed soon
    virtual std::string store(const std::shared_ptr<MetadataSchema>& spSchema);
    virtual std::string store(const std::shared_ptr<Metadata>& spMetadata);

};

}//vmf

#endif //__VMF_JSONWRITER_H__
