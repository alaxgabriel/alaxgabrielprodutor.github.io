#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent), ui(new Ui::MainWindow){
  ui->setupUi(this);
  socket = new QTcpSocket(this);


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
  QDateTime datetime;
  QString str;
  qint64 msecdate;
  QString random;
  QString datenumber;

  //QString::number(qrand()%35)

  random = QString::number(qrand()%35);


  if(socket->state()== QAbstractSocket::ConnectedState){

    msecdate = QDateTime::currentDateTime().toMSecsSinceEpoch();
    datenumber = QString::number(msecdate);
    str = "set "+ datenumber + " " + random +"\r\n";



      qDebug() << str;
      qDebug() << socket->write(str.toStdString().c_str()) << " bytes written";
      if(socket->waitForBytesWritten(3000)){
        qDebug() << "wrote";
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

void MainWindow::starting()
{
    putData();
}

MainWindow::~MainWindow(){
  delete socket;
  delete ui;
}
