#pragma once  

#include "Variable.h"
#include "Zivid_Camera_Remote.h"
#include <TCP_Client.h>


namespace Ui{class c_Zivid_Camera;}


class c_Zivid_Camera : public QWidget
{
    Q_OBJECT

public:
    explicit c_Zivid_Camera(QStringList info, QWidget* parent = nullptr);
    ~c_Zivid_Camera() override;

signals:
    void Connect();
    void Disconnect();
    void Send_File_Info(QString filePath);

protected:
    void closeEvent(QCloseEvent* event) override;  
    void keyPressEvent(QKeyEvent* event) override; 
    void showEvent(QShowEvent* event) override;

private slots:
    void System_Scan();
    void VTK_Init();
    
    void Camera_Init(); 
    void Camera_DB();    
    void Camera_Scan();  
    void Camera_Delete();

    void TCP_Client_Init();
	void TCP_Client_DB();
	void TCP_Client_Scan();
	void TCP_Client_Delete();

    void onCaptureCompleted(QString path);

    void readMessage(QString cmd);

    void Write_Communicate_DB(QString key, int value);
    void Write_Communicate_DB(QString key, QString value);
    void Write_Communicate_DB(QString key, double value);

private:
    Ui::c_Zivid_Camera* ui;                         
    bool m_Scan = false;  
    bool m_vtkInitialized = false;

    QString m_DB_Path = "C:/ZividDebugLogs";    
	QString m_Path_Name;
    
    QThread* m_cameraThread;
    c_Zivid_Camera_Remote* m_cameras; 

    QThread *m_Client_Thread;
    c_TCP_Client *m_Client_Remote;

    QFile m_file;
    QSocketNotifier *m_pNotifier;
    char  m_arrRecv[128];

    vtkNew<vtkRenderer> m_renderer;
    vtkNew<vtkGenericOpenGLRenderWindow> m_renderWindow;
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr m_cloud;
    boost::shared_ptr<pcl::visualization::PCLVisualizer> m_viewer;
};

