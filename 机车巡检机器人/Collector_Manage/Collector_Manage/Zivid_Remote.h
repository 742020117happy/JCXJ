#pragma once
#include "Process_Remote.h"

class c_Zivid_Remote : public c_Process_Remote {
	Q_OBJECT

public:
	explicit c_Zivid_Remote(QString device_name, QObject * parent = nullptr);
	virtual ~c_Zivid_Remote();

public slots:
	virtual void Device_Data(QString message);

 	void updateSaveInfo(QString save_path);// 更新保存路径
    void Capture(QString image_name);// 触发主界面“捕获”按钮点击

signals:
	void updateCompleted(); // 更新保存路径完成回调
    void CaptureCompleted(); // 相机采集完成回调
};
