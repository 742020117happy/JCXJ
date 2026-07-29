#include "TCP_Client.h"

c_TCP_Client::c_TCP_Client(c_Object *parent) : c_Object(parent)
{

}

c_TCP_Client::~c_TCP_Client()
{
	c_TCP_Client::Disconnect();
	m_Socket->deleteLater();
}

void c_TCP_Client::Init()
{
    //实例化
    m_Socket = new QTcpSocket;
    //如果，客户端，状态改变，发送，本线程，状态信息
    QObject::connect(m_Socket, &QAbstractSocket::errorOccurred, this, [=](QAbstractSocket::SocketError socketError)
    {
       emit Status(c_Object::TCP_Status(socketError));
    });
    //如果，客户端，状态改变，执行，本线程，状态改变函数
    QObject::connect(m_Socket, &QTcpSocket::stateChanged, this, [=](int state) {
		emit Status(c_Object::TCP_Status(state+23));
	});

    // connected/disconnected 信号（主逻辑）
    QObject::connect(m_Socket, &QTcpSocket::connected, this, 
                     &c_TCP_Client::Connect_Done, Qt::QueuedConnection);  // ✅
    QObject::connect(m_Socket, &QTcpSocket::disconnected, this, 
                     &c_TCP_Client::Disconnect_Done, Qt::QueuedConnection);  // ✅

	//开启监听模式{机器人，有可读取通道，对象，读取信号}
	QObject::connect(m_Socket, &QTcpSocket::readyRead, this, [=]() {

		QByteArray buffer = m_Socket->readAll();

		if (buffer.isEmpty()) { return; }

		QJsonObject fileInfo = QJsonDocument::fromJson(buffer).object();

		if (fileInfo.isEmpty()) { return; }

		emit Status(QJsonDocument(fileInfo).toJson());

		QString cmd;
		if (fileInfo.value("cmd") == QJsonValue::Undefined) {
			emit Status("cmd不存在");
			return;
		}
		cmd = fileInfo.value("cmd").toString();

		if (cmd == "START")
		{
			//开始发送
			c_Variable::getInstance().g_Transmission.Start = true;
			c_Variable::getInstance().g_Transmission.Finish = false;

			QString filePath;
			if (fileInfo.value("filePath") == QJsonValue::Undefined) {
				emit Status("cmd::filePath不存在");
				return;
			}
			filePath = fileInfo.value("filePath").toString();

			m_filePath = filePath;

			QFile file(filePath);
			if (!file.open(QIODevice::ReadOnly)) {
				emit Send_File_Error(filePath);
				return;
			}
			m_Socket->write(file.readAll());

			if (m_Socket->flush()) {
				emit Write_Byte_Done();
			}

			file.close();
		}
		if (cmd == "FINISH") {
			QString savePath = fileInfo.value("filePath").toString();
			msleep(10);
			//发送完成
			c_Variable::getInstance().g_Transmission.Start = false;
			c_Variable::getInstance().g_Transmission.Finish = true;
		}
		if (cmd == "ERROR") {
			if (fileInfo.value("filePath") != QJsonValue::Undefined) {
				QString filePath = fileInfo.value("filePath").toString();
				emit Send_File_Error(filePath);
			}
			msleep(100);
			c_Variable::getInstance().g_Transmission.Start = false;
			c_Variable::getInstance().g_Transmission.Finish = true;
		}
	});
	QTimer::singleShot(3000, this, &c_TCP_Client::Connect);
}

void c_TCP_Client::Connect()
{
	//如果已连接则返回
    if(!m_Socket) {return;}
	if(m_Socket->state() != QAbstractSocket::UnconnectedState) {return;}

	//如果发出断开连接请求则终止循环连接，并复位请求标志
	if (m_Stop_Connect) {
		m_Stop_Connect = false;
		return;
	}

    //建立新的连接
	m_Ip = c_Variable::getInstance().g_Communicate_DB.value("Server_Ip").toString();
	m_Port = c_Variable::getInstance().g_Communicate_DB.value("Server_Port").toInt();

	emit Status(m_Ip + ":" + QString::number(m_Port));

	m_Socket->connectToHost(m_Ip, m_Port);

	//等待连接3秒
	if(!m_Socket->waitForConnected(3000)){
		QTimer::singleShot(6000, this, &c_TCP_Client::Connect);
	}
}
void c_TCP_Client::Disconnect()
{
	if(!m_Socket){
		m_Stop_Connect = true;
		return;
	}
	if (m_Socket->state() != QAbstractSocket::ConnectedState) {
		m_Stop_Connect = true;
		return;
	}
	else {
		m_Socket->close();
	}
}
void c_TCP_Client::Send_File_Info(QString filePath)
{
    //如果设备未打开
	if (!m_Socket) { return; }
	if (m_Socket->state() != QAbstractSocket::ConnectedState)
	{
		emit Send_File_Error(filePath);
		return;
	}
	if (filePath.isEmpty()) { return; }

	c_Variable::getInstance().g_Transmission.Start = true;
	c_Variable::getInstance().g_Transmission.Finish = false;

	QFile file(filePath);

	if (!file.open(QFile::ReadOnly)) {
		c_Variable::getInstance().g_Transmission.Start = false;
		c_Variable::getInstance().g_Transmission.Finish = true;
		return;
	}
	
	int fileSize = file.size();

	// 计算文件的MD5哈希值
	QCryptographicHash hash(QCryptographicHash::Md5);
	if (file.seek(0)) {
		hash.addData(file.readAll());
		file.seek(0); // 重置文件指针
	}
	QString fileHash = hash.result().toHex();

	file.close();

	QJsonObject fileInfo;
	fileInfo.insert("cmd", "file_info");
	fileInfo.insert("filePath", filePath);
	fileInfo.insert("fileSize", QString::number(fileSize));
	fileInfo.insert("hash", fileHash);

	emit Status(QJsonDocument(fileInfo).toJson());

	msleep(100);

	m_Socket->write(QJsonDocument(fileInfo).toJson());

	if (m_Socket->flush()) {
		emit Write_Byte_Done();
	}


}

