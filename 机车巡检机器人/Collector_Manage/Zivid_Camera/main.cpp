#include "Zivid_Camera.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    c_Zivid_Camera w(a.arguments());
    return a.exec();
}
