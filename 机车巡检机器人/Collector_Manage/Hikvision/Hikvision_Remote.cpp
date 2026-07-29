#include "Hikvision_Remote.h"

/*************************************************************************************************************************************************
**Function:构造函数
*************************************************************************************************************************************************/
c_Hikvision_Remote::c_Hikvision_Remote(QObject * parent) : c_Object(parent) {
	
}
/*************************************************************************************************************************************************
**Function:析构指针
*************************************************************************************************************************************************/
c_Hikvision_Remote::~c_Hikvision_Remote() {
	//关闭预览
    NET_DVR_StopRealPlay(c_Variable::getInstance().g_Hikvision_DB.lRealPlayHandle);
	//注销用户
    NET_DVR_Logout(c_Variable::getInstance().g_Hikvision_DB.lUserID);
	//释放 SDK 资源
	NET_DVR_Cleanup();
}
/*************************************************************************************************************************************************
**Function:连接设备
*************************************************************************************************************************************************/
void c_Hikvision_Remote::Connect()
{
    if (c_Variable::getInstance().g_Hikvision_DB.Connected) {
		return;
	}
    if(!QtPing(c_Variable::getInstance().g_Hikvision_DB.Device_Ip)){
        c_Variable::getInstance().g_Hikvision_DB.Connected = false;
        emit Status("监控相机未上电");
        QTimer::singleShot(6000, this, &c_Hikvision_Remote::Connect);
        return;
    }
	//注册设备登录参数，包括设备地址、登录用户、密码等
    NET_DVR_DEVICEINFO_V30 struDeviceInfoV30;
    NET_DVR_USER_LOGIN_INFO struLoginInfo;

    // 初始化结构体
    std::memset(&struLoginInfo, 0, sizeof(struLoginInfo));
    struLoginInfo.bUseAsynLogin = 0; // 同步登录方式

    // 安全地设置设备 IP 地址
    QByteArray deviceIp = c_Variable::getInstance().g_Hikvision_DB.Device_Ip.toLatin1();
    std::snprintf(struLoginInfo.sDeviceAddress, sizeof(struLoginInfo.sDeviceAddress), "%s", deviceIp.constData());

    // 设置设备服务端口
    struLoginInfo.wPort = c_Variable::getInstance().g_Hikvision_DB.Device_port;

    // 安全地设置设备登录用户名
    QByteArray userName = c_Variable::getInstance().g_Hikvision_DB.name.toLatin1();
    std::snprintf(struLoginInfo.sUserName, sizeof(struLoginInfo.sUserName), "%s", userName.constData());

    // 安全地设置设备登录密码
    QByteArray password = c_Variable::getInstance().g_Hikvision_DB.key.toLatin1();
    std::snprintf(struLoginInfo.sPassword, sizeof(struLoginInfo.sPassword), "%s", password.constData());
    //初始化和线程绑定

    if (!NET_DVR_Init()) {
        emit Status("监控相机SDK 初始化失败！\n");
        return;
    }
	//调试时使用，不需要可以不注册（可选）
    NET_DVR_SetExceptionCallBack_V30(0, nullptr, c_Variable::getInstance().ExceptionCallBack, nullptr);
	//设备信息, 输出参数
    c_Variable::getInstance().g_Hikvision_DB.lUserID = NET_DVR_Login_V30(
		struLoginInfo.sDeviceAddress,
		struLoginInfo.wPort,
		struLoginInfo.sUserName,
		struLoginInfo.sPassword,
		&struDeviceInfoV30);
    if ( c_Variable::getInstance().g_Hikvision_DB.lUserID < 0) {
        emit Status("监控相机地址:" + QString(struLoginInfo.sDeviceAddress));
        emit Status("监控相机端口:"+ QString::number(struLoginInfo.wPort));
        emit Status("登录用户名:"+ QString(struLoginInfo.sUserName));
        emit Status("登录用密码:"+ QString(struLoginInfo.sPassword));
        emit Status("监控相机：登陆错误代码:" + QString::number(NET_DVR_GetLastError()));
        QTimer::singleShot(6000, this, &c_Hikvision_Remote::Connect);
    }
	else {
		NET_DVR_CLIENTINFO ClientInfo;
		ClientInfo.lChannel = 1; //Channel number 设备通道号
        ClientInfo.hPlayWnd = 0;  //窗口为空，设备SDK不解码只取流
		ClientInfo.lLinkMode = 0;    //Main Stream
        ClientInfo.sMultiCastIP = nullptr;
        c_Variable::getInstance().g_Hikvision_DB.lRealPlayHandle = NET_DVR_RealPlay_V30(c_Variable::getInstance().g_Hikvision_DB.lUserID, &ClientInfo, nullptr, nullptr, 0);
        if (c_Variable::getInstance().g_Hikvision_DB.lRealPlayHandle < 0)
		{
            emit Status("监控相机：实时预览错误代码:" + QString::number(NET_DVR_GetLastError()));
            NET_DVR_Logout(c_Variable::getInstance().g_Hikvision_DB.lUserID);
			Disconnect();
			return;
		}
		//回调解码
        if (!NET_DVR_SetRealDataCallBack(c_Variable::getInstance().g_Hikvision_DB.lRealPlayHandle, c_Variable::getInstance().RealDataCallBack_V30, 0)) {
            emit Status("监控相机：回调函数注册失败");
			Disconnect();
			return;
		}
		else {
            c_Variable::getInstance().g_Hikvision_DB.Connected = true;
            emit Status("监控相机：连接成功");
		}
	}
}
void c_Hikvision_Remote::Disconnect()
{
    NET_DVR_StopRealPlay(c_Variable::getInstance().g_Hikvision_DB.lRealPlayHandle);
    NET_DVR_Logout(c_Variable::getInstance().g_Hikvision_DB.lUserID);
	//释放 SDK 资源
	NET_DVR_Cleanup();
    c_Variable::getInstance().g_Hikvision_DB.lRealPlayHandle = -1;
    c_Variable::getInstance().g_Hikvision_DB.lUserID = -1;
    c_Variable::getInstance().g_Hikvision_DB.Connected = false;
    emit Status("监控相机：断开连接成功");
}

