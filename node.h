#ifndef NODE_H
#define NODE_H
//#include "mycanvas.h"
#include "net.h"
#include "rel.h"
#include "role.h"
#include <QDebug>
#include <QString>

class Role;
class Rel;
struct RoleData {
  int ID, color;
  Role *item;
  QString name, imgPath;
  int impact = 0; //影响力

public:
  RoleData() = default;
  RoleData(int x, const QString &s, Role *p = nullptr, int c = 1,
           QString path = "")
      : ID(x), color(c), item(p), name(s), imgPath(path) {}

  void print() { qDebug() << QString("Role %1: %2").arg(ID).arg(name); }
  void setItem(Role *p) { item = p; }
  void setImage(const QString &path) { imgPath = path; }
  //  int getImpact() {
  //    auto ver = item->view->canvas->hashID[ID];
  //    auto Degree = [](decltype(ver->_Adj[0].to) x) { return x; };
  //    for (auto &e : ver->_Adj) {
  //      impact += e._data.cohesion * net.totalDegree(e.to);
  //    }
  //    for (auto &e : ver->_rAdj) {
  //      impact += e._data.cohesion * net.totalDegree(e.from);
  //    }
  //  }
};

struct RelData {
  QString label;
  int color;
  Rel *item;
  int cohesion; //亲密度

public:
  RelData() = default;
  RelData(const QString &s, int c = 1, Rel *p = nullptr, int coh = 60)
      : label(s), color(c), item(p), cohesion(coh) {}

  void print() { qDebug() << QString("Label: %1").arg(label); }
};

#endif // NODE_H
