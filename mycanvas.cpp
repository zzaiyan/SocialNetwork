#include "mycanvas.h"
#include "ui_mycanvas.h"
#include <QDebug>

#define ID_FILE "D:\\QTproject\\SocialNetworkAnalist-master\\data\\id0.csv"
#define REL_FILE "D:\\QTproject\\SocialNetworkAnalist-master\\data\\data0.csv"

MyCanvas::MyCanvas(QWidget *parent) : QWidget(parent), ui(new Ui::MyCanvas) {
  ui->setupUi(this);

  ui->addRole->setIconSize(QSize(25, 25));
  ui->addRole->setIcon(QIcon(":/icon/role2.svg"));
  ui->deleteRole->setIconSize(QSize(20, 20));
  ui->deleteRole->setIcon(QIcon(":/icon/delete.svg"));
  scene = new QGraphicsScene;
  view = new GraphView;
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
    selectedItem = dynamic_cast<Role *>(scene->selectedItems().front());
    QString imgPath = selectedItem->imgPath, name = selectedItem->nameText;
    QPixmap p(imgPath.isEmpty() ? ":/icon/role.svg" : imgPath);
    ui->imgLabel->setPixmap(p);
    ui->nameEdit->setText(name);
    ui->nameEdit->setReadOnly(false);
    ui->infoEdit->setReadOnly(false);
    ui->openFile->setEnabled(true);
  } else {
    selectedItem = nullptr;
    QPixmap p;
    ui->imgLabel->setPixmap(p);
    ui->nameEdit->setText("Null");
    ui->infoEdit->clear();
    ui->infoEdit->appendPlainText("No description");
    ui->nameEdit->setReadOnly(true);
    ui->infoEdit->setReadOnly(true);
    ui->openFile->setEnabled(false);
  }
}

void MyCanvas::on_addRole_clicked() {
  Role *role1 = new Role();
  QSize viewSize = view->size();
  // 每次添加人物的位置都保证在视图的中心点处
  role1->setPos(view->mapToScene(viewSize.width() / 2, viewSize.height() / 2));
  scene->addItem(role1);
}

void MyCanvas::on_deleteRole_clicked() {
  if (selectedItem != nullptr) {
    //    selectedItem = nullptr;
    auto ver = hashName[selectedItem->nameText];
    selectedItem->removeThis();
    net.rmVer(ver);
  }
}

void MyCanvas::on_blueBtn_clicked() {
  if (selectedItem != nullptr) {
    selectedItem->setColor(1);
  }
}

void MyCanvas::on_redBtn_clicked() {
  if (selectedItem != nullptr) {
    selectedItem->setColor(2);
  }
}

void MyCanvas::on_purpleBtn_clicked() {
  if (selectedItem != nullptr) {
    selectedItem->setColor(3);
  }
}

void MyCanvas::on_yellowBtn_clicked() {
  if (selectedItem != nullptr) {
    selectedItem->setColor(4);
  }
}

void MyCanvas::on_nameEdit_editingFinished() {
  if (selectedItem != nullptr) {
    auto oldName = selectedItem->nameText;
    auto newName = ui->nameEdit->text();
    auto ver = hashName[oldName];
    hashName.remove(oldName);
    hashName.insert(newName, ver);
    selectedItem->setName(newName);
  }
}

void MyCanvas::on_openFile_clicked() {
  QImage image;
  QString OpenFile;
  // 打开文件夹中的图片文件
  OpenFile = QFileDialog::getOpenFileName(
      this, "Please choose an image file", "",
      "Image Files(*.jpg *.png *.bmp *.pgm *.pbm);;All(*.*)");
  if (OpenFile != "" && selectedItem != nullptr) {
    if (image.load(OpenFile)) {
      ui->imgLabel->setPixmap(QPixmap::fromImage(image));
      selectedItem->setImgPath(OpenFile);
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

  ifs.open(ID_FILE, std::ios::in);
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
    auto ID = it->toInt();

    qDebug() << QString("%1 -> %2").arg(name).arg(ID);

    // 建立Role
    auto roleItem = new Role();
    roleItem->setName(name);

    scene->addItem(roleItem);

    // 建立顶点，绑定Role
    auto ver = net.addVer({ID, name});

    ver->_data.setItem(roleItem);

    // 哈希表记录
    hashName.insert(name, ver);
    hashID.insert(ID, ver);
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

    // 在net中生成边，同时绑定relItem
    auto relData = net.addArc(pa, pb, {qBuf});
    //    relData->setRel(relItem);
    //  Item绑定Data
    //    relItem->setData(relData);
  }

  ifs.close();
}

RelData *MyCanvas::getRelData(const QString &name1, const QString &name2) {
  auto ver1 = hashName[name1];
  auto ver2 = hashName[name2];
  auto relNode = net.getArc(ver1, ver2);
  return &relNode->_data;
}
