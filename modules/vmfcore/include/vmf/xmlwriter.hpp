// Copyright (C) 2014, Intel Corporation, all rights reserved.

/*!
* \file xmlwriter.hpp
* \brief %XMLWriter class header file
*/

#ifndef __VMF_XMLWRITER_H__
#define __VMF_XMLWRITER_H__

#include "metadataset.hpp"
#include "metadataschema.hpp"
#include "iwriter.hpp"

namespace vmf
{
/*!
* class XMLWriter
* \brief XMLWriter class is a %IWriter interface implementation for XML format representation
*/
class VMF_EXPORT XMLWriter : public IWriter
{
public:
    /*!
    * \brief Default class constructor
    */
    XMLWriter();

    /*!
    * \brief Class destructor
    */
    ~XMLWriter();

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

#endif //__VMF_XMLWRITER_H__
