#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    if(QT_VERSION >= QT_VERSION_CHECK(5,6,0))
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);
    MainWindow w;
    const QRect availableRect = QApplication::desktop()->availableGeometry();
    w.resize(availableRect.width()*0.75, availableRect.height()*0.75);
    w.move((availableRect.width() - w.width())/2, (availableRect.height() - w.height())/2);
    w.show();
    return a.exec();
}
