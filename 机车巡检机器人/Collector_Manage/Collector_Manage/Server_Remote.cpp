#include "Server_Remote.h"

c_Server_Remote::c_Server_Remote(QObject *parent) : c_Object(parent)
{

}

c_Server_Remote::~c_Server_Remote()
{
	if (m_Robot_Server_Thread->isRunning()) {
		m_Robot_Server_Thread->requestInterruption();
		m_Robot_Server_Thread->quit();
		m_Robot_Server_Thread->wait();
	}
}

void c_Server_Remote::Init()
{
	m_Robot_Server = new c_Robot_Server;
	m_Robot_Server_Thread = new QThread;
	m_Robot_Server->moveToThread(m_Robot_Server_Thread);
	QObject::connect(m_Robot_Server_Thread, &QThread::started, m_Robot_Server, &c_Robot_Server::Init);
	QObject::connect(m_Robot_Server_Thread, &QThread::finished, m_Robot_Server, &c_Robot_Server::deleteLater);
	QObject::connect(m_Robot_Server, &c_Robot_Server::Status, this, [=](int state) {
		emit Status(QString::number(m_Port)+"->服务：" + TCP_Status(state));
	});
	//循环连接
	QObject::connect(this, &c_Server_Remote::Connect_Device, m_Robot_Server, &c_Robot_Server::Connect_Device);
	QObject::connect(this, &c_Server_Remote::Write_Json, m_Robot_Server, &c_Robot_Server::Write_Json);

	QObject::connect(m_Robot_Server, &c_Robot_Server::Connect_Done, this, &c_Server_Remote::Connect_Done);
	QObject::connect(m_Robot_Server, &c_Robot_Server::Disconnect_Done, this, &c_Server_Remote::Disconnect_Done);
	QObject::connect(m_Robot_Server, &c_Robot_Server::Connect_Loop, this, &c_Server_Remote::Connect_Loop);
	//读完成
	QObject::connect(m_Robot_Server, &c_Robot_Server::Read_Done, this, &c_Server_Remote::Read_Done);
	//开启监听
	m_Robot_Server_Thread->start();

	QTimer::singleShot(3000, this, &c_Server_Remote::Connect);
}

void c_Server_Remote::Connect()
{

}
void c_Server_Remote::Connect_Done()
{
	
	
}
void c_Server_Remote::Disconnect_Done()
{
	
}

void c_Server_Remote::Connect_Loop()
{
	QTimer::singleShot(6000, this, &c_Server_Remote::Connect);
}

void c_Server_Remote::Read_Done(QByteArray buffer)
{
	if (buffer.isEmpty()) { return; }
	QJsonDocument doc = QJsonDocument::fromJson(buffer);
	QJsonObject json = doc.object();
	if (json.isEmpty()) { return; }
	if (json.value("Cmd_Name") == QJsonValue::Undefined) { return; } 
	if (json.value("Value") == QJsonValue::Undefined) { return; }
	
	QString Cmd_Name = json.value("Cmd_Name").toString();
	bool Cmd_Value = json.value("Value").toBool();

	QString Cmd_Date;
	if (json.value("Date") != QJsonValue::Undefined) {
		Cmd_Date = json.value("Date").toString();
	}
	//服务器读准备就绪
	if (Cmd_Name == "Read_Ready" && Cmd_Value) {
		c_Variable::getInstance().g_Local_Remote.Tran = !c_Variable::getInstance().g_Local_Remote.Tran;
		//此处添加反馈消息

		emit Write_Json(m_value);
	}

	if (json.value("Checksum") == QJsonValue::Undefined) { return; }
	//启动
	if (Cmd_Name == "Work_Start") {
		emit Status(QString::number(m_Port)+"->服务：Work_Start");
		emit Start_Cmd(json);
		return;
	}
}
