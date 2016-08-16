// Copyright (C) 2013, Intel Corporation, all rights reserved.
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QCloseEvent>

#include "umf/umf.hpp"

#define MS_NAME "trek"
#define PROP_X "gps_coordinate_x"
#define PROP_Y "gps_coordinate_y"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    coordinatesIsNeeded(true)
{
    ui->setupUi(this);
    ui->enableGpsCheckbox->setChecked(coordinatesIsNeeded);
    ui->videoSlider->setEnabled(false);
    connect(ui->actionOpen, SIGNAL(triggered()), SLOT(openFile()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(stepVideo()));
    connect(ui->videoSlider, SIGNAL(sliderMoved(int)), this, SLOT(startVideo(int)));
    connect(ui->videoSlider, SIGNAL(valueChanged(int)), this, SLOT(startVideo(int)));
    connect(ui->calcSpeedButton, SIGNAL(clicked()), this, SLOT(calculateSpeed()));
    connect(ui->enableGpsCheckbox, SIGNAL(stateChanged(int)), this, SLOT(changeGpsOutputState(int)));
    connect(ui->playPauseButton, SIGNAL(clicked()), this, SLOT(playPause()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stop()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));

    QStyle *pStyle = qApp->style();
    QIcon icon = pStyle->standardIcon(QStyle::SP_MediaPlay);
    ui->playPauseButton->setIcon(icon);

    icon = pStyle->standardIcon(QStyle::SP_MediaStop);
    ui->stopButton->setIcon(icon);

    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::startVideo(int percentage)
{
    bool isCurrentlyPlaying = timer.isActive();
    if (isCurrentlyPlaying)
        timer.stop();
    vmstream.seek(percentage);
    showCurrentFrame();
    if (isCurrentlyPlaying)
        timer.start(frameDelay);
}

void MainWindow::calculateSpeed()
{
    if (!vmstream.isOpened())
    {
        return;
    }
    bool isCurrentlyPlaying = timer.isActive();
    if (isCurrentlyPlaying)
        pause();
    vmstream.calculateSpeed();
    if (isCurrentlyPlaying)
        resume();
}

void MainWindow::resume()
{
    timer.start(frameDelay);

    QStyle *pStyle = qApp->style();
    QIcon icon = pStyle->standardIcon(QStyle::SP_MediaPause);
    ui->playPauseButton->setIcon(icon);
}

void MainWindow::stop()
{
    pause();
    vmstream.seek(0);
    updateVideSlider();
    clearDataLabels();
    QGraphicsScene* oldScene = ui->videoFrame->scene();
    ui->videoFrame->setScene(new QGraphicsScene(ui->videoFrame));
    delete oldScene;

	ui->frameLabel->setText("");
}

void MainWindow::playPause()
{
    if (!vmstream.isOpened())
    {
        return;
    }

    if (timer.isActive())
        pause();
    else
        resume();
}


void MainWindow::pause()
{
    timer.stop();

    QStyle *pStyle = qApp->style();
    QIcon icon = pStyle->standardIcon(QStyle::SP_MediaPlay);
    ui->playPauseButton->setIcon(icon);
}

void MainWindow::stepVideo()
{
    showCurrentFrame();
    vmstream.next();
}

void MainWindow::showCurrentFrame()
{
    cv::Mat frame = vmstream.getCurrentFrame();
    if (frame.empty())
    {
        stop();
        return;
    }

    int w = frame.cols, h = frame.rows;
    QImage image(frame.data, w, h, QImage::Format_RGB888);
    double dw = double(w)/ui->videoFrame->width();
    double dh = double(h)/ui->videoFrame->height();
    double scale = MAX(dw, dh);
    image = image.scaled(floor(w/scale), floor(h/scale));
    QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    QGraphicsScene* oldScene = ui->videoFrame->scene();
    QGraphicsScene* scene = new QGraphicsScene(ui->videoFrame);
    scene->setBackgroundBrush(QBrush(Qt::black));
    scene->addItem(pixmapItem);
    ui->videoFrame->setScene(scene);
    delete oldScene;
	ui->frameLabel->setText(QString::number(vmstream.getCurrentFrameNumber()));

    if (coordinatesIsNeeded)
    {
        GpsCoordinates coords = vmstream.getCurrentCoordinates();
        ui->coord_x->setText(QString::number(coords.first));
        ui->coord_y->setText(QString::number(coords.second));
        if (vmstream.hasSpeed())
        {
            float speed = vmstream.getCurrentSpeed();
            ui->speedLabel->setText(QString::number(speed));
        }
    }
    else
    {
        clearDataLabels();
    }

    updateVideSlider();
}


void MainWindow::openFile()
{
    stop();
    if (vmstream.isOpened())
    {
        if (QMessageBox::question(this, "Save file", "Do you want to save this file before opening other?",
            QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        {
            vmstream.saveSpeed();
        }
    }
    QString filename = QFileDialog::getOpenFileName( this, "Open video file", QString(),
        "Video files (*.avi *.mpg *.mp4 *.mov *.3gp *.wmv);; All files (*.*)" );
    if (filename.length() == 0)
        return;
    try
    {
        if (vmstream.isOpened())
        {
            vmstream.closeVideo();
            vmstream.closeMetadata();
        }
        vmstream.openFile(filename);
        frameDelay = vmstream.getFrameDelay();
        ui->videoSlider->setEnabled(true);
        startVideo();
        resume();
    }
    catch (const std::runtime_error& e)
    {
        QMessageBox::critical(this, "Error", QString("Cannot open specified file: ") + e.what());
        vmstream.closeVideo();
        vmstream.closeMetadata();
    }
}

void MainWindow::changeGpsOutputState(int checkBoxState)
{
    coordinatesIsNeeded = (static_cast<Qt::CheckState>(checkBoxState) == Qt::Checked);
    if (coordinatesIsNeeded)
        showCurrentFrame();
    else
        clearDataLabels();
}

void MainWindow::showAboutDialog()
{
    QMessageBox::about(this, "About Visualizer",
        "Visualizer is a desktop component of the UMF library Ski Resort sample. "
        "Please see the User guide for more info.\n"
        "Copyright (C) 2013, Intel Corporation, all rights reserved.");
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    stop();
    if (vmstream.isOpened())
    {
        if (QMessageBox::question(this, "Save file", "Do you want to save this file before exit?",
            QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        {
            vmstream.saveSpeed();
        }
    }
    timer.stop();
    vmstream.closeVideo();
    vmstream.closeMetadata();
    event->accept();
}

void MainWindow::updateVideSlider()
{
    if (!vmstream.isOpened())
        ui->videoSlider->setValue(0);
    else
        ui->videoSlider->setValue(vmstream.getCurrentPercentage());
}

void MainWindow::clearDataLabels()
{
    ui->coord_x->setText("");
    ui->coord_y->setText("");
    ui->speedLabel->setText("");
}
