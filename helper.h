#ifndef HELPER_H
#define HELPER_H

#include <QRect>

class Helper
{
public:
    static bool isFpscWindowActive();
    static QRect gameWindowRect();
};

#endif // HELPER_H
