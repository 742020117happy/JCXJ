#pragma once
#include "Process_Remote.h"

class c_E1R_Remote : public c_Process_Remote {
	Q_OBJECT

public:
	explicit c_E1R_Remote(QString device_name, QObject * parent = nullptr);
	virtual ~c_E1R_Remote();

public slots:
	virtual void Device_Data(QString message);

signals:
	void is_Obstacle(QString message);
};
