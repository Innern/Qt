#include "previewwidget.h"

#include <QtWidgets>

PreviewWidget::PreviewWidget(QWidget *parent)
    : QWidget{parent}
{
    textEdit = new QTextEdit(this);
    QPushButton *closeButton = new QPushButton(tr("&Close"), this);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(textEdit);
    mainLayout->addWidget(closeButton);

    setLayout(mainLayout);

    setWindowTitle(tr("Preview Window"));
}

void PreviewWidget::setWindowFlags(Qt::WindowFlags flags)
{
    QWidget::setWindowFlags(flags);

    QString text;

    Qt::WindowFlags type = (flags & Qt::WindowType_Mask);
    if(type == Qt::Window)
        text = tr("Qt::Window");
    else if(type == Qt::Dialog)
        text = tr("Qt::Dialog");
    else if(type == Qt::Sheet)
        text = tr("Qt::Sheet");
    else if(type == Qt::Drawer)
        text = tr("Qt::Drawer");
    else if(type == Qt::Popup)
        text = tr("Qt::Popup");
    else if(type == Qt::Tool)
        text = tr("Qt::Tool");
    else if(type == Qt::ToolTip)
        text = tr("Qt::ToolTip");
    else if(type == Qt::SplashScreen)
        text = tr("Qt::SplashScreen");

    if(flags & Qt::MSWindowsFixedSizeDialogHint)
        text += tr("\n  | Qt::MSWindowsFixedSizeDialogHint");
    if(flags & Qt::MSWindowsOwnDC)
        text += tr("\n  | Qt::MSWindowsOwnDC");
    if(flags & Qt::BypassGraphicsProxyWidget)
        text += tr("\n  | Qt::BypassGraphicsProxyWidget");
    if(flags & Qt::X11BypassWindowManagerHint)
        text += tr("\n  | Qt::X11BypassWindowManagerHint");
    if(flags & Qt::FramelessWindowHint)
        text += tr("\n  | Qt::FramelessWindowHint");
    if(flags & Qt::NoDropShadowWindowHint)
        text += tr("\n  | Qt::NoDropShadowWindowHint");
    if(flags & Qt::WindowTitleHint)
        text += tr("\n  | Qt::WindowTitleHint");
    if(flags & Qt::WindowSystemMenuHint)
        text += tr("\n  | Qt::WindowSystemMenuHint");
    if(flags & Qt::CustomizeWindowHint)
        text += tr("\n  | Qt::CustomizeWindowHint");
    if(flags & Qt::WindowMinimizeButtonHint)
        text += tr("\n  | Qt::WindowMinimizeButtonHint");
    if(flags & Qt::WindowMaximizeButtonHint)
        text += tr("\n  | Qt::WindowMaximizeButtonHint");
    if(flags & Qt::WindowCloseButtonHint)
        text += tr("\n  | Qt::WindowCloseButtonHint");
    if(flags & Qt::WindowContextHelpButtonHint)
        text += tr("\n  | Qt::WindowContextHelpButtonHint");
    if(flags & Qt::MacWindowToolBarButtonHint)
        text += tr("\n  | Qt::MacWindowToolBarButtonHint");
    if(flags & Qt::WindowFullscreenButtonHint)
        text += tr("\n  | Qt::WindowFullscreenButtonHint");
    if(flags & Qt::WindowShadeButtonHint)
        text += tr("\n  | Qt::WindowShadeButtonHint");
    if(flags & Qt::WindowStaysOnBottomHint)
        text += tr("\n  | Qt::WindowStaysOnBottomHint");
    if(flags & Qt::WindowStaysOnTopHint)
        text += tr("\n  | Qt::WindowStaysOnTopHint");


    textEdit->setPlainText(text);
}
