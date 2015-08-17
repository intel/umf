// Copyright (C) 2013, Intel Corporation, all rights reserved.

#ifndef __EVENTS_H__
#define __EVENTS_H__

#include <QEvent>

class FaceRectBase;

class AssociateEvent : public QEvent
{
public:
    AssociateEvent(FaceRectBase *r);
    virtual ~AssociateEvent();

    static QEvent::Type type();

    FaceRectBase* getRectangle();

private:
    static QEvent::Type associateEventType;

    FaceRectBase* rectangle;
};

class DeleteEvent : public QEvent
{
public:
    DeleteEvent(FaceRectBase *r);
    virtual ~DeleteEvent();

    static QEvent::Type type();

    FaceRectBase* getRectangle();

private:
    static QEvent::Type deleteEventType;

    FaceRectBase* rectangle;
};

class ResizeEvent : public QEvent
{
public:
    ResizeEvent(FaceRectBase *r);
    virtual ~ResizeEvent();

    static QEvent::Type type();

    FaceRectBase* getRectangle();

private:
    static QEvent::Type resizeEventType;

    FaceRectBase* rectangle;
};

class UnassociateEvent : public QEvent
{
public:
    UnassociateEvent(FaceRectBase *r);
    virtual ~UnassociateEvent();

    static QEvent::Type type();

    FaceRectBase* getRectangle();

private:
    static QEvent::Type unassociateEventType;

    FaceRectBase* rectangle;
};

#endif /* __EVENTS_H__ */
