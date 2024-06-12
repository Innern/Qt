#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QRadioButton;
class QCheckBox;
QT_END_NAMESPACE

class PreviewWidget;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void updatePreview();
private:
    QRadioButton *createRadioButton(const QString &text, const QString &tooltip = QString());
    QCheckBox *createCheckBox(const QString &text, const QString &tooltip = QString());
private:
    PreviewWidget *m_previewDlg;

    QRadioButton *windowButton;
    QRadioButton *dialogButton;
    QRadioButton *sheetButton;
    QRadioButton *drawerButton;
    QRadioButton *popupButton;
    QRadioButton *toolButton;
    QRadioButton *tooltipButton;
    QRadioButton *splashScreenButton;
    QRadioButton *desktopButton;

    QCheckBox *msWindowsFixedSizeDialog;
    QCheckBox *msWindowsOwnDC;
    QCheckBox *byPassWindowManager;  // 禁用所有窗口管理器协议，根据运行的操作系统不同有差异
    QCheckBox *x11ByPassWindowManager;  // 完全绕过窗口管理器，导致一个完全不受管理的无边界窗口
    QCheckBox *framelessWindow; // 无边框的窗口
    QCheckBox *noDropShadowWindow;  // 无窗口阴影
    QCheckBox *windowTitle;  // 标题栏
    QCheckBox *windowSystemMenu;  // 窗口的系统菜单
    QCheckBox *customizeWindow;  // 自定义窗口
    QCheckBox *windowMinimumButton;  // 窗口最小化按钮
    QCheckBox *windowMaximumButton;  // 窗口最大化按钮
    QCheckBox *windowCloseButton;  // 窗口关闭按钮
    QCheckBox *windowContextHelpButton;  // 窗口帮助按钮
    QCheckBox *macWindowToolBarButton;  // 窗口工具栏按钮
    QCheckBox *windowFullScreenButton;  // 窗口全屏按钮
    QCheckBox *windowShadeButton;  // 阴影按钮代替最小化按钮
    QCheckBox *windowStaysOnTop;  // 顶级窗口
    QCheckBox *windowStaysOnBottom;  // 底层窗口
};
#endif // WIDGET_H
