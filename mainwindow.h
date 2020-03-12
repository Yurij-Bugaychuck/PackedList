#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "slidewidget.h"
#include <QPropertyAnimation>
#include<PackedLib/PackedObject.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    PackedObjectContainer* v;
    PackedLib* Packed;
private slots:


    void on_widget_customContextMenuRequested(const QPoint &pos);

protected:
//    void changeEvent(QEvent *e);
    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *ev);

private slots:
    void slotShowHideSlide();

private:
    Ui::MainWindow *ui;
    //Ui::SlideWidget *slwg;
    SlideWidget *m_slideWIdget;

    QPropertyAnimation *m_animation;

    void showHideSlideWidget(bool f_flag);
    bool m_boolHide;
    QRect m_geometry;

    void paintEvent(QPaintEvent *event);
};

#endif // MAINWINDOW_H
