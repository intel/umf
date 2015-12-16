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
* \file xmlreader.hpp
* \brief %XMLReader class header file
*/

#ifndef __VMF_XMLREADER_H__
#define __VMF_XMLREADER_H__

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

#include "metadata.hpp"
#include "metadataset.hpp"
#include "metadataschema.hpp"
#include "metadatastream.hpp"
#include "ireader.hpp"

namespace vmf
{
/*!
* class XMLReader
* \brief XMLReader class is a %IReader interface implementation for XML format representation
*/
class VMF_EXPORT XMLReader : public IReader
{
public:
    /*!
    * \brief Default class constructor
    */
    XMLReader(std::shared_ptr<Compressor> impl = nullptr);

    /*!
    * \brief Class destructor
    */
    ~XMLReader();

    // IReader implementation
    virtual bool parseAll(const std::string& text, IdType& nextId, std::string& filepath, std::string& checksum,
	std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
        std::vector<std::shared_ptr<MetadataSchema>>& schemas,
        std::vector<std::shared_ptr<MetadataInternal>>& metadata);

    virtual bool parseSchemas(const std::string& text, std::vector<std::shared_ptr<MetadataSchema>>& schemas);

    virtual bool parseMetadata(
        const std::string& text,
        const std::vector<std::shared_ptr<MetadataSchema>>& schemas,
        std::vector<std::shared_ptr<MetadataInternal>>& metadata );

    virtual bool parseVideoSegments(const std::string& text, std::vector<std::shared_ptr<MetadataStream::VideoSegment> >& segments);
};

}//vmf

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif //__VMF_XMLREADER_H__
