#ifndef LAUNCHER_H
#define LAUNCHER_H

#include "canvaswidget.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class Launcher;
}
QT_END_NAMESPACE

class Launcher : public QMainWindow {
  Q_OBJECT

public:
  Launcher(QWidget *parent = nullptr);
  ~Launcher();

private slots:
  void on_createNew_clicked();

  void on_openFile_clicked();

private:
  Ui::Launcher *ui;
  CanvasWidget *canvas = nullptr;
};
#endif // LAUNCHER_H
