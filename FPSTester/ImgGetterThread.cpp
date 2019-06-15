#include "ImgGetterThread.h"
#include "BlockQueue.h"
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <QDir>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include <QDebug>

using namespace cv;
using std::shared_ptr;

fpsTest::ImgGetterThread::ImgGetterThread(QObject *parent)
    : QThread (parent)
    , mStop (false)
    , mImgQueue(new BlockQueue(8U))
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
    if(mImgDir.isNull()) {
        emit errorMsg(tr("Image dir is not set!"));
        return;
    }
    QDir dir(mImgDir);

    dir.setNameFilters({"*.bmp", "*.jpeg", "*.jpg", "*.png"});
    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Name);

    auto imgFiles = dir.entryList();
    if(imgFiles.size() <= 0) {
        emit errorMsg(tr("No image file!"));
        return;
    }
    qInfo("There is %d image files.", imgFiles.size());

    int i = 0;
    while (!mStop) {
        QString imgFile = imgFiles[i];
        i = (i + 1) % imgFiles.size();

        Mat img = imread(imgFile.toStdString());
        mImgQueue->enqueue(std::move(img));
        Q_UNUSED(img)
    }
}

void fpsTest::ImgGetterThread::stop()
{
    mStop = true;
    mImgQueue->setBlock(false);
}
