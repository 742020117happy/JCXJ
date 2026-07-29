#include "HIPNUC_CH10X_Remote.h"
#include <QtMath>
#include <QDateTime>

// ============================================================================
// 构造、析构、初始化、连接、断开 (保持标准实现)
// ============================================================================
c_HIPNUC_CH10X_Remote::c_HIPNUC_CH10X_Remote(QObject *parent) : c_Object(parent), m_serialPort(nullptr) {}
c_HIPNUC_CH10X_Remote::~c_HIPNUC_CH10X_Remote() { Disconnect(); }

void c_HIPNUC_CH10X_Remote::Init() {
    emit Status("CH10X 惯导后台线程已启动 (工程修正版)");
    if (!m_serialPort) {
        m_serialPort = new QSerialPort(this);
        QObject::connect(m_serialPort, &QSerialPort::readyRead, this, &c_HIPNUC_CH10X_Remote::onReadyRead);
        QObject::connect(m_serialPort, &QSerialPort::errorOccurred, this, [this](QSerialPort::SerialPortError error) {
            if (error == QSerialPort::ResourceError) { emit Status("⚠️ CH10X 串口拔出"); Disconnect(); }
        });
    }
    m_vel_enu.setZero();
    m_pos_enu.setZero();
    QTimer::singleShot(10, this, &c_HIPNUC_CH10X_Remote::Connect);
}

void c_HIPNUC_CH10X_Remote::Connect() {
    auto& config = c_Variable::getInstance().g_HIPNUC_CH10X;
    if (config.connected) return;
    if (m_serialPort->isOpen()) m_serialPort->close();
    m_serialPort->setPortName(config.port_name);
    m_serialPort->setBaudRate(config.baud_rate); 
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    if (m_serialPort->open(QIODevice::ReadWrite)) {
        config.connected = true; m_buffer.clear();
        emit Status(QString("✅ 串口打开成功: %1 @ %2").arg(config.port_name).arg(config.baud_rate));
    } else {
        config.connected = false;
        emit Status(QString("❌ 串口打开失败: %1").arg(m_serialPort->errorString()));
    }
}

void c_HIPNUC_CH10X_Remote::Disconnect() {
    if (m_serialPort && m_serialPort->isOpen()) m_serialPort->close();
    c_Variable::getInstance().g_HIPNUC_CH10X.connected = false;
    emit Status("CH10X 已断开连接");
}

void c_HIPNUC_CH10X_Remote::crc16_update(uint16_t *currentCrc, const uint8_t *src, uint32_t lengthInBytes) {
    uint32_t crc = *currentCrc;
    for (uint32_t j = 0; j < lengthInBytes; ++j) {
        uint32_t byte = src[j]; crc ^= byte << 8;
        for (int i = 0; i < 8; ++i) { uint32_t temp = crc << 1; if (crc & 0x8000) temp ^= 0x1021; crc = temp; }
    }
    *currentCrc = crc;
}

void c_HIPNUC_CH10X_Remote::onReadyRead() {
    m_buffer.append(m_serialPort->readAll());
    if (m_buffer.size() > 8192) { m_buffer.clear(); return; }
    while (m_buffer.size() >= 6) {
        int headerIdx = m_buffer.indexOf("\x5A\xA5");
        if (headerIdx < 0) { if (m_buffer.endsWith('\x5A')) m_buffer = m_buffer.right(1); else m_buffer.clear(); break; }
        if (headerIdx > 0) m_buffer.remove(0, headerIdx);
        if (m_buffer.size() < 6) break;
        uint16_t payloadLen = (uint8_t)m_buffer.at(2) | ((uint8_t)m_buffer.at(3) << 8);
        int totalLen = 6 + payloadLen;
        if (m_buffer.size() < totalLen) break;
        uint16_t recvCrc = (uint8_t)m_buffer.at(4) | ((uint8_t)m_buffer.at(5) << 8);
        uint16_t calcCrc = 0;
        crc16_update(&calcCrc, reinterpret_cast<const uint8_t*>(m_buffer.constData()), 4);
        crc16_update(&calcCrc, reinterpret_cast<const uint8_t*>(m_buffer.constData()) + 6, payloadLen);
        if (calcCrc == recvCrc) {
            const uint8_t* frameData = reinterpret_cast<const uint8_t*>(m_buffer.constData()) + 6;
            int subPos = 0;
            while (subPos < payloadLen) {
                uint8_t tag = frameData[subPos];
                if (tag == 0x91 && subPos + 76 <= payloadLen) { parseFrame(frameData + subPos, 76); subPos += 76; }
                else break;
            }
            m_buffer.remove(0, totalLen);
        } else { m_buffer.remove(0, 2); }
    }
}

bool c_HIPNUC_CH10X_Remote::parseFrame(const uint8_t* data, int size) {
    if (size < 76) return false;
    s_CH10X_DataPayload payload;
    std::memcpy(&payload, data, sizeof(s_CH10X_DataPayload));
    auto& g_imu = c_Variable::getInstance().g_HIPNUC_CH10X;
    g_imu.timestamp = payload.timestamp;
    for(int i = 0; i < 3; ++i) {
        g_imu.acc[i] = payload.acc[i]; g_imu.gyro[i] = payload.gyro[i];
        g_imu.mag[i] = payload.mag[i]; g_imu.euler[i] = payload.euler[i];
    }
    for(int i = 0; i < 4; ++i) g_imu.quat[i] = payload.quat[i];
    g_imu.last_update_time = QDateTime::currentMSecsSinceEpoch();
    g_imu.fps_count++;
    Update_IMU_Odom(payload);
    return true;
}

void c_HIPNUC_CH10X_Remote::Set_High_Performance_Mode() {
    if (!m_serialPort || !m_serialPort->isOpen()) return;
    emit Status("⚠️ 正在配置 100Hz 高性能模式 (6轴 + 100Hz)...");
    auto safeWrite = [this](int delayMs, const char* cmd, const QString& msg) {
        QTimer::singleShot(delayMs, this, [this, cmd, msg]() {
            if (m_serialPort && m_serialPort->isOpen()) { m_serialPort->write(cmd); emit Status(msg); }
        });
    };
    safeWrite(0,    "AT+EOUT=0\r\n",   "📤 AT+EOUT=0");
    safeWrite(500,  "AT+MODE=0\r\n",   "📤 AT+MODE=0 (6轴)");
    safeWrite(1000, "AT+ODR=100\r\n",  "📤 AT+ODR=100");
    safeWrite(1500, "AT+RST\r\n",      "📤 AT+RST");
    QTimer::singleShot(2000, this, [this]() {
        emit Status("✅ 配置完成！请在 UI 点击 [连接惯导] 重新连接！");
    });
}

double c_HIPNUC_CH10X_Remote::wrapToPi(double angle) {
    while (angle > M_PI) angle -= 2.0 * M_PI;
    while (angle < -M_PI) angle += 2.0 * M_PI;
    return angle;
}

// ============================================================================
// 🌟 核心：2D 惯导里程计解算 (Body系零偏补偿 + 防抖晃动抑制 + 修正航向转换)
// ============================================================================
void c_HIPNUC_CH10X_Remote::Update_IMU_Odom(const s_CH10X_DataPayload& data) {
    // 1. 时间差计算
    // 🌟 数学保证：uint32_t 减法自动处理 49 天回绕，无需额外 if 判断
    uint32_t dt_ms = data.timestamp - m_last_timestamp_ms;
    if (m_last_timestamp_ms == 0) { m_last_timestamp_ms = data.timestamp; return; }
    double dt = dt_ms / 1000.0; 
    if (dt <= 0.0 || dt > 0.1) { m_last_timestamp_ms = data.timestamp; return; }
    m_last_timestamp_ms = data.timestamp;

    // 读取外部静止状态
    bool is_static = c_Variable::getInstance().g_IMU_Odom.is_static;

    // ======================================================================
    // 🌟 2. Body系零偏在线补偿 (仅在静止时采集)
    // ======================================================================
    if (!m_bias_initialized && is_static) {
        m_acc_bias_body_sum(0) += data.acc[0];
        m_acc_bias_body_sum(1) += data.acc[1];
        m_bias_sample_count++;
        if (m_bias_sample_count >= 100) { // 采集 1秒 (100Hz)
            m_acc_bias_body = m_acc_bias_body_sum / 100.0;
            m_bias_initialized = true;
            emit Status(QString("✅ Body系零偏标定完成: X=%1G, Y=%2G")
                        .arg(m_acc_bias_body(0), 0, 'f', 4).arg(m_acc_bias_body(1), 0, 'f', 4));
        }
    }

    // 扣除 Body系零偏，然后转换为 m/s^2
    const double G_TO_MSS = 9.80665;
    double acc_bx = (data.acc[0] - m_acc_bias_body(0)) * G_TO_MSS;
    double acc_by = (data.acc[1] - m_acc_bias_body(1)) * G_TO_MSS;
    double acc_bz = data.acc[2] * G_TO_MSS; // 🌟 修复：补全 Z 轴定义，Z轴不参与2D积分，无需扣除零偏

    // 3. 四元数归一化与旋转 (Body RFU -> ENU)
    double qw = data.quat[0], qx = data.quat[1], qy = data.quat[2], qz = data.quat[3];
    double norm = sqrt(qw*qw + qx*qx + qy*qy + qz*qz);
    if (norm < 1e-6) return;
    qw /= norm; qx /= norm; qy /= norm; qz /= norm;

    double ax_n = (1 - 2*(qy*qy + qz*qz))*acc_bx + 2*(qx*qy - qw*qz)*acc_by + 2*(qx*qz + qw*qy)*acc_bz;
    double ay_n = 2*(qx*qy + qw*qz)*acc_bx + (1 - 2*(qx*qx + qz*qz))*acc_by + 2*(qy*qz - qw*qx)*acc_bz;

    // 4. 一阶低通滤波 (α=0.4 提升动态响应)
    m_filtered_acc_n[0] = m_alpha_acc * ax_n + (1.0 - m_alpha_acc) * m_filtered_acc_n[0];
    m_filtered_acc_n[1] = m_alpha_acc * ay_n + (1.0 - m_alpha_acc) * m_filtered_acc_n[1];

    // 5. 加速度死区
    if (std::abs(m_filtered_acc_n[0]) < m_acc_deadzone_thresh) m_filtered_acc_n[0] = 0.0;
    if (std::abs(m_filtered_acc_n[1]) < m_acc_deadzone_thresh) m_filtered_acc_n[1] = 0.0;

    // ======================================================================
    // 🌟 6. 防抖晃动抑制 (解决起步误杀问题)
    // ======================================================================
    double current_vel_mag = sqrt(m_vel_enu(0)*m_vel_enu(0) + m_vel_enu(1)*m_vel_enu(1));
    double current_acc_mag = sqrt(m_filtered_acc_n[0]*m_filtered_acc_n[0] + m_filtered_acc_n[1]*m_filtered_acc_n[1]);
    
    // 矛盾条件：速度极小，但加速度极大
    if (current_vel_mag < 0.05 && current_acc_mag > 0.5) {
        m_shake_counter++;
    } else {
        m_shake_counter = 0; // 只要有一帧不满足，立即清零计数器
    }
    
    // 连续 20 帧 (0.2秒) 满足矛盾条件，才判定为机械晃动，强制置零
    if (m_shake_counter > 20) {
        m_filtered_acc_n[0] = 0.0;
        m_filtered_acc_n[1] = 0.0;
    }

    // ======================================================================
    // 7. 核心逻辑：外部静止硬重置 / 运动学积分
    // ======================================================================
    if (is_static) {
        m_vel_enu.setZero();
        m_shake_counter = 0; // 静止时重置晃动计数器
    } else {
        m_vel_enu(0) += m_filtered_acc_n[0] * dt;
        m_vel_enu(1) += m_filtered_acc_n[1] * dt;
        
        if (std::abs(m_vel_enu(0)) < m_vel_deadzone_thresh) m_vel_enu(0) = 0.0;
        if (std::abs(m_vel_enu(1)) < m_vel_deadzone_thresh) m_vel_enu(1) = 0.0;
        
        m_pos_enu(0) += m_vel_enu(0) * dt;
        m_pos_enu(1) += m_vel_enu(1) * dt;
    }

    // 8. 航向角提取 (四元数 -> Yaw)
    double siny_cosp = 2.0 * (qw * qz + qx * qy);
    double cosy_cosp = 1.0 - 2.0 * (qy * qy + qz * qz);
    double yaw_enu = std::atan2(siny_cosp, cosy_cosp);
    
    // ======================================================================
    // 🌟 9. 坐标系转换：ENU -> 地图系 (X右，Y下，逆时针为正)
    // ======================================================================
    // 位置转换：X=东，Y=南(北取反)
    double map_x = m_pos_enu(0);
    double map_y = -m_pos_enu(1);
    double map_vx = m_vel_enu(0);
    double map_vy = -m_vel_enu(1);
    
    // 🌟 航向角修正：
    // ENU下：北=0，逆时针为正 (东=90°)
    // 地图系(X右, Y下)：期望 X(东)=0°，逆时针为正 (北=90°)
    // 公式推导：map_yaw = π/2 - yaw_enu
    double map_yaw = wrapToPi(M_PI / 2.0 - yaw_enu); 

    // 10. 降采样发布 (10Hz)
    if (data.timestamp - m_last_publish_time_ms >= PUBLISH_INTERVAL_MS) {
        m_last_publish_time_ms = data.timestamp;
        
        auto& g_odom = c_Variable::getInstance().g_IMU_Odom;
        g_odom.x = map_x;
        g_odom.y = map_y;
        g_odom.vx = map_vx;
        g_odom.vy = map_vy;
        g_odom.yaw = map_yaw;
        g_odom.timestamp = data.timestamp;
        g_odom.is_static = is_static;

        QString odom_str = QString("IMU_ODOM&%1&%2&%3&%4&%5&%6&%7")
            .arg(map_x, 0, 'f', 3).arg(map_y, 0, 'f', 3)
            .arg(map_vx, 0, 'f', 3).arg(map_vy, 0, 'f', 3)
            .arg(map_yaw, 0, 'f', 4).arg(data.timestamp).arg(is_static ? 1 : 0);
            
        c_Variable::getInstance().writeMessage(1, odom_str);
    }
}
