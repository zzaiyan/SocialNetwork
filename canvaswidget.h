#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include "mygraphicsview.h"
#include "net.h"
#include "node.h"
#include "querywidget.h"
#include "rel.h"
#include "role.h"
#include <QApplication>
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
class CanvasWidget;
}

class MyGraphicsView;
struct RoleData;
struct RelData;

class CanvasWidget : public QWidget {
  Q_OBJECT

public:
  explicit CanvasWidget(QWidget *parent = nullptr);
  ~CanvasWidget();

private slots:

  void repaint();

  void on_addRole_clicked();

  void on_deleteItem_clicked();

  void on_nameEdit_editingFinished();

  //  void on_openFile_clicked();

  void on_exportImg_clicked();

  void on_debugButton_clicked();

  //  void on_queryButton1_clicked();

  void on_comboBox_currentIndexChanged(int index);

  void on_checkBox_stateChanged(int arg1);

  //  void on_queryButton2_clicked();

  void on_resetCanvas_clicked();

  void on_pushButton_clicked();

  void on_readButton_clicked();

  void on_queryBtn_clicked();

  void on_significance_clicked();

  void on_littleFamily_clicked();

public: // from Zzaiyan
  void setRelData(int ID1, int ID2, const RelData &e);
  RelData *addNetArc(const QString &name1, const QString &name2,
                     const QString &label, Rel *item);
  void setColor(int c);
  void shuffle();
  void clear();

  void queryRelation(const QString &a, const QString &b);
  void queryRole(const QString &a);
  void checkQuery();

  ALNet<RoleData, RelData> net; // 有向网
  QHash<QString, ALNet<RoleData, RelData>::VerNode *> hashName;
  QHash<int, ALNet<RoleData, RelData>::VerNode *> hashID;

  QString key1, key2;
  int queryParaCnt = 0;
  int SearchModelChoice = 0;

  void readFile();
  void writeFile();
  void updateZoomText();
  void updateValue();

private:
  Ui::CanvasWidget *ui;
  QGraphicsScene *scene;
  MyGraphicsView *view;

  QGraphicsItem *selectedItem = nullptr;
  Role *selectedRole = nullptr;
  Rel *selectedRel = nullptr;

  //  int roleCnt = 0;
  //  int relCnt = 0;
  int roleID = 0; // 作为ID池
  void getImpact();
  // 自动识别文件编码
  QString FileCharacterEncoding(const QString &fileName);
  QString codec;
};

#endif // CANVASWIDGET_H
