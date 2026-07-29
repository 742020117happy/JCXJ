#include "E1R_Remote.h"

c_E1R_Remote::c_E1R_Remote(QString device_name, QObject *parent) : c_Process_Remote(parent)
{
	m_process_name = "robosense_manage";
	m_device_name = device_name;
}

c_E1R_Remote::~c_E1R_Remote()
{

}


void c_E1R_Remote::Device_Data(QString message)
{
	if (message.split("&", Qt::SkipEmptyParts).at(0) == "PointDate") {
		emit is_Obstacle(message);
	}
}