#include "role.h"
#include "graphview.h"

Role::Role(int id, GraphView *view, QString tname, QString imgPath)
    : ID(id), view(view), imgPath(imgPath), color(BLUE), radius(30) {
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

//推拉力算法
void Role::calculateForces() {
  if (!scene() || scene()->mouseGrabberItem() == this) {
    newPos = pos();
    return;
  }
  //画布中的节点会互相排斥
  // 计算将节点推开的力
  qreal xvel = 0;
  qreal yvel = 0;
  const QList<QGraphicsItem *> items = scene()->items();
  for (QGraphicsItem *item : items) {
    //使用qgraphicsitem_cast（）来查找Role实例并进行类型转换，即去掉边Item
    Role *role = qgraphicsitem_cast<Role *>(item);
    if (!role)
      continue;
    //创建一个临时矢量，从this节点指向另一个节点
    QPointF vec = mapToItem(role, 0, 0);
    //我们使用此矢量的分解分量来确定应用于this节点的力的方向和强度。
    qreal dx = vec.x();
    qreal dy = vec.y();
    //力为每个节点累积，然后进行调整，以便为最近的节点提供最强的力，当距离增加时会迅速退化。
    double l = 2.0 * (dx * dx + dy * dy);
    if (l > 0) {
      xvel += (dx * 1000.0) / l;
      yvel += (dy * 1000.0) / l;
    }
  }
  //有关联的节点通过连线相连，并且相互靠拢
  // 减去将节点拉在一起的力
  double weight = (relList.size() + 1) * 10;
  //通过访问连接到此节点的每个边，我们可以使用与上面类似的方法来找到所有拉力的方向和强度。
  for (Rel *rel : relList) {
    QPointF vec;
    if (rel->startRole() == this)
      vec = mapToItem(rel->endRole(), 0, 0);
    else
      vec = mapToItem(rel->startRole(), 0, 0);
    xvel -= vec.x() / weight;
    yvel -= vec.y() / weight;
  }
  //为了规避数值精度的误差，我们只需强制力的总和在小于0.1时为0。
  if (qAbs(xvel) < 1.1 && qAbs(yvel) < 1.1)
    xvel = yvel = 0;
  qDebug() << xvel << " " << yvel;

  //确定节点的新位置。我们将力添加到节点的当前位置。
  newPos = pos() + QPointF(xvel, yvel);
  newPos.setX(newPos.x());
  newPos.setY(newPos.y());
}

bool Role::advancePosition() {
  if (newPos == pos())
    return false;

  setPos(newPos);
  return true;
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
    view->itemMoved();
    break;
  default:
    break;
  };
  return QGraphicsItem::itemChange(change, value);
}

void Role::removeThis() {
  foreach (Rel *rel, relList) { rel->removeThis(); }
  scene()->removeItem(this);
}
