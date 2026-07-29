#pragma once
#include "Variable.h"
#include "TCP_Client.h"

class c_Huayan_Monitor : public c_Object
{
    Q_OBJECT
public:
    explicit c_Huayan_Monitor(QObject *parent = nullptr);
    virtual ~c_Huayan_Monitor();
    
    s_Datasheet_Data m_Datasheet_Data;

public slots:
    void Init();// 初始化 Monitor
    void Disconnect();// 断开连接
    virtual void Connect() = 0;// 连接 DataSheet 服务 (默认 10005 端口，10Hz)
    virtual void Connect_Done() = 0;// 连接完成
    virtual void Disconnect_Done() = 0;// 断开连接完成
    virtual void CopyParsedDataToGlobal() = 0;// 拷贝解析数据到全局（子类实现）
    
signals:
    void Connect_Device(QString ip, int port);// 连接 DataSheet 服务
    void Disconnect_Device();// 断开连接
    void Datasheet_Updated();// 数据更新通知信号

private slots:
    void Read_Byte_Done(QByteArray buffer);
    void Parse_Datasheet(const QJsonObject& json);
    void Parse_PosAndVel(const QJsonObject& json);
    void Parse_EndIO(const QJsonObject& json);
    void Parse_ElectricBoxIO(const QJsonObject& json);
    void Parse_ElectricBoxAnalogIO(const QJsonObject& json);
    void Parse_StateAndError(const QJsonObject& json);
    void Parse_FTData(const QJsonObject& json);
    void Parse_HardLoad(const QJsonObject& json);
    void Parse_Modbus(const QJsonObject& json);
    void Parse_RobotAuthorization(const QJsonObject& json);
    void Parse_SafePlane(const QJsonObject& json);
    void Parse_ConstraintArea(const QJsonObject& json);
    void Parse_Script(const QJsonObject& json);
    void Parse_MsgTitle(const QJsonObject& json);
    
private:
    c_TCP_Client* m_Client;
    QThread* m_Thread;
};