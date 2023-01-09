#include "littlefamily.h"
#include "ui_littlefamily.h"
#include <QStandardItem>
#include <QStandardItemModel>

LittleFamily::LittleFamily(CanvasWidget *p,
                           vector<ALNet<RoleData, RelData>::VerNode *> *v,
                           QWidget *parent)
    : QWidget(parent), canvas(p), vers(v), ui(new Ui::LittleFamily) {
  ui->setupUi(this);

  //  ui->tableView->setHorizontalHeader(new QHeaderView())
  auto model = new QStandardItemModel(this);

  QStringList table_h_headers;
  table_h_headers << "团体"
                  << "颜色"
                  << "人数规模"
                  << "团体势力"
                  << "代表人物";

  model->setHorizontalHeaderLabels(table_h_headers);

  std::vector<std::pair<QString, QString>> trans{
      {"王家", "蓝色"}, {"宁国府", "红色"}, {"荣国府", "紫色"},
      {"史家", "黄色"}, {"薛家", "洋红"},   {"林家", "绿色"},
      {"其他", "灰色"},
  };

  struct Unit {
    int index = 0;     // 对应trans中的下标
    int cnt = 0;       // 人数
    double impact = 0; // 总影响力
    int roleID = 0;    // 代表人物下标
    bool operator<(const Unit &b) const { return impact > b.impact; }
  };

  vector<Unit> vecSort; // 初始化7个小团体
  for (int i = 0; i < 7; i++)
    vecSort.push_back({i, 0, 0, 0});

  for (size_t i = 0; i < vers->size(); i++) {
    // 人物数据
    auto data = vers->at(i)->_data;
    int index = data.color - 1;
    // 对于的小团体
    auto &unit = vecSort.at(index);
    // 更新小团体数据
    unit.cnt++;
    unit.impact += data.impact;
    // 更新影响力最大人物下标
    if (vers->at(unit.roleID)->_data.impact < data.impact) {
      unit.roleID = i;
    }
  }

  std::sort(vecSort.begin(), vecSort.end());

  for (size_t i = 0; i < vecSort.size(); i++) {
    auto &unit = vecSort.at(i);
    auto &family = trans.at(unit.index);
    auto role = vers->at(unit.roleID)->_data.name;

    model->insertRow(i);

    model->setItem(i, 0, new QStandardItem(family.first));
    model->setItem(i, 1, new QStandardItem(family.second));
    model->setItem(i, 2, new QStandardItem(QString::number(unit.cnt)));
    model->setItem(i, 3, new QStandardItem(QString::number(unit.impact)));
    model->setItem(i, 4, new QStandardItem(role));
  }

  auto text = QString("共有 %1 个小团体，其中 %2 势力最强。")
                  .arg(vecSort.size())
                  .arg(trans.at(vecSort.front().index).first);
  ui->label->setText(text);

  ui->tableView->setModel(model);
}

LittleFamily::~LittleFamily() { delete ui; }
