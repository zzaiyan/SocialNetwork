#include "rel.h"

Rel::Rel(Role *startRole, Role *endRole)
    : start(startRole), end(endRole), text(""), arrowSize(15) {
  start->addRel(this);
  end->addRel(this);
  relTag = new QGraphicsTextItem;
  relTag->setParentItem(this);
  setZValue(-1000);
  setColor(1);
  adjust();
}

Rel::Rel(Role *startRole, Role *endRole, int c, QString text)
    : start(startRole), end(endRole), text(text), arrowSize(15) {
  start->addRel(this);
  end->addRel(this);
  relTag = new QGraphicsTextItem;
  relTag->setParentItem(this);
  setZValue(-1000);
  setColor(c);
  adjust();
}

void Rel::adjust() {
  if (!start || !end || isRemoved)
    return;
  QLineF line(mapFromItem(start, 0, 0), mapFromItem(end, 0, 0));
  qreal length = line.length();
  prepareGeometryChange();
  if (length > qreal(80.)) {
    QPointF edgeOffset((line.dx() * 30) / length, (line.dy() * 30) / length);
    startPoint = line.p1() + edgeOffset;
    endPoint = line.p2() - edgeOffset;
  } else {
    startPoint = endPoint = line.p1();
  }
}

QRectF Rel::boundingRect() const {
  if (!start || !end || isRemoved)
    return QRectF();

  qreal penWidth = 1;
  qreal extra = (penWidth + arrowSize) / 2.0;

  return QRectF(startPoint, QSizeF(endPoint.x() - startPoint.x(),
                                   endPoint.y() - startPoint.y()))
      .normalized()
      .adjusted(-extra, -extra, extra, extra);
}

void Rel::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                QWidget *) {
  if (!start || !end || isRemoved)
    return;
  QLineF line(startPoint, endPoint);
  if (qFuzzyCompare(line.length(), qreal(0.)))
    return;
  // Draw the line itself
  painter->setPen(QPen(color, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  painter->drawLine(line);
  // Draw the arrows
  double angle = std::atan2(-line.dy(), line.dx());
  QPointF startArrowP1 =
      startPoint + QPointF(sin(angle + M_PI / 3) * arrowSize,
                           cos(angle + M_PI / 3) * arrowSize);
  QPointF startArrowP2 =
      startPoint + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                           cos(angle + M_PI - M_PI / 3) * arrowSize);
  QPointF endArrowP1 = endPoint + QPointF(sin(angle - M_PI / 3) * arrowSize,
                                          cos(angle - M_PI / 3) * arrowSize);
  QPointF endArrowP2 =
      endPoint + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                         cos(angle - M_PI + M_PI / 3) * arrowSize);
  painter->setBrush(color);
  painter->drawPolygon(QPolygonF() << line.p2() << endArrowP1 << endArrowP2);
  //
  if (!text.isEmpty()) {
    center = (start->scenePos() + start->boundingRect().center() +
              end->scenePos() + end->boundingRect().center()) /
             2;
    relTag->setFont(tagFont);
    relTag->adjustSize();
    relTag->setZValue(this->zValue());
    relTag->setDefaultTextColor(color.darker(150));
    relTag->setPos(center - QPointF(relTag->boundingRect().width(),
                                    relTag->boundingRect().height()) /
                                2);
    relTag->setHtml("<div style='background-color:rgba(255,255,255,150);'>" +
                    text + "</div>");
    // relTag->setRotation(-line.angle());
  }
}

void Rel::setColor(int c) {
  switch (c) {
  case 1:
    color = BLUE;
    break;
  case 2:
    color = RED;
    break;
  case 3:
    color = PURPLE;
    break;
  case 4:
    color = YELLOW;
    break;
  }
  adjust();
}
void Rel::removeThis() {
  if (!isRemoved) {
    if (relTag)
      scene()->removeItem(relTag);
    scene()->removeItem(this);
  }
  isRemoved = true;
}