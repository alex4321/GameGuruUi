#include "uithread.h"
#include <cstdio>
#include <QFileInfo>
#include <QThread>
#ifdef _WIN32
#include <windows.h>
#endif

UIThread* UIThread::instance = NULL;

QString UIThread::getAppPath()
{
#ifdef _WIN32
    char modulename[MAX_PATH];
    GetModuleFileNameA(NULL, modulename, MAX_PATH);
    QString path = QString(modulename);
    QFileInfo info(path);
    return info.absoluteFilePath();
#else
    #error Not implemented now
#endif
}

UIThread::UIThread()
{
    pthread_mutex_init(&baseReady, NULL);
    this->start();
}

UIThread::~UIThread()
{
    pthread_mutex_destroy(&baseReady);
}

UIThread* UIThread::get()
{
    if(UIThread::instance == NULL)
    {
        UIThread::instance = new UIThread();
    }
    return UIThread::instance;
}

void UIThread::start()
{
    pthread_mutex_lock(&baseReady);
    pthread_create(&thread, NULL, &UIThread::run, NULL);
}

void* UIThread::run(void*)
{
    int argc = 1;
    QString appPath = UIThread::get()->getAppPath();
    const char* cPath = appPath.toStdString().c_str();
    const char** argv = &cPath;
    QApplication* app = UIThread::get()->app = new QApplication(argc, (char**)argv);
    app->setQuitOnLastWindowClosed(false);
    UITimer* timer = UIThread::get()->timer = new UITimer();
    timer->start(0);
    pthread_mutex_unlock(&UIThread::get()->baseReady);
    app->exec();
    pthread_exit(NULL);
    return NULL;
}

void UIThread::execute(std::function<void ()> func)
{
    pthread_mutex_lock(&baseReady);
    pthread_mutex_unlock(&baseReady);

    pthread_mutex_lock(&timer->executeMutex);
    timer->handler = func;
    pthread_mutex_lock(&timer->executedMutex);
    pthread_mutex_unlock(&timer->executeMutex);

    pthread_mutex_lock(&timer->executedMutex);
    pthread_mutex_unlock(&timer->executedMutex);
}
