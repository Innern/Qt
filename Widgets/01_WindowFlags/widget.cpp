#include "widget.h"
#include "previewwidget.h"

#include <QtWidgets>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    m_previewDlg = new PreviewWidget(this);

    QGroupBox *typeGroup = new QGroupBox(tr("Type"), this);
    windowButton = createRadioButton(tr("Window"));
    dialogButton = createRadioButton(tr("Dialog"));
    sheetButton = createRadioButton(tr("Sheet"));
    drawerButton = createRadioButton(tr("Drawer"), tr("The type has no effect."));
    popupButton = createRadioButton(tr("Popup"));
    toolButton = createRadioButton(tr("Tool"));
    tooltipButton = createRadioButton(tr("Tooltip"));
    splashScreenButton = createRadioButton(tr("Splash screen"));

    QGridLayout *typeLayout = new QGridLayout(this);
    typeLayout->addWidget(windowButton, 0,0);
    typeLayout->addWidget(dialogButton, 1,0);
    typeLayout->addWidget(sheetButton, 2,0);
    typeLayout->addWidget(drawerButton, 3,0);
    typeLayout->addWidget(popupButton, 0,1);
    typeLayout->addWidget(toolButton, 1,1);
    typeLayout->addWidget(tooltipButton, 2,1);
    typeLayout->addWidget(splashScreenButton, 3,1);
    typeGroup->setLayout(typeLayout);

    QGroupBox *hintsGroup = new QGroupBox(tr("Hints"), this);

    msWindowsFixedSizeDialog = createCheckBox(tr("MS Windows fixed size dialog"));
    msWindowsOwnDC = createCheckBox(tr("MS Windows Own DC"));
    byPassWindowManager = createCheckBox(tr("Bypass window manager"));
    x11ByPassWindowManager = createCheckBox(tr("X11 bypass window manager"));
    framelessWindow = createCheckBox(tr("Frameless Window"));
    noDropShadowWindow = createCheckBox(tr("No drop shadow window"));
    windowTitle = createCheckBox(tr("Window Title"));
    windowSystemMenu = createCheckBox(tr("Window System Menu"));
    customizeWindow = createCheckBox(tr("Customsize Window"));
    windowMinimumButton = createCheckBox(tr("Window minimum button"));
    windowMaximumButton = createCheckBox(tr("MWindow maximum button"));
    windowCloseButton = createCheckBox(tr("Window close button"));
    windowContextHelpButton = createCheckBox(tr("Window context help button"));
    macWindowToolBarButton = createCheckBox(tr("Window toolbar button"));
    windowFullScreenButton = createCheckBox(tr("Window full screen button"));
    windowShadeButton = createCheckBox(tr("Window shade button"));
    windowStaysOnTop = createCheckBox(tr("Window stays on top"));
    windowStaysOnBottom = createCheckBox(tr("Window stays on bottom"));

    QGridLayout *hintsLayout = new QGridLayout(this);

    hintsLayout->addWidget(msWindowsFixedSizeDialog, 0, 0);
    hintsLayout->addWidget(msWindowsOwnDC, 1, 0);
    hintsLayout->addWidget(byPassWindowManager, 2, 0);
    hintsLayout->addWidget(x11ByPassWindowManager, 3, 0);
    hintsLayout->addWidget(framelessWindow, 4, 0);
    hintsLayout->addWidget(noDropShadowWindow, 5, 0);
    hintsLayout->addWidget(windowTitle, 6, 0);
    hintsLayout->addWidget(windowSystemMenu, 7, 0);
    hintsLayout->addWidget(customizeWindow, 8, 0);
    hintsLayout->addWidget(windowMinimumButton, 0, 1);
    hintsLayout->addWidget(windowMaximumButton, 1, 1);
    hintsLayout->addWidget(windowCloseButton, 2, 1);
    hintsLayout->addWidget(windowContextHelpButton, 3, 1);
    hintsLayout->addWidget(macWindowToolBarButton, 4, 1);
    hintsLayout->addWidget(windowFullScreenButton, 5, 1);
    hintsLayout->addWidget(windowShadeButton, 6, 1);
    hintsLayout->addWidget(windowStaysOnTop, 7, 1);
    hintsLayout->addWidget(windowStaysOnBottom, 8, 1);

    hintsGroup->setLayout(hintsLayout);


    QPushButton *quitButton = new QPushButton(tr("Quit"), this);
    connect(quitButton, &QPushButton::clicked, this, &Widget::close);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(typeGroup);
    mainLayout->addWidget(hintsGroup);
    mainLayout->addWidget(quitButton);
    setLayout(mainLayout);

    setWindowTitle(tr("Window Flags"));

    windowButton->setChecked(true);  // 默认选择Window

    m_previewDlg->show();
}

Widget::~Widget()
{
}

void Widget::updatePreview()
{
    Qt::WindowFlags flags;
    if(windowButton->isChecked())
        flags = Qt::Window;
    else if(dialogButton->isChecked())
        flags = Qt::Dialog;
    else if(sheetButton->isChecked())
        flags = Qt::Sheet;
    else if(drawerButton->isChecked())
        flags = Qt::Drawer;
    else if(popupButton->isChecked())
        flags = Qt::Popup;
    else if(toolButton->isChecked())
        flags = Qt::Tool;
    else if(tooltipButton->isChecked())
        flags = Qt::ToolTip;
    else if(splashScreenButton->isChecked())
        flags = Qt::SplashScreen;

    if(msWindowsFixedSizeDialog->isChecked())
        flags |= Qt::MSWindowsFixedSizeDialogHint;  // 用于固定窗口大小
    if(msWindowsOwnDC->isChecked())
        flags |= Qt::MSWindowsOwnDC;
    if(byPassWindowManager->isChecked())
        flags |= Qt::BypassWindowManagerHint;
    if(x11ByPassWindowManager->isChecked())
        flags |= Qt::X11BypassWindowManagerHint;
    if(framelessWindow->isChecked())
        flags |= Qt::FramelessWindowHint;
    if(noDropShadowWindow->isChecked())
        flags |= Qt::NoDropShadowWindowHint;
    if(windowTitle->isChecked())
        flags |= Qt::WindowTitleHint;
    if(windowSystemMenu->isChecked())
        flags |= Qt::WindowSystemMenuHint;
    if(customizeWindow->isChecked())
        flags |= Qt::CustomizeWindowHint;
    if(windowMinimumButton->isChecked())
        flags |= Qt::WindowMinimizeButtonHint;
    if(windowMaximumButton->isChecked())
        flags |= Qt::WindowMaximizeButtonHint;
    if(windowCloseButton->isChecked())
        flags |= Qt::WindowCloseButtonHint;
    if(windowContextHelpButton->isChecked())
        flags |= Qt::WindowContextHelpButtonHint;
    if(macWindowToolBarButton->isChecked())
        flags |= Qt::MacWindowToolBarButtonHint;
    if(windowFullScreenButton->isChecked())
        flags |= Qt::WindowFullscreenButtonHint;
    if(windowShadeButton->isChecked())
        flags |= Qt::WindowShadeButtonHint;
    if(windowStaysOnBottom->isChecked())
        flags |= Qt::WindowStaysOnBottomHint;
    if(windowStaysOnTop->isChecked())
        flags |= Qt::WindowStaysOnTopHint;


    m_previewDlg->setWindowFlags(flags);

    m_previewDlg->move(0,0);

    m_previewDlg->show();
}

QRadioButton *Widget::createRadioButton(const QString &text, const QString &tooltip)
{
    QRadioButton *button = new QRadioButton(text, this);
    if(!tooltip.isEmpty())
        button->setToolTip(tooltip);
    connect(button, &QRadioButton::clicked, this, &Widget::updatePreview);
    return button;
}

QCheckBox *Widget::createCheckBox(const QString &text, const QString &tooltip)
{
    QCheckBox *checkbox = new QCheckBox(text, this);
    if(!tooltip.isEmpty())
        checkbox->setToolTip(tooltip);
    connect(checkbox, &QCheckBox::clicked, this, &Widget::updatePreview);
    return checkbox;

}

