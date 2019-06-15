#include "SimpleTest.h"
#include <QDebug>

SimpleTest::SimpleTest(QObject *parent) : QObject(parent)
{
}

void SimpleTest::init()
{
    mFPSTester.setImgDir("C:/Users/pplxl/Pictures");
}

void SimpleTest::start(qint64 msec)
{
    qDebug()<<"SimpleTest start!";
    mFPSTester.setTestTime(msec);
    mFPSTester.start();
}
