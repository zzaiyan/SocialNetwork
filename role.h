#ifndef ROLE_H
#define ROLE_H

#include <QObject>
#include <QDrag>
#include <QDebug>
#include <QMimeData>
#include <QGraphicsItem>
#include <QGraphicsSceneEvent>
#include <QGraphicsScene>;
#include <QGraphicsView>;
#include <QRectF>
#include <QApplication>
#include <QtSvg>

class Role : public QGraphicsItem {
  public:
    Role(QString imgPath = "");
    QString imgPath;
    QString nameText = "";
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void setColor(int c);
    void setName(QString name);
    void setImgPath(QString path);
  protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  private:
    qreal radius;
    /* For display temporary tag */
    QGraphicsTextItem *nameTag = nullptr;
    QFont nameFont = QFont("微软雅黑", 13, QFont::Normal);
    /* color */
    const QColor BLUE = QColor(115,165,210);
    const QColor RED = QColor(216,135,133);
    const QColor PURPLE = QColor(164,130,210);
    const QColor YELLOW = QColor(208,181,117);
    QColor color;
};

#endif // ROLE_H
