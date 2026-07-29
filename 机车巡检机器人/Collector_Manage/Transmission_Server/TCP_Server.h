#pragma once
#include "Variable.h"  // 包含全局变量头文件

// TCP服务器类
class c_TCP_Server : public c_Object
{
	Q_OBJECT  // Qt元对象系统宏
public:
	explicit c_TCP_Server(quint16 port, QObject *parent = nullptr);  // 构造函数
	virtual ~c_TCP_Server();  // 析构函数

public slots:
	void Init();  // 初始化服务器
	void Start_Server();  // 启动服务器
	void Stop_Server();  // 停止服务器
	void get_SaveDirectory(QString path);

signals:
	void Server_Started();        // 服务器启动信号
	void Server_Stopped();        // 服务器停止信号
	void Client_Connected();      // 客户端连接信号
	void Client_Disconnected();   // 客户端断开信号

private:
	// 客户端状态枚举
	enum ClientState {
		WaitingForCommand,    // 等待命令
		ReceivingFileInfo,    // 接收文件信息
		ReceivingFileData,    // 接收文件数据
		WaitingForVerification // 等待验证
	};

	// 客户端文件数据结构
	struct ClientFileData {
		ClientState state;          // 客户端状态
		QString filePath;           // 文件远程路径
		QString fileName;           // 文件名
		QString fileDir;            // 文件远程目录
		quint64 fileSize;           // 文件大小
		quint64 receivedSize;       // 已接收大小
		QString fileHash;           // 文件哈希
		QFile* file;                // 文件指针
		QString saveDir;            // 存储目录
		QString savePath;           // 保存文件完整路径
		int retryCount;             // 重试计数
		QTimer* timeoutTimer;       // 超时定时器
		QByteArray buffer;          // 缓冲区
	};

	void Process_File_Info(QTcpSocket* socket, const QJsonObject& json);  // 处理文件信息
	void Process_File_Data(QTcpSocket* socket);  // 处理文件数据
	void Send_Command(QTcpSocket* socket, const QJsonObject& command);  // 发送命令
	void Close_File_For_Client(QTcpSocket* socket, bool success);  // 关闭客户端文件
	void Cleanup_Client_Data(QTcpSocket* socket);  // 清理客户端数据

private:
	QTcpServer* m_Server;              // TCP服务器实例
	QList<QTcpSocket*> m_SocketList;   // 套接字列表
	QMap<QTcpSocket*, ClientFileData*> m_ClientData;  // 客户端数据映射
	QString m_SaveDirectory;           // 保存目录
	quint16 m_port;                    // 监听端口
};