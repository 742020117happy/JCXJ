#pragma once
#include "Variable.h"

class c_Sql_Remote : public c_Object
{
	Q_OBJECT
public:
	explicit c_Sql_Remote(QObject *parent = nullptr);
	virtual ~c_Sql_Remote();

signals:
	void Connect_Done();

public slots:
	void Init();
	void connectMysql();
	void updateActualReturnTime();
	void updateFastScanTime();
	void Prec_Scan_Write_Done(QString value);

private:
	MYSQL *m_conn;
	QMutex *m_Mutex;
	QList<QString> m_imagePathList;

	private slots:
	void updateCollectTime();
};

class c_Work_Remote : public c_Object
{
	Q_OBJECT
public:
	explicit c_Work_Remote(QObject *parent = nullptr);
	virtual ~c_Work_Remote();

public slots:
	void Init();//初始化
	void Start_Cmd(QJsonObject object);//巡检指令


signals:
	void System_Scan_Done();
	void updateFastScanTime();
	void updateActualReturnTime();
	void Write_Json(QJsonObject json);

	void Work_Disconnected();//停止巡检

	void AutoEn_120();// 左机械臂自动使能
    void AutoDn_120();// 左机械臂自动下电
	void AutoEn_121();// 右机械臂自动使能
    void AutoDn_121();// 右机械臂自动下电

	void AutoEn_Done_120();// 左机械臂自动使能完成
    void AutoDn_Done_120();// 左机械臂自动下电完成
	void AutoEn_Done_121();// 右机械臂自动使能完成
    void AutoDn_Done_121();// 右机械臂自动下电完成

	void RunFunc_120(QString cmd);//左机械臂运行脚本函数
	void RunFunc_121(QString cmd);//右机械臂运行脚本函数

	void Huayan_120_Moving();//左机械臂运行中
	void Huayan_121_Moving();//右机械臂运行中

	void Robot_120();//左机械臂采集完成
	void Robot_121();//右机械臂采集完成
	
    void Pre_Scan_Done();//同步采集完成
private:
	QJsonObject m_Work_Program;
	QJsonObject m_Current_Work;
	QString m_Checksum;

private slots:
	void wait_AutoEn_120();// 左机械臂自动使能
    void wait_AutoDn_120();// 左机械臂自动下电
	void wait_AutoEn_121();// 右机械臂自动使能
    void wait_AutoDn_121();// 右机械臂自动下电

	void wait_RunFunc_120(QString cmd);//等待左机械臂运行脚本函数
	void wait_RunFunc_121(QString cmd);//等待右机械臂运行脚本函数

	void wait_Pre_Scan_Done();//等待同步采集完成
};
