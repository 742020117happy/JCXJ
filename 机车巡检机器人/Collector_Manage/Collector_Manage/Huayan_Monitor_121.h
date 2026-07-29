#pragma once
#include "Variable.h"
#include "Huayan_Monitor.h"

class c_Huayan_Monitor_121 : public c_Huayan_Monitor
{
    Q_OBJECT
public:
    explicit c_Huayan_Monitor_121(QObject *parent = nullptr);
    virtual ~c_Huayan_Monitor_121();
    
public slots:
    virtual void Connect();         // 连接 DataSheet 服务 (默认 10005 端口，10Hz)
    virtual void Connect_Done();    // 连接完成
    virtual void Disconnect_Done(); // 断开连接完成
    virtual void CopyParsedDataToGlobal();  // 拷贝解析数据到全局（子类实现）
};