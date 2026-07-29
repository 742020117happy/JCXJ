#pragma once
#include "Process_Remote.h"

class c_Hikvision_Remote : public c_Process_Remote {
	Q_OBJECT

public:
	explicit c_Hikvision_Remote(QString device_name, QObject * parent = nullptr);
	virtual ~c_Hikvision_Remote();
};
