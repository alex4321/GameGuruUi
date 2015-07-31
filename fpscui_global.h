#ifndef FPSCUI_GLOBAL_H
#define FPSCUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FPSCUI_LIBRARY)
#  define FPSCUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define FPSCUISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FPSCUI_GLOBAL_H
