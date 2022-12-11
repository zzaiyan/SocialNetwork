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

private:
  Ui::MyCanvas *ui;
  QGraphicsScene *scene;
  GraphView *view;
  Role *selectedItem;

  ALNet<RoleData, RelData> net; // 有向网
  QHash<QString, ALNet<RoleData, RelData>::VerNode *> hashs;

  void readFile();
};

#endif // MYCANVAS_H
