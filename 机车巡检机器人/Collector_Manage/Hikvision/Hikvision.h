#pragma once
#include "Hikvision_Remote.h"
#include "ui_Hikvision.h"

class c_Hikvision : public QWidget
{
    Q_OBJECT

public:
    explicit c_Hikvision(QStringList info, QWidget* parent = nullptr);
	virtual ~c_Hikvision() override;


protected:
    void closeEvent(QCloseEvent* event) override;  
    void keyPressEvent(QKeyEvent* event) override; 
    void showEvent(QShowEvent* event) override;

private slots:
    void System_Scan();

    void Camera_Init(); 
    void Camera_DB();    
    void Camera_Scan();  
    void Camera_Delete();


    void readMessage(QString cmd);
   
    void Write_Communicate_DB(QString key, int value);
    void Write_Communicate_DB(QString key, QString value);
    void Write_Communicate_DB(QString key, double value);

private:
    Ui_u_Hikvision ui;

    bool m_Scan = false;  
    bool m_Initialized = false;
    QString m_DB_Path = "C:/ZividDebugLogs";    

    c_Hikvision_Remote *m_Hikvision_Remote;
    QThread *m_Hikvision_Remote_Thread;

    QFile m_file;
    QSocketNotifier *m_pNotifier;
    char  m_arrRecv[128];
};

