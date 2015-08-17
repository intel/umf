// Copyright (C) 2013, Intel Corporation, all rights reserved.

#include "events.hpp"
#include "face_rectangles.hpp"


QEvent::Type AssociateEvent::associateEventType = QEvent::None;

AssociateEvent::AssociateEvent(FaceRectBase* r) 
    : QEvent(AssociateEvent::type())
    , rectangle(r)
{
}

AssociateEvent::~AssociateEvent()
{
}

QEvent::Type AssociateEvent::type()
{
    if (associateEventType == QEvent::None)
    {
        int generatedType = QEvent::registerEventType();
        associateEventType = static_cast<QEvent::Type>(generatedType);
    }

    return associateEventType;
}

FaceRectBase* AssociateEvent::getRectangle()
{
    return rectangle;
}

QEvent::Type DeleteEvent::deleteEventType = QEvent::None;

DeleteEvent::DeleteEvent(FaceRectBase* r) 
    : QEvent(DeleteEvent::type())
    , rectangle(r)
{
}

DeleteEvent::~DeleteEvent()
{
}

QEvent::Type DeleteEvent::type()
{
    if (deleteEventType == QEvent::None)
    {
        int generatedType = QEvent::registerEventType();
        deleteEventType = static_cast<QEvent::Type>(generatedType);
    }

    return deleteEventType;
}

FaceRectBase* DeleteEvent::getRectangle()
{
    return rectangle;
}

QEvent::Type ResizeEvent::resizeEventType = QEvent::None;

ResizeEvent::ResizeEvent(FaceRectBase* r) 
    : QEvent(ResizeEvent::type())
    , rectangle(r)
{
}

ResizeEvent::~ResizeEvent()
{
}

QEvent::Type ResizeEvent::type()
{
    if (resizeEventType == QEvent::None)
    {
        int generatedType = QEvent::registerEventType();
        resizeEventType = static_cast<QEvent::Type>(generatedType);
    }

    return resizeEventType;
}

FaceRectBase* ResizeEvent::getRectangle()
{
    return rectangle;
}

QEvent::Type UnassociateEvent::unassociateEventType = QEvent::None;

UnassociateEvent::UnassociateEvent(FaceRectBase* r) 
    : QEvent(UnassociateEvent::type())
    , rectangle(r)
{
}

UnassociateEvent::~UnassociateEvent()
{
}

QEvent::Type UnassociateEvent::type()
{
    if (unassociateEventType == QEvent::None)
    {
        int generatedType = QEvent::registerEventType();
        unassociateEventType = static_cast<QEvent::Type>(generatedType);
    }

    return unassociateEventType;
}

FaceRectBase* UnassociateEvent::getRectangle()
{
    return rectangle;
}
