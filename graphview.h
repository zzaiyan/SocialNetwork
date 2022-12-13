#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include "role.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QObject>
#include <QRandomGenerator>
#include <QWheelEvent>
#include <math.h>

class GraphView : public QGraphicsView {
  Q_OBJECT
public:
  explicit GraphView(QWidget *parent = nullptr);
public slots:
protected:
  QGraphicsLineItem *line;
  void wheelEvent(QWheelEvent *event) override;
  void mousePressEvent(QMouseEvent *mouseEvent) override;
  void mouseMoveEvent(QMouseEvent *mouseEvent) override;
  void mouseReleaseEvent(QMouseEvent *mouseEvent) override;
};

#endif // GRAPHVIEW_H
