#pragma once 

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <fcntl.h>
#include <memory>
#include <unistd.h>

#include <QApplication>
#include <QByteArray>
#include <QCloseEvent>
#include <QColor>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QSize>
#include <QEventLoop>
#include <QFile>
#include <QFileDialog>
#include <QHostAddress>
#include <QImage>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QKeyEvent>
#include <QMainWindow>
#include <QMap>
#include <QMessageBox>
#include <QMetaType>
#include <QMutex>
#include <QObject>
#include <QPainter>
#include <QPainterPath>
#include <QPaintDevice> 
#include <QPen>       
#include <QProcess>
#include <QRegularExpression>
#include <QScopedPointer>
#include <QSettings>
#include <QSharedMemory>
#include <QSocketNotifier>
#include <QStandardPaths>
#include <QStackedWidget>
#include <QString>
#include <QTextStream>
#include <QDataStream>
#include <QThread>
#include <QTimer>
#include <QUuid>
#include <QVariant>
#include <QVector>
#include <QWidget>
#include <QSocketNotifier>

#include <QAbstractSocket>
#include <QNetworkAccessManager>
#include <QNetworkInterface>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket> 
#include <QUrlQuery>
#include <QtNetwork>
#include <QtWebSockets/qwebsocket.h>
#include <QtWebSockets/qwebsocketserver.h>
#include <QConicalGradient>

#include <QtConcurrent/QtConcurrent>
#include <QCryptographicHash>  


using namespace std;



struct s_Transmission_DB {
	bool Tran = false;
	bool Connected = false;
	bool Start = false;
	bool Finish = true;
	bool Cancel = false;
	bool Error = false;
	QString device_name = "机车巡检机器人图像接收软件-v1.0";  
	QString Server_Ip = "192.168.0.1";
	int Server_Port = 8888;
    QString Client_Ip;
	int Client_Port;
	QString Save_Path;    
    QStringList buffer;
	QStringList Tran_buffer;
	QStringList Retran_buffer;
	QStringList Received_Buffer; 
};



class c_Object : public QObject
{
    Q_OBJECT
public:
    explicit c_Object(QObject* parent = nullptr);
    virtual ~c_Object();

  	union dataConverter {
		float floatValue;     
		quint32 DWord;
		struct {
			quint16 low16Bits;
			quint16 high16Bits;
		}Word;
	};

    void BitToFloat(float& floatValue, quint16 first16Bits, quint16 second16Bits);
	void FloatToBit(float floatValue, quint16& first16Bits, quint16& second16Bits);
	void IntToBit(quint32 intValue, quint16& high16Bits, quint16& low16Bits);
	void BitToInt(quint32& intValue, quint16 high16Bits, quint16 low16Bits);

    bool QtPing(const QString ip);
    bool msleep(const int mSec);
    QString TCP_Status(int State);
    QString Modbus_Status(int State);
    QString CanOpen(int State);

signals:
    void Status(QString state);

};


class c_Variable : public QObject
{
	Q_OBJECT
public:
	static c_Variable& getInstance();  

									   
	static QJsonObject g_Communicate_DB;  
	static s_Transmission_DB g_Transmission_1;  
	static s_Transmission_DB g_Transmission_2;  


private:
	explicit c_Variable(QObject * parent = nullptr);  
	virtual ~c_Variable();  

							
	c_Variable(const c_Variable &other) = delete;
	c_Variable& operator=(const c_Variable &other) = delete;

	static QMutex g_mutex;  
	static QScopedPointer<c_Variable> g_instance;  

	friend struct QScopedPointerDeleter<c_Variable>;  
};
