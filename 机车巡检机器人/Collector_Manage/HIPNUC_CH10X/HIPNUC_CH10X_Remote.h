#pragma once
#include "Variable.h"
#include <Eigen/Dense>
#include <cmath>

/**
 * @brief CH10X 惯导模块远程管理类 (最终工程修正版)
 * 核心架构：Body系零偏在线补偿 + 防抖晃动抑制 + 外部静止硬重置
 */
class c_HIPNUC_CH10X_Remote : public c_Object 
{
    Q_OBJECT

public:
    explicit c_HIPNUC_CH10X_Remote(QObject *parent = nullptr);
    ~c_HIPNUC_CH10X_Remote();

    void Init();
    void Connect();
    void Disconnect();
    void Set_High_Performance_Mode();

signals:
    void Status(const QString& msg);

private slots:
    void onReadyRead();

private:
    // ================= 协议解析与校验 =================
    void crc16_update(uint16_t *currentCrc, const uint8_t *src, uint32_t lengthInBytes);
    bool parseFrame(const uint8_t* data, int size);
    
    // ================= 惯导里程计解算 =================
    void Update_IMU_Odom(const s_CH10X_DataPayload& data);
    double wrapToPi(double angle); 

    // ================= 硬件接口 =================
    QSerialPort* m_serialPort;
    QByteArray m_buffer;

    // ================= 运动学积分与滤波变量 =================
    uint32_t m_last_timestamp_ms = 0;       // 上一帧时间戳 (ms)
    Eigen::Vector2d m_vel_enu = Eigen::Vector2d::Zero(); // ENU系速度 [vx, vy] (m/s)
    double m_filtered_acc_n[2] = {0.0, 0.0}; // 低通滤波后的 ENU 水平加速度 (m/s^2)
    Eigen::Vector2d m_pos_enu = Eigen::Vector2d::Zero(); // ENU系位置 [x, y] (m)
    
    // ================= 🌟 新增：Body系零偏在线补偿 =================
    Eigen::Vector2d m_acc_bias_body = Eigen::Vector2d::Zero(); // Body系水平零偏 (G)
    Eigen::Vector2d m_acc_bias_body_sum = Eigen::Vector2d::Zero();
    int m_bias_sample_count = 0;
    bool m_bias_initialized = false;

    // ================= 🌟 新增：晃动抑制防抖计数器 =================
    int m_shake_counter = 0;

    // ================= 外部状态与死区参数 =================
    double m_acc_deadzone_thresh = 0.02;      // 加速度死区 (约 2mg)
    double m_vel_deadzone_thresh = 0.01;      // 速度死区 (1 cm/s)
    double m_alpha_acc = 0.4;                 // 🌟 提升动态响应 (截止频率约 5.7Hz)

    // ================= 降采样发布 =================
    uint32_t m_last_publish_time_ms = 0;      
    static const uint32_t PUBLISH_INTERVAL_MS = 100; // 10Hz
};

