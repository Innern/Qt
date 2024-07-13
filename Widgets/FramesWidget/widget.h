#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
class QPushButton;
class QLineEdit;
QT_END_NAMESPACE
class CompleterWidget;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void updateButtonClickTimes();
    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_buttonSymbol_currentIndexChanged(int index);

    void on_comboBox_tickPosition_currentIndexChanged(int index);

    void on_spinBox_tickInterval_valueChanged(int arg1);

    void on_spinBox_dial_notchesTarget_valueChanged(int arg1);

    void on_comboBox_textElideMode_currentIndexChanged(int index);

    void on_comboBox_segmentStyle_currentIndexChanged(int index);

    void on_digitCountSpinBox_valueChanged(int arg1);

    void on_modeComboBox_currentIndexChanged(int index);

private:
    QPushButton *createLineeditRightButton(QLineEdit *edit);

private:
    Ui::Widget *ui;
    CompleterWidget *completerWidget;
};
#endif // WIDGET_H
