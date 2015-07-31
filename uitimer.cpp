#include "uitimer.h"

UITimer::UITimer(QObject *parent) : QTimer(parent)
{
    pthread_mutex_init(&executeMutex, NULL);
    pthread_mutex_init(&executedMutex, NULL);
    handler = [](){};
    QObject::connect(this, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

UITimer::~UITimer()
{
    pthread_mutex_destroy(&executeMutex);
    pthread_mutex_destroy(&executedMutex);
}

void UITimer::onTimeout()
{
    pthread_mutex_lock(&executeMutex);
    handler();
    handler = [](){};
    pthread_mutex_unlock(&executedMutex);
    pthread_mutex_unlock(&executeMutex);
}
