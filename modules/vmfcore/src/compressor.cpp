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
#include "vmf/compressor.hpp"
#include "vmf/compressor_dummy.hpp"
#include "vmf/compressor_zlib.hpp"

namespace vmf {

typedef std::map< vmf_string, std::shared_ptr<Compressor> > CompressorsMap;
enum CompressorType {USER=0, BUILTIN=1};

CompressorsMap& getMapInstance(CompressorType type)
{
    //do that to prevent user from unregistering standard compressors
    static CompressorsMap compressors[2];
    if(compressors[BUILTIN].empty())
    {
        //register standard compressors
        static std::shared_ptr<Compressor> dummy(std::make_shared<CompressorDummy>());
        static std::shared_ptr<Compressor>  zlib(std::make_shared<CompressorZlib>());
        compressors[BUILTIN][dummy->getId()] = dummy;
        compressors[BUILTIN][ zlib->getId()] = zlib;
    }
    return compressors[type];
}


std::vector<vmf_string> Compressor::getRegisteredIds()
{
    std::vector<vmf_string> result;
    for(CompressorType type: {USER, BUILTIN})
    {
        for(const auto& c : getMapInstance(type))
        {
            result.push_back(c.first);
        }
    }
    return result;
}


void Compressor::registerNew(std::shared_ptr<Compressor> compressor)
{
    if(!compressor)
    {
        VMF_EXCEPTION(IncorrectParamException, "Incorrect instance of compressor");
    }

    for(CompressorType type: {USER, BUILTIN})
    {
        auto& m = getMapInstance(type);
        if(m.find(compressor->getId()) != m.end())
        {
            VMF_EXCEPTION(IncorrectParamException, "Compressor with that ID is already registered");
        }
    }

    CompressorsMap& cmap = getMapInstance(USER);
    cmap[compressor->getId()] = compressor;
}


std::shared_ptr<Compressor> Compressor::create(const vmf_string &id)
{
    std::shared_ptr<Compressor> current;
    for(CompressorType type: {USER, BUILTIN})
    {
        auto& m = getMapInstance(type);
        if(m.find(id) != m.end())
        {
            current = m.at(id);
        }
    }

    if(current)
    {
        return current->createNewInstance();
    }
    else
    {
        VMF_EXCEPTION(IncorrectParamException,
                      "Unregistered compression algorithm: " + id);
    }
}


void Compressor::unregister(const vmf_string &id)
{
    CompressorsMap& cmap = getMapInstance(USER);
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
