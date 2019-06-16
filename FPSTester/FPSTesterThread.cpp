#include "FPSTesterThread.h"
#include "BlockQueue.h"
#include "FPSTester.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

fpsTest::FPSTesterThread::FPSTesterThread(QObject *parent)
    : QThread (parent)
    , mAlgorithm(nullptr)
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
    bool algorithmValid = true;

    if(!mImgQueue.get()) {
        emit errorMsg(tr("Image queue is not set!"));
        return;
    }

    if(!mAlgorithm || !mAlgorithm->init()) {
        algorithmValid = false;
        qWarning()<<"Algorithm is invalid!";
    }

    qint64 t = 0;
    qint64 count = 0;
    mLastEmitTime = 0;
    mElapsTimer.start();

    qDebug()<<"FPSTester Thread started!";

    while (!mStop && (mTestTime <= 0 || t < mTestTime)) {
        auto img = mImgQueue->dequeue();

        if(!img.empty()) {
            if(algorithmValid)
                mAlgorithm->update(img);
            ++count;
        }

        t = mElapsTimer.elapsed();

        if(t - mLastEmitTime >= 1000)
        {
            emit updated(t, count);
            mLastEmitTime = t;
        }
    }
    emit updated(t, ++count);
}
