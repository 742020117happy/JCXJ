#include "RoboSense.h"

c_RoboSense::c_RoboSense(QStringList info, QWidget* parent) : QWidget(parent)
{
    ui.setupUi(this);

    m_renderWindow->SetUseOffScreenBuffers(false);
    m_renderWindow->SetDoubleBuffer(true); 
    m_renderWindow->SetMultiSamples(0);   
    ui.VTK->renderWindow()->SetUseOffScreenBuffers(false);

    {
        QFile File;
        File.setFileName(QDir::currentPath() + "/stuqss.css");
        File.open(QIODevice::ReadOnly);
        QString strQss = File.readAll();
        this->setStyleSheet(strQss);
        File.close();
    }
    if (!info.isEmpty() && info.size() > 1) {
        c_Variable::getInstance().g_RoboSense.device_name = info.at(1);
        this->setWindowFlags(Qt::Widget);
        this->setAttribute(Qt::WA_NativeWindow);
        this->setAttribute(Qt::WA_DontCreateNativeAncestors, false);
        this->setWindowFlags(Qt::FramelessWindowHint);
        this->setWindowTitle(c_Variable::getInstance().g_RoboSense.device_name);
        QFile File;
        m_DB_Path = QDir::currentPath() + "/" + c_Variable::getInstance().g_RoboSense.device_name + ".json";
        File.setFileName(m_DB_Path);
        File.open(QFile::ReadOnly | QIODevice::Text);
        QByteArray Data = File.readAll();
        QJsonParseError parseError;  
        QJsonDocument DB_Doc = QJsonDocument::fromJson(Data, &parseError); 
        if (parseError.error == QJsonParseError::NoError) {
            c_Variable::getInstance().g_Communicate_DB = DB_Doc.object();
        }
        File.close();
    }
    else {
        QFile File;
        m_DB_Path = QDir::currentPath() + "/RoboSense.json";
        File.setFileName(m_DB_Path);
        File.open(QFile::ReadOnly | QIODevice::Text);
        QByteArray Data = File.readAll();
        QJsonParseError parseError;  
        QJsonDocument DB_Doc = QJsonDocument::fromJson(Data, &parseError);  
        if (parseError.error == QJsonParseError::NoError) {
            c_Variable::getInstance().g_Communicate_DB = DB_Doc.object();
        }
        File.close();

    }

    m_file.open(stdin, QIODevice::ReadOnly | QIODevice::Text);
    m_pNotifier = new QSocketNotifier(m_file.handle(), QSocketNotifier::Read, this);

    QObject::connect(m_pNotifier, &QSocketNotifier::activated, this, [=](int iHandle){
        if(m_file.handle() == iHandle){
            int iRet = read(m_file.handle(), m_arrRecv, sizeof(m_arrRecv));
            m_arrRecv[iRet] = 0;
            c_RoboSense::readMessage(QString("%1").arg(m_arrRecv));
        }
    });

    c_Variable::getInstance().writeMessage(1, QString("Run&%1").arg(this->winId()));

    this->show();
}

c_RoboSense::~c_RoboSense(){}

void c_RoboSense::readMessage(QString message)
{
    c_Variable::getInstance().writeMessage(2, message);
    if (message == "Connect") { emit ui.btnConnect->clicked(); }
    if (message == "Disconnect") { emit ui.btnDisconnect->clicked(); }
    if (message == "Close_Device") { this->close(); }
    if (message.split("&", Qt::SkipEmptyParts).isEmpty()) { return; }
}

void c_RoboSense::VTK_Init()
{
    QOpenGLContext* ctx = QOpenGLContext::currentContext();
    if (!ctx || !ctx->isValid()) {
        c_Variable::getInstance().writeMessage(2, "ERROR: OpenGL context invalid!");
        QTimer::singleShot(100, this, &c_RoboSense::VTK_Init);  // 重试
        return;
    }

    // 禁用自动重渲染（交互时手动控制）
    m_renderWindow->SetAlphaBitPlanes(0);           // 禁用Alpha缓冲节省带宽
    m_renderWindow->SetMultiSamples(0);             // 禁用MSAA抗锯齿（大幅提升性能）
    
    m_renderWindow->AddRenderer(m_renderer);
    ui.VTK->setRenderWindow(m_renderWindow);

    m_viewer.reset(new pcl::visualization::PCLVisualizer(
        m_renderer, m_renderWindow, "viewer", false));
    m_viewer->setupInteractor(ui.VTK->interactor(), ui.VTK->renderWindow());

    m_viewer->setBackgroundColor(0.05f, 0.05f, 0.05f);
    m_viewer->initCameraParameters();
    m_viewer->setCameraPosition(
        -1500,   // X: 相机在雷达后方1.5米
        0.0f,    // Y: 相机居中
        0.0f,    // Z: 相机高度
        0.0f,    // viewX: 焦点X=0
        0.0f,    // viewY: 焦点Y=0
        0.0f,    // viewZ: 焦点Z=0
        0.0f,    // upX: 上方向X=0
        0.0f,    // upY: 上方向Y=0
        1.0f     // upZ: 上方向Z=1（向上）
    );

    m_cloud.reset(new pcl::PointCloud<pcl::PointXYZI>);
    m_Roi.reset(new pcl::PointCloud<pcl::PointXYZI>);

    pcl::visualization::PointCloudColorHandlerGenericField<pcl::PointXYZI>
        color_handler(m_cloud, "intensity");
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZI>
        roi_color_handler(m_Roi, 255, 0, 0);

    m_viewer->addPointCloud<pcl::PointXYZI>(m_cloud, color_handler, "cloud");
    m_viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "cloud");

    m_viewer->addPointCloud<pcl::PointXYZI>(m_Roi, roi_color_handler, "Roi");
    m_viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "Roi");

    m_viewer->spinOnce(1, true);  // PCL推荐方式（触发VTK事件循环）
    m_renderWindow->Render();     // 直接渲染
    ui.VTK->update();             // 刷新Qt窗口

    m_vtkInitialized = true;
    QTimer::singleShot(100, this, &c_RoboSense::RoboSense_Init);
    QTimer::singleShot(200, this, &c_RoboSense::System_Scan);
}
void c_RoboSense::System_Scan()
{
    
    ui.btnConnect->setEnabled(!c_Variable::getInstance().g_RoboSense.DIFOP_connected
        || !c_Variable::getInstance().g_RoboSense.MSOP_connected);

    ui.btnDisconnect->setEnabled(c_Variable::getInstance().g_RoboSense.DIFOP_connected
        && c_Variable::getInstance().g_RoboSense.MSOP_connected);

    QString SW_Version = QString("%1.%2.%3")
        .arg(c_Variable::getInstance().g_DIFOP.data.swVersion.major)
        .arg(c_Variable::getInstance().g_DIFOP.data.swVersion.minor)
        .arg(c_Variable::getInstance().g_DIFOP.data.swVersion.patch);
    ui.SW_Version->setText(SW_Version.isEmpty() ? "N/A" : SW_Version);


    QString sn = QString("%1-%2-%3-%4-%5-%6")
        .arg(c_Variable::getInstance().g_DIFOP.data.sn[0], 2, 16, QChar('0')).toUpper()
        .arg(c_Variable::getInstance().g_DIFOP.data.sn[1], 2, 16, QChar('0')).toUpper()
        .arg(c_Variable::getInstance().g_DIFOP.data.sn[2], 2, 16, QChar('0')).toUpper()
        .arg(c_Variable::getInstance().g_DIFOP.data.sn[3], 2, 16, QChar('0')).toUpper()
        .arg(c_Variable::getInstance().g_DIFOP.data.sn[4], 2, 16, QChar('0')).toUpper()
        .arg(c_Variable::getInstance().g_DIFOP.data.sn[5], 2, 16, QChar('0')).toUpper();
    ui.SN->setText(sn.isEmpty() ? "N/A" : sn);

    QString mac = QString("%1-%2-%3-%4-%5-%6")
        .arg(c_Variable::getInstance().g_DIFOP.data.macAddress.bytes[0], 2, 16, QChar('0')).toUpper()
        .arg(c_Variable::getInstance().g_DIFOP.data.macAddress.bytes[1], 2, 16, QChar('0')).toUpper()
        .arg(c_Variable::getInstance().g_DIFOP.data.macAddress.bytes[2], 2, 16, QChar('0')).toUpper()
        .arg(c_Variable::getInstance().g_DIFOP.data.macAddress.bytes[3], 2, 16, QChar('0')).toUpper()
        .arg(c_Variable::getInstance().g_DIFOP.data.macAddress.bytes[4], 2, 16, QChar('0')).toUpper()
        .arg(c_Variable::getInstance().g_DIFOP.data.macAddress.bytes[5], 2, 16, QChar('0')).toUpper();
    ui.MAC_Address->setText(mac.isEmpty() ? "N/A" : mac);

    quint8 freq = c_Variable::getInstance().g_DIFOP.data.frequencySetting;
    ui.FrequecySetting->setText(freq == 0x00 ? "10 Hz" : QString("%1 Hz").arg(freq, 2, 16, QChar('0')).toUpper());

    QString returnMode;
    switch (c_Variable::getInstance().g_DIFOP.data.returnMode) {
    case 0x00: returnMode = "FarthestWave"; break;
    case 0x04: returnMode = "StrongestWave (Default)"; break;
    case 0x07: returnMode = "NearestWave"; break;
    case 0x08: returnMode = "2ndStrongestWave"; break;
    case 0x09: returnMode = "StrongestFarthestWave"; break;
    case 0x0A: returnMode = "NearestFarthestWave"; break;
    case 0x0B: returnMode = "Strongest2ndStrongestWave"; break;
    default: returnMode = QString("Unknown (0x%1)").arg(c_Variable::getInstance().g_DIFOP.data.returnMode, 2, 16, QChar('0'));
    }
    ui.ReturnMode->setText(returnMode);

    QString timeSync;
    switch (c_Variable::getInstance().g_DIFOP.data.timeSyncStatus) {
    case 0x00:
        timeSync = "Failed";
        break;
    case 0x01:
        timeSync = "Success";
        break;
    case 0x02:
        timeSync = "Timeout";
        break;
    default:
        timeSync = "Unknown";
    }
    ui.TimeSyncStatus->setText(timeSync);

    QString phyMode;
    switch (c_Variable::getInstance().g_DIFOP.data.phyMode) {
    case 0x00: phyMode = "Auto-negotiation"; break;
    case 0x01: phyMode = "Master (1000BASE-T1)"; break;
    case 0x02: phyMode = "Slave (1000BASE-T1)"; break;
    default: phyMode = "Auto";
    }
    ui.PHYMode->setText(phyMode);

    ui.LidarTemp->setText(QString("%1 °C").arg(c_Variable::getInstance().g_RoboSense.lidarTmp));

    quint64 seconds = 0;
    for (int i = 0; i < 6; ++i) {
        seconds = (seconds << 8) | c_Variable::getInstance().g_DIFOP.data.timeStatus.seconds[i];
    }
    quint32 microseconds = 0;
    for (int i = 0; i < 4; ++i) {
        microseconds = (microseconds << 8) | c_Variable::getInstance().g_DIFOP.data.timeStatus.microseconds[i];
    }
    QDateTime timestamp = QDateTime::fromSecsSinceEpoch(seconds);
    ui.LastUpdate->setText(timestamp.toString("yyyy-MM-dd hh:mm:ss") +
        QString(".%1").arg(microseconds / 1000, 3, 10, QChar('0')));

    ui.device_Status->Set_State(c_Variable::getInstance().g_RoboSense.device_Status);

    ui.DIFOP_Status->Set_State(c_Variable::getInstance().g_RoboSense.DIFOP_connected);
    ui.MSOP_Status->Set_State(c_Variable::getInstance().g_RoboSense.MSOP_connected);

    ui.roi_distance->setText(QString::number(m_roi_distance));
    ui.roi_num->setText(QString::number(m_roi_point_count));

    if(m_roi_distance < m_Safe_Distance && m_roi_distance > m_Safe_Num){
        ui.is_Obstacle->Set_Working();
    }else{
        ui.is_Obstacle->Set_Default();
    }
    

    QTimer::singleShot(100, this, [this]() {
        if (m_Scan) System_Scan();
        });
}
void c_RoboSense::RoboSense_Init()
{
    c_Variable::getInstance().writeMessage(2, "开始初始化雷达");

	m_E1R_MSOP = new c_E1R_MSOP;
    m_E1R_MSOP_thread = new QThread;

    m_E1R_DIFOP = new c_E1R_DIFOP;
    m_E1R_DIFOP_thread = new QThread;


	QObject::connect(m_E1R_MSOP_thread, &QThread::started, m_E1R_MSOP, &c_E1R_MSOP::Init);
    QObject::connect(m_E1R_MSOP_thread, &QThread::finished, m_E1R_MSOP, &c_E1R_MSOP::deleteLater);

    QObject::connect(m_E1R_DIFOP_thread, &QThread::started, m_E1R_DIFOP, &c_E1R_DIFOP::Init);
    QObject::connect(m_E1R_DIFOP_thread, &QThread::finished, m_E1R_DIFOP, &c_E1R_DIFOP::deleteLater);


    QObject::connect(m_E1R_MSOP, &c_E1R_MSOP::Status, this, [=](QString cmd) {
        c_Variable::getInstance().writeMessage(2, cmd);
    });

    QObject::connect(m_E1R_DIFOP, &c_E1R_DIFOP::Status, this, [=](QString cmd) {
        c_Variable::getInstance().writeMessage(2, cmd);
	});

    QObject::connect(ui.btnConnect, &QPushButton::clicked, m_E1R_MSOP, &c_E1R_MSOP::Connect);
    QObject::connect(ui.btnDisconnect, &QPushButton::clicked, m_E1R_MSOP, &c_E1R_MSOP::Disconnect);

    QObject::connect(ui.btnConnect, &QPushButton::clicked, m_E1R_DIFOP, &c_E1R_DIFOP::Connect);
    QObject::connect(ui.btnDisconnect, &QPushButton::clicked, m_E1R_DIFOP, &c_E1R_DIFOP::Disconnect);

    QObject::connect(m_E1R_MSOP, &c_E1R_MSOP::MSOP_Scan, this, &c_RoboSense::MSOP_Scan);
   
    RoboSense_DB();

    m_E1R_DIFOP_thread->start();
    m_E1R_MSOP_thread->start();
}
void c_RoboSense::RoboSense_DB()
{
    QObject::connect(ui.Local_Ip, &QLineEdit::textChanged, this, [=](QString value) {
        c_Variable::getInstance().g_RoboSense.Local_Ip = value;
        Write_Communicate_DB("Local_Ip", value);
        });
    QObject::connect(ui.Local_MSOP_Port, &QLineEdit::textChanged, this, [=](QString value) {
        c_Variable::getInstance().g_RoboSense.Local_MSOP_Port = value.toInt();
        Write_Communicate_DB("Local_MSOP_Port", value.toInt());
        });
    QObject::connect(ui.Local_DIFOP_Port, &QLineEdit::textChanged, this, [=](QString value) {
        c_Variable::getInstance().g_RoboSense.Local_DIFOP_Port = value.toInt();
        Write_Communicate_DB("Local_DIFOP_Port", value.toInt());
        });
    QObject::connect(ui.E1R_IP, &QLineEdit::textChanged, this, [=](QString value) {
        c_Variable::getInstance().g_RoboSense.E1R_IP = value;
        Write_Communicate_DB("E1R_IP", value);
        });
    QObject::connect(ui.E1R_MSOP_Port, &QLineEdit::textChanged, this, [=](QString value) {
        c_Variable::getInstance().g_RoboSense.E1R_MSOP_Port = value.toInt();
        Write_Communicate_DB("E1R_MSOP_Port", value.toInt());
        });
    QObject::connect(ui.E1R_DIFOP_Port, &QLineEdit::textChanged, this, [=](QString value) {
        c_Variable::getInstance().g_RoboSense.E1R_DIFOP_Port = value.toInt();
        Write_Communicate_DB("E1R_DIFOP_Port", value.toInt());
        });

    QObject::connect(ui.Safe_Distance, &QLineEdit::textChanged, this, [=](QString value) {
        m_Safe_Distance = value.toInt();
        Write_Communicate_DB("Safe_Distance", m_Safe_Distance);
        });
    QObject::connect(ui.Safe_Num, &QLineEdit::textChanged, this, [=](QString value) {
        m_Safe_Num = value.toInt();
        Write_Communicate_DB("Safe_Num", m_Safe_Num);
        });
    QObject::connect(ui.max_point_1, &QLineEdit::textChanged, this, [=](QString data) {
        m_max_point_1 = data.toDouble();
        Write_Communicate_DB("max_point_1", m_max_point_1);
        });
    QObject::connect(ui.max_point_2, &QLineEdit::textChanged, this, [=](QString data) {
        m_max_point_2 = data.toDouble();
        Write_Communicate_DB("max_point_2", m_max_point_2);
        });
    QObject::connect(ui.max_point_3, &QLineEdit::textChanged, this, [=](QString data) {
        m_max_point_3 = data.toDouble();
        Write_Communicate_DB("max_point_3", m_max_point_3);
        });
    QObject::connect(ui.min_point_1, &QLineEdit::textChanged, this, [=](QString data) {
        m_min_point_1 = data.toDouble();
        Write_Communicate_DB("min_point_1", m_min_point_1);
        });
    QObject::connect(ui.min_point_2, &QLineEdit::textChanged, this, [=](QString data) {
        m_min_point_2 = data.toDouble();
        Write_Communicate_DB("min_point_2", m_min_point_2);
        });
    QObject::connect(ui.min_point_3, &QLineEdit::textChanged, this, [=](QString data) {
        m_min_point_3 = data.toDouble();
        Write_Communicate_DB("min_point_3", m_min_point_3);
        });

    ui.Local_Ip->setText(c_Variable::getInstance().g_Communicate_DB.value("Local_Ip").toString());
    ui.Local_MSOP_Port->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("Local_MSOP_Port").toInt()));
    ui.Local_DIFOP_Port->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("Local_DIFOP_Port").toInt()));
    ui.E1R_IP->setText(c_Variable::getInstance().g_Communicate_DB.value("E1R_IP").toString());
    ui.E1R_MSOP_Port->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("E1R_MSOP_Port").toInt()));
    ui.E1R_DIFOP_Port->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("E1R_DIFOP_Port").toInt()));

    ui.Safe_Distance->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("Safe_Distance").toInt()));
    ui.Safe_Num->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("Safe_Num").toInt()));

    ui.max_point_1->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("max_point_1").toDouble()));
    ui.max_point_2->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("max_point_2").toDouble()));
    ui.max_point_3->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("max_point_3").toDouble()));

    ui.min_point_1->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("min_point_1").toDouble()));
    ui.min_point_2->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("min_point_2").toDouble()));
    ui.min_point_3->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("min_point_3").toDouble()));
} 
void c_RoboSense::RoboSense_Delete()
{
    if (m_E1R_DIFOP_thread->isRunning()) {
        m_E1R_DIFOP_thread->requestInterruption();
        m_E1R_DIFOP_thread->quit();
        m_E1R_DIFOP_thread->wait();
    }
    if (m_E1R_MSOP_thread->isRunning()) {
        m_E1R_MSOP_thread->requestInterruption();
        m_E1R_MSOP_thread->quit();
        m_E1R_MSOP_thread->wait();
    }
}
void c_RoboSense::MSOP_Scan(QVector<pcl::PointXYZI> points)
{
    if (points.isEmpty()) return;
    m_cloud->clear();
    m_cloud->resize(points.size());
    m_cloud->width = points.size();
    m_cloud->height = 1;
    m_cloud->is_dense = false;

    std::memcpy(m_cloud->points.data(), points.constData(),
        points.size() * sizeof(pcl::PointXYZI));

    Eigen::Vector4f min_point(
        m_min_point_1,
        m_min_point_2,
        m_min_point_3,
        1);

    Eigen::Vector4f max_point(
        m_max_point_1,
        m_max_point_2,
        m_max_point_3,
        1);

    m_Roi->points.clear();
    pcl::CropBox<pcl::PointXYZI> crop;//创建裁剪框对象
    crop.setInputCloud(m_cloud);//设置输入点云
    crop.setMin(min_point);//设置裁剪框的最小点
    crop.setMax(max_point);//设置裁剪框的最大点
    crop.filter(*m_Roi);//执行裁剪过滤

    float roi_distance = 0;
    int roi_point_count = 0;
    float distance = 0;

    // 计算ROI内障碍物的距离
    for (const pcl::PointXYZI& point : m_Roi->points)
    {
        // 计算点到原点的距离
        distance = qAbs(point.x);
        if (distance > 100.0 && distance < 10000.0) {
            // 更新最小距离
            roi_distance = roi_distance + distance;
            roi_point_count++;
        }
    }
    if (roi_point_count > m_Safe_Num) {
        roi_distance = roi_distance / roi_point_count;
    }
    if (roi_point_count <= m_Safe_Num) {
        roi_distance = 30000.0;
    }
    m_roi_distance = roi_distance;
    m_roi_point_count = roi_point_count;

    pcl::visualization::PointCloudColorHandlerGenericField<pcl::PointXYZI>
        color_handler(m_cloud, "intensity");
    m_viewer->updatePointCloud<pcl::PointXYZI>(m_cloud, color_handler, "cloud");

    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZI>
        roi_color_handler(m_Roi, 255, 0, 0);
    m_viewer->updatePointCloud<pcl::PointXYZI>(m_Roi, roi_color_handler, "Roi");

    if (m_first_Scan) {
        m_viewer->setCameraPosition(
            -1500,   // X: 相机在雷达后方1.5米
            0.0f,    // Y: 相机居中
            0.0f,    // Z: 相机高度
            0.0f,    // viewX: 焦点X=0
            0.0f,    // viewY: 焦点Y=0
            0.0f,    // viewZ: 焦点Z=0
            0.0f,    // upX: 上方向X=0
            0.0f,    // upY: 上方向Y=0
            1.0f     // upZ: 上方向Z=1（向上）
        );
        m_first_Scan = false;
    }

    m_viewer->spinOnce(1, true);  // PCL推荐方式（触发VTK事件循环）
    m_renderWindow->Render();     // 直接渲染
    ui.VTK->update();             // 刷新Qt窗口
}
void c_RoboSense::Write_Communicate_DB(QString key, int value)
{
    c_Variable::getInstance().g_Communicate_DB.insert(key, value);

    QFile File;
    File.setFileName(m_DB_Path);
    
	if (!File.open(QIODevice::ReadWrite | QIODevice::Text)) {
    	return;
	}
	
    QJsonDocument DB_Doc;
    DB_Doc.setObject(c_Variable::getInstance().g_Communicate_DB);

    File.write(DB_Doc.toJson());
    File.close();
}
void c_RoboSense::Write_Communicate_DB(QString key, QString value)
{
    c_Variable::getInstance().g_Communicate_DB.insert(key, value);

    QFile File;
    File.setFileName(m_DB_Path);
    
	if (!File.open(QIODevice::ReadWrite | QIODevice::Text)) {
    	return;
	}
	
    QJsonDocument DB_Doc;
    DB_Doc.setObject(c_Variable::getInstance().g_Communicate_DB);

    File.write(DB_Doc.toJson());
    File.close();
}
void c_RoboSense::Write_Communicate_DB(QString key, double value)
{
    c_Variable::getInstance().g_Communicate_DB.insert(key, value);

    QFile File;
    File.setFileName(m_DB_Path);

	if (!File.open(QIODevice::ReadWrite | QIODevice::Text)) {
    	return;
	}
	
    QJsonDocument DB_Doc;
    DB_Doc.setObject(c_Variable::getInstance().g_Communicate_DB);

    File.write(DB_Doc.toJson());
    File.close();
}
void c_RoboSense::closeEvent(QCloseEvent* event) {
    // 释放资源
    m_Scan = false;
    RoboSense_Delete();
    c_Variable::getInstance().writeMessage(1, "Stop");
    // 允许关闭
    event->accept();
}
void c_RoboSense::keyPressEvent(QKeyEvent* event)
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

    QWidget::keyPressEvent(event);
}
void c_RoboSense::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    if (m_vtkInitialized) return;
    QTimer::singleShot(100, this, &c_RoboSense::VTK_Init);
}
