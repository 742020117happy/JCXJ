#include "E1R_DIFOP.h"

c_E1R_DIFOP::c_E1R_DIFOP(QObject *parent): c_Object(parent)
{
	
}

c_E1R_DIFOP::~c_E1R_DIFOP()
{
	if (m_DIFOP_thread->isRunning()) {
		m_DIFOP_thread->requestInterruption();
		m_DIFOP_thread->quit();
		m_DIFOP_thread->wait();
	}
}

void c_E1R_DIFOP::Init()
{
	
	m_DIFOP_thread = new QThread;
	m_DIFOP_client = new c_Scan_Client;
	m_DIFOP_client->moveToThread(m_DIFOP_thread);


	QObject::connect(m_DIFOP_thread, &QThread::started, m_DIFOP_client, &c_Scan_Client::Init);
	QObject::connect(m_DIFOP_thread, &QThread::finished, m_DIFOP_client, &c_Scan_Client::deleteLater);
	QObject::connect(m_DIFOP_client, &c_Scan_Client::Status, this, [=](QString msg) {
		emit Status("DIFOP:" + msg);
	});

	QObject::connect(m_DIFOP_client, &c_Scan_Client::Read_Done, this, &c_E1R_DIFOP::onDIFOPData);
	QObject::connect(m_DIFOP_client, &c_Scan_Client::Connect_Loop, this, &c_E1R_DIFOP::Connect);
	QObject::connect(this, &c_E1R_DIFOP::Connect_DIFOP, m_DIFOP_client, &c_Scan_Client::Connect_Device);
	QObject::connect(this, &c_E1R_DIFOP::Disconnect_Device, m_DIFOP_client, &c_Scan_Client::Disconnect_Device);

	QObject::connect(m_DIFOP_client, &c_Scan_Client::Connect_Done, this, &c_E1R_DIFOP::Connect_DIFOP_Done);
	QObject::connect(m_DIFOP_client, &c_Scan_Client::Disconnect_Done, this, &c_E1R_DIFOP::Disconnect_DIFOP_Done);

	m_DIFOP_thread->start();
	c_E1R_DIFOP::Connect();
}
void c_E1R_DIFOP::Connect()
{
	emit Connect_DIFOP(c_Variable::getInstance().g_RoboSense.Local_Ip, c_Variable::getInstance().g_RoboSense.Local_DIFOP_Port);
}
void c_E1R_DIFOP::Disconnect()
{
	emit Disconnect_Device();
}


void c_E1R_DIFOP::Connect_DIFOP_Done()
{
	c_Variable::getInstance().g_RoboSense.DIFOP_connected = true;
}

void c_E1R_DIFOP::Disconnect_DIFOP_Done()
{
	c_Variable::getInstance().g_RoboSense.DIFOP_connected = false;
}

//捕获雷达信息
void c_E1R_DIFOP::onDIFOPData(QByteArray data, QString device_ip, quint16 device_port)
{

	if (device_ip != c_Variable::getInstance().g_RoboSense.E1R_IP) {
		emit Status("onDIFOPData雷达地址错误");
		return;
	}
	if (device_port != c_Variable::getInstance().g_RoboSense.E1R_DIFOP_Port) {
		emit Status("onDIFOPData雷达端口错误");
		return;
	}
	if (data.size() < 256) {
		emit Status("onDIFOPData雷达数据错误");
		return;
	}

	std::memcpy(c_Variable::getInstance().g_DIFOP.raw, data.constData(), data.size());

	c_Variable::getInstance().g_RoboSense.device_Status = !c_Variable::getInstance().g_RoboSense.device_Status;
}


