#ifndef ABOUTWINDOW_HPP
#define ABOUTWINDOW_HPP

#include <QDialog>

namespace Ui 
{
class AboutDialog;
}

class AboutWindow : public QDialog
{
    Q_OBJECT
public:
    explicit AboutWindow(QWidget *parent = 0);

signals:

public slots:

private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTWINDOW_HPP
