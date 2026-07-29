#include "Huayan_Monitor_121.h"

c_Huayan_Monitor_121::c_Huayan_Monitor_121(QObject *parent) : c_Huayan_Monitor(parent)
{ 
   
}

c_Huayan_Monitor_121::~c_Huayan_Monitor_121()
{
    
}

void c_Huayan_Monitor_121::Connect()
{
    
    QString ip = c_Variable::getInstance().g_Communicate_DB.value("Huayan_121_IP").toString();
    int port = c_Variable::getInstance().g_Communicate_DB.value("Huayan_121_MonitorPort").toInt();
    emit Connect_Device(ip, port);
}



void c_Huayan_Monitor_121::Connect_Done()
{
    c_Variable::getInstance().g_Huayan_121.Monitor_connected = true;
    c_Variable::getInstance().g_Huayan_121.device_Status = c_Variable::getInstance().g_Huayan_121.Monitor_connected 
        &&c_Variable::getInstance().g_Huayan_121.Remote_connected;
}

void c_Huayan_Monitor_121::Disconnect_Done()
{
    c_Variable::getInstance().g_Huayan_121.Monitor_connected = false;
    c_Variable::getInstance().g_Huayan_121.device_Status = c_Variable::getInstance().g_Huayan_121.Monitor_connected 
        &&c_Variable::getInstance().g_Huayan_121.Remote_connected;
}


void c_Huayan_Monitor_121::CopyParsedDataToGlobal()
{
    c_Variable::getInstance().g_Datasheet_121 = m_Datasheet_Data;
    c_Variable::getInstance().g_Huayan_121.Tran = !c_Variable::getInstance().g_Huayan_121.Tran;
}

