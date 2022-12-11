#ifndef REL_H
#define REL_H

#include "node.h"
#include "role.h"
#include <QGraphicsItem>

struct RelData;
class Role;
class Rel : public QGraphicsItem {
public:
  Rel(Role *startRole, Role *endRole);
  Rel(Role *startRole, Role *endRole, QColor c, QString text = "");
  void setColor(int c);
  void adjust();

  RelData *data;
  void setData(RelData *p) { data = p; }

protected:
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;

private:
  QString text;
  Role *start, *end;
  qreal arrowSize;
  QPointF startPoint;
  QPointF endPoint;
  /* color */
  const QColor BLUE = QColor(115, 165, 210);
  const QColor RED = QColor(216, 135, 133);
  const QColor PURPLE = QColor(164, 130, 210);
  const QColor YELLOW = QColor(208, 181, 117);
  QColor color;
};

#endif // REL_H
