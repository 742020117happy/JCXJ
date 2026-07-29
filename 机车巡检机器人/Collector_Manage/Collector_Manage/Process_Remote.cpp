#include "Process_Remote.h"

c_Process_Remote::c_Process_Remote(QObject *parent) : c_Object(parent){}
c_Process_Remote::~c_Process_Remote()
{
	m_Start = false;
	emit Status(m_device_name + "：父类正常析构");
	Close_Device();
	if (m_Process->state() == QProcess::Running) {
		m_Process->terminate();
		if (!m_Process->waitForFinished(3000)) { // 等待3秒，若未结束则强制终止
			m_Process->kill();
		}
	}
	m_Process->deleteLater();
}
void c_Process_Remote::Init()
{
	m_Process = new QProcess(this);
	QObject::connect(m_Process, &QProcess::readyReadStandardOutput, this, &c_Process_Remote::Read_Data);
	QObject::connect(m_Process, &QProcess::readyReadStandardError, this, &c_Process_Remote::Error_Data);
	QObject::connect(m_Process, static_cast<void(QProcess::*)(int , QProcess::ExitStatus)>(&QProcess::finished), this, [=](int exitCode, QProcess::ExitStatus exitStatus) {
		QString mesage = QString("%1进程退出代号：%2 -> 退出状态：%3").arg(m_device_name).arg(exitCode).arg(exitStatus);
		emit Status(mesage);
		if (m_Start) {
			emit Disconnect_Done();
			emit is_Stop();
			QTimer::singleShot(3000, this, &c_Process_Remote::Connect_Device);
		}		
	});
	QTimer::singleShot(3000, this, &c_Process_Remote::Connect_Device);
}
void c_Process_Remote::Connect()
{
	if (m_Process->state() != QProcess::Running) {
		return;
	}
	m_Process->write("Connect");
}
void c_Process_Remote::Disconnect()
{
	if (m_Process->state() != QProcess::Running) {
		return;
	}
	m_Process->write("Disconnect");
}
void c_Process_Remote::Close_Device()
{
	if (m_Process->state() != QProcess::Running) {
		return;
	}
	m_Process->write("Close_Device");
	m_Process->terminate();
	if (!m_Process->waitForFinished(3000)) { 
		m_Start = false;
		m_Process->kill();
	}
}
void c_Process_Remote::Connect_Device()
{
	if (m_Process->state() == QProcess::Running) {
		emit Status(m_device_name + "：进程已启动");
		return;
	}
	m_Start = false;

	emit Status(m_device_name + "：进程启动");

	QString exePath = "/home/nvidia/Collector_Manage/build/bin/" + m_process_name;

	if (!QFileInfo(exePath).isExecutable()) {
		emit Status(m_device_name + "：可执行文件不存在或不可执行");
		return;
	}

	QStringList mainArgs;
	mainArgs.append(m_device_name);
	m_Process->start(exePath, mainArgs);  // 启动子进程

	if (!m_Process->waitForStarted(8000)) {
		emit Status(m_device_name + "：进程启动失败" + m_Process->errorString());
		return;
	}
}
void c_Process_Remote::Write(QString msg) 
{
	if (m_Process->state() != QProcess::Running) {
		emit Status(m_device_name + "：消息发送失败：进程未启动");
		return;
	}
	m_Process->write(msg.toUtf8().data());
}
void c_Process_Remote::Read_Data()
{
	QByteArray output = m_Process->readAllStandardOutput();
	QString message = QString::fromUtf8(output);

	if (message == "Connect_Done") {
		emit Connect_Done();
		return;
	}
	if (message == "Disconnect_Done") {
		emit Disconnect_Done();
		return;
	}
	if (message == "Stop") {
		m_Start = false;
		emit is_Stop();
		return;
	}
	if (message.split("&", Qt::SkipEmptyParts).isEmpty()) {
		return;
	}
	if (message.split("&", Qt::SkipEmptyParts).at(0) == "Run") {
		m_Start = true;
		emit is_Run();
		QString windowIdStr = message.split("&", Qt::SkipEmptyParts).at(1);
		bool ok = false;
		WId windowId = windowIdStr.toULong(&ok);
		if (ok && windowId) {
			emit Show(windowId);
		}
	}
	Device_Data(message);
}
void c_Process_Remote::Error_Data()
{
	QByteArray errorOutput = m_Process->readAllStandardError();
	emit Status(m_device_name + "->" + QString::fromUtf8(errorOutput));
}
void c_Process_Remote::Device_Data(QString message) {}

