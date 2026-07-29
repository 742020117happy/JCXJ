#pragma once
#include "Variable.h"
#include "Scan_Client.h"


class c_E1R_MSOP  : public c_Object
{
	Q_OBJECT

public:
	explicit c_E1R_MSOP(QObject *parent = nullptr);
	~c_E1R_MSOP() override;

public slots:
	void Init();
	void Connect();
	void Disconnect();

signals:
	void Connect_Msop(QString ip, int port);
	void Disconnect_Device();
	void MSOP_Scan(QVector<pcl::PointXYZI> points);

private:
	QThread* m_MSOP_thread;
	c_Scan_Client* m_MSOP_client;

	QVector<pcl::PointXYZI> m_buffer;

private slots:
	void Connect_Msop_Done();
	void Disconnect_Msop_Done();
	void onMsopData(QByteArray data, QString device_ip, quint16 device_port);

};
