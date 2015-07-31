#ifndef UITIMER_H
#define UITIMER_H

#include <QTimer>
#include <functional>
#include <pthread.h>

/*
 * Used to execute operations in GUI thread
 */
class UITimer : public QTimer
{
    Q_OBJECT
public:
    pthread_mutex_t executeMutex;
    pthread_mutex_t executedMutex;
    explicit UITimer(QObject *parent = 0);
    ~UITimer();
    std::function<void()> handler;
signals:

public slots:
    void onTimeout();
};

#endif // UITIMER_H
