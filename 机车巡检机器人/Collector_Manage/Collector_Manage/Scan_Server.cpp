#include "Scan_Server.h"

// 构造函数
c_Scan_Server::c_Scan_Server(QObject *parent) : c_Object(parent)
{
}
// 析构函数：安全释放资源
c_Scan_Server::~c_Scan_Server()
{
    Disconnect_Device();
    if (m_Server && m_Server->isListening()) {
        m_Server->close();
    }
    if (m_Server) {
        m_Server->deleteLater();
    }
}
// 初始化 Server 对象并绑定信号槽
void c_Scan_Server::Init()
{
    m_Server = new QTcpServer(this);
    // 连接错误处理
	QObject::connect(m_Server, &QTcpServer::acceptError, this, [=](int state){emit Status(TCP_Status(state));});
    // 新连接到来
    QObject::connect(m_Server, &QTcpServer::newConnection,this, &c_Scan_Server::New_Connection);

    QTimer::singleShot(3000, this, &c_Scan_Server::Connect_Device);
}
// 启动 TCP 监听（IP/Port 来自全局配置）
void c_Scan_Server::Connect_Device()
{
    
}
// 断开当前客户端并清理
void c_Scan_Server::Disconnect_Device()
{
    if (m_ClientSocket) {
        m_ClientSocket->disconnectFromHost();
        m_ClientSocket->deleteLater();
        m_ClientSocket = nullptr;
    }
     if (m_Server && m_Server->isListening()) {
        m_Server->close();
    }

    emit Dislisten_Done(1);
}
// 处理新连接（严格一对一）
void c_Scan_Server::New_Connection()
{
    // 若已有活跃连接，直接拒绝新客户端
    if (m_ClientSocket && m_ClientSocket->state() == QTcpSocket::ConnectedState) {
        QTcpSocket* pending = m_Server->nextPendingConnection();
        if (pending) {
            pending->abort();      // 立即断开
            pending->deleteLater();
        }
        return;
    }

    // 接受新连接
    m_ClientSocket = m_Server->nextPendingConnection();
    if (!m_ClientSocket) return;

    emit Connect_Done();
   emit Write_Prec_Scan_Cmd("c_Scan_Server::New_Connection:客户端已连接");

    // 绑定读取与断开信号
    QObject::connect(m_ClientSocket, &QTcpSocket::readyRead,this, &c_Scan_Server::On_Socket_ReadyRead);
    QObject::connect(m_ClientSocket, &QTcpSocket::disconnected,this, &c_Scan_Server::On_Socket_Disconnected);
}
// 接收并处理客户端指令
void c_Scan_Server::On_Socket_ReadyRead()
{
    if (!m_ClientSocket) return;

    // 读取全部数据（外部保证每条指令独立且间隔 >0.5s，无需粘包处理）
    QByteArray buffer = m_ClientSocket->readAll();
    if (buffer.isEmpty()) return;

    QString command = QString::fromUtf8(buffer).trimmed();
    emit Read_Done(command); // 调试日志

	//机械臂发送指令：START!
	//成功创建文件夹或对应文件夹已经存在则返回START&SUCCESSED&0!
    if (command.startsWith("START") && command.endsWith("!")) {
        // 构建保存路径
        c_Variable::getInstance().g_Work.Work_Num = QString("%1%2%3")
            .arg(c_Variable::getInstance().g_Work.Begin_Time)//年月日时分
            .arg(c_Variable::getInstance().g_Work.Car_Type)//车型
            .arg(c_Variable::getInstance().g_Work.Car_Num);//车号

        m_save_path = QString("%1/%2/%3/%4")
            .arg(c_Variable::getInstance().g_Work.Track_Id)
            .arg(c_Variable::getInstance().g_Work.Begin_Time.left(8))//年月日
            .arg(c_Variable::getInstance().g_Work.Begin_Time.left(4))//时分
            .arg(c_Variable::getInstance().g_Work.Work_Num);//任务编号
        emit Write_Prec_Scan_Cmd("c_Scan_Server::On_Socket_ReadyRead:START");
        Tran_Start();
        Start();
    }
	//工控机发送指令：PHOTO&R-ZDZZ-05-08!相机开始采集图像
	//若拍照成功，返回PHOTO&T1&SUCCESSED&0!
    if (command.startsWith("PHOTO&") && command.endsWith("!")) {
        QStringList list = command.split("&", QString::SkipEmptyParts);
	    if (list.size() < 2) {
            return;
        }
	    QStringList value = list.at(1).split("-", QString::SkipEmptyParts);//部件位号-一级部件编号-二级部件编号-巡检点号
        if (value.size() < 4) {
            return;
        }
        emit Write_Prec_Scan_Cmd("c_Scan_Server::On_Socket_ReadyRead:PHOTO");
        Tran_Photo(value);
        Collection();
    }
    if (command.startsWith("FINISH") && command.endsWith("!")) {
        Tran_Finish();
    }
}
// 客户端断开处理
void c_Scan_Server::On_Socket_Disconnected()
{
    if (m_ClientSocket) {
        m_ClientSocket->deleteLater();
        m_ClientSocket = nullptr;
    }
    emit Status("客户端已断开");
    emit Disconnect_Done();
}
// 相机采集完成回调
void c_Scan_Server::updateCompleted()
{
    Write_String("<START&SUCCESSED&0!>");
}
void c_Scan_Server::CaptureCompleted()
{
    emit updateCollectTime(m_photo_name);
    // 此时才向工控机回复采集成功（确保 2D/3D/DepthMap 全部保存完毕）
    Write_String("<PHOTO&" + m_camera_num + "&SUCCESSED&0!>");
}
// 向客户端发送字符串（仅当有连接且处于 Connected 状态）
void c_Scan_Server::Write_String(QString str)
{
    if (str.isEmpty() || !m_ClientSocket) return;
    if (m_ClientSocket->state() != QTcpSocket::ConnectedState) return;

    m_ClientSocket->write(str.toUtf8());
    // 不使用 flush()，依赖 Qt 事件循环异步发送，避免阻塞主线程
    emit Write_String_Done(str);
}
//操作接口
void c_Scan_Server::Start()
{
    emit Write_Prec_Scan_Cmd("c_Scan_Server::Start:" + m_save_path);
    emit updateSaveInfo(m_save_path);   
}
void c_Scan_Server::Collection()
{
     emit Write_Prec_Scan_Cmd("c_Scan_Server::Collection:" + m_photo_name);
     emit Capture(m_photo_name);
}
//虚函数
void c_Scan_Server::Tran_Start()
{
    // c_Variable::getInstance().g_Work.Robot_120 = true; 
}
void c_Scan_Server::Tran_Photo(QStringList value)
{
    /*
    c_Variable::getInstance().g_Work.Robot_120 = false; 
    c_Variable::getInstance().g_Work.Wheelset_Num_120 = value.at(0); // 机械臂120位号
	c_Variable::getInstance().g_Work.Primary_Components_120 = value.at(1); // 机械臂120巡检一级部件
	c_Variable::getInstance().g_Work.Secondary_Components_120 = value.at(2); // 机械臂120巡检二级部件
    c_Variable::getInstance().g_Work.Point_Num_120 = value.at(3).left(value.at(3).length() - 1); // 机械臂120巡检点
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
        .arg(c_Variable::getInstance().g_Work.Wheelset_Num_120)//部件位号
        .arg(c_Variable::getInstance().g_Work.Primary_Components_120)//一级部件编号
        .arg(c_Variable::getInstance().g_Work.Secondary_Components_120)//二级部件编号
        .arg(c_Variable::getInstance().g_Work.Point_Num_120 );//巡检点号
    */
}

void c_Scan_Server::Tran_Finish()
{
    // c_Variable::getInstance().g_Work.Robot_120 = true; 
}
void c_Scan_Server::Tran_Connect_Done()
{
    
}
void c_Scan_Server::Tran_Disconnect_Done()
{

}
