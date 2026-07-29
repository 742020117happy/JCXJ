#include "RGV_Client.h"

c_RGV_Client::c_RGV_Client(QObject *parent) : QObject(parent)
{

}

c_RGV_Client::~c_RGV_Client()
{
	c_RGV_Client::Disconnect_Device();
	m_ModbusDevice->deleteLater();
}

void c_RGV_Client::Init()
{
	//对象实列化
    m_ModbusDevice = new QModbusTcpClient;
    //状态改变
    QObject::connect(m_ModbusDevice, &QModbusTcpClient::stateChanged, this, &c_RGV_Client::State_Changed);
    //错误诊断
    QObject::connect(m_ModbusDevice, &QModbusTcpClient::errorOccurred, this, &c_RGV_Client::Status);
}

void c_RGV_Client::Connect_Device(QString ip, int port)
{
	//如果已连接，则返回
    if (m_ModbusDevice->state() != QModbusDevice::UnconnectedState){return;}

	if (m_Stop_Connect) {
		m_Stop_Connect = false;
		return;
	}
    //配置modbus tcp的连接参数 IP + Port
    m_ModbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, ip);
    m_ModbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
    //再设置从机无响应时的动作
    m_ModbusDevice->setTimeout(1000);//从设备回复信息的超时时间
    m_ModbusDevice->setNumberOfRetries(2);//重复发送次数
	m_ModbusDevice->connectDevice();
}

void c_RGV_Client::Disconnect_Device()
{
	if (!m_ModbusDevice) { return; }
	if (m_ModbusDevice->state() != QModbusDevice::ConnectedState) { return; }

	else {
		m_ModbusDevice->disconnectDevice();
	}
	m_Stop_Connect = true;
}

void c_RGV_Client::Read_Coils(int addr, int size)
{
	if (!m_ModbusDevice) { return; }
	if (m_ModbusDevice->state() != QModbusDevice::ConnectedState){ return; }

    if(size == 0){
		emit Read_Coils_Error();;
        return;
    }
    QModbusDataUnit ReadUnit(QModbusDataUnit::Coils, addr, size);
    auto *reply = m_ModbusDevice->sendReadRequest(ReadUnit, 1);
    if(reply)
    {
        if (!reply->isFinished()){
          QObject::connect(reply, &QModbusReply::finished, this, &c_RGV_Client::Read_Ready_Coils);
        }
        else{
            //reply->deleteLater();
            emit Read_Coils_Error();
            return;
        }
    }
    else{
        //reply->deleteLater();
        emit Read_Coils_Error();
        return;
    }
}

void c_RGV_Client::Read_DiscreteInputs(int addr, int size)
{
	if (!m_ModbusDevice) { return; }
	if (m_ModbusDevice->state() != QModbusDevice::ConnectedState) { return; }

    if(size == 0)
    {
        emit Read_DiscreteInputs_Error();
        return;
    }
    QModbusDataUnit ReadUnit(QModbusDataUnit::DiscreteInputs, addr, size);
    auto *reply = m_ModbusDevice->sendReadRequest(ReadUnit, 1);
    if(reply)
    {
        if (!reply->isFinished()){
          QObject::connect(reply, &QModbusReply::finished, this, &c_RGV_Client::Read_Ready_DiscreteInputs);
        }
        else{
            //reply->deleteLater();
            emit Read_DiscreteInputs_Error();
            return;
        }
    }
    else{
        //reply->deleteLater();
        emit Read_DiscreteInputs_Error();
        return;
    }
}

void c_RGV_Client::Read_InputRegisters(int addr, int size)
{
	if (!m_ModbusDevice) { return; }
	if (m_ModbusDevice->state() != QModbusDevice::ConnectedState) { return; }

    if(size == 0)
    {
        emit Read_InputRegisters_Error();
        return;
    }
    QModbusDataUnit ReadUnit(QModbusDataUnit::InputRegisters, addr, size);
    auto *reply = m_ModbusDevice->sendReadRequest(ReadUnit, 1);
    if(reply)
    {
        if (!reply->isFinished())
        {
          QObject::connect(reply, &QModbusReply::finished, this, &c_RGV_Client::Read_Ready_InputRegisters);
        }
        else
        {
            //reply->deleteLater();
            emit Read_InputRegisters_Error();
            return;
        }
    }
    else
    {
        //reply->deleteLater();
        emit Read_InputRegisters_Error();
        return;
    }
}

void c_RGV_Client::Read_HoldingRegisters(int addr, int size)
{
	if (!m_ModbusDevice) { return; }
	if (m_ModbusDevice->state() != QModbusDevice::ConnectedState) { return; }

    if(size == 0)
    {
        emit Read_HoldingRegisters_Error();
        return;
    }
    QModbusDataUnit ReadUnit(QModbusDataUnit::HoldingRegisters, addr, size);
    auto *reply = m_ModbusDevice->sendReadRequest(ReadUnit, 1);
    if(reply)
    {
        if (!reply->isFinished())
        {
          QObject::connect(reply, &QModbusReply::finished, this, &c_RGV_Client::Read_Ready_HoldingRegisters);
        }
        else
        {
            //reply->deleteLater();
            emit Read_HoldingRegisters_Error();
            return;
        }
    }
    else
    {
        //reply->deleteLater();
        emit Read_HoldingRegisters_Error();
        return;
    }
}

void c_RGV_Client::Write_Coils(int addr, int size, quint16 value)
{
	if (!m_ModbusDevice) { return; }
	if (m_ModbusDevice->state() != QModbusDevice::ConnectedState) { return; }

    if(size <= 0)
    {
        emit Write_HoldingRegisters_Error();
        return;
    }
    //写，地址，写多少位
    QModbusDataUnit writeUnit(QModbusDataUnit::Coils, addr, size);
    //该位置，数据
    for(int i=0; i<size; i++)
    {
        writeUnit.setValue(i, value);
    }
    //发送校验 1 代表设备地址
    if(auto *reply = m_ModbusDevice->sendWriteRequest(writeUnit, 1))
    {
        if (!reply->isFinished())
        {
            //如果接收到响应信息
            QObject::connect(reply, &QModbusReply::finished, this, [&, reply]()
            {
                if (reply->error() == QModbusDevice::NoError)
                {
                    //接收到的响应信无错误，发送完成信号
                    emit Write_Coils_Done();
                }
                else
                {
                    //reply->deleteLater();
                    emit Write_Coils_Error();
                    return;
                }
                //reply->deleteLater();
            });
        }
        else
        {
            //reply->deleteLater();
            emit Write_Coils_Error();
            return;
        }
    }
}

void c_RGV_Client::Write_HoldingRegisters(int addr, int size)
{
	if (!m_ModbusDevice) { return; }
	if (m_ModbusDevice->state() != QModbusDevice::ConnectedState) { return; }

    if(size <= 0)
    {
        emit Write_HoldingRegisters_Error();
        return;
    }

    //写，地址，写多少位
    QModbusDataUnit writeUnit(QModbusDataUnit::HoldingRegisters, addr, size);
    //该位置，数据
    for(int i=0; i<size; i++)
    {
        writeUnit.setValue(i, c_Variable::getInstance().g_RGV.write_HoldingRegisters[addr + i]);
    }
    //发送校验 1 代表设备地址
    if(auto *reply = m_ModbusDevice->sendWriteRequest(writeUnit, 1))
    {
        if (!reply->isFinished())
        {
            //如果接收到响应信息
            QObject::connect(reply, &QModbusReply::finished, this, [&, reply]()
            {
                if (reply->error() == QModbusDevice::NoError)
                {
                    //接收到的响应信无错误，发送完成信号
                    emit Write_HoldingRegisters_Done();
                }
                else
                {
                    //reply->deleteLater();
                    emit Write_HoldingRegisters_Error();
                    return;
                }
                //reply->deleteLater();
            });
        }
        else
        {
            //reply->deleteLater();
            emit Write_HoldingRegisters_Error();
            return;
        }
    }
}

void c_RGV_Client::State_Changed()
{
	if (!m_ModbusDevice) { return; }

    if(m_ModbusDevice->state() == QModbusDevice::ConnectedState)
    {
        emit Status(0);
        emit Connect_Done();
    }
    if(m_ModbusDevice->state() == QModbusDevice::UnconnectedState)
    {
        emit Status(9);
        emit Disconnect_Done();
		emit Connect_Loop();
    }
    if(m_ModbusDevice->state() == QModbusDevice::ConnectingState)
    {
        emit Status(10);
    }
    if(m_ModbusDevice->state() == QModbusDevice::ClosingState)
    {
        emit Status(11);
    }
}

void c_RGV_Client::Read_Ready_Coils()
{
	if (!m_ModbusDevice) { return; }
	if (m_ModbusDevice->state() != QModbusDevice::ConnectedState) { return; }

    auto *reply = qobject_cast<QModbusReply *>(sender());
    if (!reply){
        //reply->deleteLater();
        emit Read_Coils_Error();
        return;
    }
    //如果校验无误
    if (reply->error() == QModbusDevice::NoError){
        const QModbusDataUnit readUnit = reply->result();//获取结果
		std::memcpy(c_Variable::getInstance().g_RGV.Coils + readUnit.startAddress(), &readUnit.values()[0], sizeof(quint16)*readUnit.valueCount());
		
    }
    //如果校验有误
    else{
        //reply->deleteLater();
        emit Read_Coils_Error();
        return;
    }
    //reply->deleteLater(); //删除答复
    //发送信号
    emit Read_Coils_Done();//输出数据
}

void c_RGV_Client::Read_Ready_DiscreteInputs()
{
	if (!m_ModbusDevice) { return; }
	if (m_ModbusDevice->state() != QModbusDevice::ConnectedState) { return; }

    auto *reply = qobject_cast<QModbusReply *>(sender());
    if (!reply){
        //reply->deleteLater();
        emit Read_DiscreteInputs_Error();
        return;
    }
    //如果校验无误
    if (reply->error() == QModbusDevice::NoError){
        const QModbusDataUnit readUnit = reply->result();//获取结果
		std::memcpy(c_Variable::getInstance().g_RGV.DiscreteInputs + readUnit.startAddress(), &readUnit.values()[0], sizeof(quint16)*readUnit.valueCount());
    }
    //如果校验有误
    else{
        //reply->deleteLater();
        emit Read_DiscreteInputs_Error();
        return;
    }
    //reply->deleteLater(); //删除答复
    //发送信号
    emit Read_DiscreteInputs_Done();//输出数据
}

void c_RGV_Client::Read_Ready_InputRegisters()
{
	if (!m_ModbusDevice) { return; }
	if (m_ModbusDevice->state() != QModbusDevice::ConnectedState) { return; }

    auto *reply = qobject_cast<QModbusReply *>(sender());
    if (!reply){
        //reply->deleteLater();
        emit Read_InputRegisters_Error();
        return;
    }
    //如果校验无误
    if (reply->error() == QModbusDevice::NoError){
        const QModbusDataUnit readUnit = reply->result();//获取结果
		std::memcpy(c_Variable::getInstance().g_RGV.InputRegisters + readUnit.startAddress(), &readUnit.values()[0], sizeof(quint16)*readUnit.valueCount());
    }
    //如果校验有误
    else{
        //reply->deleteLater();
        emit Read_InputRegisters_Error();
        return;
    }
    //reply->deleteLater(); //删除答复
    //发送信号
    emit Read_InputRegisters_Done();//输出数据
}

void c_RGV_Client::Read_Ready_HoldingRegisters()
{
	if (!m_ModbusDevice) { return; }
	if (m_ModbusDevice->state() != QModbusDevice::ConnectedState) { return; }

    auto *reply = qobject_cast<QModbusReply *>(sender());
    if (!reply){
        //reply->deleteLater();
        emit Read_HoldingRegisters_Error();
        return;
    }
    //如果校验无误
    if (reply->error() == QModbusDevice::NoError){
        const QModbusDataUnit readUnit = reply->result();//获取结果
		std::memcpy(c_Variable::getInstance().g_RGV.read_HoldingRegisters + readUnit.startAddress(), &readUnit.values()[0], sizeof(quint16)*readUnit.valueCount());
    }
    //如果校验有误
    else{
        //reply->deleteLater();
        emit Read_HoldingRegisters_Error();
        return;
    }
    //reply->deleteLater(); //删除答复
    //发送信号
    emit Read_HoldingRegisters_Done();//输出数据
}
