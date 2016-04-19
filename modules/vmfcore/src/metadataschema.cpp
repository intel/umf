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
#include "vmf/metadataschema.hpp"
#include <algorithm>

namespace vmf
{
MetadataSchema::MetadataSchema(const std::string& sName, const std::string& sAuthor, bool useEncryption)
    : m_sName(sName), m_sAuthor(sAuthor), m_useEncryption(useEncryption)
{
    if (sName.empty())
    {
        VMF_EXCEPTION(IncorrectParamException, "Schema name can't be empty.");
    }
}

MetadataSchema::MetadataSchema(const std::string &sName, bool isEncrypted)
    : MetadataSchema(sName, "", isEncrypted)
{

}

MetadataSchema::~MetadataSchema(void)
{
}

std::string MetadataSchema::getName() const
{
    return m_sName;
}

std::string MetadataSchema::getAuthor() const
{
    return m_sAuthor;
}

void MetadataSchema::setUseEncryption(bool useEncryption)
{
    m_useEncryption = useEncryption;
}

bool MetadataSchema::getUseEncryption() const
{
    return m_useEncryption;
}

size_t MetadataSchema::size() const
{
    return std::vector< std::shared_ptr< MetadataDesc >>::size();
}

void MetadataSchema::add( std::shared_ptr< MetadataDesc >& spDesc )
{
    if( spDesc == nullptr )
    {
        VMF_EXCEPTION(NullPointerException, "Description pointer is empty!" );
    }

    if( this->findMetadataDesc( spDesc->getMetadataName() ) != nullptr )
    {
        VMF_EXCEPTION(IncorrectParamException, "Metadata with same name already exists!" );
    }

    spDesc->validate();

    spDesc->setSchemaName( this->m_sName );
    this->push_back( spDesc );
}

const std::shared_ptr< MetadataDesc > MetadataSchema::findMetadataDesc( const std::string& sMetadataName ) const
{
    auto it = std::find_if( this->begin(), this->end(), [&]( const std::shared_ptr< MetadataDesc >& spItem )->bool
    {
        return spItem->getMetadataName() == sMetadataName;
    });

    if( it != this->end() )
        return *it;

    return nullptr;
}

std::vector<std::shared_ptr< MetadataDesc >> MetadataSchema::getAll() const
{
    std::vector<std::shared_ptr< MetadataDesc >> set;
    std::for_each( this->begin(), this->end(), [&]( const std::shared_ptr< MetadataDesc >& spItem )
    {
        set.push_back(spItem);
    });

    return set;
}

} // namespace vmf
