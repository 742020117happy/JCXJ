#include "Work_Remote.h"

c_Sql_Remote::c_Sql_Remote(QObject *parent) : c_Object(parent)
{

}

c_Sql_Remote::~c_Sql_Remote()
{
	mysql_close(m_conn);
}
void c_Sql_Remote::Init()
{
	m_Mutex = new QMutex;
	m_conn = mysql_init(NULL);
	emit Status("MySQL 初始化！");
}
void c_Sql_Remote::connectMysql()
{
	QString HOST = c_Variable::getInstance().g_Communicate_DB.value("Web_Server_Ip").toString();
	int PORT = c_Variable::getInstance().g_Communicate_DB.value("MySql_Port").toInt();
	QString USER = c_Variable::getInstance().g_Communicate_DB.value("MySql_User").toString();
	QString PASSWORD = c_Variable::getInstance().g_Communicate_DB.value("MySql_Password").toString();
	QString DATABASE = c_Variable::getInstance().g_Communicate_DB.value("MySql_Database").toString();

	if (!c_Object::QtPing(HOST)) {
		emit Status("MySQL 网络未准备好！");
		QTimer::singleShot(1000, this, &c_Sql_Remote::connectMysql);
		return;
	}

	if (mysql_ping(m_conn) == 0) {
		emit Status("MySQL 已连接");

		if (m_imagePathList.size() > 1) {
			emit Status("重新发送采集信息");
			QTimer::singleShot(10, this, &c_Sql_Remote::updateCollectTime);
		}

		return;
	}

	if (!mysql_real_connect(m_conn, HOST.toLatin1().data(), USER.toLatin1().data(), PASSWORD.toLatin1().data(), DATABASE.toLatin1().data(), PORT, NULL, 0)) {
		emit Status("MySQL 连接失败：" + QString(mysql_error(m_conn)));
		return;
	}
	else {
		emit Status("MySQL 连接成功！");
	}

	if (mysql_query(m_conn, "SET NAMES utf8mb4")) {
		emit Status("设置字符集失败: " + QString(mysql_error(m_conn)));
		return;
	}
	else {
		emit Status("设置字符集成功");
		if (m_imagePathList.size() > 1) {
			QTimer::singleShot(10, this, &c_Sql_Remote::updateCollectTime);
		}
	}
	emit Connect_Done();
}
void c_Sql_Remote::updateActualReturnTime()
{
	if (mysql_ping(m_conn) != 0) {
		QTimer::singleShot(100, this, &c_Sql_Remote::connectMysql);
		emit Status("连接到数据库失败,重新连接");
		QTimer::singleShot(2000, this, &c_Sql_Remote::updateActualReturnTime);
		return;
	}
	// 构造 SQL 语句（参数化查询）
	QString sql = QString("UPDATE tbl_robot_inspection_tasks SET actual_return_time = NOW() WHERE task_number = '%1'")
		.arg(c_Variable::getInstance().g_Work.inspectionTaskNum);

	std::string stdStr = sql.toStdString();

	// 执行 SQL 语句
	if (mysql_query(m_conn, stdStr.c_str())) {
		//emit Status("数据库更新失败");
	}
	else {
		emit Status("数据库更新成功");
	}

}
void c_Sql_Remote::updateFastScanTime()
{
	if (mysql_ping(m_conn) != 0) {
		QTimer::singleShot(100, this, &c_Sql_Remote::connectMysql);
		emit Status("连接到数据库失败,重新连接");
		QTimer::singleShot(2000, this, &c_Sql_Remote::updateActualReturnTime);
		return;
	}
	// 构造 SQL 语句（参数化查询）
	QString sql = QString("UPDATE tbl_robot_inspection_tasks SET fast_scan_end_time = NOW() WHERE task_number = '%1'")
		.arg(c_Variable::getInstance().g_Work.inspectionTaskNum);

	std::string stdStr = sql.toStdString();

	// 执行 SQL 语句
	if (mysql_query(m_conn, stdStr.c_str())) {
		//emit Status("数据库更新失败");
	}
	else {
		emit Status("数据库更新成功");
	}
}
void c_Sql_Remote::updateCollectTime()
{
	if (mysql_ping(m_conn) != 0) {
		QTimer::singleShot(100, this, &c_Sql_Remote::connectMysql);
		emit Status("重新连接到数据库");
		return;
	}

	if (m_imagePathList.isEmpty()) {
		return;
	}

	QString imagePath = m_imagePathList.first();

	QString sql = QString("INSERT INTO tbl_robot_detection_time (inspection_task_num, image, collection_time) VALUES('%1','%2',NOW())")
		.arg(c_Variable::getInstance().g_Work.Work_Num).arg(imagePath);

	std::string stdStr = sql.toStdString();

	// 执行 SQL 语句
	if (mysql_query(m_conn, stdStr.c_str())) {
		//emit Status(imagePath + "数据库更新失败");
	}
	else {
		m_imagePathList.removeFirst();
	}
	//直到把m_imagePathList写完
	QTimer::singleShot(10, this, &c_Sql_Remote::updateCollectTime);
}
void c_Sql_Remote::Prec_Scan_Write_Done(QString value)
{
	if (!c_Variable::getInstance().g_Work.Connected) { emit Status("退出任务"); return; }

	QMutexLocker locker(m_Mutex);
	m_imagePathList.append(value);

	if (m_imagePathList.size() == 1) {
		QTimer::singleShot(10, this, &c_Sql_Remote::updateCollectTime);
	}
}

//构造函数
c_Work_Remote::c_Work_Remote(QObject *parent) : c_Object(parent)
{
}
//析构函数
c_Work_Remote::~c_Work_Remote()
{
	
}
//Function:初始化函数
void c_Work_Remote::Init()
{
	//打开文件
	QFile File;
	File.setFileName(QDir::currentPath() + "/Axis_Position.json");
	File.open(QFile::ReadOnly | QIODevice::Text);
	//读取文件
	QByteArray Data = File.readAll();
	//转换JSON
	QJsonDocument DB_Doc(QJsonDocument::fromJson(Data));
	m_Work_Program = DB_Doc.object();
	//关闭文件
	File.close();
	if (m_Work_Program.isEmpty()) {
		emit Status("巡检配置文件为空，请检查配置文件后，重新启动程序");
	}
	else {
		emit Status("巡检配置文加载完成，等待巡检任务");
	}

	//开始循环,系统启动
	emit System_Scan_Done();
}
//开始任务
void c_Work_Remote::Start_Cmd(QJsonObject object)
{
	QString Cmd_Name = object.value("Cmd_Name").toString();
	/*{
		"Status": "Start",
		"Car_Num": "1246",
		"Car_Type_Num": "HXD11246",
		"hm": "1230",
		"Cmd_Name": "Work_Start",
		"Begin_Time": "202605271230",
		"Name": ["车号识别"],
		"ymd": "20260527",
		"Car_Type": "HXD1",
		"Checksum": "1779856284544",
		"Value": true,
		"Track_ID": "J9",
		"Head_Toward": "BA"
	} */
	if (Cmd_Name == "Work_Start") {
		m_Checksum = object.value("Checksum").toString();
		//未执行任务，要取消
		if (!c_Variable::getInstance().g_Work.Connected && !object.value("Value").toBool()) {
			emit Status("任务不存在");

			QJsonObject json;
			json.insert("Cmd_Name", "Work_Start");
			json.insert("Value", false);
			json.insert("Status", "Error");
			json.insert("Message", "任务不存在");
			json.insert("Checksum", m_Checksum);
			emit Write_Json(object);
			return;
		}
		//正在执行任务要取消
		if (c_Variable::getInstance().g_Work.Connected && !object.value("Value").toBool()) {
			emit Status("取消任务");

			c_Variable::getInstance().g_Work.Connected = object.value("Value").toBool();

			QJsonObject json;
			json.insert("Cmd_Name", "Work_Start");
			json.insert("Value", false);
			json.insert("Status", "Success");
			json.insert("Message", "取消任务");
			json.insert("Checksum", m_Checksum);
			emit Write_Json(json);
			return;
		}
		//正在执行任务，二次触发
		if (c_Variable::getInstance().g_Work.Connected && object.value("Value").toBool()) {
			emit Status("存在执行中任务");

			QJsonObject json;
			json.insert("Cmd_Name", "Work_Start");
			json.insert("Value", true);
			json.insert("Status", "Error");
			json.insert("Message", "存在执行中任务");
			json.insert("Checksum", m_Checksum);
			emit Write_Json(json);
			return;
		}
		//未执行任务，第一次触发
		if (!c_Variable::getInstance().g_Work.Connected && object.value("Value").toBool()) {
			c_Variable::getInstance().g_Work.Connected = object.value("Value").toBool();
		}
		if (object.value("Begin_Time") == QJsonValue::Undefined) {
			emit Status("巡检指令缺少Begin_Time");
			c_Variable::getInstance().g_Work.Connected = false;
			QJsonObject json;
			json.insert("Cmd_Name", "Work_Start");
			json.insert("Value", true);
			json.insert("Status", "Error");
			json.insert("Message", "巡检指令缺少Begin_Time");
			json.insert("Checksum", m_Checksum);
			emit Write_Json(json);
			return;
		}
		if (object.value("Track_ID") == QJsonValue::Undefined) {
			emit Status("巡检指令缺少Track_ID");
			c_Variable::getInstance().g_Work.Connected = false;
			QJsonObject json;
			json.insert("Cmd_Name", "Work_Start");
			json.insert("Value", true);
			json.insert("Status", "Error");
			json.insert("Message", "巡检指令缺少Track_ID");
			json.insert("Checksum", m_Checksum);
			emit Write_Json(json);
			return;
		}
		if (object.value("Car_Type") == QJsonValue::Undefined) {
			emit Status("巡检指令缺少Car_Type");
			c_Variable::getInstance().g_Work.Connected = false;
			QJsonObject json;
			json.insert("Cmd_Name", "Work_Start");
			json.insert("Value", true);
			json.insert("Status", "Error");
			json.insert("Message", "巡检指令缺少Car_Type");
			json.insert("Checksum", m_Checksum);
			emit Write_Json(json);
			return;
		}
		if (object.value("Car_Num") == QJsonValue::Undefined) {
			emit Status("巡检指令缺少Car_Num");
			c_Variable::getInstance().g_Work.Connected = false;
			QJsonObject json;
			json.insert("Cmd_Name", "Work_Start");
			json.insert("Value", true);
			json.insert("Status", "Error");
			json.insert("Message", "巡检指令缺少Car_Num");
			json.insert("Checksum", m_Checksum);
			emit Write_Json(json);
			return;
		}
		if (object.value("Head_Toward") == QJsonValue::Undefined) {
			emit Status("巡检指令缺少Head_Toward");
			c_Variable::getInstance().g_Work.Connected = false;
			QJsonObject json;
			json.insert("Cmd_Name", "Work_Start");
			json.insert("Value", true);
			json.insert("Status", "Error");
			json.insert("Message", "巡检指令缺少Toward");
			json.insert("Checksum", m_Checksum);
			emit Write_Json(json);
			return;
		}

		c_Variable::getInstance().g_Work.Begin_Time = object.value("Begin_Time").toString();//开始时间	
		c_Variable::getInstance().g_Work.Track_Id = object.value("Track_ID").toString();//股道号
		c_Variable::getInstance().g_Work.Car_Type= object.value("Car_Type").toString();
		c_Variable::getInstance().g_Work.Car_Num= object.value("Car_Num").toString();
		c_Variable::getInstance().g_Work.Head_Toward = object.value("Head_Toward").toString();

		if (m_Work_Program.value(c_Variable::getInstance().g_Work.Car_Type) == QJsonValue::Undefined) {
			emit Status("巡检程序中车型不存在" + c_Variable::getInstance().g_Work.Car_Type);
			c_Variable::getInstance().g_Work.Connected = false;
			return;
		}else {
			m_Current_Work = m_Work_Program.value(c_Variable::getInstance().g_Work.Car_Type).toObject();
			emit Status("巡检程序加载完毕，开始写入");
		}
	}		
	
	c_Work_Remote::wait_AutoEn_120();
	c_Work_Remote::wait_RunFunc_120("Func_1");


	c_Variable::getInstance().g_Work.Connected = false;
	emit Status("退出当前任务");
}
// 左机械臂自动使能
void c_Work_Remote::wait_AutoEn_120()
{
  QEventLoop loop;
  QObject::connect(this, &c_Work_Remote::Work_Disconnected, &loop, &QEventLoop::quit);
  QObject::connect(this, &c_Work_Remote::AutoEn_Done_120, &loop, &QEventLoop::quit);
  emit Status("左机械臂自动使能");
  emit AutoEn_120();
  if(!c_Variable::getInstance().g_Work.Connected){ return; }
  loop.exec();
  if(!c_Variable::getInstance().g_Work.Connected){ return; }
}
// 左机械臂自动下电
void c_Work_Remote::wait_AutoDn_120()
{
  QEventLoop loop;
  QObject::connect(this, &c_Work_Remote::Work_Disconnected, &loop, &QEventLoop::quit);
  QObject::connect(this, &c_Work_Remote::AutoDn_Done_120, &loop, &QEventLoop::quit);
  emit Status("左机械臂自动下电");
  emit AutoDn_120();
  if(!c_Variable::getInstance().g_Work.Connected){ return; }
  loop.exec();
  emit Status("左机械臂自动下电完成");
  if(!c_Variable::getInstance().g_Work.Connected){ return; }
}
// 右机械臂自动使能
void c_Work_Remote::wait_AutoEn_121()
{
  QEventLoop loop;
  QObject::connect(this, &c_Work_Remote::Work_Disconnected, &loop, &QEventLoop::quit);
  QObject::connect(this, &c_Work_Remote::AutoEn_Done_121, &loop, &QEventLoop::quit);
  emit Status("右机械臂自动使能");
  emit AutoEn_121();
  if(!c_Variable::getInstance().g_Work.Connected){ return; }
  loop.exec();
  emit Status("右机械臂自动使能完成");
  if(!c_Variable::getInstance().g_Work.Connected){ return; }
}
// 右机械臂自动下电
void c_Work_Remote::wait_AutoDn_121()
{
  QEventLoop loop;
  QObject::connect(this, &c_Work_Remote::Work_Disconnected, &loop, &QEventLoop::quit);
  QObject::connect(this, &c_Work_Remote::AutoDn_Done_121, &loop, &QEventLoop::quit);
  emit Status("右机械臂自动下电");
  emit AutoDn_121();
  if(!c_Variable::getInstance().g_Work.Connected){ return; }
  loop.exec();
  emit Status("右机械臂自动下电完成");
  if(!c_Variable::getInstance().g_Work.Connected){ return; }
}
//等待左机械臂运行脚本函数
void c_Work_Remote::wait_RunFunc_120(QString cmd)
{
  QEventLoop loop;
  QObject::connect(this, &c_Work_Remote::Work_Disconnected, &loop, &QEventLoop::quit);
  QObject::connect(this, &c_Work_Remote::Huayan_120_Moving, &loop, &QEventLoop::quit);
  emit Status("左机械臂运行脚本函数");
  emit RunFunc_120(cmd);
  if(!c_Variable::getInstance().g_Work.Connected){ return; }
  loop.exec();
  emit Status("左左机械臂运行脚本函数中");
  if(!c_Variable::getInstance().g_Work.Connected){ return; }
}
//等待右机械臂运行脚本函数
void c_Work_Remote::wait_RunFunc_121(QString cmd)
{
  QEventLoop loop;
  QObject::connect(this, &c_Work_Remote::Work_Disconnected, &loop, &QEventLoop::quit);
  QObject::connect(this, &c_Work_Remote::Huayan_121_Moving, &loop, &QEventLoop::quit);
  emit Status("右机械臂运行脚本函数");
  emit RunFunc_121(cmd);
  if(!c_Variable::getInstance().g_Work.Connected){ return; }
  loop.exec();
  emit Status("右机械臂运行脚本函数中");
  if(!c_Variable::getInstance().g_Work.Connected){ return; }
}
//等待同步采集完成
void c_Work_Remote::wait_Pre_Scan_Done()
{
   QString key_1 = QString("%1_%2%3_%4左")
		.arg(c_Variable::getInstance().g_Work.Car_Type)
		.arg(c_Variable::getInstance().g_Work.Carbox_Num)
		.arg(c_Variable::getInstance().g_Work.Bogie_Num)
		.arg(c_Variable::getInstance().g_Work.Axis_Num);

	QString key_2 = QString("%1_%2%3_%4右")
		.arg(c_Variable::getInstance().g_Work.Car_Type)
		.arg(c_Variable::getInstance().g_Work.Carbox_Num)
		.arg(c_Variable::getInstance().g_Work.Bogie_Num)
		.arg(c_Variable::getInstance().g_Work.Axis_Num);

	if (m_Current_Work.value(key_1) == QJsonValue::Undefined) {
		emit Status("加载左机械臂程序未准备好：" + key_1);
		return;
	}
	if (m_Current_Work.value(key_2) == QJsonValue::Undefined) {
		emit Status("加载右机械臂程序未准备好：" + key_2);
		return;
	}

	emit Status("加载左机械臂程序：" + key_1 + "->" + m_Current_Work.value(key_1).toString());
	emit Status("加载右机械臂程序：" + key_2 + "->" + m_Current_Work.value(key_2).toString());

	QEventLoop loop;
  	QObject::connect(this, &c_Work_Remote::Work_Disconnected, &loop, &QEventLoop::quit);
  	QObject::connect(this, &c_Work_Remote::Pre_Scan_Done, &loop, &QEventLoop::quit);

	c_Work_Remote::wait_RunFunc_120(m_Current_Work.value(key_1).toString());
	c_Work_Remote::wait_RunFunc_121(m_Current_Work.value(key_2).toString());

	if(!c_Variable::getInstance().g_Work.Connected){ return; }
	loop.exec();

	emit Status("右左机械臂同步完成");
	if(!c_Variable::getInstance().g_Work.Connected){ return; }
}

