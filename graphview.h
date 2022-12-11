#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QWheelEvent>
#include <math.h>
#include <QRandomGenerator>
#include "role.h"

class GraphView : public QGraphicsView {
    Q_OBJECT
  public:
    explicit GraphView(QWidget *parent = nullptr);
  public slots:
  protected:
    bool sign;
    bool m_bMouseTranslate = false;
    qreal m_scale,m_translateSpeed;
    QPoint m_lastMousePos;
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *event) override;
#endif
};

#endif // GRAPHVIEW_H
