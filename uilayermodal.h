#ifndef UILAYERMODAL_H
#define UILAYERMODAL_H

#include <QWidget>
#include <QCloseEvent>

class UILayerModal : public QWidget
{
private:
    static UILayerModal* instance;
    UILayerModal();
    void closeEvent(QCloseEvent* e);
    bool showed;
public:
    static UILayerModal* get();
    void show();
    void hide();
    bool isShown();
};

#endif // UILAYERMODAL_H
