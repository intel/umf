#include "vmf/metadatareference.hpp"

namespace vmf
{
Reference::Reference() : desc(NULL) {}

Reference::Reference(std::shared_ptr<ReferenceDesc>& desc, std::weak_ptr<Metadata>& md) : desc(desc), md(md){}

Reference::Reference(std::shared_ptr<ReferenceDesc>& desc, const std::shared_ptr<Metadata>& md) : desc(desc), md(md){}

Reference::~Reference(){}

void Reference::setReferenceMetadata(const std::shared_ptr<Metadata>& spMetadata) { md = spMetadata; }

std::weak_ptr<Metadata> Reference::getReferenceMetadata() const { return md; }

std::shared_ptr<ReferenceDesc> Reference::getReferenceDescription() const { return desc; }
} //namespace vmf