#ifndef SIMPLETEST_H
#define SIMPLETEST_H

#include "FPSTester.h"
#include <QObject>
#include <opencv2/core.hpp>

using namespace fpsTest;


class SimpleImgGetter : public ImgGetter
{
public:
    SimpleImgGetter();

    bool init();
    cv::Mat getImg();

private:
    cv::Mat mSrcImg;
    cv::Rect mRect;
};


class SimpleAlgorithm : public Algorithm
{
public:
    SimpleAlgorithm() {}

    bool init();
    void update(cv::Mat &img);
};


class SimpleTest : public QObject
{
    Q_OBJECT
public:
    explicit SimpleTest(QObject *parent = nullptr);

    void init();

    /**
     * @brief start
     *   开始运行测试
     * @param msec
     *   测试时间，若<=0，则直到用户停止时菜结束测试。
     */
    void start(qint64 msec = 0);

private:
    FPSTester mFPSTester;
    SimpleImgGetter mImgGetter;
    SimpleAlgorithm mAlgorithm;
};




#endif // SIMPLETEST_H
