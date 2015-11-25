#ifndef SOCKETCONNECTION_H
#define SOCKETCONNECTION_H

#include <QTcpServer>
#include <iostream>
#include "window.h"


//!  Clase SocketConnection
/*!
    Esta clase hace posible aceptar conexiones TCP entrantes ya que funciona como un servidor (hereda de la clase QTcpServer.
*/
class SocketConnection : public QTcpServer {
    Q_OBJECT

public:
    //!  Constructor
    /*!
         Se asignan los valores iniciales de algunos elementos de la clase, tales como:
             - El puerto de entrada. Para pruebas en la misma PC se coloca 7777. Para conectarla con
               los programas clientes del telescopio, debe cambiarse a 2241.
             - Un objeto tipo Window para la GUI.
             - Conecta las señales para nuevas conexiones entrantes.
    */
    SocketConnection();

    //!  Destructor
    /*!
        Detiene la conexion.
    */
    ~SocketConnection();

    /*!
        Comienza la escucha para alguna conexion en el puerto indicado. Por defecto escucha cualquier
        dirección IP.
    */
    void start(int port);

    /*!
        Solo imprime por pantalla un mensaje: Conexion Detenida.
    */
    void stop();


public slots:
    /*!
        Slot para la funcion connect() llamada en el constructor. El objeto Window se conecta con
        el socket.
    */
        void newConnection();


    /*!
        Al conectarse, esta función imprime por pantalla la direccion IP de la conexion entrante.
    */
        void connected();

private:
        Window *win;    /*!< Único atributo de la clase. Objeto de tipo Window que despliega las líneas de datos que se capturan a través del socket creado por la conexión creada por esta clase. */

};




#endif // SOCKETCONNECTION_H
