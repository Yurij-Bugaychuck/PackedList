#include "polygoninputerwidget.h"
#include<QPainter>
#include<QDebug>
#include<QMouseEvent>
#include<QtAlgorithms>
PolygonInputerWidget::PolygonInputerWidget(QWidget *parent) : QWidget(parent)
{
    this->setMouseTracking(true);
}

void PolygonInputerWidget::paintEvent(QPaintEvent *){
    QPainter painter(this);

    QPen *k = new QPen();
    k->setWidth(7);
    k->setCapStyle(Qt::RoundCap);
//    k->setJoinStyle(Qt::RoundJoin);
    k->setColor(QColor("#bdbdbd"));
    painter.setPen(*k);
    painter.translate(7, 7);
    for(int i = 0; i < this->width(); i += 1){
        for(int j = 0; j < this->height(); j += 1){
            if (i % 30 == 0 && j % 30 == 0){
                k->setColor("#e0e0e0");
                if (Poly.isEmpty() || (i == Poly.last().x() || j == Poly.last().y())) k->setColor("#cecece");
                painter.setPen(*k);

                painter.drawPoint(QPoint(i, j));
            }
        }
    }
    k->setColor("#2196f3");
    k->setWidth(9);

    if (Poly.last() == MousePointTo) k->setColor("#f44336");

    painter.setPen(*k);
    painter.drawPoint(MousePointTo);


    k->setColor("#2196f3");
    k->setWidth(4);
     painter.setPen(*k);

    for(int i = 0; i < Poly.size() - 1; ++i){

        painter.drawLine(Poly[i], Poly[i + 1]);


    }

    k->setColor("#eab830");
    k->setWidth(9);
    painter.setPen(*k);
    for(int i = 0; i < Poly.size(); ++i){
        painter.drawPoint(Poly[i]);
    }

    k->setColor("#2196f3");
    k->setWidth(9);

    if (Poly.last() == MousePointTo) k->setColor("#f44336");

    painter.setPen(*k);
    painter.drawPoint(MousePointTo);
}

void PolygonInputerWidget::mouseMoveEvent( QMouseEvent *e )
{
    //qInfo() << e->x() << e->y();
    int x = static_cast<int>(std::round(double(e->x() / 30.0)) * 30);
    int y = static_cast<int>(std::round(double(e->y() / 30.0)) * 30);
    MousePointTo = QPoint(x, y);

    this->repaint();
}

void PolygonInputerWidget::mousePressEvent(QMouseEvent *event)
{
    if (MousePointTo == Poly.last()) Poly.pop_back();
    else
    if (Poly.isEmpty()) Poly.push_back(MousePointTo);
    else if (MousePointTo.x() == Poly.last().x() || MousePointTo.y() == Poly.last().y()){
        Poly.push_back(MousePointTo);
    }

    this->repaint();
}

QPolygon PolygonInputerWidget::getPolygon(int MSH){
    QPolygon ans;
    int mn_x = INT_MAX;
    int mn_y = INT_MAX;
    for(auto i : Poly){
        i.setX(i.x() / 30 * MSH);
        i.setY(i.y() / 30 * MSH);
        ans.push_back(i);
        mn_x = std::min(i.x(), mn_x);
        mn_y = std::min(i.y(), mn_y);
    }

    ans.translate(-mn_x, -mn_y);

    return ans;
}
void PolygonInputerWidget::updatep(QPolygon p, int MSH){
    QPolygon ans;

    for(auto i : p){
        i.setX(i.x() / MSH * 30);
        i.setY(i.y() / MSH * 30);
        ans.push_back(i);

    }
    Poly = ans;
    qInfo () << Poly;
    return;
}
