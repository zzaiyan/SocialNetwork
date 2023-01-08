#ifndef QUERYWIDGET_H
#define QUERYWIDGET_H

#include "canvaswidget.h"
#include <QWidget>
class CanvasWidget;

namespace Ui {
class QueryWidget;
}

class QueryWidget : public QWidget {
  Q_OBJECT

  CanvasWidget *canvas;

public:
  explicit QueryWidget(CanvasWidget *p, QWidget *parent = nullptr);
  ~QueryWidget();

private slots:
  void on_queryRole_clicked();

  void on_queryRel_clicked();

private:
  Ui::QueryWidget *ui;
};

#endif // QUERYWIDGET_H
