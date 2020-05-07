// Adding Polygon Widget for Modal Window


#ifndef POLYGINONPUTERWIDGET_H
#define POLYGINONPUTERWIDGET_H

#include <QWidget>

class PolygonInputerWidget : public QWidget
{
    Q_OBJECT

    QPoint MousePointTo;
    QPolygon Poly;

public:
    explicit PolygonInputerWidget(QWidget *parent = nullptr);
    QPolygon getPolygon(int);
    void updatep(QPolygon p, int);

protected:
     void paintEvent(QPaintEvent *event) override;
     void mouseMoveEvent( QMouseEvent *e ) override;
     void mousePressEvent(QMouseEvent *event) override;

signals:

};

#endif // POLYGININPUTERWIDGET_H
