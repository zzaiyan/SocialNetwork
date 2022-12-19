#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  //设置窗口标题和大小
  this->setWindowTitle("基于社交网络的人物关系分析");
  this->setFixedSize(500, 400);
  canvas = new MyCanvas;
}

MainWindow::~MainWindow() {
  delete ui;
  delete canvas;
}

void MainWindow::on_createNew_clicked() {
  canvas->setWindowModality(Qt::ApplicationModal);
  canvas->clear();
  canvas->show();
}

void MainWindow::on_openFile_clicked() {
  canvas->setWindowModality(Qt::ApplicationModal);
  canvas->clear();
  canvas->show();
  //  QString OpenFile;
  //  // 打开文件夹中的csv文件
  //  OpenFile = QFileDialog::getOpenFileName(this, "Please choose a csv file",
  //  "",
  //                                          "csv file(*.csv)");
  //  if (OpenFile != "") {
  //      canvas->readFile(OpenFile);
  //  }
  canvas->readFile();
}
