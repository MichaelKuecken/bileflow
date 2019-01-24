#include "gbileflow.h"
#include <QApplication>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Gbileflow w;
    w.show();

    return a.exec();
}
