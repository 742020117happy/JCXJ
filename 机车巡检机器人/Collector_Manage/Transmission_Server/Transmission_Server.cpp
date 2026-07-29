#include "Transmission_Server.h"  // 头文件

// 构造函数
c_Transmission_Server::c_Transmission_Server(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);  
    
    QFile File;
    File.setFileName(QDir::currentPath() + "/stuqss.css");
    File.open(QIODevice::ReadOnly);
    QString strQss = File.readAll();
    this->setStyleSheet(strQss);
    File.close();

    
    File.setFileName(QDir::currentPath() + "/Transmission_Server.json");
    File.open(QFile::ReadOnly | QIODevice::Text);
    
    QByteArray Data = File.readAll();  
    
    QJsonParseError parseError; 
    QJsonDocument DB_Doc = QJsonDocument::fromJson(Data, &parseError);  
    if (parseError.error == QJsonParseError::NoError) {
        c_Variable::getInstance().g_Communicate_DB = DB_Doc.object();
    }
    // 关闭文件
    File.close();  
    
    ui.Worry_List->document()->setMaximumBlockCount(5000000);

	 m_logDir = QDir::currentPath() + "/LOG/" + 
                     QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
					 
    QObject::connect(ui.get_Directory_1, &QPushButton::clicked, this, [=]() {
        QString dir = QFileDialog::getExistingDirectory(this, "选择保存目录");
        if (!dir.isEmpty()) {
            ui.Save_Directory_1->setText(dir);  // 设置目录文本
        }
    });
    QObject::connect(ui.get_Directory_2, &QPushButton::clicked, this, [=]() {
        QString dir = QFileDialog::getExistingDirectory(this, "选择保存目录");
        if (!dir.isEmpty()) {
            ui.Save_Directory_2->setText(dir);  // 设置目录文本
        }
        });

    this->show();
    TCP_Server_Init();
}

// 析构函数
c_Transmission_Server::~c_Transmission_Server()
{
    TCP_Server_Delete();  // 删除TCP服务器
}

// TCP服务器初始化
void c_Transmission_Server::TCP_Server_Init()
{
    m_Server_1_Thread = new QThread();
    m_Server_1_Remote = new c_TCP_Server(c_Variable::getInstance().g_Communicate_DB.value("Server_1_Port").toInt());
    m_Server_1_Remote->moveToThread(m_Server_1_Thread);

    QObject::connect(m_Server_1_Thread, &QThread::started,m_Server_1_Remote, &c_TCP_Server::Init);
    QObject::connect(m_Server_1_Thread, &QThread::finished, m_Server_1_Remote, &c_TCP_Server::deleteLater);
    
    QObject::connect(ui.startButton_1, &QPushButton::clicked, this, [=](){
        if (c_Variable::getInstance().g_Transmission_1.Connected) {
            emit Stop_Server_Command();  // 发送停止服务器命令
        }
        else {
            emit Start_Server_Command();  // 发送启动服务器命令
        }
    });
    QObject::connect(this, &c_Transmission_Server::Start_Server_Command,m_Server_1_Remote, &c_TCP_Server::Start_Server);
    QObject::connect(this, &c_Transmission_Server::Stop_Server_Command,m_Server_1_Remote, &c_TCP_Server::Stop_Server);

    QObject::connect(m_Server_1_Remote, &c_TCP_Server::Server_Started, this, [=]() {
        c_Variable::getInstance().g_Transmission_1.Connected = true;  // 设置连接状态
        ui.startButton_1->setText("停止监听");
    });
    QObject::connect(m_Server_1_Remote, &c_TCP_Server::Server_Stopped, this, [=](){
        c_Variable::getInstance().g_Transmission_1.Connected = false;  // 设置断开监听状态
        ui.startButton_1->setText("开始监听");  // 更新按钮文本
    }); 
    QObject::connect(m_Server_1_Remote, &c_TCP_Server::Client_Connected,this, [=](){
        c_Variable::getInstance().g_Transmission_1.Tran = true;
    });
    QObject::connect(m_Server_1_Remote, &c_TCP_Server::Client_Disconnected, this, [=]() {
        c_Variable::getInstance().g_Transmission_1.Tran = false;  // 设置传输状态
    });

    QObject::connect(m_Server_1_Remote, &c_TCP_Server::Status,this, & c_Transmission_Server::Write_Worry_List);
    QObject::connect(ui.Save_Directory_1, &QLineEdit::textChanged, m_Server_1_Remote, &c_TCP_Server::get_SaveDirectory);
    // 启动服务器线程
    m_Server_1_Thread->start();

    m_Server_2_Thread = new QThread();
    m_Server_2_Remote = new c_TCP_Server(c_Variable::getInstance().g_Communicate_DB.value("Server_2_Port").toInt());
    m_Server_2_Remote->moveToThread(m_Server_2_Thread);

    QObject::connect(m_Server_2_Thread, &QThread::started, m_Server_2_Remote, &c_TCP_Server::Init);
    QObject::connect(m_Server_2_Thread, &QThread::finished, m_Server_2_Remote, &c_TCP_Server::deleteLater);

    QObject::connect(ui.startButton_2, &QPushButton::clicked, this, [=]() {
        if (c_Variable::getInstance().g_Transmission_2.Connected) {
            emit Stop_Server_Command();  // 发送停止服务器命令
        }
        else {
            emit Start_Server_Command();  // 发送启动服务器命令
        }
        });
    QObject::connect(this, &c_Transmission_Server::Start_Server_Command, m_Server_2_Remote, &c_TCP_Server::Start_Server);
    QObject::connect(this, &c_Transmission_Server::Stop_Server_Command, m_Server_2_Remote, &c_TCP_Server::Stop_Server);

    QObject::connect(m_Server_2_Remote, &c_TCP_Server::Server_Started, this, [=]() {
        c_Variable::getInstance().g_Transmission_2.Connected = true;  // 设置连接状态
        ui.startButton_2->setText("停止监听");
        });
    QObject::connect(m_Server_2_Remote, &c_TCP_Server::Server_Stopped, this, [=]() {
        c_Variable::getInstance().g_Transmission_2.Connected = false;  // 设置断开监听状态
        ui.startButton_2->setText("开始监听");  // 更新按钮文本
        });
    QObject::connect(m_Server_2_Remote, &c_TCP_Server::Client_Connected, this, [=]() {
        c_Variable::getInstance().g_Transmission_2.Tran = true;
        });
    QObject::connect(m_Server_2_Remote, &c_TCP_Server::Client_Disconnected, this, [=]() {
        c_Variable::getInstance().g_Transmission_2.Tran = false;  // 设置传输状态
        });

    QObject::connect(m_Server_2_Remote, &c_TCP_Server::Status, this, &c_Transmission_Server::Write_Worry_List);

    QObject::connect(ui.Save_Directory_2, &QLineEdit::textChanged, m_Server_2_Remote, &c_TCP_Server::get_SaveDirectory);
    // 启动服务器线程
    m_Server_2_Thread->start();

    // 设置扫描标志
    m_Scan = true;  

    TCP_Server_DB();
    // 10毫秒后开始扫描
    TCP_Server_Scan();
    
    QTimer::singleShot(100, this, &c_Transmission_Server::Start_Server_Command);
}
void c_Transmission_Server::TCP_Server_DB()
{
    // 绑定UI信号
    QObject::connect(ui.Server_1_Ip, &QLineEdit::textChanged, this, [=](QString ip) {
        Write_Communicate_DB("Server_1_Ip", ip);  // 写入通信数据库
    });

    QObject::connect(ui.Server_1_Port, &QLineEdit::textChanged, this, [=](QString port) {
        bool ok;
        int value = port.toInt(&ok);  // 转换为整数
        if (ok) {
            Write_Communicate_DB("Server_1_Port", value);  // 写入通信数据库
        }
    });

    QObject::connect(ui.Save_Directory_1, &QLineEdit::textChanged, this, [=](QString dir) {
        Write_Communicate_DB("Save_Directory_1", dir);  // 写入通信数据库
    });

    QObject::connect(ui.Server_2_Ip, &QLineEdit::textChanged, this, [=](QString ip) {
        Write_Communicate_DB("Server_2_Ip", ip);  // 写入通信数据库
        });

    QObject::connect(ui.Server_2_Port, &QLineEdit::textChanged, this, [=](QString port) {
        bool ok;
        int value = port.toInt(&ok);  // 转换为整数
        if (ok) {
            Write_Communicate_DB("Server_2_Port", value);  // 写入通信数据库
        }
        });

    QObject::connect(ui.Save_Directory_2, &QLineEdit::textChanged, this, [=](QString dir) {
        Write_Communicate_DB("Save_Directory_2", dir);  // 写入通信数据库
        });

    // 初始化UI控件
    ui.Server_1_Ip->setText(c_Variable::getInstance().g_Communicate_DB.value("Server_1_Ip").toString());
    ui.Server_1_Port->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("Server_1_Port").toInt()));
    ui.Save_Directory_1->setText(c_Variable::getInstance().g_Communicate_DB.value("Save_Directory_1").toString());

    ui.Server_2_Ip->setText(c_Variable::getInstance().g_Communicate_DB.value("Server_2_Ip").toString());
    ui.Server_2_Port->setText(QString::number(c_Variable::getInstance().g_Communicate_DB.value("Server_2_Port").toInt()));
    ui.Save_Directory_2->setText(c_Variable::getInstance().g_Communicate_DB.value("Save_Directory_2").toString());
}
void c_Transmission_Server::TCP_Server_Scan()
{
    ui.Server_1_State->Set_State(c_Variable::getInstance().g_Transmission_1.Connected);
    ui.Receiving_1->Set_State(c_Variable::getInstance().g_Transmission_1.Tran);  // 设置接收状态为真
    ui.Start_1->Set_State(c_Variable::getInstance().g_Transmission_1.Start);  // 设置开始状态为真
    ui.Finish_1->Set_State(c_Variable::getInstance().g_Transmission_1.Finish);  // 设置完成状态为真
    ui.Error_1->Set_State(c_Variable::getInstance().g_Transmission_1.Error);  // 设置错误状态为真
    ui.File_Name_Num_1->setText(QString::number(c_Variable::getInstance().g_Transmission_1.Received_Buffer.size()));

	 ui.Server_2_State->Set_State(c_Variable::getInstance().g_Transmission_2.Connected);
    ui.Receiving_2->Set_State(c_Variable::getInstance().g_Transmission_2.Tran);  
    ui.Start_2->Set_State(c_Variable::getInstance().g_Transmission_2.Start); 
    ui.Finish_2->Set_State(c_Variable::getInstance().g_Transmission_2.Finish);  
    ui.Error_2->Set_State(c_Variable::getInstance().g_Transmission_2.Error); 
    ui.File_Name_Num_2->setText(QString::number(c_Variable::getInstance().g_Transmission_2.Received_Buffer.size()));

    if (m_Scan) {
        // 10毫秒后再次扫描
        QTimer::singleShot(10, this, SLOT(TCP_Server_Scan()));
    }
}
void c_Transmission_Server::TCP_Server_Delete()
{
    m_Scan = false;  // 清除扫描标志

    if (m_Server_1_Thread->isRunning()) {  // 检查线程是否运行
        emit Stop_Server_Command(); // 停止服务器
        m_Server_1_Thread->requestInterruption();  // 请求中断
        m_Server_1_Thread->quit();  // 退出线程
        m_Server_1_Thread->wait();  // 等待线程结束
    }

 if (m_Server_2_Thread->isRunning()) {  // 检查线程是否运行
        emit Stop_Server_Command(); // 停止服务器
        m_Server_2_Thread->requestInterruption();  // 请求中断
        m_Server_2_Thread->quit();  // 退出线程
        m_Server_2_Thread->wait();  // 等待线程结束
    }
}

void c_Transmission_Server::Write_Communicate_DB(QString key, int value)
{
    c_Variable::getInstance().g_Communicate_DB.insert(key, value);  // 插入值

                                                                    // 保存到文件
    QFile File;
    File.setFileName(QDir::currentPath() + "/Transmission_Server.json");
    File.open(QIODevice::ReadWrite | QIODevice::Text);
    QJsonDocument DB_Doc;
    DB_Doc.setObject(c_Variable::getInstance().g_Communicate_DB);
    File.write(DB_Doc.toJson());  // 写入JSON
    File.close();  // 关闭文件
}
void c_Transmission_Server::Write_Communicate_DB(QString key, QString value)
{
    c_Variable::getInstance().g_Communicate_DB.insert(key, value);  // 插入值

                                                                    // 保存到文件
    QFile File;
    File.setFileName(QDir::currentPath() + "/Transmission_Server.json");
    File.open(QIODevice::ReadWrite | QIODevice::Text);
    QJsonDocument DB_Doc;
    DB_Doc.setObject(c_Variable::getInstance().g_Communicate_DB);
    File.write(DB_Doc.toJson());  // 写入JSON
    File.close();  // 关闭文件
}

void c_Transmission_Server::Write_Worry_List(QString value)
{
     // 1. 更新 UI 显示
    ui.Worry_List->append(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss") + "->" + value);
    QDir().mkpath(m_logDir);  // 递归创建目录

    QFile logFile(m_logDir + "/system_log.txt");  // ← 英文文件名

    // 3. 安全写入日志
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        logFile.write(ui.Worry_List->toPlainText().toUtf8());
        logFile.close();
    } else {
        qWarning() << "Log write failed:" << logFile.errorString();
    }
}

void c_Transmission_Server::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F6) {  // F6键
        QFile File;
        File.setFileName(QDir::currentPath() + "/stuqss.css");
        File.open(QIODevice::ReadOnly);
        QString strQss = File.readAll();
        this->setStyleSheet(strQss);  // 重新加载样式表
        File.close();  // 关闭文件
    }
}
void c_Transmission_Server::closeEvent(QCloseEvent *event)
{
    TCP_Server_Delete();  // 删除TCP服务器
    Write_Worry_List( "Stop");  // 写入停止信息
    event->accept();  // 接受关闭事件
}
