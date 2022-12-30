#ifndef MYCANVAS_H
#define MYCANVAS_H

#include "mygraphicsview.h"
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
#include <QTextCodec>
#include <QWidget>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

namespace Ui {
class MyCanvas;
}

class MyGraphicsView;
struct RoleData;
struct RelData;

class MyCanvas : public QWidget {
  Q_OBJECT

public:
  explicit MyCanvas(QWidget *parent = nullptr);
  ~MyCanvas();

private slots:

  void repaint();

  void on_addRole_clicked();

  void on_deleteItem_clicked();

  void on_nameEdit_editingFinished();

  void on_openFile_clicked();

  void on_exportImg_clicked();

  void on_debugButton_clicked();

  void on_queryButton1_clicked();

  void on_comboBox_currentIndexChanged(int index);

  void on_checkBox_stateChanged(int arg1);

  void on_queryButton2_clicked();

  void on_resetCanvas_clicked();

  void on_pushButton_clicked();

public: // from Zzaiyan
  void setRelData(int ID1, int ID2, const RelData &e);
  RelData *addNetArc(const QString &name1, const QString &name2,
                     const QString &label, Rel *item);
  void setColor(int c);
  void shuffle();
  void clear();

  ALNet<RoleData, RelData> net; // 有向网
  QHash<QString, ALNet<RoleData, RelData>::VerNode *> hashName;
  QHash<int, ALNet<RoleData, RelData>::VerNode *> hashID;

  void readFile();
  void writeFile();
  void updateZoomText();
  void updateValue();

private:
  Ui::MyCanvas *ui;
  QGraphicsScene *scene;
  MyGraphicsView *view;

  QGraphicsItem *selectedItem = nullptr;
  Role *selectedRole = nullptr;
  Rel *selectedRel = nullptr;

  int SearchModelChoice = 0;
  int roleCnt = 0;
  int relCnt = 0;
  void getImpact();
  // 自动识别文件编码
  QString FileCharacterEncoding(const QString &fileName);
  QString codec;
};

#endif // MYCANVAS_H
