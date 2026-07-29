#pragma once
#include "Process_Remote.h"

class c_CH10X_Remote : public c_Process_Remote {
	Q_OBJECT

public:
	explicit c_CH10X_Remote(QString device_name, QObject * parent = nullptr);
	virtual ~c_CH10X_Remote();

public slots:
	virtual void Device_Data(QString message);
};
