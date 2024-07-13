/// 该类是一个菜单项，包含一个标签和一个行编辑器
/// 在行编辑器中输入字符，按下回车键后可以自动将字符输入到中心部件的文本编辑器中
#ifndef MYACTION_H
#define MYACTION_H

#include <QWidgetAction>

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

class MyAction : public QWidgetAction
{
    Q_OBJECT
public:
    explicit MyAction(QObject *parent = nullptr);

signals:
    void getText(const QString &text);
private slots:
    // 行编辑器按下回车信号与该函数关联
    void sendText();
protected:

    QWidget * createWidget(QWidget *parent) override;



private:
    QLineEdit *edit;
};

#endif // MYACTION_H
