#include "Huayan_Remote.h"

c_Huayan_Remote::c_Huayan_Remote(QObject *parent)  : c_Object(parent)
{
}

c_Huayan_Remote::~c_Huayan_Remote()
{
    // 安全清理线程资源
    if (m_Thread && m_Thread->isRunning()) {
        m_Thread->requestInterruption();
        m_Thread->quit();
        m_Thread->wait();
    }
    // m_Client 由 QThread 管理，无需手动 delete
}

// ===== 初始化 =====
void c_Huayan_Remote::Init()
{
    // 创建专属线程
    m_Thread = new QThread(this);
    
    // 创建并配置 TCP 客户端
    m_Client = new c_TCP_Client();
    m_Client->moveToThread(m_Thread);
    
    // 连接线程生命周期信号
    QObject::connect(m_Thread, &QThread::started, m_Client, &c_TCP_Client::Init);
    QObject::connect(m_Thread, &QThread::finished, m_Client, &c_TCP_Client::deleteLater);
    
    // 连接客户端信号到本类槽函数（跨线程通信）
    QObject::connect(this, &c_Huayan_Remote::Connect_Device, m_Client, &c_TCP_Client::Connect_Device);
    QObject::connect(this, &c_Huayan_Remote::Disconnect_Device, m_Client, &c_TCP_Client::Disconnect_Device);

    QObject::connect(m_Client, &c_TCP_Client::Connect_Done, this, &c_Huayan_Remote::Connect_Done);
    QObject::connect(m_Client, &c_TCP_Client::Disconnect_Done, this, &c_Huayan_Remote::Disconnect_Done);
    QObject::connect(m_Client, &c_TCP_Client::Status, this, [=](int state) {emit Status(TCP_Status(state)); });
    QObject::connect(m_Client, &c_TCP_Client::Read_String_Done, this, &c_Huayan_Remote::Read_String_Done);
    QObject::connect(m_Client, &c_TCP_Client::Connect_Loop, this, &c_Huayan_Remote::Connect);
    
    // 启动线程
    m_Thread->start();

    QTimer::singleShot(3000, this, &c_Huayan_Remote::Connect);
}

void c_Huayan_Remote::Disconnect()
{
    emit Disconnect_Device();
}

// ===== 4.1 基础控制指令实现 =====

void c_Huayan_Remote::Electrify()
{
    QString cmd = Build_Command("Electrify", QStringList());
    SendCommand(cmd);
}
void c_Huayan_Remote::AutoEn()
{
   
}
void c_Huayan_Remote::AutoDn()
{
    
}
void c_Huayan_Remote::BlackOut()
{
    QString cmd = Build_Command("BlackOut", QStringList());
    SendCommand(cmd);
}

void c_Huayan_Remote::StartMaster()
{
    QString cmd = Build_Command("StartMaster", QStringList());
    SendCommand(cmd);
}

void c_Huayan_Remote::CloseMaster()
{
    QString cmd = Build_Command("CloseMaster", QStringList());
    SendCommand(cmd);
}

// ===== 4.2 轴组控制指令实现 =====

void c_Huayan_Remote::GrpEnable(int nRbtID)
{
    QStringList params;
    params << QString::number(nRbtID);
    QString cmd = Build_Command("GrpEnable", params);
    SendCommand(cmd);
}

void c_Huayan_Remote::GrpDisable(int nRbtID)
{
    QStringList params;
    params << QString::number(nRbtID);
    QString cmd = Build_Command("GrpDisable", params);
    SendCommand(cmd);
}

void c_Huayan_Remote::GrpReset(int nRbtID)
{
    QStringList params;
    params << QString::number(nRbtID);
    QString cmd = Build_Command("GrpReset", params);
    SendCommand(cmd);
}

void c_Huayan_Remote::GrpStop(int nRbtID)
{
    QStringList params;
    params << QString::number(nRbtID);
    QString cmd = Build_Command("GrpStop", params);
    SendCommand(cmd);
}

void c_Huayan_Remote::GrpInterrupt(int nRbtID)
{
    QStringList params;
    params << QString::number(nRbtID);
    QString cmd = Build_Command("GrpInterrupt", params);
    SendCommand(cmd);
}

void c_Huayan_Remote::GrpContinue(int nRbtID)
{
    QStringList params;
    params << QString::number(nRbtID);
    QString cmd = Build_Command("GrpContinue", params);
    SendCommand(cmd);
}

void c_Huayan_Remote::EnterSafeGuard(int nRbtID, int bFlag)
{
    QStringList params;
    params << QString::number(nRbtID) << QString::number(bFlag);
    QString cmd = Build_Command("EnterSafeGuard", params);
    SendCommand(cmd);
}

void c_Huayan_Remote::XToStandby(int nRbtID)
{
    QStringList params;
    params << QString::number(nRbtID);
    QString cmd = Build_Command("XToStandby", params);
    SendCommand(cmd);
}

// ===== 4.3 脚本控制指令实现（调用示教程序）=====
void c_Huayan_Remote::RunFunc(QString strFuncName, QStringList sParams)
{
    // 协议 4.3.1 节：RunFunc,strFuncName,sParams1,...,sParamsn,;
    QStringList params;
    params << strFuncName;
    
    // 添加可选参数
    for (const QString& param : sParams) {
        params << param;
    }
    
    QString cmd = Build_Command("RunFunc", params);
    SendCommand(cmd);
}
void c_Huayan_Remote::StartScript()
{
    QString cmd = Build_Command("StartScript", QStringList());
    SendCommand(cmd);
}

void c_Huayan_Remote::StopScript()
{
    QString cmd = Build_Command("StopScript", QStringList());
    SendCommand(cmd);
}

void c_Huayan_Remote::PauseScript()
{
    QString cmd = Build_Command("PauseScript", QStringList());
    SendCommand(cmd);
}

void c_Huayan_Remote::ContinueScript()
{
    QString cmd = Build_Command("ContinueScript", QStringList());
    SendCommand(cmd);
}

void c_Huayan_Remote::SwitchScript(int nRbtID, QString sScriptName)
{
    QStringList params;
    params << QString::number(nRbtID) << sScriptName;
    QString cmd = Build_Command("SwitchScript", params);
    SendCommand(cmd);
}

void c_Huayan_Remote::ReadDefaultScript(int nRbtID)
{
    QStringList params;
    params << QString::number(nRbtID);
    QString cmd = Build_Command("ReadDefaultScript", params);
    SendCommand(cmd);
}


// ===== 4.5 状态读取与设置命令实现 =====
void c_Huayan_Remote::SetOverride(int nRbtID, QString dOverride)
{
    // 协议 4.5.1 节：SetOverride,nRbtID,dOverride,;
    // 安全边界保护：确保速度比在 0.01 ~ 1.0 之间
    if (dOverride.toDouble() < 0.01) {
        dOverride = "0.01";
    } else if (dOverride.toDouble() > 1.0) {
        dOverride = "1.00";
    }

    QStringList params;
    params << QString::number(nRbtID) 
           << dOverride; // 保留3位小数，如 "0.500"
    
    QString cmd = Build_Command("SetOverride", params);
    SendCommand(cmd);
}

void c_Huayan_Remote::ReadOverride(int nRbtID)
{
    // 协议 4.5.14 节：ReadOverride,nRbtID,;
    QStringList params;
    params << QString::number(nRbtID);
    
    QString cmd = Build_Command("ReadOverride", params);
    SendCommand(cmd);
}

// ===== 4.10.14-4.10.18 点动控制指令实现（手动操作核心）=====

void c_Huayan_Remote::ShortJogJ(int nRbtID, int nAxisId, int nDirection)
{
    // 协议 4.10.14 节：ShortJogJ,nRbtID,nAxisId,nDirection,;
    QStringList params;
    params << QString::number(nRbtID) << QString::number(nAxisId) << QString::number(nDirection);
    QString cmd = Build_Command("ShortJogJ", params);
    SendCommand(cmd);
}

void c_Huayan_Remote::ShortJogL(int nRbtID, int nAxisId, int nDirection)
{
    // 协议 4.10.15 节：ShortJogL,nRbtID,nAxisId,nDirection,;
    QStringList params;
    params << QString::number(nRbtID) << QString::number(nAxisId) << QString::number(nDirection);
    QString cmd = Build_Command("ShortJogL", params);
    SendCommand(cmd);
}

void c_Huayan_Remote::LongJogJ(int nRbtID, int nAxisId, int nDirection, int nState)
{
    // 协议 4.10.16 节：LongJogJ,nRbtID,nAxisId,nDirection,nState,;
    QStringList params;
    params << QString::number(nRbtID) << QString::number(nAxisId) 
           << QString::number(nDirection) << QString::number(nState);
    QString cmd = Build_Command("LongJogJ", params);
    SendCommand(cmd);
}

void c_Huayan_Remote::LongJogL(int nRbtID, int nAxisId, int nDirection, int nState)
{
    // 协议 4.10.17 节：LongJogL,nRbtID,nAxisId,nDirection,nState,;
    QStringList params;
    params << QString::number(nRbtID) << QString::number(nAxisId) 
           << QString::number(nDirection) << QString::number(nState);
    QString cmd = Build_Command("LongJogL", params);
    SendCommand(cmd);
}

void c_Huayan_Remote::LongMoveEvent(int nRbtID)
{
    // 协议 4.10.18 节：LongMoveEvent,nRbtID,;
    // 注意：此命令需周期调用（≤200ms），否则自动停止长点动
    QStringList params;
    params << QString::number(nRbtID);
    QString cmd = Build_Command("LongMoveEvent", params);
    SendCommand(cmd);
}

// ===== 4.10.12-4.10.13 运动到点指令实现 =====

void c_Huayan_Remote::MoveJTo(int nRbtID, double dJ1, double dJ2, double dJ3,
                               double dJ4, double dJ5, double dJ6)
{
    // 协议 4.10.12 节：MoveJTo,nRbtID,dJ1,dJ2,dJ3,dJ4,dJ5,dJ6,;
    QStringList params;
    params << QString::number(nRbtID)
           << QString::number(dJ1) << QString::number(dJ2) << QString::number(dJ3)
           << QString::number(dJ4) << QString::number(dJ5) << QString::number(dJ6);
    QString cmd = Build_Command("MoveJTo", params);
    SendCommand(cmd);
}

void c_Huayan_Remote::MoveLTo(int nRbtID, double dX, double dY, double dZ,
                               double dRx, double dRy, double dRz,
                               QString sTcpName, QString sUcsName)
{
    // 协议 4.10.13 节：MoveLTo,nRbtID,dX,dY,dZ,dRx,dRy,dRz,sTcpName,sUcsName,;
    QStringList params;
    params << QString::number(nRbtID)
           << QString::number(dX) << QString::number(dY) << QString::number(dZ)
           << QString::number(dRx) << QString::number(dRy) << QString::number(dRz)
           << sTcpName << sUcsName;
    QString cmd = Build_Command("MoveLTo", params);
    SendCommand(cmd);
}

// ===== 4.10.5-4.10.6 路点运动指令实现 =====

void c_Huayan_Remote::WayPoint(int nRbtID,
                                double dX, double dY, double dZ, double dRx, double dRy, double dRz,
                                double dJ1, double dJ2, double dJ3, double dJ4, double dJ5, double dJ6,
                                QString sTcpName, QString sUcsName,
                                double dVelocity, double dAcc, double dRadius,
                                int nMoveType, int nIsUseJoint,
                                int nIsSeek, int nIOBit, int nIOState,
                                QString strCmdID)
{
    // 协议 4.10.5 节完整参数
    QStringList params;
    params << QString::number(nRbtID)
           << QString::number(dX) << QString::number(dY) << QString::number(dZ)
           << QString::number(dRx) << QString::number(dRy) << QString::number(dRz)
           << QString::number(dJ1) << QString::number(dJ2) << QString::number(dJ3)
           << QString::number(dJ4) << QString::number(dJ5) << QString::number(dJ6)
           << sTcpName << sUcsName
           << QString::number(dVelocity) << QString::number(dAcc) << QString::number(dRadius)
           << QString::number(nMoveType) << QString::number(nIsUseJoint)
           << QString::number(nIsSeek) << QString::number(nIOBit) << QString::number(nIOState)
           << strCmdID;
    QString cmd = Build_Command("WayPoint", params);
    SendCommand(cmd);
}

void c_Huayan_Remote::WayPoint2(int nRbtID,
                                 double dEndPos_X, double dEndPos_Y, double dEndPos_Z,
                                 double dEndPos_Rx, double dEndPos_Ry, double dEndPos_Rz,
                                 double dJ1, double dJ2, double dJ3, double dJ4, double dJ5, double dJ6,
                                 QString sTcpName, QString sUcsName,
                                 double dVelocity, double dAcc, double dRadius,
                                 int nMoveType, int nIsUseJoint,
                                 int nIsSeek, int nIOBit, int nIOState,
                                 double dAuxPos_X, double dAuxPos_Y, double dAuxPos_Z,
                                 double dAuxPos_Rx, double dAuxPos_Ry, double dAuxPos_Rz,
                                 QString strCmdID)
{
    // 协议 4.10.6 节完整参数（支持圆弧过渡）
    QStringList params;
    params << QString::number(nRbtID)
           << QString::number(dEndPos_X) << QString::number(dEndPos_Y) << QString::number(dEndPos_Z)
           << QString::number(dEndPos_Rx) << QString::number(dEndPos_Ry) << QString::number(dEndPos_Rz)
           << QString::number(dJ1) << QString::number(dJ2) << QString::number(dJ3)
           << QString::number(dJ4) << QString::number(dJ5) << QString::number(dJ6)
           << sTcpName << sUcsName
           << QString::number(dVelocity) << QString::number(dAcc) << QString::number(dRadius)
           << QString::number(nMoveType) << QString::number(nIsUseJoint)
           << QString::number(nIsSeek) << QString::number(nIOBit) << QString::number(nIOState)
           << QString::number(dAuxPos_X) << QString::number(dAuxPos_Y) << QString::number(dAuxPos_Z)
           << QString::number(dAuxPos_Rx) << QString::number(dAuxPos_Ry) << QString::number(dAuxPos_Rz)
           << strCmdID;
    QString cmd = Build_Command("WayPoint2", params);
    SendCommand(cmd);
}

// ===== 4.11 轨迹运动指令实现（运行已示教轨迹）=====

void c_Huayan_Remote::InitPath(int nRbtID, int nRawDataType, QString sPathName,
                                double dSpeedRatio, double dRadius)
{
    // 协议 4.11.1 节：关节轨迹初始化
    QStringList params;
    params << QString::number(nRbtID) << QString::number(nRawDataType) 
           << sPathName << QString::number(dSpeedRatio) << QString::number(dRadius);
    QString cmd = Build_Command("InitPath", params);
    SendCommand(cmd);
}

void c_Huayan_Remote::InitPath(int nRbtID, int nRawDataType, QString sPathName,
                                double dSpeedRatio, double dRadius,
                                double dVelocity, double dAcc, double dJerk,
                                QString sUcsName, QString sTcpName)
{
    // 协议 4.11.1 节：空间轨迹初始化
    QStringList params;
    params << QString::number(nRbtID) << QString::number(nRawDataType) 
           << sPathName << QString::number(dSpeedRatio) << QString::number(dRadius)
           << QString::number(dVelocity) << QString::number(dAcc) << QString::number(dJerk)
           << sUcsName << sTcpName;
    QString cmd = Build_Command("InitPath", params);
    SendCommand(cmd);
}

void c_Huayan_Remote::PushPathPoints(int nRbtID, QString sPathName, const QVector<double>& points)
{
    // 协议 4.11.2 节：批量推送轨迹点
    // points 格式：[x1,y1,z1,rx1,ry1,rz1, x2,y2,z2,rx2,ry2,rz2, ...]
    if (points.size() % 6 != 0) {
        emit Error_Occurred(-1, "PushPathPoints: 点位数据必须是 6 的倍数");
        return;
    }
    
    QStringList params;
    params << QString::number(nRbtID) << sPathName;
    for (double val : points) {
        params << QString::number(val);
    }
    QString cmd = Build_Command("PushPathPoints", params);
    SendCommand(cmd);
}

void c_Huayan_Remote::EndPushPathPoints(int nRbtID, QString sPathName)
{
    // 协议 4.11.3 节：结束推送并开始计算轨迹
    QStringList params;
    params << QString::number(nRbtID) << sPathName;
    QString cmd = Build_Command("EndPushPathPoints", params);
    SendCommand(cmd);
}

void c_Huayan_Remote::MovePathJ(int nRbtID, QString sPathName)
{
    // 协议 4.11.8 节：以关节方式运行轨迹
    QStringList params;
    params << QString::number(nRbtID) << sPathName;
    QString cmd = Build_Command("MovePathJ", params);
    SendCommand(cmd);
}

void c_Huayan_Remote::MovePathL(int nRbtID, QString sPathName)
{
    // 协议 4.11.9 节：以空间方式运行轨迹
    QStringList params;
    params << QString::number(nRbtID) << sPathName;
    QString cmd = Build_Command("MovePathL", params);
    SendCommand(cmd);
}

void c_Huayan_Remote::ReadPathState(int nRbtID, QString sPathName)
{
    // 协议 4.11.7 节：读取轨迹状态
    QStringList params;
    params << QString::number(nRbtID) << sPathName;
    QString cmd = Build_Command("ReadPathState", params);
    SendCommand(cmd);
}

// ===== 4.5-4.6 状态查询指令实现 =====

void c_Huayan_Remote::ReadRobotState(int nRbtID)
{
    QStringList params;
    params << QString::number(nRbtID);
    QString cmd = Build_Command("ReadRobotState", params);
    SendCommand(cmd);
}

void c_Huayan_Remote::ReadActPos(int nRbtID)
{
    QStringList params;
    params << QString::number(nRbtID);
    QString cmd = Build_Command("ReadActPos", params);
    SendCommand(cmd);
}

void c_Huayan_Remote::ReadCurFSM(int nRbtID)
{
    QStringList params;
    params << QString::number(nRbtID);
    QString cmd = Build_Command("ReadCurFSM", params);
    SendCommand(cmd);
}

// ===== 便捷封装函数 =====

void c_Huayan_Remote::EmergencyStop(int nRbtID)
{
    // 双重保护：软急停 + 硬停止
    EnterSafeGuard(nRbtID, 1);  // 进入软急停
    GrpStop(nRbtID);            // 硬停止
    emit Status(QString("⚠️ 紧急停止指令已发送 [RobotID:%1]").arg(nRbtID));
}

void c_Huayan_Remote::MoveJToHome(int nRbtID)
{
    // 回原点：所有关节角度为 0°
    MoveJTo(nRbtID, 0, 0, 0, 0, 0, 0);
}

bool c_Huayan_Remote::WaitForMotionFinish(int nRbtID, int timeoutMs)
{
    // 注意：此函数为同步阻塞，建议在独立线程中调用
    // 实际应用中建议使用状态机轮询 + 信号槽异步方式
    Q_UNUSED(nRbtID);
    Q_UNUSED(timeoutMs);
    // 实际实现需配合 ReadRobotState 轮询 + 状态解析
    return true;  // 占位返回
}

// ===== 协议工具函数 =====

QString c_Huayan_Remote::Build_Command(const QString& cmdName, const QStringList& params)
{
    // 协议 2.1 节格式：消息名称，Param1,Param2,Param3......Paramn,;
    QString cmd = cmdName;
    for (const QString& param : params) {
        cmd += "," + param;
    }
    cmd += ",;";  // 关键：协议要求的结束标记
    return cmd;
}

bool c_Huayan_Remote::ParseResponse(const QString& response, QString& errorCode, QString& errorMsg)
{
    QString resp = response.trimmed();
    
    // 检查成功标记（使用正则避免误匹配）
    QRegExp okPattern(",OK,");
    if (okPattern.indexIn(resp) != -1) {
        return true;
    }
    
    // 检查失败标记
    QRegExp failPattern(",Fail,");
    if (failPattern.indexIn(resp) != -1) {
        QStringList parts = resp.split(",", Qt::SkipEmptyParts);
        if (parts.size() >= 4) {
            errorCode = parts[2].trimmed();
            errorMsg = parts[3].trimmed();
        } else if (parts.size() >= 3) {
            errorCode = parts[2].trimmed();
            errorMsg = "Unknown error";
        } else {
            errorCode = "-1";
            errorMsg = "Invalid response format";
        }
        return false;
    }
    
    // 未知格式
    errorCode = "-999";
    errorMsg = "Response format not recognized";
    return false;
}

// ===== 命令发送 =====

bool c_Huayan_Remote::SendCommand(const QString& cmd)
{
    // 基础校验
    if (!m_Client || !m_Thread || !m_Thread->isRunning()) {
        emit Error_Occurred(-1, "Client not initialized or thread not running");
        return false;
    }
    
    if (cmd.isEmpty()) {
        emit Error_Occurred(-2, "Command is empty");
        return false;
    }
    
    // 跨线程调用发送（确保在 TCP_Client 所在线程执行）
    QMetaObject::invokeMethod(m_Client, "Write_String", 
                              Qt::QueuedConnection,
                              Q_ARG(QString, cmd));
    
    return true;
}

// ===== 响应处理=====
void c_Huayan_Remote::Read_String_Done(QString str)
{
    // 协议解析
    QString errorCode, errorMsg;
    bool success = ParseResponse(str, errorCode, errorMsg);
    
    if (success) {
        // ===== 成功响应处理 =====
        QStringList parts = str.split(",", Qt::SkipEmptyParts);
        QString cmdName = parts.isEmpty() ? "Unknown" : parts[0];
        QString cmdID = parts.size() >= 4 ? parts[3].trimmed() : "";
        
        emit Command_Response(cmdName, true, str, cmdID);
        
        // 特殊处理：运动类命令完成后发射运动完成信号
        if (cmdName.contains("Move", Qt::CaseInsensitive) ||
            cmdName.contains("Jog", Qt::CaseInsensitive) ||
            cmdName.contains("Path", Qt::CaseInsensitive)) {
            emit Motion_Finished(true, "");
        }
        
        // 特殊处理：脚本命令
        if (cmdName == "StartScript") {
            emit Script_Started("Main");
        } else if (cmdName == "StopScript") {
            emit Script_Stopped("Main");
        }
        
        // ⚠️ 新增：RunFunc 成功响应
        if (cmdName == "RunFunc" && parts.size() >= 3) {
            QString funcName = parts[2].trimmed();
            emit RunFunc_Response(funcName, true, "");
        }
        
        // 特殊处理：状态查询命令解析并转发状态更新
        if (cmdName == "ReadRobotState" && parts.size() >= 15) {
            int moving = parts[2].toInt();
            int enabled = parts[3].toInt();
            int errorState = parts[4].toInt();
            int errorCode = parts[5].toInt();
            emit Robot_State_Updated(moving, enabled, errorState, errorCode);
        }
        
    } else {
        // ===== 失败响应处理 =====
        QString cmdName = str.split(",", Qt::SkipEmptyParts).value(0, "Unknown");
        m_LastErrorCode = errorCode;
        
        emit Command_Response(cmdName, false, str, "");
        emit Error_Occurred(errorCode.toInt(), errorMsg);
        
        // ⚠️ 新增：RunFunc 失败响应
        if (cmdName == "RunFunc") {
            // 失败返回格式：RunFunc,Fail,ErrorCode,strName,strErr,ErrorExplanation,;
            QStringList parts = str.split(",", Qt::SkipEmptyParts);
            QString funcName = parts.size() >= 4 ? parts[3].trimmed() : "Unknown";
            QString funcError = parts.size() >= 5 ? parts[4].trimmed() : errorMsg;
            emit RunFunc_Response(funcName, false, funcError);
        }
        
        // 运动类命令失败也发射运动完成信号（带错误）
        if (cmdName.contains("Move", Qt::CaseInsensitive) ||
            cmdName.contains("Jog", Qt::CaseInsensitive) ||
            cmdName.contains("Path", Qt::CaseInsensitive)) {
            emit Motion_Finished(false, errorMsg);
        }
    }
}