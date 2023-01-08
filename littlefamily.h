#ifndef LITTLEFAMILY_H
#define LITTLEFAMILY_H

#include "canvaswidget.h"
#include "net.h"
#include "node.h"
#include <QWidget>

namespace Ui {
class LittleFamily;
}

class LittleFamily : public QWidget {
  Q_OBJECT

  CanvasWidget *canvas;
  vector<ALNet<RoleData, RelData>::VerNode *> *vers;

public:
  explicit LittleFamily(CanvasWidget *p,
                        vector<ALNet<RoleData, RelData>::VerNode *> *v,
                        QWidget *parent = nullptr);
  ~LittleFamily();

private:
  Ui::LittleFamily *ui;
};

#endif // LITTLEFAMILY_H
