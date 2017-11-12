#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

    QTimer *timer;


  void tcpConnect();
public slots:
  //Cabecalhos dos slots publicos
  void putData();
  void connecting();
  void disconnecting();
  void timerEvent(QTimerEvent *e);
  void starting();
  void stopping();
private:
  Ui::MainWindow *ui;
  QTcpSocket *socket;
  int id;
};

#endif // MAINWINDOW_H
