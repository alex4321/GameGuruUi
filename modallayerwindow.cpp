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
    QString title = Helper::getActiveWindowTitle();
    if(title.indexOf("fpscuiwindow")>-1 || title.indexOf("Guru-Map Editor")>-1)
    {
        return;
    }
    if(Helper::isFpscWindowActive())
    {
        QWidget::show();
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
}
