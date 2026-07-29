#pragma once
#include "Variable.h"

class c_Scan_Client :  public c_Object
{
	Q_OBJECT
public:
	explicit c_Scan_Client(QObject *parent = nullptr);
	~c_Scan_Client() override;

public slots:
	void Init();//子线程初始化
	void Connect_Device(QString ip, int port);
	void Disconnect_Device();

signals:
	void Connect_Done();
	void Disconnect_Done();
	void Read_Done(QByteArray msg, QString device_ip, quint16 device_port);
	void Connect_Loop();

private:
	QUdpSocket* m_Socket;
	int m_Port;
};

