#ifndef FPSTESTERTHREAD_H
#define FPSTESTERTHREAD_H

#include <QThread>

#include <QElapsedTimer>

namespace fpsTest {

class Algorithm;
class BlockQueue;

class FPSTesterThread : public QThread
{
    Q_OBJECT
public:
    explicit FPSTesterThread(QObject *parent = nullptr);
    ~FPSTesterThread();

    void setImageQueue(const std::shared_ptr<BlockQueue> &imgQueue);

public slots:
    void stop();

protected:
    void run();

signals:
    void updated(qint64 t, qint64 count);
    void errorMsg(QString msg);

public slots:

private:
    std::shared_ptr<BlockQueue> mImgQueue;
    Algorithm *mAlgorithm;

    bool mStop;
    QElapsedTimer mElapsTimer;
    qint64 mTestTime;
    qint64 mLastEmitTime;

    friend class FPSTester;
};

}

#endif // FPSTESTERTHREAD_H
