#pragma once
#include "Variable.h"
#include "TCP_Client.h"

class c_Huayan_Remote : public c_Object
{
    Q_OBJECT
public:
    explicit c_Huayan_Remote(QObject *parent = nullptr);
    virtual ~c_Huayan_Remote();
    
    // ===== 纯虚接口：子类必须实现 =====
    virtual void Connect() = 0;              // 连接 IF 服务 (默认 10003 端口)
    virtual void Connect_Done() = 0;         // 连接完成
    virtual void Disconnect_Done() = 0;      // 断开连接完成
    
    int AutoEn_Count = 0;
    int AutoDn_Count = 0;

    bool is_AutoEn = false;
    bool is_AutoDn = false;
signals:
    // 连接相关
    void Connect_Device(QString ip, int port);
    void Disconnect_Device();
    
    void GrpEnable_Done();
    void BlackOut_Done();

    // 命令响应（通用）
    void Command_Response(QString cmdName, bool success, QString response, QString cmdID);
    void Error_Occurred(int errorCode, QString errorMsg);
    
    // 运动状态（专用）
    void Motion_Finished(bool success, QString errorMsg);
    void Robot_State_Updated(int state, int enabled, int moving, int errorCode);
    
    // 位置更新（用于 UI 刷新）
    void Position_Updated(double jointPos[6], double tcpPos[6]);
    
    // 脚本状态
    void Script_Started(QString scriptName);
    void Script_Stopped(QString scriptName);
    
    void RunFunc_Response(QString funcName, bool success, QString errorMsg);

    
public slots:
    virtual void AutoEn();          // 自动使能
    virtual void AutoDn();          // 自动下电
    // ===== 生命周期管理 =====
    void Init();                    // 初始化 Remote（子线程）
    void Disconnect();              // 断开连接
    
    // ===== 4.1 基础控制指令 =====
    void Electrify();               // 机器人上电
    void StartMaster();             // 激活控制器主站
    void BlackOut();                // 机器人断电
    void CloseMaster();             // 断开控制器
    
    // ===== 4.2 轴组控制指令（手动操作核心）=====
    void GrpEnable(int nRbtID = 0);      // 上使能
    void GrpDisable(int nRbtID = 0);     // 下使能
    void GrpReset(int nRbtID = 0);       // 复位清错
    void GrpStop(int nRbtID = 0);        // 停止运动（急停）
    void GrpInterrupt(int nRbtID = 0);   // 暂停运动
    void GrpContinue(int nRbtID = 0);    // 继续运动
    void EnterSafeGuard(int nRbtID = 0, int bFlag = 1);  // 软急停
    void XToStandby(int nRbtID = 0);     // 返回使能状态
    
    // ===== 4.3 脚本控制指令（调用示教程序）=====
    void RunFunc(QString strFuncName, QStringList sParams);
    void StartScript();                          // 启动脚本 Main 函数
    void StopScript();                           // 停止脚本
    void PauseScript();                          // 暂停脚本
    void ContinueScript();                       // 继续脚本
    void SwitchScript(int nRbtID, QString sScriptName);  // 切换/加载示教程序
    void ReadDefaultScript(int nRbtID);          // 读取当前运行的脚本名称
    // ===== 4.5 状态读取与设置命令 =====
    /// <summary>
    /// 设置机器人全局速度比
    /// </summary>
    /// <param name="nRbtID">机器人 ID，默认 0</param>
    /// <param name="dOverride">速度比，范围 0.01 ~ 1.0 (例如 0.5 代表 50%)</param>
    void SetOverride(int nRbtID = 0, QString dOverride = "1.0");
    /// <summary>
    /// 读取当前机器人全局速度比
    /// </summary>
    /// <param name="nRbtID">机器人 ID，默认 0</param>
    void ReadOverride(int nRbtID = 0);
    // ===== 4.10.14-4.10.18 点动控制指令（手动操作）=====
    void ShortJogJ(int nRbtID, int nAxisId, int nDirection);      // 关节短点动 2°
    void ShortJogL(int nRbtID, int nAxisId, int nDirection);      // 空间短点动 2mm/°
    void LongJogJ(int nRbtID, int nAxisId, int nDirection, int nState);  // 关节长点动
    void LongJogL(int nRbtID, int nAxisId, int nDirection, int nState);  // 空间长点动
    void LongMoveEvent(int nRbtID);                               // 长点动事件循环
    
    // ===== 4.10.12-4.10.13 运动到点指令（精准定位）=====
    void MoveJTo(int nRbtID, double dJ1, double dJ2, double dJ3,
                 double dJ4, double dJ5, double dJ6);  // 关节空间运动到点
    void MoveLTo(int nRbtID, double dX, double dY, double dZ,
                 double dRx, double dRy, double dRz,
                 QString sTcpName, QString sUcsName);  // 空间直线运动到点
    
    // ===== 4.10.5-4.10.6 路点运动指令（示教点位执行）=====
    void WayPoint(int nRbtID,
                  double dX, double dY, double dZ, double dRx, double dRy, double dRz,
                  double dJ1, double dJ2, double dJ3, double dJ4, double dJ5, double dJ6,
                  QString sTcpName, QString sUcsName,
                  double dVelocity, double dAcc, double dRadius,
                  int nMoveType, int nIsUseJoint,
                  int nIsSeek, int nIOBit, int nIOState,
                  QString strCmdID);
    void WayPoint2(int nRbtID,
                   double dEndPos_X, double dEndPos_Y, double dEndPos_Z,
                   double dEndPos_Rx, double dEndPos_Ry, double dEndPos_Rz,
                   double dJ1, double dJ2, double dJ3, double dJ4, double dJ5, double dJ6,
                   QString sTcpName, QString sUcsName,
                   double dVelocity, double dAcc, double dRadius,
                   int nMoveType, int nIsUseJoint,
                   int nIsSeek, int nIOBit, int nIOState,
                   double dAuxPos_X, double dAuxPos_Y, double dAuxPos_Z,
                   double dAuxPos_Rx, double dAuxPos_Ry, double dAuxPos_Rz,
                   QString strCmdID);
    
    // ===== 4.11 轨迹运动指令（运行已示教轨迹）=====
    void InitPath(int nRbtID, int nRawDataType, QString sPathName,
                  double dSpeedRatio, double dRadius);  // 初始化关节轨迹
    void InitPath(int nRbtID, int nRawDataType, QString sPathName,
                  double dSpeedRatio, double dRadius,
                  double dVelocity, double dAcc, double dJerk,
                  QString sUcsName, QString sTcpName);   // 初始化空间轨迹
    void PushPathPoints(int nRbtID, QString sPathName, const QVector<double>& points);  // 批量推送轨迹点
    void EndPushPathPoints(int nRbtID, QString sPathName);  // 结束推送并开始计算
    void MovePathJ(int nRbtID, QString sPathName);  // 以关节方式运行轨迹
    void MovePathL(int nRbtID, QString sPathName);  // 以空间方式运行轨迹
    void ReadPathState(int nRbtID, QString sPathName);  // 读取轨迹状态
    
    // ===== 4.5-4.6 状态查询指令（用于轮询运动完成）=====
    void ReadRobotState(int nRbtID = 0);   // 读取机器人状态标志
    void ReadActPos(int nRbtID = 0);       // 读取实际位置
    void ReadCurFSM(int nRbtID = 0);       // 读取当前状态机
    
    // ===== 便捷封装函数（简化手动操作流程）=====
    void EmergencyStop(int nRbtID = 0);    // 紧急停止（软急停 + 硬停止）
    void MoveJToHome(int nRbtID = 0);      // 关节空间回原点
    bool WaitForMotionFinish(int nRbtID = 0, int timeoutMs = 10000);  // 等待运动完成（需配合 ReadRobotState 轮询）
    
    // ===== 辅助函数 =====
    QString lastErrorCode() const { return m_LastErrorCode; }
    
private slots:
    // 内部处理槽
    void Read_String_Done(QString str);      // 处理字符串响应（协议解析）
    
private:
    // ===== 协议工具函数 =====
    QString Build_Command(const QString& cmdName, const QStringList& params);
    bool ParseResponse(const QString& response, QString& errorCode, QString& errorMsg);
    
    // ===== 命令发送 =====
    bool SendCommand(const QString& cmd);
    
    // ===== 成员变量 =====
    c_TCP_Client* m_Client;         // TCP 客户端（运行在子线程）
    QThread* m_Thread;              // Remote 专属线程
    // 状态跟踪
    QString m_LastErrorCode;        // 最后错误码
};