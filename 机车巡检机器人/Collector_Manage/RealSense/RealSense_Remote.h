#pragma once  

#include "Variable.h"

class c_RealSense_Remote : public c_Object
{
    Q_OBJECT
public:
    explicit c_RealSense_Remote(QObject* parent = nullptr);
    ~c_RealSense_Remote() override;

    public slots:
    void Init();
	void Connect();
    void Disconnect();
    void Capture();
  
signals:
    void Point_Scan(QVector<pcl::PointXYZRGB> points);

private:
    // RealSense数据管道
    rs2::pipeline m_pipeline;   
    // 管道配置文件
    rs2::pipeline_profile m_profile;   
    // 设备句柄
    rs2::device m_device;            
    // 深度图→彩色图坐标对齐器
    rs2::align m_align{ RS2_STREAM_COLOR };
    // 点云缓存（预分配内存）
    QVector<pcl::PointXYZRGB> m_buffer;
};
