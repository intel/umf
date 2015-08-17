// Copyright (C) 2013, Intel Corporation, all rights reserved.

#ifndef __PHOTO_BOOK_H__
#define __PHOTO_BOOK_H__

#include <QGraphicsItem>
#include <opencv2/opencv.hpp>
#include "vmf/vmf.hpp"

#define UNASSOCIATED_RECT_TYPE          0
#define ASSOCIATED_RECT_TYPE            1
#define EMPTY_RECT_TYPE                 2

namespace photo
{
enum ItemTypes
{
    FaceRectBaseType = QGraphicsItem::UserType + 1
};
} /* photo */

class FaceRect
{
public:
    FaceRect();

    cv::Rect_<int> r;
    quint64 getIndex() const;

private:
    static quint64 indexCounter;

    quint64 rectIndex;
};

struct FieldNameConstants
{
    static const std::string NAME_FIELD;
    static const std::string SEX_FIELD;
    static const std::string AGE_FIELD;
    static const std::string WEIGHT_FIELD;
    static const std::string HEIGHT_FIELD;

    static const std::string X_RECT_FIELD;
    static const std::string Y_RECT_FIELD;
    static const std::string WIDTH_RECT_FIELD;
    static const std::string HEIGHT_RECT_FIELD;
    static const std::string TYPE_RECT_FIELD;
};

enum PersonSex
{
    male = 0,
    female
};

class Utilities
{
public:
    static QString sexEnum2String(PersonSex ps);
};

#endif /* __PHOTO_BOOK_H__ */
