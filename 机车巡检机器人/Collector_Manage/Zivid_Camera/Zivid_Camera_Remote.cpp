#include "Zivid_Camera_Remote.h"

c_Zivid_Camera_Remote::c_Zivid_Camera_Remote(QObject* parent): c_Object(parent)
{
	
}
// 析构函数
c_Zivid_Camera_Remote::~c_Zivid_Camera_Remote()
{
	Disconnect();
}
// 初始化
void c_Zivid_Camera_Remote::Init()
{
	emit Status("相机线程启动");
    QTimer::singleShot(3000, this, &c_Zivid_Camera_Remote::Connect);
}
// 点云转深度图（成员函数实现）
cv::Mat c_Zivid_Camera_Remote::pointCloudToCvZ(const Zivid::PointCloud &pointCloud)
{
    const size_t width = pointCloud.width();
    const size_t height = pointCloud.height();

    // ✅ Zivid::Array2D 无 empty()，直接判断尺寸即可
    if (width == 0 || height == 0) {
        return cv::Mat();
    }

    // ✅ 正确获取 Zivid::Array2D 的连续内存指针
    const auto zArray = pointCloud.copyData<Zivid::PointZ>();
    const Zivid::PointZ* rawData = zArray.data();
    const size_t totalPoints = width * height;

    cv::Mat depthMap(static_cast<int>(height), static_cast<int>(width), CV_32FC1);

    for (size_t i = 0; i < totalPoints; ++i) {
        float z = rawData[i].z;
        depthMap.at<float>(static_cast<int>(i / width), static_cast<int>(i % width)) =
            std::isfinite(z) ? z : std::numeric_limits<float>::quiet_NaN();
    }

    // 🎨 归一化到 0-255
    double minVal = std::numeric_limits<double>::max();
    double maxVal = std::numeric_limits<double>::lowest();

    for (int r = 0; r < depthMap.rows; ++r) {
        for (int c = 0; c < depthMap.cols; ++c) {
            float val = depthMap.at<float>(r, c);
            if (std::isfinite(val)) {
                minVal = std::min(minVal, static_cast<double>(val));
                maxVal = std::max(maxVal, static_cast<double>(val));
            }
        }
    }

    cv::Mat depthMapVis;
    if (std::isfinite(minVal) && std::isfinite(maxVal) && maxVal > minVal) {
        depthMap.convertTo(depthMapVis, CV_8UC1, 
                          255.0 / (maxVal - minVal), 
                          -255.0 * minVal / (maxVal - minVal));
    } else {
        depthMapVis = cv::Mat::zeros(depthMap.size(), CV_8UC1);
    }

    return depthMapVis;
}
// 静态工具函数实现（类内封装）
float c_Zivid_Camera_Remote::getValueZ(const Zivid::PointZ &p)
{
    return p.z;
}
bool c_Zivid_Camera_Remote::isLesserOrNan(const Zivid::PointZ &a, const Zivid::PointZ &b)
{
    if (std::isnan(getValueZ(a)) && std::isnan(getValueZ(b))) {
        return false;
    }
    return getValueZ(a) < getValueZ(b) ? true : std::isnan(getValueZ(a));
}
bool c_Zivid_Camera_Remote::isGreaterOrNaN(const Zivid::PointZ &a, const Zivid::PointZ &b)
{
    if (std::isnan(getValueZ(a)) && std::isnan(getValueZ(b))) {
        return false;
    }
    return getValueZ(a) > getValueZ(b) ? true : std::isnan(getValueZ(a));
}
//连接相机
void c_Zivid_Camera_Remote::Connect()
{
    if (c_Variable::getInstance().g_Zivid_Camera.camera_connected) {
        emit Status("相机已连接，无需再次连接");
        return;
    }
    
    if(!QtPing(c_Variable::getInstance().g_Zivid_Camera.Camera_Ip)){
        emit Status("相机未上电，6秒后重连连接");
        QTimer::singleShot(6000, this, &c_Zivid_Camera_Remote::Connect);
        return;
    }
    QElapsedTimer timer;
    timer.start();
    emit Status("相机连接中");
    
    try {
        m_camera = m_application.connectCamera(
            Zivid::CameraInfo::SerialNumber(
                c_Variable::getInstance().g_Zivid_Camera.camera_serial.toStdString()
            )
        );
    }
    catch (const std::exception& e) {
        emit Status(QString("连接失败: %1").arg(e.what()));
        c_Variable::getInstance().g_Zivid_Camera.camera_connected = false;
        c_Variable::getInstance().writeMessage(1, "Disconnect_Done");
        QTimer::singleShot(6000, this, &c_Zivid_Camera_Remote::Connect);
        return;
    }
    
    if (m_camera.state().isConnected()) {
        c_Variable::getInstance().g_Zivid_Camera.camera_connected = true;
        emit Status(QString("找到序列号为 %1 的相机")
            .arg(c_Variable::getInstance().g_Zivid_Camera.camera_serial));
        c_Variable::getInstance().g_Zivid_Camera.settingTime = timer.elapsed();
        c_Variable::getInstance().writeMessage(1, "Connect_Done");
    }
    else {
        c_Variable::getInstance().g_Zivid_Camera.camera_connected = false;
        emit Status(QString("未找到序列号为 %1 的相机")
            .arg(c_Variable::getInstance().g_Zivid_Camera.camera_serial));
        c_Variable::getInstance().writeMessage(1, "Disconnect_Done");
        QTimer::singleShot(6000, this, &c_Zivid_Camera_Remote::Connect);
    }
}
// 释放相机资源
void c_Zivid_Camera_Remote::Disconnect()
{
	if (c_Variable::getInstance().g_Zivid_Camera.camera_connected && m_camera.state().isConnected()) {
		m_camera.disconnect();
		c_Variable::getInstance().g_Zivid_Camera.camera_connected = false;
		emit Status(QString("相机已断开: %1").arg(
			c_Variable::getInstance().g_Zivid_Camera.camera_serial));

		c_Variable::getInstance().writeMessage(1, "Disconnect_Done");
	}
}
// 捕获2D+3D帧
void c_Zivid_Camera_Remote::Capture()
{
	if (!c_Variable::getInstance().g_Zivid_Camera.camera_connected && !m_camera.state().isConnected()) {
		emit Status("相机未连接，无法捕获图像");
		return;
	}
	
	Zivid::Frame frame;
    Zivid::Settings settings;

	QElapsedTimer timer;

    QString basePath =  c_Variable::getInstance().g_Zivid_Camera.base_Path;
	QString savePath = c_Variable::getInstance().g_Zivid_Camera.save_path;
	QString imageName = c_Variable::getInstance().g_Zivid_Camera.image_name;

    QString imagePath = basePath + savePath;

 	timer.start();

	if (!QDir().mkpath(imagePath)) {
		emit Status(QString("无法创建目录: %1").arg(c_Variable::getInstance().g_Zivid_Camera.save_path));
		return;
	}
	
	try {
        Zivid::CaptureAssistant::SuggestSettingsParameters params{
            Zivid::CaptureAssistant::SuggestSettingsParameters::MaxCaptureTime{ 
                std::chrono::milliseconds{ 2000 } 
            },
            Zivid::CaptureAssistant::SuggestSettingsParameters::AmbientLightFrequency::none
        };

        settings = Zivid::CaptureAssistant::suggestSettings(m_camera, params);

        // 强制全分辨率
        settings.set(Zivid::Settings::Sampling::Pixel{ Zivid::Settings::Sampling::Pixel::all });
        if (settings.color().hasValue()) {
            auto s2d = settings.color().value();
            s2d.set(Zivid::Settings2D::Sampling::Pixel{ Zivid::Settings2D::Sampling::Pixel::all });
            settings.set(Zivid::Settings::Color{ s2d });
        }
        settings.set(Zivid::Settings::Processing::Resampling::Mode{ 
            Zivid::Settings::Processing::Resampling::Mode::disabled 
        });

        // 【关键修复】正确钳位 3D Brightness ≤ 2.2
        {
            auto acqs = settings.acquisitions();
            Zivid::Settings::Acquisitions newAcqs;
            
            for (const auto& acq : acqs) {
                float b = acq.brightness().value();
                if (b > 2.2f) {
                    emit Status(QString("⚠️ 3D Brightness 钳位: %1 → 1.8").arg(b));  // 修复格式化
                    Zivid::Settings::Acquisition newAcq = acq;
                    newAcq.brightness() = Zivid::Settings::Acquisition::Brightness{ 1.8f };
                    newAcqs.emplaceBack(newAcq);
                } else {
                    newAcqs.emplaceBack(acq);
                }
            }
            settings.set(Zivid::Settings::Acquisitions{newAcqs});  // 回写生效
        }
		// 在钳位 Brightness 后，追加滤波参数优化
		{
    		//离群点去除：室外风沙噪声
    		settings.set(Zivid::Settings::Processing::Filters::Outlier::Removal::Threshold{ 6.5 });
    
    		//噪声三重处理
    		settings.set(Zivid::Settings::Processing::Filters::Noise::Removal::Threshold{ 4.0 });
    		settings.set(Zivid::Settings::Processing::Filters::Noise::Suppression::Enabled{ 
        		Zivid::Settings::Processing::Filters::Noise::Suppression::Enabled::yes });
    
    		// 孔洞修复：机车曲面
    		settings.set(Zivid::Settings::Processing::Filters::Hole::Repair::HoleSize{ 0.25f });
    		settings.set(Zivid::Settings::Processing::Filters::Hole::Repair::Strictness{ 2 });
    
    		//对比度校正：强光边缘锐化
    		settings.set(Zivid::Settings::Processing::Filters::Experimental::ContrastDistortion::Correction::Strength{ 0.35f });
    
    		//高斯平滑：保留裂纹细节
    		settings.set(Zivid::Settings::Processing::Filters::Smoothing::Gaussian::Sigma{ 0.6f });
    
   			// 聚类去除：移除悬浮小簇
    		settings.set(Zivid::Settings::Processing::Filters::Cluster::Removal::MaxNeighborDistance{ 5.0 });
    		settings.set(Zivid::Settings::Processing::Filters::Cluster::Removal::MinArea{ 30.0 });
    
    		//启用诊断（便于后续问题追溯）
    		settings.set(Zivid::Settings::Diagnostics::Enabled::yes);
    
    		emit Status("室外滤波参数优化已应用");
		}
		frame = m_camera.capture2D3D(settings);
	}
	catch (const Zivid::Exception& e) {
		emit Status(QString("捕获失败: %1").arg(e.what()));
		return;
	}
	
	//保存settings
	QFuture<void> saveSettings = QtConcurrent::run([settings, imagePath, imageName, this]() {
		//保存设置参数
		QString setting_path = imagePath + "/" + imageName + ".txt";
		QFile file(setting_path);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QMetaObject::invokeMethod(this, "Status", Qt::QueuedConnection,
				Q_ARG(QString, "打开失败" + setting_path));
				return;
			}
		file.write(QByteArray::fromStdString(settings.toString()));
		file.close();
	});	
	
	//保存3D
	QFuture<void> save3D = QtConcurrent::run([frame, imagePath, imageName, this]() {
		QElapsedTimer timer;
		timer.start();
		QString point_cloud_path = imagePath + "/" + imageName + ".xyz";

		QFile file(point_cloud_path);
		if (!file.open(QIODevice::WriteOnly)) {
			QMetaObject::invokeMethod(this, "Status", Qt::QueuedConnection,
				Q_ARG(QString, "3D保存路径错误"));
			return;
		}
		auto points = frame.pointCloud().copyData<Zivid::PointXYZ>();
		const size_t numPoints = points.size();
		const Zivid::PointXYZ* rawData = points.data();
		std::vector<int16_t> buffer(numPoints * 3);
		constexpr float SCALE = 10.0f;
		constexpr int16_t INVALID = -32768; // INT16_MIN

		int16_t* out = buffer.data();
		for (size_t i = 0; i < numPoints; ++i) {
			const auto& pt = rawData[i];
			out[i * 3 + 0] = std::isfinite(pt.x) ?
				static_cast<int16_t>(std::round(std::clamp(pt.x * SCALE, -32768.0f, 32767.0f))) : INVALID;
			out[i * 3 + 1] = std::isfinite(pt.y) ?
				static_cast<int16_t>(std::round(std::clamp(pt.y * SCALE, -32768.0f, 32767.0f))) : INVALID;
			out[i * 3 + 2] = std::isfinite(pt.z) ?
				static_cast<int16_t>(std::round(std::clamp(pt.z * SCALE, -32768.0f, 32767.0f))) : INVALID;
		}
		// 一次性写入
		file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size() * sizeof(int16_t));
		file.close();

		QMetaObject::invokeMethod(this, "onCapture3DCompleted", Qt::QueuedConnection,
			Q_ARG(QString, point_cloud_path),
			Q_ARG(qint64, timer.elapsed()));
		});
	
	//保存2D
	QFuture<void> save2D = QtConcurrent::run([frame, imagePath, imageName, this]() {
		QElapsedTimer timer;
		timer.start();
		// 保存2D图像
		QString image_path = imagePath + "/" + imageName + ".jpg";

		try {
			frame.frame2D().value().imageRGBA().save(image_path.toStdString());
		}
		catch (const std::exception& e) {
			QMetaObject::invokeMethod(this, "Status", Qt::QueuedConnection,
				Q_ARG(QString, e.what()));
			return;
		}

		QMetaObject::invokeMethod(this, "onCapture2DCompleted", Qt::QueuedConnection,
			Q_ARG(QString, image_path),
			Q_ARG(qint64, timer.elapsed()));
	});

	// ✅ 新增：保存深度图
    QFuture<void> saveDepthMap = QtConcurrent::run([this, imagePath, imageName, frame]() {
        QElapsedTimer timer; timer.start();
        try {
            const auto pointCloud = frame.pointCloud();
            cv::Mat depthMap = this->pointCloudToCvZ(pointCloud);
            if (depthMap.empty()) {
                QMetaObject::invokeMethod(this, "Status", Qt::QueuedConnection, Q_ARG(QString, "⚠️ 深度图转换结果为空"));
                return;
            }
            QString depthMapPath = imagePath + "/" + imageName + ".png";
            std::vector<int> params = { cv::IMWRITE_PNG_COMPRESSION, 0 };
            if (!cv::imwrite(depthMapPath.toStdString(), depthMap, params)) {
                QMetaObject::invokeMethod(this, "Status", Qt::QueuedConnection, Q_ARG(QString, "❌ 深度图保存失败"));
                return;
            }
            QMetaObject::invokeMethod(this, [this, depthMapPath]() {
                emit Status("✅ DepthMap saved: " + depthMapPath); // ✅ 修复 logMessage 未声明
            }, Qt::QueuedConnection);
            QMetaObject::invokeMethod(this, "onCaptureDepthMapCompleted", Qt::QueuedConnection, Q_ARG(QString, depthMapPath), Q_ARG(qint64, timer.elapsed()));
        } catch (const std::exception &e) {
            QMetaObject::invokeMethod(this, "Status", Qt::QueuedConnection, Q_ARG(QString, "💥 深度图保存异常: " + QString(e.what())));
        } catch (...) {
            QMetaObject::invokeMethod(this, "Status", Qt::QueuedConnection, Q_ARG(QString, "💥 深度图保存未知错误"));
        }
    });


	emit Status("捕获完成");
		
	c_Variable::getInstance().g_Zivid_Camera.captureTime = timer.elapsed();
	c_Variable::getInstance().g_Zivid_Camera.last_operation_time = QDateTime::currentDateTime();
}
void c_Zivid_Camera_Remote::onCapture2DCompleted(QString path, qint64 time)
{
    c_Variable::getInstance().g_Zivid_Camera.save2DTime = time;
    emit Status("2D保存完成");
    isCapture2DCompleted = true;
    
    if (isCapture2DCompleted && isCapture3DCompleted && isCaptureDepthMapCompleted) {
        isCapture2DCompleted = false;
        isCapture3DCompleted = false;
        isCaptureDepthMapCompleted = false;
        emit CaptureCompleted(path);  // path为2D主图路径，保持接口兼容
    }
}
void c_Zivid_Camera_Remote::onCapture3DCompleted(QString path, qint64 time)
{
    c_Variable::getInstance().g_Zivid_Camera.save3DTime = time;
    emit Status("3D保存完成");
    isCapture3DCompleted = true;
    
    if (isCapture2DCompleted && isCapture3DCompleted && isCaptureDepthMapCompleted) {
        isCapture2DCompleted = false;
        isCapture3DCompleted = false;
        isCaptureDepthMapCompleted = false;
        emit CaptureCompleted(path);
    }
}
void c_Zivid_Camera_Remote::onCaptureDepthMapCompleted(QString path, qint64 time)
{
    c_Variable::getInstance().g_Zivid_Camera.saveDepthMapTime = time;
    emit Status("深度图保存完成");
    isCaptureDepthMapCompleted = true;
    
    if (isCapture2DCompleted && isCapture3DCompleted && isCaptureDepthMapCompleted) {
        isCapture2DCompleted = false;
        isCapture3DCompleted = false;
        isCaptureDepthMapCompleted = false;
        emit CaptureCompleted(path);  // 统一发射总完成信号
    }
}


