#include "ImgGetterThread.h"
#include "BlockQueue.h"
#include "FPSTester.h"
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <QDir>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <QDebug>

using namespace cv;
using std::shared_ptr;

fpsTest::ImgGetterThread::ImgGetterThread(QObject *parent)
    : QThread (parent)
    , mStop (false)
    , mImgQueue(new BlockQueue(8U))
    , mImgGetter(nullptr)
{

}

fpsTest::ImgGetterThread::~ImgGetterThread()
{
    mStop = true;
    mImgQueue->setBlock(false);
    wait(2000UL);
}

std::shared_ptr<fpsTest::BlockQueue> fpsTest::ImgGetterThread::imgQueue()
{
    return mImgQueue;
}

void fpsTest::ImgGetterThread::run()
{
    bool imgGetterVaild = true;
    if(!mImgGetter || !mImgGetter->init()) {
        imgGetterVaild = false;
        emit errorMsg(tr("ImgGetter is invaild!"));
        return;
    }

    while (!mStop) {
        mImgQueue->enqueue(mImgGetter->getImg());
    }
}

void fpsTest::ImgGetterThread::stop()
{
    mStop = true;
    mImgQueue->setBlock(false);
}
