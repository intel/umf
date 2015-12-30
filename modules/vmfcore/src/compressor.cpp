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

#include <stdexcept>
#include <map>
#include "vmf/compressor.hpp"

namespace vmf {

typedef std::map< MetaString, std::shared_ptr<Compressor> > CompressorsMap;

CompressorsMap& getMapInstance()
{
    static CompressorsMap registeredCompressors;
    return registeredCompressors;
}

void Compressor::registerNew(std::shared_ptr<Compressor> compressor)
{
    if(!compressor)
    {
        VMF_EXCEPTION(IncorrectParamException, "Incorrect instance of compressor");
    }

    CompressorsMap& cmap = getMapInstance();
    cmap[compressor->getId()] = compressor;
}


std::shared_ptr<Compressor> Compressor::create(const vmf_string &id)
{
    CompressorsMap& cmap = getMapInstance();
    if(cmap.find(id) != cmap.end())
        return cmap.at(id)->createNewInstance();
    else
        return std::shared_ptr<Compressor>();
}

void Compressor::unregister(const vmf_string &id)
{
    CompressorsMap& cmap = getMapInstance();
    if(cmap.find(id) != cmap.end())
    {
        cmap.erase(id);
    }
    else
    {
        VMF_EXCEPTION(IncorrectParamException,
                      "Unregistered compression algorithm: " + id);
    }
}

} /* vmf */
