#include "SimpleTest.h"
#include <QDebug>
#include <opencv2/highgui.hpp>

using namespace cv;

SimpleTest::SimpleTest(QObject *parent) : QObject(parent)
{
}

void SimpleTest::init()
{
    mFPSTester.setImgGetter(&mImgGetter);
    mFPSTester.setAlgorithm(&mAlgorithm);
}

void SimpleTest::start(qint64 msec)
{
    qDebug()<<"SimpleTest start!";
    mFPSTester.setTestTime(msec);
    mFPSTester.start();
}

SimpleImgGetter::SimpleImgGetter()
    : mSrcImg(Mat(600, 800, CV_8UC3))
    , mRect(0, 0, 10, 10)
{
}

bool SimpleImgGetter::init()
{
    return true;
}

cv::Mat SimpleImgGetter::getImg()
{
    cv::Mat res = mSrcImg.clone();
    res(mRect) = Scalar(0, 255, 0);
    mRect.x = ++mRect.x % (res.cols - mRect.width);
    mRect.y = ++mRect.y % (res.rows - mRect.height);
    return res;
}

bool SimpleAlgorithm::init()
{
    return true;
}

void SimpleAlgorithm::update(Mat &img)
{
    cv::imshow("FPS Test", img);
    cv::waitKey(1);
}
