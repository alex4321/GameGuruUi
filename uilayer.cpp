#include "uilayer.h"
#include "modallayerwindow.h"
#include "uithread.h"
#include <QDir>
#include <QFileInfo>
#include <QPixmap>
#include <QCursor>
#include <QSettings>

#ifdef _WIN32
#include <windows.h>
#endif

QString UILayer::getPath(const QString& name)
{
#ifdef _WIN32
    char modulename[MAX_PATH];
    GetModuleFileNameA(NULL, modulename, MAX_PATH);
    QString path = QString(modulename);
    QFileInfo info(path);
    QDir dir = info.absoluteDir();
    return dir.absolutePath() + "/Files/uilayerbank/" + name;
#else
    return "/Files/uilayerbank/" + name;
#endif
}

UILayer::UILayer(const QString& name) : QObject(NULL)
{
    UILayer::lastId++;
    this->id = UILayer::lastId;
    UILayer::layers.insert(this->id, this);

    cursorPath = "";
    QDir dir(this->getPath(name));
    if(dir.exists())
    {
        QString configPath = getPath(name + "/config.ini");
        if(QFileInfo(configPath).exists())
        {
            loadLayerConfig(configPath, dir.absolutePath());
        }

        QStringList filters {"*.html"};
        for(QFileInfo item: dir.entryInfoList(filters))
        {
            this->loadBlock(item.absoluteFilePath());
        }
    }
    QObject::connect(&table, SIGNAL(updated(QString,QObject*)), this, SLOT(bindingUpdated(QString,QObject*)));
    QObject::connect(&table, SIGNAL(filled()), this, SLOT(bindingFilled()));
}

void UILayer::loadLayerConfig(const QString& configPath, const QString& layerDir)
{
    QSettings settings(configPath, QSettings::IniFormat);
    settings.beginGroup("Layer");
    QString configuredCursorPath = settings.value("cursor", "").toString();
    if(configuredCursorPath != "")
    {
        if(QFileInfo(layerDir + "/" + configuredCursorPath).exists())
        {
            cursorPath = layerDir + "/" + configuredCursorPath;
        }
    }
}

void UILayer::loadBlock(const QString &path)
{
    UIThread::get()->execute([this, path]() {
        UIBlock* block = new UIBlock(path, &table);
        blocks.append(block);
    });
}

UILayer::~UILayer()
{
    UIThread::get()->execute([this]() {
        for(UIBlock* block : blocks)
        {
            delete block;
        }
    });
}

uint UILayer::getId()
{
    return this->id;
}

UILayer* UILayer::getLayer(uint id)
{
    if(UILayer::layers.keys().contains(id))
    {
        return UILayer::layers[id];
    }
    else
    {
        return NULL;
    }
}

void UILayer::show()
{
    UIThread::get()->execute([this]() {
        for(UIBlock* block : this->blocks)
        {
            block->showNonModal();
        }
    });
}

void UILayer::hide()
{
    UIThread::get()->execute([this]() {
        for(UIBlock* block : this->blocks)
        {
            block->hide();
        }
    });
}

void UILayer::showModal()
{
    UIThread::get()->execute([this]() {
        table.update("modalOpened", QVariant(1), NULL);
        ModalLayerWindow window;
        if(cursorPath != "")
        {
            QPixmap image;
            image.load(cursorPath);
            QApplication::setOverrideCursor(QCursor(image));
        }
        window.show();
        for(UIBlock* block: blocks)
        {
            block->setParent(&window);
            block->showModal();
        }
        QEventLoop loop;
        while(1)
        {
            QObject::connect(&table, SIGNAL(updated(QString,QObject*)), &loop, SLOT(quit()));
            loop.exec();
            if(table["modalOpened"].toInt() == 0)
            {
                break;
            }
        }
        QApplication::restoreOverrideCursor();
    });
}

void UILayer::bindingUpdated(QString key, QObject* updater)
{
    UIThread::get()->execute([this, key, updater]() {
        for(UIBlock* block : this->blocks)
        {
            block->bindingUpdate(key, updater);
        }
    });
}

void UILayer::bindingFilled()
{
    UIThread::get()->execute([this]() {
        for(UIBlock* block : this->blocks)
        {
            block->bindingFilled();
        }
    });
}

void UILayer::deleteAll()
{
    for(UILayer* layer: layers)
    {
        delete layer;
    }
}

QVariant UILayer::getVariable(const QString& name)
{
    return table[name];
}

void UILayer::setVariable(const QString& name, QVariant value)
{
    table.update(name, value, NULL);
}
