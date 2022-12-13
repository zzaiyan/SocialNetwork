#ifndef NODE_H
#define NODE_H
#include "net.h"
#include "role.h"
#include <QDebug>
#include <QString>

class Role;

struct RoleData {
  int ID, color;
  QString name;
  QString desc;
  Role *item;

public:
  RoleData() = default;
  RoleData(int x, const QString &s, Role *p = nullptr)
      : ID(x), name(s), item(p) {}

  void print() { qDebug() << QString("Role %1: %2").arg(ID).arg(name); }
  void setItem(Role *p) { item = p; }
};

struct RelData {
  QString label;

public:
  RelData() = default;
  RelData(const QString &s) : label(s) {}

  void print() { qDebug() << QString("Label: %1").arg(label); }
};

#endif // NODE_H
