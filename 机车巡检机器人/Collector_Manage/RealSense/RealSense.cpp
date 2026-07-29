#include "RealSense.h"

c_RealSense::c_RealSense(QStringList info, QWidget *parent): QWidget(parent)
{
    ui.setupUi(this);
    m_renderWindow->SetUseOffScreenBuffers(false);
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
        c_Variable::getInstance().g_RealSense.device_name = info.at(1);

        this->setWindowFlags(Qt::Widget);
        this->setAttribute(Qt::WA_NativeWindow);
        this->setAttribute(Qt::WA_DontCreateNativeAncestors, false);

        this->setWindowFlags(Qt::FramelessWindowHint);
        this->setWindowTitle(c_Variable::getInstance().g_RealSense.device_name);
        QFile File;
        m_DB_Path = QDir::currentPath() + "/" + c_Variable::getInstance().g_RealSense.device_name + ".json";
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
        m_DB_Path = QDir::currentPath() + "/RealSense.json";
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
            c_RealSense::readMessage(QString("%1").arg(m_arrRecv));
        }
    });
    
    c_Variable::getInstance().writeMessage(1, QString("Run&%1").arg(this->winId()));

    m_Scan = true;

    this->show();
}

c_RealSense::~c_RealSense()
{
    emit Disconnect();
}

void c_RealSense::readMessage(QString message)
{
    c_Variable::getInstance().writeMessage(2, message);
    if (message == "Connect") { emit Connect(); }
    if (message == "Disconnect") { emit Disconnect(); }
    if (message == "Close_Device") { this->close(); }
    if (message.split("&", Qt::SkipEmptyParts).isEmpty()) { return; }
}
//ϵͳɨ��
void c_RealSense::System_Scan()
{
    ui.device_Status->Set_State(c_Variable::getInstance().g_RealSense.device_Status);
    ui.btnConnect->setEnabled(!c_Variable::getInstance().g_RealSense.device_connected);
    ui.btnDisconnect->setEnabled(c_Variable::getInstance().g_RealSense.device_connected);
    ui.Capture->setEnabled(c_Variable::getInstance().g_RealSense.device_connected);
    ui.Connect_Status->Set_State(c_Variable::getInstance().g_RealSense.device_connected);
    QTimer::singleShot(100, this, [this]() {
        if (m_Scan) System_Scan();
        });
}

void c_RealSense::VTK_Init()
{
    QOpenGLContext* ctx = QOpenGLContext::currentContext();
    if (!ctx || !ctx->isValid()) {
        c_Variable::getInstance().writeMessage(2, "ERROR: OpenGL context invalid!");
        QTimer::singleShot(100, this, &c_RealSense::VTK_Init);  // ����
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

    m_viewer->setBackgroundColor(0.0f, 0.0f, 0.0f);
    m_viewer->initCameraParameters();
  
    m_cloud.reset(new pcl::PointCloud<pcl::PointXYZRGB>);
    m_cloud->is_dense = false;

    m_vtkInitialized = true;

    QTimer::singleShot(100, this, &c_RealSense::RealSense_Init);
    QTimer::singleShot(200, this, &c_RealSense::System_Scan);
}
void c_RealSense::RealSense_Init()
{
    c_Variable::getInstance().writeMessage(2, "RealSense_Init");

    m_RealSense = new c_RealSense_Remote;
    m_RealSense_thread = new QThread;

    QObject::connect(m_RealSense_thread, &QThread::started, m_RealSense, &c_RealSense_Remote::Init);
    QObject::connect(m_RealSense_thread, &QThread::finished, m_RealSense, &c_RealSense_Remote::deleteLater);

    QObject::connect(m_RealSense, &c_RealSense_Remote::Status, this, [=](QString cmd) {
        c_Variable::getInstance().writeMessage(2, cmd);
        });

    QObject::connect(ui.btnConnect, &QPushButton::clicked, m_RealSense, &c_RealSense_Remote::Connect);
    QObject::connect(ui.btnDisconnect, &QPushButton::clicked, m_RealSense, &c_RealSense_Remote::Disconnect);
    QObject::connect(ui.Capture, &QPushButton::clicked, m_RealSense, &c_RealSense_Remote::Capture);
    QObject::connect(m_RealSense, &c_RealSense_Remote::Point_Scan, this, &c_RealSense::Point_Scan);

    RealSense_DB();

    m_RealSense_thread->start();

    ui.btnConnect->click();
}
void c_RealSense::RealSense_DB()
{
    QObject::connect(ui.Camera_Serial, &QLineEdit::textChanged, this, [=](QString Serial) {
        Write_Communicate_DB("Camera_Serial", Serial);
        c_Variable::getInstance().g_RealSense.serial = Serial;
        });

    ui.Camera_Serial->setText(c_Variable::getInstance().g_Communicate_DB.value("Camera_Serial").toString());
}
void c_RealSense::RealSense_Delete()
{
    if (m_RealSense_thread->isRunning()) {
        m_RealSense_thread->requestInterruption();
        m_RealSense_thread->quit();
        m_RealSense_thread->wait();
    }
}
void c_RealSense::Point_Scan(QVector<pcl::PointXYZRGB> points)
{
    if (points.isEmpty()) { return; }
    m_cloud->clear();
    m_cloud->resize(points.size());
    m_cloud->width = static_cast<uint32_t>(points.size());
    m_cloud->height = 1;
    m_cloud->is_dense = false;
    std::memcpy(m_cloud->points.data(), points.constData(),
        points.size() * sizeof(pcl::PointXYZRGB));
   
    if (m_first_Scan) {
        m_first_Scan = false;
        m_viewer->addPointCloud(m_cloud, "cloud");
        m_viewer->setPointCloudRenderingProperties(
            pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2.0, "cloud");
        m_viewer->setPointCloudRenderingProperties(
            pcl::visualization::PCL_VISUALIZER_OPACITY, 1.0, "cloud");
        m_viewer->setCameraPosition(0.0f, 0.0f, -2.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f);
    }
    else {
      
        m_viewer->updatePointCloud(m_cloud, "cloud");
    }
    
    m_viewer->spinOnce(1, true);  
    m_renderWindow->Render();     
    ui.VTK->update();           
    c_Variable::getInstance().g_RealSense.device_Status = !c_Variable::getInstance().g_RealSense.device_Status;
    QTimer::singleShot(100, m_RealSense, &c_RealSense_Remote::Capture);
}


void c_RealSense::Write_Communicate_DB(QString key, int value)
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
void c_RealSense::Write_Communicate_DB(QString key, QString value)
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
void c_RealSense::Write_Communicate_DB(QString key, double value)
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

void c_RealSense::closeEvent(QCloseEvent* event) {
    m_Scan = false; 
    c_RealSense::RealSense_Delete();
    c_Variable::getInstance().writeMessage(1, "Stop");
    event->accept();
}
void c_RealSense::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_F6) {
        QFile File;
        File.setFileName(QDir::currentPath() + "/stuqss.css");
        File.open(QIODevice::ReadOnly);

        QString strQss = File.readAll();
        this->setStyleSheet(strQss);

        File.close();
    }

    QWidget::keyPressEvent(event);
}
void c_RealSense::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    if (m_vtkInitialized) return;
    QTimer::singleShot(100, this, &c_RealSense::VTK_Init);
}
