#pragma once

#include "ui_Collector_Manage.h"
#include "Zivid_Remote.h"
#include "E1R_Remote.h"
#include "Huayan_Monitor_120.h"
#include "Huayan_Monitor_121.h"
#include "Huayan_Remote_120.h"
#include "Huayan_Remote_121.h"
#include "Scan_Server_120.h"
#include "Scan_Server_121.h"
#include "RealSense_Remote.h"
#include "Hikvision_Remote.h"
#include "RGV_Remote.h"
#include "Local_Remote.h"
#include "Work_Remote.h"
#include "CH10X_Remote.h"
#include "Map_Widget.h"

class c_Collector_Manage : public QMainWindow
{
    Q_OBJECT

public:
	explicit c_Collector_Manage(QWidget *parent = Q_NULLPTR);
	virtual ~c_Collector_Manage();

	QThread *m_RGV_Remote_Thread;
	c_RGV_Remote *m_RGV_Remote;

	QThread *m_Scan_Server_120_Thread;
	c_Scan_Server_120 *m_Scan_Server_120;

	QThread *m_Scan_Server_121_Thread;
 	c_Scan_Server_121 *m_Scan_Server_121;
	
	QThread *m_Huayan_Monitor_120_Thread;
	c_Huayan_Monitor_120 *m_Huayan_Monitor_120;

	QThread *m_Huayan_Monitor_121_Thread;
 	c_Huayan_Monitor_121 *m_Huayan_Monitor_121;

	QThread *m_Huayan_Remote_120_Thread;
	c_Huayan_Remote_120 *m_Huayan_Remote_120;

	QThread *m_Huayan_Remote_121_Thread;
	c_Huayan_Remote_121 *m_Huayan_Remote_121;

	QThread *m_Zivid_1_Thread;
	c_Zivid_Remote *m_Zivid_1_Remote;

	QThread *m_Zivid_2_Thread;
	c_Zivid_Remote *m_Zivid_2_Remote;

	QThread* m_E1R_1_Thread;
	c_E1R_Remote* m_E1R_1_Remote;

	QThread* m_E1R_2_Thread;
	c_E1R_Remote* m_E1R_2_Remote;

	QThread* m_RealSense_1_Thread;
	c_RealSense_Remote* m_RealSense_1_Remote;

	QThread* m_RealSense_2_Thread;
	c_RealSense_Remote* m_RealSense_2_Remote;

	QThread* m_Hikvision_Thread;
	c_Hikvision_Remote* m_Hikvision_Remote;

	QThread *m_Local_Remote_Thread;
	c_Local_Remote *m_Local_Remote;

	QThread *m_Work_Remote_Thread;
	c_Work_Remote *m_Work_Remote;

	QThread *m_Sql_Remote_Thread;
	c_Sql_Remote *m_Sql_Remote;

	QThread *m_CH10X_Thread;
	c_CH10X_Remote *m_CH10X_Remote;

public slots:
	void System_Scan();

	void RGV_Init();
	void RGV_DB();
	void RGV_Button();
	void RGV_Scan();
	void RGV_Delete();

	void Huayan_120_Init();
	void Huayan_120_DB();
	void Huayan_120_Button();
	void Huayan_120_Scan();
	void Huayan_120_Delete();

	void Huayan_121_Init();
	void Huayan_121_DB();
	void Huayan_121_Button();
	void Huayan_121_Scan();
	void Huayan_121_Delete();

	void Zivid_1_Init();
	void Zivid_1_DB();
	void Zivid_1_Button();
	void Zivid_1_Scan();
	void Zivid_1_Delete();

	void Zivid_2_Init();
	void Zivid_2_DB();
	void Zivid_2_Button();
	void Zivid_2_Scan();
	void Zivid_2_Delete();

	void E1R_1_Init();
	void E1R_1_Delete();

	void E1R_2_Init();
	void E1R_2_Delete();

	void RealSense_1_Init();
	void RealSense_1_Delete();

	void RealSense_2_Init();
	void RealSense_2_Delete();

	void Hikvision_Init();
	void Hikvision_Delete();

	void CH10X_Init();
	void CH10X_Scan();
	void CH10X_Delete();

	void Work_Remote_Init();
	void Work_Remote_DB();
	void Work_Remote_Button();
	void Work_Remote_Scan();
	void Work_Remote_Delete();

protected:
	void keyPressEvent(QKeyEvent *event) override;
	void closeEvent(QCloseEvent *event) override;

private:
    Ui::c_Collector_ManageClass ui;
	c_Object m_Object;
	
	bool m_Scan = true;
	QString m_Debug_Path;
	QElapsedTimer m_Time;
	int m_Current_FPS = 0;
	int m_FPS = 0;

	bool m_dataBit = false;
	float m_dataFloat = 0.00f; 
	quint32 m_data32Bits = 0; 
	quint16 m_high16Bits = 0; 
	quint16 m_low16Bits = 0;  
	quint16 m_data16Bits = 0; 
	QString m_dataStr = ""; 

	QString m_Begin_Time;
	QString m_Car_Type;
	QString m_Car_Num;
	QString m_Car_Box;
	QString m_Bogie_Num;
	QString m_Axis_Num;

	QString m_Wheelset_Num_120;
	QString m_Primary_Components_120;
	QString m_Secondary_Components_120;
	QString m_Point_Num_120;

	QString m_Wheelset_Num_121;
	QString m_Primary_Components_121;
	QString m_Secondary_Components_121;
	QString m_Point_Num_121;

	private slots:
	void Write_Communicate_DB(QString key, int value);
	void Write_Communicate_DB(QString key, QString value);	
	void Write_Communicate_DB(QString key, double value);

	void Write_Prec_Scan_120_Cmd(QString value);//左精扫采集信息返回
	void Write_Prec_Scan_121_Cmd(QString value);//右精扫采集信息返回

	void Write_Worry_List(QString value);

	void Write_Work_List(QString value);

	void Write_RGV_List(QString value);
};
