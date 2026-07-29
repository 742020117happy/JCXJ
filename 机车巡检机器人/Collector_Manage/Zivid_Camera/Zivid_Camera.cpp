#include "Zivid_Camera.h"
#include "ui_Zivid_Camera.h"

 // 构造函数
c_Zivid_Camera::c_Zivid_Camera(QStringList info, QWidget* parent) : QWidget(parent)
{
    ui = new Ui::c_Zivid_Camera();
	ui->setupUi(this);

    m_renderWindow->SetUseOffScreenBuffers(false);
    ui->VTK->renderWindow()->SetUseOffScreenBuffers(false);

    {
    	QFile File;
		File.setFileName(QDir::currentPath() + "/stuqss.css");
		File.open(QIODevice::ReadOnly);

		QString strQss = File.readAll();
		this->setStyleSheet(strQss);

		File.close();
    }

    if (!info.isEmpty() && info.size() > 1) {
        c_Variable::getInstance().g_Zivid_Camera.device_name = info.at(1);

        this->setWindowFlags(Qt::Widget);
        this->setAttribute(Qt::WA_NativeWindow);  
        this->setAttribute(Qt::WA_DontCreateNativeAncestors, false);

        this->setWindowFlags(Qt::FramelessWindowHint);
		this->setWindowTitle(c_Variable::getInstance().g_Zivid_Camera.device_name);
        //打开文件
        QFile File;
        m_DB_Path = QDir::currentPath() + "/" + c_Variable::getInstance().g_Zivid_Camera.device_name + ".json";
        File.setFileName(m_DB_Path);
        File.open(QFile::ReadOnly | QIODevice::Text);
        //读取文件
        QByteArray Data = File.readAll();
        //转换JSON
        QJsonParseError parseError;  // JSON解析错误
        QJsonDocument DB_Doc = QJsonDocument::fromJson(Data, &parseError);  // 解析JSON
        if (parseError.error == QJsonParseError::NoError) {
            c_Variable::getInstance().g_Communicate_DB = DB_Doc.object();
        }
        File.close();
    }
	else{
        //打开文件
        QFile File;
        m_DB_Path = QDir::currentPath() + "/Zivid_Camera.json";
        File.setFileName(m_DB_Path);
        File.open(QFile::ReadOnly | QIODevice::Text);
        //读取文件
        QByteArray Data = File.readAll();
        //转换JSON
        QJsonParseError parseError;  // JSON解析错误
        QJsonDocument DB_Doc = QJsonDocument::fromJson(Data, &parseError);  // 解析JSON
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
            c_Zivid_Camera::readMessage(QString("%1").arg(m_arrRecv));
        }
    });

    QObject::connect(ui->get_Path, &QPushButton::clicked, this, [=]() {
		m_Path_Name = QFileDialog::getExistingDirectory( // 弹出目录选择对话框
			this, "选择文件目录");
		if (!m_Path_Name.isEmpty()) { // 如果选中有效目录
			ui->Read_Path_Name->setText(m_Path_Name);
		}
	});

	QObject::connect(ui->get_File, &QPushButton::clicked, this, [=]() {
		if (!m_Path_Name.isEmpty()) { // 如果选中有效目录
			c_Variable::getInstance().g_Transmission.buffer.clear();
			QDirIterator it(m_Path_Name, // 初始化目录迭代器
				QDir::Files, // 只遍历文件（不包括子目录）
				QDirIterator::Subdirectories); // 包括子目录
			while (it.hasNext()) { // 遍历所有文件
				it.next(); // 获取下一个文件
				QFileInfo fileInfo = it.fileInfo(); // 获取文件信息
				if (fileInfo.size() > 200 * 1024 * 1024) // 过滤超过200MB的文件
					continue;
				QString suffix = fileInfo.suffix().toLower(); // 获取文件扩展名（小写）
				if (suffix == "jpg" || suffix == "png" || suffix == "xyz" || suffix == "data") { // 过滤指定扩展名
					c_Variable::getInstance().g_Transmission.buffer.append(fileInfo.absoluteFilePath()); // 添加到发送队列
					QString Read_File_Name_Num = QString::number(c_Variable::getInstance().g_Transmission.buffer.size());
					ui->Read_File_Name_Num->setText(Read_File_Name_Num);
				}
			}
		}
	});

	QObject::connect(ui->File_Nane_Buffer, &QPushButton::clicked, this, [=]() {
		if (c_Variable::getInstance().g_Transmission.buffer.size() > 0) {
			c_Variable::getInstance().g_Transmission.Tran_buffer += c_Variable::getInstance().g_Transmission.buffer;
			QString  Tran_File_Name_Num = QString::number(c_Variable::getInstance().g_Transmission.Tran_buffer.size());
			ui->Write_File_Name_Num->setText(Tran_File_Name_Num);
		}
	});

	QObject::connect(ui->Clear_All, &QPushButton::clicked, this, [=]() {
		ui->Read_Path_Name->clear();
		ui->Read_File_Name_Num->clear();
		ui->Write_File_Name_Num->clear();
		m_Path_Name.clear();
		c_Variable::getInstance().g_Transmission.buffer.clear();
	});

    c_Variable::getInstance().writeMessage(1, QString("Run&%1").arg(this->winId()));

    m_Scan = true;

    this->show();
}
// 析构函数
c_Zivid_Camera::~c_Zivid_Camera()
{
    delete ui;
}
void c_Zivid_Camera::readMessage(QString message)
{
    c_Variable::getInstance().writeMessage(2, message);
	if (message == "Connect") {emit Connect();}
    if (message == "Disconnect") {emit Disconnect();}
    if (message == "Close_Device") {this->close();}
    if (message.split("&", Qt::SkipEmptyParts).isEmpty()) {return;}
    
    if (message.split("&", Qt::SkipEmptyParts).at(0) == "save_path"){
        ui->Save_Path->setText(message.split("&", Qt::SkipEmptyParts).at(1));
        QString basePath =  c_Variable::getInstance().g_Zivid_Camera.base_Path;
	    QString savePath = message.split("&", Qt::SkipEmptyParts).at(1);
        QString imagePath = basePath + savePath;

        c_Variable::getInstance().writeMessage(2, QString("相机接收转发消息: %1").arg(imagePath));

	    if (!QDir().mkpath(imagePath)) {
		    c_Variable::getInstance().writeMessage(2, QString("无法创建目录: %1").arg(imagePath));
		    return;
	    }
         c_Variable::getInstance().writeMessage(1, "updateCompleted&");
    }
    if (message.split("&", Qt::SkipEmptyParts).at(0) == "image_name"){
        c_Variable::getInstance().writeMessage(2, QString("相机接收转发消息: %1").arg(message));
        ui->Save_Name->setText(message.split("&", Qt::SkipEmptyParts).at(1));
        c_Variable::getInstance().g_Zivid_Camera.image_name = message.split("&", Qt::SkipEmptyParts).at(1);
        ui->Capture->click();
    }
}
//系统扫描
void c_Zivid_Camera::System_Scan()
{
    Camera_Scan();
    TCP_Client_Scan();

	QTimer::singleShot(100, this, [this]() {
        if (m_Scan) System_Scan();
        });
}
void c_Zivid_Camera::VTK_Init()
{
    QOpenGLContext* ctx = QOpenGLContext::currentContext();
    if (!ctx || !ctx->isValid()) {
        c_Variable::getInstance().writeMessage(2, "ERROR: OpenGL context invalid!");
        QTimer::singleShot(100, this, &c_Zivid_Camera::VTK_Init);  // 重试
        return;
    }

    // 禁用自动重渲染（交互时手动控制）
    m_renderWindow->SetAlphaBitPlanes(0);           // 禁用Alpha缓冲节省带宽
    m_renderWindow->SetMultiSamples(0);             // 禁用MSAA抗锯齿（大幅提升性能）

    m_renderWindow->AddRenderer(m_renderer);
    ui->VTK->setRenderWindow(m_renderWindow);

    m_viewer.reset(new pcl::visualization::PCLVisualizer(
        m_renderer, m_renderWindow, "viewer", false));
    m_viewer->setupInteractor(ui->VTK->interactor(), ui->VTK->renderWindow());

    m_viewer->setBackgroundColor(0.0f, 0.0f, 0.0f);
    m_viewer->initCameraParameters();
    m_viewer->setCameraPosition(
        0.0f,   // X: 相机后方
        0.0f,    // Y: 相机居中
        -1000.0f,    // Z: 相机高度
        0.0f,    // viewX: 焦点X=0
        0.0f,    // viewY: 焦点Y=0
        0.0f,    // viewZ: 焦点Z=0
        0.0f,    // upX: 上方向X=0
        0.0f,    // upY: 上方向Y=0
        -1.0f    // upZ: 上方向Z=1
    );

    m_cloud.reset(new pcl::PointCloud<pcl::PointXYZRGB>);

    m_viewer->spinOnce(1, true);  // PCL推荐方式（触发VTK事件循环）
    m_renderWindow->Render();     // 直接渲染
    ui->VTK->update();             // 刷新Qt窗口

    m_vtkInitialized = true;
    QTimer::singleShot(100, this, &c_Zivid_Camera::System_Scan);
    QTimer::singleShot(200, this, &c_Zivid_Camera::Camera_Init);
    QTimer::singleShot(300, this, &c_Zivid_Camera::TCP_Client_Init);
}
// 初始化相机
void c_Zivid_Camera::Camera_Init()
{
    c_Variable::getInstance().writeMessage(2, "开始初始化相机"); 

    // 创建相机线程
    m_cameraThread = new QThread;
    m_cameras = new c_Zivid_Camera_Remote;

    m_cameras->moveToThread(m_cameraThread);

    QObject::connect(m_cameraThread, &QThread::started, m_cameras, &c_Zivid_Camera_Remote::Init);
    QObject::connect(m_cameraThread, &QThread::finished, m_cameras, &c_Zivid_Camera_Remote::deleteLater);

    // 连接相机状态信号
    QObject::connect(m_cameras, &c_Zivid_Camera_Remote::Status, this, [=](QString cmd) {
        c_Variable::getInstance().writeMessage(2, cmd);
        });

    QObject::connect(ui->Zivid_Connect, &QPushButton::clicked, m_cameras, &c_Zivid_Camera_Remote::Connect);
    QObject::connect(ui->Zivid_Disconnect, &QPushButton::clicked, m_cameras, &c_Zivid_Camera_Remote::Disconnect);

	QObject::connect(ui->Capture, &QPushButton::clicked, m_cameras, &c_Zivid_Camera_Remote::Capture);

    QObject::connect(m_cameras, &c_Zivid_Camera_Remote::CaptureCompleted, this, &c_Zivid_Camera::onCaptureCompleted);

    m_Scan = true;

    c_Zivid_Camera::Camera_DB();
    c_Zivid_Camera::Camera_Scan();

    // 启动线程
    m_cameraThread->start();
}
// 相机参数
void c_Zivid_Camera::Camera_DB()
{
    QObject::connect(ui->basePath, &QLineEdit::textChanged, this, [=](QString value) {
        c_Variable::getInstance().g_Zivid_Camera.base_Path = value;
        Write_Communicate_DB("basePath", value);
        });
    QObject::connect(ui->Save_Path, &QLineEdit::textChanged, this, [=](QString value) {
        c_Variable::getInstance().g_Zivid_Camera.save_path = value;
        Write_Communicate_DB("Save_Path", value);
        });
    QObject::connect(ui->Save_Name, &QLineEdit::textChanged, this, [=](QString value) {
        c_Variable::getInstance().g_Zivid_Camera.image_name = value;
        Write_Communicate_DB("Save_Name", value);
        });

    QObject::connect(ui->Camera_Serial, &QLineEdit::textChanged, this, [=](QString Serial) {
        Write_Communicate_DB("Camera_Serial", Serial);
        c_Variable::getInstance().g_Zivid_Camera.camera_serial = Serial;
        });
    QObject::connect(ui->Camera_Ip, &QLineEdit::textChanged, this, [=](QString Ip) {
        Write_Communicate_DB("Camera_Ip", Ip);
        c_Variable::getInstance().g_Zivid_Camera.Camera_Ip = Ip;
        });

        
    ui->Camera_Serial->setText(c_Variable::getInstance().g_Communicate_DB.value("Camera_Serial").toString());      
    ui->Camera_Ip->setText(c_Variable::getInstance().g_Communicate_DB.value("Camera_Ip").toString());
	ui->Save_Path->setText(c_Variable::getInstance().g_Communicate_DB.value("Save_Path").toString());
	ui->Save_Name->setText(c_Variable::getInstance().g_Communicate_DB.value("Save_Name").toString());
    ui->basePath->setText(c_Variable::getInstance().g_Communicate_DB.value("basePath").toString());
}
// 扫描函数
void c_Zivid_Camera::Camera_Scan()
{
    ui->Zivid_Connect->setEnabled(!c_Variable::getInstance().g_Zivid_Camera.camera_connected);
    ui->Zivid_Disconnect->setEnabled(c_Variable::getInstance().g_Zivid_Camera.camera_connected);
    ui->Capture->setEnabled(c_Variable::getInstance().g_Zivid_Camera.camera_connected);
    ui->settingTime->setText(QString::number(c_Variable::getInstance().g_Zivid_Camera.settingTime));
    ui->captureTime->setText(QString::number(c_Variable::getInstance().g_Zivid_Camera.captureTime));
    ui->save2DTime->setText(QString::number(c_Variable::getInstance().g_Zivid_Camera.save2DTime));
    ui->save3DTime->setText(QString::number(c_Variable::getInstance().g_Zivid_Camera.save3DTime));
    ui->saveDepthMapTime->setText(QString::number(c_Variable::getInstance().g_Zivid_Camera.saveDepthMapTime));
	ui->last_operation_time->setText(c_Variable::getInstance().g_Zivid_Camera.last_operation_time.toString("yyyy-MM-dd-hh-mm-ss"));
}
// 释放资源
void c_Zivid_Camera::Camera_Delete()
{
    if (m_cameraThread->isRunning()) {
        m_cameraThread->requestInterruption();
        m_cameraThread->quit();
        m_cameraThread->wait();
    }
}

void c_Zivid_Camera::TCP_Client_Init()
{
	m_Client_Thread = new QThread();
	m_Client_Remote = new c_TCP_Client();
	m_Client_Remote->moveToThread(m_Client_Thread);

	QObject::connect(m_Client_Thread, &QThread::started, m_Client_Remote, &c_TCP_Client::Init);
	QObject::connect(m_Client_Thread, &QThread::finished, m_Client_Remote, &c_TCP_Client::deleteLater);
	
	QObject::connect(this, &c_Zivid_Camera::Connect, m_Client_Remote, &c_TCP_Client::Connect);
	QObject::connect(this, &c_Zivid_Camera::Disconnect, m_Client_Remote, &c_TCP_Client::Disconnect);

	QObject::connect(ui->Transmission_Connect, &QPushButton::clicked, m_Client_Remote, &c_TCP_Client::Connect);
	QObject::connect(ui->Transmission_Disconnect, &QPushButton::clicked, m_Client_Remote, &c_TCP_Client::Disconnect);

	QObject::connect(this, &c_Zivid_Camera::Send_File_Info, m_Client_Remote, &c_TCP_Client::Send_File_Info);

	QObject::connect(m_Client_Remote, &c_TCP_Client::Send_File_Error, this, [=](QString filePath) {
		c_Variable::getInstance().g_Transmission.Retran_buffer.append(filePath);
	});

	QObject::connect(m_Client_Remote, &c_TCP_Client::Connect_Done, this, [=]() {
		c_Variable::getInstance().g_Transmission.Connected = true;
		c_Variable::getInstance().g_Transmission.Start = false;
		c_Variable::getInstance().g_Transmission.Finish = true;
		c_Variable::getInstance().writeMessage(1, "Connect_Done");
	});

	QObject::connect( m_Client_Remote, &c_TCP_Client::Disconnect_Done, this, [=]() {
		c_Variable::getInstance().g_Transmission.Connected = false;
		c_Variable::getInstance().g_Transmission.Start = true;
		c_Variable::getInstance().g_Transmission.Finish = false;
		c_Variable::getInstance().writeMessage(1, "Disconnect_Done");
	});

	QObject::connect(m_Client_Remote, &c_TCP_Client::Write_Byte_Done, this, [=]() {
		c_Variable::getInstance().g_Transmission.Tran = !c_Variable::getInstance().g_Transmission.Tran;
	});

	QObject::connect(m_Client_Remote, &c_TCP_Client::Status, this, [=](QString value) {
		c_Variable::getInstance().writeMessage(2, value);
	});
	m_Scan = true;

	c_Zivid_Camera::TCP_Client_DB();
	c_Zivid_Camera::TCP_Client_Scan();

	m_Client_Thread->start();
}
void c_Zivid_Camera::TCP_Client_DB()
{
	ui->Server_Ip->setText(c_Variable::getInstance().g_Communicate_DB.value("Server_Ip").toString());
	ui->Server_Port->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("Server_Port").toInt()));
	ui->Client_Ip->setText(c_Variable::getInstance().g_Communicate_DB.value("Client_Ip").toString());
	ui->Client_Port->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("Client_Port").toInt()));

	QObject::connect(ui->Server_Ip, &QLineEdit::textChanged, this, [=](QString ip) {Write_Communicate_DB("Server_Ip", ip); });
	QObject::connect(ui->Server_Port, &QLineEdit::textChanged, this, [=](QString port) {Write_Communicate_DB("Server_Port", port.toInt()); });
	QObject::connect(ui->Client_Ip, &QLineEdit::textChanged, this, [=](QString ip) {Write_Communicate_DB("Client_Ip", ip); });
	QObject::connect(ui->Client_Port, &QLineEdit::textChanged, this, [=](QString port) {Write_Communicate_DB("Client_Port", port.toInt()); });
}
void c_Zivid_Camera::TCP_Client_Scan()
{
	ui->Connected->Set_State(c_Variable::getInstance().g_Transmission.Connected);
	ui->Tran->Set_State(c_Variable::getInstance().g_Transmission.Tran);
	ui->Start->Set_State(c_Variable::getInstance().g_Transmission.Start);
	ui->Finish->Set_State(c_Variable::getInstance().g_Transmission.Finish);

	ui->Transmission_Connect->setEnabled(!c_Variable::getInstance().g_Transmission.Connected);
	ui->Transmission_Disconnect->setEnabled(c_Variable::getInstance().g_Transmission.Connected);

	ui->File_Name_Num->setText(QString::number(
		c_Variable::getInstance().g_Transmission.Tran_buffer.size()
		+ c_Variable::getInstance().g_Transmission.Retran_buffer.size()
	));

	ui->Tran_File_Name_Num->setText(QString::number(c_Variable::getInstance().g_Transmission.Tran_buffer.size()));
	ui->Retran_File_Name_Num->setText(QString::number(c_Variable::getInstance().g_Transmission.Retran_buffer.size()));

	if (c_Variable::getInstance().g_Transmission.Connected 
		&& !c_Variable::getInstance().g_Transmission.Start
		&& c_Variable::getInstance().g_Transmission.Finish
		&& !c_Variable::getInstance().g_Transmission.Tran_buffer.isEmpty()) {
		     emit c_Zivid_Camera::Send_File_Info(c_Variable::getInstance().g_Transmission.Tran_buffer.takeAt(0));
	}
	if (c_Variable::getInstance().g_Transmission.Connected
		&& !c_Variable::getInstance().g_Transmission.Start
		&& c_Variable::getInstance().g_Transmission.Finish
		&& c_Variable::getInstance().g_Transmission.Tran_buffer.isEmpty()
		&& !c_Variable::getInstance().g_Transmission.Retran_buffer.isEmpty()) {
		emit c_Zivid_Camera::Send_File_Info(c_Variable::getInstance().g_Transmission.Retran_buffer.takeAt(0));
	}
}
void c_Zivid_Camera::TCP_Client_Delete()
{
	m_Scan = false;
	if (m_Client_Thread->isRunning()) {
		m_Client_Thread->requestInterruption();
		m_Client_Thread->quit();
		m_Client_Thread->wait();
	}
}
// 采集图像可视化
void c_Zivid_Camera::onCaptureCompleted(QString path)
{
    QFileInfo fileInfo(path);
    QString image_path = QDir(fileInfo.path()).filePath(fileInfo.completeBaseName() + ".jpg");
    QString point_cloud_path = QDir(fileInfo.path()).filePath(fileInfo.completeBaseName() + ".xyz");
    QString point_depth_path = QDir(fileInfo.path()).filePath(fileInfo.completeBaseName() + ".png");

    if (!QFile::exists(point_depth_path) )
    {
         c_Variable::getInstance().writeMessage(2, "无法加载 PNG 图像: " + point_depth_path);
    }else{
        c_Variable::getInstance().g_Transmission.Tran_buffer.append(point_depth_path);
    }


    if (!QFile::exists(image_path) || !QFile::exists(point_cloud_path))
    {
        c_Variable::getInstance().writeMessage(2, QString("可视化加载文件不完整"));
    	return;
    }

    // === 1. 读取 JPG 图像并获取尺寸 ===
    cv::Mat bgrImage = cv::imread(image_path.toStdString(), cv::IMREAD_COLOR);
    if (bgrImage.empty()) {
        c_Variable::getInstance().writeMessage(2, "无法加载 JPG 图像: " + image_path);
        return;
    }

    c_Variable::getInstance().g_Transmission.Tran_buffer.append(image_path);

    size_t width = static_cast<size_t>(bgrImage.cols);
    size_t height = static_cast<size_t>(bgrImage.rows);
    size_t pointCount = width * height;

    // === 2. 验证点云文件大小 ===
    QFile file(point_cloud_path);
    if (!file.open(QIODevice::ReadOnly)) {
        c_Variable::getInstance().writeMessage(2, "无法打开点云文件: " + point_cloud_path);
        return;
    }

    constexpr size_t bytesPerPoint = 3 * sizeof(int16_t);
    size_t expectedSize = pointCount * bytesPerPoint;

    if (static_cast<size_t>(file.size()) != expectedSize) {
        c_Variable::getInstance().writeMessage(2,
            QString("点云尺寸不匹配：图像 %1x%2，点云文件大小 %3 字节（期望 %4）")
            .arg(width).arg(height).arg(file.size()).arg(expectedSize));
        file.close();
        return;
    }

    c_Variable::getInstance().g_Transmission.Tran_buffer.append(point_cloud_path);

    c_Variable::getInstance().writeMessage(1, "CaptureCompleted&");

    // === 3. 读取点云数据 ===
    std::vector<int16_t> buffer(pointCount * 3);
    qint64 bytesRead = file.read(reinterpret_cast<char*>(buffer.data()), expectedSize);
    file.close();

    if (static_cast<size_t>(bytesRead) != expectedSize) {
        c_Variable::getInstance().writeMessage(2, "点云文件读取不完整");
        return;
    }

    // === 4. 构建带颜色的点云 ===
    m_cloud->clear(); 
    m_cloud->resize(pointCount);
    m_cloud->width = width;
    m_cloud->height = height;
    m_cloud->is_dense = false;

    const float scale = 0.1f; // 与保存时 *10 对应
    constexpr int16_t INVALID = -32768;

    for (size_t idx = 0; idx < pointCount; ++idx) {
        int16_t x_i = buffer[3 * idx];
        int16_t y_i = buffer[3 * idx + 1];
        int16_t z_i = buffer[3 * idx + 2];

        size_t row = idx / width;
        size_t col = idx % width;

        if (row >= height || col >= width) {
            m_cloud->points[idx].x = m_cloud->points[idx].y = m_cloud->points[idx].z = std::numeric_limits<float>::quiet_NaN();
        }
        else {
            cv::Vec3b pixel = bgrImage.at<cv::Vec3b>(static_cast<int>(row), static_cast<int>(col));

            if (x_i == INVALID || y_i == INVALID || z_i == INVALID) {
                m_cloud->points[idx].x = m_cloud->points[idx].y = m_cloud->points[idx].z = std::numeric_limits<float>::quiet_NaN();
            }
            else {
                m_cloud->points[idx].x = static_cast<float>(x_i) * scale;
                m_cloud->points[idx].y = static_cast<float>(y_i) * scale;
                m_cloud->points[idx].z = static_cast<float>(z_i) * scale;
            }

            m_cloud->points[idx].b = pixel[0]; // B
            m_cloud->points[idx].g = pixel[1]; // G
            m_cloud->points[idx].r = pixel[2]; // R
        }
    }

    m_viewer->updatePointCloud(m_cloud, "cloud");

    m_viewer->removeAllPointClouds();
    m_viewer->addPointCloud(m_cloud, "cloud");
    m_viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "cloud");
    m_viewer->setCameraPosition(0.0f, 0.0f, -1000.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f);

    m_viewer->spinOnce(1, true);  // PCL推荐方式（触发VTK事件循环）
    m_renderWindow->Render();     // 直接渲染
    ui->VTK->update();             // 刷新Qt窗口

    c_Variable::getInstance().writeMessage(2, "可视化加载成功");
}

// 写入日志
void c_Zivid_Camera::Write_Communicate_DB(QString key, int value)
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
void c_Zivid_Camera::Write_Communicate_DB(QString key, QString value)
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
void c_Zivid_Camera::Write_Communicate_DB(QString key, double value)
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
// 窗口关闭事件
void c_Zivid_Camera::closeEvent(QCloseEvent* event) {
    m_Scan = false;
    // 释放资源
    Camera_Delete();
    TCP_Client_Delete();
    c_Variable::getInstance().writeMessage(1, "Stop");
    // 允许关闭
    event->accept();
}
// 键盘事件 - F6加载样式表
void c_Zivid_Camera::keyPressEvent(QKeyEvent* event)
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
void c_Zivid_Camera::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    if (m_vtkInitialized) return;
    QTimer::singleShot(100, this, &c_Zivid_Camera::VTK_Init);
}