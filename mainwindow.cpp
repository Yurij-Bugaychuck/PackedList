#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<PackedLib/PackedObject.h>
#include<QDebug>
#include<QtGui>
#include<QPainter>
#include <QResizeEvent>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect( ui->pushButton, SIGNAL(clicked()), this, SLOT(slotShowHideSlide()));
    m_slideWIdget = new SlideWidget(this);
    m_slideWIdget->hide();
    m_geometry = m_slideWIdget->geometry();
    m_boolHide = true;
    ui->centralWidget->setMouseTracking(true);
    m_slideWIdget->setStyleSheet("background-color:#374039;");
    setStyleSheet("background-color:#374039;");

    qInfo() << "Ok";
    QPolygon p1(4);
    p1.putPoints(0, 4, 0, 0, 0, 1, 2, 1, 2, 0);

    QPolygon p2(4);
    p2.putPoints(0, 4, 0, 0, 0, 3, 1, 3, 1, 0);

    QPolygon p3(4);
    p3.putPoints(0, 4, 0, 0, 0, 2, 1, 1, 2, 0);


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







    for(auto i : *v->reverseObjects()){
        qInfo() << i.polygon;
    }

    qInfo() << "OKKKK";




    Packed = new PackedLib(30, 10);

    Packed->initPopulation(v,100);

    v = Packed->Top();



}

MainWindow::~MainWindow()
{
    delete ui;
}
//------------------------------------------------------------------------------
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (!m_boolHide)
        m_slideWIdget->setGeometry(QRect(0,0, m_slideWIdget->width(), height()));
}
//------------------------------------------------------------------------------
void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
    if (ev->pos().x() < 50)
    {
        if (m_boolHide)
            slotShowHideSlide();
    }

}
//------------------------------------------------------------------------------
void MainWindow::slotShowHideSlide()
{
    if (m_slideWIdget->isHidden())
        m_slideWIdget->show();

    showHideSlideWidget(m_boolHide);
}
//------------------------------------------------------------------------------
void MainWindow::showHideSlideWidget(bool f_flag)
{
    if (f_flag)
        m_slideWIdget->setGeometry(m_geometry);

    m_animation = new QPropertyAnimation(m_slideWIdget, "geometry");
    m_animation->setDuration(300);

    QRect startRect(-500, 0, m_slideWIdget->width(), height());
    QRect endRect(0, 0, m_slideWIdget->width(), height());

    if (f_flag)
    {
        m_animation->setStartValue(startRect);
        m_animation->setEndValue(endRect);
    }
    else
    {
        m_animation->setStartValue(endRect);
        m_animation->setEndValue(startRect);
    }
    m_animation->start();
    m_boolHide = !f_flag;
}
//------------------------------------------------------------------------------

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

