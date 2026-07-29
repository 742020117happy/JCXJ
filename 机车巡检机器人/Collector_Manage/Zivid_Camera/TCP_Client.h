#pragma once
#include "Variable.h"

class c_TCP_Client : public c_Object
{
	Q_OBJECT
public:
	explicit c_TCP_Client(c_Object *parent = nullptr);
	virtual ~c_TCP_Client();

	public slots:
	void Init();//子线程初始化
	void Connect();//连接到服务器
	void Disconnect();//断开连接
	void Send_File_Info(QString filePath); //写Json数据

signals:
	void Connect_Done();//连接到服务器完成
	void Disconnect_Done();//断开连接完成
	void Connect_Loop();//循环检测连接状态
	void Write_Byte_Done(); //写完成
	void Send_File_Error(QString filePath);//读消息转发

private:
	QTcpSocket *m_Socket;//通讯对象
	QString m_Ip;
	int m_Port;
	bool m_Stop_Connect = false;
	QString m_filePath;
};

