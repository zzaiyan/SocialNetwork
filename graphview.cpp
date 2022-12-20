#include "graphview.h"
#include <QtOpenGL>

GraphView::GraphView(MyCanvas *p, QWidget *parent)
    : QGraphicsView(parent), canvas(p) {
  this->setBackgroundBrush(Qt::transparent);
  this->setRenderHints(QPainter::Antialiasing |
                       QPainter::SmoothPixmapTransform); // 图像平滑和抗锯齿
  this->setCacheMode(
      QGraphicsView::CacheBackground); // 设置缓存模式，加速渲染背景图。
  this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
  setMouseTracking(true);                               // 跟踪鼠标位置
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 隐藏水平条
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // 隐藏竖条
  this->setDragMode(ScrollHandDrag);
  line = nullptr;
}

//通过此itemMoved（）函数收到节点移动的通知。
//它的工作只是重新启动主计时器，以防它尚未运行。
void GraphView::itemMoved() {
  if (!timerId)
    timerId = startTimer(0);
}

//计时器事件处理程序的工作是将整个力计算机制作为平滑动画运行。
void GraphView::timerEvent(QTimerEvent *event) {
  Q_UNUSED(event);
  if (mode) {
    //计算所有节点受到的力
    QVector<Role *> roles;
    const QList<QGraphicsItem *> items = scene()->items();
    for (QGraphicsItem *item : items) {
      if (Role *role = qgraphicsitem_cast<Role *>(item))
        roles << role;
    }
    for (Role *role : qAsConst(roles))
      role->calculateForces();
    //将所有节点移动到其新位置
    bool itemsMoved = false;
    for (Role *role : qAsConst(roles)) {
      if (role->advancePosition())
        itemsMoved = true;
    }
    //如果没有节点移动，停止计时器
    if (!itemsMoved) {
      killTimer(timerId);
      timerId = 0;
    }
  }
}

void GraphView::wheelEvent(QWheelEvent *event) {
  int wheelDeltaValue = event->delta();
  // 向上滚动，放大
  if (wheelDeltaValue > 0) {
    this->scale(1.2, 1.2);
  }
  // 向下滚动，缩小
  else {
    this->scale(1.0 / 1.2, 1.0 / 1.2);
  }
}

void GraphView::mousePressEvent(QMouseEvent *mouseEvent) {
  if (mouseEvent->button() == Qt::RightButton) {
    line = new QGraphicsLineItem(
        QLineF(mapToScene(mouseEvent->pos()), mapToScene(mouseEvent->pos())));
    line->setPen(QPen(Qt::gray, 2, Qt::DashLine));
    scene()->addItem(line);
  } else
    QGraphicsView::mousePressEvent(mouseEvent);
}

void GraphView::mouseMoveEvent(QMouseEvent *mouseEvent) {
  if (line != nullptr) {
    QLineF newLine(line->line().p1(), mapToScene(mouseEvent->pos()));
    line->setLine(newLine);
  } else {
    QGraphicsView::mouseMoveEvent(mouseEvent);
  }
}

void GraphView::mouseReleaseEvent(QMouseEvent *mouseEvent) {
  if (line != nullptr) {
    QList<QGraphicsItem *> startItems = scene()->items(line->line().p1());
    if (startItems.count() && startItems.first() == line)
      startItems.removeFirst();
    QList<QGraphicsItem *> endItems = scene()->items(line->line().p2());
    if (endItems.count() && endItems.first() == line)
      endItems.removeFirst();
    if (!startItems.isEmpty())
      qDebug() << startItems.count();
    scene()->removeItem(line);
    delete line;
    if (startItems.count() > 0 && endItems.count() > 0 &&
        startItems.first()->type() == Role::Type &&
        endItems.first()->type() == Role::Type &&
        startItems.first() != endItems.first()) {
      auto startItem = qgraphicsitem_cast<Role *>(startItems.first());
      auto endItem = qgraphicsitem_cast<Role *>(endItems.first());
      // 判断是否已有边
      auto ver1 = canvas->hashID[startItem->ID];
      auto ver2 = canvas->hashID[endItem->ID];
      if (!canvas->net.haveArc(ver1, ver2) &&
          !canvas->net.haveArc(ver2, ver1)) {
        auto rel = new Rel(startItem, endItem);
        rel->setText("newRel");
        canvas->addNetArc(startItem->name, endItem->name, "newRel");
        qDebug() << startItem->name << endItem->name;
        scene()->addItem(rel);
      }
    }
  }
  line = nullptr;
  QGraphicsView::mouseReleaseEvent(mouseEvent);
}
