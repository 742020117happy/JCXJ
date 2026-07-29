#pragma once
#include "RGV_Client.h"

class c_RGV_Remote : public c_Object
{
	Q_OBJECT
public:
	explicit c_RGV_Remote(QObject *parent = nullptr);
	virtual ~c_RGV_Remote();

	public slots:
	void Init();//初始化
	void Connect();//连接
	void Connect_Loop();//循环

signals:
	void Connect_Device(QString ip, int port);//连接到服务器
	void Disconnect_Device();//断开连接
	void Set_Working();//工作状态
	void Set_Default();//非工作状态
	void Write_Coils(int addr, int size, quint16 value);//写线圈
	void Write_HoldingRegisters(int addr, int size);//写输入寄存器
	void Read_Coils(int addr, int size);//读线圈
	void Read_HoldingRegisters(int addr, int size);//读输入寄存器
	void Read_DiscreteInputs(int addr, int size); //读离散输入
	void Read_InputRegisters(int addr, int size);//读输入寄存器

private:
	QThread *m_RGV_Remote_Thread;//线程
	c_RGV_Client *m_RGV_Remote;//对象

	QElapsedTimer m_Time;//计时器
	int m_FPS = 0;//帧率计数

	int m_Coils_Addr;//线圈起始地址
	int m_Coils_Size;//线圈数据长度
	int m_Read_Coils_Count;//读线圈长度
	int m_DiscreteInputs_Addr;//离散起始地址
	int m_DiscreteInputs_Size;//离散数据长度
	int m_Read_DiscreteInputs_Count;//读离散次数
	int m_InputRegisters_Addr;//输入起始地址
	int m_InputRegisters_Size;//输入数据长度
	int m_Read_InputRegisters_Count;//读输入次数
	int m_HoldingRegisters_Addr;//保持起始地址
	int m_HoldingRegisters_Size;//保持数据长度
	int m_Read_HoldingRegisters_Count;//读保持次数

	float m_dataFloat = 0.00f; //浮点数缓冲区
	quint32 m_data32Bits = 0; //32位整型缓冲区
	quint16 m_high16Bits = 0; //32位高16位缓冲区
	quint16 m_low16Bits = 0;  //32位低16位缓冲区
	quint16 m_data16Bits = 0; //16位整型缓冲区

	bool m_Set_Coils[1000] = { false };
	bool m_Reset_Coils[1000] = { false };
	bool m_Set_HoldingRegisters[1000] = { false };

	private slots:
	void Connect_Done();//连接成功
	void Disconnect_Done();

	void Write_Coils_Done();//读线圈
	void Write_HoldingRegisters_Done();//读保持寄存器
	void Read_Coils_Done();//读线圈
	void Read_HoldingRegisters_Done();//读输入寄存器
	void Read_DiscreteInputs_Done();//读离散输入
	void Read_InputRegisters_Done();//读输入寄存器	

	void Read_DiscreteInputs_Action();//处理读到的离散
	void Read_InputRegisters_Action();//处理读到的输入
	bool Write_Coils_Action();//处理线圈写入
	bool Write_HoldingRegisters_Action();//处理保持寄存器写入
	void Read_Coils_Action();//处理线圈读取
	void Read_HoldingRegisters_Action();//处理保持寄存器读取
};
