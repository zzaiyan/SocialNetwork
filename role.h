#ifndef ROLE_H
#define ROLE_H

#include "node.h"
#include "rel.h"
#include <QApplication>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QGraphicsView>
#include <QRectF>
#include <QtSvg>

struct RoleData;
class Rel;
class Role : public QGraphicsItem {
public:
  Role(QString imgPath = "");
  ~Role();
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;
  void setColor(int c);
  void setName(QString name);
  void setImgPath(QString path);
  void addRel(Rel *rel);

  RoleData *data = nullptr;
  void setData(RoleData *p) { data = p; }

  QString imgPath;
  QString nameText = "";

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
  qreal radius;
  QList<Rel *> relList;
  /* For display temporary tag */
  QGraphicsTextItem *nameTag = nullptr;
  QFont nameFont = QFont("微软雅黑", 13, QFont::Normal);
  /* color */
  const QColor BLUE = QColor(115, 165, 210);
  const QColor RED = QColor(216, 135, 133);
  const QColor PURPLE = QColor(164, 130, 210);
  const QColor YELLOW = QColor(208, 181, 117);
  QColor color;
};
#endif // ROLE_H
