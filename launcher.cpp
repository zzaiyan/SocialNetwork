#include "launcher.h"
#include "ui_launcher.h"

Launcher::Launcher(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Launcher) {
  ui->setupUi(this);
  //设置窗口标题和大小
  this->setWindowTitle("基于社交网络的人物关系分析");
  this->setFixedSize(500, 400);
}

Launcher::~Launcher() {
  delete ui;
  delete canvas;
}

void Launcher::on_createNew_clicked() {
  if (canvas != nullptr)
    delete canvas;
  canvas = new CanvasWidget;
  canvas->setWindowModality(Qt::ApplicationModal);
  canvas->show();
}

void Launcher::on_openFile_clicked() {
  if (canvas != nullptr)
    delete canvas;
  canvas = new CanvasWidget;
  canvas->setWindowModality(Qt::ApplicationModal);
  canvas->show();
  canvas->readFile();
  //  QString OpenFile;
  // 打开文件夹中的csv文件
  //  OpenFile = QFileDialog::getOpenFileName(this, "Please choose a csv file",
  //  "",
  //                                          "csv file(*.csv)");
  //  if (OpenFile != "") {
  //    canvas->readFile(OpenFile);
  //  }
}
