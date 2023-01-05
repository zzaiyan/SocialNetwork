#ifndef NET_H
#define NET_H

#include <QDebug>
#include <QString>

#include <list>
#include <queue>
#include <vector>

using std::list;
using std::queue;
using std::vector;

template <class VerData, class ArcData> class ALNet {
public:
  struct ArcNode;
  struct VerNode { // 顶点
    int _pos;
    VerData _data;
    list<ArcNode> _Adj, _rAdj; // 出度表 + 入度表

    VerNode(int i, const VerData &e = {}) : _pos(i), _data(e) {}
  };
  struct ArcNode { // 有向边
    VerNode *from = nullptr;
    VerNode *to = nullptr;
    ArcData _data;
  };

private:
  vector<VerNode *> vers; // 顶点数组

  int fromNum(const ArcNode &a) const {
    return a.from->_pos; // 获取起点下标
  }
  int toNum(const ArcNode &a) const {
    return a.to->_pos; // 获取终点下标
  }

public:
  // 这两个其实是语法糖
  // 获取邻接表
  auto &Adj(int id) const { return vers.at(id)->_Adj; }
  // 获取逆邻接表
  auto &rAdj(int id) const { return vers.at(id)->_rAdj; }
  // 获取邻接表
  auto &Adj(VerNode *p) const { return p->_Adj; }
  // 获取逆邻接表
  auto &rAdj(VerNode *p) const { return p->_rAdj; }

  auto &getVers() { return vers; } // 获取顶点和邻接表，用于遍历

public:
  ALNet(const int s = 0) {
    for (int i = 0; i < s; i++) // initialize
      vers.push_back(new VerNode{i});
  }
  void clear() {
    for (auto &&p : vers)
      delete p;
    vers.clear();
  }
  // 获取顶点序偶对应的边指针
  ArcNode *getArc(VerNode *a, VerNode *b) {
    ArcNode *arc = nullptr;
    for (auto it = Adj(a).begin(); it != Adj(a).end(); it++)
      if (it->to->_pos == b->_pos) {
        arc = &(*it);
        break;
      }
    return arc;
  }

  ArcNode *getRArc(VerNode *a, VerNode *b) {
    ArcNode *arc = nullptr;
    for (auto it = rAdj(b).begin(); it != rAdj(b).end(); it++)
      if (it->to->_pos == a->_pos) {
        arc = &(*it);
        break;
      }
    return arc;
  }

  int getVerNum() const { return vers.size(); }
  int getArcNum() const {
    int arcNum = 0;
    for (auto ver : vers)
      arcNum += ver->_Adj.size();
    return arcNum;
  }

  int _inDegree(int id) const { return rAdj(id).size(); }
  int inDegree(VerNode *ver) const { return ver->_rAdj.size(); }

  int _outDegree(int id) const { return Adj(id).size(); }
  int outDegree(VerNode *ver) const { return ver->_Adj.size(); }

  int _totalDegree(int id) const { return inDegree(id) + outDegree(id); }
  int totalDegree(VerNode *ver) const { return inDegree(ver) + outDegree(ver); }

  // 尾插顶点
  VerNode *addVer(const VerData &e = {}) {
    auto ver = new VerNode{vers.size(), e};
    vers.push_back(ver);
    return ver;
  }
  // 加边
  ArcData *addArc(VerNode *a, VerNode *b, const ArcData &e = {}) {
    Adj(a).push_back({a, b, e});
    rAdj(b).push_back({b, a, e});
    return &Adj(a).back()._data;
  }
  // 判断边
  bool haveArc(VerNode *a, VerNode *b) { return getArc(a, b) != nullptr; }

  bool rmArc(VerNode *a, VerNode *b) {
    auto it1 = a->_Adj.begin();
    auto it2 = b->_rAdj.begin();

    for (; it1 != a->_Adj.end(); it1++)
      if (it1->to->_pos == b->_pos)
        break;

    for (; it2 != b->_rAdj.end(); it2++)
      if (it2->to->_pos == a->_pos)
        break;

    a->_Adj.erase(it1);
    b->_rAdj.erase(it2);
    return true;
  }

  void rmVer(VerNode *delVer) {
    for (auto &e : delVer->_Adj)
      rmArc(e.from, e.to);

    for (auto &e : delVer->_rAdj)
      rmArc(e.to, e.from);

    int i = delVer->_pos + 1;
    for (; i < vers.size(); i++) {
      vers[i]->_pos--;
      vers[i - 1] = vers[i];
    }
    delete delVer;
    vers.pop_back();
  }

  // 以下函数依照项目特化

  void printVers() {
    for (int i = 0; i < vers.size(); i++) {
      vers[i]->_data.print();
    }
  }

  QString getList(const list<ArcNode> &li) {
    QString buf;
    for (auto it = li.begin(); it != li.end(); it++) {
      if (it != li.begin())
        buf += " -> ";
      buf += it->_data.label;
    }
    return buf;
  }

  void printTable() {
    qDebug() << "Out Table:";
    for (int i = 0; i < vers.size(); i++) {
      QString buf;
      buf += (vers[i]->_data.name + ": ");
      auto line = Adj(i);
      buf += getList(line);
      qDebug() << buf;
    }
    qDebug() << ""
             << "In Table:";
    for (int i = 0; i < vers.size(); i++) {
      QString buf;
      buf += (vers[i]->_data.name + ": ");
      auto line = rAdj(i);
      buf += getList(line);
      qDebug() << buf;
    }
  }
};

#endif // NET_H
