#include "HIPNUC_CH10X.h"

c_HIPNUC_CH10X::c_HIPNUC_CH10X(QStringList info, QWidget *parent) : QWidget(parent) {
    ui.setupUi(this);
    
    // 加载样式表
    QFile File(QDir::currentPath() + "/stuqss.css");
    if (File.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(File.readAll());
        File.close();
    }
    
    // 解析命令行参数 (设备名称)
    if (!info.isEmpty() && info.size() > 1) {
        c_Variable::getInstance().g_HIPNUC_CH10X.device_name = info.at(1);
        this->setWindowFlags(Qt::Widget);
        this->setAttribute(Qt::WA_NativeWindow);
        this->setWindowFlags(Qt::FramelessWindowHint);
        this->setWindowTitle(c_Variable::getInstance().g_HIPNUC_CH10X.device_name);
        m_DB_Path = QDir::currentPath() + "/" + c_Variable::getInstance().g_HIPNUC_CH10X.device_name + ".json";
    } else {
        m_DB_Path = QDir::currentPath() + "/HIPNUC_CH10X.json";
    }
    
    // 加载 JSON 配置
    QFile DbFile(m_DB_Path);
    if (DbFile.open(QFile::ReadOnly | QIODevice::Text)) {
        QJsonParseError parseError;
        QJsonDocument DB_Doc = QJsonDocument::fromJson(DbFile.readAll(), &parseError);
        if (parseError.error == QJsonParseError::NoError) {
            c_Variable::getInstance().g_Communicate_DB = DB_Doc.object();
        }
        DbFile.close();
    }
    
    // 监听 stdin (主进程通信)
    m_file.open(stdin, QIODevice::ReadOnly | QIODevice::Text);
    m_pNotifier = new QSocketNotifier(m_file.handle(), QSocketNotifier::Read, this);
    QObject::connect(m_pNotifier, &QSocketNotifier::activated, this, [=](int iHandle){
        if(m_file.handle() == iHandle){
            int iRet = read(m_file.handle(), m_arrRecv, sizeof(m_arrRecv));
            m_arrRecv[iRet] = 0;
            readMessage(QString("%1").arg(m_arrRecv));
        }
    });
    
    // 通知主进程嵌入窗口
    c_Variable::getInstance().writeMessage(1, QString("Run&%1").arg(this->winId()));
    m_Scan = true;
    this->show();
}

c_HIPNUC_CH10X::~c_HIPNUC_CH10X() {}

void c_HIPNUC_CH10X::readMessage(QString message) {
    c_Variable::getInstance().writeMessage(2, "收到主进程指令: " + message);
    if (message == "Connect") ui.CH10X_Connect->click();
    if (message == "Disconnect") ui.CH10X_Disconnect->click();
    if (message == "Close_Device") this->close();
    if (message.startsWith("Set_Static&")) {
        QStringList parts = message.split("&");
        if (parts.size() == 2) {
            c_Variable::getInstance().g_IMU_Odom.is_static = (parts.at(1).toInt() == 1);
        }
    }
}

void c_HIPNUC_CH10X::Refresh_Ports() {
    ui.Device_Port->clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        ui.Device_Port->addItem(info.systemLocation());
    }
    if (ui.Device_Port->count() == 0) {
        ui.Device_Port->addItem("/dev/ttyUSB0");
    }
}

void c_HIPNUC_CH10X::Init() {
    m_Remote = new c_HIPNUC_CH10X_Remote;
    m_Remote_Thread = new QThread;
    m_Remote->moveToThread(m_Remote_Thread);
    
    Refresh_Ports();

    QObject::connect(m_Remote_Thread, &QThread::started, m_Remote, &c_HIPNUC_CH10X_Remote::Init);
    QObject::connect(m_Remote_Thread, &QThread::finished, m_Remote, &c_HIPNUC_CH10X_Remote::deleteLater);
    
    // 绑定 UI 按钮
    QObject::connect(ui.CH10X_Connect, &QPushButton::clicked, m_Remote, &c_HIPNUC_CH10X_Remote::Connect);
    QObject::connect(ui.CH10X_Disconnect, &QPushButton::clicked, m_Remote, &c_HIPNUC_CH10X_Remote::Disconnect);
    QObject::connect(ui.Refresh_Ports, &QPushButton::clicked, this, &c_HIPNUC_CH10X::Refresh_Ports);
    QObject::connect(ui.Set_High_Performance_Mode, &QPushButton::clicked, m_Remote, &c_HIPNUC_CH10X_Remote::Set_High_Performance_Mode);

    // 绑定日志
    QObject::connect(m_Remote, &c_HIPNUC_CH10X_Remote::Status, this, [=](QString cmd) {
        c_Variable::getInstance().writeMessage(2, cmd);
    });
    
    // 绑定配置保存
    QObject::connect(ui.Device_Port, &QComboBox::currentTextChanged, this, [=](QString val){
        c_Variable::getInstance().g_HIPNUC_CH10X.port_name = val;
        Write_Communicate_DB("Device_Port", val);
    });
    QObject::connect(ui.Baud_Rate, &QComboBox::currentTextChanged, this, [=](QString rate){
        c_Variable::getInstance().g_HIPNUC_CH10X.baud_rate = rate.toInt();
        Write_Communicate_DB("Baud_Rate", rate.toInt());
    });
    
    // 从全局变量或 JSON 加载配置
    c_Variable::getInstance().g_HIPNUC_CH10X.port_name = c_Variable::getInstance().g_Communicate_DB.value("Device_Port").toString();
    c_Variable::getInstance().g_HIPNUC_CH10X.baud_rate = c_Variable::getInstance().g_Communicate_DB.value("Baud_Rate").toInt();
    
    // ✅ 修正：使用 findText 而不是 findData，因为 addItem 时没有传入 userData
    int baudIdx = ui.Baud_Rate->findText(QString::number(c_Variable::getInstance().g_HIPNUC_CH10X.baud_rate));
    if (baudIdx >= 0) {
        ui.Baud_Rate->setCurrentIndex(baudIdx);
    }
    
    int portIdx = ui.Device_Port->findText(c_Variable::getInstance().g_HIPNUC_CH10X.port_name);
    if (portIdx >= 0) {
        ui.Device_Port->setCurrentIndex(portIdx);
    }
    
    m_Remote_Thread->start();
}

void c_HIPNUC_CH10X::System_Scan() {
    ui.CH10X_Connected->Set_State(c_Variable::getInstance().g_HIPNUC_CH10X.connected);
    ui.CH10X_Connect->setDisabled(c_Variable::getInstance().g_HIPNUC_CH10X.connected);
    ui.CH10X_Disconnect->setEnabled(c_Variable::getInstance().g_HIPNUC_CH10X.connected);
    
    static qint64 lastTime = QDateTime::currentMSecsSinceEpoch();
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    if (now - lastTime >= 1000) {
        c_Variable::getInstance().g_HIPNUC_CH10X.current_fps = c_Variable::getInstance().g_HIPNUC_CH10X.fps_count;
        c_Variable::getInstance().g_HIPNUC_CH10X.fps_count = 0;
        lastTime = now;
    }
    
    static bool heartbeat = false;
    if (c_Variable::getInstance().g_HIPNUC_CH10X.connected && (now - c_Variable::getInstance().g_HIPNUC_CH10X.last_update_time < 2000)) {
        heartbeat = !heartbeat;
        ui.CH10X_Heartbeat->Set_State(heartbeat);
    } else {
        ui.CH10X_Heartbeat->Set_State(false);
    }
    
    // 注意：手册中 euler 顺序为 [0]=Roll, [1]=Pitch, [2]=Yaw
    ui.Euler_Roll->setText(QString::number(c_Variable::getInstance().g_HIPNUC_CH10X.euler[0], 'f', 2));
    ui.Euler_Pitch->setText(QString::number(c_Variable::getInstance().g_HIPNUC_CH10X.euler[1], 'f', 2));
    ui.Euler_Yaw->setText(QString::number(c_Variable::getInstance().g_HIPNUC_CH10X.euler[2], 'f', 2));
    ui.Acc_X->setText(QString::number(c_Variable::getInstance().g_HIPNUC_CH10X.acc[0], 'f', 3));
    ui.Acc_Y->setText(QString::number(c_Variable::getInstance().g_HIPNUC_CH10X.acc[1], 'f', 3));
    ui.Acc_Z->setText(QString::number(c_Variable::getInstance().g_HIPNUC_CH10X.acc[2], 'f', 3));
    ui.Gyro_X->setText(QString::number(c_Variable::getInstance().g_HIPNUC_CH10X.gyro[0], 'f', 2));
    ui.Gyro_Y->setText(QString::number(c_Variable::getInstance().g_HIPNUC_CH10X.gyro[1], 'f', 2));
    ui.Gyro_Z->setText(QString::number(c_Variable::getInstance().g_HIPNUC_CH10X.gyro[2], 'f', 2));
    ui.Quat_W->setText(QString::number(c_Variable::getInstance().g_HIPNUC_CH10X.quat[0], 'f', 4));
    ui.Quat_X->setText(QString::number(c_Variable::getInstance().g_HIPNUC_CH10X.quat[1], 'f', 4));
    ui.Quat_Y->setText(QString::number(c_Variable::getInstance().g_HIPNUC_CH10X.quat[2], 'f', 4));
    ui.Quat_Z->setText(QString::number(c_Variable::getInstance().g_HIPNUC_CH10X.quat[3], 'f', 4));
    ui.FPS_Display->setText(QString::number(c_Variable::getInstance().g_HIPNUC_CH10X.current_fps));

    ui.IMU_Odom_x->setText(QString::number(c_Variable::getInstance().g_IMU_Odom.x, 'f', 3));
    ui.IMU_Odom_y->setText(QString::number(c_Variable::getInstance().g_IMU_Odom.y, 'f', 3));
    ui.IMU_Odom_vx->setText(QString::number(c_Variable::getInstance().g_IMU_Odom.vx, 'f', 3));
    ui.IMU_Odom_vy->setText(QString::number(c_Variable::getInstance().g_IMU_Odom.vy, 'f', 2));
    ui.IMU_Odom_tamp->setText(QString::number(c_Variable::getInstance().g_IMU_Odom.timestamp));
    ui.IMU_Odom_static->Set_State(c_Variable::getInstance().g_IMU_Odom.is_static);

    
    QTimer::singleShot(100, this, [this]() {
        if (m_Scan) System_Scan();
    });
}

void c_HIPNUC_CH10X::Write_Communicate_DB(QString key, int value) {
    c_Variable::getInstance().g_Communicate_DB.insert(key, value);
    QFile File(m_DB_Path);
    if (File.open(QIODevice::WriteOnly | QIODevice::Text)) {
        File.write(QJsonDocument(c_Variable::getInstance().g_Communicate_DB).toJson());
        File.close();
    }
}

void c_HIPNUC_CH10X::Write_Communicate_DB(QString key, QString value) {
    c_Variable::getInstance().g_Communicate_DB.insert(key, value);
    QFile File(m_DB_Path);
    if (File.open(QIODevice::WriteOnly | QIODevice::Text)) {
        File.write(QJsonDocument(c_Variable::getInstance().g_Communicate_DB).toJson());
        File.close();
    }
}

void c_HIPNUC_CH10X::closeEvent(QCloseEvent* event) {
    m_Scan = false;
    if (m_Remote_Thread && m_Remote_Thread->isRunning()) {
        m_Remote_Thread->requestInterruption();
        m_Remote_Thread->quit();
        m_Remote_Thread->wait();
    }
    c_Variable::getInstance().writeMessage(1, "Stop");
    event->accept();
}

void c_HIPNUC_CH10X::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_F6) {
        QFile File(QDir::currentPath() + "/stuqss.css");
        if (File.open(QIODevice::ReadOnly)) {
            this->setStyleSheet(File.readAll());
            File.close();
        }
    }
    QWidget::keyPressEvent(event);
}

void c_HIPNUC_CH10X::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    if (m_Initialized) return;
    m_Initialized = true;
    QTimer::singleShot(200, this, &c_HIPNUC_CH10X::Init);
    QTimer::singleShot(300, this, &c_HIPNUC_CH10X::System_Scan);
}