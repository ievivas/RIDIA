#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QTcpServer>
#include <QtNetwork>
#include <QGridLayout>
#include <QScrollArea>
#include <QScrollBar>
#include <QPushButton>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QFrame;
class QWidget;
class QBoxLayout;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLayout;
class QRect;
class QPushButton;
QT_END_NAMESPACE
class Modia;


//!  Clase Window
/*!
    Esta clase establece los métodos necesarios para las siguientes funcionalidades:
        - Desplegar por pantalla las líneas de datos que son procesadas por la clase Modia.
        - Los botones para cambiar la captura de otro programa cliente.
        - Imprimir por pantalla la información de las cuentas (valor y posición en la imagen), y
          la información del fondo del cielo (media y desviación estándar).
*/
class Window : public QWidget
{
    Q_OBJECT

public:
    //!  Constructor
    /*!
         Se asignan los valores iniciales de algunos elementos de la clase, tales como:
             - Los layouts donde se insertarán los labels y botones.
             - Los nombres a algunas etiquetas.
             - Los nombres a los botones.
             - Las distancias entre los labels.
             - Los tamaños de los botones.
             - Las posiciones de botones y labels en el grid del layout.
             - Se conectan la señal de clicked de cada botón con su respectiva función.
             - Se crea un objeto tipo Modia que se inserta en un sollArea, que a su vez se inserta en un layout.
    */
    Window();

    /*!
      Establece las conexiones de señales y slots. Principalmente, cuando el socket
      emita la señal readyRead(), entonces se ejecuta la función recv().
      También conecta señales en función del tiempo para imprimir en el label los datos del fondo del cielo.
      Se colocó por defecto que se actualicen estos datos cada 3000 ms.
    */
    void connection(QTcpSocket *_socket);

    /*!
        Método utilizado en los slots que se ejecutan cuando la señal clicked del ratón es emitida.
        Manda a través del socket (a los programas clientes) una etiqueta con el CCD correspondiente
        al botón en el que se hizo click.
    */
    void sendCCDMessage();


    /*!
        Método utilizado en los slots que se ejecutan cuando la señal clicked del ratón es emitida.
        Manda a través del socket (a los programas clientes) una etiqueta con la QNX correspondiente
        al botón en el que se hizo click.
    */
    void sendQNXMessage();



    QTcpSocket *socket; /*!< Atributo público utilizado en el método connection() para la lectura de datos provenientes del cliente.  */

protected:
    /*!
        Reimplemento del método mousePressEvent() de QWidget. Calcula la posición del cursor al hacer
        click sobre el area del modia. También calcula el valor del píxel en ese punto. Toma en cuenta
        la posición del scrollbar.
    */
    void mousePressEvent(QMouseEvent *event);

private slots:
    /*!
        Imprime a través de los labels correspondientes, los valores de la media y desviación estándar
        utilizando las funciones getMean() y getSigma().
    */
    void setMeanSigma();

    /*!
        Recibe a través del socket la línea de datos. Está implementado un buffer ya que el módulo TCP
        segmenta la línea. Luego de reconstruida la línea, se procesa a través de la clase Modia por medio
        del método nextLine().
    */
    void recv();

    /*!
        Función que se ejecuta al hacer click sobre el botón qnx1. Realiza las siguientes acciones:
            - Resetea a sus valores iniciales el objeto Modia.
            - Deshabilita los botones del CCD y QNX que actualmente se utilizan.
    */
    void qnx1Event();

    /*!
        Función que se ejecuta al hacer click sobre el botón qnx2. Realiza las siguientes acciones:
            - Resetea a sus valores iniciales el objeto Modia.
            - Deshabilita los botones del CCD y QNX que actualmente se utilizan.
    */
    void qnx2Event();

    /*!
        Función que se ejecuta al hacer click sobre el botón qnx3. Realiza las siguientes acciones:
            - Resetea a sus valores iniciales el objeto Modia.
            - Deshabilita los botones del CCD y QNX que actualmente se utilizan.
    */
    void qnx3Event();

    /*!
        Función que se ejecuta al hacer click sobre el botón qnx4. Realiza las siguientes acciones:
            - Resetea a sus valores iniciales el objeto Modia.
            - Deshabilita los botones del CCD y QNX que actualmente se utilizan.
    */
    void qnx4Event();

    /*!
        Función que se ejecuta al hacer click sobre el botón CCD1. Realiza las siguientes acciones:
            - Resetea a sus valores iniciales el objeto Modia.
            - Deshabilita los botones del CCD y QNX que actualmente se utilizan.
    */
    void ccd1Event();

    /*!
        Función que se ejecuta al hacer click sobre el botón CCD2. Realiza las siguientes acciones:
            - Resetea a sus valores iniciales el objeto Modia.
            - Deshabilita los botones del CCD y QNX que actualmente se utilizan.
    */
    void ccd2Event();

    /*!
        Función que se ejecuta al hacer click sobre el botón CCD3. Realiza las siguientes acciones:
            - Resetea a sus valores iniciales el objeto Modia.
            - Deshabilita los botones del CCD y QNX que actualmente se utilizan.
    */
    void ccd3Event();

    /*!
        Función que se ejecuta al hacer click sobre el botón CCD4. Realiza las siguientes acciones:
            - Resetea a sus valores iniciales el objeto Modia.
            - Deshabilita los botones del CCD y QNX que actualmente se utilizan.
    */
    void ccd4Event();

    /*!
        Método que deshabilita los botones que actualmente se están utilizando.
    */
  void buttomDisabled();


private:
  QGridLayout *infoLayout;  /*!< Crea un layout en forma de cuadrícula para colocar otros layouts o widgets. */
  QHBoxLayout *titleLayout; /*!< Crea un layout para ordenar widgets u otros layouts en forma horizontal. */
  QVBoxLayout *mainLayout;  /*!< Crea un layout para ordenar widgets u otros layouts en forma vertical. */
  QVBoxLayout *topLayout;   /*!< Crea un layout para ordenar widgets u otros layouts en forma vertical. */
  QBoxLayout *bottomLayout; /*!< Crea un layout para ordenar widgets u otros layouts. Puede ser horizontal o vertical*/
  QLabel *infoLabel1;       /*!< Etiqueta del los botones CCDs. */
  QLabel *infoLabel2;       /*!< Etiqueta de las cuentas. */
  QLabel *infoLabel3;       /*!< Etiqueta del fondo del cielo. */
  QLabel *meanLabel;        /*!< Etiqueta de la media del fondo del cielo.  */
  QLabel *meanValue;        /*!< Etiqueta para mostrar el valor de la media del fondo del cielo.*/
  QLabel *sigmaLabel;       /*!< Etiqueta de la desviación estándar del fondo del cielo.  */
  QLabel *sigmaValue;       /*!< Etiqueta para mostrar el valor de la desviación estándar del fondo del cielo. */
  QLabel *xPosLabel;        /*!< Etiqueta de la posición X del cursor. */
  QLabel *xPosValue;        /*!< Etiqueta para mostrar el valor de la posición X del cursor. */
  QLabel *yPosLabel;        /*!< Etiqueta de la posición Y del cursor. */
  QLabel *yPosValue;        /*!< Etiqueta para mostrar el valor de la posición Y del cursor.  */
  QLabel *countLabel;       /*!< Etiqueta del las cuentas.  */
  QLabel *countValue;       /*!< Etiqueta para mostrar el valor de las cuentas.  */
  QPushButton *qnx1;        /*!< Botón qnx1  */
  QPushButton *qnx2;        /*!< Botón qnx2  */
  QPushButton *qnx3;        /*!< Botón qnx3  */
  QPushButton *qnx4;        /*!< Botón qnx4  */
  QPushButton *ccd1;        /*!< Botón ccd1  */
  QPushButton *ccd2;        /*!< Botón ccd2  */
  QPushButton *ccd3;        /*!< Botón ccd3  */
  QPushButton *ccd4;        /*!< Botón ccd4  */
  QScrollArea *scrollArea;  /*!< Área de scroll.  */
  QScrollBar *scrollBar;
  quint16 line[2048];       /*!< Vector que almecena temporalmente la línea de datos en la función recv(). */
  quint16 line2[2048];      /*!< Vector que almecena temporalmente la línea de datos en la función recv().  */
  Modia *modia;             /*!< Objeto de la clase Modia. Utilizado para el procesamiento de la imagen y para los eventos del mouse.  */
  QPoint point;             /*!< Utilizado para calcular la posición del mouse al hacer click. Se utiliza en la función mousePressEvent()  */
  int nread;                /*!< Utilizado en la función recv() para saber cuántos bytes se recibieron a través del socket.  */
  int nleft;                /*!< Utilizado en el buffer de la función recv() para saber cuántos bytes faltan por leer.  */
  int pos_i;                /*!< Utilizado en el buffer de la función recv() para saber el índice de la posición inicial en el vector al llegar varios segmentos de la misma línea.  */
  int pos_f;                /*!< Utilizado en el buffer de la función recv() para saber el índice de la posición final en el vector al llegar varios segmentos de la misma línea.  */
  int i;                    /*!< Iterador en los for.  */
  int j;                    /*!< Iterador en los for.  */
  int useqnx;               /*!< Valor del botón qnx que actualmente se utiliza.  */
  int useccd;               /*!< Valor del botón ccd que actualmente se utiliza.  */
  int hvalue;               /*!< Valor de desplazamiento del scrollbar.  */
};

#endif

