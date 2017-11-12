#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QString>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent), ui(new Ui::MainWindow){
  ui->setupUi(this);
  socket = new QTcpSocket(this);
  id = 0;
  timer = new QTimer(this);
  connect(timer,SIGNAL(timeout()),this,SLOT(putData()));

}

void MainWindow::tcpConnect(){
    qDebug() << ui->lineEdit_ip->text();
  socket->connectToHost(ui->lineEdit_ip->text(),1234);
  if(socket->waitForConnected(3000)){
    qDebug() << "Connected";
  }
  else{
    qDebug() << "Disconnected";
  }
}

void MainWindow::putData(){
  QString str;
  qint64 msecdate;
  QString random;
  QString datenumber;
  int interval;
  int numRand;
  int min;
  int max;

  min = ui->horizontalSlider_Min->sliderPosition();
  max = ui->horizontalSlider_Max->sliderPosition();

  if(max>min){
    interval = max - min + 1;
    numRand = (qrand()%interval) + (min);
  }
  else if(min>max){
    interval = min - max + 1;
    numRand = (qrand()%interval) + (max);
  }
  else
      numRand = 0;

  random = QString::number(numRand);


  if(socket->state()== QAbstractSocket::ConnectedState){

    msecdate = QDateTime::currentDateTime().toMSecsSinceEpoch();
    datenumber = QString::number(msecdate);
    str = "set "+ datenumber + " " + random +"\r\n";



      qDebug() << str;
      qDebug() << socket->write(str.toStdString().c_str()) << " bytes written";
      if(socket->waitForBytesWritten(3000)){
        qDebug() << "wrote - " << id;
      }
  }
  ui->textBrowser->append(str);
}

void MainWindow::connecting()
{
    tcpConnect();
}

void MainWindow::disconnecting()
{
    socket->disconnectFromHost();
    qDebug() << "Disconnected";
}

void MainWindow::timerEvent(QTimerEvent *e)
{
    putData();
    id = e->timerId();
}

void MainWindow::starting()
{
    if(id==0){
        startTimer(ui->horizontalSlider_Timing->value()*1000);
    }
    else{
        killTimer(id);
        startTimer(ui->horizontalSlider_Timing->value()*1000);
    }
}



void MainWindow::stopping()
{
    killTimer(id);
    id = 0;
}

MainWindow::~MainWindow(){
  delete socket;
  delete ui;
  delete timer;
}
