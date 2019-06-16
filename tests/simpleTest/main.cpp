#include "SimpleTest.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SimpleTest simpleTest;
    simpleTest.init();
    simpleTest.start(15*1000);

    return a.exec();
}
