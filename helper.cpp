#include "helper.h"

#include <QScreen>
#include <QString>
#include <QGuiApplication>
#ifdef _WIN32
#include <windows.h>
#endif

bool Helper::isFpscWindowActive()
{
#ifdef _WIN32
    //Get active window title
    HWND active = GetForegroundWindow();
    if(!active)
    {
        return false;
    }
    int length = GetWindowTextLength(active);
    if(!active)
    {
        return false;
    }
    char* c_title = new char[length];
    GetWindowTextA(active, c_title, length);
    QString title(c_title);
    delete[] c_title;
    //TODO: Log title
    //and check it
    return (
            title.indexOf("Game Guru") != -1 ||
            title.indexOf("fpscuiwindow") != -1
            );
#else
    #error Not implemented now
#endif
}

/*
 * TODO: use finding of GameGuru window to work in windowed mode;
 */
QRect Helper::gameWindowRect()
{
    QSize screen = QGuiApplication::primaryScreen()->availableSize();
    return QRect(0, 0, screen.width(), screen.height());
}
