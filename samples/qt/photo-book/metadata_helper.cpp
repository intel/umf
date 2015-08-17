// Copyright (C) 2013, Intel Corporation, all rights reserved.

#include <QDebug>
#include "metadata_helper.hpp"
#include "photo_book.hpp"

const std::string MetadataHelper::PHOTO_BOOK_SCHEMA_NAME = "photo_book";
const std::string MetadataHelper::PEOPLE_DESC_NAME = "people_info";
const std::string MetadataHelper::MARKUP_DESC_NAME = "markup";

bool MetadataHelper::loadMetaFile(const std::string& fileName)
{
    bool result = metaStream.open(fileName);
    if (result)
    {
        loadRequiredProperties();
    }

    return result;
}

void MetadataHelper::loadRequiredProperties()
{
    if (!metaStream.load(PHOTO_BOOK_SCHEMA_NAME))
    {
        std::shared_ptr<vmf::MetadataSchema> schema(new vmf::MetadataSchema(PHOTO_BOOK_SCHEMA_NAME));

        std::vector<vmf::FieldDesc> peopleFields;
        peopleFields.push_back(vmf::FieldDesc(FieldNameConstants::NAME_FIELD, vmf::Variant::type_string));
        peopleFields.push_back(vmf::FieldDesc(FieldNameConstants::AGE_FIELD, vmf::Variant::type_integer));
        peopleFields.push_back(vmf::FieldDesc(FieldNameConstants::HEIGHT_FIELD, vmf::Variant::type_integer));
        peopleFields.push_back(vmf::FieldDesc(FieldNameConstants::WEIGHT_FIELD, vmf::Variant::type_integer));
        peopleFields.push_back(vmf::FieldDesc(FieldNameConstants::SEX_FIELD, vmf::Variant::type_integer));

        std::shared_ptr<vmf::MetadataDesc> peopleDescription(new vmf::MetadataDesc(PEOPLE_DESC_NAME,
            peopleFields));
        schema->add(peopleDescription);

        std::vector<vmf::FieldDesc> markupFields;
        markupFields.push_back(vmf::FieldDesc(FieldNameConstants::X_RECT_FIELD, vmf::Variant::type_real));
        markupFields.push_back(vmf::FieldDesc(FieldNameConstants::Y_RECT_FIELD, vmf::Variant::type_real));
        markupFields.push_back(vmf::FieldDesc(FieldNameConstants::WIDTH_RECT_FIELD, vmf::Variant::type_real));
        markupFields.push_back(vmf::FieldDesc(FieldNameConstants::HEIGHT_RECT_FIELD, vmf::Variant::type_real));
        markupFields.push_back(vmf::FieldDesc(FieldNameConstants::TYPE_RECT_FIELD, vmf::Variant::type_integer));

        std::shared_ptr<vmf::MetadataDesc> markupDescription(new vmf::MetadataDesc(MARKUP_DESC_NAME,
            markupFields));
        schema->add(markupDescription);

        metaStream.addSchema(schema);
    }
}

void MetadataHelper::closeFile()
{
    metaStream.close();
}

bool MetadataHelper::openFile(vmf::MetadataStream::OpenMode mode)
{
    return metaStream.reopen(mode);
}

void MetadataHelper::saveMetadata()
{
    metaStream.save();
}

vmf::MetadataStream *MetadataHelper::getMetadataStream()
{
    return &metaStream;
}
