#include <QApplication>
#include "mainwindow.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    cout<<"LOLOLOL";

    system ("pause");

    MainWindow mainWindow;
    mainWindow.show();


    return app.exec();
}
