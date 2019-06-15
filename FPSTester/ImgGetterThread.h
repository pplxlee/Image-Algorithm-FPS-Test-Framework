#ifndef IMGGETTERTHREAD_H
#define IMGGETTERTHREAD_H

#include <QThread>
#include <memory>

namespace fpsTest {

class BlockQueue;

class ImgGetterThread : public QThread
{
    Q_OBJECT
public:
    explicit ImgGetterThread(QObject *parent = nullptr);
    ~ImgGetterThread();

    std::shared_ptr<BlockQueue> imgQueue();

public slots:
    void stop();

signals:
    void errorMsg(QString err);

protected:
    void run();

private:
    bool mStop;

    std::shared_ptr<BlockQueue> mImgQueue;

    QString mImgDir;

    friend class FPSTester;
};

}


#endif // IMGGETTERTHREAD_H
