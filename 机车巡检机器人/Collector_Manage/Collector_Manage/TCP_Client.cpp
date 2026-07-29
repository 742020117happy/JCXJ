#include "TCP_Client.h"

c_TCP_Client::c_TCP_Client(QObject *parent) : QObject(parent)
{
}

c_TCP_Client::~c_TCP_Client()
{
    c_TCP_Client::Disconnect_Device();

    if (m_Socket) {
        m_Socket->deleteLater();
        m_Socket = nullptr;
    }
}

void c_TCP_Client::Init()
{
    // 实例化
    m_Socket = new QTcpSocket;
    
    // 错误信号连接
    QObject::connect(m_Socket, &QTcpSocket::stateChanged, this, [=](int state){
         emit Status(state+23);
    });
    QObject::connect(m_Socket, &QTcpSocket::errorOccurred, this, &c_TCP_Client::Status);
    // 连接状态信号
    QObject::connect(m_Socket, &QTcpSocket::connected, this, &c_TCP_Client::Connect_Done);
    QObject::connect(m_Socket, &QTcpSocket::disconnected, this, &c_TCP_Client::Disconnect_Done);
    
    // 数据接收处理
    QObject::connect(m_Socket, &QTcpSocket::readyRead, this, [=]() {
        QByteArray buffer = m_Socket->readAll();
        
        if (buffer.isEmpty()) {
            return;
        }
        
        // ⚠️ 始终发射原始字节数据（Monitor 用于解析 LTBR 包头）
        emit Read_Byte_Done(buffer);
        
        // ⚠️ 修复：DataSheet 端口 (10004-10006) 数据含"LTBR"包头，不是纯 JSON
        // 只有命令端口 (10003) 才发射 JSON 信号
        if (!m_IsDataSheetPort) {
            QString str = QString::fromUtf8(buffer);
            emit Read_String_Done(str);
            
            QJsonParseError parseError;
            QJsonDocument doc = QJsonDocument::fromJson(buffer, &parseError);
            if (parseError.error == QJsonParseError::NoError && !doc.object().isEmpty()) {
                emit Read_Json_Done(doc.object());
            }
        } else {
            // DataSheet 端口只发射字符串（供 Remote 解析命令响应）
            emit Read_String_Done(QString::fromUtf8(buffer));
        }
    });
}

void c_TCP_Client::Connect_Device(QString ip, int port)
{
    // 标识是否为 DataSheet 端口
    m_IsDataSheetPort = (port == 10004 || port == 10005 || port == 10006);
    
    if (!m_Socket || m_Socket->state() != QAbstractSocket::UnconnectedState) {
        return;
    }
    
    if (m_Stop_Connect) {
        m_Stop_Connect = false;
        return;
    }
    
    m_Ip = ip;
    m_Port = port;
    m_Socket->connectToHost(ip, port);
    
    if (!m_Socket->waitForConnected(3000)) {
        QTimer::singleShot(3000, this, &c_TCP_Client::Connect_Loop);
    }
}

void c_TCP_Client::Disconnect_Device()
{
    if (!m_Socket || m_Socket->state() != QAbstractSocket::ConnectedState) {
        m_Stop_Connect = true;
        return;
    } else {
        m_Socket->close();
    }
}

void c_TCP_Client::Write_Json(QJsonObject value)
{
    if (!m_Socket) { return; }
    if (m_Socket->state() != QAbstractSocket::ConnectedState) { return; }
    if (value.isEmpty()) { return; }
    
    QByteArray sendMessage = QJsonDocument(value).toJson();
    m_Socket->write(sendMessage);
    
    if (m_Socket->flush()) {
        emit Write_Json_Done();
    } else {
        emit Write_Json_Error();
    }
}

void c_TCP_Client::Write_String(QString str)
{
    if (str.isEmpty()) { return; }
    if (!m_Socket) { return; }
    
    m_Socket->write(str.toUtf8());
    
    if (m_Socket->flush()) {
        emit Write_String_Done();
    } else {
        emit Write_String_Error();
    }
}