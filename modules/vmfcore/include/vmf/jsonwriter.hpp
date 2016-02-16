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
    virtual ~JSONWriter();

    // IWriter implementation
    virtual std::string store(const std::vector<std::shared_ptr<MetadataSchema>>& schemas);
    virtual std::string store(const MetadataSet& set);
    virtual std::string store(const IdType& nextId,
                              const std::string& filepath,
                              const std::string& checksum,
                              const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
                              const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
                              const MetadataSet& set,
                              const std::vector< Stat >& stats);
    virtual std::string store(const std::shared_ptr<MetadataStream::VideoSegment>& spSegment);
    virtual std::string store(const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments);
    virtual std::string store(const Stat& stat);
    virtual std::string store(const std::vector< Stat >& stats);

private:
    // hiding API that may be removed soon
    virtual std::string store(const std::shared_ptr<MetadataSchema>& spSchema);
    virtual std::string store(const std::shared_ptr<Metadata>& spMetadata);
};

}//vmf

#endif //__VMF_JSONWRITER_H__
