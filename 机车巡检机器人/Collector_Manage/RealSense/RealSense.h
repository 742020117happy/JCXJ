#pragma once

#include "Variable.h"
#include "RealSense_Remote.h"
#include "ui_c_RealSense.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class c_RealSense;
}
QT_END_NAMESPACE

class c_RealSense : public QWidget
{
    Q_OBJECT

public:
    explicit c_RealSense(QStringList info, QWidget *parent = nullptr);
    ~c_RealSense() override;

signals:
    void Connect();
    void Disconnect();

protected:
    void closeEvent(QCloseEvent* event) override;  ///< ���ڹر��¼�
    void keyPressEvent(QKeyEvent* event) override; ///< �����¼�
    void showEvent(QShowEvent* event) override;

private slots:
    void readMessage(QString cmd);
    void System_Scan();

    void Write_Communicate_DB(QString key, int value);//дϵͳ����
    void Write_Communicate_DB(QString key, QString value);//дϵͳ����	
    void Write_Communicate_DB(QString key, double value);//дϵͳ����

    void VTK_Init();
    void RealSense_Init();
    void RealSense_DB();
    void RealSense_Delete();
    void Point_Scan(QVector<pcl::PointXYZRGB> points);
private:
    Ui::c_RealSenseClass ui;
    bool m_Scan = false;
    bool m_vtkInitialized = false;
    bool m_first_Scan = true;
    QString m_DB_Path = "C:/ZividDebugLogs";

    c_RealSense_Remote* m_RealSense;
    QThread* m_RealSense_thread;

    QFile m_file;
    QSocketNotifier *m_pNotifier;
    char  m_arrRecv[128];

    vtkNew<vtkRenderer> m_renderer;
    vtkNew<vtkGenericOpenGLRenderWindow> m_renderWindow;
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr m_cloud;
    boost::shared_ptr<pcl::visualization::PCLVisualizer> m_viewer;
};

