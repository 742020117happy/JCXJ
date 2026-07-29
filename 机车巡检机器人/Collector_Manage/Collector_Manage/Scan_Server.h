#pragma once
#include <Variable.h>

class c_Scan_Server : public c_Object
{
    Q_OBJECT

public:
    explicit c_Scan_Server(QObject *parent = nullptr);
    virtual ~c_Scan_Server();

    QTcpServer* m_Server = nullptr;     // TCP 服务端
    QTcpSocket* m_ClientSocket = nullptr; // 一对一：仅维护一个客户端

    QString m_camera_num;
    QString m_save_path;
    QString m_photo_name;

public slots:
    void Init();// 初始化 TCP Server
    void Disconnect_Device();// 断开所有连接并清理
    void Write_String(QString str); // 向客户端发送字符串

    void updateCompleted(); // 更新保存路径完成回调
    void CaptureCompleted(); // 相机采集完成回调

	void Start();//准备
	void Collection();//采集
    
public slots:
    virtual void Connect_Device();// 启动监听
    virtual void Tran_Start();
    virtual void Tran_Photo(QStringList value);
    virtual void Tran_Finish();
    virtual void Tran_Connect_Done();
    virtual void Tran_Disconnect_Done();

signals:
    void Listen_Done(bool state);// 监听成功
    void Dislisten_Done(bool state);// 监听成功

    void Connect_Done();// 客户端连接成功
    void Disconnect_Done();// 客户端断开
    void Read_Done(QString value);  // 收到完整指令（调试用）
    void updateSaveInfo(QString save_path);// 更新保存路径
    void Capture(QString image_name);// 触发主界面“捕获”按钮点击
    void Write_String_Done(QString value); // 发送成功
    void Write_String_Error();// 发送失败
    void Write_Prec_Scan_Cmd(QString value); 
    void updateCollectTime(QString image_name);

private slots:
    void New_Connection();// 新客户端连接
    void On_Socket_ReadyRead();// 接收数据
    void On_Socket_Disconnected();// 客户端断开
   
};