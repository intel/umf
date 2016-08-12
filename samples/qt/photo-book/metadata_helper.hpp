// Copyright (C) 2013, Intel Corporation, all rights reserved.

#ifndef __METADATA_HELPER_H__
#define __METADATA_HELPER_H__

#include "vmf/vmf.hpp"

class MetadataHelper
{
public:
    static const std::string PHOTO_BOOK_SCHEMA_NAME;
    static const std::string PEOPLE_DESC_NAME;
    static const std::string MARKUP_DESC_NAME;

public:
    bool loadMetaFile(const std::string& fileName);
    void closeFile();
    bool openFile(umf::MetadataStream::OpenMode mode);
    void saveMetadata();
    umf::MetadataStream *getMetadataStream();

private:
    void loadRequiredProperties();

private:
    umf::MetadataStream metaStream;
    umf::MetadataSet infoProperty;
    umf::MetadataSet markupProperty;
};

#endif /* __METADATA_HELPER_H__ */
