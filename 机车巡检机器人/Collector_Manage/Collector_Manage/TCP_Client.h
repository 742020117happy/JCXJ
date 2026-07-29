#pragma once
#include "Variable.h"

class c_TCP_Client : public QObject
{
    Q_OBJECT
public:
    explicit c_TCP_Client(QObject *parent = nullptr);
    virtual ~c_TCP_Client();
    
public slots:
    void Init();                                    // 子线程初始化
    void Connect_Device(QString ip, int port);      // 连接到服务器
    void Disconnect_Device();                       // 断开连接
    void Write_Json(QJsonObject value);             // 写 Json 数据
    void Write_String(QString str);                 // 写字符串
    
signals:
    void Connect_Done();                            // 连接到服务器完成
    void Disconnect_Done();                         // 断开连接完成
    void Connect_Loop();                            // 循环检测连接状态
    void Read_Byte_Done(QByteArray buffer);         // ⚠️ 原始字节数据（含 LTBR 包头）
    void Read_String_Done(QString str);             // 读字符串完成
    void Read_Json_Done(QJsonObject value);         // ⚠️ 仅纯 JSON 数据时发射
    
    void Write_Json_Done();                         // 写完成
    void Write_String_Done();                       // 写完成
    void Write_Json_Error();                        // 写错误
    void Write_String_Error();                      // 写错误
    void Status(int state);                         //通讯状态

private:
    QTcpSocket *m_Socket;                           // 通讯对象
    QString m_Ip;
    int m_Port;
    bool m_Stop_Connect = false;
    bool m_IsDataSheetPort = false;                 // ⚠️ 新增：标识是否为 DataSheet 端口 (10004-10006)
};