#pragma once 

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <fcntl.h>
#include <memory>

#include <QApplication>
#include <QByteArray>
#include <QCloseEvent>
#include <QColor>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QSize>
#include <QEventLoop>
#include <QFile>
#include <QFileDialog>
#include <QHostAddress>
#include <QImage>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QKeyEvent>
#include <QMainWindow>
#include <QMap>
#include <QMessageBox>
#include <QMetaType>
#include <QMutex>
#include <QObject>
#include <QPainter>
#include <QPainterPath>
#include <QPaintDevice> 
#include <QPen>       
#include <QProcess>
#include <QRegularExpression>
#include <QScopedPointer>
#include <QSettings>
#include <QSharedMemory>
#include <QSocketNotifier>
#include <QStandardPaths>
#include <QStackedWidget>
#include <QString>
#include <QTextStream>
#include <QDataStream>
#include <QThread>
#include <QTimer>
#include <QUuid>
#include <QVariant>
#include <QVector>
#include <QWidget>
#include <QSocketNotifier>

#include <QAbstractSocket>
#include <QNetworkAccessManager>
#include <QNetworkInterface>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket> 
#include <QUrlQuery>
#include <QtNetwork>
#include <QtWebSockets/qwebsocket.h>
#include <QtWebSockets/qwebsocketserver.h>
#include <QConicalGradient>
#include <QPolarChart>
#include <QScatterSeries>
#include <QtCharts>
#include <QModbusDevice>
#include <QModbusTcpClient>
#include <QModbusDataUnit>

#include <QtConcurrent/QtConcurrent>

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);   
VTK_MODULE_INIT(vtkInteractionStyle);  

#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKOpenGLNativeWidget.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/common/common.h>
#include <boost/thread/thread.hpp>
#include <pcl/filters/passthrough.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/crop_box.h>  
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/frustum_culling.h>
#include <pcl/segmentation/sac_segmentation.h> 
#include <pcl/segmentation/extract_clusters.h>
#include <pcl/common/common.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl/PolygonMesh.h>
#include <pcl/search/kdtree.h>  
#include <pcl/surface/convex_hull.h>

#include <boost/thread/thread.hpp>

#include "librealsense2/rs.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <Zivid/Zivid.h>
#include <Zivid/Camera.h>

#include <HCNetSDK.h>
#include <LinuxPlayM4.h>

#include <mysql/mysql.h>

using namespace std;
using namespace pcl;
using namespace rs2;
using namespace cv;
using namespace Zivid;

#if defined(_MSC_VER)
    #define PACKED_STRUCT __pragma(pack(push, 1)) struct
    #define PACKED_END __pragma(pack(pop))
#elif defined(__GNUC__) || defined(__clang__)
    #if defined(__aarch64__) || defined(__arm64__)
        #define PACKED_STRUCT struct __attribute__((packed, aligned(1)))
        #define PACKED_END
    #else
        #define PACKED_STRUCT struct __attribute__((packed))
        #define PACKED_END
    #endif
#else
    #error "Unsupported compiler - Jetson Thor requires GCC/Clang"
#endif

PACKED_STRUCT s_Timestamp {
    /* 0-5字节: 秒计数（6字节大端序） */
    uint8_t seconds[6];
    /* 6-9字节: 微秒计数（4字节大端序） */
    uint8_t microseconds[4];
}PACKED_END;
// MSOP帧头结构体（32字节）
PACKED_STRUCT s_MSOPHeader {
    /* 0-3字节: 同步头（4字节，固定值 0x55AA5AA5） */
    uint32_t sync;
    /* 4-5字节: 包序列号（2字节） */
    uint16_t pktCnt;
    /* 6-7字节: 协议版本号（2字节） */
    uint16_t ver;
    /* 8字节: 回波模式（1字节） */
    uint8_t returnMode;
    /* 9字节: 时间同步模式（1字节） */
    uint8_t timeMode;
    /* 10-19字节: 时间戳 */
    s_Timestamp timesTamp;
    /* 20字节: 帧同步状态（1字节） */
    uint8_t frameSync;
    /* 21-29字节: 预留字段（9字节） */
    uint8_t res0[9];
    /* 30字节: 雷达类型标识（1字节） */
    uint8_t lidarType;
    /* 31字节: 芯片温度原始值（1字节） */
    int8_t lidarTmp;
}PACKED_END;
// MSOP数据块结构体（12字节）
PACKED_STRUCT s_DataBlock {
    /* 0-1字节: 时间偏移量（2字节，无符号） */
    uint16_t timeOffset;
    /* 2-3字节: 径向距离（2字节，无符号） */
    uint16_t radius;
    /* 4-5字节: 单位方向向量X分量（2字节，有符号） */
    int16_t dirVectorX;
    /* 6-7字节: 单位方向向量Y分量（2字节，有符号） */
    int16_t dirVectorY;
    /* 8-9字节: 单位方向向量Z分量（2字节，有符号） */
    int16_t dirVectorZ;
    /* 10字节: 反射强度（1字节，无符号） */
    uint8_t intensity;
    /* 11字节: 点属性（1字节，使用联合体解析） */
    uint8_t pointAttr;
}PACKED_END;
// MSOP帧尾结构体（16字节）
PACKED_STRUCT s_MSOPTrailer {
    /* 0-3字节: 预留字段（4字节） */
    uint8_t res1[4];
    /* 4-5字节: 数据长度（2字节，固定值 0x04B0 = 1200） */
    uint16_t dataLength;
    /* 6-7字节: 计数器（2字节） */
    uint16_t counter;
    /* 8-11字节: 数据ID（4字节，固定值 0x00000E5C） */
    uint32_t dataId;
    /* 12-15字节: CRC32校验码（4字节） */
    uint32_t crc32;
}PACKED_END;
// MSOP完整MSOP包结构体（1200字节定长）
PACKED_STRUCT s_MSOPPacket {
    s_MSOPHeader header;
    s_DataBlock blocks[96];
    s_MSOPTrailer trailer;
}PACKED_END;
// DIFOP帧头结构体 (8字节) - 位于包偏移0~7字节
PACKED_STRUCT s_DIFOPHeader {
    /* [0-3] 主识别头 (4字节) */
    uint32_t sync1;
    /* [4-7] 次识别头 (4字节) */
    uint32_t sync2;
}PACKED_END;
// 网络地址结构体 (辅助类型) - 用于IP/MAC地址封装
PACKED_STRUCT s_IPv4Address {
    uint8_t bytes[4];
}PACKED_END;
PACKED_STRUCT s_MACAddress {
    uint8_t bytes[6];
}PACKED_END;
// 固件版本结构体 (3字节) - 位于包偏移16~18字节
PACKED_STRUCT s_FirmwareVersion {
    uint8_t major;           // 主版本号
    uint8_t minor;           // 次版本号
    uint8_t patch;           // 修订版本号
}PACKED_END;
// 完整DIFOP包结构体 (256字节定长) - 严格对应手册4.3.2节
PACKED_STRUCT s_DIFOPPacket {
    /* [0-7] DIFOP帧头 (8字节) */
    s_DIFOPHeader header;
    /* [8-15] 预留字段 (8字节) */
    uint8_t res0[8];
    /* [16-18] 固件版本号 (3字节) */
    s_FirmwareVersion swVersion;
    /* [19] 预留字段 (1字节) */
    uint8_t res1;
    /* [20-25] 设备序列号 (6字节) - ASCII编码 */
    uint8_t sn[6];              // 例如: "E1R001"
    /* [26-43] 预留字段 (18字节) */
    uint8_t res2[18];
    /* [44-47] 本地IP地址 (4字节) - 雷达本机IP */
    s_IPv4Address localIp;
    /* [48-51] 子网掩码 (4字节) */
    s_IPv4Address netMask;
    /* [52-57] MAC地址 (6字节) */
    s_MACAddress macAddress;
    /* [58-61] MSOP远程IP (4字节) - 上位机IP */
    s_IPv4Address msopRemoteIp;
    /* [62-63] MSOP本地端口 (2字节) - 雷达发送端口 */
    uint16_t msopLocalPort;
    /* [64-65] MSOP远程端口 (2字节) - 上位机接收端口 */
    uint16_t msopRemotePort;
    /* [66-69] DIFOP远程IP (4字节) - 上位机IP */
    s_IPv4Address difopRemoteIp;
    /* [70-71] DIFOP本地端口 (2字节) - 雷达发送端口 */
    uint16_t difopLocalPort;
    /* [72-73] DIFOP远程端口 (2字节) - 上位机接收端口 */
    uint16_t difopRemotePort;
    /* [74-98] 预留字段 (25字节) */
    uint8_t res3[25];
    /* [99] 帧率设置 (1字节) */
    uint8_t frequencySetting; // 0x0A = 10Hz (E1R默认)
    /* [100] 回波模式 (1字节) - 与MSOP协议一致 */
    uint8_t returnMode;       // 0x04 = StrongestWave (默认)
    /* [101] 时间同步模式 (1字节) */
    uint8_t timeSyncMode;     // 0x00=Internal, 0x02=E2E L2, 0x03=gPTP
    /* [102] 时间同步状态 (1字节) */
    uint8_t timeSyncStatus;   // 0x00=failed, 0x01=success, 0x02=timeout
    /* [103-112] 时间状态 (10字节) - 与MSOP Timestamp格式相同 */
    s_Timestamp timeStatus;
    /* [113] 物理层工作模式 (1字节) */
    uint8_t phyMode;          // 0x00=auto, 0x01=master, 0x02=slave
    /* [114-255] 预留字段 (142字节) */
    uint8_t res4[142];
}PACKED_END;

//EIR雷达
struct s_RoboSense_DB {
    bool MSOP_connected = false;
    bool DIFOP_connected = false;
    bool device_Status = false;

    QString device_name;

    QString Local_Ip;
    int Local_MSOP_Port;
    int Local_DIFOP_Port;
    QString E1R_IP;
    int  E1R_MSOP_Port;
    int E1R_DIFOP_Port;

    int8_t lidarTmp;
};
//D555相机
struct s_RealSense_DB {
    bool device_connected = false; //相机连接状态
    bool device_Status = false;
    QString device_name;
    QString serial; //相机序列号

    QString Local_Ip;
    int Local_Port;

    QString Device_IP;
    int  Device_Port;
};
//Zivid相机
struct s_Zivid_Camera_DB {
    bool camera_connected = false;//相机连接状态

    QString device_name;
    QString camera_serial;//相机序列号

    QString server_ip;
    quint16 server_port;

    qint64 settingTime = 0;//捕获耗时
    qint64 captureTime = 0;//捕获耗时
    qint64 save2DTime = 0;//捕获耗时
    qint64 save3DTime = 0;//捕获耗时
  
    QString image_name;//基础路径
    QString save_path;//当前保存路径

    QString system_status;         //系统状态
    QDateTime last_operation_time; //最后操作时间
};
//服务
struct s_Server_DB {
    bool server_connected = false;    //连接状态
    QString camera_num;  //相机编号
    QString track;       // 股道号
    QString date;        // 年月日
    QString time;       // 时分
    QString trainType;  // 车型车号
    QString taskID;     // 任务编号
    QString basePath;   // 保存路径
};
// 服务状态数据库结构
struct s_Transmission_DB {
	bool Tran = false;// 传输状态
	bool Connected = false;// 连接状态
	bool Start = false;// 开始状态
	bool Finish = true;// 完成状态
	bool Cancel = false;// 取消状态
	bool Error = false;
	QString device_name;  // 设备名称
	QString Server_Ip;// 服务器IP
	int Server_Port;// 服务器端口
    QString Client_Ip;
	int Client_Port;
	QString Save_Path;    
    QStringList buffer;
	QStringList Tran_buffer;
	QStringList Retran_buffer;
	QStringList Received_Buffer; 
};
//机械臂
struct s_Huayan_DB {
    bool Remote_connected = false;
    bool Monitor_connected = false;
    bool device_Status = false; 
    bool Tran = false;
};
// ===== 华沿机器人 DataSheet 数据结构 =====
// 依据协议 V1.0.19.1 第五章 DataSheet 服务
struct s_Datasheet_Data {
    // ===== PosAndVel 模块（位置与速度）=====
    double Actual_Position[12];        // 关节位置 (°)，12 个元素（含冗余轴）
    double Actual_PCS_TCP[6];          // TCP 坐标系下笛卡尔坐标 (mm/°)
    double Actual_PCS_Base[6];         // 基座坐标系下笛卡尔坐标 (mm/°)
    double Actual_Joint_Current[6];    // 关节电流 (A)
    double Actual_Joint_Velocity[6];   // 关节速度 (°/s)
    double Actual_Joint_Acceleration[6];// 关节加速度 (°/s²)
    double Actual_Override;            // 速度比 (0~1)
    
    // ===== EndIO 模块（末端 IO）=====
    int EndDI[4];                      // 末端数字输入 (0/1) - 协议示例为 4 个
    int EndDO[3];                      // ⚠️ 修复：协议 V1.0.18.1 明确 EndDO 只支持 3 个
    int EndButton[4];                  // 末端按钮状态 (0/1)
    int EnableEndBTN;                  // 末端按钮使能 (0/1)
    double EndAI[2];                   // 末端模拟输入
    
    // ===== ElectricBoxIO 模块（电箱 IO）=====
    int BoxCI[8];                      // 电箱控制输入 (0/1)
    int BoxCO[8];                      // 电箱控制输出 (0/1)
    int BoxDI[8];                      // 电箱通用输入 (0/1)
    int BoxDO[8];                      // 电箱通用输出 (0/1)
    double Conveyor;                   // 传送带值
    int Encode;                        // 编码器值
    
    // ===== ElectricBoxAnalogIO 模块（电箱模拟 IO）=====
    int BoxAnalogOutMode_1;            // 模拟输出通道 1 模式 (0=关闭，1=电压，2=电流)
    int BoxAnalogOutMode_2;            // 模拟输出通道 2 模式
    double BoxAnalogOut_1;             // 模拟输出通道 1 值
    double BoxAnalogOut_2;             // 模拟输出通道 2 值
    double BoxAnalogIn_1;              // 模拟输入通道 1 值
    double BoxAnalogIn_2;              // 模拟输入通道 2 值
    
    // ===== StateAndError 模块（状态与错误）=====
    int robotState;                    // 状态机
    int robotEnabled;                  // 使能状态 (0=未使能，1=使能)
    int robotPaused;                   // 暂停状态 (0=未暂停，1=暂停)
    int robotMoving;                   // 运动状态 (0=静止，1=运动中)
    int robotBlendingDone;             // 路点运动状态 (0=运动中，1=完成)
    int InPos;                         // 到位状态 (0=未到位，1=到位)
    int Error_AxisID;                  // 错误轴 ID(0~6,0 表示无错误)
    int Error_Code;                    // 错误码
    int IsReduceMode;                  // 减速模式 (0/1)
    int IsFreeDriveMode;               // 零力示教模式状态 (0/1) - V6 新增
    int AutoMode;                      // 自动运行模式 - V6 新增
    int BrakeState[6];                 // 抱闸状态 (0=抱闸，1=松闸)
    int nAxisStatus[6];                // 轴状态
    int nAxisErrorCode[6];             // 轴错误码
    int nResetSafeSpace[1];            // 安全空间复位标志
    int nAxisGroupStatus[1];           // 轴组状态
    int nAxisGroupErrorCode[1];        // 轴组错误码
    
    // ===== FTData 模块（力传感器数据）=====
    int FTControlState;                // 力控状态 (0=关闭，1=开启)
    double FTData[6];                  // 力传感器标定后数据 (N/NM)
    double FTSrcData[6];               // 力传感器原始数据
    
    // ===== HardLoad 模块（硬件负载）=====
    int EtherCAT_TotalFrame;           // EtherCAT 总帧数
    int EtherCAT_FramesPerSecond;      // EtherCAT 每秒帧数
    int EtherCAT_TotalLostFrame;       // EtherCAT 丢失帧数
    int EtherCAT_TxErrorFrame;         // EtherCAT 发送错误帧
    int EtherCAT_RxErrorFrame;         // EtherCAT 接收错误帧
    double Box48IN_Voltage;            // 48V 输入电压 (V)
    double Box48IN_Current;            // 48V 输入电流 (A)
    double Box48Out_Voltage;           // 48V 输出电压 (V)
    double Box48Out_Current;           // 48V 输出电流 (A)
    double joint_temperature[6];       // 关节温度 (°C) - V6 改为 6 个模组
    double joint_Voltage[6];           // 关节电压 (V) - V6 改为 6 个模组
    
    // ===== Modbus 模块（V6 新增）=====
    int Modbus_connectStatus;          // Modbus 从站连接状态 (0/1)
    int Modbus_errorCode;              // Modbus 从站错误码
    
    // ===== RobotAuthorization 模块（机器人授权）=====
    QString DynDeviceCode;             // 动态设备码
    QString AuthorizedTimeLeftMinutes; // 剩余授权时间
    QString AuthorizedTimeUsedMinutes; // 已使用授权时间
    QString LangnDeviceCode;           // 语言授权设备码 - V6 新增
    QString LangAuthStatus;            // 语言授权状态 - V6 新增
    QString LangAuthData;              // 语言授权数据 - V6 新增
    QString DeviceMac;                 // 设备 MAC 地址 - V6 新增
    QString DeviceSN;                  // 设备 SN 号 - V6 新增
    QString SystemDate;                // 系统日期 - V6 新增
    
    // ===== SafePlane 模块（安全平面，V6 新增）=====
    QString SafePlane_Name;            // 安全平面名称
    double SafePlane_Distance;         // 安全平面距离
    int SafePlane_Trendency;           // 运动与安全平面趋势 (0=远离，1=靠近)
    
    // ===== ConstraintArea 模块（安全空间，V6 新增）=====
    QString ConstraintArea_Name;       // 安全空间名称
    double ConstraintArea_Distance;    // 与安全空间距离
    int ConstraintArea_Trendency;      // 运动与安全空间趋势 (0=远离，1=靠近)
    
    // ===== Script 模块（脚本信息）=====
    QString Script_errorCode;          // 脚本错误码
    int ScriptChanged;                 // 脚本变化状态 (0/1) - V6 新增
    QString Script_cmdid[6];           // 脚本命令 ID
    QString Script_callstackcmdid[6];  // ⚠️ 修复：协议示例为数组，非单个 QString
    
    // ===== MsgTitle 模块（消息标题，V6 新增）=====
    QString Stamp;                     // 绝对时间戳（毫秒）
    QString UpdateTime;                // 更新时间（格式化显示）
};

struct s_RGV_DB {
	bool Connected = false;
	bool Ready = false;
	int FPS = 0;
	quint16 Coils[1000]; // 数据缓存区间
	quint16 DiscreteInputs[1000]; // 数据缓存区间
	quint16 InputRegisters[1000]; // 数据缓存区间
	quint16 read_HoldingRegisters[1000]; // 数据缓存区间
	quint16 write_HoldingRegisters[1000]; // 数据缓存区间
};

//DIFOP解析联合体
union s_DIFOPPacketUnion {
    /* 原始字节数组 (256字节) - 用于接收UDP数据 */
    uint8_t raw[256];
    /* 结构化视图 - 与raw共享同一内存区域 */
    s_DIFOPPacket data;
};

// MSOP数据解析联合体
union s_MSOPPacketUnion {
    /* 成员1: 原始字节数组（1200字节）- 用于接收网络数据 */
    uint8_t raw[1200];
    /* 成员2: 结构化视图 - 与raw共享同一块内存 */
    s_MSOPPacket data;
};

//一键巡检
struct s_Work_DB {
	bool Connected = false;
	QString Head_Toward = "AB";
	QString Track_Id = "J11"; // 股道号
	QString Begin_Time = "202407171615"; // 开始时间
	QString Car_Type = "HXD1"; // 任务车型
	QString Car_Num = "0001"; // 任务车号
	QString Work_Num =  "202407171615CR400AFA2195"; // 任务编号
	QString Carbox_Num = "01"; // 车厢号
	QString Bogie_Num = "A"; // 转向架号
	QString Axis_Num = "01"; // 轴号
	QString Work_Stage = "Debuge"; // 巡检工作阶段
	QString Wheelset_Num_120 = "xx"; // 机械臂120轮对号
	QString Wheelset_Num_121 = "xx"; // 机械臂121轮对号
	QString Point_Num_120 = "00"; // 机械臂120巡检点
	QString Point_Num_121 = "O0"; // 机械臂121巡检点
	QString Primary_Components_120 = "ZDJQ"; // 机械臂120巡检一级部件
	QString Secondary_Components_120 = "01"; // 机械臂120巡检二级部件
	QString Primary_Components_121 = "ZDJQ"; // 机械臂121巡检一级部件
	QString Secondary_Components_121 = "01"; // 机械臂121巡检二级部件
	bool Robot_120 = false;
	bool Robot_121 = false;

    QString inspectionTaskNum;
};

// 精扫转发
struct s_Scan_DB {
	bool Remote_Connected = false; // 精扫发送状态
	bool Monitor_Connected = false; // 精扫接收状态
	bool Tran_Connected = false; // 精扫转发状态
};
struct s_Robot_DB {
	bool Connected = false;
	bool Tran = false;
	int num = 0;
};

// CH10X 全局状态数据库
struct s_HIPNUC_CH10X_DB {
    bool connected = false;
    QString device_name = "CH10X_IMU";
    QString port_name = "/dev/ttyUSB0";
    // ✅ 修正：CH104-USB 默认波特率为 115200 (如需 400Hz 高频输出，请先用 AT 指令修改设备波特率，再在此处同步修改)
    int baud_rate = 115200; 
    
    // 实时解析数据
    uint32_t timestamp = 0;
    float acc[3] = {0};
    float gyro[3] = {0};
    float mag[3] = {0};
    float euler[3] = {0}; // [0]=Roll, [1]=Pitch, [2]=Yaw
    float quat[4] = {0};  // [0]=W, [1]=X, [2]=Y, [3]=Z
    
    // 状态统计
    qint64 last_update_time = 0;
    int fps_count = 0;
    int current_fps = 0;
};
struct s_IMU_Odom {
    double x = 0.0;         // 东向位置 (m)
    double y = 0.0;         // 北向位置 (m)
    double vx = 0.0;        // 东向速度 (m/s)
    double vy = 0.0;        // 北向速度 (m/s)
    double yaw = 0.0;       // 航向角 (rad)
    uint32_t timestamp = 0; // 时间戳 (ms)
    bool is_static = false; // 静止状态标志 (供主进程 EKF 调整协方差使用)
};

class c_Object : public QObject
{
    Q_OBJECT
public:
    explicit c_Object(QObject* parent = nullptr);
    virtual ~c_Object();

  	union dataConverter {
		float floatValue;     // 浮点数
		quint32 DWord;
		struct {
			quint16 low16Bits;
			quint16 high16Bits;
		}Word;
	};

    void BitToFloat(float& floatValue, quint16 first16Bits, quint16 second16Bits);
	void FloatToBit(float floatValue, quint16& first16Bits, quint16& second16Bits);
	void IntToBit(quint32 intValue, quint16& high16Bits, quint16& low16Bits);
	void BitToInt(quint32& intValue, quint16 high16Bits, quint16 low16Bits);

    bool QtPing(const QString ip);
    bool msleep(const int mSec);
    QString TCP_Status(int State);
    QString Modbus_Status(int State);
    QString CanOpen(int State);
    QString Hikvision_Status(DWORD state);
    QString Huayan_Status(int State);

signals:
    void Status(QString state);

};


class c_Variable : public QObject
{
	Q_OBJECT
public:
	static c_Variable& getInstance();
	//成员变量
	static QJsonObject g_Communicate_DB ;
	static QJsonArray g_Worry_List;
	static QJsonArray g_Work_List;
    static s_Work_DB  g_Work;
    static s_Huayan_DB g_Huayan_120;
    static s_Huayan_DB g_Huayan_121;
    static s_Scan_DB g_Prec_Scan_120;
	static s_Scan_DB g_Prec_Scan_121;
    static s_Datasheet_Data g_Datasheet_120;
    static s_Datasheet_Data g_Datasheet_121;
    static s_RGV_DB g_RGV;
    static s_Robot_DB g_Local_Remote;
    static s_HIPNUC_CH10X_DB g_HIPNUC_CH10X;
    static s_IMU_Odom g_IMU_Odom; 
private:
	explicit c_Variable(QObject * parent = nullptr);
	virtual ~c_Variable();

	// 禁止复制
	c_Variable(const c_Variable &other) = delete;
	c_Variable& operator=(const c_Variable &other) = delete;

	static QMutex g_mutex;
	static QScopedPointer<c_Variable> g_instance;
	friend struct QScopedPointerDeleter<c_Variable>;
};

