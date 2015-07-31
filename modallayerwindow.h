#ifndef MODALLAYERWINDOW_H
#define MODALLAYERWINDOW_H

#include <QWidget>
#include <QTimer>

class ModalLayerWindow : public QWidget
{
    Q_OBJECT
private:
    QPixmap screenshot;
    void paintEvent(QPaintEvent *);
    QTimer* timer;
private slots:
    void updateWindowState();
public:
    ModalLayerWindow();
    void show();
    void activate();
};

#endif // MODALLAYERWINDOW_H
