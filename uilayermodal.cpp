#include "uilayermodal.h"

UILayerModal* UILayerModal::instance = NULL;

UILayerModal::UILayerModal()
{
    showed = false;
    setWindowTitle("fpscuiwindow");
}

UILayerModal* UILayerModal::get()
{
    if(UILayerModal::instance == NULL)
    {
        UILayerModal::instance = new UILayerModal();
    }
    return UILayerModal::instance;
}

void UILayerModal::show()
{
    showed = true;
    QWidget::showFullScreen();
}

void UILayerModal::hide()
{
    showed = false;
    QWidget::hide();
}

bool UILayerModal::isShown()
{
    return showed;
}

void UILayerModal::closeEvent(QCloseEvent* e)
{
    showed = false;
    e->ignore();
}
