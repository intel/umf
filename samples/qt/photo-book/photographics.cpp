#include <QMenu>
#include <QContextMenuEvent>

#include "photographics.hpp"
#include "ui_photographics.h"
#include "face_rectangles.hpp"
#include "events.hpp"

PhotoGraphics::PhotoGraphics(QWidget *parent) :
    QGraphicsView(parent),
    ui(new Ui::PhotoGraphics),
    scaleFactor(1.0),
    pixmapItem(NULL),
    frameIndex(0)
{
    ui->setupUi(this);

    installEventFilter(this);
    setContextMenuPolicy(Qt::DefaultContextMenu);
}

void PhotoGraphics::setBackgroundImage(const cv::Mat& img)
{
    scene()->clear();

    int imageWidth = img.cols;
    int imageHeight = img.rows;
    QSize viewSize = viewport()->size();

    double scX = (double) imageWidth / viewSize.width();
    double scY = (double) imageHeight / viewSize.height();

    if (imageHeight / scX <= viewSize.height())
    {
        scaleFactor = scX;
    }
    else
    {
        scaleFactor = scY;
    }

    QImage qimg(img.data, img.cols, img.rows, QImage::Format_RGB888);

    QPixmap pxmp;
    pxmp.convertFromImage(qimg.scaled(img.cols / scaleFactor, img.rows / scaleFactor,
        Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    pixmapItem = new QGraphicsPixmapItem(pxmp);
    auto sc = scene();
    if(sc) sc->addItem(pixmapItem);
    setSceneRect(pixmapItem->boundingRect());
}

void PhotoGraphics::reset()
{
    auto sc = scene();

    clearFaceRectangles();
    sc->clear();
}

void PhotoGraphics::setFaceRectangles(const std::vector<FaceRect>& rectanges)
{
    std::for_each(rectanges.begin(), rectanges.end(),
        [this] (const FaceRect& fr)
    {
        FaceRect scaledRect = scaleFaceRect(fr, 1 / scaleFactor);
        FaceRectBase *pRect = new UnAssociatedFaceRect(scaledRect, pixmapItem);

        pRect->setMoveArea(pixmapItem->boundingRect());
    });
}

void PhotoGraphics::setAssociatedRectangles(const std::vector<FaceRect>& rectanges)
{
    std::for_each(rectanges.begin(), rectanges.end(),
        [this] (const FaceRect& fr)
    {
        FaceRect scaledRect = scaleFaceRect(fr, 1 / scaleFactor);
        FaceRectBase *pRect = new AssociatedFaceRect(scaledRect, pixmapItem);

        pRect->setMoveArea(pixmapItem->boundingRect());
    });
}

void PhotoGraphics::clearFaceRectangles()
{
    auto items = scene()->items();
    std::for_each(items.begin(), items.end(),
        [&items, this] (QGraphicsItem* item)
    {
        FaceRectBase *pRect = dynamic_cast<FaceRectBase *>(item);

        if (pRect)
        {
            pRect->setParentItem(NULL);
            scene()->removeItem(pRect);
        }

        delete pRect;
    });
}

void PhotoGraphics::clearMarkupRectangles()
{
    auto items = scene()->items();
    std::for_each(items.begin(), items.end(),
        [&items, this] (QGraphicsItem* item)
    {
        UnAssociatedFaceRect *pRect = dynamic_cast<UnAssociatedFaceRect *>(item);

        if (pRect)
        {
            emit deleteRectangle(frameIndex, pRect->getModelRect().getIndex());

            pRect->setParentItem(NULL);
            scene()->removeItem(pRect);
        }

        delete pRect;
    });
}

void PhotoGraphics::clearAssociatedRectangles()
{
    auto items = scene()->items();
    std::for_each(items.begin(), items.end(),
        [&items, this] (QGraphicsItem* item)
    {
        auto *pRect = dynamic_cast<AssociatedFaceRect *>(item);

        if (pRect)
        {
            pRect->setParentItem(NULL);
            scene()->removeItem(pRect);
        }

        delete pRect;
    });
}

void PhotoGraphics::contextMenuEvent(QContextMenuEvent *me)
{
    QGraphicsView::contextMenuEvent(me);

    if (me->isAccepted())
    {
        return;
    }

    QMenu widgetMenu;
    QAction* addAction = widgetMenu.addAction("New region");
    addAction->setData(menuAddAction);

    QPointF scenePos = mapToScene(me->pos());
    QPoint globalPos = mapToGlobal(me->pos());

    QAction* selectedItem = widgetMenu.exec(globalPos);
    if (selectedItem)
    {
        ContextMenuAction actionType = (ContextMenuAction) selectedItem->data().toInt();

        switch (actionType)
        {
        case menuAddAction:
            addUnassociatedRect(frameIndex, scenePos);
            break;
        }
    }
}

bool PhotoGraphics::eventFilter(QObject *dist, QEvent *evt)
{
    bool result = false;

    if (evt->type() == DeleteEvent::type())
    {
        result = processDeleteEvent(evt);
    }
    else if (evt->type() == ResizeEvent::type())
    {
        result = processResizeEvent(evt);
    }
    else if (evt->type() == AssociateEvent::type())
    {
        result = processAssociateEvent(evt);
    }
    else if (evt->type() == UnassociateEvent::type())
    {
        result = processUnassociateEvent(evt);
    }

    if (result)
    {
        return true;
    }

    return QGraphicsView::eventFilter(dist, evt);
}

bool PhotoGraphics::processDeleteEvent(QEvent *evt)
{
    DeleteEvent *de = dynamic_cast<DeleteEvent*>(evt);

    if (de != NULL)
    {
        FaceRectBase *r = de->getRectangle();

        if (r != NULL)
        {
            if (dynamic_cast<UnAssociatedFaceRect*>(de->getRectangle()))
            {
                emit deleteRectangle(frameIndex, r->getModelRect().getIndex());
            }
            else if (dynamic_cast<AssociatedFaceRect*>(de->getRectangle()))
            {
                emit deleteAssociatedRectangle(frameIndex, r->getModelRect().getIndex());
            }

            r->setParentItem(NULL);
            scene()->removeItem(r);
            delete r;
        }

        return true;
    }

    return false;
}

bool PhotoGraphics::processResizeEvent(QEvent *evt)
{
    ResizeEvent *re = dynamic_cast<ResizeEvent*> (evt);

    if (re != NULL)
    {
        FaceRectBase *r = re->getRectangle();

        if (r != NULL)
        {
            FaceRect faceRect = scaleFaceRect(r->getModelRect(), scaleFactor);

            if (dynamic_cast<UnAssociatedFaceRect*>(re->getRectangle()))
            {
                emit resizeRectangle(frameIndex, faceRect);
            }
            else if (dynamic_cast<AssociatedFaceRect*>(re->getRectangle()))
            {
                emit resizeAssociatedRectangle(frameIndex, faceRect);
            }
        }

        return true;
    }

    return false;
}

void PhotoGraphics::setFrameIndex(quint64 index)
{
    frameIndex = index;
}

FaceRect PhotoGraphics::scaleFaceRect(const FaceRect& fr, double scale)
{
    FaceRect out = fr;

    out.r.x *= scale;
    out.r.y *= scale;
    out.r.width *= scale;
    out.r.height *= scale;

    return out;
}

bool PhotoGraphics::processAssociateEvent(QEvent *evt)
{
    AssociateEvent *ae = dynamic_cast<AssociateEvent*> (evt);

    if (ae != NULL)
    {
        FaceRectBase *r = ae->getRectangle();

        if (r != NULL)
        {
            FaceRect faceRect = scaleFaceRect(r->getModelRect(), scaleFactor);

            emit deleteRectangle(frameIndex, faceRect.getIndex());

            emit associateRectangle(frameIndex, faceRect);
            scene()->removeItem(r);
            r->setParentItem(NULL);
            delete r;
        }
    }

    return false;
}

void PhotoGraphics::addAssociatedRectangle(const FaceRect& rct)
{
    FaceRect scaledRect = scaleFaceRect(rct, 1 / scaleFactor);
    FaceRectBase *pRect = new AssociatedFaceRect(scaledRect, pixmapItem);

    pRect->setMoveArea(pixmapItem->boundingRect());
    pRect->setSelected(true);
}

void PhotoGraphics::addUnassociatedRect(quint64 frameIndex, const QPointF& pt)
{
    FaceRect faceRect;
    faceRect.r.x = pt.x();
    faceRect.r.y = pt.y();
    faceRect.r.width = FaceRectBase::MIN_REGION_WIDTH;
    faceRect.r.height = FaceRectBase::MIN_REGION_HEIGHT;

    auto pRect = new UnAssociatedFaceRect(faceRect, pixmapItem);
    pRect->setMoveArea(pixmapItem->boundingRect());

    faceRect = scaleFaceRect(faceRect, scaleFactor);

    emit addRectangle(frameIndex, faceRect);
}

bool PhotoGraphics::processUnassociateEvent(QEvent *evt)
{
    UnassociateEvent *ae = dynamic_cast<UnassociateEvent*> (evt);

    if (ae != NULL)
    {
        FaceRectBase *r = ae->getRectangle();

        if (r != NULL)
        {
            FaceRect faceRect = scaleFaceRect(r->getModelRect(), scaleFactor);

            emit deleteAssociatedRectangle(frameIndex, faceRect.getIndex());

            emit addRectangle(frameIndex, faceRect);

            auto pRect = new UnAssociatedFaceRect(r->getModelRect(), pixmapItem);
            pRect->setMoveArea(pixmapItem->boundingRect());

            scene()->removeItem(r);
            r->setParentItem(NULL);
            delete r;
        }
    }

    return false;
}

void PhotoGraphics::clearAll()
{
    scene()->clear();

    frameIndex = 0;
    scaleFactor = 1.0;
    pixmapItem = NULL;
}
