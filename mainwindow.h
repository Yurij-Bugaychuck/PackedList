#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<PackedLib/PackedObject.h>
#include<QListWidget>
#include<PackedLib/PackedList.h>
#include<QDialog>
#include<QFormLayout>
#include<QLineEdit>
#include<QSpinBox>
#include<QDialogButtonBox>
#include<QMap>
#include<QLabel>
#include<PackedLib/polygoninputerwidget.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    int time = 0;
    int stime = 0;
public:
    double GlobalScale = 1;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    PackedObjectContainer* v = nullptr;
    PackedLib* Packed = nullptr;
    QTimer *timer = new QTimer(this);
    void addNewToList(PackedListItem*);
    void handleCanceled();
    QLabel *imageLabel = nullptr;
    QListWidgetItem* novitem;
private slots:
    void upScale();
    void processOneThing();
    void handleFinished();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void removeFromList();

    void copyFromList();
    void editFromList();

    void setnovitem(QListWidgetItem* itm);


    void on_action_JSON_triggered();

    void on_action_5_triggered();

    void on_saveAsImage_triggered();

    void on_Print_triggered();

    void on_addPolygon_clicked();

    void on_aboutDev_triggered();



    void on_pushButton__up_clicked();

    void on_pushButton_down_clicked();

    void on_action_triggered();

public slots:

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
