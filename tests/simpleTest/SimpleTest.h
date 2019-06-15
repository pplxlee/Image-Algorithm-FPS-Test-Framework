#ifndef SIMPLETEST_H
#define SIMPLETEST_H

#include <QObject>
#include "FPSTester.h"

using fpsTest::FPSTester;

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

signals:

public slots:

private:
    FPSTester mFPSTester;
};

#endif // SIMPLETEST_H
