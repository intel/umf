#include "vmf/metadatainternal.hpp"

namespace vmf
{

MetadataInternal::MetadataInternal( const std::shared_ptr< MetadataDesc >& spDescription )
    : Metadata(spDescription)
{}

MetadataInternal::MetadataInternal( const MetadataInternal& that )
    : Metadata(that)
{
    *this = that;
}

MetadataInternal::MetadataInternal( const Metadata& base )
    : Metadata(base)
{}

MetadataInternal::~MetadataInternal()
{}

}//vmf
