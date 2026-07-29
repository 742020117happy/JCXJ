#include "Hikvision_Remote.h"

c_Hikvision_Remote::c_Hikvision_Remote(QString device_name, QObject *parent) : c_Process_Remote(parent)
{
	m_process_name = "hikvision_manage";
	m_device_name = device_name;
}

c_Hikvision_Remote::~c_Hikvision_Remote()
{
	emit Status(m_device_name + "：子类正常析构");
}