#include "socketconnection.h"

SocketConnection::SocketConnection() {
    this->start(7777); //port
    win = new Window;
    win->show();
    std::cout << "SocketConnection" << std::endl;
    connect(this, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

SocketConnection::~SocketConnection() {
    this->stop();
}


void SocketConnection::start(int port) {
        if (listen(QHostAddress::Any, port)) {
           qDebug() << "Conexion iniciada en puerto:" << this->serverPort();
        }
        else {
            qDebug() << "Conexion no se pudo iniciar:" << this->errorString();
        }
}

void SocketConnection::stop() {
        qDebug() << "Conexion detenida!!";

}


void SocketConnection::newConnection() {
        win->connection(nextPendingConnection());
        qDebug() << "Nueva conexion:"  <<  win->socket->peerAddress().toString();
}

void SocketConnection::connected() {
        qDebug() << "Conectado" << win->socket->peerAddress().toString();

}
