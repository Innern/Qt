/*
 * 设置IP输入验证器，仅IP格式允许输入
 *
 *
*/

#ifndef IPLINEEDIT_H
#define IPLINEEDIT_H

#include <QLineEdit>

class IPLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    IPLineEdit(QWidget *parent=0);
    ~IPLineEdit();

    void setText(const QString &text);
    QString text() const;

    void setStyleSheet(const QString &styleSheet);

protected:
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);

private:
    int getIndex(QLineEdit *pEdit);
    bool isTextValid(const QString &strIP);
private:
    QLineEdit *m_lineEdit[4];
    QString mainBackground;  // 设置背景颜色，使4个输入框连城一个输入框
};

#endif // IPLINEEDIT_H
