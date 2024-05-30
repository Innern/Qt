#ifndef RAWINFODIALOG_H
#define RAWINFODIALOG_H

#include <QDialog>

namespace Ui {
class RawInfoDialog;
}

class RawInfoDialog : public QDialog
{
    Q_OBJECT
public:
    enum RawBayer{Bayer_RGGB, Bayer_GRBG, Bayer_GBRG, Bayer_BGGR};
    enum RawBits{Bits_Raw8, Bits_Raw10, Bits_Raw12, Bits_Raw16};
public:
    explicit RawInfoDialog(QWidget *parent = nullptr);
    ~RawInfoDialog();

private slots:
    void on_widthSpinBox_valueChanged(int arg1);
    void on_heightSpinBox_valueChanged(int arg1);
    void on_bayerComboBox_activated(int index);

    void on_bitsComboBox_activated(int index);

private:
    Ui::RawInfoDialog *ui;

public:
    int m_width;
    int m_height;
    RawBayer m_bayer;
    RawBits m_bits;
};

#endif // RAWINFODIALOG_H
