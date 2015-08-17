#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QLabel>

#include "face_recognizer.hpp"
#include "metadata_helper.hpp"
#include "markup_model.hpp"
#include "photo_book.hpp"

namespace Ui 
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(const cv::String& cascadesPath, QWidget *parent = 0);
    ~MainWindow();
    
public Q_SLOTS:
    void openFile();
    void showAboutInfo();
    void sliderChanged(int value);
    void detectFaces();
    void renewMarkup();
    void commitMarkup();
    void editUserData();
    void associateRectangle(quint64 frameIndex, const FaceRect& r);
    void personChanged(int index);
    void onSceneSelection();
    void addNewRegion();
    void deleteRegion();

protected:
    void closeEvent(QCloseEvent *evt);

private:
    void loadVideoData();
    void loadVideoData(int frameNumber);
    void showFaceRectangels(MarkupModel::FrameIndex index);
    void scrollFrame(int delta);
    bool eventFilter(QObject *obj, QEvent *ev);
    void fillPeopleList();
    void clearPersonInfo();
    bool checkSave();
    void clearWidgets();
    void enableWidgets(bool bEnabled);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    cv::VideoCapture capture;
    qint64 totalFrames;
    cv::Mat currentFrameImg;
    QTimer *timer;
    FaceRecognizer recognizer;
    std::vector<FaceRect> currentFaces;
    int videoWidth;
    int videoHeight;
	MetadataHelper metadataHelper;
    std::string currentFileName;
    MarkupModel markupModel;
    quint64 currentFrameNumber;
    bool disableRectAssociate;
    QLabel *lblFrameNumber;
};

#endif // MAINWINDOW_HPP
