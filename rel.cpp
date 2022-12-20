#include "rel.h"

Rel::Rel(Role* startRole, Role* endRole, QString text, int c)
    : start(startRole), end(endRole), text(text), arrowSize(15) {
  setColor(c);
  init();
  adjust();
}

void Rel::init() {
  start->addRel(this);
  end->addRel(this);
  setZValue(-1000);
  relTag = new QGraphicsTextItem;
  relTag->setParentItem(this);
  relTag->setFlag(QGraphicsItem::ItemIsSelectable);
  relTag->setFont(tagFont);
  relTag->setZValue(this->zValue());
}
void Rel::adjust() {
  if (!start || !end || isRemoved)
    return;
  QLineF line(mapFromItem(start, 0, 0), mapFromItem(end, 0, 0));
  qreal length = line.length();
  prepareGeometryChange();
  if (length > qreal(80.)) {
    //计算箭头偏移量
    QPointF edgeOffset1((line.dx() * startRole()->getRadius()) / length,
                        (line.dy() * startRole()->getRadius()) / length),
        edgeOffset2((line.dx() * endRole()->getRadius()) / length,
                    (line.dy() * endRole()->getRadius()) / length);
    startPoint = line.p1() + edgeOffset1;
    endPoint = line.p2() - edgeOffset2;
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

void Rel::paint(QPainter* painter,
                const QStyleOptionGraphicsItem* option,
                QWidget*) {
  if (!start || !end || isRemoved)
    return;
  QLineF line(startPoint, endPoint);
  if (qFuzzyCompare(line.length(), qreal(0.)))
    return;
  prepareGeometryChange();
  painter->setPen(QPen(color, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  painter->setBrush(color);
  // Draw the line itself
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

  painter->drawPolygon(QPolygonF() << line.p2() << endArrowP1 << endArrowP2);
  const qreal lod =
      option->levelOfDetailFromTransform(painter->worldTransform());
  if (lod > 0.5) {
    // Draw the text
    relTag->setVisible(true);
    drawText();
  } else {
    relTag->setVisible(false);
  }
}
void Rel::drawText() {
  if (!text.isEmpty()) {
    // relTag->setRotation(-line.angle());
    relTag->setDefaultTextColor(color.darker(150));
    relTag->setHtml("<div style = 'background-color:rgba(255,255,255,150);' >" +
                    text + "</div>");
    relTag->adjustSize();
    auto vecLen = [](QPointF p) { return sqrt(p.x() * p.x() + p.y() * p.y()); };
    auto AB = end->scenePos() - start->scenePos();
    auto AM = AB *
              (start->getRadius() +
               0.5 * (vecLen(AB) - start->getRadius() - end->getRadius()) -
               arrowSize * 1.732 / 4) /
              vecLen(AB);
    center = (start->scenePos() + AM);
    relTag->setPos(center - QPointF(relTag->boundingRect().width(),
                                    relTag->boundingRect().height()) /
                                2);
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
  update();
}

void Rel::removeThis() {
  if (!isRemoved) {
    if (relTag)
      scene()->removeItem(relTag);
    scene()->removeItem(this);
    start->removeRel(this);
    end->removeRel(this);
    isRemoved = true;
  }
}
