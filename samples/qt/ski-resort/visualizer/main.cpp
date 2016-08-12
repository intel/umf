// Copyright (C) 2013, Intel Corporation, all rights reserved.
#include "mainwindow.hpp"
#include <QApplication>
#include "umf/vmf.hpp"

int main(int argc, char *argv[])
{
    umf::initialize();
    QApplication a(argc, argv);
    MainWindow w;

    QSize sz = w.size();
    w.setFixedSize(sz);
    w.show();

    int retval = a.exec();
    umf::terminate();
    return retval;
}
