// Copyright (C) 2013, Intel Corporation, all rights reserved.

#ifndef PHOTOGRAPHICS_H
#define PHOTOGRAPHICS_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <opencv2/opencv.hpp>

#include "photo_book.hpp"

namespace Ui {
    class PhotoGraphics;
} // namespace Ui

class PhotoGraphics : public QGraphicsView
{
    Q_OBJECT
private:
    enum ContextMenuAction
    {
        menuAddAction
    };

public:
    explicit PhotoGraphics(QWidget *parent = 0);

    void setBackgroundImage(const cv::Mat& img);
    void reset();
    void setFaceRectangles(const std::vector<FaceRect>& rectanges);
    void setAssociatedRectangles(const std::vector<FaceRect>& rectanges);
    void clearFaceRectangles();
    void setFrameIndex(quint64 index);
    void addAssociatedRectangle(const FaceRect& rct);
    void clearMarkupRectangles();
    void clearAssociatedRectangles();
    void addUnassociatedRect(quint64 frameIndex, const QPointF& pt);
    void clearAll();

signals:
    void addRectangle(quint64 frameIndex, const FaceRect& r);
    void deleteRectangle(quint64 frameIndex, quint64 id);
    void resizeRectangle(quint64 frameIndex, const FaceRect& r);
    void associateRectangle(quint64 frameIndex, const FaceRect& r);
    void deleteAssociatedRectangle(quint64 frameIndex, quint64 id);
    void resizeAssociatedRectangle(quint64 frameIndex, const FaceRect& r);

public slots:

protected:
    void contextMenuEvent(QContextMenuEvent *me);
    bool eventFilter(QObject *dist, QEvent *evt);
    bool processDeleteEvent(QEvent *evt);
    bool processResizeEvent(QEvent *evt);
    bool processAssociateEvent(QEvent *evt);
    FaceRect scaleFaceRect(const FaceRect& fr, double scale);
    bool processUnassociateEvent(QEvent *evt);

private:
    Ui::PhotoGraphics *ui;
    double scaleFactor;
    QGraphicsPixmapItem *pixmapItem;
    quint64 frameIndex;
};

#endif // PHOTOGRAPHICS_H
