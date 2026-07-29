#include "Variable.h"


c_Object::c_Object(QObject* parent) : QObject(parent)
{

}
c_Object::~c_Object()
{
   
}
bool c_Object::msleep(const int mSec)
{
    QEventLoop loop;
    QTimer::singleShot(mSec, &loop, &QEventLoop::quit);
    loop.exec();
    return true;
}
bool c_Object::QtPing(const QString ip)
{
    QProcess cmd;
    QStringList arguments;
    
    // 1. 参数配置
    // -c 1: 只发 1 个包
    // -W 2: 超时 2 秒 (防止网络不通时挂起太久)
    // -s 1: 包大小 (保持你原有的设置)
    arguments << "-c" << "1" << "-W" << "2" << "-s" << "1" << ip;

    // 2. 启动进程 (直接传参，不调用 shell)
    cmd.start("ping", arguments);

    // 3. 等待结束 
    // 总超时 = Ping 超时 * 次数 + 缓冲。这里设为 3000ms 足够覆盖 -W 2
    if (!cmd.waitForFinished(3000)) {
        cmd.kill(); // 如果超时，强制杀死进程
        emit Status(ip + " Ping 超时");
        return false;
    }

    // 4. 获取结果 (使用 exitCode 判断，比解析字符串更可靠)
    int exitCode = cmd.exitCode();

    if (exitCode == 0) {
        emit Status(ip + " Ping 通");
        return true;
    } else {
        emit Status(ip + " Ping 不通");
        return false;
    }
}
QString c_Object::TCP_Status(int State)
{
	switch (State)
	{
	case 0:return "连接被对等方拒绝（或超时）";
	case 1:return "远程主机关闭了连接。请注意，客户端套接字（即此套接字）将在发送远程关闭通知后关闭";
	case 2:return "找不到主机地址";
	case 3:return "套接字操作失败，因为应用程序缺少所需的权限";
	case 4:return "本地系统资源不足（例如，套接字过多）";
	case 5:return "套接字操作超时";
	case 6:return "数据报大于操作系统的限制（可低至8192字节）";
	case 7:return "网络发生错误（例如，网络电缆意外插拔）";
	case 8:return "为QAbstractSocket:：bind（）指定的地址已在使用中，并被设置为独占";
	case 9:return "为QAbstractSocket:：bind（）指定的地址不属于主机";
	case 10:return "本地操作系统不支持请求的套接字操作（例如，缺少IPv6支持）";
	case 11:return "仅由QAbstractSocketEngine使用，上次尝试的操作尚未完成（仍在后台进行）";
	case 12:return "套接字正在使用代理，代理需要身份验证";
	case 13:return "SSL/TLS握手失败，因此连接已关闭（仅在QSslSocket中使用）";
	case 14:return "无法联系代理服务器，因为与该服务器的连接被拒绝";
	case 15:return "与代理服务器的连接意外关闭（在与最终对等方建立连接之前）";
	case 16:return "与代理服务器的连接超时，或者代理服务器在身份验证阶段停止响应。";
	case 17:return "找不到使用setProxy（）设置的代理地址（或应用程序代理）";
	case 18:return "与代理服务器的连接协商失败，因为无法理解来自代理服务器的响应";
	case 19:return "当套接字处于不允许的状态时，尝试了一个操作";
	case 20:return "正在使用的SSL库报告了一个内部错误。这可能是由于库的安装错误或配置错误造成的";
	case 21:return "提供了无效数据（证书、密钥、密码等），其使用导致SSL库中出现错误";
	case 22:return "发生临时错误（例如，操作将被阻塞，套接字为非阻塞）";
	case 23:return "套接字未连接";
	case 24:return "套接字正在执行主机名查找";
	case 25:return "套接字已开始建立连接";
	case 26:return "建立了一个连接";
	case 27:return "套接字绑定到一个地址和端口";
	case 28:return "套接字即将关闭（数据可能仍在等待写入）";
	case 29:return "套接字监听状态（仅供内部使用）";
	case -1:return "发生了一个无法识别的错误";
	default:return "无状态";
	}
}
QString c_Object::Modbus_Status(int State)
{
	switch (State)
	{
	case 0:return "已连接";
	case 1:return "读取操作期间发生错误";
	case 2:return "写入操作期间发生错误";
	case 3:return "尝试打开后端时出错";
	case 4:return "尝试设置配置参数时出错";
	case 5:return "I/O期间发生超时。I/O操作未在给定的时间范围内完成";
	case 6:return "发生Modbus特定协议错误";
	case 7:return "由于设备断开连接，回复被中止";
	case 8:return "发生未知错误";
	case 9:return "设备已断开连接";
	case 10:return "正在连接设备";
	case 11:return "设备正在关闭";
	default:return "无状态";
	}
}
QString c_Object::CanOpen(int State)
{
	switch (State) {
	case 0x0000:return "正常";
	case 0x0001:return "DCBUS 过压";
	case 0x0002:return "DCBUS 欠压";
	case 0x0004:return "电机过流";
	case 0x0008:return "编码器故障";
	case 0x0010:return "控制电源欠压";
	case 0x0020:return "驱动过热";
	case 0x0040:return "电机过热";
	case 0x0080:return "电机过载报警";
	case 0x0100:return "霍尔信号异常";
	case 0x0200:return "编码器短线故障";
	case 0x0400:return "电机过速";
	case 0x0800:return "指令超差错误";
	case 0x1000:return "存储参数校验错误";
	case 0x2000:return "功率模块过流（硬件）";
	case 0x4000:return "电机制动过载错误";
	case 0x8001:return "电机自学习错误";
	case 0x8002:return "电流检测基准错误";
	case 0x8004:return "电机缺相报警";
	case 0x8005:return "通讯超时";
	default:return "未知错误";
	}
}
QString c_Object::Hikvision_Status(DWORD state)
{
    switch (state){
    case EXCEPTION_EXCHANGE:return "用户交互时异常";
    case EXCEPTION_AUDIOEXCHANGE:return "语音对讲异常";
    case EXCEPTION_ALARM:return "报警异常";
    case EXCEPTION_PREVIEW:return "网络预览异常";
    case EXCEPTION_SERIAL:return "透明通道异常";
    case EXCEPTION_RECONNECT:return "预览时重连";
    case EXCEPTION_ALARMRECONNECT:return "报警时重连";
    case EXCEPTION_SERIALRECONNECT:return "透明通道重连";
    case SERIAL_RECONNECTSUCCESS:return "透明通道重连成功";
    case EXCEPTION_PLAYBACK:return "回放异常";
    case EXCEPTION_DISKFMT:return "硬盘格式化";
    case EXCEPTION_PASSIVEDECODE:return "被动解码异常";
    case EXCEPTION_EMAILTEST:return "邮件测试异常";
    case EXCEPTION_BACKUP:return "备份异常";
    case PREVIEW_RECONNECTSUCCESS:return "预览时重连成功";
    case ALARM_RECONNECTSUCCESS:return "报警时重连成功";
    case RESUME_EXCHANGE:return "用户交互恢复";
    case NETWORK_FLOWTEST_EXCEPTION:return "网络流量检测异常";
    case EXCEPTION_PICPREVIEWRECONNECT:return "图片预览重连";
    case PICPREVIEW_RECONNECTSUCCESS:return "图片预览重连成功";
    case EXCEPTION_PICPREVIEW:return "图片预览异常";
    case EXCEPTION_MAX_ALARM_INFO:return "报警信息缓存已达上限";
    case EXCEPTION_LOST_ALARM:return "报警丢失";
    case EXCEPTION_PASSIVETRANSRECONNECT:return "被动转码重连";
    case PASSIVETRANS_RECONNECTSUCCESS:return "被动转码重连成功";
    case EXCEPTION_PASSIVETRANS:return "被动转码异常";
    case SUCCESS_PUSHDEVLOGON:return "推模式设备注册成功";
    case EXCEPTION_RELOGIN:return "用户重登陆";
    case RELOGIN_SUCCESS:return "用户重登陆成功";
    case EXCEPTION_PASSIVEDECODE_RECONNNECT:return "被动解码重连";
    case EXCEPTION_CLUSTER_CS_ARMFAILED:return "集群报警异常";
    case EXCEPTION_RELOGIN_FAILED:return "重登陆失败，停止重登陆";
    case EXCEPTION_PREVIEW_RECONNECT_CLOSED:return "关闭预览重连功能";
    case EXCEPTION_ALARM_RECONNECT_CLOSED:return "关闭报警重连功能";
    case EXCEPTION_SERIAL_RECONNECT_CLOSED:return "关闭透明通道重连功能";
    case EXCEPTION_PIC_RECONNECT_CLOSED:return "关闭回显重连功能";
    case EXCEPTION_PASSIVE_DECODE_RECONNECT_CLOSED:return "关闭被动解码重连功能";
    case EXCEPTION_PASSIVE_TRANS_RECONNECT_CLOSED:return "关闭被动转码重连功能 ";
    case EXCEPTION_VIDEO_DOWNLOAD:return "录像下载异常";
    default:return "未知异常";
    }
}
QString c_Object::Huayan_Status(int State)
{
	switch (State) {
    // ===== 初始化与电箱连接阶段 =====
    case 0:  return "未初始化 ";
    case 1:  return "初始化中 ";
    case 2:  return "与电箱控制板断开";
    case 3:  return "连接电箱控制板中";
    
    // ===== 供电与主站阶段 =====
    case 4:  return "急停处理中 ";
    case 5:  return "急停 ";
    case 6:  return "正在切断本体供电";
    case 7:  return "本体供电已切断";
    case 8:  return "正在准备给本体供电";
    
    // ===== 安全光幕阶段 =====
    case 9:  return "安全光幕错误处理中";
    case 10: return "安全光幕错误";
    case 11: return "安全光幕处理中 ";
    case 12: return "安全光幕触发";
    
    // ===== 控制器初始化与检查阶段 =====
    case 13: return "正在反初始化控制器";
    case 14: return "控制器已处于未初始化状态";
    case 15: return "正在初始化控制器";
    case 16: return "控制器版本过低错误";
    case 17: return "EtherCAT总线错误";
    case 18: return "控制器初始化后检查";
    
    // ===== 错误与复位阶段 =====
    case 19: return "正在复位机器人";
    case 20: return "机器人超出安全空间";
    case 21: return "机器人安全碰撞停车";
    case 22: return "机器人错误";
    
    // ===== 使能与运动阶段 =====
    case 23: return "机器人使能中";
    case 24: return "机器人去使能";
    case 25: return "机器人运动中";
    case 26: return "机器人长点动运动中";
    case 27: return "机器人停止运动中";
    case 28: return "机器人去使能中";
    
    // ===== 零力示教阶段 =====
    case 29: return "正在开启零力示教";
    case 30: return "正在关闭零力示教";
    case 31: return "处于零力示教";
    
    // ===== 就绪与脚本阶段 =====
    case 32: return "机器人暂停";
    case 33: return "机器人就绪/已使能"; // 🎯 自动使能的目标状态
    case 34: return "脚本运行中";
    case 35: return "脚本暂停处理中";
    case 36: return "脚本暂停";
    case 37: return "脚本停止中";
    case 38: return "脚本已停止";
    
    // ===== 扩展部件与辨识阶段 =====
    case 39: return "HRApp部件断开";
    case 40: return "HRApp部件错误";
    case 41: return "负载辨识中";
    case 42: return "开关抱闸中";
    
    // ===== 兜底 =====
    default: return QString("未知状态码 (%1)").arg(State);
    }
}

void c_Object::BitToFloat(float& floatValue, quint16 first16Bits, quint16 second16Bits)
{
	dataConverter unionConverter;
	unionConverter.Word.high16Bits = second16Bits;
	unionConverter.Word.low16Bits = first16Bits;
	// 返回浮点数
	floatValue = unionConverter.floatValue;
}
void c_Object::FloatToBit(float floatValue, quint16& first16Bits, quint16& second16Bits)
{
	// 使用联合来存储浮点数和其对应的32位整数表示
	dataConverter unionConverter;
	unionConverter.floatValue = floatValue;
	// 提取两个16位值的位信息
	second16Bits = unionConverter.Word.high16Bits;
	first16Bits = unionConverter.Word.low16Bits;
}
void c_Object::IntToBit(quint32 intValue, quint16& high16Bits, quint16& low16Bits) {
	// 使用位操作将32位整数拆分为两个16位整数
	dataConverter unionConverter;
	unionConverter.DWord = intValue;
	// 提取两个16位值的位信息
	high16Bits = unionConverter.Word.high16Bits;
	low16Bits = unionConverter.Word.low16Bits;
}
void c_Object::BitToInt(quint32& intValue, quint16 high16Bits, quint16 low16Bits)
{
	// 使用位操作将32位整数拆分为两个16位整数
	dataConverter unionConverter;
	unionConverter.Word.high16Bits = high16Bits;
	unionConverter.Word.low16Bits = low16Bits;
	intValue = unionConverter.DWord;
}


QMutex c_Variable::g_mutex;
QScopedPointer<c_Variable> c_Variable::g_instance;

QJsonObject c_Variable::g_Communicate_DB;
QJsonArray c_Variable::g_Worry_List;
QJsonArray c_Variable::g_Work_List;
s_Work_DB  c_Variable::g_Work;
s_Huayan_DB c_Variable::g_Huayan_120;
s_Huayan_DB c_Variable::g_Huayan_121;
s_Scan_DB c_Variable::g_Prec_Scan_120;
s_Scan_DB c_Variable::g_Prec_Scan_121;
s_Datasheet_Data c_Variable::g_Datasheet_120;
s_Datasheet_Data c_Variable::g_Datasheet_121;
s_RGV_DB c_Variable::g_RGV;
s_Robot_DB c_Variable::g_Local_Remote;
s_HIPNUC_CH10X_DB c_Variable::g_HIPNUC_CH10X;
s_IMU_Odom c_Variable::g_IMU_Odom;

c_Variable::c_Variable(QObject * parent) : QObject(parent)
{
	
}

c_Variable::~c_Variable()
{
	
}

c_Variable& c_Variable::getInstance()
{
	if (g_instance.isNull()) {
		QMutexLocker locker(&g_mutex);
		if (g_instance.isNull()) {
			g_instance.reset(new c_Variable());
		}
	}
	return *g_instance.data();
}

