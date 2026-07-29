#pragma once
#include "Variable.h"

class c_Fr_Light : public QWidget {
	Q_OBJECT

public:
	enum State {
		Default,
		Error,
		Working,
		Null
	};
	explicit c_Fr_Light(QWidget * parent = nullptr);
	virtual ~c_Fr_Light();

	public slots:
	void Set_Working();
	void Set_Default();
	void Set_Error();
	void Set_Null();
	void Set_State(const bool state);
	void Set_Radius(int value);
	bool Get_State();

signals:
	void Working_State(bool state);
	void Default_State(bool state);
	void Error_State(bool state);

protected:
	void paintEvent(QPaintEvent *);

private:
	int m_Radius = 50;
	State m_State = Default;
	QColor m_Default_Color;
	QColor m_Error_Color;
	QColor m_Working_Color;
	QColor m_Null_Color;
};
