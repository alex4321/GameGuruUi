#ifndef TABLEBINDING_H
#define TABLEBINDING_H

#include <QObject>
#include "table.h"

/*
 * Proxy object to pass binded variables to JavaScript
 */
class TableBinding : public QObject
{
    Q_OBJECT
private:
    Table* table;
public:
    TableBinding(Table* table, QObject* parent = NULL);
    Q_INVOKABLE QStringList keys();
    Q_INVOKABLE QVariant get(QString key);
    Q_INVOKABLE void set(QString key, QVariant value);
};

#endif // TABLEBINDING_H
