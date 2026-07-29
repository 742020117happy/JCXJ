#include "Fr_Light.h"

c_Fr_Light::c_Fr_Light(QWidget * parent) :QWidget(parent)
{
	m_Working_Color = QColor("green");
	m_Default_Color = QColor("red");
	m_Error_Color = QColor("yellow");
	m_Null_Color = QColor("gray");
}

c_Fr_Light::~c_Fr_Light()
{
	
}

void c_Fr_Light::Set_Working()
{
	if (m_State == Working) { return; }//如果状态未改变则立即退出
	m_State = Working;
	update();
	emit Working_State(true);
}
bool c_Fr_Light::Get_State()
{
	if (m_State == Working) { return true; }
	else { return false; }
}
void c_Fr_Light::Set_Default()
{
	if (m_State == Default) { return; }	  //如果状态未改变则立即退出
	m_State = Default;
	update();
	emit Default_State(true);
}
void c_Fr_Light::Set_Error()
{
	if (m_State == Error) { return; }	   //如果状态未改变则立即退出
	m_State = Error;
	update();
	emit Error_State(true);
}
void c_Fr_Light::Set_Null()
{
	if (m_State == Null){ return; }
	m_State = Null;
	update();
}
void c_Fr_Light::Set_State(const bool state)
{
	if (state) {
		Set_Working();
	}
	else {
		Set_Default();
	}
}
void c_Fr_Light::Set_Radius(int value)
{
	m_Radius = value;
	update();
}
void c_Fr_Light::paintEvent(QPaintEvent*) {
	int side = qMin(width(), height());
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.translate(width() / 2, height() / 2);
	painter.scale(side / 100.0, side / 100.0);
	QColor color;
	switch (m_State) {
	case Default:
		color = m_Default_Color;
		break;
	case Working:
		color = m_Working_Color;
		break;
	case Error:
		color = m_Error_Color;
		break;
	default:
		color = m_Null_Color;
		break;
	}
	QRadialGradient radial(0, 0, m_Radius, 0, 0);    //设置圆的原点和焦点在中心,半径50
	radial.setSpread(QGradient::PadSpread);
	radial.setColorAt(1, color);
	painter.setPen(Qt::transparent);
	painter.setBrush(radial);
	painter.drawEllipse(-50, -50, 100, 100);
}