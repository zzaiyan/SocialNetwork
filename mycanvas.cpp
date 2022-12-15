#include "mycanvas.h"
#include "ui_mycanvas.h"
#include <QDebug>
// 再筵的
#define ROLE_FILE "C:\\Users\\1\\Desktop\\SocialNetworkAnalist\\data\\id0.csv"
#define REL_FILE "C:\\Users\\1\\Desktop\\SocialNetworkAnalist\\data\\data0.csv"
// 琪琪的
// #define ROLE_FILE "D:\\QTproject\\SocialNetworkAnalist\\data\\id0.csv"
// #define REL_FILE "D:\\QTproject\\SocialNetworkAnalist\\data\\data0.csv"

MyCanvas::MyCanvas(QWidget *parent) : QWidget(parent), ui(new Ui::MyCanvas) {
  ui->setupUi(this);

  ui->addRole->setIconSize(QSize(25, 25));
  ui->addRole->setIcon(QIcon(":/icon/role2.svg"));
  ui->deleteItem->setIconSize(QSize(20, 20));
  ui->deleteItem->setIcon(QIcon(":/icon/delete.svg"));

  scene = new QGraphicsScene;
  view = new GraphView(this); // 绑定this与view

  view->setScene(scene);
  view->setMinimumWidth(400);
  ui->horizontalLayout_6->addWidget(view);
  ui->imgLabel->setScaledContents(true);
  ui->openFile->setEnabled(false);
  // advance();
  connect(scene, SIGNAL(selectionChanged()), this, SLOT(repaint()));

  readFile();
}

MyCanvas::~MyCanvas() { delete ui; }

void MyCanvas::repaint() {
  if (!scene->selectedItems().isEmpty()) {
    selectedItem = scene->selectedItems().front();
    if (selectedItem->type() == Role::Type) { // 当前选中的是role
      selectedRel = nullptr;
      selectedRole = qgraphicsitem_cast<Role *>(selectedItem);
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
    } else { // 当前选中的是rel
      selectedRole = nullptr;
      selectedRel = qgraphicsitem_cast<Rel *>(selectedItem->parentItem());
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
  } else { // 选中的是别的地方
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
  auto *newRole = new Role(roleCnt++);

  auto viewSize = view->size();
  // 每次添加人物的位置都保证在视图的中心点处
  newRole->setPos(
      view->mapToScene(viewSize.width() / 2, viewSize.height() / 2));

  // net中添加结点
  auto ver = net.addVer({newRole->ID, newRole->name});
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
    auto ver1 = hashID[selectedRel->startRole()->ID];
    auto ver2 = hashID[selectedRel->endRole()->ID];
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
  QSize mysize(scene->width() * 2,
               scene->height() * 2); // 获取 QGraphicsScene.size
  QImage image(mysize, QImage::Format_ARGB32_Premultiplied); // 保存透明度
  QPainter painter(&image);
  painter.setRenderHint(QPainter::HighQualityAntialiasing);
  scene->render(&painter); // 关键函数
  image.save("D:\\SaveGraphicsScene.png");
}

void MyCanvas::readFile() {
  hashName.clear();
  //  names.clear();
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
    auto roleItem = new Role(ID, name);
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

  ifs.close();

  qDebug() << "文件读取完成!";
}

void MyCanvas::writeFile() {
  std::ofstream ofs;
  ofs.open(ROLE_FILE, std::ios::out);
}

void MyCanvas::setRelData(int ID1, int ID2, const RelData &e) {
  auto ver1 = hashID[ID1];
  auto ver2 = hashID[ID2];
  net.getArc(ver1, ver2)->_data = e;
  net.getRArc(ver1, ver2)->_data = e;
}

RelData *MyCanvas::addNetArc(const QString &name1, const QString &name2,
                             const QString &label) {
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
void MyCanvas::on_blueBtn_clicked() { setColor(1); }

void MyCanvas::on_redBtn_clicked() { setColor(2); }

void MyCanvas::on_purpleBtn_clicked() { setColor(3); }

void MyCanvas::on_yellowBtn_clicked() { setColor(4); }
