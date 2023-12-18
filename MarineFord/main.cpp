#include <QApplication>
#include "Gui.h"

#include <iostream>

GUI* UI;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UI = new GUI();
    UI->setFixedSize(1000, 500);
    UI->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    UI->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    UI->show();
    UI->showMenu();

    return a.exec();
}
