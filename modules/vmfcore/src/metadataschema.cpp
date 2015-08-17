#include "vmf/metadataschema.hpp"
#include <algorithm>

namespace vmf
{
MetadataSchema::MetadataSchema( const std::string& sName, const std::string& sAuthor )
    : m_sName(sName), m_sAuthor(sAuthor)
{
    if (sName.empty())
    {
        VMF_EXCEPTION(IncorrectParamException, "Schema name can't be empty.");
    }
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
