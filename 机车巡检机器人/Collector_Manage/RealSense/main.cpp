#include "RealSense.h"
#include <QtWidgets/QApplication>
#include <QVector>
#include <pcl/point_types.h>
Q_DECLARE_METATYPE(QVector<pcl::PointXYZRGB>)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<QVector<pcl::PointXYZRGB>>("QVector<pcl::PointXYZRGB>");
    c_RealSense w(a.arguments());
    return a.exec();
}
