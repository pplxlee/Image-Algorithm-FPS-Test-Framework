#ifndef BLOCKQUEUE_H
#define BLOCKQUEUE_H

#include <queue>
#include <memory>
#include <opencv2/core.hpp>
#include <QMutex>
#include <QWaitCondition>
#include <QDir>

#include <QDebug>

namespace fpsTest {
class BlockQueue
{
public:
    BlockQueue(uint maxSize) : mBlock(true) , mMaxSize(maxSize) {}

    // 设置false解除阻塞，用于停止时调用
    inline void setBlock(bool flag)
    {
        mBlock = flag;
        mFullCond.wakeAll();
        mEmptyCond.wakeAll();
    }

    // 若达到maxsize，则阻塞；dequeue会唤醒。
    inline void enqueue(const cv::Mat& value)
    {
        if(!mBlock)
            return;
        QMutexLocker locker(&mLock);
        if(mQueue.size() >= mMaxSize)
            mFullCond.wait(&mLock);
        mBlock ? mQueue.push(value) : void();
        mEmptyCond.wakeAll();
    }


    inline cv::Mat dequeue()
    {
        cv::Mat res;

        if(!mBlock)
            return res;

        QMutexLocker locker(&mLock);
        if(mQueue.empty())
            mEmptyCond.wait(&mLock);
        res = mQueue.front();
        mQueue.pop();
        mFullCond.wakeAll(); // 唤醒其他线程
        return res;
    }

private:
    bool mBlock; // false 则停止阻塞
    uint mMaxSize;
    std::queue<cv::Mat> mQueue;
    QMutex mLock;
    QWaitCondition mFullCond, mEmptyCond;
};
}


#endif // BLOCKQUEUE_H
