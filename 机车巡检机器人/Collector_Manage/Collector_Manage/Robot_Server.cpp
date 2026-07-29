#include "Robot_Server.h"

c_Robot_Server::c_Robot_Server(QObject *parent) : QObject(parent)
{

}

c_Robot_Server::~c_Robot_Server()
{
	Disconnect_Device();
	if (m_Server->isListening()) {
		m_Server->close();
	}
	m_Server->deleteLater();
}

void c_Robot_Server::Init()
{
	m_Server = new QTcpServer(this);
    QObject::connect(m_Server, &QTcpServer::acceptError, this, &c_Robot_Server::Status);
	QObject::connect(m_Server, &QTcpServer::newConnection, this, &c_Robot_Server::New_Connection);
}
void c_Robot_Server::Connect_Device(QString ip, int port)
{    
	
	if (m_Server->isListening()) {
		return;
	}
	if (!m_Server->listen(QHostAddress(ip), port)) {
		emit Connect_Loop();
	}
	else {
		emit Listen_Done();
	}
}

void c_Robot_Server::Disconnect_Device()
{
	for (int i = 0; i < m_SocketList.size(); ++i) {
		if (m_SocketList.at(i)) {
			m_SocketList.at(i)->disconnectFromHost();
			m_SocketList.at(i)->deleteLater();	
		}
	}
	m_SocketList.clear();
}

void c_Robot_Server::New_Connection()
{
	if (!m_Server->hasPendingConnections()) {
		return;
	}
		
	QTcpSocket *Socket = m_Server->nextPendingConnection();
	m_SocketList.append(Socket);
	emit Connect_Done();

	QObject::connect(Socket, &QTcpSocket::disconnected, this, [Socket, this]() {
		m_SocketList.removeOne(Socket);
		Socket->deleteLater();
		if (m_SocketList.isEmpty()) { emit Disconnect_Done(); }
	});

	QObject::connect(Socket, &QTcpSocket::readyRead, this, [Socket, this]() {
		QByteArray buffer = Socket->readAll();
		if (buffer.isEmpty()) { return; }
		emit Read_Done(buffer);
	});
}

void c_Robot_Server::Write_Json(QJsonObject json)
{
	if (json.isEmpty()) { return; }
	if (m_SocketList.isEmpty()) { return; }

	for (int i = 0; i < m_SocketList.size(); ++i) {
		if (!m_SocketList.at(i)) { continue; }
		else {
			if (m_SocketList.at(i)->state() != QTcpSocket::ConnectedState) { continue; }
			else {
				QByteArray sendMessage = QJsonDocument(json).toJson();
				if (sendMessage.isEmpty()) { continue; }
				else {
					m_SocketList.at(i)->write(sendMessage);
					if (m_SocketList.at(i)->flush()) {
						emit Write_Json_Done();
					}
					else {
						emit Write_Json_Error();
					}
				}
				
			}
		}
	}
}
void c_Robot_Server::Write_String(QString str)
{
	if (str.isEmpty()) { return; }
	if (m_SocketList.isEmpty()) { return; }

	for (int i = 0; i < m_SocketList.size(); ++i) {
		if (!m_SocketList.at(i)) { continue; }
		else {
			if (m_SocketList.at(i)->state() != QTcpSocket::ConnectedState) { continue; }
			else {
				m_SocketList.at(i)->write(str.toUtf8());
				if (m_SocketList.at(i)->flush()) {
					emit Write_String_Done();
				}
				else {
					emit Write_String_Error();
				}
			}
		}
	}
}
void c_Robot_Server::Write_Byte(QByteArray arry)
{
	if (arry.isEmpty()) { return; }
	if (m_SocketList.isEmpty()) { return; }

	for (int i = 0; i < m_SocketList.size(); ++i) {
		if (!m_SocketList.at(i)) { continue; }
		else {
			if (m_SocketList.at(i)->state() != QTcpSocket::ConnectedState) { continue; }
			else {
				m_SocketList.at(i)->write(arry);

				if (m_SocketList.at(i)->flush()) {
					emit Write_Byte_Done();
				}
				else {
					emit Write_Byte_Error();
				}
			}
		}
	}
}
