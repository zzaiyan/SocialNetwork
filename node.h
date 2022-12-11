#ifndef NODE_H
#define NODE_H
#include "net.h"
#include "rel.h"
#include "role.h"
#include <QDebug>
#include <QString>

template <class VerData, class ArcData> class ALNet;
class Rel;
class Role;
struct RelData;

struct RoleData {
  int ID, color;
  QString name;
  QString desc;

  Role *role = nullptr;

  RoleData() = default;
  RoleData(int x, const QString &s, Role *p = nullptr)
      : ID(x), name(s), role(p) {}
  //  ~RoleData() {
  //    if (role)
  //      delete role;
  //  }

  void print() { qDebug() << QString("Role %1: %2").arg(ID).arg(name); }
  void setRole(Role *p) { role = p; }
};

struct RelData {
  QString label;

  Rel *rel = nullptr;

  RelData() = default;
  RelData(const QString &s, Rel *r = nullptr) : label(s), rel(r) {}
  ~RelData() {
    if (rel) {
      qDebug() << QString("delete Rel %1").arg(label);
      delete rel;
    }
  }

  void print() { qDebug() << QString("Label: %1").arg(label); }
  void setRel(Rel *p) { rel = p; }
};

#endif // NODE_H
