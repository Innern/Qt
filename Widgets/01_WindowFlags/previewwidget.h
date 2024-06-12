#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE

class PreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewWidget(QWidget *parent = nullptr);
    void setWindowFlags(Qt::WindowFlags flags);
signals:

private:
    QTextEdit *textEdit;
};

#endif // PREVIEWWIDGET_H
