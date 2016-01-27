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
* \file xmlwriter.hpp
* \brief %XMLWriter class header file
*/

#ifndef __VMF_XMLWRITER_H__
#define __VMF_XMLWRITER_H__

#include "metadataset.hpp"
#include "metadataschema.hpp"
#include "iwriter.hpp"
#include "writer_base.hpp"

namespace vmf
{
/*!
* class XMLWriter
* \brief XMLWriter class is a %IWriter interface implementation for XML format representation
*/
class VMF_EXPORT XMLWriter : public WriterBase
{
public:
    /*!
    * \brief Default class constructor
    */
    XMLWriter(const vmf_string& _compressorId = vmf_string());

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

    /*!
     * \brief Creates a new instance of the class
     * \return Smart pointer to the new instance
     */
    virtual std::shared_ptr<WriterBase> createNewInstance() const
    {
        return std::shared_ptr<WriterBase>(new XMLWriter);
    }

private:
    // hiding API that may be removed soon
    virtual std::string store(const std::shared_ptr<MetadataSchema>& spSchema);
    virtual std::string store(const std::shared_ptr<Metadata>& spMetadata);
};

}//vmf

#endif //__VMF_XMLWRITER_H__
