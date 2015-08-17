// Copyright (C) 2013, Intel Corporation, all rights reserved.

#include "photo_book.hpp"

quint64 FaceRect::indexCounter = 0;

FaceRect::FaceRect()
    : rectIndex(indexCounter++)
{
}

quint64 FaceRect::getIndex() const
{
    return rectIndex;
}

const std::string FieldNameConstants::NAME_FIELD = "name";
const std::string FieldNameConstants::SEX_FIELD = "sex";
const std::string FieldNameConstants::AGE_FIELD = "age";
const std::string FieldNameConstants::WEIGHT_FIELD = "weight";
const std::string FieldNameConstants::HEIGHT_FIELD = "height";

const std::string FieldNameConstants::X_RECT_FIELD = "x";
const std::string FieldNameConstants::Y_RECT_FIELD = "y";
const std::string FieldNameConstants::WIDTH_RECT_FIELD = "width";
const std::string FieldNameConstants::HEIGHT_RECT_FIELD = "height";
const std::string FieldNameConstants::TYPE_RECT_FIELD = "type";

QString Utilities::sexEnum2String(PersonSex ps)
{
    switch (ps)
    {
    case male:
        return "Male";
    case female:
        return "Female";
    }

    return QString();
}
