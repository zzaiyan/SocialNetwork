#include "canvaswidget.h"
#include "littlefamily.h"
#include "significancer.h"
#include "ui_canvaswidget.h"
#include <QDebug>
#include <QFile>
#include <cmath>

CanvasWidget::CanvasWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::CanvasWidget) {
  ui->setupUi(this);

  //  ui->addRole->setIconSize(QSize(25, 25));
  //  ui->addRole->setIcon(QIcon(":/icon/role2.svg"));
  //  ui->deleteItem->setIconSize(QSize(20, 20));
  //  ui->deleteItem->setIcon(QIcon(":/icon/delete.svg"));

  scene = new QGraphicsScene;
  // scene->setSceneRect(-5000, -5000, 10000, 10000);
  view = new MyGraphicsView(this); // 绑定this与view

  view->setScene(scene);
  view->setMinimumWidth(400);
  ui->horizontalLayout_6->addWidget(view);
  //  ui->comboBox->setView(new QListView());
  //  ui->tabWidget->setTabText(0, "查询人物");
  //  ui->tabWidget->setTabText(1, "查询关系");
  // ui->tabWidget->setFont(QFont("微软雅黑",12));
  //  ui->imgLabel->setScaledContents(true);
  //  ui->openFile->setEnabled(false);
  connect(scene, SIGNAL(selectionChanged()), this, SLOT(repaint()));

  system("python C:\\Users\\1\\Desktop\\第二题\\PycharmProject\\network.py");
}

CanvasWidget::~CanvasWidget() {
  delete ui;
  delete scene;
  delete view;
}

void CanvasWidget::repaint() {
  if (!scene->selectedItems().isEmpty()) {
    selectedItem = scene->selectedItems().front();
    if (selectedItem->type() == Role::Type) { // 当前选中的是role
      selectedRel = nullptr;
      selectedRole = qgraphicsitem_cast<Role *>(selectedItem);
      auto ver = hashName[selectedRole->name];
      QString name = selectedRole->name;
      //      QPixmap p(imgPath.isEmpty() ? ":/icon/role.svg" : imgPath);
      //      ui->imgLabel->setPixmap(p);
      ui->nameEdit->setText(name);
      ui->idLabel->setText("ID:" + QString::number(selectedRole->ID));
      ui->infoEdit->clear();
      ui->infoEdit->setText(QString("影响力：%1").arg(ver->_data.impact));
      ui->nameEdit->setReadOnly(false);
      ui->infoEdit->setReadOnly(false);
      //      ui->openFile->setEnabled(true);
    } else { // 当前选中的是rel
      selectedRole = nullptr;
      selectedRel = qgraphicsitem_cast<Rel *>(selectedItem->parentItem());
      QPixmap p(":/icon/relation.svg");
      //      ui->imgLabel->setPixmap(p);
      ui->nameEdit->setText(selectedRel->text);
      ui->idLabel->setText("ID:00");
      ui->infoEdit->clear();
      QString buf;
      buf =
          selectedRel->startRole()->name + "->" + selectedRel->endRole()->name;
      ui->infoEdit->setText(buf);
      ui->nameEdit->setReadOnly(false);
      ui->infoEdit->setReadOnly(true);
      //      ui->openFile->setEnabled(false);
    }
  } else { // 选中的是别的地方
    selectedRole = nullptr;
    selectedRel = nullptr;
    QPixmap p;
    //    ui->imgLabel->setPixmap(p);
    ui->nameEdit->setText("Null");
    ui->infoEdit->clear();
    ui->infoEdit->setText("No description");
    ui->idLabel->setText("ID:00");
    ui->nameEdit->setReadOnly(true);
    ui->infoEdit->setReadOnly(true);
    //    ui->openFile->setEnabled(false);
  }
}
// 更新数值
void CanvasWidget::updateValue() {
  int roleCnt = net.getVerNum();
  int relCnt = net.getArcNum();

  ui->countLabel->setText(
      QString("Nodes：%1，Edges：%2").arg(roleCnt).arg(relCnt));
  getImpact();
}

void CanvasWidget::on_addRole_clicked() {
  // 新建roleItem，分配ID
  auto *newRole = new Role(++roleID, view);

  auto viewSize = view->size();
  // 每次添加人物的位置都保证在视图的中心点处
  int delta = scene->items().size() & 1;
  newRole->setPos(view->mapToScene(viewSize.width() / 2 + delta,
                                   viewSize.height() / 2 + delta));

  // net中添加结点
  auto ver = net.addVer({newRole->ID, newRole->name, newRole});
  // 哈希表记录
  hashID.insert(newRole->ID, ver);
  hashName.insert(newRole->name, ver);
  // 加入场景
  scene->addItem(newRole);
  updateValue();
}

void CanvasWidget::on_deleteItem_clicked() {
  if (selectedRole != nullptr) {
    //    selectedItem = nullptr;
    qDebug() << "remove:" << selectedRole->ID << selectedRole->name;
    auto ver = hashID[selectedRole->ID];
    hashID.remove(selectedRole->ID);
    hashName.remove(selectedRole->name);
    selectedRole->removeThis();
    net.rmVer(ver);
  }
  if (selectedRel != nullptr) {
    auto start = selectedRel->startRole();
    auto end = selectedRel->endRole();
    auto ver1 = hashID[start->ID];
    auto ver2 = hashID[end->ID];
    net.rmArc(ver1, ver2);
    selectedRel->removeThis();
  }
  updateValue();
}

void CanvasWidget::on_nameEdit_editingFinished() {
  if (selectedRole != nullptr) {
    auto oldName = selectedRole->name;
    auto newName = ui->nameEdit->text();
    // 获取顶点
    auto ver = hashName[oldName];
    // 更新RoleData
    ver->_data.name = newName;
    // 更新哈希表
    hashName.remove(oldName);
    hashName.insert(newName, ver);
    // 更新Item
    selectedRole->setName(newName);
  }
  if (selectedRel != nullptr) {
    auto text = ui->nameEdit->text();
    // 更新relData
    this->setRelData(selectedRel->startRole()->ID, selectedRel->endRole()->ID,
                     {text});
    // 更新Item
    selectedRel->setText(text);
  }
}

// void CanvasWidget::on_openFile_clicked() {
//   QImage image;
//   QString OpenFile;
//   // 打开文件夹中的图片文件
//   OpenFile = QFileDialog::getOpenFileName(
//       this, "Please choose an image file", "",
//       "Image Files(*.jpg *.png *.bmp *.pgm *.pbm);;All(*.*)");
//   if (OpenFile != "" && selectedRole != nullptr) {
//     if (image.load(OpenFile)) {
//       //      ui->imgLabel->setPixmap(QPixmap::fromImage(image));
//       selectedRole->setImgPath(OpenFile);
//       auto ver = hashName[selectedRole->name];
//       ver->_data.imgPath = OpenFile;
//     }
//   }
// }

void CanvasWidget::on_exportImg_clicked() {
  QString filePath =
      QFileDialog::getSaveFileName(this, "save", QString(), " *.png");
  if (!filePath.isEmpty()) {
    scene->update();
    // 导出区域为包含所有item的边界矩形
    QSize mysize(scene->itemsBoundingRect().width(),
                 scene->itemsBoundingRect().height());
    QPixmap pixmap(mysize);
    pixmap.fill(Qt::transparent); // 设置背景为透明
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    scene->render(&painter, QRectF(), scene->itemsBoundingRect()); // 关键函数

    pixmap.save(filePath, "PNG", 100);
  }
}

void CanvasWidget::shuffle() {
  auto viewSize = view->size();
  QPointF p(view->mapToScene(viewSize.width() / 2, viewSize.height() / 2));
  const QList<QGraphicsItem *> items = scene->items();
  for (QGraphicsItem *item : items) {
    if (qgraphicsitem_cast<Role *>(item))
      item->setPos(p.x() + QRandomGenerator::global()->bounded(100),
                   p.y() + QRandomGenerator::global()->bounded(100));
  }
}

void CanvasWidget::clear() {
  // 清空场景
  QList<QGraphicsItem *> all = scene->items();
  foreach (QGraphicsItem *item, all) {
    scene->removeItem(item);
  }
  // 清空数据结构
  net.clear();
  hashName.clear();
  hashID.clear();
  roleID &= 0;
  updateValue();
}

void CanvasWidget::readFile() {
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Excel file"), qApp->applicationDirPath(), tr("Files (*.csv)"));
  QFile inFile(fileName);

  if (!inFile.open(QIODevice::ReadOnly)) {
    qDebug() << "DATA_FILE read error!";
    exit(1);
  }
  QStringList list; // 读取每一行的内容
  QTextStream in(&inFile);
  //  in.setCodec("gbk");
  codec = FileCharacterEncoding(fileName);
  in.setCodec(codec.toLocal8Bit()); // 这行的目的是支持读取中文信息
  qDebug() << codec;
  int lineCnt = 0, verNum, arcNum;
  { // Code Scope : avoid name conflict
    QString fileLine = in.readLine();
    list = fileLine.split(","); // 根据","开分隔开每行的列
    auto it = list.begin();
    verNum = (*it++).toInt();
    roleID = verNum; // update
    arcNum = (*it++).toInt();
    auto x = (*it++).toDouble();
    auto y = (*it++).toDouble();
    scene->setSceneRect(-x / 2, -y / 2, x, y);
  }

  while (lineCnt < verNum) {
    QString fileLine = in.readLine();
    list = fileLine.split(","); // 根据","开分隔开每行的列
    auto it = list.begin();
    auto name = *it++;
    auto color = (*it++).toInt();
    auto pos_x = (*it++).toDouble();
    auto pos_y = (*it++).toDouble();
    //    auto path = *it++;
    //    if (path == "null")
    //      path = "";

    int ID = ++lineCnt; // ID: 1~n

    // 建立Role
    auto roleItem = new Role(ID, view, name);
    roleItem->setColor(color);

    // 加入场景
    scene->addItem(roleItem);
    // 设置坐标
    roleItem->setPos(pos_x, pos_y);
    //    qDebug() << pos_x << " " << pos_y;

    // 建立顶点，绑定Role
    auto ver = net.addVer({ID, name, roleItem, color});

    qDebug() << QString("%1 -> %2").arg(name).arg(ID);

    ver->_data.setItem(roleItem);

    // 哈希表记录
    hashID.insert(ID, ver);
    hashName.insert(name, ver);
  }

  lineCnt = 0; // reset counter

  while (lineCnt < arcNum && !in.atEnd()) {
    QString fileLine = in.readLine();
    list = fileLine.split(","); // 根据","开分隔开每行的列
    auto it = list.begin();

    auto ver1 = hashName[*it++];
    auto ver2 = hashName[*it++];
    auto label = *it++;
    auto color = (*it++).toInt();
    auto cohesion = (*it++).toDouble();

    qDebug() << QString("<%1, %2> = %3")
                    .arg(ver1->_data.name)
                    .arg(ver2->_data.name)
                    .arg(label);
    // 初始化标签，new Rel对象
    auto relItem = new Rel(ver1->_data.item, ver2->_data.item, label, color);
    scene->addItem(relItem);
    // 在net中生成边
    net.addArc(ver1, ver2, {label, color, relItem, cohesion});
  }
  //  shuffle();

  inFile.close();
  scene->update();
  updateValue();
  qDebug() << "文件读取完成!";
}

void CanvasWidget::writeFile() {
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("save file"), qApp->applicationDirPath(), tr("Files (*.csv)"));
  QFile file(fileName);
  QTextStream out(&file);
  if (!file.open(QIODevice::WriteOnly)) {
    qDebug() << "Write file error!";
    exit(1);
  }
  auto &vers = net.getVers();
  int verNum = vers.size(), arcNum = 0;

  for (int i = 0; i < verNum; i++) {
    arcNum += vers[i]->_Adj.size();
  }
  QString buf = QString("%1,%2,%3,%4")
                    .arg(verNum)
                    .arg(arcNum)
                    .arg(scene->itemsBoundingRect().width())
                    .arg(scene->itemsBoundingRect().height());
  out << buf << '\n';

  for (int i = 0; i < verNum; i++) { // for Vers
    auto &data = vers[i]->_data;
    //    QString imgPath = data.imgPath.isEmpty() ? "null" : data.imgPath;
    buf = QString("%1,%2,%3,%4")
              .arg(data.name)
              .arg(data.color)
              .arg(data.item->scenePos().x())
              .arg(data.item->scenePos().y());
    //              .arg(imgPath);

    out << buf << '\n';
  }

  for (int i = 0; i < verNum; i++) {
    auto li = vers[i]->_Adj;
    for (auto &&arc : li) {
      buf = QString("%1,%2,%3,%4,%5")
                .arg(arc.from->_data.name)
                .arg(arc.to->_data.name)
                .arg(arc._data.label)
                .arg(arc._data.color)
                .arg(arc._data.cohesion);
      out << buf << '\n';
    }
  }
  file.close();
}

void CanvasWidget::getImpact() {
  for (auto &ver : net.getVers()) {
    ver->_data.impact = 0;
    qDebug() << ver->_Adj.size() << ver->_rAdj.size();

    for (auto &e : ver->_Adj)
      ver->_data.impact += e._data.cohesion * net.totalDegree(e.to);

    for (auto &e : ver->_rAdj)
      ver->_data.impact += e._data.cohesion * net.totalDegree(e.to);

    ver->_data.impact += net.totalDegree(ver);
  }
}

void CanvasWidget::setRelData(int ID1, int ID2, const RelData &e) {
  auto ver1 = hashID[ID1];
  auto ver2 = hashID[ID2];
  net.getArc(ver1, ver2)->_data = e;
  net.getRArc(ver1, ver2)->_data = e;
}

RelData *CanvasWidget::addNetArc(const QString &name1, const QString &name2,
                                 const QString &label, Rel *item) {
  auto ver1 = hashName[name1];
  auto ver2 = hashName[name2];
  auto relData = net.addArc(ver1, ver2, {label, 1, item});
  return relData;
}

void CanvasWidget::on_debugButton_clicked() {
  net.printVers();
  net.printTable();
  writeFile();
}

void CanvasWidget::setColor(int c) {
  if (selectedRole != nullptr) {
    selectedRole->setColor(c);
    auto ver = hashID[selectedRole->ID];
    ver->_data.color = c;
  }
  if (selectedRel != nullptr) {
    selectedRel->setColor(c);
    auto ver1 = hashID[selectedRel->startRole()->ID];
    auto ver2 = hashID[selectedRel->endRole()->ID];
    net.getArc(ver1, ver2)->_data.color = c;
    net.getArc(ver1, ver2)->_data.color = c;
  }
}

void CanvasWidget::on_comboBox_currentIndexChanged(int index) {
  SearchModelChoice = index;
  return;
}

void CanvasWidget::queryRole(const QString &a) {
  //  QString queryText = ui->queryEdit->text();
  ALNet<RoleData, RelData>::VerNode *ver = nullptr;
  switch (SearchModelChoice) {
  case 0: // 按姓名查询
    if (hashName.contains(a))
      ver = hashName[a];
    break;
  case 1: // 按ID查询
    if (hashID.contains(a.toInt()))
      ver = hashID[a.toInt()];
    break;
  default:
    break;
  };
  if (ver != nullptr) {
    Role *role = ver->_data.item;
    view->centerOn(role);
    scene->clearSelection();
    role->setSelected(true);
  }
}

void CanvasWidget::queryRelation(const QString &a, const QString &b) {
  //  QString text1 = ui->role1Edit->text(), b = ui->role2Edit->text();
  ALNet<RoleData, RelData>::VerNode *ver1 = nullptr, *ver2 = nullptr;
  //  ALNet<RoleData, RelData>::ArcNode *arc;
  switch (SearchModelChoice) {
  case 0: // 按姓名查询
    if (hashName.contains(a))
      ver1 = hashName[a];
    if (hashName.contains(b))
      ver2 = hashName[b];
    break;
  case 1: // 按ID查询
    if (hashID.contains(a.toInt()))
      ver1 = hashID[a.toInt()];
    if (hashID.contains(b.toInt()))
      ver2 = hashID[a.toInt()];
    break;
  default:
    break;
  };
  if (ver1 != nullptr && ver2 != nullptr) {
    auto arc = net.getArc(ver1, ver2);
    if (arc == nullptr)
      arc = net.getArc(ver2, ver1);
    if (arc != nullptr) {
      auto tag = arc->_data.item->relTag;
      view->centerOn(tag);
      scene->clearSelection();
      tag->setSelected(true);
    }
  }
}

void CanvasWidget::on_checkBox_stateChanged(int arg1) {
  if (arg1 == 0) {
    view->mode = 0;
  } else if (arg1 == 2) {
    view->mode = 1;
  }
}
void CanvasWidget::updateZoomText() {
  ui->zoomLabel->setText(QString("缩放比例：%1%").arg(int(view->zoom * 100)));
}
QString CanvasWidget::FileCharacterEncoding(const QString &fileName) {
  // 假定默认编码utf8
  QString code = "UTF-8";

  QFile file(fileName);
  if (file.open(QIODevice::ReadOnly)) {
    // 读取30字节用于判断
    QByteArray buffer = file.read(30);
    // 尝试用utf8转换,如果无效字符数大于0,则表示是ansi编码
    QTextCodec::ConverterState cs;
    QTextCodec *tc = QTextCodec::codecForName("UTF-8");
    tc->toUnicode(buffer.constData(), buffer.size(), &cs);
    code = (cs.invalidChars > 0) ? "ANSI" : "UTF-8";

    file.close();
  }

  return code;
}

void CanvasWidget::on_resetCanvas_clicked() { clear(); }

void CanvasWidget::on_pushButton_clicked() {
  setColor(ui->comboBox_2->currentIndex() + 1);
}

void CanvasWidget::on_readButton_clicked() {
  clear();
  readFile();
}

void CanvasWidget::on_queryBtn_clicked() {
  auto query = new QueryWidget(this);

  query->setAttribute(Qt::WA_DeleteOnClose, true);
  query->setWindowModality(Qt::ApplicationModal);
  query->show();
}

void CanvasWidget::checkQuery() {
  if (queryParaCnt == 1) {
    queryRole(key1);
  }
  if (queryParaCnt == 2) {
    queryRelation(key1, key2);
  }
}

void CanvasWidget::on_significance_clicked() {
  if (net.getVerNum() == 0)
    return;

  auto &vers = net.getVers();

  auto sig = new Significancer(this, &vers);

  sig->setAttribute(Qt::WA_DeleteOnClose, true);
  sig->setWindowModality(Qt::ApplicationModal);
  sig->show();
}

void CanvasWidget::on_littleFamily_clicked() {
  if (net.getVerNum() == 0)
    return;

  //  QDesktopServices::openUrl(QUrl::fromLocalFile("../SocialNetwork/python.bat"));

  auto &vers = net.getVers();

  auto family = new LittleFamily(this, &vers);

  family->setAttribute(Qt::WA_DeleteOnClose, true);
  family->setWindowModality(Qt::ApplicationModal);
  family->show();
}

void CanvasWidget::on_littleFamily_2_clicked() {
  if (net.getVerNum() == 0)
    return;

  QString fileName = "../SocialNetwork/in.csv";
  QFile file(fileName);
  QTextStream out(&file);
  if (!file.open(QIODevice::WriteOnly)) {
    qDebug() << "Write file error!";
    exit(1);
  }
  auto &vers = net.getVers();
  int verNum = net.getVerNum();

  for (int i = 0; i < verNum; i++) {
    auto li = vers[i]->_Adj;
    for (auto &&arc : li) {
      auto buf = QString("%1,%2,%3")
                     .arg(arc.from->_data.name)
                     .arg(arc.to->_data.name)
                     .arg(1);
      out << buf << '\n';
    }
  }
  file.close();

  QDesktopServices::openUrl(QUrl::fromLocalFile("../SocialNetwork/python.bat"));
}
