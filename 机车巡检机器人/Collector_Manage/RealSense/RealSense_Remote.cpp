#include "RealSense_Remote.h"

c_RealSense_Remote::c_RealSense_Remote(QObject* parent): c_Object(parent)
{
	
}

c_RealSense_Remote::~c_RealSense_Remote()
{
	c_RealSense_Remote::Disconnect();
}

void c_RealSense_Remote::Init()
{
	m_buffer.reserve(1280 * 720);
	emit Status("相机线程启动");
}

void c_RealSense_Remote::Connect()
{
	if (c_Variable::getInstance().g_RealSense.device_connected) {
		emit Status("相机已连接，无需再次连接");
		return;
	}
	emit Status("相机连接中");
	// 创建RealSense上下文（设备枚举入口）
	rs2::context ctx;
	// 查询所有连接的设备
	auto devices = ctx.query_devices();
	int device_count = static_cast<int>(devices.size());
	if (device_count == 0) {
		emit Status("未检测到任何RealSense设备");
		c_Variable::getInstance().g_RealSense.device_connected = false;
		return;
	}
	emit Status(QString("检测到%1个RealSense设备").arg(device_count));
	for (int i = 0; i < device_count; ++i) {
		auto dev = devices[i];
		// 获取当前设备的序列号（转换为QString）
		QString serial = QString::fromStdString(dev.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
		QString name = QString::fromStdString(dev.get_info(RS2_CAMERA_INFO_NAME));
		if (serial == c_Variable::getInstance().g_RealSense.serial) {
			m_device = dev;
			c_Variable::getInstance().g_RealSense.device_connected = true;
			emit Status(QString("匹配设备[%1]: Serial=%2, Name=%3")
				.arg(i).arg(serial).arg(name));
			break;
		}
	}
	if (!c_Variable::getInstance().g_RealSense.device_connected) {
		emit Status(QString("未找到序列号为%1的设备")
			.arg(c_Variable::getInstance().g_RealSense.serial));
		c_Variable::getInstance().g_RealSense.device_connected = false;
		return ;
	}
	if (c_Variable::getInstance().g_RealSense.device_connected) {
		// 创建RealSense配置对象
		rs2::config cfg;
		// 指定目标设备
		cfg.enable_device(c_Variable::getInstance().g_RealSense.serial.toStdString());
		// 启用深度流（1280×720，16位深度，30fps）
		cfg.enable_stream(RS2_STREAM_DEPTH, 1280, 720, RS2_FORMAT_Z16, 30);
		// 启用彩色流（1280×720，BGR8，30fps）
		cfg.enable_stream(RS2_STREAM_COLOR, 1280, 720, RS2_FORMAT_BGR8, 30);
		try {
			// 启动数据管道
			m_profile = m_pipeline.start(cfg);
			c_Variable::getInstance().g_RealSense.device_connected = true;
			emit Status("连接成功1280*720@30fps");
			c_Variable::getInstance().writeMessage(1, "Connect_Done");

			QTimer::singleShot(3000, this, &c_RealSense_Remote::Capture);
		}
		catch (const rs2::error& e) {
			// 捕获RealSense SDK异常
			c_Variable::getInstance().g_RealSense.device_connected = false;
			emit Status(QString("启动失败: %1").arg(QString::fromStdString(e.what())));
			c_Variable::getInstance().writeMessage(1, "Disconnect_Done");
		}
	}
}

void c_RealSense_Remote::Disconnect()
{
	if (!c_Variable::getInstance().g_RealSense.device_connected) {
		emit Status(QStringLiteral("相机未连接，无需断开"));
		return;
	}
	emit Status(QStringLiteral("正在断开相机连接..."));
	try {
		// 检查管道是否处于活动状态
		if (m_pipeline.get_active_profile()) {
			// 停止管道
			m_pipeline.stop();
			emit Status(QStringLiteral("Pipeline已停止"));
		}
	}
	catch (const rs2::error& e) {
		// 设备可能已物理断开，属于正常情况
		QString error_msg = QString::fromStdString(e.what());
		emit Status(QStringLiteral("Pipeline停止异常: %1").arg(error_msg));
	}
	// 重置RealSense SDK对象
	m_profile = rs2::pipeline_profile(); 
	m_device = rs2::device();           
	c_Variable::getInstance().g_RealSense.device_connected = false;
	emit Status("断开成功");
	c_Variable::getInstance().writeMessage(1, "Disconnect_Done");
}

void c_RealSense_Remote::Capture()
{
	if (!c_Variable::getInstance().g_RealSense.device_connected) { return; }
	// 获取帧数据集（深度+彩色）
	rs2::frameset frames;
	// 非阻塞等待帧（100ms超时）
	if (!m_pipeline.try_wait_for_frames(&frames,100)) {
		emit Status("帧获取超时（100ms）");
		return;
	}

	// 对齐深度图到彩色图坐标系（关键：确保2D/3D像素对应）
	frames = m_align.process(frames);
	// 获取深度帧和彩色帧
	auto depth = frames.get_depth_frame();
	auto color = frames.get_color_frame();
	// 任一帧无效则返回失败
	if (!depth || !color) {
		emit Status("无效帧（深度/彩色缺失）");
		return;
	}
	// 1280
	int width = color.get_width();   
	// 720
	int height = color.get_height(); 
	
	// 点云生成器
	rs2::pointcloud pc;      
	// 纹理映射：将彩色帧映射到点云
	pc.map_to(color);      
	// 生成点云（单位：米）
	rs2::points points = pc.calculate(depth);

	// 获取顶点数据（3D坐标，单位：米）
	const rs2::vertex* vertices = points.get_vertices();  
	// UV纹理坐标（归一化0.0~1.0）
	const rs2::texture_coordinate* tex_coords = points.get_texture_coordinates(); 
	//bgr数据
	const uint8_t* bgr_data = (const uint8_t*)color.get_data();
	//清空并重置缓存（保留预分配内存，避免重建）
	m_buffer.clear();
	m_buffer.resize(height * width);
	// 使用纹理坐标精确采样（避免2D-3D映射错位）
	for (int i = 0; i < height * width; ++i) {
		// 获取3D坐标（单位：米）
		const rs2::vertex& v = vertices[i];
		// 纹理坐标 → 像素坐标（精确采样 + 边界保护）
		int px = static_cast<int>(tex_coords[i].u * width + 0.5f);
		int py = static_cast<int>(tex_coords[i].v * height + 0.5f);
		px = qBound(0, px, width - 1);
		py = qBound(0, py, height - 1);

		int bgr_idx = (py * width + px) * 3;
		m_buffer[i].r = bgr_data[bgr_idx + 2]; 
		m_buffer[i].g = bgr_data[bgr_idx + 1];  
		m_buffer[i].b = bgr_data[bgr_idx + 0];  
		m_buffer[i].a = 255;     

		// XYZ坐标（单位：米，PCL/VTK标准）
		m_buffer[i].x = v.x;
		m_buffer[i].y = v.y;
		m_buffer[i].z = v.z;
	}
	emit Point_Scan(m_buffer);
}
