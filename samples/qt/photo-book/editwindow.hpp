// Copyright (C) 2013, Intel Corporation, all rights reserved.

#ifndef __EDIT_WINDOW_H__
#define __EDIT_WINDOW_H__

#include <QDialog>
#include "metadata_helper.hpp"
#include "markup_model.hpp"

namespace Ui 
{
class EditWindow;
}

class EditWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EditWindow(MetadataHelper *pHelper, MarkupModel *pModel,
        QWidget *parent = 0);
    ~EditWindow();

signals:

public slots:
    void onAddPressed();
    void onSavePressed();
    void listSelectionChanged();
    void onDeletePressed();

private:
    void fillPeopleList();
    void exitEditMode();
    bool checkInputData(std::shared_ptr<vmf::Metadata>& newInfo);
    void clearInfoFields();
    void fillInfo();
    void enableEditControls(bool bEnable);

private:
    Ui::EditWindow *ui;
    MetadataHelper *helperPtr;
    bool inAddMode;
    MarkupModel *markupPtr;
};

#endif /* __EDIT_WINDOW_H__ */
