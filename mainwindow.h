#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mycanvas.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_createNew_clicked();

  void on_openFile_clicked();

private:
  Ui::MainWindow *ui;
  MyCanvas *canvas = nullptr;
};
#endif // MAINWINDOW_H
