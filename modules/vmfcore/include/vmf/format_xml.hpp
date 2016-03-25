/*
* Copyright 2016 Intel(r) Corporation
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
* \file format_xml.hpp
* \brief %FormatXML class header file
*/

#ifndef VMF_FORMAT_XML_H
#define VMF_FORMAT_XML_H

#include "vmf/format_json.hpp"

namespace vmf
{
/*!
* class FormatXML
* \brief FormatXML class is a %Format interface implementation for XML format representation
*/
class VMF_EXPORT FormatXML : public Format
{
public:
    /*!
    * \brief Default class constructor
    */
    FormatXML();

    /*!
    * \brief Class destructor
    */
    virtual ~FormatXML();

    /*!
    * \brief Sserialize input metadata and related stuff to XML string.
    */
    virtual std::string store(
        const MetadataSet& set,
        const std::vector<std::shared_ptr<MetadataSchema>>& schemas = {},
        const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments = {},
        //const std::vector<Stat>& stats = {},
		const AttribMap& attribs = AttribMap() // nextId, checksum, etc
        );

    /*!
    * \brief Deserialize input XML string to metadata and related stuff.
    */
    virtual ParseCounters parse(
        const std::string& text,
        std::vector<std::shared_ptr<MetadataInternal>>& metadata,
        std::vector<std::shared_ptr<MetadataSchema>>& schemas,
        std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments,
        //std::vector<Stat>& stats,
        AttribMap& attribs // nextId, checksum, etc
        );
};

}//vmf

#endif //VMF_FORMAT_XML_H
