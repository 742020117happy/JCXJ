#include "Scan_Client.h"

c_Scan_Client::c_Scan_Client(QObject *parent) : c_Object(parent)
{

}

c_Scan_Client::~c_Scan_Client()
{
	c_Scan_Client::Disconnect_Device();
    m_Socket->close();
	m_Socket->deleteLater();
}

void c_Scan_Client::Init()
{
    //实列化
    m_Socket = new QUdpSocket;
    //如果，UDP，状态改变，执行，本线程，状态改变函数
    QObject::connect(m_Socket, &QUdpSocket::stateChanged, this, [=](QAbstractSocket::SocketState state){
        int status = 0;
        switch (state) {
        case QAbstractSocket::UnconnectedState:
            status = 23;  
            break;
        case  QAbstractSocket::HostLookupState:
            status = 24;
            break;
        case  QAbstractSocket::ConnectingState:
            status = 25;
            break;
        case  QAbstractSocket::ConnectedState:
            status = 26;
            break;
        case  QAbstractSocket::BoundState:
            status = 27;
            emit Connect_Done();
            break;
        case  QAbstractSocket::ClosingState:
            status = 28;
            break;
        case  QAbstractSocket::ListeningState:
            status = 29;
            break;
        default:
            status = 30;
            break;
        }
        emit Status(c_Object::TCP_Status(status));
    });
    //错误诊断
    QObject::connect(m_Socket, &QUdpSocket::errorOccurred, this, [=](QAbstractSocket::SocketError socketError){
            emit Status(c_Object::TCP_Status(socketError));
    });
	//绑定读函数
	QObject::connect(m_Socket, &QUdpSocket::readyRead, this, [=](){
        if (m_Socket->hasPendingDatagrams()) {
            QByteArray dataGram;
            dataGram.resize(static_cast<int>(m_Socket->pendingDatagramSize()));
            QHostAddress device_ip;
            quint16 device_port;

            qint64 len = m_Socket->readDatagram(dataGram.data(), dataGram.size(), &device_ip, &device_port);
            //如果消息不为空则发送消息到主线程
            if (len > 0) {
                emit Read_Done(dataGram, device_ip.toString(), device_port);
            }
        }
	});
}

void c_Scan_Client::Connect_Device(QString ip, int port)
{
    //如果已连接则返回
    if(m_Socket->state() == QAbstractSocket::BoundState){	
		return;
    }
    //绑定主机、监听信号
	m_Port = port;
	if(!m_Socket->bind(QHostAddress(ip), port)){
        QTimer::singleShot(6000, this, &c_Scan_Client::Connect_Loop);
	}else{
        emit Connect_Done();
        emit Status("监听" + ip + ":" + QString::number(port));
	}
}

void c_Scan_Client::Disconnect_Device()
{
	if(!m_Socket){
        emit Disconnect_Done();
		return;
    }
	if (m_Socket->state() != QAbstractSocket::BoundState) {
        emit Disconnect_Done();
		return;
	}
	else {
		m_Socket->close();
        emit Disconnect_Done();
	}
}


