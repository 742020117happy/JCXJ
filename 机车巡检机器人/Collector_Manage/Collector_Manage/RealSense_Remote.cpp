#include "RealSense_Remote.h"

c_RealSense_Remote::c_RealSense_Remote(QString device_name, QObject *parent) : c_Process_Remote(parent)
{
	m_process_name = "realsense_manage";
	m_device_name = device_name;
}

c_RealSense_Remote::~c_RealSense_Remote()
{
	emit Status(m_device_name + "：子类正常析构");
}