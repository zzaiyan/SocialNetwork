#ifndef MYCANVAS_H
#define MYCANVAS_H

#include "graphview.h"
#include "net.h"
#include "node.h"
#include "rel.h"
#include "role.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QHash>
#include <QImage>
#include <QWidget>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

namespace Ui {
class MyCanvas;
}

class GraphView;

class MyCanvas : public QWidget {
  Q_OBJECT

public:
  explicit MyCanvas(QWidget *parent = nullptr);
  ~MyCanvas();

private slots:

  void repaint();

  void on_addRole_clicked();

  void on_deleteRole_clicked();

  void on_blueBtn_clicked();

  void on_redBtn_clicked();

  void on_purpleBtn_clicked();

  void on_yellowBtn_clicked();

  void on_nameEdit_editingFinished();

  void on_openFile_clicked();

  void on_exportImg_clicked();

  void on_debugButton_clicked();

public: // from Zzaiyan
  RelData *getRelData(const QString &name1, const QString &name2);
  RelData *addNetArc(const QString &name1, const QString &name2,
                     const QString &label);

private:
  Ui::MyCanvas *ui;
  QGraphicsScene *scene;
  GraphView *view;
  Role *selectedItem = nullptr;

  int roleCnt = 0;

  ALNet<RoleData, RelData> net; // 有向网
  QHash<QString, ALNet<RoleData, RelData>::VerNode *> hashName;
  QHash<int, ALNet<RoleData, RelData>::VerNode *> hashID;

  void readFile();
};

#endif // MYCANVAS_H
