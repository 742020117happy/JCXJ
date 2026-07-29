#include "TCP_Server.h"

c_TCP_Server::c_TCP_Server(quint16 port, QObject *parent) : c_Object(parent)
{
    m_Server = nullptr;  
    m_SaveDirectory = QDir::currentPath() + "/received_files"; 

    QDir dir(m_SaveDirectory);
    if (!dir.exists()) {
        dir.mkpath(m_SaveDirectory);  // 创建目录
    }

    m_port = port;
}

c_TCP_Server::~c_TCP_Server()
{
    Stop_Server();  // 停止服务器

    // 清理所有客户端数据
    for (auto it = m_ClientData.begin(); it != m_ClientData.end(); ++it) {
        QTcpSocket* socket = it.key();
        ClientFileData* data = it.value();

        if (data->file) {  // 检查文件指针是否存在
            if (data->file->isOpen()) {  // 检查文件是否打开
                data->file->close();  // 关闭文件
            }
            delete data->file;  // 删除文件指针
        }

        if (data->timeoutTimer) {  // 检查定时器是否存在
            data->timeoutTimer->stop();  // 停止定时器
            data->timeoutTimer->deleteLater();  // 延迟删除定时器
        }

        delete data;  // 删除客户端文件数据
    }
    m_ClientData.clear();  // 清空客户端数据映射

    // 清理套接字
    for (int i = 0; i < m_SocketList.size(); ++i) {
        if (m_SocketList.at(i)) {
            m_SocketList.at(i)->deleteLater();  // 延迟删除套接字
        }
    }
    m_SocketList.clear();  // 清空套接字列表

    // 清理服务器
    if (m_Server) {
        m_Server->deleteLater();  // 延迟删除服务器
        m_Server = nullptr;  // 重置服务器指针
    }
}

void c_TCP_Server::Init()
{
    m_Server = new QTcpServer(this); 
    QObject::connect(m_Server, &QTcpServer::acceptError, this, [=](int state) {
        emit Status(TCP_Status(state));
    });

    QObject::connect(m_Server, &QTcpServer::newConnection, this, [=](){
        if (!m_Server->hasPendingConnections()) { return; }
        QTcpSocket* socket = m_Server->nextPendingConnection();
        if (!socket) { return; }

        m_SocketList.append(socket);

        ClientFileData* clientData = new ClientFileData;
        clientData->state = WaitingForCommand;
        clientData->filePath.clear();
        clientData->fileName.clear();
        clientData->fileDir.clear();
        clientData->fileSize = 0;
        clientData->receivedSize = 0;
        clientData->fileHash.clear();
        clientData->file = nullptr;
        clientData->saveDir.clear();
        clientData->savePath.clear();
        clientData->retryCount = 0;
        clientData->timeoutTimer = new QTimer(this);
        clientData->timeoutTimer->setSingleShot(true);
        clientData->timeoutTimer->setInterval(60000);  // 60秒超时
        m_ClientData[socket] = clientData;
        
        QObject::connect(clientData->timeoutTimer, &QTimer::timeout, this, [=]() {
            for (auto it = m_ClientData.begin(); it != m_ClientData.end(); ++it) {
                QTcpSocket* sock = it.key();
                ClientFileData* data = it.value();

                // 检查定时器是否超时
                if (data->timeoutTimer->isActive() && data->timeoutTimer->remainingTime() <= 0) {
                    QJsonObject response;  // 构建响应JSON
                    response["cmd"] = "ERROR";
                    response["filePath"] = data->filePath;
                    response["message"] = "Operation timed out";  // 超时信息
                    Send_Command(sock, response);  // 发送错误响应

                    Close_File_For_Client(sock, false);  // 关闭文件
                    break;
                }
            }
        });
        
        emit Client_Connected();
        emit Status("客户端连接: " + socket->peerAddress().toString());
        
        QObject::connect(socket, &QTcpSocket::disconnected, this, [socket, this]() {
            emit Client_Disconnected();
            emit Status("客户端断开: " + socket->peerAddress().toString());
            msleep(100);
            Cleanup_Client_Data(socket);
            msleep(100);
            m_SocketList.removeOne(socket);
            msleep(100);
            socket->deleteLater();
        });
        
        QObject::connect(socket, &QTcpSocket::readyRead, this, [socket, this]() {
            if (!m_ClientData.contains(socket)) { return; }
            m_ClientData[socket]->timeoutTimer->start();
            m_ClientData[socket]->buffer = socket->readAll();
            if (m_ClientData[socket]->buffer.isEmpty()) { return; }
            
            if (m_ClientData[socket]->state == ReceivingFileData) {
                Process_File_Data(socket); 
            }
            
            if (m_ClientData[socket]->state != ReceivingFileData) {
                QJsonObject json = QJsonDocument::fromJson(m_ClientData[socket]->buffer).object();

                if (json.isEmpty()) { return; }

                emit Status("接收: " + m_ClientData[socket]->buffer);

                QString cmd = json.value("cmd").toString();  

                if (cmd == "file_info") {  
                    Process_File_Info(socket, json);  
                }
            }
        });
    });
}

void c_TCP_Server::Start_Server()
{
    if(!m_Server || m_Server->isListening()) {
        return;
    }

    QString serverIp = c_Variable::getInstance().g_Communicate_DB.value("Server_Ip").toString();

    QHostAddress address;
    if (serverIp == "0.0.0.0" || serverIp.isEmpty()) {  // 判断IP是否为空
        address = QHostAddress::Any;  // 监听所有接口
    }
    else {
        address = QHostAddress(serverIp); 
    }

    if(m_Server->listen(address, m_port)) {
        emit Server_Started(); 
        emit Status(QString("服务器启动成功，监听: %1:%2").arg(serverIp).arg(m_port));  // 写入成功信息
    } else {
        emit Server_Stopped(); 
        emit Status(QString("服务器启动失败: %1").arg(m_Server->errorString()));  // 写入错误信息
    }
}

void c_TCP_Server::Stop_Server()
{
    // 断开所有客户端连接
    for (int i = 0; i < m_SocketList.size(); ++i) {
        QTcpSocket* socket = m_SocketList.at(i);
        if (socket) {
            socket->disconnectFromHost();  // 断开连接
            Cleanup_Client_Data(socket);  // 清理客户端数据
            socket->deleteLater();  // 延迟删除套接字
        }
    }
    m_SocketList.clear();  // 清空套接字列表

    // 关闭服务器
    if (m_Server && m_Server->isListening()) {
        m_Server->close();  // 关闭服务器
    }

    emit Server_Stopped();  // 发送服务器停止信号
}

// 处理文件信息
void c_TCP_Server::Process_File_Info(QTcpSocket* socket, const QJsonObject& json)
{
    ClientFileData* clientData = m_ClientData[socket];  

    clientData->filePath = json["filePath"].toString();  // 获取文件路径
    clientData->fileSize = json["fileSize"].toString().toULongLong();  // 获取文件大小
    clientData->fileHash = json["hash"].toString();  // 获取文件哈希
    QFileInfo fileInfo(clientData->filePath);
    clientData->fileName = fileInfo.fileName();  // 获取文件名
    clientData->fileDir = fileInfo.path();  // F:/RawImageDisk/PreScan_raw/J11/.../202404231143CR400AF2200
    
    // 检查文件后缀
    QString suffix = QFileInfo(clientData->filePath).suffix().toLower();  // 获取文件后缀(小写)
    bool ok = suffix == "jpg" || suffix == "png" || suffix == "xyz" || suffix == "data";  // 检查是否为有效后缀
    if (!ok) {
        QJsonObject response;
        response["cmd"] = "ERROR";
        response["message"] = "文件类型错误";
        Send_Command(socket, response); 
        return;
    }

    //m_SaveDirectory = c_Variable::getInstance().g_Communicate_DB.value("Save_Directory_1").toString();

    QString normalizedPath = QDir::fromNativeSeparators(clientData->fileDir);
    QString baseMarker = QStringLiteral("/PreScan_raw/");
    int idx = normalizedPath.indexOf(baseMarker);
    if (idx == -1) {
        baseMarker = QStringLiteral("PreScan_raw/");
        idx = normalizedPath.indexOf(baseMarker);
        if (idx == -1) {
            // 无法解析路径结构
            QJsonObject response;
            response["cmd"] = "ERROR";
            response["message"] = "无法解析路径结构";
            Send_Command(socket, response);  // 发送错误响应
            return;
        }
        clientData->saveDir = QDir::cleanPath(m_SaveDirectory + QLatin1Char('/')
            + normalizedPath.mid(idx + baseMarker.length() - 1)); 
    }
    else {
        clientData->saveDir = QDir::cleanPath(m_SaveDirectory + QLatin1Char('/')
            + normalizedPath.mid(idx + baseMarker.length() - 1));
    }
    clientData->savePath = QDir::cleanPath(clientData->saveDir + QLatin1Char('/') + clientData->fileName);
    
    QDir dir;
    dir.mkpath(clientData->saveDir);

    clientData->file = new QFile(clientData->savePath);
    if (!clientData->file->open(QFile::WriteOnly)) {
        QJsonObject response;
        response["cmd"] = "ERROR";
        response["filePath"] = clientData->filePath;
        response["message"] = "Cannot create file: " + clientData->savePath;
        Send_Command(socket, response);  // 发送错误响应
        delete clientData->file;  // 删除文件指针
        clientData->file = nullptr;  // 重置文件指针
        return;
    }

    QJsonObject ack;
    ack["cmd"] = "START";
    ack["filePath"] = clientData->filePath;
    ack["fileSize"] = QString::number(clientData->fileSize);
    Send_Command(socket, ack);  // 发送确认响应

    // 更新状态
    clientData->state = ReceivingFileData;
    clientData->receivedSize = 0;
}

// 处理文件数据
void c_TCP_Server::Process_File_Data(QTcpSocket* socket)
{
    ClientFileData* clientData = m_ClientData[socket];  // 获取客户端数据

    // 检查缓冲区是否为空
    if (m_ClientData[socket]->buffer.isEmpty()) {
        emit Status("m_ClientData[socket]->buffer.isEmpty()");
        return; 
    }  
    if (!clientData->file) {
        emit Status("!clientData->file");
        return;
    }
    if (!clientData->file->isOpen()) {
        emit Status("!clientData->file->isOpen()");
        return;
    }
    
    // 写入文件
    qint64 bytesWritten = clientData->file->write(m_ClientData[socket]->buffer);
    if (bytesWritten != m_ClientData[socket]->buffer.size()) {
        QJsonObject response;
        response["cmd"] = "ERROR";
        response["filePath"] = clientData->filePath;
        response["message"] = "Error writing to file: " + clientData->file->errorString();
        Send_Command(socket, response);  // 发送错误响应

        Close_File_For_Client(socket, false);  // 关闭文件
        return;
    }
    
    // 更新已接收大小
    clientData->receivedSize += bytesWritten;  
    
    // 检查是否接收完成
    if (clientData->receivedSize >= clientData->fileSize) {
        clientData->file->close();  // 关闭文件

        // 计算接收文件的MD5哈希值
        QCryptographicHash hash(QCryptographicHash::Md5);
        QFile receivedFile(clientData->savePath);
        QString serverHash;
        bool success = false;

        if (receivedFile.open(QFile::ReadOnly)) {
            hash.addData(&receivedFile);
            serverHash = hash.result().toHex();
            receivedFile.close();

            // 比较哈希值
            if (serverHash == clientData->fileHash) {
                success = true;
                emit Status("文件完整性验证成功");  // 写入成功信息
            } else {
                emit Status(QString("文件完整性验证失败. 客户端哈希: %1, 服务器哈希: %2")
                    .arg(clientData->fileHash)
                    .arg(serverHash));  // 写入错误信息
            }
        } else {
            emit Status(QString("无法打开接收文件进行验证: %1")
                .arg(receivedFile.errorString()));  // 写入错误信息
        }

        // 构建验证响应
        QJsonObject response;
        response["cmd"] = success ? "FINISH" : "ERROR";  // 修复: ERORR -> ERROR
        response["status"] = success ? "ok" : "failed";
        response["filePath"] = clientData->savePath;
        response["server_hash"] = serverHash;
        
        // 发送验证响应
        Send_Command(socket, response);
        
        // 关闭文件传输
        Close_File_For_Client(socket, success);  // 关闭文件
        clientData->state = WaitingForVerification;  // 设置状态为等待验证
    }
}

// 发送命令
void c_TCP_Server::Send_Command(QTcpSocket* socket, const QJsonObject& command)
{
    // 检查套接字状态
    if (!socket || socket->state() != QTcpSocket::ConnectedState) { return; }
    
    // 将JSON对象转换为字节数组
    QByteArray data = QJsonDocument(command).toJson();

    msleep(100);

    emit Status("发送: " + data);
    socket->write(data);  // 写入数据
    socket->flush();  // 刷新缓冲区
}

// 为客户端关闭文件
void c_TCP_Server::Close_File_For_Client(QTcpSocket* socket, bool success)
{
    // 检查客户端数据是否存在
    if (!m_ClientData.contains(socket)) return;

    ClientFileData* clientData = m_ClientData[socket];  // 获取客户端数据

    if (clientData->file) {
        if (clientData->file->isOpen()) {
            clientData->file->close();
        }
        delete clientData->file;  // 删除文件指针
        clientData->file = nullptr;  // 重置文件指针
    }

    // 重置状态
    clientData->state = WaitingForCommand;  // 设置状态为等待命令
    clientData->receivedSize = 0;  // 重置已接收大小
    clientData->buffer.clear();  // 清空缓冲区
}

// 清理客户端数据
void c_TCP_Server::Cleanup_Client_Data(QTcpSocket* socket)
{
    // 检查客户端数据是否存在
    if (m_ClientData.contains(socket)) {
        ClientFileData* data = m_ClientData[socket];  // 获取客户端数据
        
        // 关闭文件
        if (data->file) {
            if (data->file->isOpen()) {
                data->file->close();
            }
            delete data->file; 
        }

        // 停止并删除定时器
        if (data->timeoutTimer) {
            data->timeoutTimer->stop();
            data->timeoutTimer->deleteLater();
        }

        delete data;  // 删除客户端数据
        m_ClientData.remove(socket);  // 从映射中移除
    }
}

void c_TCP_Server::get_SaveDirectory(QString path)
{
    m_SaveDirectory = path;
}
