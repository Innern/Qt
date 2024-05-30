#ifndef IMAGEVIEWFRAM_H
#define IMAGEVIEWFRAM_H

#include <QFrame>
#include <QGraphicsView>

QT_BEGIN_NAMESPACE
class ImageViewFram;
class QSlider;
class QLabel;
class QToolButton;
QT_END_NAMESPACE

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(ImageViewFram *frame);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    ImageViewFram *m_view;
};

class ImageViewFram : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(int m_zoomLevel READ getZoomLevel WRITE setZoomLevel)
    Q_PROPERTY(QString m_title READ getTitle WRITE setTitle)

public:
    explicit ImageViewFram(QWidget *parent = nullptr);
    explicit ImageViewFram(const QString &title, QWidget *parent = nullptr);
    GraphicsView *view() const;

    int getZoomLevel() const;
    QString getTitle() const;

public slots:
    void zoomIn(int level = 1);
    void zoomOut(int level = 1);
    void rotateLeft();
    void rotateRight();
    void setMatrix();
    void resetView();
    void setResetButtonEnable();
    void togglePointerMode();

    void setZoomLevel(int level);
    void setTitle(const QString &title);


signals:

public slots:

private:
    void initFrame();

private:
    QString m_title;

    int m_zoomLevel;

private:
    GraphicsView *m_graphicsView;
    QSlider *m_zoomSlider;
    QSlider *m_rotateSlider;
    QLabel *m_labTitle;
    QToolButton *m_resetBtn;
    QToolButton *m_selectBtn;
    QToolButton *m_dragBtn;

};

#endif // IMAGEVIEWFRAM_H
