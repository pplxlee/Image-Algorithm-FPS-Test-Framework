#ifndef FPSTESTER_H
#define FPSTESTER_H

#include "FPSTesterThread.h"
#include "ImgGetterThread.h"
#include <QObject>

namespace cv {
class Mat;
}

namespace fpsTest {

/**
 * @brief The FPSTester class
 * 算法帧率测试类
 * 本类提供本测试框架的直接调用API
 */
class FPSTester : public QObject
{
    Q_OBJECT
public:
    explicit FPSTester(QObject *parent = nullptr);

    /**
     * @brief setTestTime
     *   设置测试时长
     * @param time
     *   测试时长，单位毫秒
     */
    void setTestTime(qint64 time);
    /**
     * @brief setImgGetter
     *   设置图像获取接口
     * @param igt
     */
    void setImgGetter(ImgGetter *igt);
    /**
     * @brief setAlgorithm
     *   设置算法接口
     * @note 可以不设置算法，得到的结果相当于测试图像获取的帧率
     * @param alg
     */
    void setAlgorithm(Algorithm *alg);
    /**
     * @brief setSavePath
     *   TODO: 设置结果保存路径 (还没有实现)
     * @param path
     */
    void setSavePath(const QString &path);

    /**
     * @brief start
     * 开始测试
     */
    void start();
    /**
     * @brief stop
     * 停止测试
     */
    void stop();

signals:
    /**
     * @brief stopped
     * 测试停止时发出此信号。
     * 此信号由此类内部触发，请不要自己发射此信号。
     */
    void stopped();
    /**
     * @brief errorMsg
     * 发生错误时发出此信号。
     * 此信号由此类内部触发，请不要自己发射此信号。
     * @param msg
     * 错误信息。
     */
    void errorMsg(const QString& msg);

private slots:
    /**
     * @brief onTesterUpdated
     *   测试线程更新时触发
     * @param t 算法计时
     * @param count 算法已处理的帧数
     */
    void onTesterUpdated(qint64 t, qint64 count);
    void onTesterStopped();
    void onImgGetterStopped();

    void onStopped();

    void onErrorMsg(const QString& msg);

private:
    ImgGetterThread mImgGetThread;
    FPSTesterThread mFPSTestThread;

    QString mSavePath;

    qint64 mTestTime;
    qint64 mTestCount;
};


/**
 * @brief The ImgGetter class
 * 图像获取接口
 * 需要自己实现此接口的init(), getImg()方法
 */
class ImgGetter
{
public:
    ImgGetter() {}
    virtual ~ImgGetter() {}

    /**
     * @brief init
     * 初始化图像获取
     * @details
     * 如果算法速度可能超过图片的读取或产生速度，
     * 可以在此方法内预先加载待处理图像序列在内存中，
     * 避免图像的加载成为测试的速度瓶颈。
     * @return
     * true 成功
     * false 失败
     */
    virtual bool init() = 0;

    /**
     * @brief getImg
     * 获取每帧图像
     * @details
     * 框架测试算法帧率时，会循环调用此函数，获取每帧图像，
     * 写入一个线程安全的队列中，供算法测试线程使用。
     * @return
     * 获取图像
     */
    virtual cv::Mat getImg() = 0;
};

/**
 * @brief The Algorithm class
 */
class Algorithm
{
public:
    Algorithm() {}
    virtual ~Algorithm() {}

    /**
     * @brief init
     * 初始化算法接口
     * @return
     * true 成功
     * false 失败
     */
    virtual bool init() = 0;

    /**
     * @brief update
     * 算法处理新的一帧
     * @details
     *
     * @param img
     */
    virtual void update(cv::Mat& img) = 0;
};
}

#endif // FPSTESTER_H
