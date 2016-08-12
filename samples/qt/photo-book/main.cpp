#include "mainwindow.hpp"

#include "vmf/vmf.hpp"

#include <QApplication>
#include <opencv2/opencv.hpp>

int main(int argc, char *argv[])
{
    const cv::String keys =
        "{|cascades-path||eeee}"
        ;

    cv::CommandLineParser parser(argc, 
        (const char**) argv, keys.c_str());

    umf::initialize();
    QApplication a(argc, argv);

    cv::String cascadesPath = parser.get<cv::String>("cascades-path");

    if (cascadesPath.empty())
    {
        cascadesPath = qApp->applicationDirPath().toStdString();
    }

    MainWindow w(cascadesPath);

    QSize sz = w.size();
    w.setFixedSize(sz);
    w.show();

    int r = a.exec();
    umf::terminate();
    return r;
}
