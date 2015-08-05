#include "helper.h"

#include <QScreen>
#include <QString>
#include <QGuiApplication>
#ifdef _WIN32
#include <windows.h>
#endif

QString Helper::getActiveWindowTitle()
{
#ifdef _WIN32
    //Get active window title
    HWND active = GetForegroundWindow();
    if(!active)
    {
        return "";
    }
    int length = GetWindowTextLength(active);
    if(!active)
    {
        return "";
    }
    char* c_title = new char[length+1];
    int res = GetWindowTextA(active, c_title, length+1);
    if(res == 0)
    {
        delete[] c_title;
        return "";
    }
    QString title(c_title);
    delete[] c_title;
    return title;
#else
    #error Not implemented now
#endif
}

bool Helper::isFpscWindowActive()
{
    QString title = Helper::getActiveWindowTitle();
    return (
            title.indexOf("Game Guru") != -1 ||
            title.indexOf("fpscuiwindow") != -1
            );
}

/*
 * TODO: use finding of GameGuru window to work in windowed mode;
 */
QRect Helper::gameWindowRect()
{
    QSize screen = QGuiApplication::primaryScreen()->availableSize();
    return QRect(0, 0, screen.width(), screen.height());
}
