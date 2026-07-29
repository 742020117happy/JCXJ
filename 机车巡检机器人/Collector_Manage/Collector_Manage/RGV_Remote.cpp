#include "RGV_Remote.h"
/*************************************************************************************************************************************************
**Function:构造函数
*************************************************************************************************************************************************/
c_RGV_Remote::c_RGV_Remote(QObject *parent) : c_Object(parent)
{

}
/*************************************************************************************************************************************************
**Function:析构函数
*************************************************************************************************************************************************/
c_RGV_Remote::~c_RGV_Remote()
{
	if (m_RGV_Remote_Thread->isRunning()) {
		//线程中断
		m_RGV_Remote_Thread->requestInterruption();
		//线程退出
		m_RGV_Remote_Thread->quit();
		//线程等待
		m_RGV_Remote_Thread->wait();
	}
	emit Status("RGV遥控：子类正常析构");
}
/*************************************************************************************************************************************************
**Function:初始化函数
*************************************************************************************************************************************************/
void c_RGV_Remote::Init()
{
	//实例化
	m_RGV_Remote = new c_RGV_Client;
	m_RGV_Remote_Thread = new QThread;
	m_RGV_Remote->moveToThread(m_RGV_Remote_Thread);
	//初始化数据交换层
	QObject::connect(m_RGV_Remote_Thread, &QThread::started, m_RGV_Remote, &c_RGV_Client::Init);
	QObject::connect(m_RGV_Remote_Thread, &QThread::finished, m_RGV_Remote, &c_RGV_Client::deleteLater);
	//连接设备
	QObject::connect(this, &c_RGV_Remote::Connect_Device, m_RGV_Remote, &c_RGV_Client::Connect_Device);
	QObject::connect(this, &c_RGV_Remote::Disconnect_Device, m_RGV_Remote, &c_RGV_Client::Disconnect_Device);
	//循环连接设备
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Connect_Loop, this, &c_RGV_Remote::Connect_Loop);
	//读数据
	QObject::connect(this, &c_RGV_Remote::Read_Coils, m_RGV_Remote, &c_RGV_Client::Read_Coils);
	QObject::connect(this, &c_RGV_Remote::Read_HoldingRegisters, m_RGV_Remote, &c_RGV_Client::Read_HoldingRegisters);
	QObject::connect(this, &c_RGV_Remote::Read_DiscreteInputs, m_RGV_Remote, &c_RGV_Client::Read_DiscreteInputs);
	QObject::connect(this, &c_RGV_Remote::Read_InputRegisters, m_RGV_Remote, &c_RGV_Client::Read_InputRegisters);
	//写数据
	QObject::connect(this, &c_RGV_Remote::Write_Coils, m_RGV_Remote, &c_RGV_Client::Write_Coils);
	QObject::connect(this, &c_RGV_Remote::Write_HoldingRegisters, m_RGV_Remote, &c_RGV_Client::Write_HoldingRegisters);
	//设备状态改变
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Connect_Done, this, &c_RGV_Remote::Set_Working);
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Disconnect_Done, this, &c_RGV_Remote::Set_Default);
	//读到消息
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Write_Coils_Done, this, &c_RGV_Remote::Write_Coils_Done);
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Write_HoldingRegisters_Done, this, &c_RGV_Remote::Write_HoldingRegisters_Done);
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Read_Coils_Done, this, &c_RGV_Remote::Read_Coils_Done);
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Read_HoldingRegisters_Done, this, &c_RGV_Remote::Read_HoldingRegisters_Done);
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Read_DiscreteInputs_Done, this, &c_RGV_Remote::Read_DiscreteInputs_Done);
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Read_InputRegisters_Done, this, &c_RGV_Remote::Read_InputRegisters_Done);

	QObject::connect(m_RGV_Remote, &c_RGV_Client::Write_Coils_Error, this, &c_RGV_Remote::Write_Coils_Done);
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Write_HoldingRegisters_Error, this, &c_RGV_Remote::Write_HoldingRegisters_Done);
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Read_Coils_Error, this, &c_RGV_Remote::Read_Coils_Done);
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Read_HoldingRegisters_Error, this, &c_RGV_Remote::Read_HoldingRegisters_Done);
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Read_DiscreteInputs_Error, this, &c_RGV_Remote::Read_DiscreteInputs_Done);
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Read_InputRegisters_Error, this, &c_RGV_Remote::Read_InputRegisters_Done);
	//向状态服务写入状态
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Connect_Done, this, &c_RGV_Remote::Connect_Done);
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Disconnect_Done, this, &c_RGV_Remote::Disconnect_Done);
	//提示信息
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Status, this, [=](int state) {emit Status("RGV遥控：" + Modbus_Status(state)); });
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Read_Coils_Error, this, [=] {emit Status("RGV遥控：读线圈失败" ); });
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Read_DiscreteInputs_Error, this, [=] {emit Status("RGV遥控：读离散失败"); });
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Read_InputRegisters_Error, this, [=] {emit Status("RGV遥控：读输入寄存器失败"); });
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Read_HoldingRegisters_Error, this, [=] {emit Status("RGV遥控：读保持寄存器失败"); });
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Write_Coils_Error, this, [=] {emit Status("RGV遥控：写线圈失败"); });
	QObject::connect(m_RGV_Remote, &c_RGV_Client::Write_HoldingRegisters_Error, this, [=] {emit Status("RGV遥控：写保持寄存器失败"); });
	//启动线程
	m_RGV_Remote_Thread->start();

	QTimer::singleShot(3000, this, &c_RGV_Remote::Connect);
}
/*************************************************************************************************************************************************
**Function:连接设备
*************************************************************************************************************************************************/
void c_RGV_Remote::Connect()
{
	QString ip = c_Variable::getInstance().g_Communicate_DB.value("RGV_Ip").toString();
	int port = c_Variable::getInstance().g_Communicate_DB.value("RGV_Port").toInt();
	emit Status("RGV遥控：" + ip + "：" + QString::number(port));
	emit Connect_Device(ip, port);
}
void c_RGV_Remote::Connect_Loop()
{
	if ( c_Variable::getInstance().g_RGV.Connected) { return; }
	QTimer::singleShot(3000, this, &c_RGV_Remote::Connect);
}
void c_RGV_Remote::Connect_Done()
{
	m_Time.start();
	c_Variable::getInstance().g_RGV.Connected = true;
	//更新轮询参数
	m_Coils_Addr = c_Variable::getInstance().g_Communicate_DB.value("Write_Coils_Addr").toInt();
	m_Coils_Size = c_Variable::getInstance().g_Communicate_DB.value("Write_Coils_Size").toInt();
	m_Read_Coils_Count = 1;
	if (m_Coils_Size > 1000) {
		emit Read_Coils(m_DiscreteInputs_Addr, 1000);
		return;
	}
	emit Read_Coils(m_Coils_Addr, m_Coils_Size);
}
void c_RGV_Remote::Disconnect_Done()
{
	c_Variable::getInstance().g_RGV.Connected = false;
}
/*************************************************************************************************************************************************
**处理读到的离散数据
**处理读到的保持寄存器数据
**判断是否写线圈
**判断是否写保持寄存器
************************************************************************************************************************************************/
void c_RGV_Remote::Read_DiscreteInputs_Action()
{

}
void c_RGV_Remote::Read_InputRegisters_Action()
{

}
bool c_RGV_Remote::Write_Coils_Action()
{
	m_Coils_Size = c_Variable::getInstance().g_Communicate_DB.value("Write_Coils_Size").toInt();
	for (int i = 0; i < m_Coils_Size; i++) {
		if (m_Set_Coils[i]) {
			emit Status("RGV遥控：线圈置位" + QString::number(i));
			emit Write_Coils(i, 1, 1);
			return true;
		}
		if (m_Reset_Coils[i]) {
			emit Status("RGV遥控：线圈复位" + QString::number(i));
			emit Write_Coils(i, 1, 0);
			return true;
		}
	}
	return false;
}
bool c_RGV_Remote::Write_HoldingRegisters_Action()
{
	m_HoldingRegisters_Size = c_Variable::getInstance().g_Communicate_DB.value("Write_HoldingRegisters_Size").toInt();
	for (int i = 0; i < m_HoldingRegisters_Size; i++) {
		//少数为16位写入，多数为32位写入
		if (m_Set_HoldingRegisters[i]) {
			emit Status("RGV遥控：写寄存器" + QString::number(i));
			switch (i) {
			case 131:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 132:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 133:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 134:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 135:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 136:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 137:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 138:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 139:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 140:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 141:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 142:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 143:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 144:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 145:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 146:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 147:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 148:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 149:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 150:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 151:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 152:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 153:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 154:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 155:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 156:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 157:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 158:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 167:
				emit Write_HoldingRegisters(i, 1);
				return true;
			case 170:
				emit Write_HoldingRegisters(172, 64);
				emit Status("RGV遥控：轴数据矫正起始172，长度64");
				m_Set_HoldingRegisters[i] = false;
				return true;
			case 171:
				emit Write_HoldingRegisters(236, 64);
				emit Status("RGV遥控：轴数据矫正起始236，长度64");
				m_Set_HoldingRegisters[i] = false;
				return true;
			case 304:
				emit Write_HoldingRegisters(306, 64);
				emit Status("RGV遥控：轴数据矫正起始306，长度64");
				m_Set_HoldingRegisters[i] = false;
				return true;
			case 305:
				emit Write_HoldingRegisters(370, 64);
				emit Status("RGV遥控：轴数据矫正起始370，长度64");
				m_Set_HoldingRegisters[i] = false;
				return true;
			default:
				emit Write_HoldingRegisters(i, 2);
				return true;
			}
		}
	}
	return false;
}
void c_RGV_Remote::Read_Coils_Action()
{
	m_Coils_Size = c_Variable::getInstance().g_Communicate_DB.value("Write_Coils_Size").toInt();
	for (int i = 0; i < m_Coils_Size; i++) {
		if (m_Set_Coils[i] && c_Variable::getInstance().g_RGV.Coils[i]) {	
			emit Status("RGV遥控：线圈置位完成" + QString::number(i));
			m_Set_Coils[i] = false;
		}
		if (m_Reset_Coils[i] && !c_Variable::getInstance().g_RGV.Coils[i]) {
			emit Status("RGV遥控：线圈复位完成" + QString::number(i));
			m_Reset_Coils[i] = false;
		}
	}
}
void c_RGV_Remote::Read_HoldingRegisters_Action()
{
	m_HoldingRegisters_Size = c_Variable::getInstance().g_Communicate_DB.value("Write_HoldingRegisters_Size").toInt();
	for (int i = 0; i < m_HoldingRegisters_Size; i++) {
		//少数为16位写入，多数为32位写入
		if (m_Set_HoldingRegisters[i]) {
			emit Status("RGV遥控：写寄存器完成" + QString::number(i));
			switch (i) {
			case 131:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 132:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 133:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 134:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 135:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 136:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 137:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 138:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 139:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 140:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 141:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 142:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 143:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 144:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 145:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 146:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 147:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 148:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 149:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 150:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 151:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 152:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 153:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 154:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 155:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 156:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 157:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 158:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 167:
				if (c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) {
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			case 170:
				//开环写入无操作
				break;
			case 171:
				//开环写入无操作
				break;
			case 304:
				//开环写入无操作
				break;
			case 305:
				//开环写入无操作
				break;
			default:
				if ((c_Variable::getInstance().g_RGV.read_HoldingRegisters[i] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i]) &&
					(c_Variable::getInstance().g_RGV.read_HoldingRegisters[i+1] == c_Variable::getInstance().g_RGV.write_HoldingRegisters[i+1]) ){
					m_Set_HoldingRegisters[i] = false;
				}
				break;
			}
		}
	}
}
/*************************************************************************************************************************************************
**轮询操作:每次读取数据完成中都会判断是否有数据操作
**无数据操作:读线圈完成->读保持寄存器完成->读离散输入完成->读输入寄存器完成------------------------------------->读离散输入完成->读输入寄存器完成
**有数据操作:有写线圈------->写线圈完成------->读线圈完成------->直到数据操作完成->读线圈完成->读保持寄存器完成->读离散输入完成->读输入寄存器完成
             有写保持寄存器->写保持寄存器完成->读保持寄存器完成->直到数据操作完成------------->读保持寄存器完成->读离散输入完成->读输入寄存器完成
************************************************************************************************************************************************/
void c_RGV_Remote::Read_Coils_Done()
{
	c_Variable::getInstance().g_RGV.Ready = !c_Variable::getInstance().g_RGV.Ready;
	//当前需要读取的线圈起始地址 = 读取次数 * 每次读取长度 (0~999=1000)
	int Read_Coils_Addr = m_Read_Coils_Count * 1000;
	//当前需要读取的线圈数据长度 = 总数据长度（1000） - 当前起始地址（0）
	int Read_Coils_Size = m_Coils_Size - Read_Coils_Addr;
	//如果当前地址 < 总长度 且 还需读取长度大于124 则 从当前地址开始再读124个长度 计数加1 并返回
	if (Read_Coils_Addr < m_Coils_Size && Read_Coils_Size > 1000) {
		emit Read_Coils(Read_Coils_Addr, 1000);
		m_Read_Coils_Count += 1;
		return;
	}
	//如果当前地址 < 总长度 且 还需读取长度小于124 则 从当前地址开始读完剩余数据 并返回
	if (Read_Coils_Addr < m_Coils_Size && Read_Coils_Size < 1000) {
		emit Read_Coils(Read_Coils_Addr, Read_Coils_Size);
		m_Read_Coils_Count += 1;
		return;
	}
	//判断写入是否有效
	Read_Coils_Action();
	//判断是否有数据操作
	if (Write_Coils_Action()) { return; }
	if (Write_HoldingRegisters_Action()) { return; }
	//更新轮询参数
	m_HoldingRegisters_Addr = c_Variable::getInstance().g_Communicate_DB.value("Write_HoldingRegisters_Addr").toInt();
	m_HoldingRegisters_Size = c_Variable::getInstance().g_Communicate_DB.value("Write_HoldingRegisters_Size").toInt();
	m_Read_HoldingRegisters_Count = 1;
	if (m_HoldingRegisters_Size >= 124) {
		emit Read_HoldingRegisters(m_HoldingRegisters_Addr, 124);
		return;
	}
	emit Read_HoldingRegisters(m_HoldingRegisters_Addr, m_HoldingRegisters_Size);
}
void c_RGV_Remote::Write_Coils_Done()
{
	c_Variable::getInstance().g_RGV.Ready = !c_Variable::getInstance().g_RGV.Ready;
	//更新轮询参数
	m_Coils_Addr = c_Variable::getInstance().g_Communicate_DB.value("Write_Coils_Addr").toInt();
	m_Coils_Size = c_Variable::getInstance().g_Communicate_DB.value("Write_Coils_Size").toInt();
	m_Read_Coils_Count = 1;
	if (m_Coils_Size > 1000) {
		emit Read_Coils(m_DiscreteInputs_Addr, 1000);
		return;
	}
	emit Read_Coils(m_Coils_Addr, m_Coils_Size);
}
void c_RGV_Remote::Read_HoldingRegisters_Done()
{
	c_Variable::getInstance().g_RGV.Ready = !c_Variable::getInstance().g_RGV.Ready;
	//当前需要读取的保持寄存器起始地址 = 读取次数 * 每次读取长度 (0~123=124)
	int Read_HoldingRegisters_Addr = m_Read_HoldingRegisters_Count * 124;
	//当前需要读取的保持寄存器数据长度 = 总数据长度（250） - 当前起始地址（124）
	int Read_HoldingRegisters_Size = m_HoldingRegisters_Size - Read_HoldingRegisters_Addr;
	//如果当前地址 < 总长度 且 还需读取长度大于124 则 从当前地址开始再读124个长度 计数加1 并返回
	if (Read_HoldingRegisters_Addr < m_HoldingRegisters_Size && Read_HoldingRegisters_Size >= 124) {
		emit Read_HoldingRegisters(Read_HoldingRegisters_Addr, 124);
		m_Read_HoldingRegisters_Count += 1;
		return;
	}
	//如果当前地址 < 总长度 且 还需读取长度小于124 则 从当前地址开始读完剩余数据 并返回
	if (Read_HoldingRegisters_Addr < m_HoldingRegisters_Size && Read_HoldingRegisters_Size < 124) {
		emit Read_HoldingRegisters(Read_HoldingRegisters_Addr, Read_HoldingRegisters_Size);
		m_Read_HoldingRegisters_Count += 1;
		return;
	}
	//判断写入是否有效
	Read_HoldingRegisters_Action();
	//判断是否有数据操作
	if (Write_Coils_Action()) { return; }
	if (Write_HoldingRegisters_Action()) { return; }

	//读离散输入更新轮询参数
	m_DiscreteInputs_Addr = c_Variable::getInstance().g_Communicate_DB.value("Read_DiscreteInputs_Addr").toInt();
	m_DiscreteInputs_Size = c_Variable::getInstance().g_Communicate_DB.value("Read_DiscreteInputs_Size").toInt();
	m_Read_DiscreteInputs_Count = 1;
	if (m_DiscreteInputs_Size > 1000) {
		emit Read_DiscreteInputs(m_DiscreteInputs_Addr, 1000);
	}
	else {
		emit Read_DiscreteInputs(m_DiscreteInputs_Addr, m_DiscreteInputs_Size);
	}	
}
void c_RGV_Remote::Read_DiscreteInputs_Done()
{
	if (m_FPS == 100) {
		c_Variable::getInstance().g_RGV.FPS = 100000 / m_Time.restart();
		m_FPS = 0;
	}
	m_FPS += 1;
	c_Variable::getInstance().g_RGV.Ready = !c_Variable::getInstance().g_RGV.Ready;
	//当前需要读取的离散起始地址 = 读取次数 * 每次读取长度 (0~999=1000)
	int Read_DiscreteInputs_Addr = m_Read_DiscreteInputs_Count * 1000;
	//当前需要读取的离散数据长度 = 总数据长度（1000） - 当前起始地址（0）
	int Read_DiscreteInputs_Size = m_DiscreteInputs_Size - Read_DiscreteInputs_Addr;
	//如果当前地址 < 总长度 且 还需读取长度大于124 则 从当前地址开始再读124个长度 计数加1 并返回
	if (Read_DiscreteInputs_Addr < m_DiscreteInputs_Size && Read_DiscreteInputs_Size > 1000) {
		emit Read_DiscreteInputs(Read_DiscreteInputs_Addr, 1000);
		m_Read_DiscreteInputs_Count += 1;
		return;
	}
	//如果当前地址 < 总长度 且 还需读取长度小于124 则 从当前地址开始读完剩余数据 并返回
	if (Read_DiscreteInputs_Addr < m_DiscreteInputs_Size && Read_DiscreteInputs_Size < 1000) {
		emit Read_DiscreteInputs(Read_DiscreteInputs_Addr, Read_DiscreteInputs_Size);
		m_Read_DiscreteInputs_Count += 1;
		return;
	}
	//处理读到的数据
	Read_DiscreteInputs_Action();
	//判断是否有数据操作
	if (Write_Coils_Action()) { return; }
	if (Write_HoldingRegisters_Action()){ return; }
	//如果线圈不改变则不读线圈，直接读输入寄存器，更新轮询参数
	m_InputRegisters_Addr = c_Variable::getInstance().g_Communicate_DB.value("Read_InputRegisters_Addr").toInt();
	m_InputRegisters_Size = c_Variable::getInstance().g_Communicate_DB.value("Read_InputRegisters_Size").toInt();
	//如果读取的数据长度大于124则读124个数据,并初始化计数
	m_Read_InputRegisters_Count = 1;
	if (m_InputRegisters_Size >= 124) {
		emit Read_InputRegisters(m_InputRegisters_Addr, 124);
		return;
	}
	emit Read_InputRegisters(m_InputRegisters_Addr, m_InputRegisters_Size);
}
void c_RGV_Remote::Read_InputRegisters_Done()
{
	c_Variable::getInstance().g_RGV.Ready = !c_Variable::getInstance().g_RGV.Ready;
	//当前需要读取的输入寄存器起始地址 = 读取次数 * 每次读取长度 (0~123=124)
	int Read_InputRegisters_Addr = m_Read_InputRegisters_Count * 124;
	//当前需要读取的输入寄存器数据长度 = 总数据长度（250） - 当前起始地址（124）
	int Read_InputRegisters_Size = m_InputRegisters_Size - Read_InputRegisters_Addr;
	//如果当前地址 < 总长度 且 还需读取长度大于124 则 从当前地址开始再读124个长度 计数加1 并返回

	if (Read_InputRegisters_Addr < m_InputRegisters_Size && Read_InputRegisters_Size >= 124) {
		emit Read_InputRegisters(Read_InputRegisters_Addr, 124);
		m_Read_InputRegisters_Count += 1;
		return;
	}
	//如果当前地址 < 总长度 且 还需读取长度小于124 则 从当前地址开始读完剩余数据 并返回
	if (Read_InputRegisters_Addr < m_InputRegisters_Size && Read_InputRegisters_Size < 124) {
		emit Read_InputRegisters(Read_InputRegisters_Addr, Read_InputRegisters_Size);
		m_Read_InputRegisters_Count += 1;
		return;
	}
	//处理读到的数据
	Read_InputRegisters_Action();
	//判断是否有数据操作
	if (Write_Coils_Action()) { return; }
	if (Write_HoldingRegisters_Action()) { return; }
	//如果保持寄存器没有改变，则直接读离散输入
	//更新轮询参数
	m_DiscreteInputs_Addr = c_Variable::getInstance().g_Communicate_DB.value("Read_DiscreteInputs_Addr").toInt();
	m_DiscreteInputs_Size = c_Variable::getInstance().g_Communicate_DB.value("Read_DiscreteInputs_Size").toInt();
	m_Read_DiscreteInputs_Count = 1;
	if (m_DiscreteInputs_Size > 1000) {
		emit Read_DiscreteInputs(m_DiscreteInputs_Addr, 1000);
		return;
	}
	emit Read_DiscreteInputs(m_DiscreteInputs_Addr, m_DiscreteInputs_Size);
}
void c_RGV_Remote::Write_HoldingRegisters_Done()
{
	c_Variable::getInstance().g_RGV.Ready = !c_Variable::getInstance().g_RGV.Ready;
	//更新轮询参数
	m_HoldingRegisters_Addr = c_Variable::getInstance().g_Communicate_DB.value("Write_HoldingRegisters_Addr").toInt();
	m_HoldingRegisters_Size = c_Variable::getInstance().g_Communicate_DB.value("Write_HoldingRegisters_Size").toInt();
	m_Read_HoldingRegisters_Count = 1;
	if (m_HoldingRegisters_Size >= 124) {
		emit Read_HoldingRegisters(m_HoldingRegisters_Addr, 124);
	}
	else {
		emit Read_HoldingRegisters(m_HoldingRegisters_Addr, m_HoldingRegisters_Size);
	}	
}
