#ifndef __CORNERGRABBER_H__
#define __CORNERGRABBER_H__

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QPointF>

class CornerGrabber : public QGraphicsItem
{
public:
    enum MouseState
    {
        kMouseReleased=0, 
        kMouseDown, 
        kMouseMoving
    };

public:
    explicit CornerGrabber(QGraphicsItem *parent = 0, int corner=0);

    int getCorner();
    void setMouseState(MouseState);
    int getMouseState();

    qreal mouseDownX;
    qreal mouseDownY;

public:
    static const int CORNER_WIDTH;
    static const int CORNER_HEIGHT;

private:
    virtual QRectF boundingRect() const;
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );

    QColor _outterborderColor;
    QPen _outterborderPen;

    qreal cornerWidth;
    qreal cornerHeight;

    int cornerIndex;

    MouseState mouseButtonState;
    QColor defaultCornerColor;
    QColor selectedCornerColor;

};

#endif // CORNERGRABBER_H
