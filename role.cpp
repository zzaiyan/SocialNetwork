#include "role.h"

Role::Role(int id, QString tname, QString imgPath)
    : ID(id), imgPath(imgPath), color(BLUE), radius(30) {
  // 可选择、可移动
  setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
  setFlag(ItemSendsGeometryChanges);   // 打开通知
  setCacheMode(DeviceCoordinateCache); // 加快渲染性能
  if (tname == "")
    name = "Role-" + QString::number(id);
  else
    name = tname;
  nameTag = new QGraphicsTextItem;
  nameTag->setParentItem(this);
  nameTag->setFont(nameFont);
  nameTag->setPlainText(name);
  nameTag->adjustSize();
  nameTag->setPos(-nameTag->textWidth() / 2, radius);
  nameTag->setZValue(this->zValue());
}

QRectF Role::boundingRect() const {
  qreal adjust = 2;
  return QRectF(-radius - adjust, -radius - adjust, 2 * radius + adjust,
                2 * radius + adjust);
}

void Role::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget) {
  Q_UNUSED(widget);
  QPen pen(Qt::NoPen);
  QBrush brush;
  brush.setStyle(Qt::SolidPattern);
  if (option->state & QStyle::State_Sunken) { // 设置按下时的颜色
    brush.setColor(color.darker(130));
  } else if (option->state & QStyle::State_Selected) { // 设置选中时的颜色
    brush.setColor(color.darker(120));
  } else { // 设置松开时的颜色
    brush.setColor(color);
  }
  painter->setPen(pen);
  painter->setBrush(brush);
  painter->drawEllipse(QPointF(0, 0), radius, radius);
  // 根据缩放级别调整绘制细节，只有缩放级别较大时才绘制图标和图片
  const qreal lod =
      option->levelOfDetailFromTransform(painter->worldTransform());
  if (lod > 0.3) {
    if (!imgPath.isEmpty()) {
      qreal delta = 4;
      brush.setColor(Qt::white);
      painter->setBrush(brush);
      painter->drawEllipse(QPointF(0, 0), radius - delta, radius - delta);
      QPixmap pix(imgPath);
      painter->setRenderHint(QPainter::Antialiasing);
      painter->setRenderHint(QPainter::SmoothPixmapTransform);
      QPainterPath path;
      delta = 8;
      path.addEllipse(-(radius - delta), -(radius - delta),
                      2 * (radius - delta), 2 * (radius - delta));
      painter->setClipPath(path);
      painter->drawPixmap(QRect(-(radius - delta), -(radius - delta),
                                (radius - delta) * 2, (radius - delta) * 2),
                          pix);
    } else {
      QString filename = ":/icon/role1.svg";
      QSvgRenderer m_svgRender(filename);
      m_svgRender.render(painter,
                         QRectF(-radius, -radius, 2 * radius, 2 * radius));
    }
  }
}

void Role::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (event->buttons() & Qt::LeftButton) { // 若按下左键
    QGraphicsItem::mousePressEvent(event);
  }
  update();
}
void Role::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mouseMoveEvent(event);
}
void Role::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mouseReleaseEvent(event);
  update();
}
void Role::setColor(int c) {
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
void Role::setName(QString tname) {
  name = tname;
  nameTag->setPlainText(tname);
  nameTag->adjustSize();
  nameTag->setPos(-nameTag->textWidth() / 2, radius);
}
void Role::setImgPath(QString path) {
  imgPath = path;
  update();
}
void Role::addRel(Rel *rel) {
  relList << rel;
  rel->adjust();
}
QVariant Role::itemChange(GraphicsItemChange change, const QVariant &value) {
  switch (change) {
  case ItemPositionHasChanged:
    foreach (Rel *rel, relList)
      rel->adjust();
    break;
  default:
    break;
  };
  return QGraphicsItem::itemChange(change, value);
}

void Role::removeThis() {
  foreach (Rel *rel, relList) {
    rel->removeThis();
  }
  scene()->removeItem(this);
}
