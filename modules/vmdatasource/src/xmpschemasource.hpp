// Copyright (C) 2013, Intel Corporation, all rights reserved.
#ifndef __XMPSCHEMASOURCE_HPP__
#define __XMPSCHEMASOURCE_HPP__

#include "xmpdatasource.hpp"

namespace vmf {

class XMPSchemaSource
{
public:

    explicit XMPSchemaSource(const std::shared_ptr<SXMPMeta>& schemaStorage);

    void save(const std::shared_ptr<vmf::MetadataSchema>& schema);

    void load(std::map< vmf::MetaString, std::shared_ptr<vmf::MetadataSchema> >& schemas);

    virtual void clear();

    virtual void remove(const MetaString &schemaName);

    virtual ~XMPSchemaSource();

protected:

    std::shared_ptr<vmf::MetadataSchema> loadMetadataSchemaByPath(const vmf::MetaString& pathToSchema);

    void saveDescription(const MetadataDesc& desc, const MetaString& pathToDescriptions);

    void saveField(const FieldDesc& desc, const MetaString& pathToFields);

    void saveReference(const std::shared_ptr<ReferenceDesc> ref, const MetaString& pathToRefs);

    std::shared_ptr<MetadataDesc> loadDescription(const MetaString& pathToDesc);

private:
    XMPSchemaSource();
    XMPSchemaSource(const XMPSchemaSource& origin);
    XMPSchemaSource& operator=(const XMPSchemaSource& origin);

    std::shared_ptr<SXMPMeta> metadata;

};

} // namespace vmf

#endif // __XMPSCHEMASOURCE_HPP__
