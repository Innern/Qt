#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(res);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    if(QT_VERSION > QT_VERSION_CHECK(5,6,0))
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("MDI");  // 设置应用程序名称
    QCoreApplication::setOrganizationName("QtProjects");  // 设置组织名

    // 国际化
    QString lang = MainWindow::currentLanguage();
    QTranslator translator;
    if(QString::compare("chinese",lang, Qt::CaseInsensitive) == 0){
        translator.load("../../MDI_zh_CN.qm");
        a.installTranslator(&translator);
    }

    MainWindow w;
    w.show();
    return a.exec();
}
