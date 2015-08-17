#include "aboutwindow.hpp"
#include "ui_about.h"

#include <QStyle>

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    QStyle *pStyle = qApp->style();
    QIcon icon = pStyle->standardIcon(QStyle::SP_MessageBoxInformation);
    
    setWindowIcon(icon);
}
