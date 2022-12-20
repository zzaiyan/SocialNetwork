#ifndef NET_H
#define NET_H

#include <QDebug>
#include <QString>
#include <queue>
#include <stack>
#include <vector>

using std::list;
using std::queue;
using std::stack;
using std::vector;

template <class VerData, class ArcData> class ALNet {
public:
  struct ArcNode;
  struct VerNode { // 顶点
    int _pos;
    VerData _data;
    list<ArcNode> _Adj, _rAdj; // 出度表 + 入度表

    VerNode(int i = -1) : _pos(i) {}
    VerNode(int i, const VerData &e) : _pos(i), _data(e) {}
  };
  struct ArcNode { // 有向边
    VerNode *from = nullptr;
    VerNode *to = nullptr;
    ArcData _data;
  };

private:
  vector<VerNode *> vers; // 顶点数组

  int fromNum(const ArcNode &a) {
    return a.from->_pos; // 获取起点下标
  }
  int toNum(const ArcNode &a) {
    return a.to->_pos; // 获取终点下标
  }

public:
  list<ArcNode> &Adj(int id) {
    return vers[id]->_Adj; // 获取邻接表
  }
  list<ArcNode> &rAdj(int id) {
    return vers[id]->_rAdj; // 获取逆邻接表
  }
  const auto &getVers() { return vers; }

public:
  ALNet(const int s = 0) {
    for (int i = 0; i < s; i++) // initialize
      vers.push_back(new VerNode{i, {}});
  }
  void clear() {
    for (auto &&p : vers)
      delete p;
    vers.clear();
  }
  // 获取下标对应的顶点指针
  VerNode *getVer(int id) { return vers[id]; }
  // 获取顶点序偶对应的边指针
  ArcNode *getArc(int a, int b) {
    ArcNode *arc = nullptr;
    for (auto it = Adj(a).begin(); it != Adj(a).end(); it++)
      if (it->to->_pos == b) {
        arc = &(*it);
        break;
      }
    return arc;
  }
  ArcNode *getArc(VerNode *a, VerNode *b) {
    if (a && b)
      return getArc(a->_pos, b->_pos);
    qDebug() << "Arc Get ERROR!";
    return nullptr;
  }

  ArcNode *getRArc(int a, int b) {
    ArcNode *arc = nullptr;
    for (auto it = rAdj(b).begin(); it != rAdj(b).end(); it++)
      if (it->to->_pos == a) {
        arc = &(*it);
        break;
      }
    return arc;
  }
  ArcNode *getRArc(VerNode *a, VerNode *b) {
    if (a && b)
      return getRArc(a->_pos, b->_pos);
    qDebug() << "rArc Get ERROR!";
    return nullptr;
  }

  int getVerNum() const { return vers.size(); }

  int inDegree(int id) const { return rAdj(id).size(); }
  int inDegree(VerNode *ver) const { return inDegree(ver->_pos); }

  int outDegree(int id) const { return Adj(id).size(); }
  int outDegree(VerNode *ver) const { return outDegree(ver->_pos); }

  int totalDegree(VerNode *ver) const { return inDegree(ver) + outDegree(ver); }

  // 尾插顶点并赋值
  VerNode *addVer(const VerData &e) {
    auto ver = new VerNode{vers.size(), e};
    vers.push_back(ver);
    return ver;
  }
  // 尾插顶点
  VerNode *addVer() { return addVer({}); }
  // 加边
  ArcData *addArc(int a, int b, const ArcData &e) {
    Adj(a).push_back({getVer(a), getVer(b), e});
    rAdj(b).push_back({getVer(b), getVer(a), e});
    return &Adj(a).back()._data;
  }
  ArcData *addArc(VerNode *a, VerNode *b, const ArcData &e) {
    if (a && b)
      return addArc(a->_pos, b->_pos, e);
    qDebug() << "Arc Add ERROR!";
    return nullptr;
  }
  // 判断边
  bool haveArc(int a, int b) { return getArc(a, b) != nullptr; }
  bool haveArc(VerNode *a, VerNode *b) { return haveArc(a->_pos, b->_pos); }
  // 改变顶点的数据
  bool setVer(int id, const VerData &e) {
    auto ver = getVer(id);
    if (ver == nullptr)
      return false;
    ver->_data = e;
    return true;
  }
  // 改变边的数据
  bool setArc(int a, int b, const ArcData &e) {
    auto arc = getArc(a, b);
    //    auto str = arc->_data;
    if (arc == nullptr)
      return false;
    arc->_data = e;
    return true;
  }

  bool rmArc(int a, int b) {
    //    ArcData ret;
    auto it1 = Adj(a).begin();
    auto it2 = rAdj(b).begin();
    for (; it1 != Adj(a).end(); it1++)
      if (it1->to->_pos == b) {
        //        ret = it1->_data;
        break;
      }
    for (; it2 != rAdj(b).end(); it2++)
      if (it2->to->_pos == a)
        break;
    //    qDebug() << QString("delete <%1,%2>.")
    //                    .arg(vers[a]->_data.name)
    //                    .arg(vers[b]->_data.name);
    Adj(a).erase(it1);
    rAdj(b).erase(it2);
    return true;
  }
  bool rmArc(VerNode *a, VerNode *b) {
    auto arc = getArc(a, b);
    if (arc == nullptr)
      return false;
    return rmArc(a->_pos, b->_pos);
  }

  void rmVer(int id) {
    // 首先删除以该点为终点的边
    for (int i = 0; i < vers.size(); i++) {
      if (i == id)
        continue;
      queue<std::pair<int, int>> que;
      for (auto &e : Adj(i)) {
        int a = fromNum(e), b = toNum(e);
        if (b == id)
          que.push({a, b});
      }
      while (que.size()) {
        rmArc(que.front().first, que.front().second);
        que.pop();
      }
    }
    // 然后删除顶点以及邻接表（发出的边）
    delete vers[id];
    for (int i = id + 1; i < vers.size(); i++) {
      vers[i]->_pos--;
      vers[i - 1] = vers[i];
    }
    vers.pop_back();
    //    return ret;
  }
  void rmVer(VerNode *ver) {
    if (ver)
      rmVer(ver->_pos);
    else
      qDebug() << "Remove Ver ERROR!";
  }

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
