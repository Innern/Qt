#include "imageviewfram.h"

#include <QWheelEvent>
#include <QVBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QSlider>
#include <QGridLayout>
#include <QStyle>
#include <QDebug>
#include <QScrollBar>
#include <QDialogButtonBox>
#include <qmath.h>


ImageViewFram::ImageViewFram(QWidget *parent) : QFrame(parent)
{
    initFrame();
}

ImageViewFram::ImageViewFram(const QString &title, QWidget *parent):
    QFrame(parent),
    m_title(title)
{
    initFrame();
    setTitle(m_title);
}

GraphicsView *ImageViewFram::view() const
{
    return m_graphicsView;
}

int ImageViewFram::getZoomLevel() const
{
    return m_zoomLevel;
}

QString ImageViewFram::getTitle() const
{
    return m_title;
}

void ImageViewFram::zoomIn(int level)
{
    m_zoomSlider->setValue(m_zoomSlider->value() + level);
}

void ImageViewFram::zoomOut(int level)
{
    m_zoomSlider->setValue(m_zoomSlider->value() - level);
}

void ImageViewFram::rotateLeft()
{
    m_rotateSlider->setValue(m_rotateSlider->value() - 10);
}

void ImageViewFram::rotateRight()
{
    m_rotateSlider->setValue(m_rotateSlider->value() + 10);
}

void ImageViewFram::setMatrix()
{
    qreal scale = qPow(qreal(2), (m_zoomSlider->value() - 50)/ qreal(10));

    QMatrix matrix;
    matrix.scale(scale, scale);
    matrix.rotate(m_rotateSlider->value());

    m_graphicsView->setMatrix(matrix);
    m_resetBtn->setEnabled(true);
}

void ImageViewFram::resetView()
{
    m_zoomSlider->setValue(50);
    m_rotateSlider->setValue(0);
    setMatrix();
    // 保证当前视图中心
    m_graphicsView->ensureVisible(QRectF(0,0,0,0));
    m_resetBtn->setEnabled(false);

}

void ImageViewFram::setResetButtonEnable()
{
 m_resetBtn->setEnabled(true);
}

void ImageViewFram::togglePointerMode()
{
    m_graphicsView->setDragMode(m_selectBtn->isChecked() ? QGraphicsView::RubberBandDrag :
                                                           QGraphicsView::ScrollHandDrag);
    // 当选择按钮被选中时，设置允许场景交互，如果拖放按钮被选中，不允许交互
    m_graphicsView->setInteractive(m_selectBtn->isChecked());
}

void ImageViewFram::setZoomLevel(int level)
{
    m_rotateSlider->setValue(m_rotateSlider->value() + 10);
}

void ImageViewFram::setTitle(const QString &title)
{
    m_labTitle->setText(title);
    setWindowTitle(title);
}

void ImageViewFram::initFrame()
{
    setFrameStyle(Sunken | StyledPanel);
    m_graphicsView = new GraphicsView(this);

    // 添加控件
    QHBoxLayout *topLabelLayout = new QHBoxLayout;
    m_labTitle = new QLabel;
    m_labTitle->setObjectName("labelTitle");
    m_labTitle->setText(m_title);
    m_labTitle->setFrameStyle(Raised |WinPanel);

    QLabel *labPointerMode = new QLabel;
    labPointerMode->setText(tr("Pointer Mode:"));

    m_selectBtn = new QToolButton;
    m_selectBtn->setText(tr("Select"));
    m_selectBtn->setCheckable(true);
    m_selectBtn->setChecked(true);
    m_selectBtn->setAutoExclusive(true);

    m_dragBtn = new QToolButton;
    m_dragBtn->setText(tr("Drag"));
    m_dragBtn->setCheckable(true);
    m_dragBtn->setChecked(false);
    m_dragBtn->setAutoExclusive(true);

    QDialogButtonBox *btnBox = new QDialogButtonBox;
    btnBox->addButton(m_selectBtn, QDialogButtonBox::ActionRole);
    btnBox->addButton(m_dragBtn, QDialogButtonBox::ActionRole);
    QHBoxLayout *btnBoxLayout = new QHBoxLayout;
    btnBoxLayout->addWidget(m_selectBtn);
    btnBoxLayout->addWidget(m_dragBtn);
//    btnBox->setLayout(btnBoxLayout);

    QToolButton *antialiasingBtn = new QToolButton;
    antialiasingBtn->setText(tr("Antialiasing"));

    topLabelLayout->addWidget(m_labTitle);
    topLabelLayout->addStretch();
    topLabelLayout->addWidget(labPointerMode);
    topLabelLayout->addLayout(btnBoxLayout);
//    topLabelLayout->addWidget(m_selectBtn);
//    topLabelLayout->addWidget(m_dragBtn);
    topLabelLayout->addStretch();
    topLabelLayout->addWidget(antialiasingBtn);
    topLabelLayout->addStretch();

    const int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    const QSize iconSize(size, size);

    QToolButton *zoomInBtn = new QToolButton;
    zoomInBtn->setIconSize(iconSize);
    zoomInBtn->setAutoRepeat(true);  // 设置可重复点击
    zoomInBtn->setAutoRepeatDelay(0);  // 设置自动重复的初始延时
    zoomInBtn->setAutoRepeatInterval(33);  // 设置自动重复的间隔
    zoomInBtn->setStatusTip(tr("Zoom In"));
    zoomInBtn->setIcon(QIcon(":/images/zoomin.png"));

    m_zoomSlider = new QSlider;
    m_zoomSlider->setValue(50);
    m_zoomSlider->setRange(0, 100);

    QToolButton *zoomOutBtn = new QToolButton;
    zoomOutBtn->setIconSize(iconSize);
    zoomOutBtn->setAutoRepeat(true);  // 设置可重复点击
    zoomOutBtn->setAutoRepeatDelay(0);  // 设置自动重复的初始延时
    zoomOutBtn->setAutoRepeatInterval(33);  // 设置自动重复的间隔
    zoomOutBtn->setStatusTip(tr("Zoom Out"));
    zoomOutBtn->setIcon(QIcon(":/images/zoomout.png"));

    QVBoxLayout *zoomLayout = new QVBoxLayout;
    zoomLayout->addWidget(zoomInBtn);
    zoomLayout->addWidget(m_zoomSlider);
    zoomLayout->addWidget(zoomOutBtn);

    QToolButton *rotateLeftBtn = new QToolButton;
    rotateLeftBtn->setIconSize(iconSize);
    rotateLeftBtn->setStatusTip(tr("Zoom Out"));
    rotateLeftBtn->setIcon(QIcon(":/images/rotateleft.png"));

    m_rotateSlider = new QSlider;
    m_rotateSlider->setOrientation(Qt::Horizontal);
    m_rotateSlider->setRange(-360, 360);
    m_rotateSlider->setValue(0);

    QToolButton *rotateRightBtn = new QToolButton;
    rotateRightBtn->setIconSize(iconSize);
    rotateRightBtn->setStatusTip(tr("Zoom Out"));
    rotateRightBtn->setIcon(QIcon(":/images/rotateright.png"));

    QHBoxLayout *rotateLayout = new QHBoxLayout;
    rotateLayout->addWidget(rotateLeftBtn);
    rotateLayout->addWidget(m_rotateSlider);
    rotateLayout->addWidget(rotateRightBtn);

    m_resetBtn = new QToolButton;
    m_resetBtn->setText(tr("0"));
    m_resetBtn->setIconSize(iconSize);
    m_resetBtn->setEnabled(false);

    QGridLayout *layout = new QGridLayout;
    layout->addLayout(topLabelLayout,0,0, 1, 2);
    layout->addWidget(m_graphicsView, 1, 0);
    layout->addLayout(zoomLayout,1, 1);
    layout->addLayout(rotateLayout,2,0);
    layout->addWidget(m_resetBtn, 2, 1);
    setLayout(layout);

    connect(zoomInBtn, SIGNAL(clicked(bool)), this, SLOT(zoomIn()));
    connect(zoomOutBtn, SIGNAL(clicked(bool)), this, SLOT(zoomOut()));
    connect(rotateLeftBtn, &QToolButton::clicked, this, &ImageViewFram::rotateLeft);
    connect(rotateRightBtn, &QToolButton::clicked, this, &ImageViewFram::rotateRight);
    connect(m_resetBtn, &QToolButton::clicked, this, &ImageViewFram::resetView);
    connect(m_zoomSlider, &QSlider::valueChanged, this, &ImageViewFram::setMatrix);
    connect(m_rotateSlider, &QSlider::valueChanged, this, &ImageViewFram::setMatrix);
    connect(m_graphicsView->horizontalScrollBar(), &QScrollBar::valueChanged, this, &ImageViewFram::setResetButtonEnable);
    connect(m_graphicsView->verticalScrollBar(), &QScrollBar::valueChanged, this, &ImageViewFram::setResetButtonEnable);
    connect(m_selectBtn, &QToolButton::toggled, this, &ImageViewFram::togglePointerMode);
    connect(m_dragBtn, &QToolButton::toggled, this, &ImageViewFram::togglePointerMode);
}

GraphicsView::GraphicsView(ImageViewFram *frame): m_view(frame)
{
    setDragMode(ScrollHandDrag);  // 设置可拖动
    setDragMode(QGraphicsView::RubberBandDrag);  // 设置默认选择
    setRenderHint(QPainter::Antialiasing);  // 设置抗锯齿渲染
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    // 当按下ctrl键并且滑轮滚动时，缩放图片
    if(event->modifiers() & Qt::ControlModifier) {
        if(event->delta() > 0)
        {
            m_view->zoomIn(6);
        } else {
            m_view->zoomOut(6);
        }
        event->accept();
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Plus){
        m_view->zoomIn();
    } else if(event->key() == Qt::Key_Minus){
        m_view->zoomOut();
    } else if(event->key() == Qt::Key_0){
        m_view->resetView();
    } else {
        QGraphicsView::keyPressEvent(event);
    }
}
