#include "mainwindow.h"
#include "welcomedialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    if(QT_VERSION > QT_VERSION_CHECK(5,6,0))
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#else
    if(QT_VERSION > QT_VERSION_CHECK(5,6,0))
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication a(argc, argv);
    MainWindow w;
    WelcomeDialog welcomeDlg;
    if(QDialog::Accepted == welcomeDlg.exec()){
        w.show();
        return a.exec();
    }
    return 0;
}
