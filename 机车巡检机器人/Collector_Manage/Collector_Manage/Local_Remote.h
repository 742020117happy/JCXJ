#pragma once
#include "Server_Remote.h"

class c_Local_Remote : public c_Server_Remote
{
	Q_OBJECT
public:
	explicit c_Local_Remote(QObject *parent = nullptr);
	virtual ~c_Local_Remote();
	public slots:
	virtual void Connect();
	virtual void Connect_Done();
	virtual void Disconnect_Done();
};
