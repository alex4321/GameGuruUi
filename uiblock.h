#ifndef UIBLOCK_H
#define UIBLOCK_H

#include <QTimer>
#include <QWebView>
#include "table.h"
#include "tablebinding.h"

/*
 * Single layer element (layer can content multiple of this)
 */
class UIBlock : public QWebView
{
    Q_OBJECT
private slots:
    void onLoadFinished(bool);
    void checkWindowState();
private:
    QTimer* timer;
    QString path;
    void updatePosition();
    void updateSize();
    void updateGeometry();
    void initializeBindings();
    void initializeInfo();

    Table* variables;
    TableBinding* variablesBinding;

    //is must be shown? (setted by show/hide)
    bool activated;
    //must be active item of modal layer?
    bool modalActive;
public:
    UIBlock(const QString& fileName, Table* variables, QObject* parent = NULL);
    void bindingUpdate(QString key, QObject* by);
    void bindingFilled();
    void showNonModal();
    void showModal();
    void hide();
    void switchWindow();
};

#endif // UIBLOCK_H
