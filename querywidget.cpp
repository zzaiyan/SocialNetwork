#include "querywidget.h"
#include "ui_querywidget.h"
#include <QMessageBox>

QueryWidget::QueryWidget(CanvasWidget *p, QWidget *parent)
    : canvas(p), QWidget(parent), ui(new Ui::QueryWidget) {
  ui->setupUi(this);

  canvas->queryParaCnt = -1;
  canvas->key1.clear();
  canvas->key2.clear();
}

QueryWidget::~QueryWidget() {
  delete ui;
  canvas->checkQuery();
}

void QueryWidget::on_queryRole_clicked() {
  auto A = ui->lineEdit->text();
  if (A.isEmpty()) {
    QMessageBox::warning(nullptr, "警告", "输入为空！");
    return;
  }
  canvas->key1 = A;

  canvas->SearchModelChoice = ui->comboBox->currentIndex();

  canvas->queryParaCnt = 1;

  delete this;
}

void QueryWidget::on_queryRel_clicked() {
  auto A = ui->lineEdit_2->text();
  auto B = ui->lineEdit_3->text();
  if (A.isEmpty() || B.isEmpty()) {
    QMessageBox::warning(nullptr, "警告", "输入为空！");
    return;
  }
  canvas->key1 = A;
  canvas->key2 = B;

  canvas->SearchModelChoice = ui->comboBox->currentIndex();

  canvas->queryParaCnt = 2;

  delete this;
}
