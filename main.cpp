#include <QApplication>
#include "socketconnection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SocketConnection w;
    return a.exec();
}

