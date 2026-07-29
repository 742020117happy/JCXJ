#pragma once
#include "Process_Remote.h"

class c_RealSense_Remote : public c_Process_Remote {
	Q_OBJECT

public:
	explicit c_RealSense_Remote(QString device_name, QObject * parent = nullptr);
	virtual ~c_RealSense_Remote();
};
