#include "FPSTester.h"
#include <QDebug>

fpsTest::FPSTester::FPSTester(QObject *parent) : QObject (parent)
{
    mFPSTestThread.setImageQueue(mImgGetThread.imgQueue());

    connect(&mFPSTestThread, &FPSTesterThread::finished, this, &FPSTester::onTesterStopped, Qt::QueuedConnection);
    connect(&mImgGetThread, &ImgGetterThread::finished, this, &FPSTester::onImgGetterStopped, Qt::QueuedConnection);
    connect(&mFPSTestThread, &FPSTesterThread::errorMsg, this, &FPSTester::onErrorMsg, Qt::QueuedConnection);
    connect(&mImgGetThread, &ImgGetterThread::errorMsg, this, &FPSTester::onErrorMsg, Qt::QueuedConnection);

    connect(&mFPSTestThread, &FPSTesterThread::updated, this, &FPSTester::onTesterUpdated, Qt::QueuedConnection);
    connect(this, &FPSTester::stopped, this, &FPSTester::onStopped);
}

void fpsTest::FPSTester::setTestTime(qint64 time)
{
    mFPSTestThread.mTestTime = time;
}

void fpsTest::FPSTester::setImgGetter(fpsTest::ImgGetter *igt)
{
    mImgGetThread.mImgGetter = igt;
}

void fpsTest::FPSTester::setAlgorithm(fpsTest::Algorithm *alg)
{
    mFPSTestThread.mAlgorithm = alg;
}

void fpsTest::FPSTester::setSavePath(const QString &path)
{
    mSavePath = path;
}

void fpsTest::FPSTester::start()
{
    mTestTime = 0;
    mTestCount = 0;
    mImgGetThread.start();
    mFPSTestThread.start();
}

void fpsTest::FPSTester::stop()
{
    mFPSTestThread.stop();
    mImgGetThread.stop();
}

void fpsTest::FPSTester::onTesterUpdated(qint64 t, qint64 count)
{
    qint64 t_diff = t - mTestTime;
    qint64 count_diff = count - mTestCount;
    double fps = count_diff / (t_diff * .001);
    mTestTime = t;
    mTestCount = count;
    qInfo("FPS: %.3f", fps);

    // TODO: 保存文件
}

void fpsTest::FPSTester::onTesterStopped()
{
    qDebug()<<"Tester thread is stopped.";
    if(mImgGetThread.isRunning())
        mImgGetThread.stop();
    else {
        qDebug()<<"FPS Tester Stopped.";
        emit stopped();
    }
}

void fpsTest::FPSTester::onImgGetterStopped()
{
    qDebug()<<"Image getter thread is stopped.";
    if(mFPSTestThread.isRunning())
        mFPSTestThread.stop();
    else {
        qDebug()<<"FPS Tester Stopped.";
        emit stopped();
    }
}

void fpsTest::FPSTester::onStopped()
{
    double fps = mTestCount / (mTestTime * .001) ;
    qInfo("FPS: %.3f", fps);
}

void fpsTest::FPSTester::onErrorMsg(const QString &msg)
{
    QString newMsg = QString(tr("Error: ")) + msg;
    qCritical() << newMsg;
    emit errorMsg(newMsg);
}
