#include "FPSTesterThread.h"
#include "BlockQueue.h"

fpsTest::FPSTesterThread::FPSTesterThread(QObject *parent)
    : QThread (parent)
    , mpAlgorithm(nullptr)
    , mStop (false)
    , mTestTime(0)
{
}

fpsTest::FPSTesterThread::~FPSTesterThread()
{
    mStop = true;
    wait(2000UL);
}

void fpsTest::FPSTesterThread::setImageQueue(const std::shared_ptr<fpsTest::BlockQueue> &imgQueue)
{
    mImgQueue = imgQueue;
}

void fpsTest::FPSTesterThread::stop()
{
    mStop = true;
}

void fpsTest::FPSTesterThread::run()
{
    if(!mImgQueue.get()) {
        emit errorMsg(tr("Image queue is not set!"));
        return;
    }

    // TODO: 定义算法接口类，并取消这部分注释
//    if(!mpAlgorithm)
//    {
//        emit errorMsg(tr("Algorithm is not set!"));
//        return;
//    }

    qint64 t = 0;
    qint64 count = 0;
    mLastEmitTime = 0;
    mElapsTimer.start();

    qDebug()<<"FPSTester Thread started!";

    while (!mStop && (mTestTime <= 0 || t < mTestTime)) {
//        auto img = mImgQueue->dequeue();

        // TODO: 具体逻辑
        //
        // mpAlgorithm->update()

        t = mElapsTimer.elapsed();
        ++count;
        if(t - mLastEmitTime >= 1000)
        {
            emit updated(t, count);
            mLastEmitTime = t;
        }
    }
    emit updated(t, ++count);
}
