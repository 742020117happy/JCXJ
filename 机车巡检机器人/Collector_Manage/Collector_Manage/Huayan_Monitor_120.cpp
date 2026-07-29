#include "Huayan_Monitor_120.h"

c_Huayan_Monitor_120::c_Huayan_Monitor_120(QObject *parent) : c_Huayan_Monitor(parent)
{ 
   
}

c_Huayan_Monitor_120::~c_Huayan_Monitor_120()
{
    
}

void c_Huayan_Monitor_120::Connect()
{
    
    QString ip = c_Variable::getInstance().g_Communicate_DB.value("Huayan_120_IP").toString();
    int port = c_Variable::getInstance().g_Communicate_DB.value("Huayan_120_MonitorPort").toInt();
    emit Connect_Device(ip, port);
}



void c_Huayan_Monitor_120::Connect_Done()
{
    c_Variable::getInstance().g_Huayan_120.Monitor_connected = true;
    c_Variable::getInstance().g_Huayan_120.device_Status = c_Variable::getInstance().g_Huayan_120.Monitor_connected 
        &&c_Variable::getInstance().g_Huayan_120.Remote_connected;
}

void c_Huayan_Monitor_120::Disconnect_Done()
{
    c_Variable::getInstance().g_Huayan_120.Monitor_connected = false;
    c_Variable::getInstance().g_Huayan_120.device_Status = c_Variable::getInstance().g_Huayan_120.Monitor_connected 
        &&c_Variable::getInstance().g_Huayan_120.Remote_connected;
}


void c_Huayan_Monitor_120::CopyParsedDataToGlobal()
{
    c_Variable::getInstance().g_Datasheet_120 = m_Datasheet_Data;
    c_Variable::getInstance().g_Huayan_120.Tran = !c_Variable::getInstance().g_Huayan_120.Tran;
}

