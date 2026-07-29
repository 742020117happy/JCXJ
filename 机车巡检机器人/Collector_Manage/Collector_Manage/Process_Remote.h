#pragma once
#include "Variable.h"

/*************************************************************************************************************************************************
**Function:面阵雷达遥控父类定义
*************************************************************************************************************************************************/
class c_Process_Remote : public c_Object
{
	Q_OBJECT
public:
	explicit c_Process_Remote(QObject *parent = nullptr);
	virtual ~c_Process_Remote();
public:
	QString m_process_name;
	QString m_device_name;
public slots:
	void Init();
	void Connect_Device();
	void Connect();
	void Disconnect();
	void Close_Device();
	void Write(QString msg);
public slots:
	virtual void Device_Data(QString message);
signals:
	void Connect_Done();
	void Disconnect_Done();
	void Show(WId Window);
	void is_Run();
	void is_Stop();
private:
	QProcess* m_Process;
	bool m_Start = false;
private slots:
	void Read_Data();
	void Error_Data();
};
