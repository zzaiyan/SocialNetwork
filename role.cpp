#include "role.h"
#include "graphview.h"
#include <algorithm>
#include <cmath>

Role::Role(int id, GraphView *view, QString tname, QString imgPath)
    : ID(id), view(view), imgPath(imgPath), color(BLUE), radius(20) {
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
  // prepareGeometryChange();
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
    nameTag->setVisible(true);
    nameTag->setPos(-nameTag->textWidth() / 2, radius);
  } else {
    nameTag->setVisible(false);
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

// 推拉力算法
void Role::calculateForces() {
  if (!scene() || scene()->mouseGrabberItem() == this) {
    newPos = pos();
    return;
  }
  // 画布中的节点会互相排斥
  //  计算将节点推开的力
  qreal xvel = 0;
  qreal yvel = 0;
  const QList<QGraphicsItem *> items = scene()->items();
  for (QGraphicsItem *item : items) {
    // 使用qgraphicsitem_cast（）来查找Role实例并进行类型转换，即去掉边Item
    Role *role = qgraphicsitem_cast<Role *>(item);
    if (!role)
      continue;
    // 创建一个临时矢量，从this节点指向另一个节点
    QPointF vec = mapToItem(role, 0, 0);
    // 我们使用此矢量的分解分量来确定应用于this节点的力的方向和强度。
    qreal dx = vec.x();
    qreal dy = vec.y();
    // 力为每个节点累积，然后进行调整，以便为最近的节点提供最强的力，当距离增加时会迅速退化。
    double l = 2.0 * (dx * dx + dy * dy);
    if (l > 0) {
      //      int degree = relList.size();
      xvel += (dx * 2000.0) / l;
      yvel += (dy * 2000.0) / l;
    }
  }
  // 假设所有节点与画布中心点都有一条看不见的弹性绳
  // 节点距离画布中心越远，收到绳子的拉力越大
  QPointF vec = scenePos();
  qreal dx = vec.x();
  qreal dy = vec.y();
  // qDebug() << vec.x() << " " << vec.y();
  xvel -= dx / 50;
  yvel -= dy / 50;

  // 有关联的节点通过连线相连，并且相互靠拢
  //  减去将节点拉在一起的力
  double weight = (relList.size() + 1) * 10;
  // 通过访问连接到此节点的每个边，我们可以使用与上面类似的方法来找到所有拉力的方向和强度。
  auto vecLen = [](QPointF p) { return sqrt(p.x() * p.x() + p.y() * p.y()); };
  for (auto &&rel : relList) {
    QPointF vec;
    auto start = rel->startRole();
    auto end = rel->endRole();
    auto dis = vecLen(end->scenePos() - start->scenePos()); // center distance
    if (start == this)
      vec = mapToItem(end, 0, 0);
    else
      vec = mapToItem(start, 0, 0);
    auto edgeDis = std::max(0.0, dis - start->getRadius() - end->getRadius());
    auto rate = edgeDis / dis;
    xvel -= vec.x() * rate / weight;
    yvel -= vec.y() * rate / weight;
  }
  // 为了规避数值精度的误差，我们只需强制力的总和在小于1时为0。
  if (vecLen({xvel, yvel}) < 1)
    xvel = 0, yvel = 0;

  // 确定节点的新位置。我们将力添加到节点的当前位置。
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
  int degree = relList.size();
  radius = 12 * log(degree + 1) / log(3) + 20;
  update();
  view->itemMoved();
  foreach (Rel *rel, relList) { rel->adjust(); }
}

QVariant Role::itemChange(GraphicsItemChange change, const QVariant &value) {
  switch (change) {
  case ItemPositionHasChanged:
    foreach (Rel *rel, relList) {
      rel->adjust();
      rel->drawText();
    }
    view->itemMoved();
    break;
  default:
    break;
  };
  return QGraphicsItem::itemChange(change, value);
}

void Role::removeThis() {
  foreach (Rel *rel, relList) { rel->removeThis(); }
  this->relList.clear();
  scene()->removeItem(this);
  view->itemMoved();
}

void Role::removeRel(Rel *rel) {
  for (auto it = relList.begin(); it != relList.end(); it++) {
    if (*it == rel) {
      relList.erase(it);
      break;
    }
  }
  view->itemMoved();
}
