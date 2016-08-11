// Copyright (C) 2013, Intel Corporation, all rights reserved.

#include <QMessageBox>

#include "editwindow.hpp"
#include "photo_book.hpp"
#include "ui_editwindow.h"

EditWindow::EditWindow(MetadataHelper *pHelper, MarkupModel *pModel, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditWindow)
    , helperPtr(pHelper)
    , inAddMode(false)
    , markupPtr(pModel)
{
    ui->setupUi(this);

    ui->btnSave->setEnabled(false);
    ui->btnDelete->setEnabled(false);

    ui->cmbSex->addItem(Utilities::sexEnum2String(male), male);
    ui->cmbSex->addItem(Utilities::sexEnum2String(female), female);

    connect(ui->btnAdd, SIGNAL(clicked()), SLOT(onAddPressed()));
    connect(ui->btnSave, SIGNAL(clicked()), SLOT(onSavePressed()));
    connect(ui->btnDelete, SIGNAL(clicked()), SLOT(onDeletePressed()));
    connect(ui->listWidget, SIGNAL(itemSelectionChanged()), SLOT(listSelectionChanged()));

    fillPeopleList();

    if (ui->listWidget->count() > 0)
    {
        ui->listWidget->setCurrentRow(0);
    }
}

EditWindow::~EditWindow()
{
}

void EditWindow::fillPeopleList()
{
    if (helperPtr == NULL)
    {
        return;
    }

    ui->listWidget->clear();

    auto stream = helperPtr->getMetadataStream();
    auto itemSet = stream->queryByName(MetadataHelper::PEOPLE_DESC_NAME);

    for (size_t i = 0; i < itemSet.size(); i++)
    {
        auto item = itemSet[i];

        try
        {
            std::string personName = item->getFieldValue(FieldNameConstants::NAME_FIELD);

            QListWidgetItem *qlwi = new QListWidgetItem();
            qlwi->setText(QString::fromStdString(personName));
            qlwi->setData(Qt::UserRole, (qulonglong) item->getId());

            ui->listWidget->addItem(qlwi);
        }
        catch (vmf::Exception&)
        {
        }
    }

    if (ui->listWidget->count() > 0)
    {
        enableEditControls(true);
    }
    else
    {
        enableEditControls(false);
    }
}

void EditWindow::onAddPressed()
{
    if (!inAddMode)
    {
        ui->btnAdd->setText("Cancel");
        inAddMode = true;
        ui->btnSave->setEnabled(true);
        ui->btnDelete->setEnabled(false);

        clearInfoFields();
        enableEditControls(true);
    }
    else
    {
        exitEditMode();
    }
}

void EditWindow::onSavePressed()
{
    try
    {
        auto stream = helperPtr->getMetadataStream();
        auto schema = stream->getSchema(MetadataHelper::PHOTO_BOOK_SCHEMA_NAME);
        auto desc = schema->findMetadataDesc(MetadataHelper::PEOPLE_DESC_NAME);

        if (!desc)
        {
            return;
        }


        if (inAddMode)
        {
            std::shared_ptr<vmf::Metadata> newInfo(new vmf::Metadata(desc));

            if (checkInputData(newInfo))
            {
                stream->add(newInfo);
            }
        }
        else
        {
            QModelIndex index = ui->listWidget->currentIndex();
            auto item = stream->getById(index.row());
            checkInputData(item);
        }
    }
    catch (vmf::Exception& e)
    {
        QMessageBox::warning(NULL, "Error", QString("Internal error: ") + e.what(), QMessageBox::Ok);
        return;
    }

    fillPeopleList();
    exitEditMode();
}

void EditWindow::exitEditMode()
{
    ui->btnAdd->setText("Add new");
    inAddMode = false;
    
    if (ui->listWidget->count() <= 0)
    {
        enableEditControls(false);
        clearInfoFields();
    }

    fillInfo();
}

bool EditWindow::checkInputData(std::shared_ptr<vmf::Metadata>& newInfo)
{
    QTextCursor cursor = ui->txtName->textCursor();
    newInfo->setFieldValue(FieldNameConstants::NAME_FIELD, cursor.document()->toPlainText().toStdString());

    int listIndex = ui->cmbSex->currentIndex();
    if (listIndex != -1)
    {
        PersonSex ps = (PersonSex) ui->cmbSex->itemData(listIndex).toInt();
        newInfo->setFieldValue(FieldNameConstants::SEX_FIELD, (vmf::umf_integer) ps);
    }

    cursor = ui->txtAge->textCursor();

    bool ok;
    vmf::umf_integer result = cursor.document()->toPlainText().toUShort(&ok);

    if (!ok)
    {
        QMessageBox::warning(NULL, "Error", "Invalid age", QMessageBox::Ok);
        return false;
    }

    newInfo->setFieldValue(FieldNameConstants::AGE_FIELD, result);

    cursor = ui->txtWeight->textCursor();
    result = cursor.document()->toPlainText().toUShort(&ok);

    if (!ok)
    {
        QMessageBox::warning(NULL, "Error", "Invalid weight", QMessageBox::Ok);
        return false;
    }

    newInfo->setFieldValue(FieldNameConstants::WEIGHT_FIELD, result);

    cursor = ui->txtHeight->textCursor();
    result = cursor.document()->toPlainText().toUShort(&ok);

    if (!ok)
    {
        QMessageBox::warning(NULL, "Error", "Invalid height", QMessageBox::Ok);
        return false;
    }

    newInfo->setFieldValue(FieldNameConstants::HEIGHT_FIELD, result);

    return true;
}

void EditWindow::listSelectionChanged()
{
    if (inAddMode)
    {
        exitEditMode();
    }
    else
    {
        fillInfo();
    }
}

void EditWindow::clearInfoFields()
{
    ui->txtName->clear();
    ui->txtAge->clear();
    ui->txtHeight->clear();
    ui->txtWeight->clear();
    ui->cmbSex->setCurrentIndex(0);
}

void EditWindow::fillInfo()
{
    auto listItem = ui->listWidget->currentItem();

    if (listItem != NULL)
    {
        ui->btnDelete->setEnabled(true);

        vmf::IdType requiredId = listItem->data(Qt::UserRole).toULongLong();

        auto stream = helperPtr->getMetadataStream();
        auto itemSet = stream->queryByName(MetadataHelper::PEOPLE_DESC_NAME);

        std::shared_ptr<vmf::Metadata> item;
        bool result = [&item, &itemSet](vmf::IdType id) -> bool
            {
                for (size_t i = 0; i < itemSet.size(); i++)
                {
                    if (itemSet[i]->getId() == id)
                    {
                        item = itemSet[i];
                        return true;
                    }
                }

                return false;
            }(requiredId);

        if (!result)
        {
            return;
        }

        try
        {
            std::string name = item->getFieldValue(FieldNameConstants::NAME_FIELD);
            ui->txtName->textCursor().document()->setPlainText(QString::fromStdString(name));

            vmf::umf_integer age = item->getFieldValue(FieldNameConstants::AGE_FIELD);
            QString str = QString::number(age);
            ui->txtAge->textCursor().document()->setPlainText(str);

            vmf::umf_integer weight = item->getFieldValue(FieldNameConstants::WEIGHT_FIELD);
            str = QString::number(weight);
            ui->txtWeight->textCursor().document()->setPlainText(str);

            vmf::umf_integer height = item->getFieldValue(FieldNameConstants::HEIGHT_FIELD);
            str = QString::number(height);
            ui->txtHeight->textCursor().document()->setPlainText(str);

            vmf::umf_integer sex = item->getFieldValue(FieldNameConstants::SEX_FIELD);
            int dataIndex = ui->cmbSex->findData((qlonglong) sex);

            if (dataIndex != -1)
            {
                ui->cmbSex->setCurrentIndex(dataIndex);
            }
        }
        catch (vmf::Exception&)
        {
            // Ignore exception
        }
    }
    else
    {
        ui->btnDelete->setEnabled(false);
    }
}

void EditWindow::onDeletePressed()
{
    auto listItem = ui->listWidget->currentItem();

    if (listItem != NULL)
    {
        vmf::IdType personId = listItem->data(Qt::UserRole).toULongLong();

        auto stream = helperPtr->getMetadataStream();
        stream->remove(personId);
        ui->listWidget->removeItemWidget(listItem);
        delete listItem;

        if (ui->listWidget->count() == 0)
        {
            clearInfoFields();
            enableEditControls(false);
        }

        markupPtr->removeAssociatedRegions(personId);
    }
}

void EditWindow::enableEditControls(bool bEnable)
{
    ui->txtAge->setEnabled(bEnable);
    ui->txtHeight->setEnabled(bEnable);
    ui->txtName->setEnabled(bEnable);
    ui->txtWeight->setEnabled(bEnable);
    ui->cmbSex->setEnabled(bEnable);
    ui->btnSave->setEnabled(bEnable);
}
