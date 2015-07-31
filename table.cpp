#include "table.h"

/*
 * Fill binded variables from Lua
 * Table must be located at index position inside state stack
 */
bool Table::fillFromLua(lua_State *state, int index)
{
    for(lua_pushnil(state); lua_next(state, index-1); lua_pop(state, 1))
    {
        QString key = QString(lua_tostring(state, -2));
        QString valuetype = QString(lua_typename(state, lua_type(state, -1)));
        if(valuetype == "string")
        {
            QString value = lua_tostring(state, index);
            this->insert(key, QVariant(value));
        }
        else if(valuetype == "number")
        {
            double value = lua_tonumber(state, index);
            this->insert(key, QVariant(value));
        }
        else
        {
            return false;
        }
    }
    emit filled();
    return true;
}

/*
 * Insert variable at table
 */
void Table::insert(const QString &key, QVariant value)
{
    map.insert(key, value);
}

/*
 * Get table keys
 */
QStringList Table::keys()
{
    return map.keys();
}

/*
 * Set variable at emit update signal
 */
void Table::update(QString key, QVariant value, QObject* updater)
{
    (*this)[key] = value;
    emit updated(key, updater);
}

/*
 * Access by index
 */
QVariant& Table::operator [](const QString& index)
{
    return map[index];
}
