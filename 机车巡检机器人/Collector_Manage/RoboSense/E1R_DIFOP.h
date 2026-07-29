#pragma once
#include "Variable.h"
#include "Scan_Client.h"

class c_E1R_DIFOP  : public c_Object
{
	Q_OBJECT

public:
	explicit c_E1R_DIFOP(QObject *parent = nullptr);
	~c_E1R_DIFOP() override;

public slots:
	void Init();
	void Connect();
	void Disconnect();

signals:
	void Connect_DIFOP(QString ip, int port);
	void Disconnect_Device();

private:
	QThread* m_DIFOP_thread;
	c_Scan_Client *m_DIFOP_client;

private slots:
	void Connect_DIFOP_Done();
	void Disconnect_DIFOP_Done();
	void onDIFOPData(QByteArray data, QString device_ip, quint16 device_port);
};

