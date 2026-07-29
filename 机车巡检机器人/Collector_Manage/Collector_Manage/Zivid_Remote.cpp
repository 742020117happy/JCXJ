#include "Zivid_Remote.h"

c_Zivid_Remote::c_Zivid_Remote(QString device_name, QObject *parent) : c_Process_Remote(parent)
{
	m_process_name = "zivid_manage";
	m_device_name = device_name;
}

c_Zivid_Remote::~c_Zivid_Remote()
{
	emit Status(m_device_name + "：子类正常析构");
}

void c_Zivid_Remote::updateSaveInfo(QString save_path)
{
	c_Process_Remote::Write("save_path&"+save_path);
}

void c_Zivid_Remote::Capture(QString image_name)
{
	c_Process_Remote::Write("image_name&"+image_name);
}

void c_Zivid_Remote::Device_Data(QString message)
{
	if (message.split("&", Qt::SkipEmptyParts).at(0) == "CaptureCompleted") {
    	emit CaptureCompleted();
	}

	if (message.split("&", Qt::SkipEmptyParts).at(0) == "updateCompleted") {
		emit updateCompleted(); 
	}
}