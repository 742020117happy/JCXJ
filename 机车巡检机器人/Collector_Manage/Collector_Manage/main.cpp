#include "Collector_Manage.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    qRegisterMetaType<WId>("WId");
    c_Collector_Manage w;   
    return a.exec();
}
