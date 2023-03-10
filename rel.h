#ifndef REL_H
#define REL_H

#include "node.h"
#include "role.h"
#include <QFont>
#include <QGraphicsItem>

struct RelData;
class Role;
class Rel : public QGraphicsItem {
public:
  enum { Type = UserType + 11 };
  int type() const override { return Type; }
  bool isRemoved = 0;
  QString text;
  Rel(Role *startRole, Role *endRole, QString text = "", int c = 1);
  void init();
  void adjust();
  void setColor(int c);
  void removeThis();
  //  RelData *data;
  //  void setData(RelData *p) { data = p; }
  void setText(QString t) {
    text = t;
    adjust();
  }
  void drawText();
  Role *startRole() { return start; }
  Role *endRole() { return end; }
  /* For display temporary tag */
  QGraphicsTextItem *relTag = nullptr;

protected:
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;

private:
  QFont tagFont = QFont("微软雅黑", 12, QFont::Normal);
  /* basic data */
  Role *start, *end;
  qreal arrowSize;
  QPointF startPoint;
  QPointF endPoint;
  QPointF center;
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

#endif // REL_H
