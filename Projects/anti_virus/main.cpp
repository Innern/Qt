#include "anti_virusdlg.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    anti_virusDlg w;
    w.show();
    return a.exec();
}
