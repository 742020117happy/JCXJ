#pragma once
#include <Scan_Server.h>

class c_Scan_Server_121 : public c_Scan_Server
{
    Q_OBJECT

public:
    explicit c_Scan_Server_121(QObject *parent = nullptr);
    virtual ~c_Scan_Server_121();

public slots:
    virtual void Connect_Device();
    
public slots:
    virtual void Tran_Start();
    virtual void Tran_Photo(QStringList value);
    virtual void Tran_Finish();
    virtual void Tran_Connect_Done();
    virtual void Tran_Disconnect_Done();

};