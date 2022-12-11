#ifndef MYCANVAS_H
#define MYCANVAS_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QImage>
#include <QFileDialog>
#include <QFileInfo>
#include "role.h"
#include "graphview.h"

namespace Ui {
class MyCanvas;
}

class MyCanvas : public QWidget {
    Q_OBJECT

  public:
    explicit MyCanvas(QWidget *parent = nullptr);
    ~MyCanvas();

  private slots:
    void advance();

    void on_addRole_clicked();

    void on_deleteRole_clicked();

    void on_blueBtn_clicked();

    void on_redBtn_clicked();

    void on_purpleBtn_clicked();

    void on_yellowBtn_clicked();


    void on_nameEdit_editingFinished();

    void on_openFile_clicked();

    void on_exportImg_clicked();

  private:
    Ui::MyCanvas *ui;
    QGraphicsScene *scene;
    GraphView *view;
    Role *selectedItem;
};

#endif // MYCANVAS_H
