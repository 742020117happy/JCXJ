#include "Huayan_Monitor.h"

c_Huayan_Monitor::c_Huayan_Monitor(QObject *parent) : c_Object(parent)
{ 
   
}

c_Huayan_Monitor::~c_Huayan_Monitor()
{
    if (m_Thread && m_Thread->isRunning()) {
        m_Thread->requestInterruption();
        m_Thread->quit();
        m_Thread->wait();
    }
}

void c_Huayan_Monitor::Init()
{
    m_Thread = new QThread(this);
    m_Client = new c_TCP_Client();
    m_Client->moveToThread(m_Thread);
    
    QObject::connect(m_Thread, &QThread::started, m_Client, &c_TCP_Client::Init);
    QObject::connect(m_Thread, &QThread::finished, m_Client, &c_TCP_Client::deleteLater);
    
    QObject::connect(this, &c_Huayan_Monitor::Connect_Device, m_Client, &c_TCP_Client::Connect_Device);
    QObject::connect(this, &c_Huayan_Monitor::Disconnect_Device, m_Client, &c_TCP_Client::Disconnect_Device);
    
    QObject::connect(m_Client, &c_TCP_Client::Connect_Done, this, &c_Huayan_Monitor::Connect_Done);
    QObject::connect(m_Client, &c_TCP_Client::Disconnect_Done, this, &c_Huayan_Monitor::Disconnect_Done);
    QObject::connect(m_Client, &c_TCP_Client::Status,  this, [=](int state) {emit Status(TCP_Status(state)); });
    
    // ⚠️ 修复：DataSheet 使用 Read_Byte_Done 处理 LTBR 包头
    QObject::connect(m_Client, &c_TCP_Client::Read_Byte_Done, this, &c_Huayan_Monitor::Read_Byte_Done);
    
    QObject::connect(m_Client, &c_TCP_Client::Connect_Loop, this, &c_Huayan_Monitor::Connect);
     
    m_Thread->start();
    QTimer::singleShot(3000, this, &c_Huayan_Monitor::Connect);
    
}

void c_Huayan_Monitor::Disconnect()
{
    emit Disconnect_Device();
}

void c_Huayan_Monitor::Read_Byte_Done(QByteArray buffer)
{
    // ===== DataSheet 数据包格式 (协议 5.1 节) =====
    // 4 字节"LTBR" + 4 字节总长度 + 4 字节数据长度 + JSON 数据
    if (buffer.size() < 12) {
        return;
    }
    
    // 检查包头"LTBR"
    if (buffer.left(4) != "LTBR") {
        return;
    }
    
    // 提取 JSON 数据 (从第 12 字节开始)
    QByteArray jsonData = buffer.mid(12);
    
    // 安全解析 JSON
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
    
    if (!doc.isObject() || parseError.error != QJsonParseError::NoError) {
        return;
    }
    
    // 解析到本地缓存（线程内操作，无需锁）
    Parse_Datasheet(doc.object());
    
    // 调用纯虚函数完成数据落地
    CopyParsedDataToGlobal();
    
    // 通知数据已更新
    emit Datasheet_Updated();
}

void c_Huayan_Monitor::Parse_Datasheet(const QJsonObject& json)
{
    Parse_PosAndVel(json);
    Parse_EndIO(json);
    Parse_ElectricBoxIO(json);
    Parse_ElectricBoxAnalogIO(json);
    Parse_StateAndError(json);
    Parse_FTData(json);
    Parse_HardLoad(json);
    Parse_Modbus(json);
    Parse_RobotAuthorization(json);
    Parse_SafePlane(json);
    Parse_ConstraintArea(json);
    Parse_Script(json);
    Parse_MsgTitle(json);
}

void c_Huayan_Monitor::Parse_PosAndVel(const QJsonObject& json)
{
    if (json.contains("PosAndVel") && json["PosAndVel"].isObject()) {
        QJsonObject posVel = json["PosAndVel"].toObject();
        
        // 💡 核心修复：安全解析 Lambda
        // 协议 V1.0.19.1 中所有数值字段均以 String 类型下发 (如 "81.099")
        // 直接使用 toDouble() 会失败并返回 0.0，此处兼容 String 与 Number 双类型
        auto safeParseDouble = [](const QJsonValue& val) -> double {
            if (val.isDouble()) return val.toDouble();
            if (val.isString()) return val.toString().toDouble();
            return 0.0; // 异常类型兜底
        };

        // 1. 关节位置 + 笛卡尔坐标 (协议固定 12 个元素：前6关节 + 后6空间)
        if (posVel.contains("Actual_Position") && posVel["Actual_Position"].isArray()) {
            QJsonArray arr = posVel["Actual_Position"].toArray();
            for (int i = 0; i < 12 && i < arr.size(); ++i) {
                m_Datasheet_Data.Actual_Position[i] = safeParseDouble(arr[i]);
            }
        }

        // 2. TCP 坐标系下笛卡尔坐标
        if (posVel.contains("Actual_PCS_TCP") && posVel["Actual_PCS_TCP"].isArray()) {
            QJsonArray arr = posVel["Actual_PCS_TCP"].toArray();
            for (int i = 0; i < 6 && i < arr.size(); ++i) {
                m_Datasheet_Data.Actual_PCS_TCP[i] = safeParseDouble(arr[i]);
            }
        }

        // 3. 基座坐标系下笛卡尔坐标
        if (posVel.contains("Actual_PCS_Base") && posVel["Actual_PCS_Base"].isArray()) {
            QJsonArray arr = posVel["Actual_PCS_Base"].toArray();
            for (int i = 0; i < 6 && i < arr.size(); ++i) {
                m_Datasheet_Data.Actual_PCS_Base[i] = safeParseDouble(arr[i]);
            }
        }

        // 4. 关节电流 (A)
        if (posVel.contains("Actual_Joint_Current") && posVel["Actual_Joint_Current"].isArray()) {
            QJsonArray arr = posVel["Actual_Joint_Current"].toArray();
            for (int i = 0; i < 6 && i < arr.size(); ++i) {
                m_Datasheet_Data.Actual_Joint_Current[i] = safeParseDouble(arr[i]);
            }
        }

        // 5. 关节速度 (°/s)
        if (posVel.contains("Actual_Joint_Velocity") && posVel["Actual_Joint_Velocity"].isArray()) {
            QJsonArray arr = posVel["Actual_Joint_Velocity"].toArray();
            for (int i = 0; i < 6 && i < arr.size(); ++i) {
                m_Datasheet_Data.Actual_Joint_Velocity[i] = safeParseDouble(arr[i]);
            }
        }

        // 6. 关节加速度 (°/s²)
        if (posVel.contains("Actual_Joint_Acceleration") && posVel["Actual_Joint_Acceleration"].isArray()) {
            QJsonArray arr = posVel["Actual_Joint_Acceleration"].toArray();
            for (int i = 0; i < 6 && i < arr.size(); ++i) {
                m_Datasheet_Data.Actual_Joint_Acceleration[i] = safeParseDouble(arr[i]);
            }
        }

        // 7. 速度比 (协议中为字符串 "1.000"，原 toDouble 会失效)
        if (posVel.contains("Actual_Override")) {
            m_Datasheet_Data.Actual_Override = safeParseDouble(posVel["Actual_Override"]);
        }
    }
}

void c_Huayan_Monitor::Parse_EndIO(const QJsonObject& json)
{
    if (json.contains("EndIO") && json["EndIO"].isObject()) {
        QJsonObject endIO = json["EndIO"].toObject();
        
        if (endIO.contains("EndDI") && endIO["EndDI"].isArray()) {
            QJsonArray diArray = endIO["EndDI"].toArray();
            for (int i = 0; i < 4 && i < diArray.size(); ++i) {
                m_Datasheet_Data.EndDI[i] = diArray[i].toInt();
            }
        }
        
        // ⚠️ 修复：协议 V1.0.18.1 明确 EndDO 只支持 3 个
        if (endIO.contains("EndDO") && endIO["EndDO"].isArray()) {
            QJsonArray doArray = endIO["EndDO"].toArray();
            for (int i = 0; i < 3 && i < doArray.size(); ++i) {  // 改为 i < 3
                m_Datasheet_Data.EndDO[i] = doArray[i].toInt();
            }
        }
        
        if (endIO.contains("EndButton") && endIO["EndButton"].isArray()) {
            QJsonArray btnArray = endIO["EndButton"].toArray();
            for (int i = 0; i < 4 && i < btnArray.size(); ++i) {
                m_Datasheet_Data.EndButton[i] = btnArray[i].toInt();
            }
        }
        
        if (endIO.contains("EnableEndBTN")) {
            m_Datasheet_Data.EnableEndBTN = endIO["EnableEndBTN"].toInt();
        }
        
        if (endIO.contains("EndAI") && endIO["EndAI"].isArray()) {
            QJsonArray aiArray = endIO["EndAI"].toArray();
            for (int i = 0; i < 2 && i < aiArray.size(); ++i) {
                m_Datasheet_Data.EndAI[i] = aiArray[i].toDouble();
            }
        }
    }
}

void c_Huayan_Monitor::Parse_ElectricBoxIO(const QJsonObject& json)
{
    if (json.contains("ElectricBoxIO") && json["ElectricBoxIO"].isObject()) {
        QJsonObject boxIO = json["ElectricBoxIO"].toObject();
        
        if (boxIO.contains("BoxCI") && boxIO["BoxCI"].isArray()) {
            QJsonArray ciArray = boxIO["BoxCI"].toArray();
            for (int i = 0; i < 8 && i < ciArray.size(); ++i) {
                m_Datasheet_Data.BoxCI[i] = ciArray[i].toInt();
            }
        }
        
        if (boxIO.contains("BoxCO") && boxIO["BoxCO"].isArray()) {
            QJsonArray coArray = boxIO["BoxCO"].toArray();
            for (int i = 0; i < 8 && i < coArray.size(); ++i) {
                m_Datasheet_Data.BoxCO[i] = coArray[i].toInt();
            }
        }
        
        if (boxIO.contains("BoxDI") && boxIO["BoxDI"].isArray()) {
            QJsonArray diArray = boxIO["BoxDI"].toArray();
            for (int i = 0; i < 8 && i < diArray.size(); ++i) {
                m_Datasheet_Data.BoxDI[i] = diArray[i].toInt();
            }
        }
        
        if (boxIO.contains("BoxDO") && boxIO["BoxDO"].isArray()) {
            QJsonArray doArray = boxIO["BoxDO"].toArray();
            for (int i = 0; i < 8 && i < doArray.size(); ++i) {
                m_Datasheet_Data.BoxDO[i] = doArray[i].toInt();
            }
        }
        
        if (boxIO.contains("Conveyor")) {
            m_Datasheet_Data.Conveyor = boxIO["Conveyor"].toDouble();
        }
        
        if (boxIO.contains("Encode")) {
            m_Datasheet_Data.Encode = boxIO["Encode"].toInt();
        }
    }
}

void c_Huayan_Monitor::Parse_ElectricBoxAnalogIO(const QJsonObject& json)
{
    if (json.contains("ElectricBoxAnalogIO") && json["ElectricBoxAnalogIO"].isObject()) {
        QJsonObject analogIO = json["ElectricBoxAnalogIO"].toObject();
        
        if (analogIO.contains("BoxAnalogOutMode_1")) {
            m_Datasheet_Data.BoxAnalogOutMode_1 = analogIO["BoxAnalogOutMode_1"].toInt();
        }
        
        if (analogIO.contains("BoxAnalogOutMode_2")) {
            m_Datasheet_Data.BoxAnalogOutMode_2 = analogIO["BoxAnalogOutMode_2"].toInt();
        }
        
        if (analogIO.contains("BoxAnalogOut_1")) {
            m_Datasheet_Data.BoxAnalogOut_1 = analogIO["BoxAnalogOut_1"].toDouble();
        }
        
        if (analogIO.contains("BoxAnalogOut_2")) {
            m_Datasheet_Data.BoxAnalogOut_2 = analogIO["BoxAnalogOut_2"].toDouble();
        }
        
        if (analogIO.contains("BoxAnalogIn_1")) {
            m_Datasheet_Data.BoxAnalogIn_1 = analogIO["BoxAnalogIn_1"].toDouble();
        }
        
        if (analogIO.contains("BoxAnalogIn_2")) {
            m_Datasheet_Data.BoxAnalogIn_2 = analogIO["BoxAnalogIn_2"].toDouble();
        }
    }
}

void c_Huayan_Monitor::Parse_StateAndError(const QJsonObject& json)
{
    if (json.contains("StateAndError") && json["StateAndError"].isObject()) {
        QJsonObject stateErr = json["StateAndError"].toObject();
        
        if (stateErr.contains("robotState")) {
            m_Datasheet_Data.robotState = stateErr["robotState"].toInt();
        }
        
        if (stateErr.contains("robotEnabled")) {
            m_Datasheet_Data.robotEnabled = stateErr["robotEnabled"].toInt();
        }
        
        if (stateErr.contains("robotPaused")) {
            m_Datasheet_Data.robotPaused = stateErr["robotPaused"].toInt();
        }
        
        if (stateErr.contains("robotMoving")) {
            m_Datasheet_Data.robotMoving = stateErr["robotMoving"].toInt();
        }
        
        if (stateErr.contains("robotBlendingDone")) {
            m_Datasheet_Data.robotBlendingDone = stateErr["robotBlendingDone"].toInt();
        }
        
        if (stateErr.contains("InPos")) {
            m_Datasheet_Data.InPos = stateErr["InPos"].toInt();
        }
        
        if (stateErr.contains("Error_AxisID")) {
            m_Datasheet_Data.Error_AxisID = stateErr["Error_AxisID"].toInt();
        }
        
        if (stateErr.contains("Error_Code")) {
            m_Datasheet_Data.Error_Code = stateErr["Error_Code"].toInt();
        }
        
        if (stateErr.contains("IsReduceMode")) {
            m_Datasheet_Data.IsReduceMode = stateErr["IsReduceMode"].toInt();
        }
        
        if (stateErr.contains("IsFreeDriveMode")) {
            m_Datasheet_Data.IsFreeDriveMode = stateErr["IsFreeDriveMode"].toInt();
        }
        
        if (stateErr.contains("AutoMode")) {
            m_Datasheet_Data.AutoMode = stateErr["AutoMode"].toInt();
        }
        
        if (stateErr.contains("BrakeState") && stateErr["BrakeState"].isArray()) {
            QJsonArray brakeArray = stateErr["BrakeState"].toArray();
            for (int i = 0; i < 6 && i < brakeArray.size(); ++i) {
                m_Datasheet_Data.BrakeState[i] = brakeArray[i].toInt();
            }
        }
        
        if (stateErr.contains("nAxisStatus") && stateErr["nAxisStatus"].isArray()) {
            QJsonArray statusArray = stateErr["nAxisStatus"].toArray();
            for (int i = 0; i < 6 && i < statusArray.size(); ++i) {
                m_Datasheet_Data.nAxisStatus[i] = statusArray[i].toInt();
            }
        }
        
        if (stateErr.contains("nAxisErrorCode") && stateErr["nAxisErrorCode"].isArray()) {
            QJsonArray errArray = stateErr["nAxisErrorCode"].toArray();
            for (int i = 0; i < 6 && i < errArray.size(); ++i) {
                m_Datasheet_Data.nAxisErrorCode[i] = errArray[i].toInt();
            }
        }
        
        if (stateErr.contains("nResetSafeSpace") && stateErr["nResetSafeSpace"].isArray()) {
            QJsonArray resetArray = stateErr["nResetSafeSpace"].toArray();
            for (int i = 0; i < 1 && i < resetArray.size(); ++i) {
                m_Datasheet_Data.nResetSafeSpace[i] = resetArray[i].toInt();
            }
        }
        
        if (stateErr.contains("nAxisGroupStatus") && stateErr["nAxisGroupStatus"].isArray()) {
            QJsonArray groupStatusArray = stateErr["nAxisGroupStatus"].toArray();
            for (int i = 0; i < 1 && i < groupStatusArray.size(); ++i) {
                m_Datasheet_Data.nAxisGroupStatus[i] = groupStatusArray[i].toInt();
            }
        }
        
        if (stateErr.contains("nAxisGroupErrorCode") && stateErr["nAxisGroupErrorCode"].isArray()) {
            QJsonArray groupErrArray = stateErr["nAxisGroupErrorCode"].toArray();
            for (int i = 0; i < 1 && i < groupErrArray.size(); ++i) {
                m_Datasheet_Data.nAxisGroupErrorCode[i] = groupErrArray[i].toInt();
            }
        }
    }
}

void c_Huayan_Monitor::Parse_FTData(const QJsonObject& json)
{
    if (json.contains("FTData") && json["FTData"].isObject()) {
        QJsonObject ftData = json["FTData"].toObject();
        
        if (ftData.contains("FTControlState")) {
            m_Datasheet_Data.FTControlState = ftData["FTControlState"].toInt();
        }
        
        if (ftData.contains("FTData") && ftData["FTData"].isArray()) {
            QJsonArray ftArray = ftData["FTData"].toArray();
            for (int i = 0; i < 6 && i < ftArray.size(); ++i) {
                m_Datasheet_Data.FTData[i] = ftArray[i].toDouble();
            }
        }
        
        if (ftData.contains("FTSrcData") && ftData["FTSrcData"].isArray()) {
            QJsonArray srcArray = ftData["FTSrcData"].toArray();
            for (int i = 0; i < 6 && i < srcArray.size(); ++i) {
                m_Datasheet_Data.FTSrcData[i] = srcArray[i].toDouble();
            }
        }
    }
}

void c_Huayan_Monitor::Parse_HardLoad(const QJsonObject& json)
{
    if (json.contains("HardLoad") && json["HardLoad"].isObject()) {
        QJsonObject hardLoad = json["HardLoad"].toObject();
        
        if (hardLoad.contains("EtherCAT_TotalFrame")) {
            m_Datasheet_Data.EtherCAT_TotalFrame = hardLoad["EtherCAT_TotalFrame"].toInt();
        }
        
        if (hardLoad.contains("EtherCAT_FramesPerSecond")) {
            m_Datasheet_Data.EtherCAT_FramesPerSecond = hardLoad["EtherCAT_FramesPerSecond"].toInt();
        }
        
        if (hardLoad.contains("EtherCAT_TotalLostFrame")) {
            m_Datasheet_Data.EtherCAT_TotalLostFrame = hardLoad["EtherCAT_TotalLostFrame"].toInt();
        }
        
        if (hardLoad.contains("EtherCAT_TxErrorFrame")) {
            m_Datasheet_Data.EtherCAT_TxErrorFrame = hardLoad["EtherCAT_TxErrorFrame"].toInt();
        }
        
        if (hardLoad.contains("EtherCAT_RxErrorFrame")) {
            m_Datasheet_Data.EtherCAT_RxErrorFrame = hardLoad["EtherCAT_RxErrorFrame"].toInt();
        }
        
        if (hardLoad.contains("Box48IN_Voltage")) {
            m_Datasheet_Data.Box48IN_Voltage = hardLoad["Box48IN_Voltage"].toDouble();
        }
        
        if (hardLoad.contains("Box48IN_Current")) {
            m_Datasheet_Data.Box48IN_Current = hardLoad["Box48IN_Current"].toDouble();
        }
        
        if (hardLoad.contains("Box48Out_Voltage")) {
            m_Datasheet_Data.Box48Out_Voltage = hardLoad["Box48Out_Voltage"].toDouble();
        }
        
        if (hardLoad.contains("Box48Out_Current")) {
            m_Datasheet_Data.Box48Out_Current = hardLoad["Box48Out_Current"].toDouble();
        }
        
        // ⚠️ V6 改为 6 个模组 (协议 5.2 节)
        if (hardLoad.contains("joint_temperature") && hardLoad["joint_temperature"].isArray()) {
            QJsonArray tempArray = hardLoad["joint_temperature"].toArray();
            for (int i = 0; i < 6 && i < tempArray.size(); ++i) {
                m_Datasheet_Data.joint_temperature[i] = tempArray[i].toDouble();
            }
        }
        
        if (hardLoad.contains("joint_Voltage") && hardLoad["joint_Voltage"].isArray()) {
            QJsonArray voltArray = hardLoad["joint_Voltage"].toArray();
            for (int i = 0; i < 6 && i < voltArray.size(); ++i) {
                m_Datasheet_Data.joint_Voltage[i] = voltArray[i].toDouble();
            }
        }
    }
}

void c_Huayan_Monitor::Parse_Modbus(const QJsonObject& json)
{
    if (json.contains("modbus") && json["modbus"].isObject()) {
        QJsonObject modbusObj = json["modbus"].toObject();
        if (modbusObj.contains("Modbus_1") && modbusObj["Modbus_1"].isObject()) {
            QJsonObject modbus1 = modbusObj["Modbus_1"].toObject();
            
            if (modbus1.contains("connectStatus")) {
                m_Datasheet_Data.Modbus_connectStatus = modbus1["connectStatus"].toString().toInt();
            }
            
            if (modbus1.contains("errorCode")) {
                m_Datasheet_Data.Modbus_errorCode = modbus1["errorCode"].toString().toInt();
            }
        }
    }
}

void c_Huayan_Monitor::Parse_RobotAuthorization(const QJsonObject& json)
{
    if (json.contains("RobotAuthorization") && json["RobotAuthorization"].isObject()) {
        QJsonObject authObj = json["RobotAuthorization"].toObject();
        
        if (authObj.contains("LangnDeviceCode")) {
            m_Datasheet_Data.LangnDeviceCode = authObj["LangnDeviceCode"].toString();
        }
        if (authObj.contains("LangAuthStatus")) {
            m_Datasheet_Data.LangAuthStatus = authObj["LangAuthStatus"].toString();
        }
        if (authObj.contains("LangAuthData")) {
            m_Datasheet_Data.LangAuthData = authObj["LangAuthData"].toString();
        }
        if (authObj.contains("DeviceMac")) {
            m_Datasheet_Data.DeviceMac = authObj["DeviceMac"].toString();
        }
        if (authObj.contains("DeviceSN")) {
            m_Datasheet_Data.DeviceSN = authObj["DeviceSN"].toString();
        }
        if (authObj.contains("SystemDate")) {
            m_Datasheet_Data.SystemDate = authObj["SystemDate"].toString();
        }
        
        if (authObj.contains("DynDeviceCode")) {
            m_Datasheet_Data.DynDeviceCode = authObj["DynDeviceCode"].toString();
        }
        if (authObj.contains("AuthorizedTimeLeftMinutes")) {
            m_Datasheet_Data.AuthorizedTimeLeftMinutes = authObj["AuthorizedTimeLeftMinutes"].toString();
        }
        if (authObj.contains("AuthorizedTimeUsedMinutes")) {
            m_Datasheet_Data.AuthorizedTimeUsedMinutes = authObj["AuthorizedTimeUsedMinutes"].toString();
        }
    }
}

void c_Huayan_Monitor::Parse_SafePlane(const QJsonObject& json)
{
    if (json.contains("SafePlane") && json["SafePlane"].isArray()) {
        QJsonArray safePlaneArray = json["SafePlane"].toArray();
        if (safePlaneArray.size() > 0 && safePlaneArray[0].isObject()) {
            QJsonObject safePlaneObj = safePlaneArray[0].toObject();
            
            if (safePlaneObj.contains("Name")) {
                m_Datasheet_Data.SafePlane_Name = safePlaneObj["Name"].toString();
            }
            
            if (safePlaneObj.contains("Distance")) {
                m_Datasheet_Data.SafePlane_Distance = safePlaneObj["Distance"].toString().toDouble();
            }
            
            if (safePlaneObj.contains("Trendency")) {
                m_Datasheet_Data.SafePlane_Trendency = safePlaneObj["Trendency"].toString().toInt();
            }
        }
    }
}

void c_Huayan_Monitor::Parse_ConstraintArea(const QJsonObject& json)
{
    if (json.contains("ConstraintArea") && json["ConstraintArea"].isArray()) {
        QJsonArray constraintArray = json["ConstraintArea"].toArray();
        if (constraintArray.size() > 0 && constraintArray[0].isObject()) {
            QJsonObject constraintObj = constraintArray[0].toObject();
            
            if (constraintObj.contains("Name")) {
                m_Datasheet_Data.ConstraintArea_Name = constraintObj["Name"].toString();
            }
            
            if (constraintObj.contains("Distance")) {
                m_Datasheet_Data.ConstraintArea_Distance = constraintObj["Distance"].toString().toDouble();
            }
            
            if (constraintObj.contains("Trendency")) {
                m_Datasheet_Data.ConstraintArea_Trendency = constraintObj["Trendency"].toString().toInt();
            }
        }
    }
}

void c_Huayan_Monitor::Parse_Script(const QJsonObject& json)
{
    if (json.contains("Script") && json["Script"].isObject()) {
        QJsonObject scriptObj = json["Script"].toObject();
        
        if (scriptObj.contains("ScriptChanged")) {
            m_Datasheet_Data.ScriptChanged = scriptObj["ScriptChanged"].toString().toInt();
        }
        
        if (scriptObj.contains("errorCode")) {
            m_Datasheet_Data.Script_errorCode = scriptObj["errorCode"].toString();
        }
        
        if (scriptObj.contains("cmdid") && scriptObj["cmdid"].isArray()) {
            QJsonArray cmdIdArray = scriptObj["cmdid"].toArray();
            for (int i = 0; i < 6 && i < cmdIdArray.size(); ++i) {
                m_Datasheet_Data.Script_cmdid[i] = cmdIdArray[i].toString();
            }
        }
        
        // ⚠️ 修复：协议示例显示 callstackcmdid 为数组
        if (scriptObj.contains("callstackcmdid") && scriptObj["callstackcmdid"].isArray()) {
            QJsonArray callstackArray = scriptObj["callstackcmdid"].toArray();
            for (int i = 0; i < 6 && i < callstackArray.size(); ++i) {
                m_Datasheet_Data.Script_callstackcmdid[i] = callstackArray[i].toString();
            }
        }
    }
}

void c_Huayan_Monitor::Parse_MsgTitle(const QJsonObject& json)
{
    if (json.contains("MsgTitle") && json["MsgTitle"].isObject()) {
        QJsonObject msgTitleObj = json["MsgTitle"].toObject();
        
        if (msgTitleObj.contains("Stamp")) {
            m_Datasheet_Data.Stamp = msgTitleObj["Stamp"].toString();
        }
        
        if (msgTitleObj.contains("UpdateTime")) {
            m_Datasheet_Data.UpdateTime = msgTitleObj["UpdateTime"].toString();
        }
    }
}