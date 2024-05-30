#include "mainwindow.h"
#include <QApplication>

#include "ui/logindialog.h"

#define LOGIN_DIALOG_EN 0

int main(int argc, char *argv[])
{
    if(QT_VERSION > QT_VERSION_CHECK(5,6,0))
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

#if LOGIN_DIALOG_EN
    LoginDialog loginDlg;
    MainWindow w;
   if(loginDlg.exec() == QDialog::Accepted){
       w.show();
       return a.exec();
   } else{
       loginDlg.show();
       return a.exec();
   }
#else
    MainWindow w;
    w.show();
#endif
    return a.exec();
}
