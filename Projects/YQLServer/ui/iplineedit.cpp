#pragma execution_character_set("utf-8")
#include "iplineedit.h"
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QRegExpValidator>  // 正则表达式验证器
#else
#include <QRegularExpressionValidator>
#endif
#include <QPainter>  // 绘图
#include <QHBoxLayout>  // 水平布局
#include <QKeyEvent>  // 键盘事件
#include <QMessageBox>  // 信息框
#include <QDebug>

IPLineEdit::IPLineEdit(QWidget *parent):
    QLineEdit(parent)
{
    mainBackground = this->styleSheet();
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QRegExp rx("(2[0-5]{2}|2[0-4][0-9]|1?[0-9]{1,2})");
#else
    QRegularExpression rx("(2[0-5]{2}|2[0-4][0-9]|1?[0-9]{1,2})");
#endif

    QHBoxLayout *phBoxLayout = new QHBoxLayout(this);
    phBoxLayout->setSpacing(10);  // 设置间距
    phBoxLayout->setContentsMargins(0,0,0,0);  // 设置内容间距
    // 创建QLineEdit
    for(int i=0;i<4;i++)
    {
        m_lineEdit[i] = new QLineEdit(this);
        m_lineEdit[i]->setAttribute(Qt::WA_DeleteOnClose);
        m_lineEdit[i]->setStyleSheet("background:rgb(255,255,255)");  // 设置背景样式
        m_lineEdit[i]->setFrame(false);  // 设置无边框
        m_lineEdit[i]->setMaxLength(3);  // 设置文本允许的最大长度
        m_lineEdit[i]->setAlignment(Qt::AlignCenter);  // 设置文本对齐方式
        m_lineEdit[i]->installEventFilter(this);  // 安装事件过滤器
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        m_lineEdit[i]->setValidator(new QRegExpValidator(rx,this));  // 设置验证器
#else
        m_lineEdit[i]->setValidator(new QRegularExpressionValidator(rx,this));  // 设置验证器
#endif
        m_lineEdit[i]->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);  // 设置尺寸
        phBoxLayout->addWidget(m_lineEdit[i]);
    }
    m_lineEdit[0]->setText("127");
    m_lineEdit[1]->setText("0");
    m_lineEdit[2]->setText("0");
    m_lineEdit[3]->setText("1");
    // 设置为只读
    //    this->setReadOnly(true);
}

IPLineEdit::~IPLineEdit()
{

}

void IPLineEdit::setText(const QString &text)
{
    if(!isTextValid(text))
    {
        QMessageBox::warning(this,tr("Warning"),tr("Your IP Address is invalid!"),QMessageBox::Ok);
        return;
    }
    else
    {
        int i=0;
        QStringList ipList = text.split(".");
        foreach (QString ip, ipList) {
            m_lineEdit[i++]->setText(ip);
        }
    }
}

QString IPLineEdit::text() const
{
    QString ip;
    for(int i=0;i<4;i++)
    {
        ip.append(m_lineEdit[i]->text());
        if(i < 3)
            ip.append(".");
    }
    return ip;
}

void IPLineEdit::setStyleSheet(const QString &styleSheet)
{
    //    this->setStyleSheet(mainBackground);
}

void IPLineEdit::paintEvent(QPaintEvent *event)
{
    // 绘制事件
    QWidget::paintEvent(event);  // 父类处理绘制事件
    QPainter painter(this);
    QBrush brush;
    brush.setStyle(Qt::BrushStyle::SolidPattern);  // 设置画刷样式
    QPen pen;
    int width = 0;
    for(int i=0;i<3;i++)
    {
        width += m_lineEdit[i]->width() + ( i==0 ? 3 : 10);  // 布局的间隔距离
        // 画矩形，与输入栏相同的颜色
        brush.setColor(Qt::white);  // 设置画刷颜色
        painter.setBrush(brush);

        pen.setColor(Qt::white);  // 设置画笔颜色
        painter.setPen(pen);

        // 绘制矩形
        painter.drawRect(width-4,0,10,height());

        // 绘制IP 点分
        brush.setColor(Qt::black);
        painter.setBrush(brush);

        pen.setColor(Qt::black);
        painter.setPen(pen);

        painter.drawEllipse(width,height()/2,2,2);
    }
}

bool IPLineEdit::eventFilter(QObject *watched, QEvent *event)
{
    if(children().contains(watched) && QEvent::KeyPress == event->type())
    {
        QKeyEvent *keyEvent = dynamic_cast<QKeyEvent*>(event);
        QLineEdit *pEdit = qobject_cast<QLineEdit*>(watched);
        switch (keyEvent->key()) {
        case Qt::Key_0:
        case Qt::Key_1:
        case Qt::Key_2:
        case Qt::Key_3:
        case Qt::Key_4:
        case Qt::Key_5:
        case Qt::Key_6:
        case Qt::Key_7:
        case Qt::Key_8:
        case Qt::Key_9:
        {
            QString text = pEdit->text();
            if(pEdit->selectedText().length())
            {
                pEdit->text().replace(pEdit->selectedText(),QChar(keyEvent->key()));
            }
            else if(text.length() == 3)
            {
                int index = getIndex(pEdit);
                if(index != -1)
                {
                    m_lineEdit[index]->setFocus();
                    m_lineEdit[index]->selectAll();
                }
            }
            else if( text.length() <3 && text.toInt()*10 > 255)
            {
                int index = getIndex(pEdit);
                if(index != -1 && index != 3)
                {
                    m_lineEdit[index+1]->setFocus();
                    m_lineEdit[index+1]->selectAll();
                }
            }
            else if(text.length() == 2 && text.toInt()*10 < 255)
            {
                if(Qt::Key_0 == keyEvent->key() && text.toInt())
                {
                    pEdit->setText(text.insert(pEdit->cursorPosition(),QChar(Qt::Key_0)));
                }
            }
            else
            {
                qDebug() << "input text:" << text;
            }
            return QWidget::eventFilter(watched,event);
        }
            break;
        case Qt::Key_Backspace:
        {
            QString text = pEdit->text();
            if(!text.length() || text.length() && !pEdit->cursorPosition())
            {
                int index = getIndex(pEdit);
                if(index != -1 && index != 0)
                {
                    m_lineEdit[index-1]->setFocus();
                    int length = m_lineEdit[index-1]->text().length();
                    m_lineEdit[index-1]->setCursorPosition(length?length:0);
                }
            }
            return QWidget::eventFilter(watched,event);
        }
            break;
        case Qt::Key_Left:
        {
            if(!pEdit->cursorPosition())
            {
                int index = getIndex(pEdit);
                if(index != -1 && index != 0)
                {
                    m_lineEdit[index-1]->setFocus();
                    int length = m_lineEdit[index-1]->text().length();
                    m_lineEdit[index-1]->setCursorPosition(length?length:0);
                }
            }
            return QWidget::eventFilter(watched,event);
        }
            break;
        case Qt::Key_Period:
        {
            qDebug() << Qt::Key_Period;
            int index = getIndex(pEdit);
            if(index != -1 && index != 3)
            {
                m_lineEdit[index+1]->setFocus();
                m_lineEdit[index+1]->selectAll();
            }
            return QWidget::eventFilter(watched,event);
        }
            break;
        case Qt::Key_Right:
        {
            if(pEdit->cursorPosition() == pEdit->text().length())
            {
                int index = getIndex(pEdit);
                if(index != -1 && index != 3)
                {
                    m_lineEdit[index+1]->setFocus();
                    m_lineEdit[index+1]->setCursorPosition(0);
                }
            }
            return QWidget::eventFilter(watched,event);
        }
            break;
        default:
            break;
        }
    }

    return false;
}

int IPLineEdit::getIndex(QLineEdit *pEdit)
{
    int index = -1;
    for(int i=0;i<4;i++)
    {
        if(pEdit == m_lineEdit[i])
        {
            index = i;
            break;
        }
    }
    return index;
}

bool IPLineEdit::isTextValid(const QString &strIP)
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QRegExp rx2("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    if(!rx2.exactMatch(strIP))
        return false;
#else
    QRegularExpression rx2("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    if(!rx2.match(strIP).hasMatch())
        return false;
#endif

    return true;
}
