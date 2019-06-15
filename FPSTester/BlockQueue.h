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
        mCond.wakeAll();
    }

    // 若达到maxsize，则阻塞；dequeue会唤醒。
    inline void enqueue(const cv::Mat& value)
    {
        QMutexLocker locker(&mLock);
        if(mQueue.size() >= mMaxSize)
            mCond.wait(&mLock);
        mBlock ? mQueue.push(value) : void();
    }


    // 默认在运行时，队列几乎不会空
    // 尽可能减少读取线程的挂起切换和阻塞
    inline cv::Mat dequeue()
    {
        cv::Mat res;
        // 不用lock()的原因是尽可能避免读取线程的挂起切换
        // 参考自旋锁概念
        // 不断的循环查看，直到不阻塞或者占有锁
        while(mBlock && !mLock.tryLock()) {}

        if(!mQueue.empty()) {
            res = mQueue.front();
            mQueue.pop();
        }

        mCond.wakeAll(); // 唤醒其他线程
        mLock.unlock();
        return res;
    }

private:
    bool mBlock; // false 则停止阻塞
    uint mMaxSize;
    std::queue<cv::Mat> mQueue;
    QMutex mLock;
    QWaitCondition mCond;
};
}


#endif // BLOCKQUEUE_H
