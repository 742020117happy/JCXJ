#include <QApplication>
#include "Hikvision.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    c_Hikvision w(a.arguments());
    w.show();
    return a.exec();
}
