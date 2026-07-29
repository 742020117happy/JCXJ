#pragma once
#include "Variable.h"

class c_Robot_Server : public QObject
{
	Q_OBJECT
public:
	explicit c_Robot_Server(QObject *parent = nullptr);
	virtual ~c_Robot_Server();

	public slots:
	void Init();
	void Connect_Device(QString ip, int port);
	void Disconnect_Device();
	void Write_Json(QJsonObject json);
	void Write_String(QString str);
	void Write_Byte(QByteArray arry);

signals:
	void Listen_Done();
	void Connect_Done();
	void Connect_Loop();
	void Disconnect_Done();
	void Write_Json_Done();
	void Write_Json_Error();
	void Write_String_Done();
	void Write_String_Error();
	void Write_Byte_Done();
	void Write_Byte_Error();
	void Read_Done(QByteArray buffer);
	void Read_Error();
	void Status(int state);

	private slots:
	void New_Connection();

private:
	QTcpServer *m_Server = nullptr;
	QList<QTcpSocket*> m_SocketList;
};

