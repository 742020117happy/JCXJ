#pragma once
#include "Variable.h"
#include "E1R_MSOP.h"
#include "E1R_DIFOP.h"
#include "ui_c_RoboSense.h"

class c_RoboSense : public QWidget
{
    Q_OBJECT

public:
    explicit c_RoboSense(QStringList info, QWidget* parent = nullptr);
    virtual ~c_RoboSense();

protected:
    void closeEvent(QCloseEvent* event) override;  
    void keyPressEvent(QKeyEvent* event) override;
    void showEvent(QShowEvent* event) override;

private slots:
    void readMessage(QString cmd);
    void System_Scan();

    void Write_Communicate_DB(QString key, int value);//дϵͳ����
    void Write_Communicate_DB(QString key, QString value);//дϵͳ����	
    void Write_Communicate_DB(QString key, double value);//дϵͳ����

    void VTK_Init();
    void RoboSense_Init();
    void RoboSense_DB();
    void RoboSense_Delete();

    void MSOP_Scan(QVector<pcl::PointXYZI> points);

private:
    Ui::c_RoboSenseClass ui;
    bool m_Scan = true;
    bool m_vtkInitialized = false;
    bool m_first_Scan = true;
    QString m_DB_Path = "C:/ZividDebugLogs";

    c_E1R_MSOP* m_E1R_MSOP;
    QThread* m_E1R_MSOP_thread;

    c_E1R_DIFOP* m_E1R_DIFOP;
    QThread* m_E1R_DIFOP_thread;

    int m_Safe_Distance;
    int m_Safe_Num;
    double m_max_point_1;
    double m_max_point_2;
    double m_max_point_3;
    double m_min_point_1;
    double m_min_point_2;
    double m_min_point_3;

    float m_roi_distance = 0;
    int m_roi_point_count = 0;

    QFile m_file;
    QSocketNotifier *m_pNotifier;
    char  m_arrRecv[128];

    vtkNew<vtkRenderer> m_renderer;
    vtkNew<vtkGenericOpenGLRenderWindow> m_renderWindow;
    pcl::PointCloud<pcl::PointXYZI>::Ptr m_cloud;
    pcl::PointCloud<pcl::PointXYZI>::Ptr m_Roi;
    boost::shared_ptr<pcl::visualization::PCLVisualizer> m_viewer;
};

