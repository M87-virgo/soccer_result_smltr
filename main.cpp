#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setStyleSheet("QToolTip { background-color: lightgrey; "
                    "color: black; "
                    "border: none } "
                    "QWidget { color: lightblue; "
                    "background-color: #404040 }");
    w.show();
    return a.exec();
}
