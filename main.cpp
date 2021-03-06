#include "mainwindow.h"

#include <QApplication>
#include<random>
int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication a(argc, argv);

    // Using styles
    QFile file(":/assets/qss/style.qss");
    file.open(QFile::ReadOnly);
    a.setStyleSheet(file.readAll());
    MainWindow w;
    w.show();
    return a.exec();
}
