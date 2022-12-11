#include "mycanvas.h"
#include "ui_mycanvas.h"
#include <QDebug>

MyCanvas::MyCanvas(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyCanvas) {
    ui->setupUi(this);
    ui->addRole->setIconSize(QSize(25,25));
    ui->addRole->setIcon(QIcon(":/icon/role2.svg"));
    ui->deleteRole->setIconSize(QSize(20,20));
    ui->deleteRole->setIcon(QIcon(":/icon/delete.svg"));
    scene= new QGraphicsScene;
    view = new GraphView;
    view->setScene(scene);
    view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);//图像平滑和抗锯齿
    scene->setSceneRect(-450,-335,900,670);
    view->resize(902,672);
    view->setMinimumWidth(400);
    ui->horizontalLayout_6->addWidget(view);
    ui->imgLabel->setScaledContents(true);
    ui->openFile->setEnabled(false);
    //advance();
    connect(scene, SIGNAL(selectionChanged()),this,SLOT(advance()));
}

MyCanvas::~MyCanvas() {
    delete ui;
}

void MyCanvas::advance() {
    if(scene->itemsBoundingRect().width()>scene->sceneRect().width()||scene->itemsBoundingRect().height()>scene->sceneRect().height())
        view->setSceneRect(scene->itemsBoundingRect());
    if (!scene->selectedItems().isEmpty()) {
        selectedItem = dynamic_cast<Role*>(scene->selectedItems().front());
        QString imgPath = selectedItem->imgPath,
                name = selectedItem->nameText;
        QPixmap p(imgPath.isEmpty()?":/icon/role.svg":imgPath);
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
    Role *role = new Role();
    QSize viewSize = view->size();
    role->setPos(view->mapToScene(viewSize.width()/2,viewSize.height()/2));
    scene->addItem(role);
}

void MyCanvas::on_deleteRole_clicked() {
    if (selectedItem!=nullptr) {
        scene->removeItem(selectedItem);
    }
}

void MyCanvas::on_blueBtn_clicked() {
    if (selectedItem!=nullptr) {
        selectedItem->setColor(1);
    }
}

void MyCanvas::on_redBtn_clicked() {
    if (selectedItem!=nullptr) {
        selectedItem->setColor(2);
    }
}

void MyCanvas::on_purpleBtn_clicked() {
    if (selectedItem!=nullptr) {
        selectedItem->setColor(3);
    }
}

void MyCanvas::on_yellowBtn_clicked() {
    if (selectedItem!=nullptr) {
        selectedItem->setColor(4);
    }
}

void MyCanvas::on_nameEdit_editingFinished() {
    if (selectedItem!=nullptr) {
        selectedItem->setName(ui->nameEdit->text());
    }
}


void MyCanvas::on_openFile_clicked() {
    QImage image;
    QString OpenFile;
    //打开文件夹中的图片文件
    OpenFile = QFileDialog::getOpenFileName(this,
                                            "Please choose an image file",
                                            "",
                                            "Image Files(*.jpg *.png *.bmp *.pgm *.pbm);;All(*.*)");
    if( OpenFile != "" && selectedItem!=nullptr) {
        if( image.load(OpenFile) ) {
            ui->imgLabel->setPixmap(QPixmap::fromImage(image));
            selectedItem->setImgPath(OpenFile);
        }
    }
}


void MyCanvas::on_exportImg_clicked() {
    QSize mysize(scene->width()*2, scene->height()*2); //获取 QGraphicsScene.size
    QImage image(mysize, QImage::Format_ARGB32_Premultiplied);  //保存透明度
    QPainter painter(&image);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    scene->render(&painter);   //关键函数
    image.save("D:\\SaveGraphicsScene.png");
}

