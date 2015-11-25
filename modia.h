#ifndef MODIA_H
#define MODIA_H

#include "window.h"
#include <QWidget>
#include <QGenericMatrix>
#include <QCursor>
#include <QMouseEvent>
#include <QPainter>
#include <QImage>
#include <QRectF>
#include <pthread.h>

//#include <cfitsio/fitsio.h>
//#include <cfitsio/fitsio2.h>
//#include <CCfits/CCfits.h>
//#include <cfitsio/longnam.h>


//!  Clase Modia
/*!
    La clase Modia se encarga de las siguientes tareas:
        - Almacenar las líneas de datos en una matriz numérica.
        - Rescalar la profundidad de color de 16 bpp a 8 bpp.
        - Procesar la imagen para realzar su contraste utilizano la técnica de Expansión Lineal Modificada.
        - Hallar los intervalos de reescalamiento cada 64 líneas.
*/
class Modia : public QWidget {
  Q_OBJECT

  public:
    //!  Constructor
    /*!
         Se asignan los valores iniciales de algunos elementos de la clase, tales como:
             - Dimensiones del área de despliegue.
             - Forma al puntero del mouse.
             - Ancho y altura de la estructura de la imagen digital.
             - Formato a la estructura de la imagen digital.
             - Valores a las matrices numéricas.
             - Valores a los contadores de líneas.
    */
    Modia();

    /*!
        Recibe como argumento el vector o línea de datos. Esta función se encarga de las siguientes tareas:
            - Añadir cada línea de datos recibida  a las matrices numéricas que se utilizan para el reescalamiento
              de los datos.
            - Incrementa los contadores de líneas.
            - Cada 512 líneas hace llamado a las funciones que calculan los intervalos de reescalamiento (en un hilo paralelo de ejecución).
            - Línea por línea hace llamado a las funciones que despliegan la imagen digital.
    */
    void nextLine(quint16 *rowAux);


    /*!
        Lleva los valores de los componentes de la clase Modia a los valores iniciales. Se utiliza cuando hay cambio de CCD.
    */
    void resetModia();

    /*!
      Recibe un un píxel de 16 bpp y lo reescala linealmente a 8 bpp.
    */
    double scaleTo8bit(quint16);

    /*!
      Calcula (en un hilo de ejecucion paralelo) las estadisticas para el reescalamiento. Eso incluye: Media, Desviacion Estandar, Clipped Mean y los intervalos Z1 y z2
    */
    friend void* thread_statistics(void *ptr);

    /*!
      Reescala cada línea de datos recibida utilizando los intervalos z1 y z2. Luego almacena cada valor (píxel) en
      la estructura de imagen digital.
    */
    void imageLinealStretch();

    /*!
      Dibuja la línea de la imagen digital utilizando la función update() de QWidget.
    */
    void drawLine();

    /*!
      Devuelve la media de los datos. Esta función es llamada por gui_modia.cpp que despliega este valor a través de la
      interfaz gráfica.
    */
    double getMean();

    /*!
      Devuelve la desviación estándar de los datos. Esta función es llamada por gui_modia.cpp que despliega este valor
      a través de la interfaz gráfica.
    */
    double getSigma();

    /*!
      Devuelve la posición del cursor (coordenadas X e Y) dentro del área de despliegue al hacer clic sobre la imagen digital.
      Esta función considera cuando la imagen se desplaza línea por línea. Para esto hace uso de 2 matrices numérica auxiliares.
    */
    QPoint getPos(QMouseEvent *event, int hvalue);

    /*!
      Devuelve el valor del píxel (cuenta) con las cooredenadas X e Y calculadas con la función getPos.
    */
    int getCountValue(QMouseEvent *event, int hvalue);

    /*!
      Esta función llena las matrices numéricas auxiliares necesarias para que la función getPos calcule la posición
      del puntero del mouse.
    */
    void fillMatrix();

    /*!
      Se utiliza para imprimir por pantalla el error específico al crear o escribir en el archivo Fits.
    */
    void printerror(int);


  protected:
    /*!
      Esta función es llamada por la función update() y es la que efectivamente pinta la línea de la imagen digital
      a través de la interfaz gráfica.
    */
    void paintEvent(QPaintEvent *event);

  private:
    QImage *image;                                  /*!< Puntero a una estructura de imagen digital propia de Qt */
    QRectF target;                                  /*!< Se utiliza como parámetro en la función drawImage dentro del método paintEvent(). Define un rectángulo en un plano.  */
    QRectF source;                                  /*!< Se utiliza como parámetro en la función drawImage dentro del método paintEvent(). Define un rectángulo en un plano.  */
    QGenericMatrix<2048, 512, quint16> matrix;      /*!< Matriz numérica que almacena las últimas 512 líneas de datos recibidas. <columnas, filas, tipo> */
    QGenericMatrix<2048, 2048, quint16> matrix1;    /*!< Las matrices matrix1 y matrix2 se utilizan para almacenar las últimas 1024 líneas de datos (512 línas en una y 512 en otra). Se utilizan en el método getCountValue(). */
    QGenericMatrix<2048, 2048, quint16> matrix2;    /*!< Las matrices matrix1 y matrix2 se utilizan para almacenar las últimas 1024 líneas de datos (512 línas en una y 512 en otra). Se utilizan en el método getCountValue(). */
    unsigned long long mean;                        /*!< Media estadística. Se utiliza para calcular los intervalos de reescalamiento z1 y z2. */
    unsigned long long sigma;                       /*!< Desviación estándar del fondo del cielo. Se utiliza para calcular los intervalos de reescalamiento z1 y z2. */
    unsigned long long sum1;                        /*!< Acumulador de la sumatoria para el cáculo de la media */
    unsigned long long sum2;                        /*!< Acumulador de la sumatoria para el cáculo de la desviación estándar */
    double x;                                       /*!< Atributo utilizado en el método imageLinealStretch(). */
    double val;                                     /*!< Atributo utilizado en el método thread_statistics(). */
    double meanAux;                                 /*!< Atributo utilizado en el método getMean(). */
    double sigmaAux;                                /*!< Atributo utilizado en el método getSigma(). */
    long min;                                       /*!< Atributo utilizado en el método thread_statistics(). */
    long max;                                       /*!< Atributo utilizado en el método thread_statistics(). */
    int i;                                          /*!< Iterador utilizado en diferentes for. */
    int j;                                          /*!< Iterador utilizado en diferentes for. */
    int k;                                          /*!< Iterador utilizado en diferentes for. */
    int line;                                       /*!< Contador del número total de líneas recibidas. Se resetea a cero cuando se cambia de CCD. */
    int lineAux;                                    /*!< Contador del número de líneas recibidas que se resetea a cero automáticamente cada 512. Se utiliza en varios métodos. */
    int n;                                          /*!< Utilizado en el ClippedMean del método thread_statistics() */
    int ndrop;                                      /*!< Utilizado en el ClippedMean del método thread_statistics() */
    int lastndrop;                                  /*!< Utilizado en el ClippedMean del método thread_statistics() */
    int MAXITER;                                    /*!< Utilizado en el ClippedMean del método thread_statistics() */
    int statArea;                                   /*!< Define el número de líneas que se utilizan para las estadísticas del método thread_statistics(). Su valor por defecto es 512. */
    int frame;                                      /*!< Similar a statArea pero se utilizae en el método nextLine() para el control del flujo de las líneas de entrada. Su valor por defecto es 512.  */
    int z1;                                         /*!< Límite izquierdo del intervalo de reescalamiento.  */
    int z2;                                         /*!< Límite derecho del intervalo de reescalamiento.  */
    int value8bit;                                  /*!< Valor del píxel en 8bpp luego del reescalamiento. */
    int countValue;                                 /*!< Valor del píxel. Se utiliza en el método getCountValue(). */
    int modiaWidht;                                 /*!< Ancho del área donde se despliega la imagen. Se inicializa con valor de 2048. Se utiliza como parámetro para iniciar el atributo QImage.  */
    int modiaHeight;                                /*!< Alto del área donde se despliega la imagen. Se inicializa con valor de 200000 (es un valor grande ya que el driftscan puede ser largo). Se utiliza como parámetro para iniciar el atributo QImage. */
    int hvalue;                                     /*!< Atributo que indica la cantidad de píxeles que se desplaza el scrollbar. Se utiliza en el método getPos(). */
    int dx;                                         /*!< Atributo que indica la cantidad de píxeles que se desplaza en el eje x el area modia de despliegue. No cambia cuando se compila en otros PC, su valor se mantiene en 14. */
    int dy;                                         /*!< Atributo que indica la cantidad de píxeles que se desplaza en el eje y el area modia de despliegue. Sí cambia cuando se compila en otros PC, su valor puede variar. Se debe cuadrar para que la posición (0,0) coincida con la esquina superior derecha del area modia. */
    quint16 row[2048];                              /*!< Vector que almacena la línea de datos.  */
    QRgb valueRGB;                                  /*!< Atributo que se utiliza como parámetro en la función setPixel dentro del método imageLinealStretch(). */
    QCursor cursor;                                 /*!< Atributo que se utiliza en el constructor para cambiar la forma del cursor del ratón. */
    QPoint point1;                                  /*!< Se utiliza en el método getPos() para almacenar la posición actual del mouse al hacer click en la imagen. */
    QPoint point2;                                  /*!< Se utiliza en el método getCounValue() para almacenar la posición actual del mouse al hacer click en la imagen. */
    bool changeMatrix;                              /*!< Valor booleano que permite intercambiar el llenado de matrix1 con matrix2 o viceversa  */
    pthread_t t1;                                   /*!< Atributo que se utiliza para crear un hilo de ejecución paralelo. Se utiliza para ejecutar la función thread_statistics(). */
    //fitsfile *fptr1;                                /*!< Puntero que se utiliza para la creación y escritura en un archivo Fits. */
    //fitsfile *fptr2;                                /*!< Puntero que se utiliza para la creación y escritura en un archivo Fits. */
    int status;                                     /*!< Se utiliza como parámetro en algunas funciones para el manejo de archivos Fits. Almacena un valor entero correspondiente al estado (de exito o fracaso) de haber utilizado la función. */
    long fpixel;                                    /*!< Almacena la posición del primer píxel (en la matriz de datos del Fits) que se escribirá. Ejem: el primer píxel de la primera fila está en la posición 1, el primer píxel de la segunda fila está en la posición 2049, etc.*/
    long nelements;                                 /*!< Almacena la cantidad de píxeles de la nueva imagen, es decir: 512 * 2048. Se utiliza como parámetro en algunas funciones para el manejo de archivos Fits.*/
    long naxis;                                     /*!< Almacena la cantidad de ejes de la imagen. Obviamente para una imagen bidimensional su valor es 2. Se utiliza como parámetro en algunas funciones para el manejo de archivos Fits.*/
    long naxes[2];                                  /*!< Vector que almacena el rango de los ejes de la imagen. Se inicializa con valores de 2048 para el eje x, y 512 para el eje y.Se utiliza como parámetro en algunas funciones para el manejo de archivos Fits.*/
    bool change;
    int filas;
};

#endif
