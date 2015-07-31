#include "modallayerwindow.h"
#include "helper.h"
#include <QPixmap>
#include <QPainter>
#ifdef _WIN32
#include <windows.h>
#else
#error Not implemented now
#endif

ModalLayerWindow::ModalLayerWindow()
{
    setWindowTitle("fpscuiwindow");
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateWindowState()));
}

void ModalLayerWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, screenshot.width(), screenshot.height(), screenshot);
}

void ModalLayerWindow::updateWindowState()
{
#ifdef _WIN32
    if((WId)GetForegroundWindow() == winId())
    {
        return;
    }
#endif
    if(Helper::isFpscWindowActive())
    {
        QWidget::show();
        activate();
    }
    else
    {
        hide();
    }
}

void ModalLayerWindow::show()
{
    timer->start(0);
    #ifdef _WIN32
        screenshot = QPixmap::grabWindow((WId)GetForegroundWindow());
    #endif
    setWindowFlags( (windowFlags() & ~Qt::Tool) | Qt::Window | Qt::WindowStaysOnTopHint);
    showFullScreen();
    activate();
}

void ModalLayerWindow::activate()
{
    setWindowFlags( (windowFlags() & ~Qt::Tool) | Qt::Window | Qt::WindowStaysOnTopHint);
    raise();
    activateWindow();
    //setWindowFlags( (windowFlags() & ~Qt::Window) | Qt::Tool | Qt::WindowStaysOnTopHint);
}
