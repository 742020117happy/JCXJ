#pragma once
#include "Robot_Server.h"

/*************************************************************************************************************************************************
**Function:对外接口父类定义
*************************************************************************************************************************************************/
class c_Server_Remote : public c_Object
{
	Q_OBJECT
public:
	explicit c_Server_Remote(QObject *parent = nullptr);
	virtual ~c_Server_Remote();
	c_Robot_Server *m_Robot_Server;
	QThread *m_Robot_Server_Thread;
	QJsonObject m_value;
	int m_Port = 0;
	public slots:
	virtual void Connect();
	virtual void Connect_Done();
	virtual void Disconnect_Done();
	void Init();
	void Connect_Loop();
	void Read_Done(QByteArray buffer);

signals:
	void Connect_Device(QString ip, int port);//连接到服务器
	void Write_Json(QJsonObject cmd);//写状态
	void Start_Cmd(QJsonObject cmd);//开始巡检
	void Set_Working();//工作状态
	void Set_Default();//非工作状态

private:
	bool m_dataBit = false;
	float m_dataFloat = 0.00f; //浮点数缓冲区
	quint32 m_data32Bits = 0; //32位整型缓冲区
	quint16 m_high16Bits = 0; //32位高16位缓冲区
	quint16 m_low16Bits = 0;  //32位低16位缓冲区
	quint16 m_data16Bits = 0; //16位整型缓冲区

	QString m_Checksum;
};