#ifndef SIGNIFICANCER_H
#define SIGNIFICANCER_H

#include "canvaswidget.h"
#include "net.h"
#include "node.h"
#include <QWidget>

namespace Ui {
class Significancer;
}

class Significancer : public QWidget {
  Q_OBJECT

  CanvasWidget *canvas;
  vector<ALNet<RoleData, RelData>::VerNode *> *vers;

public:
  explicit Significancer(CanvasWidget *p,
                         vector<ALNet<RoleData, RelData>::VerNode *> *v,
                         QWidget *parent = nullptr);
  ~Significancer();

private:
  Ui::Significancer *ui;
};

#endif // SIGNIFICANCER_H
