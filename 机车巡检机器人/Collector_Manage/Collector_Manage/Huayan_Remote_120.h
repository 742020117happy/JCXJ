#pragma once
#include "Variable.h"
#include "Huayan_Remote.h"

class c_Huayan_Remote_120 : public c_Huayan_Remote
{
    Q_OBJECT
public:
    explicit c_Huayan_Remote_120(QObject *parent = nullptr);
    virtual ~c_Huayan_Remote_120();

public slots:
    virtual void Connect();         // 连接 IF 服务 (默认 10003 端口)
    virtual void Connect_Done();    // 连接完成
    virtual void Disconnect_Done(); // 断开连接完成
    virtual void AutoEn();          // 自动使能
    virtual void AutoDn();          // 自动下电
};