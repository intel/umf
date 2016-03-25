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
* \file format_json.hpp
* \brief %FormatJSON class header file
*/

#ifndef VMF_FORMAT_JSON_H
#define VMF_FORMAT_JSON_H

#include "format.hpp"

namespace vmf
{
/*!
* class FormatJSON
* \brief FormatJSON class is a %Format interface implementation for JSON format representation
*/
class VMF_EXPORT FormatJSON : public Format
{
public:
    /*!
    * \brief Default class constructor
    */
    FormatJSON();

    /*!
    * \brief Class destructor
    */
    virtual ~FormatJSON();

    /*!
    * \brief Export various metadata stream items to a JSON string representation.
    */
    virtual std::string store(
        const MetadataSet& set,
        const std::vector<std::shared_ptr<MetadataSchema>>& schemas = {},
        const std::vector<std::shared_ptr<MetadataStream::VideoSegment>>& segments = {},
        //const std::vector<Stat>& stats = {},
		const AttribMap& attribs = AttribMap() // nextId, checksum, etc
        );

    /*!
    * \brief Deserialize input string to stream metadata and related stuff from JSON string.
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

#endif //VMF_FORMAT_JSON_H
