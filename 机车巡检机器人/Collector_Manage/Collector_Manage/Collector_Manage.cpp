#include "Collector_Manage.h"

c_Collector_Manage::c_Collector_Manage(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);
	{
		QFile File(QDir::currentPath() + "/stuqss.css");
		File.open(QIODevice::ReadOnly);
		QString strQss = File.readAll();
		this->setStyleSheet(strQss);
		File.close();
	}
	{
		m_Debug_Path = QDir::currentPath() + "/Debug_DB/" + QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
		QDir Dir;
		bool exist = Dir.exists(m_Debug_Path);
		if (!exist) { Dir.mkdir(m_Debug_Path); }
	}
	{
		QFile File(QDir::currentPath() + "/Collector_Manage.json");
		File.open(QFile::ReadOnly | QIODevice::Text);
		//读取文件
		QByteArray Data = File.readAll();
		//转换JSON
		QJsonParseError parseError;  // JSON解析错误
		QJsonDocument DB_Doc = QJsonDocument::fromJson(Data, &parseError);  // 解析JSON
		if (parseError.error == QJsonParseError::NoError) {
			c_Variable::getInstance().g_Communicate_DB = DB_Doc.object();
		}
		//关闭文件
		File.close();
	}

	QObject::connect(ui.Zivid_Camera_120_Show, &QPushButton::clicked, this, [=]() {
		ui.stackedWidget->setCurrentWidget(ui.Zivid_120_Camera_Widget);
	});
	QObject::connect(ui.Zivid_Camera_121_Show, &QPushButton::clicked, this, [=]() {
		ui.stackedWidget->setCurrentWidget(ui.Zivid_121_Camera_Widget);
	});
	QObject::connect(ui.Robot_Sense_Show, &QPushButton::clicked, this, [=]() {
		ui.stackedWidget->setCurrentWidget(ui.Robot_Sense_Widget);
	});

	QObject::connect(ui.Work_Widget_Show, &QPushButton::clicked, this, [=]() {
		ui.stackedWidget->setCurrentWidget(ui.Work_Widget);
	});
	QObject::connect(ui.Huayan_Show, &QPushButton::clicked, this, [=]() {
		ui.stackedWidget->setCurrentWidget(ui.Huayan_Robot_Widget);
	});
	QObject::connect(ui.Tran_Photo_Show, &QPushButton::clicked, this, [=]() {
		ui.stackedWidget->setCurrentWidget(ui.Tran_Photo);
	});
	QObject::connect(ui.AGV_Show, &QPushButton::clicked, this, [=]() {
		ui.stackedWidget->setCurrentWidget(ui.AGV_Widget);
	});
	QObject::connect(ui.Auto_Show, &QPushButton::clicked, this, [=]() {
		ui.stackedWidget->setCurrentWidget(ui.Auto_Widget);
	});
	QObject::connect(ui.Tracking_Show, &QPushButton::clicked, this, [=]() {
		ui.stackedWidget->setCurrentWidget(ui.Tracking_Widget);
	});

	this->showMaximized();
     
	ui.Worry_List->document()->setMaximumBlockCount(5000000);
	ui.RGV_List->document()->setMaximumBlockCount(5000000);
	ui.Work_List->document()->setMaximumBlockCount(5000000);

	m_Scan = true;

	c_Collector_Manage::RGV_Init();

	c_Collector_Manage::Huayan_120_Init();
	c_Collector_Manage::Huayan_121_Init();

	c_Collector_Manage::Zivid_1_Init();
	c_Collector_Manage::Zivid_2_Init();

	c_Collector_Manage::E1R_1_Init();
	c_Collector_Manage::E1R_2_Init();

	c_Collector_Manage::RealSense_1_Init();
	c_Collector_Manage::RealSense_2_Init();

	c_Collector_Manage::Hikvision_Init();

	c_Collector_Manage::CH10X_Init();

	c_Collector_Manage::Work_Remote_Init();

	c_Collector_Manage::System_Scan();

	m_Time.start();
}

c_Collector_Manage::~c_Collector_Manage()
{

}

void c_Collector_Manage::System_Scan()
{
	if (!m_Scan) {
		return;
	}

	c_Collector_Manage::Huayan_120_Scan();
	c_Collector_Manage::Huayan_121_Scan();
	c_Collector_Manage::Zivid_1_Scan();
	c_Collector_Manage::Zivid_2_Scan();
	c_Collector_Manage::CH10X_Scan();
	c_Collector_Manage::RGV_Scan();
	c_Collector_Manage::Work_Remote_Scan();

	ui.Status_Bar->showMessage(
		"系统时间：" + QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss")
		+ "------" +
		"刷新帧率：" + QString::number(m_Current_FPS) + "HZ"
	);

	if (m_FPS == 100) {
		m_Current_FPS = m_Time.restart() / 100;
		m_Current_FPS = 1000 / m_Current_FPS;
		m_FPS = 0;
	}
	m_FPS += 1;
	QTimer::singleShot(10, this, [=](){
		if (m_Scan) { c_Collector_Manage::System_Scan(); }
	});
}

void c_Collector_Manage::RGV_Init()
{
	m_RGV_Remote_Thread = new QThread;
	m_RGV_Remote = new c_RGV_Remote;
	m_RGV_Remote->moveToThread(m_RGV_Remote_Thread);
	QObject::connect(m_RGV_Remote_Thread, &QThread::started, m_RGV_Remote, &c_RGV_Remote::Init);
	QObject::connect(m_RGV_Remote_Thread, &QThread::finished, m_RGV_Remote, &c_RGV_Remote::deleteLater);

	QObject::connect(m_RGV_Remote, &c_RGV_Remote::Status, this, &c_Collector_Manage::Write_RGV_List);

	c_Collector_Manage::RGV_DB();
	c_Collector_Manage::RGV_Button();
	c_Collector_Manage::RGV_Scan();

	m_RGV_Remote_Thread->start();
}
void c_Collector_Manage::RGV_DB()
{
	ui.RGV_Ip->setText(c_Variable::getInstance().g_Communicate_DB.value("RGV_Ip").toString());
	ui.RGV_Port->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("RGV_Port").toInt()));

	ui.Write_Coils_Size->setValue(c_Variable::getInstance().g_Communicate_DB.value("Write_Coils_Size").toInt());
	ui.Read_DiscreteInputs_Size->setValue(c_Variable::getInstance().g_Communicate_DB.value("Read_DiscreteInputs_Size").toInt());
	ui.Read_InputRegisters_Size->setValue(c_Variable::getInstance().g_Communicate_DB.value("Read_InputRegisters_Size").toInt());
	ui.Write_HoldingRegisters_Size->setValue(c_Variable::getInstance().g_Communicate_DB.value("Write_HoldingRegisters_Size").toInt());
	ui.Write_Coils_Addr->setValue(c_Variable::getInstance().g_Communicate_DB.value("Write_Coils_Addr").toInt());
	ui.Read_DiscreteInputs_Addr->setValue(c_Variable::getInstance().g_Communicate_DB.value("Read_DiscreteInputs_Addr").toInt());
	ui.Read_DiscreteInputs_Addr->setValue(c_Variable::getInstance().g_Communicate_DB.value("Read_DiscreteInputs_Addr").toInt());
	ui.Write_HoldingRegisters_Addr->setValue(c_Variable::getInstance().g_Communicate_DB.value("Write_HoldingRegisters_Addr").toInt());

	QObject::connect(ui.RGV_Ip, &QLineEdit::textChanged, this, [=](QString ip) {Write_Communicate_DB("RGV_Ip", ip); });
	QObject::connect(ui.RGV_Port, &QLineEdit::textChanged, this, [=](QString port) {Write_Communicate_DB("RGV_Port", port.toInt()); });

	QObject::connect(ui.Write_Coils_Size, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int Write_Coils_Size) {Write_Communicate_DB("Write_Coils_Size", Write_Coils_Size); });
	QObject::connect(ui.Read_DiscreteInputs_Size, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int Read_DiscreteInputs_Size) {Write_Communicate_DB("Read_DiscreteInputs_Size", Read_DiscreteInputs_Size); });
	QObject::connect(ui.Read_InputRegisters_Size, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int Read_InputRegisters_Size) {Write_Communicate_DB("Read_InputRegisters_Size", Read_InputRegisters_Size); });
	QObject::connect(ui.Write_HoldingRegisters_Size, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int Write_HoldingRegisters_Size) {Write_Communicate_DB("Write_HoldingRegisters_Size", Write_HoldingRegisters_Size); });
	QObject::connect(ui.Write_Coils_Addr, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int Write_Coils_Addr) {Write_Communicate_DB("Write_Coils_Addr", Write_Coils_Addr); });
	QObject::connect(ui.Read_DiscreteInputs_Addr, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int Read_DiscreteInputs_Addr) {Write_Communicate_DB("Read_DiscreteInputs_Addr", Read_DiscreteInputs_Addr); });
	QObject::connect(ui.Read_InputRegisters_Addr, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int Read_InputRegisters_Addr) {Write_Communicate_DB("Read_InputRegisters_Addr", Read_InputRegisters_Addr); });
	QObject::connect(ui.Write_HoldingRegisters_Addr, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int Write_HoldingRegisters_Addr) {Write_Communicate_DB("Write_HoldingRegisters_Addr", Write_HoldingRegisters_Addr); });
}
void c_Collector_Manage::RGV_Button()
{
	QObject::connect(ui.RGV_Connect, &QPushButton::clicked, m_RGV_Remote, &c_RGV_Remote::Connect);
	QObject::connect(ui.RGV_Disconnect, &QPushButton::clicked, m_RGV_Remote, &c_RGV_Remote::Disconnect_Device);
}
void c_Collector_Manage::RGV_Scan()
{
	ui.RGV_Ready->Set_State(c_Variable::getInstance().g_RGV.Ready);
}
void c_Collector_Manage::RGV_Delete()
{
	if (m_RGV_Remote_Thread->isRunning()) {
		//线程中断
		m_RGV_Remote_Thread->requestInterruption();
		//线程退出
		m_RGV_Remote_Thread->quit();
		//线程等待
		m_RGV_Remote_Thread->wait();
	}
}

void c_Collector_Manage::Huayan_120_Init()
{
 	m_Huayan_Monitor_120_Thread = new QThread;
	m_Huayan_Monitor_120 = new c_Huayan_Monitor_120;
	m_Huayan_Monitor_120->moveToThread(m_Huayan_Monitor_120_Thread);

	m_Huayan_Remote_120_Thread = new QThread;
	m_Huayan_Remote_120 = new c_Huayan_Remote_120;
	m_Huayan_Remote_120->moveToThread(m_Huayan_Remote_120_Thread);

	QObject::connect(m_Huayan_Monitor_120_Thread, &QThread::started, m_Huayan_Monitor_120, &c_Huayan_Monitor_120::Init);
	QObject::connect(m_Huayan_Monitor_120_Thread, &QThread::finished, m_Huayan_Monitor_120, &c_Huayan_Monitor_120::deleteLater);

	QObject::connect(m_Huayan_Remote_120_Thread, &QThread::started, m_Huayan_Remote_120, &c_Huayan_Remote_120::Init);
	QObject::connect(m_Huayan_Remote_120_Thread, &QThread::finished, m_Huayan_Remote_120, &c_Huayan_Remote_120::deleteLater);

	QObject::connect(m_Huayan_Monitor_120, &c_Huayan_Monitor_120::Status, this, [=](QString state){
        Write_Worry_List(QString("左臂监视端口[10005] :%1").arg(state));
    });
	QObject::connect(m_Huayan_Remote_120, &c_Huayan_Remote_120::Status, this, [=](QString state){
        Write_Worry_List(QString("左臂控制端口[10003] :%1").arg(state));
    });

	// ===== 4. 连接 Remote 的命令响应与错误信号 =====
    QObject::connect(m_Huayan_Remote_120, &c_Huayan_Remote_120::Command_Response, this, 
        [=](QString cmdName, bool success, QString response, QString cmdID){
        if (success) {
            Write_Worry_List(QString("左臂 [%1] 执行成功：%2").arg(cmdName).arg(cmdID));
        } else {
            Write_Worry_List(QString("左臂 [%1] 执行失败：%2").arg(cmdName).arg(response));
        }
    });
    
    QObject::connect(m_Huayan_Remote_120, &c_Huayan_Remote_120::Error_Occurred, this,
        [=](int errorCode, QString errorMsg){
        Write_Worry_List(QString("左臂错误 [%1]: %2").arg(errorCode).arg(errorMsg));
        ui.Huayan_120_ErrorValue->setText(QString::number(errorCode));
    });
    
    // ===== 5. 连接 RunFunc 专用信号 =====
    QObject::connect(m_Huayan_Remote_120, &c_Huayan_Remote_120::RunFunc_Response, this,
        [=](QString funcName, bool success, QString errorMsg){
        if (success) {
            Write_Worry_List(QString("左臂：函数 [%1] 执行成功").arg(funcName));
        } else {
            Write_Worry_List(QString("左臂：函数 [%1] 执行失败：%2").arg(funcName).arg(errorMsg));
        }
    });

	c_Collector_Manage::Huayan_120_DB();
	c_Collector_Manage::Huayan_120_Button();
	c_Collector_Manage::Huayan_120_Scan();

	m_Huayan_Monitor_120_Thread->start();
	m_Huayan_Remote_120_Thread->start();
}
void c_Collector_Manage::Huayan_120_DB()
{
    // ===== 1. UI 输入框 <-> JSON 配置 双向绑定 =====
    // IP 地址
    QObject::connect(ui.Huayan_120_IP, &QLineEdit::textChanged, this, [=](QString ip){
        Write_Communicate_DB("Huayan_120_IP", ip);
    });
    // 控制端口 (10003)
    QObject::connect(ui.Huayan_120_RemotePort, &QLineEdit::textChanged, this, [=](QString port){
        Write_Communicate_DB("Huayan_120_RemotePort", port.toInt());
    });
    // 监控端口 (10005)
    QObject::connect(ui.Huayan_120_MonitorPort, &QLineEdit::textChanged, this, [=](QString port){
        Write_Communicate_DB("Huayan_120_MonitorPort", port.toInt());
    });

    // ===== 2. 从全局配置初始化 UI 显示 =====
    ui.Huayan_120_IP->setText(c_Variable::getInstance().g_Communicate_DB.value("Huayan_120_IP").toString());
    ui.Huayan_120_RemotePort->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("Huayan_120_RemotePort").toInt()));
    ui.Huayan_120_MonitorPort->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("Huayan_120_MonitorPort").toInt()));
}
void c_Collector_Manage::Huayan_120_Button()
{
    // ===== 连接控制 =====
    QObject::connect(ui.Huayan_120_Connect, &QPushButton::clicked, m_Huayan_Monitor_120, &c_Huayan_Monitor_120::Connect);
    QObject::connect(ui.Huayan_120_Connect, &QPushButton::clicked, m_Huayan_Remote_120, &c_Huayan_Remote_120::Connect);
    QObject::connect(ui.Huayan_120_Disconnect, &QPushButton::clicked, m_Huayan_Monitor_120, &c_Huayan_Monitor_120::Disconnect);
    QObject::connect(ui.Huayan_120_Disconnect, &QPushButton::clicked, m_Huayan_Remote_120, &c_Huayan_Remote_120::Disconnect);
    
    // ===== 电源控制 =====
    QObject::connect(ui.Huayan_120_Electrify, &QPushButton::clicked, m_Huayan_Remote_120, &c_Huayan_Remote_120::Electrify);
	QObject::connect(ui.Huayan_120_AutoDn, &QPushButton::clicked, m_Huayan_Remote_120, &c_Huayan_Remote_120::AutoDn);
	QObject::connect(ui.Huayan_120_AutoEn, &QPushButton::clicked, m_Huayan_Remote_120, &c_Huayan_Remote_120::AutoEn);
    QObject::connect(ui.Huayan_120_BlackOut, &QPushButton::clicked, m_Huayan_Remote_120, &c_Huayan_Remote_120::BlackOut);
    
    // ===== 使能控制 =====
    QObject::connect(ui.Huayan_120_Enable, &QPushButton::clicked, m_Huayan_Remote_120, &c_Huayan_Remote_120::GrpEnable);
    QObject::connect(ui.Huayan_120_Disable, &QPushButton::clicked, m_Huayan_Remote_120, &c_Huayan_Remote_120::GrpDisable);
    
    // ===== 脚本控制 =====
    QObject::connect(ui.Huayan_120_StartScript, &QPushButton::clicked, m_Huayan_Remote_120, &c_Huayan_Remote_120::StartScript);
    QObject::connect(ui.Huayan_120_StopScript, &QPushButton::clicked, m_Huayan_Remote_120, &c_Huayan_Remote_120::StopScript);
    QObject::connect(ui.Huayan_120_PauseScript, &QPushButton::clicked, m_Huayan_Remote_120, &c_Huayan_Remote_120::PauseScript);
    QObject::connect(ui.Huayan_120_ContinueScript, &QPushButton::clicked, m_Huayan_Remote_120, &c_Huayan_Remote_120::ContinueScript);
    
    // ===== 运行脚本函数 (RunFunc) =====
    QObject::connect(ui.Huayan_120_RunFunc, &QPushButton::clicked, this, [=](){
        QString funcName = ui.Huayan_120_FuncName->text();
		Write_Worry_List(QString("左臂：运行脚本函数 [%1]").arg(funcName));
        if (!funcName.isEmpty()) {
            QStringList params;
             // 跨线程调用发送（确保在 TCP_Client 所在线程执行）
    		QMetaObject::invokeMethod(m_Huayan_Remote_120, "RunFunc", Qt::QueuedConnection,
                Q_ARG(QString, funcName),Q_ARG(QStringList, params));
        }
    });
    
	 // ===== 运行脚本函数 (SetOverride) =====
    QObject::connect(ui.Huayan_120_SetOverride, &QPushButton::clicked, this, [=](){
        QString speed = ui.Huayan_120_Speed->text();
		Write_Worry_List(QString("左臂：运行速度 [%1]").arg(speed));
        if (!speed.isEmpty()) {
            QStringList params;
            // 跨线程调用发送（确保在 TCP_Client 所在线程执行）
    		QMetaObject::invokeMethod(m_Huayan_Remote_120, "SetOverride", Qt::QueuedConnection,
                Q_ARG(int, 0), Q_ARG(QString, speed));
        }
    });

    // ===== 安全控制 =====
    QObject::connect(ui.Huayan_120_EmergencyStop, &QPushButton::clicked, m_Huayan_Remote_120, &c_Huayan_Remote_120::EmergencyStop);
    QObject::connect(ui.Huayan_120_Reset, &QPushButton::clicked, m_Huayan_Remote_120, &c_Huayan_Remote_120::GrpReset);
    QObject::connect(ui.Huayan_120_Home, &QPushButton::clicked, m_Huayan_Remote_120, &c_Huayan_Remote_120::MoveJToHome);

	QObject::connect(ui.Huayan_120_Connected, &c_Fr_Light::Working_State, ui.Huayan_120_Connect, &QPushButton::setDisabled);
	QObject::connect(ui.Huayan_120_Remote_Connected, &c_Fr_Light::Working_State, ui.Huayan_120_Disconnect, &QPushButton::setEnabled);
	QObject::connect(ui.Huayan_120_Connected, &c_Fr_Light::Default_State, ui.Huayan_120_Connect, &QPushButton::setEnabled);
	QObject::connect(ui.Huayan_120_Remote_Connected, &c_Fr_Light::Default_State, ui.Huayan_120_Disconnect, &QPushButton::setDisabled);
}
void c_Collector_Manage::Huayan_120_Scan()
{
    // 10ms 周期调用，只更新轻量级状态，避免频繁刷新导致界面卡顿
	ui.Huayan_120_Connected->Set_State(c_Variable::getInstance().g_Huayan_120.device_Status);
	ui.Huayan_120_Remote_Connected->Set_State(c_Variable::getInstance().g_Huayan_120.Remote_connected);
    ui.Huayan_120_Monitor_Connected->Set_State(c_Variable::getInstance().g_Huayan_120.Monitor_connected);

    // 更新状态机显示
    ui.Huayan_120_StateValue->setText(m_Object.Huayan_Status(c_Variable::getInstance().g_Datasheet_120.robotState));
    // 更新使能状态
    ui.Huayan_120_EnabledValue->setText(c_Variable::getInstance().g_Datasheet_120.robotEnabled ? "已使能" : "未使能");
    // 更新运动状态
    ui.Huayan_120_MovingValue->setText(c_Variable::getInstance().g_Datasheet_120.robotMoving ? "运动中" : "静止");
	ui.Huayan_120_Moving->Set_State(c_Variable::getInstance().g_Datasheet_120.robotMoving);
    // 更新错误码显示 (有错误时标红)
    if (c_Variable::getInstance().g_Datasheet_120.Error_Code != 0) {
        ui.Huayan_120_ErrorValue->setText(QString::number(c_Variable::getInstance().g_Datasheet_120.Error_Code));
        ui.Huayan_120_ErrorValue->setStyleSheet("color: red; font-weight: bold;");
    } else {
        ui.Huayan_120_ErrorValue->setText("0");
        ui.Huayan_120_ErrorValue->setStyleSheet("");
    }
    // 更新关节位置 (°) - 保留 2 位小数
    ui.Huayan_120_J1->setText(QString::number(c_Variable::getInstance().g_Datasheet_120.Actual_Position[0], 'f', 2));
    ui.Huayan_120_J2->setText(QString::number(c_Variable::getInstance().g_Datasheet_120.Actual_Position[1], 'f', 2));
    ui.Huayan_120_J3->setText(QString::number(c_Variable::getInstance().g_Datasheet_120.Actual_Position[2], 'f', 2));
    ui.Huayan_120_J4->setText(QString::number(c_Variable::getInstance().g_Datasheet_120.Actual_Position[3], 'f', 2));
    ui.Huayan_120_J5->setText(QString::number(c_Variable::getInstance().g_Datasheet_120.Actual_Position[4], 'f', 2));
    ui.Huayan_120_J6->setText(QString::number(c_Variable::getInstance().g_Datasheet_120.Actual_Position[5], 'f', 2));     
    // 更新 TCP 位置 (mm/°)
    ui.Huayan_120_X->setText(QString::number(c_Variable::getInstance().g_Datasheet_120.Actual_PCS_TCP[0], 'f', 2));
    ui.Huayan_120_Y->setText(QString::number(c_Variable::getInstance().g_Datasheet_120.Actual_PCS_TCP[1], 'f', 2));
    ui.Huayan_120_Z->setText(QString::number(c_Variable::getInstance().g_Datasheet_120.Actual_PCS_TCP[2], 'f', 2));
    ui.Huayan_120_Rx->setText(QString::number(c_Variable::getInstance().g_Datasheet_120.Actual_PCS_TCP[3], 'f', 2));
    ui.Huayan_120_Ry->setText(QString::number(c_Variable::getInstance().g_Datasheet_120.Actual_PCS_TCP[4], 'f', 2));
    ui.Huayan_120_Rz->setText(QString::number(c_Variable::getInstance().g_Datasheet_120.Actual_PCS_TCP[5], 'f', 2));    
}
void c_Collector_Manage::Huayan_120_Delete()
{
 if (m_Huayan_Monitor_120_Thread->isRunning()) {
		//线程中断
		m_Huayan_Monitor_120_Thread->requestInterruption();
		//线程退出
		m_Huayan_Monitor_120_Thread->quit();
		//线程等待
		m_Huayan_Monitor_120_Thread->wait();
	}
	if (m_Huayan_Remote_120_Thread->isRunning()) {
		//线程中断
		m_Huayan_Remote_120_Thread->requestInterruption();
		//线程退出
		m_Huayan_Remote_120_Thread->quit();
		//线程等待
		m_Huayan_Remote_120_Thread->wait();
	}
}

void c_Collector_Manage::Huayan_121_Init()
{
 	m_Huayan_Monitor_121_Thread = new QThread;
	m_Huayan_Monitor_121 = new c_Huayan_Monitor_121;
	m_Huayan_Monitor_121->moveToThread(m_Huayan_Monitor_121_Thread);

	m_Huayan_Remote_121_Thread = new QThread;
	m_Huayan_Remote_121 = new c_Huayan_Remote_121;
	m_Huayan_Remote_121->moveToThread(m_Huayan_Remote_121_Thread);

	QObject::connect(m_Huayan_Monitor_121_Thread, &QThread::started, m_Huayan_Monitor_121, &c_Huayan_Monitor_121::Init);
	QObject::connect(m_Huayan_Monitor_121_Thread, &QThread::finished, m_Huayan_Monitor_121, &c_Huayan_Monitor_121::deleteLater);

	QObject::connect(m_Huayan_Remote_121_Thread, &QThread::started, m_Huayan_Remote_121, &c_Huayan_Remote_121::Init);
	QObject::connect(m_Huayan_Remote_121_Thread, &QThread::finished, m_Huayan_Remote_121, &c_Huayan_Remote_121::deleteLater);

	QObject::connect(m_Huayan_Monitor_121, &c_Huayan_Monitor_121::Status, this, [=](QString state){
        Write_Worry_List(QString("右臂监视端口[10005] :%1").arg(state));
    });
	QObject::connect(m_Huayan_Remote_121, &c_Huayan_Remote_121::Status, this, [=](QString state){
        Write_Worry_List(QString("右臂控制端口[10003] :%1").arg(state));
    });

	// ===== 4. 连接 Remote 的命令响应与错误信号 =====
    QObject::connect(m_Huayan_Remote_121, &c_Huayan_Remote_121::Command_Response, this, 
        [=](QString cmdName, bool success, QString response, QString cmdID){
        if (success) {
            Write_Worry_List(QString("左臂 [%1] 执行成功：%2").arg(cmdName).arg(cmdID));
        } else {
            Write_Worry_List(QString("左臂 [%1] 执行失败：%2").arg(cmdName).arg(response));
        }
    });
    
    QObject::connect(m_Huayan_Remote_121, &c_Huayan_Remote_121::Error_Occurred, this,
        [=](int errorCode, QString errorMsg){
        Write_Worry_List(QString("左臂错误 [%1]: %2").arg(errorCode).arg(errorMsg));
        ui.Huayan_121_ErrorValue->setText(QString::number(errorCode));
    });
    
    // ===== 5. 连接 RunFunc 专用信号 =====
    QObject::connect(m_Huayan_Remote_121, &c_Huayan_Remote_121::RunFunc_Response, this,
        [=](QString funcName, bool success, QString errorMsg){
        if (success) {
            Write_Worry_List(QString("左臂：函数 [%1] 执行成功").arg(funcName));
        } else {
            Write_Worry_List(QString("左臂：函数 [%1] 执行失败：%2").arg(funcName).arg(errorMsg));
        }
    });

	c_Collector_Manage::Huayan_121_DB();
	c_Collector_Manage::Huayan_121_Button();
	c_Collector_Manage::Huayan_121_Scan();

	m_Huayan_Monitor_121_Thread->start();
	m_Huayan_Remote_121_Thread->start();
}
void c_Collector_Manage::Huayan_121_DB()
{
    // ===== 1. UI 输入框 <-> JSON 配置 双向绑定 =====
    // IP 地址
    QObject::connect(ui.Huayan_121_IP, &QLineEdit::textChanged, this, [=](QString ip){
        Write_Communicate_DB("Huayan_121_IP", ip);
    });
    // 控制端口 (10003)
    QObject::connect(ui.Huayan_121_RemotePort, &QLineEdit::textChanged, this, [=](QString port){
        Write_Communicate_DB("Huayan_121_RemotePort", port.toInt());
    });
    // 监控端口 (10005)
    QObject::connect(ui.Huayan_121_MonitorPort, &QLineEdit::textChanged, this, [=](QString port){
        Write_Communicate_DB("Huayan_121_MonitorPort", port.toInt());
    });

    // ===== 2. 从全局配置初始化 UI 显示 =====
    ui.Huayan_121_IP->setText(c_Variable::getInstance().g_Communicate_DB.value("Huayan_121_IP").toString());
    ui.Huayan_121_RemotePort->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("Huayan_121_RemotePort").toInt()));
    ui.Huayan_121_MonitorPort->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("Huayan_121_MonitorPort").toInt()));
}
void c_Collector_Manage::Huayan_121_Button()
{
    // ===== 连接控制 =====
    QObject::connect(ui.Huayan_121_Connect, &QPushButton::clicked, m_Huayan_Monitor_121, &c_Huayan_Monitor_121::Connect);
    QObject::connect(ui.Huayan_121_Connect, &QPushButton::clicked, m_Huayan_Remote_121, &c_Huayan_Remote_121::Connect);
    QObject::connect(ui.Huayan_121_Disconnect, &QPushButton::clicked, m_Huayan_Monitor_121, &c_Huayan_Monitor_121::Disconnect);
    QObject::connect(ui.Huayan_121_Disconnect, &QPushButton::clicked, m_Huayan_Remote_121, &c_Huayan_Remote_121::Disconnect);
    
    // ===== 电源控制 =====
    QObject::connect(ui.Huayan_121_Electrify, &QPushButton::clicked, m_Huayan_Remote_121, &c_Huayan_Remote_121::Electrify);
	QObject::connect(ui.Huayan_121_AutoDn, &QPushButton::clicked, m_Huayan_Remote_121, &c_Huayan_Remote_121::AutoDn);
	QObject::connect(ui.Huayan_121_AutoEn, &QPushButton::clicked, m_Huayan_Remote_121, &c_Huayan_Remote_121::AutoEn);
    QObject::connect(ui.Huayan_121_BlackOut, &QPushButton::clicked, m_Huayan_Remote_121, &c_Huayan_Remote_121::BlackOut);
    
    // ===== 使能控制 =====
    QObject::connect(ui.Huayan_121_Enable, &QPushButton::clicked, m_Huayan_Remote_121, &c_Huayan_Remote_121::GrpEnable);
    QObject::connect(ui.Huayan_121_Disable, &QPushButton::clicked, m_Huayan_Remote_121, &c_Huayan_Remote_121::GrpDisable);
    
    // ===== 脚本控制 =====
    QObject::connect(ui.Huayan_121_StartScript, &QPushButton::clicked, m_Huayan_Remote_121, &c_Huayan_Remote_121::StartScript);
    QObject::connect(ui.Huayan_121_StopScript, &QPushButton::clicked, m_Huayan_Remote_121, &c_Huayan_Remote_121::StopScript);
    QObject::connect(ui.Huayan_121_PauseScript, &QPushButton::clicked, m_Huayan_Remote_121, &c_Huayan_Remote_121::PauseScript);
    QObject::connect(ui.Huayan_121_ContinueScript, &QPushButton::clicked, m_Huayan_Remote_121, &c_Huayan_Remote_121::ContinueScript);
    
    // ===== 运行脚本函数 (RunFunc) =====
    QObject::connect(ui.Huayan_121_RunFunc, &QPushButton::clicked, this, [=](){
        QString funcName = ui.Huayan_121_FuncName->text();
		Write_Worry_List(QString("左臂：运行脚本函数 [%1]").arg(funcName));
        if (!funcName.isEmpty()) {
            QStringList params;
             // 跨线程调用发送（确保在 TCP_Client 所在线程执行）
    		QMetaObject::invokeMethod(m_Huayan_Remote_121, "RunFunc", Qt::QueuedConnection,
                Q_ARG(QString, funcName),Q_ARG(QStringList, params));
        }
    });
    
	 // ===== 运行脚本函数 (SetOverride) =====
    QObject::connect(ui.Huayan_121_SetOverride, &QPushButton::clicked, this, [=](){
        QString speed = ui.Huayan_121_Speed->text();
		Write_Worry_List(QString("左臂：运行速度 [%1]").arg(speed));
        if (!speed.isEmpty()) {
            QStringList params;
            // 跨线程调用发送（确保在 TCP_Client 所在线程执行）
    		QMetaObject::invokeMethod(m_Huayan_Remote_121, "SetOverride", Qt::QueuedConnection,
                Q_ARG(int, 0), Q_ARG(QString, speed));
        }
    });

    // ===== 安全控制 =====
    QObject::connect(ui.Huayan_121_EmergencyStop, &QPushButton::clicked, m_Huayan_Remote_121, &c_Huayan_Remote_121::EmergencyStop);
    QObject::connect(ui.Huayan_121_Reset, &QPushButton::clicked, m_Huayan_Remote_121, &c_Huayan_Remote_121::GrpReset);
    QObject::connect(ui.Huayan_121_Home, &QPushButton::clicked, m_Huayan_Remote_121, &c_Huayan_Remote_121::MoveJToHome);

	QObject::connect(ui.Huayan_121_Connected, &c_Fr_Light::Working_State, ui.Huayan_121_Connect, &QPushButton::setDisabled);
	QObject::connect(ui.Huayan_121_Remote_Connected, &c_Fr_Light::Working_State, ui.Huayan_121_Disconnect, &QPushButton::setEnabled);
	QObject::connect(ui.Huayan_121_Connected, &c_Fr_Light::Default_State, ui.Huayan_121_Connect, &QPushButton::setEnabled);
	QObject::connect(ui.Huayan_121_Remote_Connected, &c_Fr_Light::Default_State, ui.Huayan_121_Disconnect, &QPushButton::setDisabled);
}
void c_Collector_Manage::Huayan_121_Scan()
{
    // 10ms 周期调用，只更新轻量级状态，避免频繁刷新导致界面卡顿
	ui.Huayan_121_Connected->Set_State(c_Variable::getInstance().g_Huayan_121.device_Status);
	ui.Huayan_121_Remote_Connected->Set_State(c_Variable::getInstance().g_Huayan_121.Remote_connected);
    ui.Huayan_121_Monitor_Connected->Set_State(c_Variable::getInstance().g_Huayan_121.Monitor_connected);

    // 更新状态机显示
    ui.Huayan_121_StateValue->setText(m_Object.Huayan_Status(c_Variable::getInstance().g_Datasheet_121.robotState));
    // 更新使能状态
    ui.Huayan_121_EnabledValue->setText(c_Variable::getInstance().g_Datasheet_121.robotEnabled ? "已使能" : "未使能");
    // 更新运动状态
    ui.Huayan_121_MovingValue->setText(c_Variable::getInstance().g_Datasheet_121.robotMoving ? "运动中" : "静止");
   	ui.Huayan_121_Moving->Set_State(c_Variable::getInstance().g_Datasheet_121.robotMoving);
	// 更新错误码显示 (有错误时标红)
    if (c_Variable::getInstance().g_Datasheet_121.Error_Code != 0) {
        ui.Huayan_121_ErrorValue->setText(QString::number(c_Variable::getInstance().g_Datasheet_121.Error_Code));
        ui.Huayan_121_ErrorValue->setStyleSheet("color: red; font-weight: bold;");
    } else {
        ui.Huayan_121_ErrorValue->setText("0");
        ui.Huayan_121_ErrorValue->setStyleSheet("");
    }
    // 更新关节位置 (°) - 保留 2 位小数
    ui.Huayan_121_J1->setText(QString::number(c_Variable::getInstance().g_Datasheet_121.Actual_Position[0], 'f', 2));
    ui.Huayan_121_J2->setText(QString::number(c_Variable::getInstance().g_Datasheet_121.Actual_Position[1], 'f', 2));
    ui.Huayan_121_J3->setText(QString::number(c_Variable::getInstance().g_Datasheet_121.Actual_Position[2], 'f', 2));
    ui.Huayan_121_J4->setText(QString::number(c_Variable::getInstance().g_Datasheet_121.Actual_Position[3], 'f', 2));
    ui.Huayan_121_J5->setText(QString::number(c_Variable::getInstance().g_Datasheet_121.Actual_Position[4], 'f', 2));
    ui.Huayan_121_J6->setText(QString::number(c_Variable::getInstance().g_Datasheet_121.Actual_Position[5], 'f', 2));     
    // 更新 TCP 位置 (mm/°)
    ui.Huayan_121_X->setText(QString::number(c_Variable::getInstance().g_Datasheet_121.Actual_PCS_TCP[0], 'f', 2));
    ui.Huayan_121_Y->setText(QString::number(c_Variable::getInstance().g_Datasheet_121.Actual_PCS_TCP[1], 'f', 2));
    ui.Huayan_121_Z->setText(QString::number(c_Variable::getInstance().g_Datasheet_121.Actual_PCS_TCP[2], 'f', 2));
    ui.Huayan_121_Rx->setText(QString::number(c_Variable::getInstance().g_Datasheet_121.Actual_PCS_TCP[3], 'f', 2));
    ui.Huayan_121_Ry->setText(QString::number(c_Variable::getInstance().g_Datasheet_121.Actual_PCS_TCP[4], 'f', 2));
    ui.Huayan_121_Rz->setText(QString::number(c_Variable::getInstance().g_Datasheet_121.Actual_PCS_TCP[5], 'f', 2));    
}
void c_Collector_Manage::Huayan_121_Delete()
{
 if (m_Huayan_Monitor_121_Thread->isRunning()) {
		//线程中断
		m_Huayan_Monitor_121_Thread->requestInterruption();
		//线程退出
		m_Huayan_Monitor_121_Thread->quit();
		//线程等待
		m_Huayan_Monitor_121_Thread->wait();
	}
	if (m_Huayan_Remote_121_Thread->isRunning()) {
		//线程中断
		m_Huayan_Remote_121_Thread->requestInterruption();
		//线程退出
		m_Huayan_Remote_121_Thread->quit();
		//线程等待
		m_Huayan_Remote_121_Thread->wait();
	}
}

void c_Collector_Manage::Zivid_1_Init()
{
	m_Zivid_1_Thread = new QThread();
	m_Zivid_1_Remote = new c_Zivid_Remote("1号Zivid相机");
	m_Zivid_1_Remote->moveToThread(m_Zivid_1_Thread);

    m_Scan_Server_120_Thread = new QThread();
	m_Scan_Server_120 = new c_Scan_Server_120();
	m_Scan_Server_120->moveToThread(m_Scan_Server_120_Thread);

	QObject::connect(m_Zivid_1_Thread, &QThread::started, m_Zivid_1_Remote, &c_Zivid_Remote::Init);
	QObject::connect(m_Zivid_1_Thread, &QThread::finished, m_Zivid_1_Remote, &c_Zivid_Remote::deleteLater);

	QObject::connect(m_Scan_Server_120_Thread, &QThread::started, m_Scan_Server_120, &c_Scan_Server_120::Init);
	QObject::connect(m_Scan_Server_120_Thread, &QThread::finished, m_Scan_Server_120, &c_Scan_Server_120::deleteLater);
	
	QObject::connect(m_Zivid_1_Remote, &c_Zivid_Remote::Status, this, &c_Collector_Manage::Write_Prec_Scan_120_Cmd);
	
	QObject::connect(m_Zivid_1_Remote, &c_Zivid_Remote::Show, this, [=](WId windowId) {
		QWindow* Window = QWindow::fromWinId(windowId);
    	// 1. 空指针检查（先检查后日志）
    	if (!Window) {
        	Write_Worry_List("⚠️ 1号相机窗口指针为空");
        	return;
    	}
    	Write_Worry_List("嵌入1号相机界面");

    	// 2. 设置窗口标志
    	Window->setFlags(Qt::Widget);
    
    	// 3. 清理旧容器（遍历布局项删除）
    	QLayout* layout = ui.Zivid_1_Widget->layout();
    	if (layout) {
        	QLayoutItem* item;
        	while ((item = layout->takeAt(0)) != nullptr) {
            	if (item->widget()) {
                	item->widget()->deleteLater();  // 安全删除，避免跨线程问题
            	}
            	if (item->layout()) {
                	// 递归清理子布局（如果有）
                	QLayout* subLayout = item->layout();
                	QLayoutItem* subItem;
                	while ((subItem = subLayout->takeAt(0)) != nullptr) {
                    	if (subItem->widget()) subItem->widget()->deleteLater();
                    	delete subItem;
                	}
                	delete subLayout;
            	}
            	delete item;
        	}
    	}
    
    	// 4. 使用 QueuedConnection 确保在主线程事件循环中执行
    	QMetaObject::invokeMethod(this, [this, Window]() {
        	// ⚠️ 二次检查：确保窗口仍然有效
        	if (!Window) {
            	Write_Worry_List("1号相机窗口已失效");
            	return;
        	}
        	// 5. 创建容器（必须在主线程）
        	QWidget* container = QWidget::createWindowContainer(Window, ui.Zivid_1_Widget);
        	if (!container) {
            	Write_Worry_List("1号相机嵌入失败：容器创建失败");
            	return;
        	}
        	// 6. 设置尺寸策略（关键：让容器自适应父控件）
        	container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        	container->setMinimumSize(0, 0);
        	container->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        	// 7. 确保布局存在并配置
        	if (ui.Zivid_1_Widget->layout() == nullptr) {
            	QGridLayout* newLayout = new QGridLayout(ui.Zivid_1_Widget);
            	newLayout->setContentsMargins(0, 0, 0, 0);
            	newLayout->setSpacing(0);
            	ui.Zivid_1_Widget->setLayout(newLayout);
        	}
        	// 8. 添加容器到布局
        	ui.Zivid_1_Widget->layout()->addWidget(container);
        	container->show();
        
        	// 9. 强制刷新布局（解决首次显示尺寸异常）
        	ui.Zivid_1_Widget->layout()->activate();
        	ui.Zivid_1_Widget->update();
        	ui.Zivid_1_Widget->repaint();
        
        	// 10. 延迟刷新：确保子窗口完成初始化后再计算尺寸
        	QTimer::singleShot(100, ui.Zivid_1_Widget, [=]() {
            	if (container && container->isVisible()) {
                	container->updateGeometry();
                	ui.Zivid_1_Widget->layout()->activate();
            	}
        	});
        
        	Write_Worry_List("1号相机嵌入成功");
        
    	});  // ← 关键：确保队列连接
	});
	QObject::connect(m_Zivid_1_Remote, &c_Zivid_Remote::Connect_Done, this, [=]() {
		Write_Worry_List("1号Zivid相机连接标志置位");
		});
	QObject::connect(m_Zivid_1_Remote, &c_Zivid_Remote::Disconnect_Done, this, [=]() {
		Write_Worry_List("1号Zivid相机达连接标志复位");
		});
	QObject::connect(m_Zivid_1_Remote, &c_Zivid_Remote::is_Run, this, [=]() {
		Write_Worry_List("1号Zivid相机启动标志置位");
		ui.Zivid_1_Start->setDisabled(true);
		ui.Zivid_1_Close->setEnabled(true);
		ui.Zivid_1_Status->Set_Working();

		});
	QObject::connect(m_Zivid_1_Remote, &c_Zivid_Remote::is_Stop, this, [=]() {
		Write_Worry_List("1号Zivid相机启动标志复位");
		ui.Zivid_1_Start->setEnabled(true);
		ui.Zivid_1_Close->setDisabled(true);
		ui.Zivid_1_Status->Set_Default();
		});

	QObject::connect(m_Scan_Server_120, &c_Scan_Server_120::updateSaveInfo,m_Zivid_1_Remote, &c_Zivid_Remote::updateSaveInfo);
    QObject::connect(m_Scan_Server_120, &c_Scan_Server_120::Capture, m_Zivid_1_Remote,  &c_Zivid_Remote::Capture);

    QObject::connect(m_Zivid_1_Remote, &c_Zivid_Remote::CaptureCompleted, m_Scan_Server_120, &c_Scan_Server_120::CaptureCompleted);
	QObject::connect(m_Zivid_1_Remote, &c_Zivid_Remote::updateCompleted, m_Scan_Server_120, &c_Scan_Server_120::updateCompleted);

	QObject::connect(m_Scan_Server_120, &c_Scan_Server_120::Status, this, [=](QString cmd) {
		c_Collector_Manage::Write_Prec_Scan_120_Cmd("c_Scan_Server_120::Status:" + cmd);
	});

	QObject::connect(m_Scan_Server_120, &c_Scan_Server_120::Read_Done, this, [=](QString cmd) {
		c_Collector_Manage::Write_Prec_Scan_120_Cmd("c_Scan_Server_120::Read_Done:" + cmd);
	});
	QObject::connect(m_Scan_Server_120, &c_Scan_Server_120::Write_String_Done, this, [=](QString cmd) {
		c_Collector_Manage::Write_Prec_Scan_120_Cmd("c_Scan_Server_120::Write_String_Done:" + cmd);
	});

	QObject::connect(m_Scan_Server_120, &c_Scan_Server_120::updateSaveInfo, this, [=](QString cmd) {
		c_Collector_Manage::Write_Prec_Scan_120_Cmd("c_Scan_Server_120::updateSaveInfo:" + cmd);
	});
	QObject::connect(m_Scan_Server_120, &c_Scan_Server_120::Capture, this, [=](QString cmd) {
		c_Collector_Manage::Write_Prec_Scan_120_Cmd("c_Scan_Server_120::Capture:" + cmd);
	});

	QObject::connect(m_Scan_Server_120, &c_Scan_Server_120::Write_Prec_Scan_Cmd, this, &c_Collector_Manage::Write_Prec_Scan_120_Cmd);

	c_Collector_Manage::Zivid_1_DB();
	c_Collector_Manage::Zivid_1_Button();

	m_Zivid_1_Thread->start();
	m_Scan_Server_120_Thread->start();
}
void c_Collector_Manage::Zivid_1_DB()
{
	ui.Prec_Scan_120_Local_Ip->setText(c_Variable::getInstance().g_Communicate_DB.value("Prec_Scan_120_Local_Ip").toString());
	ui.Prec_Scan_120_Tran_Port->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("Prec_Scan_120_Tran_Port").toInt()));
	
	QObject::connect(ui.Prec_Scan_120_Local_Ip, &QLineEdit::textChanged, this, [=](QString ip) {
		Write_Communicate_DB("Prec_Scan_120_Local_Ip", ip); 
	});
	QObject::connect(ui.Prec_Scan_120_Tran_Port, &QLineEdit::textChanged, this, [=](QString port) {
		Write_Communicate_DB("Prec_Scan_120_Tran_Port", port.toInt());
	});

	m_Begin_Time = c_Variable::getInstance().g_Work.Begin_Time;
	m_Car_Type = c_Variable::getInstance().g_Work.Car_Type;
	m_Car_Num = c_Variable::getInstance().g_Work.Car_Num;

	m_Car_Box = c_Variable::getInstance().g_Work.Carbox_Num;
	m_Bogie_Num = c_Variable::getInstance().g_Work.Bogie_Num;
	m_Axis_Num = c_Variable::getInstance().g_Work.Axis_Num;

	ui.Begin_Time_Setting->setText(c_Variable::getInstance().g_Work.Begin_Time);
	ui.Car_Type_Setting->setText(c_Variable::getInstance().g_Work.Car_Type);
	ui.Car_Num_Setting->setText(c_Variable::getInstance().g_Work.Car_Num);

	ui.Car_Box_Setting->setText(c_Variable::getInstance().g_Work.Carbox_Num);
	ui.Bogie_Num_Setting->setText(c_Variable::getInstance().g_Work.Bogie_Num);
	ui.Axis_Num_Setting->setText(c_Variable::getInstance().g_Work.Axis_Num);

	QObject::connect(ui.Begin_Time_Setting, &QLineEdit::textChanged, this, [=](QString value) {m_Begin_Time = value; });
	QObject::connect(ui.Car_Type_Setting, &QLineEdit::textChanged, this, [=](QString value) {m_Car_Type = value; });
	QObject::connect(ui.Car_Num_Setting, &QLineEdit::textChanged, this, [=](QString value) {m_Car_Num = value; });

	QObject::connect(ui.Car_Num_Setting, &QLineEdit::textChanged, this, [=](QString value) {m_Car_Num = value; });
	QObject::connect(ui.Car_Box_Setting, &QLineEdit::textChanged, this, [=](QString value) {m_Car_Box = value; });
	QObject::connect(ui.Bogie_Num_Setting, &QLineEdit::textChanged, this, [=](QString value) {m_Bogie_Num = value; });
	QObject::connect(ui.Axis_Num_Setting, &QLineEdit::textChanged, this, [=](QString value) {m_Axis_Num = value; });

	m_Wheelset_Num_120 = c_Variable::getInstance().g_Work.Wheelset_Num_120;
	m_Primary_Components_120 = c_Variable::getInstance().g_Work.Primary_Components_120;
	m_Secondary_Components_120 = c_Variable::getInstance().g_Work.Secondary_Components_120;
	m_Point_Num_120 = c_Variable::getInstance().g_Work.Point_Num_120;

	ui.Wheelset_Num_120_Setting->setText(c_Variable::getInstance().g_Work.Wheelset_Num_120);
	ui.Primary_Components_120_Setting->setText(c_Variable::getInstance().g_Work.Primary_Components_120);
	ui.Secondary_Components_120_Setting->setText(c_Variable::getInstance().g_Work.Secondary_Components_120);
	ui.Point_Num_120_Setting->setText(c_Variable::getInstance().g_Work.Point_Num_120);


	QObject::connect(ui.Wheelset_Num_120_Setting, &QLineEdit::textChanged, this, [=](QString data) {m_Wheelset_Num_120 = data;});
	QObject::connect(ui.Primary_Components_120_Setting, &QLineEdit::textChanged, this, [=](QString data) {m_Primary_Components_120 = data;});
	QObject::connect(ui.Secondary_Components_120_Setting, &QLineEdit::textChanged, this, [=](QString data) {m_Secondary_Components_120 = data;});
	QObject::connect(ui.Point_Num_120_Setting, &QLineEdit::textChanged, this, [=](QString data) {m_Point_Num_120 = data;});
}
void c_Collector_Manage::Zivid_1_Button()
{
	QObject::connect(ui.Zivid_1_Start, &QPushButton::clicked, m_Zivid_1_Remote, &c_Zivid_Remote::Connect_Device);
	QObject::connect(ui.Zivid_1_Close, &QPushButton::clicked, m_Zivid_1_Remote, &c_Zivid_Remote::Close_Device);

	QObject::connect(ui.Connect_Server_120, &QPushButton::clicked, m_Scan_Server_120, &c_Scan_Server_120::Connect_Device);//精扫连接
	QObject::connect(ui.Disconnect_Server_120, &QPushButton::clicked, m_Scan_Server_120, &c_Scan_Server_120::Disconnect_Device);//精扫断开

	QObject::connect(ui.Start_120, &QPushButton::clicked, m_Scan_Server_120, &c_Scan_Server_120::Start); //精扫采集
	QObject::connect(ui.Collection_120, &QPushButton::clicked, m_Scan_Server_120, &c_Scan_Server_120::Collection); //精扫采集
	
	QObject::connect(m_Scan_Server_120, &c_Scan_Server_120::Listen_Done, ui.Start_120, &QPushButton::setEnabled);//精扫
	QObject::connect(m_Scan_Server_120, &c_Scan_Server_120::Listen_Done, ui.Collection_120, &QPushButton::setEnabled);//精扫

	QObject::connect(m_Scan_Server_120, &c_Scan_Server_120::Dislisten_Done, ui.Start_120, &QPushButton::setDisabled);//精扫
	QObject::connect(m_Scan_Server_120, &c_Scan_Server_120::Dislisten_Done, ui.Collection_120, &QPushButton::setDisabled);//精扫

	QObject::connect(m_Scan_Server_120, &c_Scan_Server_120::Dislisten_Done, ui.Connect_Server_120, &QPushButton::setEnabled);//连接
	QObject::connect(m_Scan_Server_120, &c_Scan_Server_120::Dislisten_Done, ui.Disconnect_Server_120, &QPushButton::setDisabled);//连接

	QObject::connect(m_Scan_Server_120, &c_Scan_Server_120::Listen_Done, ui.Connect_Server_120, &QPushButton::setDisabled);//连接
	QObject::connect(m_Scan_Server_120, &c_Scan_Server_120::Listen_Done, ui.Disconnect_Server_120, &QPushButton::setEnabled);//连接

	QObject::connect(ui.Write_Begin_Time, &QPushButton::clicked, this, [=]() {
		c_Variable::getInstance().g_Work.Begin_Time = m_Begin_Time; 
		c_Variable::getInstance().g_Work.Work_Num = QString("%1%2%3").arg(m_Begin_Time).arg(m_Car_Type).arg(m_Car_Num);
	});
	QObject::connect(ui.Write_Car_Type, &QPushButton::clicked, this, [=]() {
		c_Variable::getInstance().g_Work.Car_Type = m_Car_Type;
		c_Variable::getInstance().g_Work.Work_Num = QString("%1%2%3").arg(m_Begin_Time).arg(m_Car_Type).arg(m_Car_Num);
	});
	QObject::connect(ui.Write_Car_Num, &QPushButton::clicked, this, [=]() {
		c_Variable::getInstance().g_Work.Car_Num = m_Car_Num; 
		c_Variable::getInstance().g_Work.Work_Num = QString("%1%2%3").arg(m_Begin_Time).arg(m_Car_Type).arg(m_Car_Num);
	});

	QObject::connect(ui.Write_Car_Box, &QPushButton::clicked, this, [=]() {
		c_Variable::getInstance().g_Work.Carbox_Num = m_Car_Box;
	});
	QObject::connect(ui.Write_Bogie_Num, &QPushButton::clicked, this, [=]() {
		c_Variable::getInstance().g_Work.Bogie_Num = m_Bogie_Num;
	});
	QObject::connect(ui.Write_Axis_Num, &QPushButton::clicked, this, [=]() {
		c_Variable::getInstance().g_Work.Axis_Num = m_Axis_Num;
	});
	QObject::connect(ui.Write_Wheelset_Num_120, &QPushButton::clicked, this, [=]() {
		c_Variable::getInstance().g_Work.Wheelset_Num_120 = m_Wheelset_Num_120; 
	});
	QObject::connect(ui.Write_Primary_Components_120, &QPushButton::clicked, this, [=]() {
		c_Variable::getInstance().g_Work.Primary_Components_120 = m_Primary_Components_120; 
	});
	QObject::connect(ui.Write_Secondary_Components_120, &QPushButton::clicked, this, [=]() {
		c_Variable::getInstance().g_Work.Secondary_Components_120 = m_Secondary_Components_120;
	});
	QObject::connect(ui.Write_Point_Num_120, &QPushButton::clicked, this, [=]() {
		c_Variable::getInstance().g_Work.Point_Num_120 = m_Point_Num_120;
	});
}
void c_Collector_Manage::Zivid_1_Scan()
{
	ui.Left_Scan_Tran_State->Set_State(c_Variable::getInstance().g_Prec_Scan_120.Tran_Connected);
}
void c_Collector_Manage::Zivid_1_Delete()
{
	ui.Zivid_1_Close->clicked();

	if (m_Zivid_1_Thread->isRunning()) {
		//线程中断
		m_Zivid_1_Thread->requestInterruption();
		//线程退出
		m_Zivid_1_Thread->quit();
		//线程等待
		m_Zivid_1_Thread->wait();
	}
	if (m_Scan_Server_120_Thread->isRunning()) {
		//线程中断
		m_Scan_Server_120_Thread->requestInterruption();
		//线程退出
		m_Scan_Server_120_Thread->quit();
		//线程等待
		m_Scan_Server_120_Thread->wait();
	}
}

void c_Collector_Manage::Zivid_2_Init()
{
	m_Zivid_2_Thread = new QThread();
	m_Zivid_2_Remote = new c_Zivid_Remote("2号Zivid相机");
	m_Zivid_2_Remote->moveToThread(m_Zivid_2_Thread);

	m_Scan_Server_121_Thread = new QThread();
 	m_Scan_Server_121 = new c_Scan_Server_121();
	m_Scan_Server_121->moveToThread(m_Scan_Server_121_Thread);

	QObject::connect(m_Zivid_2_Thread, &QThread::started, m_Zivid_2_Remote, &c_Zivid_Remote::Init);
	QObject::connect(m_Zivid_2_Thread, &QThread::finished, m_Zivid_2_Remote, &c_Zivid_Remote::deleteLater);

	QObject::connect(m_Scan_Server_121_Thread, &QThread::started, m_Scan_Server_121, &c_Scan_Server_121::Init);
	QObject::connect(m_Scan_Server_121_Thread, &QThread::finished, m_Scan_Server_121, &c_Scan_Server_121::deleteLater);

	QObject::connect(m_Zivid_2_Remote, &c_Zivid_Remote::Status, this, &c_Collector_Manage::Write_Prec_Scan_121_Cmd);
	QObject::connect(m_Zivid_2_Remote, &c_Zivid_Remote::Show, this, [=](WId windowId) {
		QWindow* Window = QWindow::fromWinId(windowId);
    	// 1. 空指针检查（先检查后日志）
    	if (!Window) {
        	Write_Worry_List("⚠️ 2号相机窗口指针为空");
        	return;
    	}
    	Write_Worry_List("嵌入2号相机界面");
    	// 2. 设置窗口标志
    	Window->setFlags(Qt::Widget);
    	// 3. 清理旧容器（遍历布局项删除）
    	QLayout* layout = ui.Zivid_2_Widget->layout();
    	if (layout) {
        	QLayoutItem* item;
        	while ((item = layout->takeAt(0)) != nullptr) {
            	if (item->widget()) {
                	item->widget()->deleteLater();  // 安全删除，避免跨线程问题
            	}
            	if (item->layout()) {
                	// 递归清理子布局（如果有）
                	QLayout* subLayout = item->layout();
                	QLayoutItem* subItem;
                	while ((subItem = subLayout->takeAt(0)) != nullptr) {
                    	if (subItem->widget()) subItem->widget()->deleteLater();
                    	delete subItem;
                	}
                	delete subLayout;
            	}
            	delete item;
        	}
    	}
    	// 4. 使用 QueuedConnection 确保在主线程事件循环中执行
    	QMetaObject::invokeMethod(this, [this, Window]() {
        	if (!Window) {
            	Write_Worry_List("2号相机窗口已失效");
            	return;
        	}
        	// 5. 创建容器（必须在主线程）
        	QWidget* container = QWidget::createWindowContainer(Window, ui.Zivid_2_Widget);
        	if (!container) {
            	Write_Worry_List("2号相机嵌入失败：容器创建失败");
            	return;
        	}
        	// 6. 设置尺寸策略（关键：让容器自适应父控件）
        	container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        	container->setMinimumSize(0, 0);
        	container->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        	// 7. 确保布局存在并配置
        	if (ui.Zivid_2_Widget->layout() == nullptr) {
            	QGridLayout* newLayout = new QGridLayout(ui.Zivid_2_Widget);
            	newLayout->setContentsMargins(0, 0, 0, 0);
            	newLayout->setSpacing(0);
            	ui.Zivid_2_Widget->setLayout(newLayout);
        	}
        	// 8. 添加容器到布局
        	ui.Zivid_2_Widget->layout()->addWidget(container);
        	container->show();
        	// 9. 强制刷新布局（解决首次显示尺寸异常）
        	ui.Zivid_2_Widget->layout()->activate();
        	ui.Zivid_2_Widget->update();
        	ui.Zivid_2_Widget->repaint();
        	// 10. 延迟刷新：确保子窗口完成初始化后再计算尺寸
        	QTimer::singleShot(100, ui.Zivid_2_Widget, [=]() {
            	if (container && container->isVisible()) {
                	container->updateGeometry();
                	ui.Zivid_2_Widget->layout()->activate();
            	}
        	});
        
        	Write_Worry_List("2号相机嵌入成功");
        
    	});  // ← 关键：确保队列连接
	});
	QObject::connect(m_Zivid_2_Remote, &c_Zivid_Remote::Connect_Done, this, [=]() {
		Write_Worry_List("2号Zivid相机连接标志置位");
		});
	QObject::connect(m_Zivid_2_Remote, &c_Zivid_Remote::Disconnect_Done, this, [=]() {
		Write_Worry_List("2号Zivid相机达连接标志复位");
		});
	QObject::connect(m_Zivid_2_Remote, &c_Zivid_Remote::is_Run, this, [=]() {
		Write_Worry_List("2号Zivid相机启动标志置位");
		ui.Zivid_2_Start->setDisabled(true);
		ui.Zivid_2_Close->setEnabled(true);
		ui.Zivid_2_Status->Set_Working();

		});
	QObject::connect(m_Zivid_2_Remote, &c_Zivid_Remote::is_Stop, this, [=]() {
		Write_Worry_List("2号Zivid相机启动标志复位");
		ui.Zivid_2_Start->setEnabled(true);
		ui.Zivid_2_Close->setDisabled(true);
		ui.Zivid_2_Status->Set_Default();
		});

 	QObject::connect(m_Scan_Server_121, &c_Scan_Server_121::updateSaveInfo,m_Zivid_2_Remote, &c_Zivid_Remote::updateSaveInfo);
    QObject::connect(m_Scan_Server_121, &c_Scan_Server_121::Capture, m_Zivid_2_Remote,  &c_Zivid_Remote::Capture);

    QObject::connect(m_Zivid_2_Remote, &c_Zivid_Remote::CaptureCompleted, m_Scan_Server_121, &c_Scan_Server_121::CaptureCompleted);
	QObject::connect(m_Zivid_2_Remote, &c_Zivid_Remote::updateCompleted, m_Scan_Server_121, &c_Scan_Server_121::updateCompleted);

	QObject::connect(m_Scan_Server_121, &c_Scan_Server_121::Status, this, [=](QString cmd) {
		c_Collector_Manage::Write_Prec_Scan_121_Cmd("c_Scan_Server_121::Status:" + cmd);
	});

	QObject::connect(m_Scan_Server_121, &c_Scan_Server_121::Read_Done, this, [=](QString cmd) {
		c_Collector_Manage::Write_Prec_Scan_121_Cmd("c_Scan_Server_121::Read_Done:" + cmd);
	});
	QObject::connect(m_Scan_Server_121, &c_Scan_Server_121::Write_String_Done, this, [=](QString cmd) {
		c_Collector_Manage::Write_Prec_Scan_121_Cmd("c_Scan_Server_121::Write_String_Done:" + cmd);
	});

	QObject::connect(m_Scan_Server_121, &c_Scan_Server_121::updateSaveInfo, this, [=](QString cmd) {
		c_Collector_Manage::Write_Prec_Scan_121_Cmd("c_Scan_Server_121::updateSaveInfo:" + cmd);
	});
	QObject::connect(m_Scan_Server_121, &c_Scan_Server_121::Capture, this, [=](QString cmd) {
		c_Collector_Manage::Write_Prec_Scan_121_Cmd("c_Scan_Server_121::Capture:" + cmd);
	});

	QObject::connect(m_Scan_Server_121, &c_Scan_Server_121::Write_Prec_Scan_Cmd, this, &c_Collector_Manage::Write_Prec_Scan_121_Cmd);

	c_Collector_Manage::Zivid_2_DB();
	c_Collector_Manage::Zivid_2_Button();

	m_Zivid_2_Thread->start();
	m_Scan_Server_121_Thread->start();
}
void c_Collector_Manage::Zivid_2_DB()
{
	ui.Prec_Scan_121_Local_Ip->setText(c_Variable::getInstance().g_Communicate_DB.value("Prec_Scan_121_Local_Ip").toString());
	ui.Prec_Scan_121_Tran_Port->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("Prec_Scan_121_Tran_Port").toInt()));
	
	QObject::connect(ui.Prec_Scan_121_Local_Ip, &QLineEdit::textChanged, this, [=](QString ip) {Write_Communicate_DB("Prec_Scan_121_Local_Ip", ip);});
	QObject::connect(ui.Prec_Scan_121_Tran_Port, &QLineEdit::textChanged, this, [=](QString port) {Write_Communicate_DB("Prec_Scan_121_Tran_Port", port.toInt()); });

	ui.Wheelset_Num_121_Setting->setText(c_Variable::getInstance().g_Work.Wheelset_Num_121);
	ui.Primary_Components_121_Setting->setText(c_Variable::getInstance().g_Work.Primary_Components_121);
	ui.Secondary_Components_121_Setting->setText(c_Variable::getInstance().g_Work.Secondary_Components_121);
	ui.Point_Num_121_Setting->setText(c_Variable::getInstance().g_Work.Point_Num_121);
	
	QObject::connect(ui.Wheelset_Num_121_Setting, &QLineEdit::textChanged, this, [=](QString value) {
		m_Wheelset_Num_121 = value;
	});
	QObject::connect(ui.Primary_Components_121_Setting, &QLineEdit::textChanged, this, [=](QString value) {
		m_Primary_Components_121 = value;
	});
	QObject::connect(ui.Secondary_Components_121_Setting, &QLineEdit::textChanged, this, [=](QString value) {
		m_Secondary_Components_121 = value; 
	});
	QObject::connect(ui.Point_Num_121_Setting, &QLineEdit::textChanged, this, [=](QString value) {
		m_Point_Num_121 = value; 
	});
}
void c_Collector_Manage::Zivid_2_Button()
{
	QObject::connect(ui.Zivid_2_Start, &QPushButton::clicked, m_Zivid_2_Remote, &c_Zivid_Remote::Connect_Device);
	QObject::connect(ui.Zivid_2_Close, &QPushButton::clicked, m_Zivid_2_Remote, &c_Zivid_Remote::Close_Device);

	QObject::connect(ui.Connect_Server_121, &QPushButton::clicked, m_Scan_Server_121, &c_Scan_Server_121::Connect_Device);//精扫连接
	QObject::connect(ui.Disconnect_Server_121, &QPushButton::clicked, m_Scan_Server_121, &c_Scan_Server_121::Disconnect_Device);//精扫断开

	QObject::connect(ui.Start_121, &QPushButton::clicked, m_Scan_Server_121, &c_Scan_Server_121::Start); //精扫采集
	QObject::connect(ui.Collection_121, &QPushButton::clicked, m_Scan_Server_121, &c_Scan_Server_121::Collection); //精扫采集
	
	QObject::connect(m_Scan_Server_121, &c_Scan_Server_121::Listen_Done, ui.Start_121, &QPushButton::setEnabled);//精扫
	QObject::connect(m_Scan_Server_121, &c_Scan_Server_121::Listen_Done, ui.Collection_121, &QPushButton::setEnabled);//精扫

	QObject::connect(m_Scan_Server_121, &c_Scan_Server_121::Dislisten_Done, ui.Start_121, &QPushButton::setDisabled);//精扫
	QObject::connect(m_Scan_Server_121, &c_Scan_Server_121::Dislisten_Done, ui.Collection_121, &QPushButton::setDisabled);//精扫

	QObject::connect(m_Scan_Server_121, &c_Scan_Server_121::Dislisten_Done, ui.Connect_Server_121, &QPushButton::setEnabled);//连接
	QObject::connect(m_Scan_Server_121, &c_Scan_Server_121::Dislisten_Done, ui.Disconnect_Server_121, &QPushButton::setDisabled);//连接

	QObject::connect(m_Scan_Server_121, &c_Scan_Server_121::Listen_Done, ui.Connect_Server_121, &QPushButton::setDisabled);//连接
	QObject::connect(m_Scan_Server_121, &c_Scan_Server_121::Listen_Done, ui.Disconnect_Server_121, &QPushButton::setEnabled);//连接

	QObject::connect(ui.Write_Wheelset_Num_121, &QPushButton::clicked, this, [=]() {
		c_Variable::getInstance().g_Work.Wheelset_Num_121 = m_Wheelset_Num_121; 
	});
	QObject::connect(ui.Write_Primary_Components_121, &QPushButton::clicked, this, [=]() {
		c_Variable::getInstance().g_Work.Primary_Components_121 = m_Primary_Components_121; 
	});
	QObject::connect(ui.Write_Secondary_Components_121, &QPushButton::clicked, this, [=]() {
		c_Variable::getInstance().g_Work.Secondary_Components_121 = m_Secondary_Components_121;
	});
	QObject::connect(ui.Write_Point_Num_121, &QPushButton::clicked, this, [=]() {
		c_Variable::getInstance().g_Work.Point_Num_121 = m_Point_Num_121;
	});
}
void c_Collector_Manage::Zivid_2_Scan()
{
	ui.Right_Scan_Tran_State->Set_State(c_Variable::getInstance().g_Prec_Scan_121.Tran_Connected);
}
void c_Collector_Manage::Zivid_2_Delete()
{
	ui.Zivid_2_Close->clicked();
	if (m_Zivid_2_Thread->isRunning()) {
		m_Zivid_2_Thread->requestInterruption();
		m_Zivid_2_Thread->quit();
		m_Zivid_2_Thread->wait();
	}
	if (m_Scan_Server_121_Thread->isRunning()) {
		//线程中断
		m_Scan_Server_121_Thread->requestInterruption();
		//线程退出
		m_Scan_Server_121_Thread->quit();
		//线程等待
		m_Scan_Server_121_Thread->wait();
	}
}

void c_Collector_Manage::E1R_1_Init()
{
	m_E1R_1_Thread = new QThread();
	m_E1R_1_Remote = new c_E1R_Remote("1号E1R雷达");
	m_E1R_1_Remote->moveToThread(m_E1R_1_Thread);

	QObject::connect(m_E1R_1_Thread, &QThread::started, m_E1R_1_Remote, &c_E1R_Remote::Init);
	QObject::connect(m_E1R_1_Thread, &QThread::finished, m_E1R_1_Remote, &c_E1R_Remote::deleteLater);

	QObject::connect(ui.E1R_1_Start, &QPushButton::clicked, m_E1R_1_Remote, &c_E1R_Remote::Connect_Device);
	QObject::connect(ui.E1R_1_Close, &QPushButton::clicked, m_E1R_1_Remote, &c_E1R_Remote::Close_Device);

	QObject::connect(m_E1R_1_Remote, &c_E1R_Remote::Status, this, &c_Collector_Manage::Write_Worry_List);
	QObject::connect(m_E1R_1_Remote, &c_E1R_Remote::Show, this, [=](WId windowId) {
    
		QWindow* Window = QWindow::fromWinId(windowId);

    	if (!Window) {
        	Write_Worry_List("⚠️ 窗口指针为空");
        	return;
    	}
    
    	Write_Worry_List("嵌入1号雷达界面");
    
   	 	// 1. 设置窗口标志（在主线程执行）
    	Window->setFlags(Qt::Widget);
    
    	// 2. 清理旧容器（遍历布局项）
    	QLayout* layout = ui.E1R_1_Widget->layout();
    	if (layout) {
        	QLayoutItem* item;
       	 	while ((item = layout->takeAt(0)) != nullptr) {
            	if (item->widget()) {
                	item->widget()->deleteLater();  // 安全删除，避免跨线程问题
            	}
            	if (item->layout()) {
                	// 递归清理子布局（如果有）
                	QLayout* subLayout = item->layout();
                	QLayoutItem* subItem;
                	while ((subItem = subLayout->takeAt(0)) != nullptr) {
                    	if (subItem->widget()) subItem->widget()->deleteLater();
                    	delete subItem;
                	}
                	delete subLayout;
            	}
            	delete item;
        	}
    	}
    
    	// 3. 使用 QueuedConnection 确保在主线程事件循环中执行
    	QMetaObject::invokeMethod(this, [this, Window]() {
        	// ⚠️ 二次检查：确保窗口仍然有效
        	if (!Window) {
            	Write_Worry_List("窗口已失效");
            	return;
        	}
        
        	// 4. 创建容器（必须在主线程）
        	QWidget* container = QWidget::createWindowContainer(Window, ui.E1R_1_Widget);
        	if (!container) {
            	Write_Worry_List("1号雷达嵌入失败：容器创建失败");
            	return;
        	}
        
        	// 5. 设置尺寸策略（关键：让容器自适应父控件）
        	container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        	container->setMinimumSize(0, 0);
        	container->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        
        	// 6. 确保布局存在并配置
        	if (ui.E1R_1_Widget->layout() == nullptr) {
            	QGridLayout* newLayout = new QGridLayout(ui.E1R_1_Widget);
            	newLayout->setContentsMargins(0, 0, 0, 0);
            	newLayout->setSpacing(0);
            	ui.E1R_1_Widget->setLayout(newLayout);
        	}
        
        	// 7. 添加容器到布局
        	ui.E1R_1_Widget->layout()->addWidget(container);
        	container->show();
        
        	// 8. 强制刷新布局（解决首次显示尺寸异常）
        	ui.E1R_1_Widget->layout()->activate();
        	ui.E1R_1_Widget->update();
        	ui.E1R_1_Widget->repaint();
        
        	// 9. 延迟刷新：确保子窗口完成初始化后再计算尺寸
        	QTimer::singleShot(50, ui.E1R_1_Widget, [=]() {
            	if (container && container->isVisible()) {
                	container->updateGeometry();
                	ui.E1R_1_Widget->layout()->activate();
            	}
        	});
        
        	Write_Worry_List("1号雷达嵌入成功");
        
    	}, Qt::QueuedConnection);
	});
	QObject::connect(m_E1R_1_Remote, &c_E1R_Remote::Connect_Done, this, [=]() {
		Write_Worry_List("1号E1R雷达连接标志置位");
	});
	QObject::connect(m_E1R_1_Remote, &c_E1R_Remote::Disconnect_Done, this, [=]() {
		Write_Worry_List("1号E1R雷达达连接标志复位");
	});
	QObject::connect(m_E1R_1_Remote, &c_E1R_Remote::is_Run, this, [=]() {
		Write_Worry_List("1号E1R雷达启动标志置位");
		ui.E1R_1_Start->setDisabled(true);
		ui.E1R_1_Close->setEnabled(true);
		ui.E1R_1_Status->Set_Working();

	});
	QObject::connect(m_E1R_1_Remote, &c_E1R_Remote::is_Stop, this, [=]() {
		Write_Worry_List("1号E1R雷达启动标志复位");
		ui.E1R_1_Start->setEnabled(true);
		ui.E1R_1_Close->setDisabled(true);
		ui.E1R_1_Status->Set_Default();
		});

	m_E1R_1_Thread->start();
}
void c_Collector_Manage::E1R_1_Delete()
{
	if (m_E1R_1_Thread->isRunning()) {
		//线程中断
		m_E1R_1_Thread->requestInterruption();
		//线程退出
		m_E1R_1_Thread->quit();
		//线程等待
		m_E1R_1_Thread->wait();
	}
}

void c_Collector_Manage::E1R_2_Init()
{
	m_E1R_2_Thread = new QThread();
	m_E1R_2_Remote = new c_E1R_Remote("2号E1R雷达");
	m_E1R_2_Remote->moveToThread(m_E1R_2_Thread);

	QObject::connect(m_E1R_2_Thread, &QThread::started, m_E1R_2_Remote, &c_E1R_Remote::Init);
	QObject::connect(m_E1R_2_Thread, &QThread::finished, m_E1R_2_Remote, &c_E1R_Remote::deleteLater);

	QObject::connect(ui.E1R_2_Start, &QPushButton::clicked, m_E1R_2_Remote, &c_E1R_Remote::Connect_Device);
	QObject::connect(ui.E1R_2_Close, &QPushButton::clicked, m_E1R_2_Remote, &c_E1R_Remote::Close_Device);

	QObject::connect(m_E1R_2_Remote, &c_E1R_Remote::Status, this, &c_Collector_Manage::Write_Worry_List);
	// ===== E1R_2_Init() 中的 Show 信号连接 =====
	QObject::connect(m_E1R_2_Remote, &c_E1R_Remote::Show, this, [=](WId windowId) {
    
		QWindow* Window = QWindow::fromWinId(windowId);
		
    	Write_Worry_List("嵌入2号雷达界面");
    
    	if (!Window) {
        	Write_Worry_List("⚠️ 窗口指针为空");
        	return;
    	}
    
    	Window->setFlags(Qt::Widget);
    
    	// ===== 1. 清理旧容器（遍历布局项删除）=====
    	QLayout* layout = ui.E1R_2_Widget->layout();
    	if (layout) {
        	QLayoutItem* item;
        	while ((item = layout->takeAt(0)) != nullptr) {
            	if (item->widget()) {
                	item->widget()->deleteLater();  // 安全删除，避免跨线程问题
            	}
            	if (item->layout()) {
                	// 递归清理子布局（如果有）
                	QLayout* subLayout = item->layout();
                	QLayoutItem* subItem;
                	while ((subItem = subLayout->takeAt(0)) != nullptr) {
                    	if (subItem->widget()) subItem->widget()->deleteLater();
                    	delete subItem;
                	}
                	delete subLayout;
            	}
            	delete item;
        	}
    	}
    
    	// ===== 2. 使用 QueuedConnection 确保在主线程事件循环中执行 =====
    	QMetaObject::invokeMethod(this, [this, Window]() {
        
        	// 二次检查：确保窗口仍然有效
        	if (!Window) {
            	Write_Worry_List("窗口已失效");
            	return;
        	}
        
        	// ===== 3. 创建容器（必须在主线程）=====
        	QWidget* container = QWidget::createWindowContainer(Window, ui.E1R_2_Widget);
        	if (!container) {
            	Write_Worry_List("2号雷达嵌入失败：容器创建失败");
            	return;
        	}
        
        	// ===== 4. 设置尺寸策略（关键：让容器自适应父控件）=====
        	container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        	container->setMinimumSize(0, 0);
        	container->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        
        	// ===== 5. 确保布局存在并配置 =====
        	if (ui.E1R_2_Widget->layout() == nullptr) {
            	QGridLayout* newLayout = new QGridLayout(ui.E1R_2_Widget);
            	newLayout->setContentsMargins(0, 0, 0, 0);
            	newLayout->setSpacing(0);
            	ui.E1R_2_Widget->setLayout(newLayout);
        	}
        
        	// ===== 6. 添加容器到布局 =====
        	ui.E1R_2_Widget->layout()->addWidget(container);
        	container->show();
        
        	// ===== 7. 强制刷新布局（解决首次显示尺寸异常）=====
        	ui.E1R_2_Widget->layout()->activate();
        	ui.E1R_2_Widget->update();
        	ui.E1R_2_Widget->repaint();
        
        	// ===== 8. 延迟刷新：确保子窗口完成初始化后再计算尺寸 =====
        	QTimer::singleShot(50, ui.E1R_2_Widget, [=]() {
            	if (container && container->isVisible()) {
                	container->updateGeometry();
                	ui.E1R_2_Widget->layout()->activate();
            	}
        	});
        
        	Write_Worry_List("2号雷达嵌入成功");
        
    	}, Qt::QueuedConnection);
	});
	QObject::connect(m_E1R_2_Remote, &c_E1R_Remote::Connect_Done, this, [=]() {
		Write_Worry_List("2号E1R雷达连接标志置位");
		});
	QObject::connect(m_E1R_2_Remote, &c_E1R_Remote::Disconnect_Done, this, [=]() {
		Write_Worry_List("2号E1R雷达达连接标志复位");
		});
	QObject::connect(m_E1R_2_Remote, &c_E1R_Remote::is_Run, this, [=]() {
		Write_Worry_List("2号E1R雷达启动标志置位");
		ui.E1R_2_Start->setDisabled(true);
		ui.E1R_2_Close->setEnabled(true);
		ui.E1R_2_Status->Set_Working();

		});
	QObject::connect(m_E1R_2_Remote, &c_E1R_Remote::is_Stop, this, [=]() {
		Write_Worry_List("2号E1R雷达启动标志复位");
		ui.E1R_2_Start->setEnabled(true);
		ui.E1R_2_Close->setDisabled(true);
		ui.E1R_2_Status->Set_Default();
		});

	m_E1R_2_Thread->start();
}
void c_Collector_Manage::E1R_2_Delete()
{
	if (m_E1R_2_Thread->isRunning()) {
		//线程中断
		m_E1R_2_Thread->requestInterruption();
		//线程退出
		m_E1R_2_Thread->quit();
		//线程等待
		m_E1R_2_Thread->wait();
	}
}

void c_Collector_Manage::RealSense_1_Init()
{
	m_RealSense_1_Thread = new QThread();
	m_RealSense_1_Remote = new c_RealSense_Remote("1号RealSense深度");
	m_RealSense_1_Remote->moveToThread(m_RealSense_1_Thread);

	QObject::connect(m_RealSense_1_Thread, &QThread::started, m_RealSense_1_Remote, &c_RealSense_Remote::Init);
	QObject::connect(m_RealSense_1_Thread, &QThread::finished, m_RealSense_1_Remote, &c_RealSense_Remote::deleteLater);

	QObject::connect(ui.RealSense_1_Start, &QPushButton::clicked, m_RealSense_1_Remote, &c_RealSense_Remote::Connect_Device);
	QObject::connect(ui.RealSense_1_Close, &QPushButton::clicked, m_RealSense_1_Remote, &c_RealSense_Remote::Close_Device);

	QObject::connect(m_RealSense_1_Remote, &c_RealSense_Remote::Status, this, &c_Collector_Manage::Write_Worry_List);
	QObject::connect(m_RealSense_1_Remote, &c_RealSense_Remote::Show, this, [=](WId windowId) {
    
		QWindow* Window = QWindow::fromWinId(windowId);

    	if (!Window) {
        	Write_Worry_List("⚠️ 窗口指针为空");
        	return;
    	}
    
    	Write_Worry_List("嵌入1号深度界面");
    
   	 	// 1. 设置窗口标志（在主线程执行）
    	Window->setFlags(Qt::Widget);
    
    	// 2. 清理旧容器（遍历布局项）
    	QLayout* layout = ui.RealSense_20_Video->layout();
    	if (layout) {
        	QLayoutItem* item;
       	 	while ((item = layout->takeAt(0)) != nullptr) {
            	if (item->widget()) {
                	item->widget()->deleteLater();  // 安全删除，避免跨线程问题
            	}
            	if (item->layout()) {
                	// 递归清理子布局（如果有）
                	QLayout* subLayout = item->layout();
                	QLayoutItem* subItem;
                	while ((subItem = subLayout->takeAt(0)) != nullptr) {
                    	if (subItem->widget()) subItem->widget()->deleteLater();
                    	delete subItem;
                	}
                	delete subLayout;
            	}
            	delete item;
        	}
    	}
    
    	// 3. 使用 QueuedConnection 确保在主线程事件循环中执行
    	QMetaObject::invokeMethod(this, [this, Window]() {
        	// ⚠️ 二次检查：确保窗口仍然有效
        	if (!Window) {
            	Write_Worry_List("窗口已失效");
            	return;
        	}
        
        	// 4. 创建容器（必须在主线程）
        	QWidget* container = QWidget::createWindowContainer(Window, ui.RealSense_20_Video);
        	if (!container) {
            	Write_Worry_List("1号深度嵌入失败：容器创建失败");
            	return;
        	}
        
        	// 5. 设置尺寸策略（关键：让容器自适应父控件）
        	container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        	container->setMinimumSize(0, 0);
        	container->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        
        	// 6. 确保布局存在并配置
        	if (ui.RealSense_20_Video->layout() == nullptr) {
            	QGridLayout* newLayout = new QGridLayout(ui.RealSense_20_Video);
            	newLayout->setContentsMargins(0, 0, 0, 0);
            	newLayout->setSpacing(0);
            	ui.RealSense_20_Video->setLayout(newLayout);
        	}
        
        	// 7. 添加容器到布局
        	ui.RealSense_20_Video->layout()->addWidget(container);
        	container->show();
        
        	// 8. 强制刷新布局（解决首次显示尺寸异常）
        	ui.RealSense_20_Video->layout()->activate();
        	ui.RealSense_20_Video->update();
        	ui.RealSense_20_Video->repaint();
        
        	// 9. 延迟刷新：确保子窗口完成初始化后再计算尺寸
        	QTimer::singleShot(50, ui.RealSense_20_Video, [=]() {
            	if (container && container->isVisible()) {
                	container->updateGeometry();
                	ui.RealSense_20_Video->layout()->activate();
            	}
        	});
        
        	Write_Worry_List("1号深度嵌入成功");
        
    	}, Qt::QueuedConnection);
	});
	QObject::connect(m_RealSense_1_Remote, &c_RealSense_Remote::Connect_Done, this, [=]() {
		Write_Worry_List("1号RealSense深度连接标志置位");
	});
	QObject::connect(m_RealSense_1_Remote, &c_RealSense_Remote::Disconnect_Done, this, [=]() {
		Write_Worry_List("1号RealSense深度达连接标志复位");
	});
	QObject::connect(m_RealSense_1_Remote, &c_RealSense_Remote::is_Run, this, [=]() {
		Write_Worry_List("1号RealSense深度启动标志置位");
		ui.RealSense_1_Start->setDisabled(true);
		ui.RealSense_1_Close->setEnabled(true);
		ui.RealSense_1_Status->Set_Working();

	});
	QObject::connect(m_RealSense_1_Remote, &c_RealSense_Remote::is_Stop, this, [=]() {
		Write_Worry_List("1号RealSense深度启动标志复位");
		ui.RealSense_1_Start->setEnabled(true);
		ui.RealSense_1_Close->setDisabled(true);
		ui.RealSense_1_Status->Set_Default();
		});

	m_RealSense_1_Thread->start();
}
void c_Collector_Manage::RealSense_1_Delete()
{
	if (m_RealSense_1_Thread->isRunning()) {
		//线程中断
		m_RealSense_1_Thread->requestInterruption();
		//线程退出
		m_RealSense_1_Thread->quit();
		//线程等待
		m_RealSense_1_Thread->wait();
	}
}

void c_Collector_Manage::RealSense_2_Init()
{
	m_RealSense_2_Thread = new QThread();
	m_RealSense_2_Remote = new c_RealSense_Remote("2号RealSense深度");
	m_RealSense_2_Remote->moveToThread(m_RealSense_2_Thread);

	QObject::connect(m_RealSense_2_Thread, &QThread::started, m_RealSense_2_Remote, &c_RealSense_Remote::Init);
	QObject::connect(m_RealSense_2_Thread, &QThread::finished, m_RealSense_2_Remote, &c_RealSense_Remote::deleteLater);

	QObject::connect(ui.RealSense_2_Start, &QPushButton::clicked, m_RealSense_2_Remote, &c_RealSense_Remote::Connect_Device);
	QObject::connect(ui.RealSense_2_Close, &QPushButton::clicked, m_RealSense_2_Remote, &c_RealSense_Remote::Close_Device);

	QObject::connect(m_RealSense_2_Remote, &c_RealSense_Remote::Status, this, &c_Collector_Manage::Write_Worry_List);
	// ===== RealSense_2_Init() 中的 Show 信号连接 =====
	QObject::connect(m_RealSense_2_Remote, &c_RealSense_Remote::Show, this, [=](WId windowId) {
    
		QWindow* Window = QWindow::fromWinId(windowId);
		
    	Write_Worry_List("嵌入2号深度界面");
    
    	if (!Window) {
        	Write_Worry_List("⚠️ 窗口指针为空");
        	return;
    	}
    
    	Window->setFlags(Qt::Widget);
    
    	// ===== 1. 清理旧容器（遍历布局项删除）=====
    	QLayout* layout = ui.RealSense_21_Video->layout();
    	if (layout) {
        	QLayoutItem* item;
        	while ((item = layout->takeAt(0)) != nullptr) {
            	if (item->widget()) {
                	item->widget()->deleteLater();  // 安全删除，避免跨线程问题
            	}
            	if (item->layout()) {
                	// 递归清理子布局（如果有）
                	QLayout* subLayout = item->layout();
                	QLayoutItem* subItem;
                	while ((subItem = subLayout->takeAt(0)) != nullptr) {
                    	if (subItem->widget()) subItem->widget()->deleteLater();
                    	delete subItem;
                	}
                	delete subLayout;
            	}
            	delete item;
        	}
    	}
    
    	// ===== 2. 使用 QueuedConnection 确保在主线程事件循环中执行 =====
    	QMetaObject::invokeMethod(this, [this, Window]() {
        
        	// 二次检查：确保窗口仍然有效
        	if (!Window) {
            	Write_Worry_List("窗口已失效");
            	return;
        	}
        
        	// ===== 3. 创建容器（必须在主线程）=====
        	QWidget* container = QWidget::createWindowContainer(Window, ui.RealSense_21_Video);
        	if (!container) {
            	Write_Worry_List("2号深度嵌入失败：容器创建失败");
            	return;
        	}
        
        	// ===== 4. 设置尺寸策略（关键：让容器自适应父控件）=====
        	container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        	container->setMinimumSize(0, 0);
        	container->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        
        	// ===== 5. 确保布局存在并配置 =====
        	if (ui.RealSense_21_Video->layout() == nullptr) {
            	QGridLayout* newLayout = new QGridLayout(ui.RealSense_21_Video);
            	newLayout->setContentsMargins(0, 0, 0, 0);
            	newLayout->setSpacing(0);
            	ui.RealSense_21_Video->setLayout(newLayout);
        	}
        
        	// ===== 6. 添加容器到布局 =====
        	ui.RealSense_21_Video->layout()->addWidget(container);
        	container->show();
        
        	// ===== 7. 强制刷新布局（解决首次显示尺寸异常）=====
        	ui.RealSense_21_Video->layout()->activate();
        	ui.RealSense_21_Video->update();
        	ui.RealSense_21_Video->repaint();
        
        	// ===== 8. 延迟刷新：确保子窗口完成初始化后再计算尺寸 =====
        	QTimer::singleShot(50, ui.RealSense_21_Video, [=]() {
            	if (container && container->isVisible()) {
                	container->updateGeometry();
                	ui.RealSense_21_Video->layout()->activate();
            	}
        	});
        
        	Write_Worry_List("2号深度嵌入成功");
        
    	}, Qt::QueuedConnection);
	});
	QObject::connect(m_RealSense_2_Remote, &c_RealSense_Remote::Connect_Done, this, [=]() {
		Write_Worry_List("2号RealSense深度连接标志置位");
		});
	QObject::connect(m_RealSense_2_Remote, &c_RealSense_Remote::Disconnect_Done, this, [=]() {
		Write_Worry_List("2号RealSense深度达连接标志复位");
		});
	QObject::connect(m_RealSense_2_Remote, &c_RealSense_Remote::is_Run, this, [=]() {
		Write_Worry_List("2号RealSense深度启动标志置位");
		ui.RealSense_2_Start->setDisabled(true);
		ui.RealSense_2_Close->setEnabled(true);
		ui.RealSense_2_Status->Set_Working();

		});
	QObject::connect(m_RealSense_2_Remote, &c_RealSense_Remote::is_Stop, this, [=]() {
		Write_Worry_List("2号RealSense深度启动标志复位");
		ui.RealSense_2_Start->setEnabled(true);
		ui.RealSense_2_Close->setDisabled(true);
		ui.RealSense_2_Status->Set_Default();
		});

	m_RealSense_2_Thread->start();
}
void c_Collector_Manage::RealSense_2_Delete()
{
	if (m_RealSense_2_Thread->isRunning()) {
		//线程中断
		m_RealSense_2_Thread->requestInterruption();
		//线程退出
		m_RealSense_2_Thread->quit();
		//线程等待
		m_RealSense_2_Thread->wait();
	}
}

void c_Collector_Manage::Hikvision_Init()
{
	m_Hikvision_Thread = new QThread();
	m_Hikvision_Remote = new c_Hikvision_Remote("监控");
	m_Hikvision_Remote->moveToThread(m_Hikvision_Thread);

	QObject::connect(m_Hikvision_Thread, &QThread::started, m_Hikvision_Remote, &c_Hikvision_Remote::Init);
	QObject::connect(m_Hikvision_Thread, &QThread::finished, m_Hikvision_Remote, &c_Hikvision_Remote::deleteLater);

	QObject::connect(ui.Hikvision_Start, &QPushButton::clicked, m_Hikvision_Remote, &c_Hikvision_Remote::Connect_Device);
	QObject::connect(ui.Hikvision_Close, &QPushButton::clicked, m_Hikvision_Remote, &c_Hikvision_Remote::Close_Device);

	QObject::connect(m_Hikvision_Remote, &c_Hikvision_Remote::Status, this, &c_Collector_Manage::Write_Worry_List);
	// ===== Hikvision_Init() 中的 Show 信号连接 =====
	QObject::connect(m_Hikvision_Remote, &c_Hikvision_Remote::Show, this, [=](WId windowId) {
    
		QWindow* Window = QWindow::fromWinId(windowId);
		
    	Write_Worry_List("嵌入监控界面");
    
    	if (!Window) {
        	Write_Worry_List("⚠️ 窗口指针为空");
        	return;
    	}
    
    	Window->setFlags(Qt::Widget);
    
    	// ===== 1. 清理旧容器（遍历布局项删除）=====
    	QLayout* layout = ui.Hikvision_Video->layout();
    	if (layout) {
        	QLayoutItem* item;
        	while ((item = layout->takeAt(0)) != nullptr) {
            	if (item->widget()) {
                	item->widget()->deleteLater();  // 安全删除，避免跨线程问题
            	}
            	if (item->layout()) {
                	// 递归清理子布局（如果有）
                	QLayout* subLayout = item->layout();
                	QLayoutItem* subItem;
                	while ((subItem = subLayout->takeAt(0)) != nullptr) {
                    	if (subItem->widget()) subItem->widget()->deleteLater();
                    	delete subItem;
                	}
                	delete subLayout;
            	}
            	delete item;
        	}
    	}
    
    	// ===== 2. 使用 QueuedConnection 确保在主线程事件循环中执行 =====
    	QMetaObject::invokeMethod(this, [this, Window]() {
        
        	// 二次检查：确保窗口仍然有效
        	if (!Window) {
            	Write_Worry_List("窗口已失效");
            	return;
        	}
        
        	// ===== 3. 创建容器（必须在主线程）=====
        	QWidget* container = QWidget::createWindowContainer(Window, ui.Hikvision_Video);
        	if (!container) {
            	Write_Worry_List("监控嵌入失败：容器创建失败");
            	return;
        	}
        
        	// ===== 4. 设置尺寸策略（关键：让容器自适应父控件）=====
        	container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        	container->setMinimumSize(0, 0);
        	container->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        
        	// ===== 5. 确保布局存在并配置 =====
        	if (ui.Hikvision_Video->layout() == nullptr) {
            	QGridLayout* newLayout = new QGridLayout(ui.Hikvision_Video);
            	newLayout->setContentsMargins(0, 0, 0, 0);
            	newLayout->setSpacing(0);
            	ui.Hikvision_Video->setLayout(newLayout);
        	}
        
        	// ===== 6. 添加容器到布局 =====
        	ui.Hikvision_Video->layout()->addWidget(container);
        	container->show();
        
        	// ===== 7. 强制刷新布局（解决首次显示尺寸异常）=====
        	ui.Hikvision_Video->layout()->activate();
        	ui.Hikvision_Video->update();
        	ui.Hikvision_Video->repaint();
        
        	// ===== 8. 延迟刷新：确保子窗口完成初始化后再计算尺寸 =====
        	QTimer::singleShot(50, ui.Hikvision_Video, [=]() {
            	if (container && container->isVisible()) {
                	container->updateGeometry();
                	ui.Hikvision_Video->layout()->activate();
            	}
        	});
        
        	Write_Worry_List("监控嵌入成功");
        
    	}, Qt::QueuedConnection);
	});
	QObject::connect(m_Hikvision_Remote, &c_Hikvision_Remote::Connect_Done, this, [=]() {
		Write_Worry_List("监控连接标志置位");
		});
	QObject::connect(m_Hikvision_Remote, &c_Hikvision_Remote::Disconnect_Done, this, [=]() {
		Write_Worry_List("监控连接标志复位");
		});
	QObject::connect(m_Hikvision_Remote, &c_Hikvision_Remote::is_Run, this, [=]() {
		Write_Worry_List("监控启动标志置位");
		ui.Hikvision_Start->setDisabled(true);
		ui.Hikvision_Close->setEnabled(true);
		ui.Hikvision_Status->Set_Working();

		});
	QObject::connect(m_Hikvision_Remote, &c_Hikvision_Remote::is_Stop, this, [=]() {
		Write_Worry_List("监控启动标志复位");
		ui.Hikvision_Start->setEnabled(true);
		ui.Hikvision_Close->setDisabled(true);
		ui.Hikvision_Status->Set_Default();
		});

	m_Hikvision_Thread->start();
}
void c_Collector_Manage::Hikvision_Delete()
{
	if (m_Hikvision_Thread->isRunning()) {
		//线程中断
		m_Hikvision_Thread->requestInterruption();
		//线程退出
		m_Hikvision_Thread->quit();
		//线程等待
		m_Hikvision_Thread->wait();
	}
}

void c_Collector_Manage::CH10X_Init()
{
	m_CH10X_Thread = new QThread();
	m_CH10X_Remote = new c_CH10X_Remote("惯导");
	m_CH10X_Remote->moveToThread(m_CH10X_Thread);

	QObject::connect(m_CH10X_Thread, &QThread::started, m_CH10X_Remote, &c_CH10X_Remote::Init);
	QObject::connect(m_CH10X_Thread, &QThread::finished, m_CH10X_Remote, &c_CH10X_Remote::deleteLater);

	QObject::connect(ui.CH10X_Start, &QPushButton::clicked, m_CH10X_Remote, &c_CH10X_Remote::Connect_Device);
	QObject::connect(ui.CH10X_Close, &QPushButton::clicked, m_CH10X_Remote, &c_CH10X_Remote::Close_Device);

	QObject::connect(m_CH10X_Remote, &c_CH10X_Remote::Status, this, &c_Collector_Manage::Write_Worry_List);
	// ===== CH10X_Init() 中的 Show 信号连接 =====
	QObject::connect(m_CH10X_Remote, &c_CH10X_Remote::Show, this, [=](WId windowId) {
    
		QWindow* Window = QWindow::fromWinId(windowId);
		
    	Write_Worry_List("嵌入惯导界面");
    
    	if (!Window) {
        	Write_Worry_List("窗口指针为空");
        	return;
    	}
    
    	Window->setFlags(Qt::Widget);
    
    	// ===== 1. 清理旧容器（遍历布局项删除）=====
    	QLayout* layout = ui.CH10X_widget->layout();
    	if (layout) {
        	QLayoutItem* item;
        	while ((item = layout->takeAt(0)) != nullptr) {
            	if (item->widget()) {
                	item->widget()->deleteLater();  // 安全删除，避免跨线程问题
            	}
            	if (item->layout()) {
                	// 递归清理子布局（如果有）
                	QLayout* subLayout = item->layout();
                	QLayoutItem* subItem;
                	while ((subItem = subLayout->takeAt(0)) != nullptr) {
                    	if (subItem->widget()) subItem->widget()->deleteLater();
                    	delete subItem;
                	}
                	delete subLayout;
            	}
            	delete item;
        	}
    	}
    
    	// ===== 2. 使用 QueuedConnection 确保在主线程事件循环中执行 =====
    	QMetaObject::invokeMethod(this, [this, Window]() {
        
        	// 二次检查：确保窗口仍然有效
        	if (!Window) {
            	Write_Worry_List("窗口已失效");
            	return;
        	}
        
        	// ===== 3. 创建容器（必须在主线程）=====
        	QWidget* container = QWidget::createWindowContainer(Window, ui.CH10X_widget);
        	if (!container) {
            	Write_Worry_List("惯导嵌入失败：容器创建失败");
            	return;
        	}
        
        	// ===== 4. 设置尺寸策略（关键：让容器自适应父控件）=====
        	container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        	container->setMinimumSize(0, 0);
        	container->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        
        	// ===== 5. 确保布局存在并配置 =====
        	if (ui.CH10X_widget->layout() == nullptr) {
            	QGridLayout* newLayout = new QGridLayout(ui.CH10X_widget);
            	newLayout->setContentsMargins(0, 0, 0, 0);
            	newLayout->setSpacing(0);
            	ui.CH10X_widget->setLayout(newLayout);
        	}
        
        	// ===== 6. 添加容器到布局 =====
        	ui.CH10X_widget->layout()->addWidget(container);
        	container->show();
        
        	// ===== 7. 强制刷新布局（解决首次显示尺寸异常）=====
        	ui.CH10X_widget->layout()->activate();
        	ui.CH10X_widget->update();
        	ui.CH10X_widget->repaint();
        
        	// ===== 8. 延迟刷新：确保子窗口完成初始化后再计算尺寸 =====
        	QTimer::singleShot(50, ui.CH10X_widget, [=]() {
            	if (container && container->isVisible()) {
                	container->updateGeometry();
                	ui.CH10X_widget->layout()->activate();
            	}
        	});
        
        	Write_Worry_List("惯导嵌入成功");
        
    	}, Qt::QueuedConnection);
	});
	QObject::connect(m_CH10X_Remote, &c_CH10X_Remote::Connect_Done, this, [=]() {
		Write_Worry_List("惯导连接标志置位");
	});
	QObject::connect(m_CH10X_Remote, &c_CH10X_Remote::Disconnect_Done, this, [=]() {
		Write_Worry_List("惯导连接标志复位");
	});
	QObject::connect(m_CH10X_Remote, &c_CH10X_Remote::is_Run, this, [=]() {
		Write_Worry_List("惯导启动标志置位");
		ui.CH10X_Start->setDisabled(true);
		ui.CH10X_Close->setEnabled(true);
		ui.CH10X_Status->Set_Working();
	});
	QObject::connect(m_CH10X_Remote, &c_CH10X_Remote::is_Stop, this, [=]() {
		Write_Worry_List("惯导启动标志复位");
		ui.CH10X_Start->setEnabled(true);
		ui.CH10X_Close->setDisabled(true);
		ui.CH10X_Status->Set_Default();
	});

	m_CH10X_Thread->start();
}
void c_Collector_Manage::CH10X_Scan()
{
 	// === 新增：更新 2D 地图 (每 50ms 更新一次，即 20Hz，避免 UI 卡顿) ===
    static int map_update_counter = 0;
    if (++map_update_counter >= 5) { 
        map_update_counter = 0;
        
        // 只有当时间戳有效（子进程已发送数据）时才更新
        if (c_Variable::getInstance().g_IMU_Odom.timestamp > 0) {
            ui.map_widget->updatePosition(c_Variable::getInstance().g_IMU_Odom.x, 
										  c_Variable::getInstance().g_IMU_Odom.y, 
										  c_Variable::getInstance().g_IMU_Odom.yaw);
        }
    }
}
void c_Collector_Manage::CH10X_Delete()
{
	if (m_CH10X_Thread->isRunning()) {
		//线程中断
		m_CH10X_Thread->requestInterruption();
		//线程退出
		m_CH10X_Thread->quit();
		//线程等待
		m_CH10X_Thread->wait();
	}
}

void c_Collector_Manage::Work_Remote_Init()
{
	m_Work_Remote_Thread = new QThread();
	m_Work_Remote = new c_Work_Remote();
	m_Work_Remote->moveToThread(m_Work_Remote_Thread);

	m_Local_Remote_Thread = new QThread();
 	m_Local_Remote = new c_Local_Remote();
	m_Local_Remote->moveToThread(m_Local_Remote_Thread);

	m_Sql_Remote_Thread = new QThread();
 	m_Sql_Remote = new c_Sql_Remote();
	m_Sql_Remote->moveToThread(m_Sql_Remote_Thread);

	QObject::connect(m_Work_Remote_Thread, &QThread::started, m_Work_Remote, &c_Work_Remote::Init);
	QObject::connect(m_Work_Remote_Thread, &QThread::finished, m_Work_Remote, &c_Work_Remote::deleteLater);

	QObject::connect(m_Local_Remote_Thread, &QThread::started, m_Local_Remote, &c_Local_Remote::Init);
	QObject::connect(m_Local_Remote_Thread, &QThread::finished, m_Local_Remote, &c_Local_Remote::deleteLater);

	QObject::connect(m_Sql_Remote_Thread, &QThread::started, m_Sql_Remote, &c_Sql_Remote::Init);
	QObject::connect(m_Sql_Remote_Thread, &QThread::finished, m_Sql_Remote, &c_Sql_Remote::deleteLater);

	QObject::connect(m_Work_Remote, &c_Work_Remote::Status, this, &c_Collector_Manage::Write_Work_List);
	QObject::connect(m_Local_Remote, &c_Local_Remote::Status, this, &c_Collector_Manage::Write_Work_List);
	QObject::connect(m_Sql_Remote, &c_Sql_Remote::Status, this, &c_Collector_Manage::Write_Work_List);

	c_Collector_Manage::Work_Remote_DB();
	c_Collector_Manage::Work_Remote_Button();

	m_Work_Remote_Thread->start();
	m_Local_Remote_Thread->start();
	m_Sql_Remote_Thread->start();
}
void c_Collector_Manage::Work_Remote_DB()
{
	ui.Local_Ip->setText(c_Variable::getInstance().g_Communicate_DB.value("Local_Ip").toString());
	ui.Web_Server_Ip->setText(c_Variable::getInstance().g_Communicate_DB.value("Web_Server_Ip").toString());
	ui.Local_Remote_Port->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("Local_Remote_Port").toInt()));
	QObject::connect(ui.Local_Ip, &QLineEdit::textChanged, this, [=](QString ip) {Write_Communicate_DB("Local_Ip", ip); });
	QObject::connect(ui.Web_Server_Ip, &QLineEdit::textChanged, this, [=](QString ip) {Write_Communicate_DB("Web_Server_Ip", ip); });
	QObject::connect(ui.Local_Remote_Port, &QLineEdit::textChanged, this, [=](QString port) {Write_Communicate_DB("Local_Remote_Port", port.toInt()); });

	ui.MySql_Port->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("MySql_Port").toInt()));
	ui.MySql_User->setText(c_Variable::getInstance().g_Communicate_DB.value("MySql_User").toString());
	ui.MySql_Password->setText(c_Variable::getInstance().g_Communicate_DB.value("MySql_Password").toString());
	ui.MySql_Database->setText(c_Variable::getInstance().g_Communicate_DB.value("MySql_Database").toString());

	QObject::connect(ui.MySql_Port, &QLineEdit::textChanged, this, [=](QString port) {Write_Communicate_DB("MySql_Port", port.toInt()); });
	QObject::connect(ui.MySql_User, &QLineEdit::textChanged, this, [=](QString user) {Write_Communicate_DB("MySql_User", user); });
	QObject::connect(ui.MySql_Password, &QLineEdit::textChanged, this, [=](QString password) {Write_Communicate_DB("MySql_Password", password); });
	QObject::connect(ui.MySql_Database, &QLineEdit::textChanged, this, [=](QString database) {Write_Communicate_DB("MySql_Database", database); });
}
void c_Collector_Manage::Work_Remote_Button()
{
	QObject::connect(m_Scan_Server_120, &c_Scan_Server_120::updateCollectTime, m_Sql_Remote, &c_Sql_Remote::Prec_Scan_Write_Done);
	QObject::connect(m_Scan_Server_121, &c_Scan_Server_121::updateCollectTime, m_Sql_Remote, &c_Sql_Remote::Prec_Scan_Write_Done);

	QObject::connect(m_Local_Remote, &c_Local_Remote::Set_Working, ui.Local_Remote_Working_State, &c_Fr_Light::Set_Working);
	QObject::connect(m_Local_Remote, &c_Local_Remote::Set_Default, ui.Local_Remote_Working_State, &c_Fr_Light::Set_Default);

	QObject::connect(m_Local_Remote, &c_Local_Remote::Start_Cmd, m_Work_Remote, &c_Work_Remote::Start_Cmd);
	QObject::connect(m_Work_Remote, &c_Work_Remote::Write_Json, m_Local_Remote, &c_Local_Remote::Write_Json);

	QObject::connect(m_Work_Remote, &c_Work_Remote::updateFastScanTime, m_Sql_Remote, &c_Sql_Remote::updateFastScanTime);//快扫完成时间
	QObject::connect(m_Work_Remote, &c_Work_Remote::updateActualReturnTime, m_Sql_Remote, &c_Sql_Remote::updateActualReturnTime);//巡检完成时间

	QObject::connect(m_Work_Remote, &c_Work_Remote::AutoEn_120, ui.Huayan_120_AutoEn, &QPushButton::click);
	QObject::connect(m_Work_Remote, &c_Work_Remote::AutoDn_120, ui.Huayan_120_AutoDn, &QPushButton::click);
	QObject::connect(m_Work_Remote, &c_Work_Remote::AutoEn_121, ui.Huayan_121_AutoEn, &QPushButton::click);
	QObject::connect(m_Work_Remote, &c_Work_Remote::AutoDn_121, ui.Huayan_121_AutoDn, &QPushButton::click);

	QObject::connect(m_Huayan_Remote_120, &c_Huayan_Remote_120::GrpEnable_Done,m_Work_Remote, &c_Work_Remote::AutoEn_Done_120);
	QObject::connect(m_Huayan_Remote_120, &c_Huayan_Remote_120::BlackOut_Done,m_Work_Remote, &c_Work_Remote::AutoDn_Done_120);
	QObject::connect(m_Huayan_Remote_121, &c_Huayan_Remote_121::GrpEnable_Done,m_Work_Remote, &c_Work_Remote::AutoEn_Done_121);
	QObject::connect(m_Huayan_Remote_121, &c_Huayan_Remote_121::BlackOut_Done,m_Work_Remote, &c_Work_Remote::AutoDn_Done_121);
	
	QObject::connect(m_Work_Remote, &c_Work_Remote::RunFunc_120, this, [=](QString cmd){
        ui.Huayan_120_FuncName->setText(cmd);
		Write_Worry_List(QString("左臂：运行脚本函数 [%1]").arg(cmd));
        if (!cmd.isEmpty()) {
            QStringList params;
             // 跨线程调用发送（确保在 TCP_Client 所在线程执行）
    		QMetaObject::invokeMethod(m_Huayan_Remote_120, "RunFunc", Qt::QueuedConnection,
                Q_ARG(QString, cmd),Q_ARG(QStringList, params));
        }
    });
	QObject::connect(m_Work_Remote, &c_Work_Remote::RunFunc_121, this, [=](QString cmd){
        ui.Huayan_121_FuncName->setText(cmd);
		Write_Worry_List(QString("右臂：运行脚本函数 [%1]").arg(cmd));
        if (!cmd.isEmpty()) {
            QStringList params;
             // 跨线程调用发送（确保在 TCP_Client 所在线程执行）
    		QMetaObject::invokeMethod(m_Huayan_Remote_121, "RunFunc", Qt::QueuedConnection,
                Q_ARG(QString, cmd),Q_ARG(QStringList, params));
        }
    });

	QObject::connect(ui.Huayan_120_Moving, &c_Fr_Light::Working_State, m_Work_Remote, &c_Work_Remote::Huayan_120_Moving);
	QObject::connect(ui.Huayan_121_Moving, &c_Fr_Light::Working_State, m_Work_Remote, &c_Work_Remote::Huayan_121_Moving);
	QObject::connect(ui.Robot_120, &c_Fr_Light::Working_State, m_Work_Remote, &c_Work_Remote::Robot_120);
	QObject::connect(ui.Robot_121, &c_Fr_Light::Working_State, m_Work_Remote, &c_Work_Remote::Robot_121);
	QObject::connect(ui.Pre_Scan_Done, &c_Fr_Light::Working_State, m_Work_Remote, &c_Work_Remote::Pre_Scan_Done);
}
void c_Collector_Manage::Work_Remote_Scan()
{
	ui.Local_Remote_Tran->Set_State(c_Variable::getInstance().g_Local_Remote.Tran);
	ui.Local_Remote_Num->setText(QString::number(c_Variable::getInstance().g_Local_Remote.num));

	ui.Work_Connected->Set_State(c_Variable::getInstance().g_Work.Connected);

	ui.Head_Toward->setText(c_Variable::getInstance().g_Work.Head_Toward);
	ui.Work_Stage->setText(c_Variable::getInstance().g_Work.Work_Stage);
	c_Variable::getInstance().g_Work.Work_Num = QString("%1%2%3")
		.arg(c_Variable::getInstance().g_Work.Begin_Time)
		.arg(c_Variable::getInstance().g_Work.Car_Type)
		.arg(c_Variable::getInstance().g_Work.Car_Num);
	ui.Work_Num->setText(c_Variable::getInstance().g_Work.Work_Num);
	ui.Track_Id->setText(c_Variable::getInstance().g_Work.Track_Id);
	ui.Car_Type->setText(c_Variable::getInstance().g_Work.Car_Type);
	ui.Car_Num->setText(c_Variable::getInstance().g_Work.Car_Num);
	ui.Begin_Time->setText(c_Variable::getInstance().g_Work.Begin_Time);
	ui.Carbox_Num->setText(c_Variable::getInstance().g_Work.Carbox_Num);
	ui.Bogie_Num->setText(c_Variable::getInstance().g_Work.Bogie_Num);
	ui.Axis_Num->setText(c_Variable::getInstance().g_Work.Axis_Num);
	ui.Wheelset_Num_120->setText(c_Variable::getInstance().g_Work.Wheelset_Num_120);
	ui.Wheelset_Num_121->setText(c_Variable::getInstance().g_Work.Wheelset_Num_121);
	ui.Primary_Components_120->setText(c_Variable::getInstance().g_Work.Primary_Components_120);
	ui.Primary_Components_121->setText(c_Variable::getInstance().g_Work.Primary_Components_121);
	ui.Secondary_Components_120->setText(c_Variable::getInstance().g_Work.Secondary_Components_120);
	ui.Secondary_Components_121->setText(c_Variable::getInstance().g_Work.Secondary_Components_121);
	ui.Point_Num_120->setText(c_Variable::getInstance().g_Work.Point_Num_120);
	ui.Point_Num_121->setText(c_Variable::getInstance().g_Work.Point_Num_121);
	ui.Robot_120->Set_State(c_Variable::getInstance().g_Work.Connected && c_Variable::getInstance().g_Work.Robot_120);
	ui.Robot_121->Set_State(c_Variable::getInstance().g_Work.Connected && c_Variable::getInstance().g_Work.Robot_121);
	ui.Pre_Scan_Done->Set_State(c_Variable::getInstance().g_Work.Connected && c_Variable::getInstance().g_Work.Robot_120 && c_Variable::getInstance().g_Work.Robot_121);
}
void c_Collector_Manage::Work_Remote_Delete()
{
	if (m_Work_Remote_Thread->isRunning()) {
		//线程中断
		m_Work_Remote_Thread->requestInterruption();
		//线程退出
		m_Work_Remote_Thread->quit();
		//线程等待
		m_Work_Remote_Thread->wait();
	}
	if (m_Local_Remote_Thread->isRunning()) {
		//线程中断
		m_Local_Remote_Thread->requestInterruption();
		//线程退出
		m_Local_Remote_Thread->quit();
		//线程等待
		m_Local_Remote_Thread->wait();
	}
	if (m_Sql_Remote_Thread->isRunning()) {
		//线程中断
		m_Sql_Remote_Thread->requestInterruption();
		//线程退出
		m_Sql_Remote_Thread->quit();
		//线程等待
		m_Sql_Remote_Thread->wait();
	}
}

void c_Collector_Manage::Write_Prec_Scan_120_Cmd(QString value)
{
	ui.Prec_Scan_120_Cmd->append(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss-zzz") + "->" + value);

	QFile File;
	File.setFileName(m_Debug_Path + "/左精扫消息.log");
	File.open(QIODevice::ReadWrite | QIODevice::Text);

	QString date = ui.Prec_Scan_120_Cmd->toPlainText();

	File.write(date.toUtf8());
	File.close();
}
void c_Collector_Manage::Write_Prec_Scan_121_Cmd(QString value)
{
	ui.Prec_Scan_121_Cmd->append(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss-zzz") + "->" + value);

	QFile File;
	File.setFileName(m_Debug_Path + "/右精扫消息.log");
	File.open(QIODevice::ReadWrite | QIODevice::Text);

	QString date = ui.Prec_Scan_121_Cmd->toPlainText();

	File.write(date.toUtf8());
	File.close();
}
void c_Collector_Manage::Write_Communicate_DB(QString key, int value)
{
	c_Variable::getInstance().g_Communicate_DB.insert(key, value);

	QFile File;
	File.setFileName(QDir::currentPath() + "/Collector_Manage.json");
	
	
	if (!File.open(QIODevice::ReadWrite | QIODevice::Text)) {
    	return;
	}
	
	QJsonDocument DB_Doc;
	DB_Doc.setObject(c_Variable::getInstance().g_Communicate_DB);

	File.write(DB_Doc.toJson());
	File.close();
}
void c_Collector_Manage::Write_Communicate_DB(QString key, QString value)
{
	c_Variable::getInstance().g_Communicate_DB.insert(key, value);

	QFile File;
	File.setFileName(QDir::currentPath() + "/Collector_Manage.json");
	
	if (!File.open(QIODevice::ReadWrite | QIODevice::Text)) {
    	return;
	}

	QJsonDocument DB_Doc;
	DB_Doc.setObject(c_Variable::getInstance().g_Communicate_DB);

	File.write(DB_Doc.toJson());
	File.close();
}
void c_Collector_Manage::Write_Communicate_DB(QString key, double value)
{
	c_Variable::getInstance().g_Communicate_DB.insert(key, value);

	QFile File;
	File.setFileName(QDir::currentPath() + "/Collector_Manage.json");
	
	if (!File.open(QIODevice::ReadWrite | QIODevice::Text)) {
    	return;
	}

	QJsonDocument DB_Doc;
	DB_Doc.setObject(c_Variable::getInstance().g_Communicate_DB);

	File.write(DB_Doc.toJson());
	File.close();
}
void c_Collector_Manage::Write_Worry_List(QString value)
{
	ui.Worry_List->append(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss") + "->" + value);

	QFile File;
	File.setFileName(m_Debug_Path + "/系统信息.log");

	if (!File.open(QIODevice::ReadWrite | QIODevice::Text)) {
    	return;
	}

	QString date = ui.Worry_List->toPlainText();

	File.write(date.toUtf8());
	File.close();
}
void c_Collector_Manage::Write_Work_List(QString value)
{
	ui.Work_List->append(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss") + "->" + value);

	c_Variable::getInstance().g_Work_List.append(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss") + "->" + value);


	if (c_Variable::getInstance().g_Work_List.size() > 10) {
		c_Variable::getInstance().g_Work_List.removeAt(0);
	}

	QFile File;
	File.setFileName(m_Debug_Path + "/巡检过程信息.log");
	
	if (!File.open(QIODevice::ReadWrite | QIODevice::Text)) {
    	return;
	}
	
	QString date = ui.Work_List->toPlainText();

	File.write(date.toUtf8());
	File.close();
}
void c_Collector_Manage::Write_RGV_List(QString value)
{
	ui.RGV_List->append(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss") + "->" + value);

	QFile File;
	File.setFileName(m_Debug_Path + "/RGV信息.log");
	
	if (!File.open(QIODevice::ReadWrite | QIODevice::Text)) {
    	return;
	}
	
	QString date = ui.RGV_List->toPlainText();

	File.write(date.toUtf8());
	File.close();
}

void c_Collector_Manage::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_F6) {
		QFile File;
		File.setFileName(QDir::currentPath() + "/stuqss.css");
		
		if (!File.open(QIODevice::ReadWrite | QIODevice::Text)) {
    		return;
		}
	
		QString strQss = File.readAll();
		this->setStyleSheet(strQss);

		File.close();
	}
}
void c_Collector_Manage::closeEvent(QCloseEvent *event) {
	int ret = QMessageBox::question(this, "确认退出", "确定要退出吗？");
	if (ret == QMessageBox::Yes) {
		m_Scan = false;
		c_Collector_Manage::Huayan_120_Delete();
		c_Collector_Manage::Huayan_121_Delete();
		c_Collector_Manage::Zivid_1_Delete();
		c_Collector_Manage::Zivid_2_Delete();
		c_Collector_Manage::E1R_1_Delete();
		c_Collector_Manage::E1R_2_Delete();
		c_Collector_Manage::RealSense_1_Delete();
		c_Collector_Manage::RealSense_2_Delete();
		c_Collector_Manage::Hikvision_Delete();
		c_Collector_Manage::RGV_Delete();
		c_Collector_Manage::CH10X_Delete();
		c_Collector_Manage::Work_Remote_Delete();

		m_Object.msleep(3000);
		
		event->accept();
	}
	else {
		event->ignore();
	}
}

