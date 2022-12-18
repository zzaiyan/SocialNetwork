#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include "mycanvas.h"
#include "node.h"
#include "role.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QObject>
#include <QRandomGenerator>
#include <QWheelEvent>
#include <math.h>

class MyCanvas;

class GraphView : public QGraphicsView {
  Q_OBJECT
public:
  explicit GraphView(MyCanvas *p, QWidget *parent = nullptr);
  void itemMoved();
public slots:
protected:
  MyCanvas *canvas;
  QGraphicsLineItem *line;
  int timerId = 0;
  void wheelEvent(QWheelEvent *event) override;
  void mousePressEvent(QMouseEvent *mouseEvent) override;
  void mouseMoveEvent(QMouseEvent *mouseEvent) override;
  void mouseReleaseEvent(QMouseEvent *mouseEvent) override;
  void timerEvent(QTimerEvent *event) override;
};

#endif // GRAPHVIEW_H
