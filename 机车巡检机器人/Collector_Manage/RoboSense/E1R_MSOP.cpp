#include "E1R_MSOP.h"

c_E1R_MSOP::c_E1R_MSOP(QObject *parent): c_Object(parent)
{
	
}

c_E1R_MSOP::~c_E1R_MSOP()
{
	if (m_MSOP_thread->isRunning()) {
		m_MSOP_thread->requestInterruption();
		m_MSOP_thread->quit();
		m_MSOP_thread->wait();
	}
}
void c_E1R_MSOP::Init()
{
	m_MSOP_thread = new QThread;
	m_MSOP_client = new c_Scan_Client;
	m_MSOP_client->moveToThread(m_MSOP_thread);

	QObject::connect(m_MSOP_thread, &QThread::started, m_MSOP_client, &c_Scan_Client::Init);
	QObject::connect(m_MSOP_thread, &QThread::finished, m_MSOP_client, &c_Scan_Client::deleteLater);
	QObject::connect(m_MSOP_client, &c_Scan_Client::Status, this, [=](QString msg){
			emit Status("MSOP:" + msg);
	});

	QObject::connect(m_MSOP_client, &c_Scan_Client::Read_Done, this, &c_E1R_MSOP::onMsopData);
	QObject::connect(m_MSOP_client, &c_Scan_Client::Connect_Loop, this, &c_E1R_MSOP::Connect);
	QObject::connect(this, &c_E1R_MSOP::Connect_Msop, m_MSOP_client, &c_Scan_Client::Connect_Device);
	QObject::connect(this, &c_E1R_MSOP::Disconnect_Device, m_MSOP_client, &c_Scan_Client::Disconnect_Device);

	QObject::connect(m_MSOP_client, &c_Scan_Client::Connect_Done, this, &c_E1R_MSOP::Connect_Msop_Done);
	QObject::connect(m_MSOP_client, &c_Scan_Client::Disconnect_Done, this, &c_E1R_MSOP::Disconnect_Msop_Done);

	m_MSOP_thread->start();
	c_E1R_MSOP::Connect();
}
void c_E1R_MSOP::Connect()
{
	emit Connect_Msop(c_Variable::getInstance().g_RoboSense.Local_Ip, c_Variable::getInstance().g_RoboSense.Local_MSOP_Port);
}
void c_E1R_MSOP::Disconnect()
{
	emit Disconnect_Device();
}
void c_E1R_MSOP::Connect_Msop_Done()
{
	c_Variable::getInstance().g_RoboSense.MSOP_connected = true;
}
void c_E1R_MSOP::Disconnect_Msop_Done()
{
	c_Variable::getInstance().g_RoboSense.MSOP_connected = false;
}
void c_E1R_MSOP::onMsopData(QByteArray data, QString device_ip, quint16 device_port)
{
	if (device_ip != c_Variable::getInstance().g_RoboSense.E1R_IP) {
		emit Status(QString("onMsopData 雷达地址错误: %1 (期望 %2)")
			.arg(device_ip).arg(c_Variable::getInstance().g_RoboSense.E1R_IP));
		return;
	}
	if (device_port != c_Variable::getInstance().g_RoboSense.E1R_MSOP_Port) {
		emit Status(QString("onMsopData 雷达端口错误: %1 (期望 %2)")
			.arg(device_port).arg(c_Variable::getInstance().g_RoboSense.E1R_MSOP_Port));
		return;
	}
	if (data.size() != 1200) { 
		emit Status(QString("onMsopData 雷达数据长度错误: %1 (期望 1200)").arg(data.size()));
		return;
	}
	s_MSOPPacketUnion MSOP;

	std::memcpy(MSOP.raw, data.constData(), data.size());	

	quint32 syncValue = qFromBigEndian<quint32>(MSOP.data.header.sync);
	if (syncValue != 0x55AA5AA5U) {
		emit Status(QString("onMsopData 雷达数据帧头错误: 0x%1 (期望 0x55AA5AA5)")
			.arg(syncValue, 8, 16, QChar('0')).toUpper());
		return;
	}

	c_Variable::getInstance().g_RoboSense.lidarTmp = qFromBigEndian<qint8>(MSOP.data.header.lidarTmp) - 80;

	quint16 pktCnt = qFromBigEndian<quint16>(MSOP.data.header.pktCnt);

	if (pktCnt == 0 && !m_buffer.isEmpty()) {
		emit MSOP_Scan(m_buffer);
		m_buffer.clear();
	}
	for (int i=0; i < 96; ++i){

		if (MSOP.data.blocks[i].pointAttr == 2 ) {continue;}

		quint16 radius = qFromBigEndian<quint16>(MSOP.data.blocks[i].radius);
		float distance = static_cast<float>(radius) * 5.0f;

		qint16 dirVectorX = qFromBigEndian<qint16>(MSOP.data.blocks[i].dirVectorX);
		qint16 dirVectorY = qFromBigEndian<qint16>(MSOP.data.blocks[i].dirVectorY);
		qint16 dirVectorZ = qFromBigEndian<qint16>(MSOP.data.blocks[i].dirVectorZ);
		qint8 intensity = qFromBigEndian<qint8>(MSOP.data.blocks[i].intensity);
		float dirX = static_cast<float>(dirVectorX) / 32768.0f;
		float dirY = static_cast<float>(dirVectorY) / 32768.0f;
		float dirZ = static_cast<float>(dirVectorZ) / 32768.0f;
		float dirI = static_cast<float>(intensity);

		float x = distance * dirX;
		float y = distance * dirY;
		float z = distance * dirZ;

		if (100.f > x || x > 30000.0f) { continue; }

		m_buffer.append(pcl::PointXYZI{ x, y, z, dirI });
	}
}
