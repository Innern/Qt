#include "mainwindow.h"
#include "connection.h"

#include <QApplication>

#if QT_VERSION > QT_VERSION_CHECK(5,15,0)
#include <QScreen>
#else
#include <QDesktopWidget>
#endif

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);

    if(QT_VERSION > QT_VERSION_CHECK(5,6,0))
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    if(!createConnection()){
        return 1;
    }

    qRegisterMetaType<QList<int> >();

    MainWindow w;
    w.show();
    return a.exec();
}
