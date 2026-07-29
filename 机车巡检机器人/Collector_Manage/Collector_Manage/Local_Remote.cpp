#include "Local_Remote.h"


c_Local_Remote::c_Local_Remote(QObject *parent) : c_Server_Remote(parent)
{

}

c_Local_Remote::~c_Local_Remote()
{
	
}

void c_Local_Remote::Connect()
{
	if (c_Variable::getInstance().g_Communicate_DB.value("Local_Ip") == QJsonValue::Undefined) {
		emit Status("->控制服务：ip不存在");
		Connect_Loop();
		return;
	}
	if (c_Variable::getInstance().g_Communicate_DB.value("Local_Remote_Port") == QJsonValue::Undefined) {
		emit Status("->控制服务：port不存在");
		Connect_Loop();
		return;
	}
	QString ip = c_Variable::getInstance().g_Communicate_DB.value("Local_Ip").toString();
	m_Port = c_Variable::getInstance().g_Communicate_DB.value("Local_Remote_Port").toInt();
	emit Connect_Device(ip, m_Port);
	emit Status("->" + ip + ":" + QString::number(m_Port) + "->控制服务：建立监听中");
}
void c_Local_Remote::Connect_Done()
{
	c_Variable::getInstance().g_Local_Remote.Connected = true;
	c_Variable::getInstance().g_Local_Remote.num++;
	emit Set_Working();
}
void c_Local_Remote::Disconnect_Done()
{
	c_Variable::getInstance().g_Local_Remote.Connected = false;
	c_Variable::getInstance().g_Local_Remote.num = 0;
	emit Set_Default();
}
