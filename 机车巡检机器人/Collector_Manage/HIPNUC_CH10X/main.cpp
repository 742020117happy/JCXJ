#include <QApplication>
#include "HIPNUC_CH10X.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    c_HIPNUC_CH10X w(a.arguments());
    return a.exec();
}