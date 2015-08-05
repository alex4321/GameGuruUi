#ifndef HELPER_H
#define HELPER_H

#include <QRect>

class Helper
{
public:
    static QString getActiveWindowTitle();
    static bool isFpscWindowActive();
    static QRect gameWindowRect();
};

#endif // HELPER_H
