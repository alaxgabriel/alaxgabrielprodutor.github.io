#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QString>
#include <QTimer>

/** \brief Construtor da classe */
MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent), ui(new Ui::MainWindow){
    /**
        \details Construtor da classe MainWindow que inicializa o objeto
    */
  ui->setupUi(this);
  socket = new QTcpSocket(this);
  id = 0;
}

/** \brief Slot de conexao */
void MainWindow::tcpConnect(){
    /**
        \details Slot que conecta o produtor de dados ao servidor
    */
    qDebug() << ui->lineEdit_ip->text();
  socket->connectToHost(ui->lineEdit_ip->text(),1234);
  if(socket->waitForConnected(3000)){
    qDebug() << "Connected";
  }
  else{
    qDebug() << "Disconnected";
  }
}

/** \brief Slot produtor de dados */
void MainWindow::putData(){
    /**
        \details Envia dados gerados aleatoriamente para o servidor
    */
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

/** \brief Slot de chamada da conexao */
void MainWindow::connecting(){
    /**
        \details Slot que chama a conexao
    */
    tcpConnect();
}

/** \brief Slot de desconexao */
void MainWindow::disconnecting(){
    /**
        \details Slot que desconecta o produtor do servidor
    */
    socket->disconnectFromHost();
    qDebug() << "Disconnected";
}

/** \brief Chamada do evento temporario */
void MainWindow::timerEvent(QTimerEvent *e){
    /**
        \details Slot que, periodicamente, chama a funcao de producao de dados
    */
    putData();
    id = e->timerId();
}

/** \brief Slot que inicia o timer */
void MainWindow::starting(){
    /**
        \details Inicia o timer que determinara o periodo de repeticao dos eventos
    */
    if(id==0){
        startTimer(ui->horizontalSlider_Timing->value()*1000);
    }
    else{
        killTimer(id);
        startTimer(ui->horizontalSlider_Timing->value()*1000);
    }
}


/** \brief Slot que mata o timer */
void MainWindow::stopping(){
    /**
        \details Finaliza o timer que estiver atuando
    */
    killTimer(id);
    id = 0;
}

/** \brief Destrutor da classe */
MainWindow::~MainWindow(){
    /**
        \details Deleta os objetos alocados dinamicamnete
    */
  delete socket;
  delete ui;
}
