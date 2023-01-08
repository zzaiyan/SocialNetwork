#include "significancer.h"
#include "ui_significancer.h"
#include <QStandardItem>
#include <QStandardItemModel>

Significancer::Significancer(CanvasWidget *p,
                             vector<ALNet<RoleData, RelData>::VerNode *> *v,
                             QWidget *parent)
    : QWidget(parent), canvas(p), vers(v), ui(new Ui::Significancer) {
  ui->setupUi(this);

  //  ui->tableView->setHorizontalHeader(new QHeaderView())
  auto model = new QStandardItemModel(this);

  QStringList table_h_headers;
  table_h_headers << "人物"
                  << "影响力";

  model->setHorizontalHeaderLabels(table_h_headers);

  std::vector<std::pair<QString, double>> sortVec;

  for (size_t i = 0; i < vers->size(); i++) {
    auto data = vers->at(i)->_data;
    auto name = data.name;
    auto impact = data.impact;
    qDebug() << name << impact;
    sortVec.push_back({name, impact});
  }

  std::sort(sortVec.begin(), sortVec.end(),
            [](auto &a, auto &b) { return a.second > b.second; });

  for (size_t i = 0; i < sortVec.size(); i++) {
    auto name = sortVec.at(i).first;
    auto impact = QString::number(sortVec.at(i).second);
    model->insertRow(i);
    model->setItem(i, 0, new QStandardItem(name));
    model->setItem(i, 1, new QStandardItem(impact));
  }

  auto text = QString("共有 %1 个人物，其中 %2 影响力最大。")
                  .arg(sortVec.size())
                  .arg(sortVec.front().first);

  ui->label->setText(text);

  ui->tableView->setModel(model);
}

Significancer::~Significancer() { delete ui; }
