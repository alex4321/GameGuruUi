#include "fpscui.h"
#include "uithread.h"
#include "uilayer.h"

uint UILayer::lastId = 0;
QMap<uint, UILayer*> UILayer::layers = QMap<uint, UILayer*>();


/*
 * Dll entry point
 */
bool WINAPI DllMain(HINSTANCE, DWORD reason, LPVOID )
{
    switch(reason)
    {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_PROCESS_DETACH:
        UILayer::deleteAll();
        UIThread::get()->execute([]() {
            QApplication::exit(0);
        });
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    }
    return true;
}


#include "lua.hpp"
const QString luaNumberType = "double";
const QString luaStringType = "QString";

extern "C"
{
    /*
     * Initialize UI Library.
     */
    static int initialize(lua_State*)
    {
        UIThread::get();
        return 1;
    }

    /*
     * Create UI layer
     *
     * must be called from Lua as 'int fpscUiShowDialog(string layername, table variables)'
     * where UI files will be Files\uilayerbank\%layername%\*.html
     * variables will be binded to showed html elements
     * return layer id
     */
    static int createLayer(lua_State* state)
    {
        //Check call signature
        const char* signature_error_message = "Signature is : int showDialog(string layername, table variables)";
        const char* variables_error_message = "Wrong variables : now support only string keys and number/string values";
        if(!lua_isstring(state, -2))
        {
            lua_pushstring(state, signature_error_message);
            lua_error(state);
        }
        if(!lua_istable(state, -1))
        {
            lua_pushstring(state, signature_error_message);
            lua_error(state);
        }

        //Build layer
        const char* name = lua_tostring(state, -2);
        UILayer* layer = new UILayer(QString(name));
        if(!layer->table.fillFromLua(state, -1))
        {
            lua_pushstring(state, variables_error_message);
            lua_error(state);
        }

        //Return layer id
        lua_pushnumber(state, layer->getId());
        return 1;
    }

    /*
     * Show UI layer
     *
     * must be called from Lua as 'bool fpscUiShowLayer(int id)'
     */
    static int showLayer(lua_State* state)
    {
        //Check signature
        const char* signature_error_message = "Signature is : bool showDialog(int id)";
        if(!lua_isnumber(state, -1))
        {
            lua_pushstring(state, signature_error_message);
            lua_error(state);
        }

        //Get and show layer
        uint id = (uint)lua_tonumber(state, -1);
        UILayer* layer = UILayer::getLayer(id);
        if(layer)
        {
            layer->show();
            lua_pushboolean(state, 1);
        }
        else
        {
            lua_pushboolean(state, 0);
        }
        return 1;
    }

    /*
     * Show UI layer
     *
     * must be called from Lua as 'bool fpscUiShowLayer(int id)'
     */
    static int showModalLayer(lua_State* state)
    {
        //Check signature
        const char* signature_error_message = "Signature is : bool showModalDialog(int id)";
        if(!lua_isnumber(state, -1))
        {
            lua_pushstring(state, signature_error_message);
            lua_error(state);
        }

        //Get and show modal layer
        uint id = (uint)lua_tonumber(state, -1);
        UILayer* layer = UILayer::getLayer(id);
        if(layer)
        {
            layer->showModal();
            lua_pushboolean(state, 1);
        }
        else
        {
            lua_pushboolean(state, 0);
        }
        return 1;
    }

    /*
     * Hide UI layer
     *
     * must be called from Lua as 'bool fpscUiHideLayer(int id)'
     */
    static int hideLayer(lua_State* state)
    {
        //Check signature
        const char* signature_error_message = "Signature is : bool hideDialog(int id)";
        if(!lua_isnumber(state, -1))
        {
            lua_pushstring(state, signature_error_message);
            lua_error(state);
        }

        //Get and hide layer
        uint id = (uint)lua_tonumber(state, -1);
        UILayer* layer = UILayer::getLayer(id);
        if(layer)
        {
            layer->hide();
            lua_pushboolean(state, 1);
        }
        else
        {
            lua_pushboolean(state, 0);
        }
        return 1;
    }

    /*
     * Destroys layer
     * Can be used, e.g. - for "dailog" modal layers
     */
    static int destroyLayer(lua_State* state)
    {
        //Check signature
        const char* signature_error_message = "Signature is : bool hideDialog(int id)";
        if(!lua_isnumber(state, -1))
        {
            lua_pushstring(state, signature_error_message);
            lua_error(state);
        }

        uint id = (uint)lua_tonumber(state, -1);
        UILayer::deleteLayer(id);

        return 1;
    }


    /*
     * Get UI layer binded variable
     *
     * must be called from Lua as 'variant fpscUiGetvariable(int id, string name)'
     */
    static int getVariable(lua_State* state)
    {
        //Check signature
        const char* signature_error_message = "Signature is : variant getVariable(int id, string name)";
        if(!lua_isnumber(state, -2))
        {
            lua_pushstring(state, signature_error_message);
            lua_error(state);
        }
        if(!lua_isstring(state, -1))
        {
            lua_pushstring(state, signature_error_message);
            lua_error(state);
        }

        //Get layer
        uint id = (uint)lua_tonumber(state, -2);
        const char* name = lua_tostring(state, -1);
        UILayer* layer = UILayer::getLayer(id);
        if(layer)
        {
            //and get variable from layer
            QVariant variable = layer->getVariable(QString(name));
            QString variableType = variable.typeName();
            if(variableType == luaNumberType)
            {
                lua_pushnumber(state, variable.toDouble());
            }
            else if(variableType == luaStringType)
            {
                lua_pushstring(state, variable.toString().toStdString().c_str());
            }
            else
            {
                QString message = "Wrong type : " + variableType;
                lua_pushstring(state, message.toStdString().c_str());
                lua_error(state);
            }
        }
        else
        {
            lua_pushnil(state);
        }
        return 1;
    }


    /*
     * Set UI layer binded variable
     *
     * must be called from Lua as 'variant fpscUiSetvariable(int id, string name, string|number value)'
     */
    static int setVariable(lua_State* state)
    {
        //Check signature
        const char* signature_error_message = "Signature is : variant setVariable(int id, string name, number|string value)";
        if(!lua_isnumber(state, -3))
        {
            lua_pushstring(state, signature_error_message);
            lua_error(state);
        }
        if(!lua_isstring(state, -2))
        {
            lua_pushstring(state, signature_error_message);
            lua_error(state);
        }
        if( (!lua_isnumber(state, -1)) && (!lua_isstring(state, -1)))
        {
            lua_pushstring(state, signature_error_message);
            lua_error(state);
        }

        //Get layer
        uint id = (uint)lua_tonumber(state, -3);
        const char* name = lua_tostring(state, -2);
        UILayer* layer = UILayer::getLayer(id);
        if(layer)
        {
            //and set variable
            if(lua_isnumber(state, -1))
            {
                lua_Number value = lua_tonumber(state, -1);
                layer->setVariable(QString(name), QVariant(value));
            }
            else if(lua_isstring(state, -1))
            {
                const char* value = lua_tostring(state, -1);
                layer->setVariable(QString(name), QVariant(QString(value)));
            }
        }

        return 1;
    }

    int __declspec(dllexport) luaopen_fpscui(lua_State* state)
    {
        lua_register(state, "fpscUiInitialize", initialize);
        lua_register(state, "fpscUiCreateLayer", createLayer);
        lua_register(state, "fpscUiShowLayer", showLayer);
        lua_register(state, "fpscUiShowModalLayer", showModalLayer);
        lua_register(state, "fpscUiHideLayer", hideLayer);
        lua_register(state, "fpscUiDestroyLayer", destroyLayer);
        lua_register(state, "fpscUiGetVariable", getVariable);
        lua_register(state, "fpscUiSetVariable", setVariable);
        return 1;
    }
}
