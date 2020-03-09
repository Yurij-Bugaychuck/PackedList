#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<PackedLib/PackedObject.h>
#include<QDebug>
#include<QtGui>
#include<QPainter>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qInfo() << "Ok";
    QPolygon p1(4);
    p1.putPoints(0, 4, 0, 0, 0, 4, 8, 4, 8, 0);

    QPolygon p2(4);
    p2.putPoints(0, 4, 0, 0, 0, 10, 1, 10, 1, 0);

    QPolygon p3(4);
    p3.putPoints(0, 4, 0, 0, 0, 2, 1, 2, 1, 0);


    v = new PackedObjectContainer();

    v->push_back(p2);
    v->push_back(p2);
    v->push_back(p2);
    v->push_back(p2);

    v->push_back(p1);
    v->push_back(p1);
    v->push_back(p1);
    v->push_back(p1);

    v->push_back(p3);
    v->push_back(p3);
    v->push_back(p3);
    v->push_back(p3);
    v->push_back(p2);
    v->push_back(p2);
    v->push_back(p2);
    v->push_back(p2);





    for(auto i : *v){
        qInfo() << i.number;
    }

    qInfo() << "OKKKK";




    Packed = new PackedLib(40, 20);

    Packed->initPopulation(v,100);

    v = Packed->Top();



}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *event)
{


    //create a QPainter and pass a pointer to the device.
    //A paint device can be a QWidget, a QPixmap or a QImage
    QImage image(800,600,QImage::Format_RGB32);
    image.fill(Qt::white);
    QPainter painter(this);
    QPainter painterImage(&image);
    painter.setPen(QPen(Qt::black, 0.09));
    painterImage.setPen(QPen(Qt::black, 0.09));
    //    painter.;
    painter.scale(20, 20);
    painterImage.scale(20, 20);

    for(int i = 0; i < v->size(); ++i){
        painter.drawPolygon(v->at(i).polygon);
        painterImage.drawPolygon(v->at(i).polygon);
    }

    QPolygon p2(3);
    p2.putPoints(0, 4, 5, 5, 5, 10, 6, 10, 6, 5);
    QTransform t;
    t.rotate(90);
//    painter.drawPolygon(t.map(p2));
    qInfo() << t.map(p2);

    image.save("fil.png");

}

void MainWindow::on_widget_customContextMenuRequested(const QPoint &pos)
{
}
