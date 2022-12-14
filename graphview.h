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
public slots:
protected:
  MyCanvas *canvas;
  QGraphicsLineItem *line;
  void wheelEvent(QWheelEvent *event) override;
  void mousePressEvent(QMouseEvent *mouseEvent) override;
  void mouseMoveEvent(QMouseEvent *mouseEvent) override;
  void mouseReleaseEvent(QMouseEvent *mouseEvent) override;
};

#endif // GRAPHVIEW_H
