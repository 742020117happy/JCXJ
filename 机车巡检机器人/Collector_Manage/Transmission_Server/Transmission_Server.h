#pragma once
#include "ui_Transmission_Server.h"
#include "TCP_Server.h"
#include "Fr_Light.h"

// 传输服务器界面类
class c_Transmission_Server : public QMainWindow
{
    Q_OBJECT
public:
    explicit c_Transmission_Server(QWidget *parent = Q_NULLPTR);
    virtual ~c_Transmission_Server();

public slots:
    void TCP_Server_Init();      // TCP服务器初始化
    void TCP_Server_DB();        // TCP服务器数据库操作
    void TCP_Server_Scan();      // TCP服务器扫描
    void TCP_Server_Delete();    // TCP服务器删除

signals:
    void Start_Server_Command();   // 启动服务器命令信号
    void Stop_Server_Command();    // 停止服务器命令信号

protected:
    void keyPressEvent(QKeyEvent *event) override;   // 键盘按下事件
    void closeEvent(QCloseEvent *event) override;    // 关闭事件

private:
    Ui::c_Transmission_Server_Class ui;    // UI对象
    c_TCP_Server *m_Server_1_Remote;       // 远程服务器实例1
    QThread *m_Server_1_Thread;            // 服务器线程1
    c_TCP_Server* m_Server_2_Remote;       // 远程服务器实例2
    QThread* m_Server_2_Thread;            // 服务器线程2
    bool m_Scan;                           // 扫描标志位

    QString m_logDir;                      // 日志目录

private slots:
    void Write_Communicate_DB(QString key, int value);      // 写通信数据库(int)
    void Write_Communicate_DB(QString key, QString value);  // 写通信数据库(QString)
    void Write_Worry_List(QString value);                   // 写入设备告警信息
};