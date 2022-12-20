#include "mycanvas.h"
#include <QDebug>
#include "ui_mycanvas.h"

#define ROLE_FILE "../SocialNetworkAnalist/data/id.csv"
#define REL_FILE "../SocialNetworkAnalist/data/data.csv"

MyCanvas::MyCanvas(QWidget* parent) : QWidget(parent), ui(new Ui::MyCanvas) {
  ui->setupUi(this);

  ui->addRole->setIconSize(QSize(25, 25));
  ui->addRole->setIcon(QIcon(":/icon/role2.svg"));
  ui->deleteItem->setIconSize(QSize(20, 20));
  ui->deleteItem->setIcon(QIcon(":/icon/delete.svg"));

  scene = new QGraphicsScene;
  // scene->setSceneRect(-5000, -5000, 10000, 10000);
  view = new GraphView(this);  // 绑定this与view

  view->setScene(scene);
  view->setMinimumWidth(400);
  ui->horizontalLayout_6->addWidget(view);
  ui->imgLabel->setScaledContents(true);
  ui->openFile->setEnabled(false);
  connect(scene, SIGNAL(selectionChanged()), this, SLOT(repaint()));
}

MyCanvas::~MyCanvas() {
  delete ui;
}

void MyCanvas::repaint() {
  if (!scene->selectedItems().isEmpty()) {
    selectedItem = scene->selectedItems().front();
    if (selectedItem->type() == Role::Type) {  // 当前选中的是role
      selectedRel = nullptr;
      selectedRole = qgraphicsitem_cast<Role*>(selectedItem);
      QString imgPath = selectedRole->imgPath, name = selectedRole->name;
      QPixmap p(imgPath.isEmpty() ? ":/icon/role.svg" : imgPath);
      ui->imgLabel->setPixmap(p);
      ui->nameEdit->setText(name);
      ui->idLabel->setText("ID:" + QString::number(selectedRole->ID));
      ui->infoEdit->clear();
      ui->infoEdit->appendPlainText("No description");
      ui->nameEdit->setReadOnly(false);
      ui->infoEdit->setReadOnly(false);
      ui->openFile->setEnabled(true);
    } else {  // 当前选中的是rel
      selectedRole = nullptr;
      selectedRel = qgraphicsitem_cast<Rel*>(selectedItem->parentItem());
      QPixmap p(":/icon/relation.svg");
      ui->imgLabel->setPixmap(p);
      ui->nameEdit->setText(selectedRel->text);
      ui->idLabel->setText("ID:00");
      ui->infoEdit->clear();
      QString buf;
      buf =
          selectedRel->startRole()->name + "->" + selectedRel->endRole()->name;
      ui->infoEdit->appendPlainText(buf);
      ui->nameEdit->setReadOnly(false);
      ui->infoEdit->setReadOnly(true);
      ui->openFile->setEnabled(false);
    }
  } else {  // 选中的是别的地方
    selectedRole = nullptr;
    selectedRel = nullptr;
    QPixmap p;
    ui->imgLabel->setPixmap(p);
    ui->nameEdit->setText("Null");
    ui->infoEdit->clear();
    ui->infoEdit->appendPlainText("No description");
    ui->idLabel->setText("ID:00");
    ui->nameEdit->setReadOnly(true);
    ui->infoEdit->setReadOnly(true);
    ui->openFile->setEnabled(false);
  }
}

void MyCanvas::on_addRole_clicked() {
  // 新建roleItem，分配ID
  auto* newRole = new Role(roleCnt++, view);

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
}

void MyCanvas::on_deleteItem_clicked() {
  if (selectedRole != nullptr) {
    //    selectedItem = nullptr;
    qDebug() << "remove:" << selectedRole->ID << selectedRole->name;
    auto ver = hashID[selectedRole->ID];
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
}

void MyCanvas::on_nameEdit_editingFinished() {
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

void MyCanvas::on_openFile_clicked() {
  QImage image;
  QString OpenFile;
  // 打开文件夹中的图片文件
  OpenFile = QFileDialog::getOpenFileName(
      this, "Please choose an image file", "",
      "Image Files(*.jpg *.png *.bmp *.pgm *.pbm);;All(*.*)");
  if (OpenFile != "" && selectedRole != nullptr) {
    if (image.load(OpenFile)) {
      ui->imgLabel->setPixmap(QPixmap::fromImage(image));
      selectedRole->setImgPath(OpenFile);
    }
  }
}

void MyCanvas::on_exportImg_clicked() {
  QString filePath =
      QFileDialog::getSaveFileName(this, "save", QString(), " *.png");
  if (!filePath.isEmpty()) {
    scene->update();
    QSize mysize(scene->width(), scene->height());  // 获取 QGraphicsScene.size
    QPixmap pixmap(mysize);
    pixmap.fill(Qt::transparent);  // 设置背景为透明
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    scene->render(&painter);  // 关键函数
    pixmap.save(filePath, "PNG", 100);
  }
}

void MyCanvas::shuffle() {
  auto viewSize = view->size();
  QPointF p(view->mapToScene(viewSize.width() / 2, viewSize.height() / 2));
  const QList<QGraphicsItem*> items = scene->items();
  for (QGraphicsItem* item : items) {
    if (qgraphicsitem_cast<Role*>(item))
      item->setPos(p.x() + QRandomGenerator::global()->bounded(100),
                   p.y() + QRandomGenerator::global()->bounded(100));
  }
}

void MyCanvas::clear() {
  // 清空场景
  QList<QGraphicsItem*> all = scene->items();
  foreach (QGraphicsItem* item, all) {
    scene->removeItem(item);
  }
  //清空数据结构
  net.clear();
  hashName.clear();
  hashID.clear();
  roleCnt &= 0;
}

void MyCanvas::readFile() {
  std::ifstream ifs;

  ifs.open(ROLE_FILE, std::ios::in);
  if (!ifs) {
    qDebug() << "ID_FILE open error!";
    exit(1);
  }
  std::string lineBuf;
  while (std::getline(ifs, lineBuf)) {
    auto qBuf = QString::fromLocal8Bit(lineBuf.data());
    auto li = qBuf.split(',');
    auto it = li.begin();
    auto name = *it++;
    auto color = it->toInt() % 4 + 1;
    auto ID = roleCnt++;
    // 建立Role
    auto roleItem = new Role(ID, view, name);
    roleItem->setColor(color);
    // 加入场景
    scene->addItem(roleItem);
    // 建立顶点，绑定Role
    auto ver = net.addVer({ID, name});
    ver->_data.color = color;

    qDebug() << QString("%1 -> %2").arg(name).arg(ID);

    ver->_data.setItem(roleItem);

    // 哈希表记录
    hashID.insert(ID, ver);
    hashName.insert(name, ver);
  }
  ifs.close();

  ifs.open(REL_FILE, std::ios::in);
  if (!ifs) {
    qDebug() << "REL_FILE open error!";
    exit(1);
  }
  while (getline(ifs, lineBuf)) {
    auto qBuf = QString::fromLocal8Bit(lineBuf.data());
    auto li = qBuf.split(',');
    auto it = li.begin();
    auto pa = hashName[*it++];
    auto pb = hashName[*it++];
    qBuf = *it;
    qDebug() << QString("<%1, %2> = %3")
                    .arg(pa->_data.name)
                    .arg(pb->_data.name)
                    .arg(qBuf);
    // 初始化标签，new Rel对象
    auto relItem = new Rel(pa->_data.item, pb->_data.item, 1, qBuf);
    scene->addItem(relItem);
    // 在net中生成边
    net.addArc(pa, pb, {qBuf});
  }
  shuffle();
  ifs.close();

  qDebug() << "文件读取完成!";
}

void MyCanvas::writeFile() {
  std::ofstream ofs;
  ofs.open(ROLE_FILE, std::ios::out);

  auto& vers = net.getVers();
  QString buf;
  for (int i = 0; i < vers.size(); i++) {
  }
}

void MyCanvas::setRelData(int ID1, int ID2, const RelData& e) {
  auto ver1 = hashID[ID1];
  auto ver2 = hashID[ID2];
  net.getArc(ver1, ver2)->_data = e;
  net.getRArc(ver1, ver2)->_data = e;
}

RelData* MyCanvas::addNetArc(const QString& name1,
                             const QString& name2,
                             const QString& label) {
  auto ver1 = hashName[name1];
  auto ver2 = hashName[name2];
  auto relData = net.addArc(ver1, ver2, {label});
  return relData;
}

void MyCanvas::on_debugButton_clicked() {
  net.printVers();
  net.printTable();
}
void MyCanvas::setColor(int c) {
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
void MyCanvas::on_blueBtn_clicked() {
  setColor(1);
}

void MyCanvas::on_redBtn_clicked() {
  setColor(2);
}

void MyCanvas::on_purpleBtn_clicked() {
  setColor(3);
}

void MyCanvas::on_yellowBtn_clicked() {
  setColor(4);
}

void MyCanvas::on_comboBox_currentIndexChanged(int index) {
  SearchModelChoice = index;
  return;
}

void MyCanvas::on_queryButton_clicked() {
  QString queryText = ui->queryEdit->text();
  ALNet<RoleData, RelData>::VerNode* ver = nullptr;
  switch (SearchModelChoice) {
    case 0:  //按ID查询
      if (hashID.contains(queryText.toInt()))
        ver = hashID[queryText.toInt()];
      break;
    case 1:  //按姓名查询
      if (hashName.contains(queryText))
        ver = hashName[queryText];
      break;
    default:
      break;
  };
  if (ver != nullptr) {
    Role* role = ver->_data.item;
    view->centerOn(role);
    scene->clearSelection();
    role->setSelected(true);
  }
}

void MyCanvas::on_checkBox_stateChanged(int arg1) {
  if (arg1 == 0) {
    view->mode = 0;
  } else if (arg1 == 2) {
    view->mode = 1;
  }
}
