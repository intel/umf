// Copyright (C) 2013, Intel Corporation, all rights reserved.
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include "umf/vmf.hpp"
#include "videometadatastream.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void openFile();
    void stop();
    void playPause();
    void stepVideo();
    void startVideo(int percentage=0);
    void calculateSpeed();
    void changeGpsOutputState(int checkBoxState);
    void showAboutDialog();
    void closeEvent(QCloseEvent* event);

private:
    void resume();
    void pause();
    void showCurrentFrame();
    void updateVideSlider();
    void clearDataLabels();

    VideoMetadataStream vmstream;
    Ui::MainWindow *ui;
    QTimer timer;
    QIcon* currentPlayStopIcon;
    int frameDelay;
    bool coordinatesIsNeeded;
};

#endif // MAINWINDOW_HPP
