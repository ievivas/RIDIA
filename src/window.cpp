#include <QtGui>
#include "modia.h"
#include "window.h"
#include <iostream>
#include <QFile>
#include <QDataStream>
#include <QTimer>

Window::Window() {
  infoLayout = new QGridLayout;
  titleLayout = new QHBoxLayout;
  infoLabel1 = new QLabel("<h3><i><font color=blue>CCDs:</font></i></h3>");
  infoLabel2 = new QLabel("<h3><i><font color=blue>Cuentas:</font></i></h3>");
  infoLabel3 = new QLabel("<h3><i><font color=blue>Fondo de Cielo:</font></i></h3>");
  meanLabel = new QLabel("<h3><i>&mu; =  <i></h3>");
  meanValue = new QLabel;
  sigmaLabel = new QLabel("<h3><i>&sigma; =  </i></h3>");
  sigmaValue = new QLabel;
  xPosLabel = new QLabel("<h3><i>&nbsp;&nbsp;&nbsp;&nbsp; X =  </i></h3>");
  xPosValue = new QLabel;
  yPosLabel = new QLabel("<h3><i>&nbsp;&nbsp;&nbsp;&nbsp; Y =  </i></h3>");
  yPosValue = new QLabel;
  countLabel = new QLabel("<h3><i>Valor =  </i></h3>");
  countValue = new QLabel;

  //Botones de CCDs
  qnx1 = new QPushButton("QNX1");
  qnx2 = new QPushButton("QNX2");
  qnx3 = new QPushButton("QNX3");
  qnx4 = new QPushButton("QNX4");
  ccd1 = new QPushButton("CCD1");
  ccd2 = new QPushButton("CCD2");
  ccd3 = new QPushButton("CCD3");
  ccd4 = new QPushButton("CCD4");

  titleLayout->setAlignment(Qt::AlignLeft);
  titleLayout->addWidget(infoLabel1);
  titleLayout->addWidget(infoLabel2);
  titleLayout->addWidget(infoLabel3);


  //Para cuadrar las distancias entre los labels:
    infoLabel1->setMinimumSize(140,10);
    infoLabel2->setMinimumSize(153,10);
    xPosLabel->setMinimumSize(70,10);
    xPosLabel->setAlignment(Qt::AlignRight);
    yPosLabel->setMinimumSize(70,10);
    yPosLabel->setAlignment(Qt::AlignRight);
    countLabel->setAlignment(Qt::AlignRight);
    countLabel->setMinimumSize(85,10);
    countValue->setMinimumSize(78,10);
    meanLabel->setMaximumWidth(40);
    //meanLabel->setMinimumHeight(10);
    meanValue->setMinimumSize(165,10);
    sigmaLabel->setMaximumWidth(40);
    //sigmaLabel->setMinimumHeight(10);

    //Tamano de los botones:
    qnx1->setMinimumSize(50,10);
    qnx2->setMinimumSize(50,10);
    qnx3->setMinimumSize(50,10);
    qnx4->setMinimumSize(50,10);
    ccd1->setMinimumSize(50,10);
    ccd2->setMinimumSize(50,10);
    ccd3->setMinimumSize(50,10);
    ccd4->setMinimumSize(50,10);

    //Quitar el foco de los botones porque crea confusion cuando se deshabilita algun boton
    qnx1->setFocusPolicy(Qt::NoFocus);
    qnx2->setFocusPolicy(Qt::NoFocus);
    qnx3->setFocusPolicy(Qt::NoFocus);
    qnx4->setFocusPolicy(Qt::NoFocus);
    ccd1->setFocusPolicy(Qt::NoFocus);
    ccd2->setFocusPolicy(Qt::NoFocus);
    ccd3->setFocusPolicy(Qt::NoFocus);
    ccd4->setFocusPolicy(Qt::NoFocus);



    //Posicion en el Grid
    infoLayout->setAlignment(Qt::AlignLeft);
    infoLayout->addWidget(qnx1, 0, 0);  //(objeto, fila, columna)
    infoLayout->addWidget(qnx2, 1, 0);
    infoLayout->addWidget(qnx3, 2, 0);
    infoLayout->addWidget(qnx4, 3, 0);
    infoLayout->addWidget(ccd1, 0, 1);
    infoLayout->addWidget(ccd2, 1, 1);
    infoLayout->addWidget(ccd3, 2, 1);
    infoLayout->addWidget(ccd4, 3, 1);

    infoLayout->addWidget(countLabel, 0, 4); //(fila, columna)
    infoLayout->addWidget(countValue, 0, 5);
    infoLayout->addWidget(xPosLabel, 1, 4);
    infoLayout->addWidget(xPosValue, 1, 5);
    infoLayout->addWidget(yPosLabel, 2, 4);
    infoLayout->addWidget(yPosValue, 2, 5);

    infoLayout->addWidget(meanLabel, 0, 6);
    infoLayout->addWidget(meanValue, 0, 7);
    infoLayout->addWidget(sigmaLabel, 1, 6);
    infoLayout->addWidget(sigmaValue, 1, 7);
    infoLayout->addWidget(sigmaValue, 1, 7);



    topLayout = new QVBoxLayout;
    topLayout->addLayout(titleLayout);
    topLayout->addLayout(infoLayout);

    bottomLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    scrollArea = new QScrollArea;
    scrollBar = new QScrollBar(Qt::Horizontal);
    scrollArea->setHorizontalScrollBar(scrollBar);
    //scrollBar = scrollArea->horizontalScrollBar();

    modia = new Modia;
    scrollArea->setWidget(modia);
    bottomLayout->addWidget(scrollArea);;

    mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);
    setLayout(mainLayout);

    //Conectar la señal de clicked de cada boton con su respectiva funcion
    connect((qnx1), SIGNAL(clicked()), this, SLOT(qnx1Event()));
    connect((qnx2), SIGNAL(clicked()), this, SLOT(qnx2Event()));
    connect((qnx3), SIGNAL(clicked()), this, SLOT(qnx3Event()));
    connect((qnx4), SIGNAL(clicked()), this, SLOT(qnx4Event()));
    connect((ccd1), SIGNAL(clicked()), this, SLOT(ccd1Event()));
    connect((ccd2), SIGNAL(clicked()), this, SLOT(ccd2Event()));
    connect((ccd3), SIGNAL(clicked()), this, SLOT(ccd3Event()));
    connect((ccd4), SIGNAL(clicked()), this, SLOT(ccd4Event()));


  setWindowTitle(QObject::trUtf8("Módulo de Despliegue de Imágenes Astronómicas en Tiempor Real (MODIA)"));
  this->setFixedSize(1024,710);
  setWindowIcon(QIcon("/home/ievivas/Documents/Modia2048_Paralelo_v1/cidaicon.png"));
  nleft = 4096;
  pos_i = 0;
  j = 0;
  qnx1->setDisabled(1);
  ccd1->setDisabled(1);
  useqnx = 1;
  useccd = 1;
}


void Window::qnx1Event(){
    modia->resetModia();
    useqnx = 1;
    useccd = 1;
    this->buttomDisabled();
    this->sendQNXMessage();
}


void Window::qnx2Event(){
    modia->resetModia();
    useqnx = 2;
    useccd = 1;
    this->buttomDisabled();
    this->sendQNXMessage();
}



void Window::qnx3Event(){
    modia->resetModia();
    useqnx = 3;
    useccd = 1;
    this->buttomDisabled();
    this->sendQNXMessage();
}

void Window::qnx4Event(){
    modia->resetModia();
    useqnx = 4;
    useccd = 1;
    this->buttomDisabled();
    this->sendQNXMessage();
}

void Window::ccd1Event(){
    modia->resetModia();
    useccd = 1;
    this->buttomDisabled();
    this->sendCCDMessage();
}

void Window::ccd2Event(){
    modia->resetModia();
    useccd = 2;
    this->buttomDisabled();
    this->sendCCDMessage();
}

void Window::ccd3Event(){
    modia->resetModia();
    useccd = 3;
    this->buttomDisabled();
    this->sendCCDMessage();
}

void Window::ccd4Event(){
    modia->resetModia();
    useccd = 4;
    this->buttomDisabled();
    this->sendCCDMessage();
}


void Window::buttomDisabled(){
    ccd1->setDisabled(0);
    ccd2->setDisabled(0);
    ccd3->setDisabled(0);
    ccd4->setDisabled(0);
    qnx1->setDisabled(0);
    qnx2->setDisabled(0);
    qnx3->setDisabled(0);
    qnx4->setDisabled(0);
    if (useqnx == 1)
        qnx1->setDisabled(1);
    else if (useqnx == 2)
        qnx2->setDisabled(1);
    else if (useqnx == 3)
        qnx3->setDisabled(1);
    else if (useqnx == 4)
        qnx4->setDisabled(1);
    if (useccd == 1)
        ccd1->setDisabled(1);
    else if (useccd == 2)
        ccd2->setDisabled(1);
    else if (useccd == 3)
        ccd3->setDisabled(1);
    else if (useccd == 4)
        ccd4->setDisabled(1);
}


void Window::connection(QTcpSocket *_socket){
    socket = _socket;
    connect(socket, SIGNAL(readyRead()), this, SLOT(recv()));
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(setMeanSigma()));
    timer->start(3000);
}

void Window::sendCCDMessage() {
    QDataStream out(socket);
    char msg[64];
    sprintf(msg, "NEWCCD %d", useccd);
    if((out.writeRawData((char*)msg, sizeof(msg)))<=0){
        qDebug() << "Error al escribir en el socket";
    }
    socket->flush();
    qDebug() << "Mensaje enviado: ";
}

void Window::sendQNXMessage() {
    QDataStream out(socket);
    char msg[64];
    sprintf(msg, "NEWQNX %d", useqnx);
    if((out.writeRawData((char*)msg, sizeof(msg)))<=0){
        qDebug() << "Error al escribir en el socket";
    }
    socket->flush();
    qDebug() << "Mensaje enviado: ";
}


void Window::recv(){
     QDataStream in(socket);
     nread = in.readRawData((char*)line, nleft);

     //Buffer
        if (nread <= 0)
                qDebug() << "Error al leer la linea del socket";

        else if (nread < 4096) {
            std::cout << "nread = " << nread << std::endl;
            nleft = nleft - nread;
            pos_f = pos_i + nread/2;
            for (i=pos_i; i<pos_f; i++){
                 line2[i] =  line[j];
                 j++;
            }
            pos_i = pos_f;
            j = 0;
            if (nleft == 0) {
                modia->nextLine(line2);
                nleft = 4096;
                pos_i = 0;
            }
        }
        else if (nread == 4096) {
            std::cout << "nread = " << nread << std::endl;
            modia->nextLine(line);
            std::cout << "paso nextline" << nread << std::endl;
            nleft = 4096;
            pos_i = 0;
        }
}


void Window::setMeanSigma(){
    meanValue->setNum(modia->getMean());
    sigmaValue->setNum(modia->getSigma());
}


void Window::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        //hvalue = scrollArea->horizontalScrollBar()->value(); //Pixeles que se ha desplazado el scroll horizontal
        hvalue = scrollBar->value(); //Pixeles que se ha desplazado el scroll horizontal
        point = modia->getPos(event, hvalue);
        xPosValue->setNum(point.x());
        yPosValue->setNum(point.y());
        countValue->setNum(modia->getCountValue(event, hvalue));
    }
}
