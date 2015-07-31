#ifndef TABLE_H
#define TABLE_H

#include <QMap>
#include <QVariant>
#include "lua.hpp"

/*
 * Binded variables storage.
 * Can be used as QMap<QString, QVariant>, but not inherites it, because it need signal about binded variables change
 */
class Table: public QObject
{
    Q_OBJECT
private:
    QMap<QString, QVariant> map;
signals:
    void updated(QString key, QObject* updater);
    void filled();
public:
    void insert(const QString& key, QVariant value);
    QStringList keys();
    QVariant& operator[](const QString& index);
    bool fillFromLua(lua_State* state, int index);
    void update(QString key, QVariant value, QObject* updater = NULL);
};

#endif // TABLE_H

