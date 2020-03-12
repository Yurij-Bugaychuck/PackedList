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
    p1.putPoints(0, 4, 0, 0, 0, 31, 25, 31, 25, 0);

    QPolygon p2(4);
    p2.putPoints(0, 4, 0, 0, 0, 31, 10, 31, 10, 0);

    QPolygon p3(4); //2
    p3.putPoints(0, 4, 0, 0, 0, 54, 15, 54, 15, 0);

    QPolygon p4(4);
    p4.putPoints(0, 4, 0, 0, 0, 15, 18, 15, 18, 0);

    QPolygon p5(4);
    p5.putPoints(0, 4, 0, 0, 0, 31, 54, 31, 54, 0);

    QPolygon p6(4); //2
    p6.putPoints(0, 4, 0, 0, 0, 48, 15, 48, 15, 0);

    QPolygon p7(4);
    p7.putPoints(0, 4, 0, 0, 0, 35, 4, 35, 4, 0);

    QPolygon p8(4);
    p8.putPoints(0, 4, 0, 0, 0, 5, 8, 5, 8, 0);

    QPolygon p9(4);
    p9.putPoints(0, 4, 0, 0, 0, 12, 22, 12, 22, 0);

    qInfo() << p1;



    v = new PackedObjectContainer();


    v->push_back(p1);
    v->push_back(p2);
    v->push_back(p3);
    v->push_back(p3);
    v->push_back(p4);
    v->push_back(p5);
    v->push_back(p6);
    v->push_back(p6);

    v->push_back(p7);
    v->push_back(p8);
    v->push_back(p9);




    Packed = new PackedLib(1000, 80);

    Packed->initPopulation(v,100);

//    Packed->fit(v);
    v = Packed->Top();
    qInfo() << v->size();



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
    painter.scale(2, 2);
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
