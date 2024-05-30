#include "rawinfodialog.h"
#include "ui_rawinfodialog.h"

RawInfoDialog::RawInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RawInfoDialog)
  , m_width(0)
  , m_height(0)
  , m_bayer(Bayer_RGGB)
  , m_bits(Bits_Raw8)
{
    ui->setupUi(this);
}

RawInfoDialog::~RawInfoDialog()
{
    delete ui;
}

void RawInfoDialog::on_widthSpinBox_valueChanged(int arg1)
{
    m_width = arg1;
}

void RawInfoDialog::on_heightSpinBox_valueChanged(int arg1)
{
    m_height = arg1;
}

void RawInfoDialog::on_bayerComboBox_activated(int index)
{
    m_bayer = (RawBayer)index;
}

void RawInfoDialog::on_bitsComboBox_activated(int index)
{
    m_bits = (RawBits)index;
}
