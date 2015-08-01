#include "uiblock.h"
#include "helper.h"
#include <QUrl>
#include <QWebFrame>
#include <QWebElement>
#include <QScreen>
#include <QGuiApplication>
#include <QFileInfo>
#include <QDir>
#include <QPixmap>
#include <QBitmap>
#ifdef _WIN32
    #include <windows.h>
#endif

/**
 * @brief UIBlock::UIBlock Initialize UI block
 * @param fileName source html file name
 * @param variables bind to this table
 */
UIBlock::UIBlock(const QString& fileName, Table* variables, QObject*)
    : QWebView(NULL)
{
    setContextMenuPolicy(Qt::NoContextMenu);
    //Remove frame border
    setWindowFlags(
                this->windowFlags() | Qt::FramelessWindowHint
                );
    //Not must be shown
    activated = false;
    modalActive = false;
    path = fileName;
    this->variables = variables;
    variablesBinding = new TableBinding(variables, this);

    //Will hide it from taskbar and hotkey (alt+tab, win+tab in windows)
    // window switchers
    setWindowFlags(this->windowFlags() | Qt::Tool);
    //Set title to determine, that it's one of out windows
    setWindowTitle("fpscuiwindow");

    //Load file
    load(QUrl::fromLocalFile(fileName));
    //Direct "asynchronius" connection not called 'onLoadFinished',
    // so I maked synchronius
    QEventLoop loop;
    connect(this, SIGNAL(loadFinished(bool)), &loop, SLOT(quit()));
    loop.exec();
    onLoadFinished(true);
    page()->mainFrame()->setScrollBarPolicy( Qt::Vertical, Qt::ScrollBarAlwaysOff );
    page()->mainFrame()->setScrollBarPolicy( Qt::Horizontal, Qt::ScrollBarAlwaysOff );

    //Used to update window state information while idle
    // e.g. if user switched from game to other window - it'll hide out windows
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkWindowState()));
    timer->start(0);
}

/**
 * @brief UIBlock::updateSize resize by body tag parameters
 */
void UIBlock::updateSize()
{
    QWebFrame* frame = this->page()->mainFrame();
    QWebElement body = frame->findFirstElement("body");
    bool widthOk;
    int width = body.attribute("data-width", "0").toInt(&widthOk);
    if(!widthOk)
    {
        return;
    }
    bool heightOk;
    int height = body.attribute("data-height", "0").toInt(&heightOk);
    if(!heightOk)
    {
        return;
    }
    resize(width, height);
}

/**
 * @brief UIBlock::updatePosition set position by body tag parameters
 */
void UIBlock::updatePosition()
{
    QWebFrame* frame = this->page()->mainFrame();
    QWebElement body = frame->findFirstElement("body");
    bool xOk;
    int x = body.attribute("data-x", "0").toInt(&xOk);
    if(!xOk)
    {
        return;
    }
    bool yOk;
    int y = body.attribute("data-y", "0").toInt(&yOk);
    if(!yOk)
    {
        return;
    }
    QString alignX = body.attribute("data-alignment-x", "left");
    bool alignXOk = alignX=="left"||alignX=="right"||alignX=="center";
    if(!alignXOk)
    {
        return;
    }
    QString alignY = body.attribute("data-alignment-y", "top");
    bool alignYOk = alignY=="top"||alignY=="bottom"||alignY=="center";
    if(!alignYOk)
    {
        return;
    }
    QRect windowRect = Helper::gameWindowRect();
    QRect rect;
    rect.setWidth(width());
    rect.setHeight(height());
    if(alignX=="left")
    {
        rect.setLeft(windowRect.left() + x);
    }
    else if(alignX=="right")
    {
        rect.setLeft(windowRect.left() + windowRect.width() - rect.width() - x);
    }
    else
    {
        rect.setLeft(windowRect.left() + windowRect.width() / 2 - rect.width() /2 + x);
    }

    if(alignY=="top")
    {
        rect.setTop(windowRect.top() + y);
    }
    else if(alignY=="bottom")
    {
        rect.setTop(windowRect.top() + windowRect.height() - rect.height() - y);
    }
    else
    {
        rect.setTop(windowRect.top() + windowRect.height() / 2 - rect.height() / 2 + y);
    }
    move(rect.left(), rect.top());
}

/**
 * @brief UIBlock::updateGeometry set window rect by mask from image (from body parameters)
 */
void UIBlock::updateGeometry()
{
    QWebFrame* frame = this->page()->mainFrame();
    QWebElement body = frame->findFirstElement("body");
    QString cutImage = body.attribute("data-cutby", "");
    QDir directory = QFileInfo(path).absoluteDir();
    QString imagePath = directory.absoluteFilePath(cutImage);
    if(!QFileInfo(imagePath).exists())
    {
        return;
    }
    QPixmap image;
    if(!image.load(imagePath))
    {
        return;
    }
    setMask(image.mask());
}

void UIBlock::initializeBindings()
{
    page()->mainFrame()->addToJavaScriptWindowObject("variables", variablesBinding);
}

void UIBlock::initializeInfo()
{
}

void UIBlock::onLoadFinished(bool)
{
    updateSize();
    updatePosition();
    updateGeometry();
    initializeBindings();
}

void UIBlock::bindingUpdate(QString key, QObject* by)
{
    if(by == (QObject*)&variablesBinding)
    {
        return;
    }
    page()->mainFrame()->evaluateJavaScript("updateBinding('" + key + "');");
}

void UIBlock::bindingFilled()
{
    page()->mainFrame()->evaluateJavaScript("filledBindings();");
}

void UIBlock::checkWindowState()
{
    updateSize();
    if(modalActive == true)
    {
        return;
    }
    if(GetForegroundWindow() == (HWND)winId())
    {
        return;
    }
    bool show = Helper::isFpscWindowActive();
    if(show && activated)
    {
        QWebView::show();
        if(modalActive)
        {
            activateWindow();
        }
    }
    else if(!show && activated)
    {
        QWebView::hide();
    }
}

void UIBlock::showNonModal()
{
    modalActive = false;
    activated = true;
    setWindowFlags( (windowFlags() & ~Qt::Tool) | Qt::Window);
    QWebView::show();
    //Show as topmost
    setWindowFlags(
                this->windowFlags() | Qt::WindowStaysOnTopHint
                );
    setWindowFlags( (windowFlags() & ~Qt::Window) | Qt::Tool);
}

void UIBlock::showModal()
{
    modalActive = true;
    updateSize();
    updatePosition();
    QWebView::show();
}

void UIBlock::hide()
{
    activated = false;
    QWebView::hide();
}

void UIBlock::switchWindow()
{
    activateWindow();
}
