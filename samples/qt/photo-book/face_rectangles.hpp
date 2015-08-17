// Copyright (C) 2013, Intel Corporation, all rights reserved.

#ifndef __FACE_RECTANGLES_H__
#define __FACE_RECTANGLES_H__

#include <QGraphicsRectItem>
#include <QPen>
#include <vector>

#include "corner_grabber.hpp"
#include "photo_book.hpp"

class FaceRectBase : public QGraphicsRectItem
{
public:
    FaceRectBase(const FaceRect& r, 
        QGraphicsItem * parent = 0);

    virtual int type() const;

    void setMoveArea(const QRectF& ma);
    FaceRect getModelRect();

    static const int MIN_REGION_WIDTH;
    static const int MIN_REGION_HEIGHT;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *me);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    bool sceneEventFilter(QGraphicsItem* watched, QEvent* me);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    void setCornerPositions();
    bool processResize(CornerGrabber* corner, QGraphicsSceneMouseEvent* mevent);
    void notifyResize();
    bool processSceneEvent(QGraphicsItem* watched, QEvent* me);

private:
    std::vector<CornerGrabber*> rectCorners;

    qreal mouseDownX;
    qreal mouseDownY;
    bool boldPen;
    QRectF moveArea;
    FaceRect modelRect;
};

class UnAssociatedFaceRect : public FaceRectBase
{
private:
    enum ContextMenuActions
    {
        deleteRegionAction
    };

public:
    UnAssociatedFaceRect(const FaceRect& r, 
        QGraphicsItem * parent = 0);

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *me);
};

class AssociatedFaceRect : public FaceRectBase
{
private:
    enum ContextMenuActions
    {
        deleteRegionAction,
        removeAssociationAction
    };

public:
    AssociatedFaceRect(const FaceRect& r, 
        QGraphicsItem * parent = 0);

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *me);
};

#endif /* __FACE_RECTANGLES_H__ */
