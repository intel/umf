// Copyright (C) 2013, Intel Corporation, all rights reserved.
#include "mainwindow.hpp"
#include <QApplication>
#include "vmf/vmf.hpp"

int main(int argc, char *argv[])
{
    vmf::initialize();
    QApplication a(argc, argv);
    MainWindow w;

    QSize sz = w.size();
    w.setFixedSize(sz);
    w.show();

    int retval = a.exec();
    vmf::terminate();
    return retval;
}
