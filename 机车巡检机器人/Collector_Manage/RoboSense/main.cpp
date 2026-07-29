#include "RoboSense.h"
#include <QApplication>
#include <QVector>
#include <pcl/point_types.h>

Q_DECLARE_METATYPE(QVector<pcl::PointXYZI>)
Q_DECLARE_METATYPE(s_Timestamp)
Q_DECLARE_METATYPE(s_MSOPHeader)
Q_DECLARE_METATYPE(s_DataBlock)
Q_DECLARE_METATYPE(s_MSOPTrailer)
Q_DECLARE_METATYPE(s_MSOPPacket)
Q_DECLARE_METATYPE(s_DIFOPHeader)
Q_DECLARE_METATYPE(s_IPv4Address)
Q_DECLARE_METATYPE(s_MACAddress)
Q_DECLARE_METATYPE(s_FirmwareVersion)
Q_DECLARE_METATYPE(s_DIFOPPacket)
Q_DECLARE_METATYPE(s_RoboSense_DB)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<QVector<pcl::PointXYZI>>("QVector<pcl::PointXYZI>");
    qRegisterMetaType<s_Timestamp>("s_Timestamp");
    qRegisterMetaType<s_MSOPHeader>("s_MSOPHeader");
    qRegisterMetaType<s_DataBlock>("s_DataBlock");
    qRegisterMetaType<s_MSOPTrailer>("s_MSOPTrailer");
    qRegisterMetaType<s_MSOPPacket>("s_MSOPPacket");
    qRegisterMetaType<s_DIFOPHeader>("s_DIFOPHeader");
    qRegisterMetaType<s_IPv4Address>("s_IPv4Address");
    qRegisterMetaType<s_MACAddress>("s_MACAddress");
    qRegisterMetaType<s_FirmwareVersion>("s_FirmwareVersion");
    qRegisterMetaType<s_DIFOPPacket>("s_DIFOPPacket");
    qRegisterMetaType<s_RoboSense_DB>("s_RoboSense_DB");

    c_RoboSense w(a.arguments());

    return a.exec();
}
