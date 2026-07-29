#pragma once
#include "Variable.h"


class c_Hikvision_Remote : public c_Object
{
    Q_OBJECT

public:
    explicit c_Hikvision_Remote(QObject* parent = Q_NULLPTR);
    virtual ~c_Hikvision_Remote();

public slots:
    void Connect();
    void Disconnect();
};


