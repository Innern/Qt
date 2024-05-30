#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();



private:
    void initCombox();
    void setInsertTextColor(QColor col);
private slots:

    void changeFont();

    void currentFontSizeChanged(const QString& indexStr);

    void getInputData();

    void createMenu(const QPoint &pos);

    void setPlainTextFont();

    void on_pushButton_selectColor_clicked();

signals:
    void sendText(const QString& str);




private:
    Ui::Dialog *ui;

    QColor m_fontColor;
    QString m_strFontSize;

public:
    QString m_strPlainTextContext;
};

#endif // DIALOG_H
