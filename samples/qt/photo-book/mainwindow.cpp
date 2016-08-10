// Copyright (C) 2013, Intel Corporation, all rights reserved.

#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QImage>
#include <QColor>
#include <QPen>
#include <QKeyEvent>
#include <QStyle>

#include <algorithm>
#include <functional>

#include "mainwindow.hpp"
#include "aboutwindow.hpp"
#include "editwindow.hpp"
#include "face_rectangles.hpp"
#include "events.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(const cv::String& cascadesPath, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    totalFrames(0ULL),
    recognizer(cascadesPath),
    currentFrameNumber(0),
    disableRectAssociate(false)
{
    ui->setupUi(this);
    timer = new QTimer(this);

    connect(ui->actionOpen, SIGNAL(triggered()), SLOT(openFile()));
    connect(ui->actionAbout, SIGNAL(triggered()), SLOT(showAboutInfo()));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), SLOT(sliderChanged(int)));
    connect(timer, SIGNAL(timeout()), this, SLOT(detectFaces()));
    connect(ui->actionRenewMarkup, SIGNAL(triggered()), SLOT(renewMarkup()));
    connect(ui->actionCommit_Markup, SIGNAL(triggered()), SLOT(commitMarkup()));
    connect(ui->graphicsView, SIGNAL(deleteRectangle(quint64, quint64)),
        &markupModel, SLOT(deleteUnassociatedRegion(quint64, quint64)));
    connect(ui->graphicsView, SIGNAL(addRectangle(quint64, const FaceRect&)),
        &markupModel, SLOT(addUnassociatedRegion(quint64, const FaceRect&)));
    connect(ui->graphicsView, SIGNAL(resizeRectangle(quint64, const FaceRect&)),
        &markupModel, SLOT(resizeUnassociatedRegion(quint64, const FaceRect&)));
    connect(ui->btnEdit, SIGNAL(clicked()), SLOT(editUserData()));
    connect(ui->graphicsView, SIGNAL(associateRectangle(quint64, const FaceRect&)),
        SLOT(associateRectangle(quint64, const FaceRect&)));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), SLOT(personChanged(int)));
    connect(ui->graphicsView, SIGNAL(deleteAssociatedRectangle(quint64, quint64)),
        &markupModel, SLOT(deleteAssociatedRegion(quint64, quint64)));
    connect(ui->graphicsView, SIGNAL(resizeAssociatedRectangle(quint64, const FaceRect&)),
        &markupModel, SLOT(resizeAssociatedRegion(quint64, const FaceRect&)));
    connect(ui->actionNew_Region, SIGNAL(triggered()), SLOT(addNewRegion()));
    connect(ui->actionDelete_Region, SIGNAL(triggered()), SLOT(deleteRegion()));

    QStyle *pStyle = qApp->style();
    QIcon icon = pStyle->standardIcon(QStyle::SP_DialogOpenButton);
    ui->actionOpen->setIcon(icon);

    icon = pStyle->standardIcon(QStyle::SP_DialogApplyButton);
    ui->actionCommit_Markup->setIcon(icon);

    icon = pStyle->standardIcon(QStyle::SP_DialogCancelButton);
    ui->actionDelete_Region->setIcon(icon);

    icon = pStyle->standardIcon(QStyle::SP_MessageBoxInformation);
    ui->actionAbout->setIcon(icon);

    icon = pStyle->standardIcon(QStyle::SP_FileDialogNewFolder);
    ui->actionNew_Region->setIcon(icon);

    icon = pStyle->standardIcon(QStyle::SP_BrowserReload);
    ui->actionRenewMarkup->setIcon(icon);

    ui->graphicsView->setMouseTracking(true);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->horizontalSlider->setEnabled(false);
    ui->actionCommit_Markup->setEnabled(false);
    ui->actionRenewMarkup->setEnabled(false);
    
    lblFrameNumber = new QLabel(this);
    statusBar()->addWidget(lblFrameNumber, 100);

    connect(ui->graphicsView->scene(), SIGNAL(selectionChanged()),
        SLOT(onSceneSelection()));

    qApp->installEventFilter(this);
    clearWidgets();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    QFileDialog dialog;

    timer->stop();

    if (capture.isOpened() && !checkSave())
    {
        return;
    }

    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("Video files (*.avi *.mpg *.mp4 *.mov *.3gp *.wmv);; All files (*.*)");

    if (dialog.exec())
    {
        QStringList fileList = dialog.selectedFiles();

        if (fileList.size() > 0)
        {
            clearWidgets();

            if (capture.isOpened())
            {
                capture.release();
            }

            QString fileName = fileList.at(0);
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);

            try
            {
                if (!metadataHelper.loadMetaFile(fileName.toStdString()))
                {
                    msgBox.setText("Can't load metadata from specified file.");
                    msgBox.exec();
                    return;
                }

                currentFileName = fileName.toStdString();
                metadataHelper.closeFile();

                auto stream = metadataHelper.getMetadataStream();
                auto peopleMetadata = stream->queryByName(MetadataHelper::PEOPLE_DESC_NAME);

                markupModel.loadPeopleInfo(metadataHelper.getMetadataStream());
                fillPeopleList();

                if (!capture.open(currentFileName))
                {
                    msgBox.setText("Can't open specified file.");
                    msgBox.exec();
                }
                else
                {
                    capture.set(CV_CAP_PROP_CONVERT_RGB, true);

                    ui->graphicsView->reset();
                    loadVideoData();
                }
            }
            catch (vmf::Exception& e)
            {
                msgBox.setText(e.what());
                msgBox.exec();
            }
            catch (cv::Exception& e)
            {
                msgBox.setText(e.what());
                msgBox.exec();
            }
        }
    }
}

void MainWindow::showAboutInfo()
{
    AboutWindow aboutDialog;
    aboutDialog.exec();
}

void MainWindow::loadVideoData()
{
    totalFrames = (quint64) capture.get(CV_CAP_PROP_FRAME_COUNT);

    if (totalFrames > 0)
    {
        ui->horizontalSlider->setMinimum(0);
        ui->horizontalSlider->setMaximum((int) totalFrames - 1);
        ui->horizontalSlider->setSliderPosition(0);

        enableWidgets(true);

        videoWidth = (int) capture.get(CV_CAP_PROP_FRAME_WIDTH);
        videoHeight = (int) capture.get(CV_CAP_PROP_FRAME_HEIGHT);
        loadVideoData(0);
    }
    else
    {
        clearWidgets();
    }
}

void MainWindow::sliderChanged(int value)
{
    if (capture.isOpened())
    {
        loadVideoData(value);
    }
}

void MainWindow::loadVideoData(int frameNumber)
{
    timer->stop();

    capture.set(CV_CAP_PROP_POS_FRAMES, frameNumber);
    currentFrameNumber = frameNumber;
    ui->graphicsView->setFrameIndex(currentFrameNumber);

    cv::Mat bgrImg;
    capture >> bgrImg;

    if (bgrImg.empty())
    {
        return;
    }

    cvtColor(bgrImg, currentFrameImg, cv::COLOR_BGR2RGB);
    ui->graphicsView->setBackgroundImage(currentFrameImg);

    lblFrameNumber->setText("Current fame number: " + QString::number(frameNumber));

    MarkupModel::RegionList rl = markupModel.getAssociatedRegions(frameNumber);
    ui->graphicsView->setAssociatedRectangles(rl);

    if (markupModel.isAlreadyViewed(frameNumber))
    {
        showFaceRectangels(frameNumber);
    }
    else
    {
        timer->start(300);
    }
}

void MainWindow::detectFaces()
{
    timer->stop();

    std::vector< cv::Rect_<int> > detectedFaces;
    recognizer.detectFaces(currentFrameImg, detectedFaces);

    currentFaces.clear();
    for (size_t i = 0; i < detectedFaces.size(); i++)
    {
        FaceRect r;
        r.r = detectedFaces[i];
        currentFaces.push_back(r);
    }

    markupModel.setUnassociatedRegions(currentFrameNumber, currentFaces);
    ui->graphicsView->setFaceRectangles(currentFaces);
}

void MainWindow::showFaceRectangels(MarkupModel::FrameIndex index)
{
    MarkupModel::RegionList rl = markupModel.getUnassociatedRegions(index);
    ui->graphicsView->setFaceRectangles(rl);
}

void MainWindow::renewMarkup()
{
    ui->graphicsView->clearMarkupRectangles();

    qApp->processEvents();
    detectFaces();
}

void MainWindow::commitMarkup()
{
    capture.release();

    try
    {
        markupModel.storeMarkup();
        markupModel.storeAssociations();

        if (metadataHelper.openFile(vmf::MetadataStream::Update))
        {
            metadataHelper.saveMetadata();
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText("Unable to save metadata.");
            msgBox.exec();

            return;
        }
    }
    catch (vmf::Exception& e)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(e.what());
        msgBox.exec();
    }

    metadataHelper.closeFile();

    capture.open(currentFileName);
    capture.set(CV_CAP_PROP_POS_FRAMES, currentFrameNumber);
}

void MainWindow::scrollFrame(int delta)
{
    MarkupModel::FrameIndex index = currentFrameNumber;

    if (delta < 0)
    {
        if (index < abs(delta))
        {
            index = 0;
        }
        else
        {
            index += delta;
        }
    }
    else
    {
        if (totalFrames <= index + delta)
        {
            index = totalFrames - 1;
        }
        else
        {
            index += delta;
        }
    }

    ui->horizontalSlider->setSliderPosition(index);

    loadVideoData(index);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *ev)
{
    if (ev->type() == QEvent::KeyPress) 
    {
        QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(ev);

        if (!keyEvent)
        {
            return false;
        }

        bool result = false;
        switch (keyEvent->key())
        {
        case Qt::Key_Left:
            scrollFrame(-1);
            result = true;
            break;
        case Qt::Key_Right:
            scrollFrame(1);
            result = true;
            break;
        }

        return result;
    } 
    else
    {
        return QObject::eventFilter(obj, ev);
    }
}

void MainWindow::editUserData()
{
    EditWindow editDialog(&metadataHelper, &markupModel, this);
    editDialog.exec();

    disableRectAssociate = true;
    fillPeopleList();
    disableRectAssociate = false;

    ui->graphicsView->clearAssociatedRectangles();
    MarkupModel::RegionList rl = markupModel.getAssociatedRegions(
        currentFrameNumber);
    ui->graphicsView->setAssociatedRectangles(rl);
}

void MainWindow::associateRectangle(quint64 frameIndex, const FaceRect& r)
{
    int index = ui->comboBox->currentIndex();

    if (index != -1)
    {
        vmf::IdType id = ui->comboBox->itemData(index).toULongLong();
        AssociatedRect ar = markupModel.addAssociatedRegion(frameIndex, id, r);
        ui->graphicsView->addAssociatedRectangle(ar);
    }
}

void MainWindow::fillPeopleList()
{
    auto stream = metadataHelper.getMetadataStream();
    auto itemSet = stream->queryByName(MetadataHelper::PEOPLE_DESC_NAME);

    ui->comboBox->clear();

    for (size_t i = 0; i < itemSet.size(); i++)
    {
        auto item = itemSet[i];

        try
        {
            std::string name = item->getFieldValue(FieldNameConstants::NAME_FIELD);
            ui->comboBox->addItem(QString::fromStdString(name), (qulonglong) item->getId());
        }
        catch (vmf::Exception&)
        {
            // Ignore exception
        }
    }

    clearPersonInfo();

    if (!disableRectAssociate)
    {
        ui->comboBox->setEnabled(false);
    }
}

void MainWindow::personChanged(int index)
{
    if (index == -1)
    {
        return;
    }

    auto stream = metadataHelper.getMetadataStream();
    auto itemSet = stream->queryByName(MetadataHelper::PEOPLE_DESC_NAME);

    try
    {
        auto metadata = itemSet[index];
        vmf::vmf_integer age = metadata->getFieldValue(FieldNameConstants::AGE_FIELD);
        ui->lblAge->setText(QString::number(age));

        vmf::vmf_integer weight = metadata->getFieldValue(FieldNameConstants::WEIGHT_FIELD);
        ui->lblWeight->setText(QString::number(weight));

        vmf::vmf_integer height = metadata->getFieldValue(FieldNameConstants::HEIGHT_FIELD);
        ui->lblHeight->setText(QString::number(height));

        vmf::vmf_integer sex = metadata->getFieldValue(FieldNameConstants::SEX_FIELD);
        ui->lblSex->setText(Utilities::sexEnum2String((PersonSex) sex));

        auto items = scene->selectedItems();
        if (!items.empty())
        {
            auto firstItem = dynamic_cast<AssociatedFaceRect *>(items.first());
            if (firstItem != NULL)
            {
                quint64 rectIndex = firstItem->getModelRect().getIndex();
                markupModel.changeRectAssociation(currentFrameNumber,
                    rectIndex, metadata->getId());
            }
            else
            {
                auto firstItem = dynamic_cast<UnAssociatedFaceRect *>(items.first());

                if (firstItem != NULL && !disableRectAssociate)
                {
                    QCoreApplication::sendEvent(ui->graphicsView, new AssociateEvent(firstItem));
                }
            }
        }
    }
    catch (vmf::Exception&)
    {
        // Ignore exception
    }
}

void MainWindow::onSceneSelection()
{
    auto selItems = scene->selectedItems();

    if (!selItems.empty())
    {
        ui->comboBox->setEnabled(true);
        auto graphicsItem = dynamic_cast<AssociatedFaceRect*>(selItems.first());

        if (graphicsItem != NULL)
        {
            FaceRect rect = graphicsItem->getModelRect();
            vmf::IdType personId;
            if (markupModel.findPersonByRegion(currentFrameNumber, rect.getIndex(), 
                personId))
            {
                int personIndex = ui->comboBox->findData((qulonglong) personId);

                if (personIndex != -1)
                {
                    ui->comboBox->setCurrentIndex(personIndex);
                }
            }
        }
        else
        {
            clearPersonInfo();
        }

        ui->actionDelete_Region->setEnabled(true);
    }
    else
    {
        clearPersonInfo();
        ui->comboBox->setEnabled(false);
        ui->actionDelete_Region->setEnabled(false);
    }
}

void MainWindow::clearPersonInfo()
{
    ui->lblAge->setText("-");
    ui->lblHeight->setText("-");
    ui->lblSex->setText("-");
    ui->lblWeight->setText("-");
    ui->comboBox->setCurrentIndex(-1);
}

void MainWindow::addNewRegion()
{
    ui->graphicsView->addUnassociatedRect(currentFrameNumber, QPointF());
}

void MainWindow::deleteRegion()
{
    auto selItems = scene->selectedItems();

    if (!selItems.empty())
    {
        auto graphicsItem = dynamic_cast<FaceRectBase*>(selItems.first());
        if (graphicsItem != NULL)
        {
            QCoreApplication::sendEvent(ui->graphicsView, new DeleteEvent(graphicsItem));
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *evt)
{
    if (!capture.isOpened())
    {
        evt->accept();
        return;
    }

    if (checkSave())
    {
        evt->accept();
    }
    else
    {
        evt->ignore();
    }
}

bool MainWindow::checkSave()
{
    int result = QMessageBox::question(this, "Exit", 
        "Would you like to save your changes before exit?",
        QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);

    switch (result)
    {
    case QMessageBox::Yes:
        commitMarkup();
    case QMessageBox::No:
        return true;
    }

    return false;
}

void MainWindow::clearWidgets()
{
    markupModel.clear();

    ui->graphicsView->clearAll();
    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(0);
    ui->horizontalSlider->setSliderPosition(0);
    ui->comboBox->clear();
    lblFrameNumber->clear();

    enableWidgets(false);

    clearPersonInfo();
}

void MainWindow::enableWidgets(bool bEnabled)
{
    ui->graphicsView->setEnabled(bEnabled);
    ui->horizontalSlider->setEnabled(bEnabled);
    ui->actionCommit_Markup->setEnabled(bEnabled);
    ui->actionRenewMarkup->setEnabled(bEnabled);
    ui->actionNew_Region->setEnabled(bEnabled);

    if (!bEnabled)
    {
        ui->comboBox->setEnabled(bEnabled);
    }

    ui->btnEdit->setEnabled(bEnabled);
}
