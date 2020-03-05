#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<PackedLib/PackedObject.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    PackedObjectContainer* v;
    PackedLib* Packed;
private slots:


    void on_widget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent *event);
};
#endif // MAINWINDOW_H
