#include "tablebinding.h"

TableBinding::TableBinding(Table* table, QObject* parent) : QObject(parent)
{
    this->table = table;
}

QStringList TableBinding::keys()
{
    return table->keys();
}

QVariant TableBinding::get(QString key)
{
    if(table->keys().indexOf(key) == -1)
    {
        return QVariant(0);
    }
    return (*table)[key];
}

void TableBinding::set(QString key, QVariant value)
{
    if(table->keys().indexOf(key) == -1)
    {
        table->insert(key, QVariant(0));
    }
    table->update(key, value, this);
}
