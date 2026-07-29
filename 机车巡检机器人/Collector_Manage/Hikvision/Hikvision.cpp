#include "Hikvision.h"

c_Hikvision::c_Hikvision(QStringList info, QWidget *parent): QWidget(parent)
{
    ui.setupUi(this);
    {
    	QFile File;
		File.setFileName(QDir::currentPath() + "/stuqss.css");
		File.open(QIODevice::ReadOnly);

		QString strQss = File.readAll();
		this->setStyleSheet(strQss);

		File.close();
    }

    if (!info.isEmpty() && info.size() > 1) {
        c_Variable::getInstance().g_Hikvision_DB.device_name = info.at(1);

        this->setWindowFlags(Qt::Widget);
        this->setAttribute(Qt::WA_NativeWindow);  
        this->setAttribute(Qt::WA_DontCreateNativeAncestors, false);

        this->setWindowFlags(Qt::FramelessWindowHint);
		this->setWindowTitle(c_Variable::getInstance().g_Hikvision_DB.device_name);
        //打开文件
        QFile File;
        m_DB_Path = QDir::currentPath() + "/" + c_Variable::getInstance().g_Hikvision_DB.device_name + ".json";
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
        m_DB_Path = QDir::currentPath() + "/Hikvision_Camera.json";
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
            c_Hikvision::readMessage(QString("%1").arg(m_arrRecv));
        }
    });

    c_Variable::getInstance().writeMessage(1, QString("Run&%1").arg(this->winId()));

    m_Scan = true;
    this->show(); 
}

c_Hikvision::~c_Hikvision()
{
    
}

void c_Hikvision::readMessage(QString message)
{
    c_Variable::getInstance().writeMessage(2, message);
    
	if (message == "Connect") {
        ui.Hikvision_Connect->clicked();
    }
    if (message == "Disconnect") {
        ui.Hikvision_Disconnect->clicked();
    }
    if (message == "Close_Device") {this->close();}
    if (message.split("&", Qt::SkipEmptyParts).isEmpty()) {return;}

}

void c_Hikvision::System_Scan()
{
      
    c_Hikvision::Camera_Scan();  

    m_Initialized = true;

    QTimer::singleShot(100, this, [this]() {
        if (m_Scan) System_Scan();
    });
}

void c_Hikvision::Camera_Init()
{
    m_Hikvision_Remote = new c_Hikvision_Remote;
    m_Hikvision_Remote_Thread = new QThread;
    m_Hikvision_Remote->moveToThread(m_Hikvision_Remote_Thread);

    QObject::connect(m_Hikvision_Remote_Thread, &QThread::started, m_Hikvision_Remote, &c_Hikvision_Remote::Connect);
    QObject::connect(m_Hikvision_Remote_Thread, &QThread::finished, m_Hikvision_Remote, &c_Hikvision_Remote::deleteLater);

    QObject::connect(ui.Hikvision_Connect, &QPushButton::clicked, m_Hikvision_Remote, &c_Hikvision_Remote::Connect);
    QObject::connect(ui.Hikvision_Disconnect, &QPushButton::clicked, m_Hikvision_Remote, &c_Hikvision_Remote::Disconnect);

    QObject::connect(m_Hikvision_Remote, &c_Hikvision_Remote::Status, this, [=](QString cmd) {
        c_Variable::getInstance().writeMessage(2, cmd);
    });
    QObject::connect(&c_Variable::getInstance(), &c_Variable::Status, this, [=](QString cmd) {
        c_Variable::getInstance().writeMessage(2, cmd);
    });

    c_Hikvision::Camera_DB();    
    c_Hikvision::Camera_Scan(); 

    m_Hikvision_Remote_Thread->start();

}
void c_Hikvision::Camera_DB()
{
    QObject::connect(ui.Device_Ip, &QLineEdit::textChanged, this, [=](QString ip){
        c_Variable::getInstance().g_Hikvision_DB.Device_Ip = ip;
        Write_Communicate_DB("Device_Ip", ip);
    });
    QObject::connect(ui.Device_port, &QLineEdit::textChanged, this, [=](QString port){
        c_Variable::getInstance().g_Hikvision_DB.Device_port = static_cast<quint16>(port.toInt());
        Write_Communicate_DB("Device_port", static_cast<quint16>(port.toInt()));
    });
    QObject::connect(ui.user_name, &QLineEdit::textChanged, this, [=](QString name){
        c_Variable::getInstance().g_Hikvision_DB.name = name;
        Write_Communicate_DB("user_name", name);

    });
    QObject::connect(ui.user_key, &QLineEdit::textChanged, this, [=](QString key){
        c_Variable::getInstance().g_Hikvision_DB.key = key;
         Write_Communicate_DB("user_key", key);

    });

    ui.Device_Ip->setText(c_Variable::getInstance().g_Communicate_DB.value("Device_Ip").toString());
    ui.Device_port->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("Device_port").toInt()));
    ui.user_name->setText(c_Variable::getInstance().g_Communicate_DB.value("user_name").toString());
    ui.user_key->setText(c_Variable::getInstance().g_Communicate_DB.value("user_key").toString());
}  
void c_Hikvision::Camera_Scan()
{
    ui.Hikvision_Connect->setDisabled(c_Variable::getInstance().g_Hikvision_DB.Connected);
    ui.Hikvision_Disconnect->setEnabled(c_Variable::getInstance().g_Hikvision_DB.Connected);
    ui.Hikvision_lPort->setText(QString::number(c_Variable::getInstance().g_Hikvision_DB.lPort));
    ui.Hikvision_lUserID->setText(QString::number(c_Variable::getInstance().g_Hikvision_DB.lUserID));
    ui.Hikvision_Connected->Set_State(c_Variable::getInstance().g_Hikvision_DB.Connected);
}
void c_Hikvision::Camera_Delete()
{
    if (m_Hikvision_Remote_Thread->isRunning()) {
        m_Hikvision_Remote_Thread->requestInterruption();
        m_Hikvision_Remote_Thread->quit();
        m_Hikvision_Remote_Thread->wait();
    }
}

// 写入日志
void c_Hikvision::Write_Communicate_DB(QString key, int value)
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
void c_Hikvision::Write_Communicate_DB(QString key, QString value)
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
void c_Hikvision::Write_Communicate_DB(QString key, double value)
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
void c_Hikvision::closeEvent(QCloseEvent* event) {
    m_Scan = false;
    m_Initialized = false;
    // 释放资源
    Camera_Delete();
    c_Variable::getInstance().writeMessage(1, "Stop");
    // 允许关闭
    event->accept();
}
// 键盘事件 - F6加载样式表
void c_Hikvision::keyPressEvent(QKeyEvent* event)
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
void c_Hikvision::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    if (m_Initialized) return;

    c_Variable::getInstance().g_Hikvision_DB.winId = static_cast<HWND>(ui.Hikvision_Video->winId());  

    QTimer::singleShot(200, this, &c_Hikvision::Camera_Init);
    QTimer::singleShot(300, this, &c_Hikvision::System_Scan);
}
