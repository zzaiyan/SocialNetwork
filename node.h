#ifndef NODE_H
#define NODE_H
#include <QDebug>
#include <QString>
#include "net.h"
#include "role.h"

class Role;

struct RoleData {
  int ID, color;
  QString name, imgPath;
  Role* item;

 public:
  RoleData() = default;
  RoleData(int x, const QString& s, Role* p = nullptr)
      : ID(x), name(s), item(p) {}

  void print() { qDebug() << QString("Role %1: %2").arg(ID).arg(name); }
  void setItem(Role* p) { item = p; }
  void setImage(const QString& path) { imgPath = path; }
};

struct RelData {
  QString label;
  int color;

 public:
  RelData() = default;
  RelData(const QString& s) : label(s) {}

  void print() { qDebug() << QString("Label: %1").arg(label); }
};

#endif  // NODE_H
