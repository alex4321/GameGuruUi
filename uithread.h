#ifndef UITHREAD_H
#define UITHREAD_H

#include <QApplication>
#include "uitimer.h"
#include <functional>
#include <pthread.h>

/*
 * GUI operations thread management object
 */
class UIThread
{
private:
    pthread_t thread;
    pthread_mutex_t baseReady;
    QApplication* app;
    UITimer* timer;
    static UIThread* instance;
    QString getAppPath();
    UIThread();
    std::function<void()> exec;
protected:
    static void* run(void*);
public:
    static UIThread* get();
    ~UIThread();
    void start();
    void execute(std::function<void()> func);
};

#endif // UITHREAD_H

