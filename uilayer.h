#ifndef UILAYER_H
#define UILAYER_H

#include <QObject>
#include <QMap>
#include <QVector>
#include "table.h"
#include "uiblock.h"

/*
 * UI layer
 */
class UILayer : public QObject
{
    Q_OBJECT
private slots:
    void bindingUpdated(QString key, QObject* updater);
    void bindingFilled();
private:
    static QMap<uint, UILayer*> layers;
    QVector<UIBlock*> blocks;
    static uint lastId;
    uint id;
    QString getPath(const QString& name);
    QString cursorPath;
    void loadLayerConfig(const QString& configPath, const QString& layerDir);
    void loadBlock(const QString& path);
public:
    UILayer(const QString& fileName);
    ~UILayer();
    uint getId();
    Table table;

    static UILayer* getLayer(uint id);
    static void deleteLayer(uint id);
    static void deleteAll();
    void show();
    void showModal();
    void hide();
    QVariant getVariable(const QString& name);
    void setVariable(const QString& name, QVariant value);
};

#endif // UILAYER_H
