#pragma once 
#include "Variable.h"

class c_Zivid_Camera_Remote : public c_Object
{
    Q_OBJECT

public:
    explicit c_Zivid_Camera_Remote(QObject* parent = nullptr);
    ~c_Zivid_Camera_Remote() override;

public slots:
    void Init();
    void Connect();
    void Disconnect();
    void Capture();
  
signals:
    /// 📡 捕获完成信号（path为2D图像主路径，保持接口兼容）
    void CaptureCompleted(QString path);

private:
    Zivid::Application m_application;   ///< Zivid应用实例
    Zivid::Camera m_camera;             ///< Zivid相机实例
    Zivid::CaptureAssistant::SuggestSettingsParameters m_Suggest_Settings;  ///< 自动参数建议

    // ========================================================================
    // 🔹 核心功能：点云转深度图（成员函数）
    // ========================================================================
    /// @brief 将Zivid点云转换为OpenCV深度图（Z通道，归一化可视化）
    /// @param pointCloud 输入点云
    /// @return CV_8UC1格式的可视化深度图
    cv::Mat pointCloudToCvZ(const Zivid::PointCloud &pointCloud);
    
    // ========================================================================
    // 🔹 工具函数：静态成员（无需对象实例，便于复用）
    // ========================================================================
    /// @brief 获取PointZ的Z值
    static float getValueZ(const Zivid::PointZ &p);
    
    /// @brief NaN安全的小于比较：a < b 或 a为NaN时返回true
    static bool isLesserOrNan(const Zivid::PointZ &a, const Zivid::PointZ &b);
    
    /// @brief NaN安全的大于比较：a > b 或 a为NaN时返回true
    static bool isGreaterOrNaN(const Zivid::PointZ &a, const Zivid::PointZ &b);

    // ========================================================================
    // 🔹 异步任务完成回调槽函数
    // ========================================================================
private slots:
    void onCapture2DCompleted(QString path, qint64 time);
    void onCapture3DCompleted(QString path, qint64 time);
    void onCaptureDepthMapCompleted(QString path, qint64 time);

    // ========================================================================
    // 🔹 状态标志：三任务完成同步（2D+3D+DepthMap）
    // ========================================================================
private:
    bool isCapture2DCompleted = false;
    bool isCapture3DCompleted = false;
    bool isCaptureDepthMapCompleted = false;
};
