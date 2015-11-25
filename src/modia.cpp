#include <QtGui>
#include "modia.h"
#include <iostream>
#include <math.h>
#include <QDataStream>


Modia::Modia() {
   resize(2048, 512);
   cursor.setShape(Qt::CrossCursor);
   this->setCursor(cursor);
   modiaWidht = 2048;
   modiaHeight = 200000;
   target.setRect(0, 0, 2048, 1);
   source.setRect(0, 0, 2048, 1);
   image = new QImage(modiaWidht, modiaHeight, QImage::Format_RGB888);
   matrix.fill(0.0);
   matrix1.fill(0);
   matrix2.fill(0);
   MAXITER = 30;
   statArea = 512;
   line = 0;
   lineAux = 0;
   frame = 512;
   dx = 14;
   dy = 150;
   z1 = 2;
   z2 = 10;
   changeMatrix = 1;
   /*
   // Se inicializan los parametros de la imagen Fits
   system("rm -rf last512rows_1.fits last512rows_2.fits");
   char filename1[] = "last512rows_1.fits";
   char filename2[] = "last512rows_2.fits";
   int bitpix   =  USHORT_IMG;
   long naxis    =   2;
   long naxes[2] = { 2048, 512 };   // imagen de 2048 pixeles de ancho y 512 filas
   fpixel = 1;                               // first pixel to write
   nelements = naxes[0] * naxes[1];          // number of pixels to write
   status = 0;
   if (fits_create_file(&fptr1, filename1, &status)) // create new FITS file
            printerror( status );
   if (fits_create_img(fptr1,  bitpix, naxis, naxes, &status) ) // write the required keywords for the primary array image. (BSCALE = 1.0 and BZERO = 32768)
            printerror( status );

   if (fits_create_file(&fptr2, filename2, &status)) // create new FITS file
            printerror( status );
   if (fits_create_img(fptr2,  bitpix, naxis, naxes, &status) ) // write the required keywords for the primary array image. (BSCALE = 1.0 and BZERO = 32768)
            printerror( status );
   change = 0;
   filas = 512;
   */

}

void Modia::resetModia(){
    target.setRect(0, 0, 2048, 1);
    source.setRect(0, 0, 2048, 1);
    line = 0;
    lineAux = 0;
    changeMatrix = 1;
}

void* thread_statistics(void *ptr)
{
    Modia *m = static_cast<Modia*>(ptr);
    //meanSigma
    m->sum1 = 0.0;
    m->sum2 = 0.0;
    m->n=0;

    for (int i=m->lineAux; i<m->statArea; i++) {
        for (int j=0; j<m->modiaWidht; j++) {
            if ((m->matrix(i,j)>5.0) && (m->matrix(i,j)<65530.0)) {
               m->sum1 = m->sum1 + m->matrix(i, j);
               m->sum2 = m->sum2 + m->matrix(i, j)*m->matrix(i, j);
               m->n = m->n + 1;
            }
        }
    }
    if (m->n > 0) {
        m->mean = m->sum1/m->n;
        m->sigma = sqrt((m->n*m->sum2 - m->sum1*m->sum1) / (m->n * (m->n - 1.0)));
    }

    //clippedMean
    for (int k=0; k<m->MAXITER; k++) {
            m->ndrop = 0;
            m->n = 0;
            m->sum1 = 0.0;
            m->sum2= 0.0;

            m->min = m->mean - 3*m->sigma;
            m->max = m->mean + 3*m->sigma;
            if (m->min < 5.0)
                m->min = 5.0;
            if (m->max > 65530.0)
                m->max = 65530.0;


            std::cout << "min = " << m->min << std::endl;
            std::cout << "max = " << m->max << std::endl;

            for (int i=m->lineAux; i<m->statArea; i++) {
                for (int j=0; j<m->modiaWidht; j++) {
                    m->val = m->matrix(i,j);
                    if (m->val<m->min || m->val>m->max) {
                       m->ndrop = m->ndrop + 1;
                       //std::cout << " ndrop = " << ndrop;
                    }
                    else {
                        m->sum1 = m->sum1 + m->val;
                        m->sum2 = m->sum2 + m->val*m->val;
                        m->n = m->n + 1;
                        //std::cout << " n = " << n;
                    }
                }
            }
            if (m->n > 0) {
                m->mean = m->sum1 / m->n;
                m->sigma = sqrt((m->n*m->sum2-m->sum1*m->sum1)/(m->n*(m->n-1.0)));
            }

            //std::cout << "\nterm1 - term2: " << n*sum2-sum1*sum1 << std::endl;
            //std::cout << "n = " << n << std::endl;
            std::cout << "mediaClipped = " << m->mean << std::endl;
            std::cout << "sigmaClipped = " << m->sigma << std::endl;

            if (m->ndrop == m->lastndrop)
                break;

            m->lastndrop = m->ndrop;
        }

    //z1z2
    m->z1 = m->scaleTo8bit(m->mean - (2.0*m->sigma)) + 0.5;
    m->z2 = m->scaleTo8bit(m->mean + (2.0*m->sigma)) + 0.5;

    if ((m->z2-m->z1) < 4) {                               //Para evitar que z1 y z2 esten muy juntos (sigma muy pequeño)
        m->z1 = m->scaleTo8bit(m->mean - (2.0*514.0)) + 0.5;  //514 en el rango [0, 65535] representa a 2 en el rango [0, 255]
        m->z2 = m->scaleTo8bit(m->mean + (2.0*514.0)) + 0.5;
    }

    if ((m->z2-m->z1) > 12) {                              //Para evitar que z1 y z2 esten muy separados (sigma muy grande)
        m->z1 = m->scaleTo8bit(m->mean - (2.0*1542.0)) + 0.5; //1542 en el rango [0, 65535] representa a 6 en el rango [0, 255]
        m->z2 = m->scaleTo8bit(m->mean + (2.0*1542.0)) + 0.5;
    }

    if (m->z1 < 0)                                      //Para evitar que z1 tome valores negativos. El rango es [0, 255]
        m->z1 = 0;

    if (m->z2 > 255)                                    //Para evitar que z1 tome valores fuera del limite superior. El rango es [0, 255]
        m->z2 = 255;

    std::cout << "z1 = " << m->z1 << std::endl;
    std::cout << "z2 = " << m->z2 << std::endl;

    return NULL;
}



/*void Modia::printerror(int status)
{
    if (status)
    {
      fits_report_error(stderr, status); // print error report
      exit( status );    / terminate the program, returning error status
    }
}*/

void Modia::nextLine(quint16 *rowAux)
{
    std::cout << "nextLine 1" << std::endl;

    for (i=0; i<modiaWidht; i++) {
        row[i] = rowAux[i];
        matrix(lineAux, i) = rowAux[i];
    }

    std::cout << "nextLine 2" << std::endl;

    if ((fmod(line, filas)) == 0.0) {
        change = !change;
        fpixel = 1;
    }

    std::cout << "nextLine 3" << std::endl;

    /*
    if (change == 0)
    {
        std::cout << "nextLine 4" << std::endl;
        if (fits_write_img(fptr1, TUSHORT, fpixel, nelements, &row[0], &status) )
                printerror( status );
        std::cout << "nextLine 4" << std::endl;
        fpixel = fpixel + 2048;
    }



    else
    {
        std::cout << "nextLine 5" << std::endl;
        if (fits_write_img(fptr2, TUSHORT, fpixel, nelements, *row[0], &status) )
                printerror( status );
        std::cout << "nextLine 5" << std::endl;
        fpixel = fpixel + 2048;
    }

*/

    if ((fmod(line, frame)) == 0.0) {
          lineAux = 0;
          changeMatrix = !changeMatrix;
    }

    std::cout << "nextLine 5" << std::endl;

    this->fillMatrix();

    std::cout << "nextLine 6" << std::endl;

    if ((fmod(line, statArea)) == 0.0) {
         //fpixel = 1;

        if(pthread_create(&t1, NULL, thread_statistics, this))
        {
              std::cout << "\n ERROR creating thread"<< std::endl;
              exit(1);
         }

        //No es necesario esperar que termine el hilo.
        /*if(pthread_join(t1, NULL)){
                  std::cout << "\n ERROR joining thread 1"<< std::endl;
                  exit(1);
        }*/

            imageLinealStretch();
            drawLine();

            line = line + 1;
            lineAux = 0;

        }

       else {
            imageLinealStretch();
            drawLine();

            line = line + 1;
            lineAux = lineAux + 1;

       }

     std::cout << "\nLinea = " << line << std::endl;

}

void Modia::fillMatrix() {
    if (changeMatrix == 0) {
        for (i=0; i<modiaWidht; i++) {
            matrix1(lineAux, i) = row[i];
        }
    }
    else {                          //changeMatrix = 1
        for (i=0; i<modiaWidht; i++) {
            matrix2(lineAux, i) = row[i];
        }
    }
}



double Modia::scaleTo8bit(quint16 value){
    return (((value*255.0)/65535.0));
}



void Modia::imageLinealStretch() {

        for (i=0; i<modiaWidht; i++) {
           x = scaleTo8bit(matrix(lineAux, i));
           value8bit = (255/(z2-z1))*(x-z1) + 0.5;
           if (x > z2)
              value8bit = 255;
           if (x < z1)
              value8bit = 0;
           valueRGB = qRgb(value8bit, value8bit, value8bit);
           image->setPixel(i, line, valueRGB);
        }
}


void Modia::drawLine(){

    if (line < frame) {
        source.setHeight(line+1);
        target.setHeight(line+1);
    }
    else {
        source.moveTop(line+1-frame);
    }
    update();
}


 void Modia::paintEvent(QPaintEvent *)
 {
    QPainter painter (this);
    painter.drawImage(target, *image, source);
  }


 double Modia::getMean() {
    meanAux = mean;
    return (meanAux);
 }


 double Modia::getSigma() {
    sigmaAux = sigma;
    return (sigmaAux);
 }

 QPoint Modia::getPos(QMouseEvent *event, int hvalue) {
      point1 = event->pos();
      //std::cout << "hvalue = " << hvalue << std::endl;
      if ((point1.x()<dx)||(point1.y()<dy)||(point1.x()>=modiaWidht+dx-hvalue)||(point1.y()>=frame+dy)) {
         point1.setX(0);
         point1.setY(0);
         return (point1);
      }

      if (line < frame) {
         point1.setX(point1.x()-dx+hvalue);
         point1.setY(point1.y()-dy);
         return (point1);
      }
      else {
          point1.setX(point1.x()-dx+hvalue);
          point1.setY(point1.y()-dy+line-frame);
          return (point1);
      }
  }

  int Modia::getCountValue(QMouseEvent *event, int hvalue) {
      point2 = event->pos();

      if ((point2.x()<dx)||(point2.y()<150)||(point2.x()>=modiaWidht+dx-hvalue)||(point2.y()>=frame+150)) {
          return (0);
      }

      if (line < frame) {
         point2.setX(point2.x()-dx+hvalue);
         point2.setY(point2.y()-150);
         countValue = matrix1(point2.y(), point2.x());
         return (countValue);
     }

      else {
          if((changeMatrix==0) && (point2.y()-dy < frame-lineAux)) {
              point2.setX(point2.x()-dx+hvalue);
              point2.setY(point2.y()-dy+lineAux);
              countValue = matrix2(point2.y(), point2.x());
              return (countValue);
          }

          else if ((changeMatrix==1) && (point2.y()-dy < frame-lineAux)){
              point2.setX(point2.x()-dx+hvalue);
              point2.setY(point2.y()-dy+lineAux);
              countValue = matrix1(point2.y(), point2.x());
              return (countValue);
          }

          else if ((changeMatrix==0) && (point2.y()-dy > frame-lineAux)){
              point2.setX(point2.x()-dx+hvalue);
              point2.setY((point2.y()-dy)-(frame-lineAux));
              countValue = matrix2(point2.y(), point2.x());
              return (countValue);
          }

          else if ((changeMatrix==1) && (point2.y()-dy > frame-lineAux)){
              point2.setX(point2.x()-dx+hvalue);
              point2.setY((point2.y()-dy)-(frame-lineAux));
              countValue = matrix1(point2.y(), point2.x());
              return (countValue);
          }
     }
  }
