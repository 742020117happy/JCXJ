#include "Scan_Server_121.h"

// 构造函数
c_Scan_Server_121::c_Scan_Server_121(QObject *parent) : c_Scan_Server(parent)
{
    m_camera_num = "T2";
    m_photo_name = QString("N-%1-%2-%3%4-%5-%6-%7-%8-%9-%10-%11-%12")
        .arg(m_camera_num)//相机编号
        .arg(c_Variable::getInstance().g_Work.Begin_Time.left(8))//年月日
        .arg(c_Variable::getInstance().g_Work.Car_Type)//车型
        .arg(c_Variable::getInstance().g_Work.Car_Num)//车号
        .arg(c_Variable::getInstance().g_Work.Head_Toward)//进车方向
        .arg(c_Variable::getInstance().g_Work.Carbox_Num)//车箱号
        .arg(c_Variable::getInstance().g_Work.Bogie_Num)//转向架号
        .arg(c_Variable::getInstance().g_Work.Axis_Num)//车轴号
        .arg(c_Variable::getInstance().g_Work.Wheelset_Num_121)//部件位号
        .arg(c_Variable::getInstance().g_Work.Primary_Components_121)//一级部件编号
        .arg(c_Variable::getInstance().g_Work.Secondary_Components_121)//二级部件编号
        .arg(c_Variable::getInstance().g_Work.Point_Num_121 );//巡检点号
    m_save_path = QString("%1/%2/%3/%4")
            .arg(c_Variable::getInstance().g_Work.Track_Id)
            .arg(c_Variable::getInstance().g_Work.Begin_Time.left(8))//年月日
            .arg(c_Variable::getInstance().g_Work.Begin_Time.left(4))//时分
            .arg(c_Variable::getInstance().g_Work.Work_Num);//任务编号
}
// 析构函数：安全释放资源
c_Scan_Server_121::~c_Scan_Server_121()
{

}

void c_Scan_Server_121::Connect_Device()
{
    QString ip = c_Variable::getInstance().g_Communicate_DB.value("Prec_Scan_121_Local_Ip").toString();
    quint16 port =c_Variable::getInstance().g_Communicate_DB.value("Prec_Scan_121_Tran_Port").toInt();

	if (m_Server->isListening()) return;

    // 尝试监听
    if (!m_Server->listen(QHostAddress(ip), port)) {
        emit Status(QString("采集端口启动失败，监听: %1:%2").arg(ip).arg(port));
    } else {
        emit Status(QString("采集端口启动成功，监听: %1:%2").arg(ip).arg(port));
        emit Listen_Done(1);
    }
}
//虚函数
void c_Scan_Server_121::Tran_Start()
{
    c_Variable::getInstance().g_Work.Robot_121 = false; 
}
void c_Scan_Server_121::Tran_Photo(QStringList value)
{
    c_Variable::getInstance().g_Work.Robot_121 = false; 
    c_Variable::getInstance().g_Work.Wheelset_Num_121 = value.at(0); // 机械臂121位号
	c_Variable::getInstance().g_Work.Primary_Components_121 = value.at(1); // 机械臂121巡检一级部件
	c_Variable::getInstance().g_Work.Secondary_Components_121 = value.at(2); // 机械臂121巡检二级部件
    c_Variable::getInstance().g_Work.Point_Num_121 = value.at(3).left(value.at(3).length() - 1); // 机械臂121巡检点
    //N-T1-20240423-HXD10001-AB-A-01-01-R-ZDZZ-05-08-2.jpg 
    m_photo_name = QString("N-%1-%2-%3%4-%5-%6-%7-%8-%9-%10-%11-%12")
        .arg(m_camera_num)//相机编号
        .arg(c_Variable::getInstance().g_Work.Begin_Time.left(8))//年月日
        .arg(c_Variable::getInstance().g_Work.Car_Type)//车型
        .arg(c_Variable::getInstance().g_Work.Car_Num)//车号
        .arg("AB")//进车方向
        .arg(c_Variable::getInstance().g_Work.Carbox_Num)//车箱号
        .arg(c_Variable::getInstance().g_Work.Bogie_Num)//转向架号
        .arg(c_Variable::getInstance().g_Work.Axis_Num)//车轴号
        .arg(c_Variable::getInstance().g_Work.Wheelset_Num_121)//部件位号
        .arg(c_Variable::getInstance().g_Work.Primary_Components_121)//一级部件编号
        .arg(c_Variable::getInstance().g_Work.Secondary_Components_121)//二级部件编号
        .arg(c_Variable::getInstance().g_Work.Point_Num_121 );//巡检点号

    emit Write_Prec_Scan_Cmd("c_Scan_Server_121::Tran_Photo:"+m_photo_name); 
}
void c_Scan_Server_121::Tran_Finish()
{
    c_Variable::getInstance().g_Work.Robot_121 = true; 
}
void c_Scan_Server_121::Tran_Connect_Done()
{
     c_Variable::getInstance().g_Prec_Scan_121.Tran_Connected = true;
}
void c_Scan_Server_121::Tran_Disconnect_Done()
{
    c_Variable::getInstance().g_Prec_Scan_121.Tran_Connected = false;
}
