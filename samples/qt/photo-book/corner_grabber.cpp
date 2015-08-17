#include <QDebug>
#include "corner_grabber.hpp"

const int CornerGrabber::CORNER_WIDTH = 8;
const int CornerGrabber::CORNER_HEIGHT = 8;

CornerGrabber::CornerGrabber(QGraphicsItem *parent, int corner) :
    QGraphicsItem(parent),
    mouseDownX(0),
    mouseDownY(0),
    _outterborderPen(),
    cornerWidth(CORNER_WIDTH),
    cornerHeight(CORNER_HEIGHT),
    cornerIndex(corner),
    mouseButtonState(kMouseReleased)
{
    defaultCornerColor = Qt::green;
    selectedCornerColor = Qt::red;
    _outterborderColor = defaultCornerColor;

    setParentItem(parent);

    _outterborderPen.setWidth(2);
    _outterborderPen.setColor(_outterborderColor);

    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

void CornerGrabber::setMouseState(MouseState s)
{
    mouseButtonState = s;
}

int CornerGrabber::getMouseState()
{
    return mouseButtonState;
}

int CornerGrabber::getCorner()
{
    return cornerIndex;
}

void CornerGrabber::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
    _outterborderColor = defaultCornerColor;
    this->update(0, 0, cornerWidth, cornerHeight);
}

void CornerGrabber::hoverEnterEvent ( QGraphicsSceneHoverEvent * )
{
    _outterborderColor = selectedCornerColor;
    update(0, 0, cornerWidth, cornerHeight);
}

QRectF CornerGrabber::boundingRect() const
{
    return QRectF(0, 0, cornerWidth, cornerHeight);
}

void CornerGrabber::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    _outterborderPen.setCapStyle(Qt::SquareCap);
    _outterborderPen.setStyle(Qt::SolidLine);
    painter->setPen(_outterborderPen);

    QPointF topLeft (0, 0);
    QPointF bottomRight (cornerWidth, cornerHeight);

    QRectF rect (topLeft, bottomRight);

    QBrush brush (Qt::SolidPattern);
    brush.setColor (_outterborderColor);
    painter->fillRect(rect, brush);
}
