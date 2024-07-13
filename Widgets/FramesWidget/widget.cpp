#include "widget.h"
#include "completerwidget.h"
#include "ui_widget.h"

#include <QMenu>
#include <QMovie>
#include <QDebug>
#include <QCompleter>

static QString text = QString("QFontMetrics functions calculate the size"
                              " of characters and strings for a given font."
                              " There are three ways you can create a QFontMetrics object."
                              "这里是中文，检测是否能够正常显示。");

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , completerWidget(new CompleterWidget(this))
{
    ui->setupUi(this);
    setWindowTitle(tr("常用窗口部件"));
    ui->stackedWidget->setCurrentIndex(0);

    ui->label_picture->setPixmap(QPixmap(":/imgs/qtlogo.png"));

    QMovie *movie = new QMovie(":/imgs/flipable.gif");
    ui->label_movie->setMovie(movie);
    movie->start();

    connect(ui->checkBox_scaledContents, &QCheckBox::clicked,
            ui->label_picture, &QLabel::setScaledContents);
    connect(ui->checkBox_scaledContents, &QCheckBox::clicked,
            ui->label_movie, &QLabel::setScaledContents);

    ui->comboBox_textElideMode->addItem(tr("Elide Left"), Qt::ElideLeft);
    ui->comboBox_textElideMode->addItem(tr("Elide Right"), Qt::ElideRight);
    ui->comboBox_textElideMode->addItem(tr("Elide Middle"), Qt::ElideMiddle);
    ui->comboBox_textElideMode->addItem(tr("Elide None"), Qt::ElideNone);
    ui->comboBox->setCurrentIndex(Qt::ElideLeft);
    connect(ui->checkBox_wordWrap, &QCheckBox::clicked, ui->label_text, &QLabel::setWordWrap);

    ui->label_text->setText(text);

    ui->comboBox_segmentStyle->addItem(tr("Outline"),QLCDNumber::Outline);
    ui->comboBox_segmentStyle->addItem(tr("Filled"),QLCDNumber::Filled);
    ui->comboBox_segmentStyle->addItem(tr("Flat"),QLCDNumber::Flat);
    ui->comboBox_segmentStyle->setCurrentIndex(QLCDNumber::Outline);

    ui->digitCountSpinBox->setValue(7);
    ui->lcdNumber->setDigitCount(ui->digitCountSpinBox->value());
    ui->lcdNumber->display(ui->lineEdit_lcdNumber_display->text());

    ui->modeComboBox->addItem(tr("Hex"), QLCDNumber::Hex);
    ui->modeComboBox->addItem(tr("Dec"), QLCDNumber::Dec);
    ui->modeComboBox->addItem(tr("Oct"), QLCDNumber::Oct);
    ui->modeComboBox->addItem(tr("Bin"), QLCDNumber::Bin);
    ui->modeComboBox->setCurrentIndex(QLCDNumber::Hex);
    connect(ui->checkBox_smallDecimalPoint, &QCheckBox::clicked,
            ui->lcdNumber, &QLCDNumber::setSmallDecimalPoint);

    QMenu *menu = new QMenu(this);
    menu->addAction(tr("下拉菜单动作"));
    ui->pushButton_menus->setMenu(menu);

    ui->pushButton_2->setAutoRepeat(true);
    ui->pushButton_2->setAutoRepeatDelay(500);
    ui->pushButton_2->setAutoRepeatInterval(500);
//    ui->pushButton_2->setDown(true);  // 与autoRepeat属性一起使用，不用一直按着鼠标
    connect(ui->pushButton_2, &QPushButton::clicked, this, &Widget::updateButtonClickTimes);

    QStringList echoModeList;
    ui->comboBox->addItem(tr("Normal") , QLineEdit::Normal);
    ui->comboBox->addItem(tr("NoEcho") , QLineEdit::NoEcho);
    ui->comboBox->addItem(tr("Password") , QLineEdit::Password);
    ui->comboBox->addItem(tr("PasswordEchoOnEdit") , QLineEdit::PasswordEchoOnEdit);

    // 自动补全
    QStringList words;
    words << tr("Qt") << tr("Qt Creator") << tr("Qt Tools");
    QCompleter *completer = new QCompleter(words,this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit_complete->setCompleter(completer);

    // 自定义行编辑器组件
    QLineEdit *edit = new QLineEdit(ui->groupBox_customLineEdit);
    QPushButton *button = createLineeditRightButton(edit);
    QVBoxLayout *editLayout = new QVBoxLayout(ui->groupBox_customLineEdit);
    editLayout->addWidget(edit);

    connect(ui->checkBox_wrapping, &QCheckBox::clicked, ui->spinBox, &QSpinBox::setWrapping);
    connect(ui->checkBox_accelerated, &QCheckBox::clicked, ui->spinBox, &QSpinBox::setAccelerated);
    connect(ui->checkBox_showGroupSeperator, &QCheckBox::clicked, ui->spinBox, &QSpinBox::setGroupSeparatorShown);
    connect(ui->checkBox_wrapping, &QCheckBox::clicked, ui->dateTimeEdit, &QDateTimeEdit::setWrapping);
    connect(ui->checkBox_accelerated, &QCheckBox::clicked, ui->dateTimeEdit, &QDateTimeEdit::setAccelerated);
    connect(ui->checkBox_showGroupSeperator, &QCheckBox::clicked, ui->dateTimeEdit, &QDateTimeEdit::setGroupSeparatorShown);

    ui->comboBox_buttonSymbol->addItem(tr("UpDownArrows"), QAbstractSpinBox::UpDownArrows);
    ui->comboBox_buttonSymbol->addItem(tr("PlusMinus"), QAbstractSpinBox::PlusMinus);
    ui->comboBox_buttonSymbol->addItem(tr("NoButtons"), QAbstractSpinBox::NoButtons);


    connect(ui->checkBox_invertedAppearance, &QCheckBox::clicked,
            ui->horizontalScrollBar, &QScrollBar::setInvertedAppearance);
    connect(ui->checkBox_invertedControls, &QCheckBox::clicked,
            ui->horizontalScrollBar, &QScrollBar::setInvertedControls);
    connect(ui->checkBox_invertedAppearance, &QCheckBox::clicked,
            ui->horizontalSlider, &QScrollBar::setInvertedAppearance);
    connect(ui->checkBox_invertedControls, &QCheckBox::clicked,
            ui->horizontalSlider, &QScrollBar::setInvertedControls);
    connect(ui->checkBox_invertedAppearance, &QCheckBox::clicked,
            ui->verticalScrollBar, &QScrollBar::setInvertedAppearance);
    connect(ui->checkBox_invertedControls, &QCheckBox::clicked,
            ui->verticalScrollBar, &QScrollBar::setInvertedControls);
    connect(ui->checkBox_invertedAppearance, &QCheckBox::clicked,
            ui->verticalSlider, &QScrollBar::setInvertedAppearance);
    connect(ui->checkBox_invertedControls, &QCheckBox::clicked,
            ui->verticalSlider, &QScrollBar::setInvertedControls);
    connect(ui->checkBox_invertedAppearance, &QCheckBox::clicked,
            ui->dial, &QScrollBar::setInvertedAppearance);
    connect(ui->checkBox_invertedControls, &QCheckBox::clicked,
            ui->dial, &QScrollBar::setInvertedControls);

    ui->comboBox_tickPosition->addItem(tr("NoTicks"),QSlider::NoTicks);
    ui->comboBox_tickPosition->addItem(tr("TicksAbove/TicksLeft"),QSlider::TicksAbove);
    ui->comboBox_tickPosition->addItem(tr("TicksBelow/TicksRight"),QSlider::TicksBelow);
    ui->comboBox_tickPosition->addItem(tr("TicksBothSides"),QSlider::TicksBothSides);

    connect(ui->checkBox_dial_wrapping, &QCheckBox::clicked,
            ui->dial, &QDial::setWrapping);
    connect(ui->checkBox_dial_notchesVisible, &QCheckBox::clicked,
            ui->dial, &QDial::setNotchesVisible);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(completerWidget);
    ui->groupBox_compleeterExample->setLayout(vLayout);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::updateButtonClickTimes()
{
    static int i=0;
    qDebug() << tr("The button clicked %1 times.").arg(++i);
}

void Widget::on_comboBox_currentIndexChanged(int index)
{
    ui->lineEdit->setEchoMode(static_cast<QLineEdit::EchoMode>(index));
}


void Widget::on_comboBox_buttonSymbol_currentIndexChanged(int index)
{
    ui->spinBox->setButtonSymbols(QAbstractSpinBox::ButtonSymbols(index));
    ui->dateTimeEdit->setButtonSymbols(QAbstractSpinBox::ButtonSymbols(index));
}

void Widget::on_comboBox_tickPosition_currentIndexChanged(int index)
{
    ui->horizontalSlider->setTickPosition(QSlider::TickPosition(index));
    ui->verticalSlider->setTickPosition(QSlider::TickPosition(index));
}

void Widget::on_spinBox_tickInterval_valueChanged(int arg1)
{
    ui->horizontalSlider->setTickInterval(arg1);
    ui->verticalSlider->setTickInterval(arg1);
}

void Widget::on_spinBox_dial_notchesTarget_valueChanged(int arg1)
{
    ui->dial->setNotchTarget(arg1);
}

void Widget::on_comboBox_textElideMode_currentIndexChanged(int index)
{

    QString str = ui->label_text->fontMetrics().elidedText(text,
                                             static_cast<Qt::TextElideMode>(index),
                                             ui->label_text->width());
    ui->label_text->setText(str);
}

void Widget::on_comboBox_segmentStyle_currentIndexChanged(int index)
{
    ui->lcdNumber->setSegmentStyle(static_cast<QLCDNumber::SegmentStyle>(index));
}

void Widget::on_digitCountSpinBox_valueChanged(int arg1)
{
    ui->lcdNumber->setDigitCount(arg1);
}

void Widget::on_modeComboBox_currentIndexChanged(int index)
{
    // 这里可以使用正则表达式实现更多的控制，待完成
    ui->lineEdit_lcdNumber_display->clear();
    ui->lineEdit_lcdNumber_display->setFocus();
    QLCDNumber::Mode mode = static_cast<QLCDNumber::Mode>(index);
    switch (mode) {
    case QLCDNumber::Hex:
    {
        // 行编辑器设置输入掩码
        int digiCount = ui->digitCountSpinBox->value();
        QString inputMask = QString();
        for(int i=0;i<digiCount-1;++i)
            inputMask.append('h');
        inputMask.append("H\\H");
        ui->lineEdit_lcdNumber_display->setInputMask(inputMask);
    }
        break;
    case QLCDNumber::Bin:
    {
        // 行编辑器设置输入掩码
        int digiCount = ui->digitCountSpinBox->value();
        QString inputMask = QString();
        for(int i=0;i<digiCount-1;++i)
            inputMask.append('b');
        inputMask.append("B\\B");
        ui->lineEdit_lcdNumber_display->setInputMask(inputMask);
    }
        break;
    case QLCDNumber::Dec:
    {
        //
        // 行编辑器设置输入掩码
        int digiCount = ui->digitCountSpinBox->value();
        QString inputMask = QString();
        for(int i=0;i<digiCount-1;++i)
            inputMask.append('0');
        inputMask.append("9;.");
        ui->lineEdit_lcdNumber_display->setInputMask(inputMask);
    }
        break;
    case QLCDNumber::Oct:
    {
        // 行编辑器设置输入掩码
        int digiCount = ui->digitCountSpinBox->value();
        QString inputMask = QString();
        for(int i=0;i<digiCount-1;++i)
            inputMask.append('d');
        inputMask.append("DO");
        ui->lineEdit_lcdNumber_display->setInputMask(inputMask);
    }
        break;
    default:
        break;
    }
}

QPushButton *Widget::createLineeditRightButton(QLineEdit *edit)
{
    QPushButton *btn = new QPushButton();
    btn->setCursor(Qt::ArrowCursor);
//    btn->setIcon(QIcon(QPixmap(":/imgs/refresh-32.png")));
//    btn->setFlat(true);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addStretch();  // 加弹簧，让按钮的位置在右边
    layout->addWidget(btn);
    layout->setContentsMargins(0,0,0,0);
    edit->setLayout(layout);
    return btn;
}
