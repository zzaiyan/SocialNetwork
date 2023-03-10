#ifndef ROLE_H
#define ROLE_H

#include "rel.h"
#include <QApplication>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QGraphicsView>
#include <QObject>
#include <QRectF>
#include <QtSvg>

class Rel;
class MyGraphicsView;
class Role : public QGraphicsItem {
public:
  enum { Type = UserType + 77 };
  int type() const override { return Type; }
  Role(int id, MyGraphicsView *view, QString tname = "");
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;
  void setColor(int c);
  void setName(QString tname);
  //  void setImgPath(QString path);
  void addRel(Rel *rel);
  void removeThis();
  void removeRel(Rel *rel);
  qreal getRadius() const { return radius; }

  void calculateForces();
  bool advancePosition();

  //  RoleData *data = nullptr;
  //  void setData(RoleData *p) { data = p; }

  int ID;
  MyGraphicsView *view;
  QString name;
  //  QString imgPath;
  QPointF newPos;

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
  /* basic data */
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
  const QColor MAGENTA = QColor(215, 123, 167);
  const QColor GREY = QColor(163, 163, 163);
  const QColor GREEN = QColor(137, 191, 125);
  QColor color;
};
#endif // ROLE_H
