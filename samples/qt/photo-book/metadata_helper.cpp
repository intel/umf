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
        std::shared_ptr<umf::MetadataSchema> schema(new umf::MetadataSchema(PHOTO_BOOK_SCHEMA_NAME));

        std::vector<umf::FieldDesc> peopleFields;
        peopleFields.push_back(umf::FieldDesc(FieldNameConstants::NAME_FIELD, umf::Variant::type_string));
        peopleFields.push_back(umf::FieldDesc(FieldNameConstants::AGE_FIELD, umf::Variant::type_integer));
        peopleFields.push_back(umf::FieldDesc(FieldNameConstants::HEIGHT_FIELD, umf::Variant::type_integer));
        peopleFields.push_back(umf::FieldDesc(FieldNameConstants::WEIGHT_FIELD, umf::Variant::type_integer));
        peopleFields.push_back(umf::FieldDesc(FieldNameConstants::SEX_FIELD, umf::Variant::type_integer));

        std::shared_ptr<umf::MetadataDesc> peopleDescription(new umf::MetadataDesc(PEOPLE_DESC_NAME,
            peopleFields));
        schema->add(peopleDescription);

        std::vector<umf::FieldDesc> markupFields;
        markupFields.push_back(umf::FieldDesc(FieldNameConstants::X_RECT_FIELD, umf::Variant::type_real));
        markupFields.push_back(umf::FieldDesc(FieldNameConstants::Y_RECT_FIELD, umf::Variant::type_real));
        markupFields.push_back(umf::FieldDesc(FieldNameConstants::WIDTH_RECT_FIELD, umf::Variant::type_real));
        markupFields.push_back(umf::FieldDesc(FieldNameConstants::HEIGHT_RECT_FIELD, umf::Variant::type_real));
        markupFields.push_back(umf::FieldDesc(FieldNameConstants::TYPE_RECT_FIELD, umf::Variant::type_integer));

        std::shared_ptr<umf::MetadataDesc> markupDescription(new umf::MetadataDesc(MARKUP_DESC_NAME,
            markupFields));
        schema->add(markupDescription);

        metaStream.addSchema(schema);
    }
}

void MetadataHelper::closeFile()
{
    metaStream.close();
}

bool MetadataHelper::openFile(umf::MetadataStream::OpenMode mode)
{
    return metaStream.reopen(mode);
}

void MetadataHelper::saveMetadata()
{
    metaStream.save();
}

umf::MetadataStream *MetadataHelper::getMetadataStream()
{
    return &metaStream;
}
