
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <iostream>
#include <QMessageBox>

#include "rootwidget.h"
#include "nanocoater.h"

void handleExit(){
    std::cout << "Exit" << std::endl;
}

int main (int argc, char *argv[]){
    QApplication::setSetuidAllowed(true);
    QApplication a(argc, argv);

    QMainWindow window;
    //window.setWindowFlags(Qt::FramelessWindowHint);

    RootWidget root;
    if (gpioInitialise() >= 0)
        root.setNanoCoater(new NanoCoater());
    else{
        QMessageBox msgBox;
        msgBox.setText("Raspberry Pi GPIO not supported");
        msgBox.exec();
    }

    window.setCentralWidget(&root);
    window.resize(1024, 640);
    window.show();

    return a.exec();
}
