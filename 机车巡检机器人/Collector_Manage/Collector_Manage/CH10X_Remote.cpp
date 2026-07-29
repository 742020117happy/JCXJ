#include "CH10X_Remote.h"

c_CH10X_Remote::c_CH10X_Remote(QString device_name, QObject *parent) : c_Process_Remote(parent)
{
	m_process_name = "hipnuc_ch10x_manage";
	m_device_name = device_name;
}

c_CH10X_Remote::~c_CH10X_Remote()
{
	emit Status(m_device_name + "：子类正常析构");
}

void c_CH10X_Remote::Device_Data(QString message)
{
	if (message.split("&", Qt::SkipEmptyParts).at(0) == "IMU_ODOM") {
    	c_Variable::getInstance().g_IMU_Odom.x = message.split("&", Qt::SkipEmptyParts).at(1).toDouble();
        c_Variable::getInstance().g_IMU_Odom.y = message.split("&", Qt::SkipEmptyParts).at(2).toDouble();
        c_Variable::getInstance().g_IMU_Odom.vx = message.split("&", Qt::SkipEmptyParts).at(3).toDouble();
        c_Variable::getInstance().g_IMU_Odom.vy = message.split("&", Qt::SkipEmptyParts).at(4).toDouble();
        c_Variable::getInstance().g_IMU_Odom.yaw = message.split("&", Qt::SkipEmptyParts).at(5).toDouble();
        c_Variable::getInstance().g_IMU_Odom.timestamp = message.split("&", Qt::SkipEmptyParts).at(6).toInt();
        c_Variable::getInstance().g_IMU_Odom.is_static = message.split("&", Qt::SkipEmptyParts).at(7).toInt();
	}
}