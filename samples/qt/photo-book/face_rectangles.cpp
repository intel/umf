// Copyright (C) 2013, Intel Corporation, all rights reserved.

#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QCursor>
#include <QApplication>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QMenu>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <algorithm>
#include <cassert>

#include "face_rectangles.hpp"
#include "photo_book.hpp"
#include "events.hpp"

#define DEFAULT_PEN_WIDTH    1
#define BOLD_PEN_WIDTH       3

const int FaceRectBase::MIN_REGION_WIDTH = 40;
const int FaceRectBase::MIN_REGION_HEIGHT = 40;

FaceRectBase::FaceRectBase(const FaceRect& r, 
    QGraphicsItem * parent)
    : QGraphicsRectItem(r.r.x, r.r.y, r.r.width, r.r.height, parent)
    , rectCorners(4)
    , boldPen(false)
    , modelRect(r)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable );

    setAcceptHoverEvents(true);
}

int FaceRectBase::type() const
{
    return photo::FaceRectBaseType;
}

void FaceRectBase::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    std::for_each(rectCorners.begin(), rectCorners.end(), [this](CornerGrabber* pItem)
    {
        pItem->setParentItem(NULL);
        delete pItem;
    });
}

void FaceRectBase::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
    for (int i = 0; i < (int) rectCorners.size(); i++)
    {
        rectCorners[i] = new CornerGrabber(this, i);
        rectCorners[i]->installSceneEventFilter(this);
    }

    setCornerPositions();
}

void FaceRectBase::setCornerPositions()
{
    assert(rectCorners.size() == 4);

    rectCorners[0]->setPos(rect().x(), rect().y() );
    rectCorners[1]->setPos(rect().x() + rect().width() - CornerGrabber::CORNER_WIDTH, 
        rect().y());
    rectCorners[2]->setPos(rect().x() + rect().width() - CornerGrabber::CORNER_WIDTH, 
        rect().y() + rect().height() - CornerGrabber::CORNER_HEIGHT);
    rectCorners[3]->setPos(rect().x(), 
        rect().y() + rect().height() - CornerGrabber::CORNER_HEIGHT);
}

bool FaceRectBase::sceneEventFilter(QGraphicsItem* watched, QEvent* me)
{
    if (processSceneEvent(watched, me))
    {
        return true;
    }

    return QGraphicsRectItem::sceneEventFilter(watched, me);
}

bool FaceRectBase::processSceneEvent(QGraphicsItem* watched, QEvent* me)
{
    QGraphicsSceneMouseEvent* mevent = dynamic_cast<QGraphicsSceneMouseEvent*>(me);
    if ( mevent == NULL)
    {
        return false;
    }

    CornerGrabber* corner = dynamic_cast<CornerGrabber *>(watched);
    if ( corner == NULL) 
    {
        return false;
    }

    return processResize(corner, mevent);
}

bool FaceRectBase::processResize(CornerGrabber* corner, QGraphicsSceneMouseEvent* mevent)
{
    switch (mevent->type())
    {
    case QEvent::GraphicsSceneMousePress:
        corner->setMouseState(CornerGrabber::kMouseDown);
        corner->mouseDownX = mevent->pos().x();
        corner->mouseDownY = mevent->pos().y();
        break;
    case QEvent::GraphicsSceneMouseRelease:
        corner->setMouseState(CornerGrabber::kMouseReleased);
        break;
    case QEvent::GraphicsSceneMouseMove:
        corner->setMouseState(CornerGrabber::kMouseMoving );
        break;
    default:
        return false;
    }

    if ( corner->getMouseState() == CornerGrabber::kMouseMoving )
    {
        qreal x = mevent->pos().x();
        qreal y = mevent->pos().y();
        int xMoved = corner->mouseDownX - x;
        int yMoved = corner->mouseDownY - y;

        int XaxisSign = 0;
        int YaxisSign = 0;

        switch (corner->getCorner())
        {
        case 0:
            XaxisSign = 1;
            YaxisSign = 1;
            break;
        case 1:
            XaxisSign = -1;
            YaxisSign = 1;
            break;
        case 2:
            XaxisSign = -1;
            YaxisSign = -1;
            break;
        case 3:
            XaxisSign = 1;
            YaxisSign = -1;
            break;
        }

        int newWidth = rect().width() + XaxisSign * xMoved;

        if (newWidth < MIN_REGION_WIDTH)
        {
            newWidth = MIN_REGION_WIDTH;
        }

        int newHeight = rect().height() + YaxisSign * yMoved;

        if (newHeight < MIN_REGION_HEIGHT)
        {
            newHeight = MIN_REGION_HEIGHT;
        }

        int deltaWidth = newWidth - rect().width();
        int deltaHeight = newHeight - rect().height();

        QRectF newR = rect();
        newR.setWidth(newWidth);
        newR.setHeight(newHeight);
        setRect(newR);

        switch (corner->getCorner())
        {
        case 0:
            setPos(pos().x() - deltaWidth,
                pos().y() - deltaHeight);
            break;
        case 1:
            setPos(pos().x(), pos().y() - deltaHeight);
            break;
        case 2:
            setPos(pos().x(), pos().y());
            break;
        case 3:
            setPos(pos().x() - deltaWidth, pos().y());
            break;
        }

        notifyResize();

        update();
        setCornerPositions();
    }

    return true;
}

void FaceRectBase::notifyResize()
{
    QGraphicsView *v = scene()->views().first();
    if (v != NULL)
    {
        QPolygonF f = mapToScene(rect());

        modelRect.r.x = f[0].x();
        modelRect.r.y = f[0].y();
        modelRect.r.width = f[1].x() - f[0].x();
        modelRect.r.height = f[3].y() - f[0].y();
            
        QApplication::postEvent(v, new ResizeEvent(this));
    }
}

void FaceRectBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QStyleOptionGraphicsItem myOption(*option);

    QPen p = pen();
    if ((option->state & QStyle::State_Selected) && !boldPen)
    {
        p.setWidth(BOLD_PEN_WIDTH);
        setPen(p);
        boldPen = true;
    }
    else if (!(option->state & QStyle::State_Selected) && boldPen)
    {
        p.setWidth(DEFAULT_PEN_WIDTH);
        setPen(p);
        boldPen = false;
    }

    myOption.state &= ~QStyle::State_Selected;
    QGraphicsRectItem::paint(painter, &myOption, widget);
}

void FaceRectBase::setMoveArea(const QRectF& ma)
{
    moveArea = ma;
}

FaceRect FaceRectBase::getModelRect()
{
    return modelRect;
}

void FaceRectBase::mouseMoveEvent(QGraphicsSceneMouseEvent *me)
{
    QGraphicsRectItem::mouseMoveEvent(me);
    notifyResize();
}

UnAssociatedFaceRect::UnAssociatedFaceRect(const FaceRect& r, 
    QGraphicsItem * parent)
    : FaceRectBase(r, parent)
{
    QPen rectPen(Qt::green);
    rectPen.setWidth(1);

    setPen(rectPen);
}

void UnAssociatedFaceRect::contextMenuEvent(QGraphicsSceneContextMenuEvent *me)
{
    QMenu regionMenu;
    QAction* deleteAction = regionMenu.addAction("Delete");
    deleteAction->setData(deleteRegionAction);

    QPointF scenePos = mapToScene(me->pos());
    QGraphicsView *v = scene()->views().first();
    QPoint tmp = v->mapFromScene(scenePos);
    QPoint globalPos = v->mapToGlobal(tmp);

    setSelected(true);

    QAction* selectedItem = regionMenu.exec(globalPos);
    if (selectedItem)
    {
        ContextMenuActions actionType = (ContextMenuActions) selectedItem->data().toInt();

        switch (actionType)
        {
        case deleteRegionAction:
            QCoreApplication::postEvent(v, new DeleteEvent(this));
            break;
        }

        me->accept();
    }
}

AssociatedFaceRect::AssociatedFaceRect(const FaceRect& r, 
    QGraphicsItem * parent)
    : FaceRectBase(r, parent)
{
    QPen rectPen(Qt::blue);
    rectPen.setWidth(1);

    setPen(rectPen);
}

void AssociatedFaceRect::contextMenuEvent(QGraphicsSceneContextMenuEvent *me)
{
    QMenu regionMenu;
    QAction* deleteAction = regionMenu.addAction("Delete");
    deleteAction->setData(deleteRegionAction);
    QAction* removeAssocAction = regionMenu.addAction("Remove association");
    removeAssocAction->setData(removeAssociationAction);

    QPointF scenePos = mapToScene(me->pos());
    QGraphicsView *v = scene()->views().first();
    QPoint tmp = v->mapFromScene(scenePos);
    QPoint globalPos = v->mapToGlobal(tmp);

    setSelected(true);

    QAction* selectedItem = regionMenu.exec(globalPos);
    if (selectedItem)
    {
        ContextMenuActions actionType = (ContextMenuActions) selectedItem->data().toInt();

        switch (actionType)
        {
        case deleteRegionAction:
            QCoreApplication::postEvent(v, new DeleteEvent(this));
            break;
        case removeAssociationAction:
            QCoreApplication::postEvent(v, new UnassociateEvent(this));
            break;
        }

        me->accept();
    }
}
