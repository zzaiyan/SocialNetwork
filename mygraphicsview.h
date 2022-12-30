#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

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

class MyGraphicsView : public QGraphicsView {
  Q_OBJECT
public:
  explicit MyGraphicsView(MyCanvas *p, QWidget *parent = nullptr);
  void itemMoved();
  int mode = 0;   //是否使用力导向分布算法，默认不使用
  qreal zoom = 1; // zoom用于记录对象的当前的缩放比
public slots:
protected:
  MyCanvas *canvas;
  QGraphicsLineItem *line;
  int timerId = 0;
  void wheelEvent(QWheelEvent *event) override;
  void MyScale(qreal step);
  void mousePressEvent(QMouseEvent *mouseEvent) override;
  void mouseMoveEvent(QMouseEvent *mouseEvent) override;
  void mouseReleaseEvent(QMouseEvent *mouseEvent) override;
  void timerEvent(QTimerEvent *event) override;
};

#endif // MYGRAPHICSVIEW_H
