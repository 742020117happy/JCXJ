#include "Huayan_Remote_121.h"

c_Huayan_Remote_121::c_Huayan_Remote_121(QObject *parent) : c_Huayan_Remote(parent)
{
}

c_Huayan_Remote_121::~c_Huayan_Remote_121()
{
   
}

void c_Huayan_Remote_121::Connect()
{
    
    QString ip = c_Variable::getInstance().g_Communicate_DB.value("Huayan_121_IP").toString();
    int port = c_Variable::getInstance().g_Communicate_DB.value("Huayan_121_RemotePort").toInt();
    emit Connect_Device(ip, port);
}

void c_Huayan_Remote_121::Connect_Done()
{
    c_Variable::getInstance().g_Huayan_121.Remote_connected = true;
    c_Variable::getInstance().g_Huayan_121.device_Status = c_Variable::getInstance().g_Huayan_121.Monitor_connected 
        &&c_Variable::getInstance().g_Huayan_121.Remote_connected;
}

void c_Huayan_Remote_121::Disconnect_Done()
{
    c_Variable::getInstance().g_Huayan_121.Remote_connected = false;
    c_Variable::getInstance().g_Huayan_121.device_Status = c_Variable::getInstance().g_Huayan_121.Monitor_connected 
        &&c_Variable::getInstance().g_Huayan_121.Remote_connected;
}

void c_Huayan_Remote_121::AutoEn()
{
   if(AutoEn_Count > 3){
        emit Status("连续3次复位失败，使能不成功，请检查");
        AutoEn_Count = 0;
        is_AutoEn = false;
        return;
    }

    if(is_AutoDn){
        emit Status("正在执行自动下电，不能自动使能");
        return;
    } 
    is_AutoEn = true;
    switch (c_Variable::getInstance().g_Datasheet_121.robotState)
    {
    case 6:
        break;  
    case 7:
        Electrify();//上电
        break;
    case 8:
        break;
    case 14:
        StartMaster();//激活主站
        break;  
    case 15:
        break;   
    case 19:
        break;   
    case 23:
        break;  
    case 24:
        GrpEnable();//直接使能
        break; 
    case 33:
        emit GrpEnable_Done();//使能完成
        AutoEn_Count = 0;
        is_AutoEn = false;
        return;
    default:
        AutoEn_Count = AutoEn_Count + 1;
        GrpReset();//复位
        break;
    }
    //500ms递归状态
    QTimer::singleShot(500, this, &c_Huayan_Remote_121::AutoEn);
}

void c_Huayan_Remote_121::AutoDn()
{
    if(AutoDn_Count > 3){
        emit Status("连续3次复位失败，下电不成功，请检查");
        AutoDn_Count = 0;
        is_AutoDn = false;
        return;
    }
    
    if(is_AutoEn){
        emit Status("正在执行自动使能，不能自动下电");
        return;
    } 

    is_AutoDn = true;
    switch (c_Variable::getInstance().g_Datasheet_121.robotState)
    {  
    case 6:  //正在切断本体供电
        break; 
    case 7:  //本体供电已切断
        emit BlackOut_Done();  
        AutoDn_Count = 0;
        is_AutoDn = false;
        return;
    case 8://正在准备给本体供电
        break;
    case 13: //正在反初始化控制器
        break;
    case 14: //控制器已处于未初始化状态
        BlackOut();//下电
        break;
    case 15://正在初始化控制器
        break;   
    case 19://正在复位机器人
        break;   
    case 23://机器人使能中
        break;  
    case 24: //机器人去使能
        CloseMaster();//断开控制柜
        break;
    case 28://机器人去使能中
        break;
    case 33://机器人就绪
        GrpDisable();//下使能
        break;
    default:
        AutoDn_Count++;
        GrpReset(); //复位
        break;
    }
    QTimer::singleShot(500, this, &c_Huayan_Remote_121::AutoDn);
}     
