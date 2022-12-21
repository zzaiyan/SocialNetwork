#ifndef NODE_H
#define NODE_H
#include "net.h"
#include "role.h"
#include <QDebug>
#include <QString>

class Role;
class Rel;
struct RoleData {
  int ID, color;
  Role *item;
  QString name, imgPath;

public:
  RoleData() = default;
  RoleData(int x, const QString &s, Role *p = nullptr, int c = 1,
           QString path = "")
      : ID(x), color(c), item(p), name(s), imgPath(path) {}

  void print() { qDebug() << QString("Role %1: %2").arg(ID).arg(name); }
  void setItem(Role *p) { item = p; }
  void setImage(const QString &path) { imgPath = path; }
};

struct RelData {
  QString label;
  int color;
  int cohesion; //亲密度
  Rel *item;

public:
  RelData() = default;
  RelData(const QString &s, int c = 1, Rel *p = nullptr, int coh = 50)
      : label(s), color(c), item(p), cohesion(coh) {}

  void print() { qDebug() << QString("Label: %1").arg(label); }
};

#endif // NODE_H
