/*
 * Copyright © 2020 beyer341@onlinehome.de
 *
 * Read the LICENSE file that comes with this project for license details.
*/

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.removeTranslator(&translator);

    MainWindow w;
    w.setStyleSheet("QToolTip { background-color: lightgrey; "
                    "color: black; "
                    "border: none } "
                    "QWidget { color: lightblue; "
                    "background-color: #404040 }");
    w.show();
    return a.exec();
}
