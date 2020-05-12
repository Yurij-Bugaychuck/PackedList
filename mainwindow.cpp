#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<QtGui>
#include<QPainter>
#include<QFile>
#include<QFileDialog>
#include<QtPrintSupport/QPrinter>
#include<QtPrintSupport/QPrintDialog>
#include<QPrintPreviewDialog>
#include<QMessageBox>
#include<QTextBrowser>
#include<QWebEngineView>
#include<QColorDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(processOneThing()));


    QWebEngineView *page = new QWebEngineView();


    page->load(QUrl(QLatin1String("qrc:/assets/help/help.html")));
    page->show();
    ui->scrollArea_2->setWidget(page);
}


MainWindow::~MainWindow()
{
    delete ui;
}

// Start proccess Calc
void MainWindow::on_pushButton_clicked()
{
    ui->pushButton->setEnabled(false);
    delete ui->scrollArea_2->widget();
    if (v) delete v;
    v = new PackedObjectContainer();


    for(int i = 0; i < ui->listWidget->count(); ++i){
        PackedListItem elem = dynamic_cast<PackedListItem*>(ui->listWidget->item(i));

        for(int j = 0; j < elem.Count; ++j){
            v->push_back(elem);
        }
    }

    if (v->size() == 0) {

        handleCanceled();
        return;
    }

    if (Packed) delete Packed;
    Packed = new PackedLib(0, ui->spinHeight->value());

    time = 0; stime = 0;
    timer->start(1000);

    //gen populations
    QFuture<void> future = QtConcurrent::run([=]() {
        Packed->initPopulation(v, 50);
    });

    connect(Packed, SIGNAL(setValue(int)), ui->progressBar, SLOT(setValue(int)));
    connect(Packed, SIGNAL(taskCompleted()), this, SLOT(handleFinished()));
}


//if error with init
void MainWindow::handleCanceled(){
    ui->pushButton->setEnabled(true);
}


//end proccess \ painting
void MainWindow::handleFinished(){
    if (!Packed) return;
    PackedObjectContainer *v = Packed->Top();

    qInfo() << v->size();


    qInfo() << v->size();
    qInfo() << v->fit << " ||typee";
    if (v->fit == INT_MAX) {
        handleCanceled();
        return;
    }
    timer->stop();

    //drawing a packing
    QPixmap* image = new QPixmap(QSize((v->fit + 100) * GlobalScale, (ui->spinHeight->value() + 100) * GlobalScale));
    QPaintEngine* eng = image->paintEngine();
    QBrush brush;

    if(eng) {
        image->fill(Qt::transparent);
        //QPainter painter(ui->centralwidget);
        QPainter painterImage(image);
        painterImage.scale(GlobalScale, GlobalScale);
        painterImage.translate(40, 40);
        //painter.setPen(QPen(Qt::black, 0.09));
        painterImage.setPen(QPen(Qt::black, 1));
        //    painter.;
        //painter.scale(2, 2);

        QFont font;
        font.setPixelSize(8);
        painterImage.setFont(font);
        int yfit = -1;
        for(int i = 0; i < v->size(); ++i){


            PackedObject itm = v->at(i);
            for(auto j: itm.polygon){
                yfit = std::max(yfit, j.y());
            }
           // painter.drawPolygon(v->at(i).polygon);
            QPainterPath path;
            path.addPolygon(itm.polygon);

                qInfo() <<itm.Name << itm.Color;

             QBrush  brush;
             brush.setColor(itm.Color);
                brush.setStyle(Qt::SolidPattern);
            painterImage.drawPolygon(itm.polygon);
            painterImage.fillPath(path, brush);
            painterImage.drawPolygon(itm.polygon);

            painterImage.drawText(itm.polygon[0].x() + 2, itm.polygon[0].y() + 8, itm.Name);

        }
        QPen k;
        k.setStyle(Qt::DashLine);
        k.setColor(QColor("#ff5722"));

        painterImage.setPen(k);
        painterImage.drawLine(v->fit + 1, 0, v->fit + 1, ui->spinHeight->value() + 20);
        painterImage.drawLine(0, yfit + 1, v->fit + 20, yfit + 1);

        font.setPixelSize(11);
        painterImage.setFont(font);
        painterImage.drawText(v->fit + 3, ui->spinHeight->value() + 10, QString::number(v->fit) + " cm");
        painterImage.drawText(10, yfit + 13, QString::number(yfit) + " cm");

        painterImage.translate(0, -40);
        font.setPixelSize(10);
        painterImage.setFont(font);
        k.setStyle(Qt::SolidLine);
        //k.setWidthF(0.5);
        painterImage.setPen(QPen(Qt::black, 1));
        for(int i = 0; i < v->fit + 20; i += 2){
            painterImage.drawLine(i, 0, i, 10);
            if (i % 20 == 0){
                painterImage.drawLine(i, 0, i, 15);

                int p = 2;
                if (i > 9) p = 4;
                if (i > 99) p = 6;
                if (i > 999) p = 8;
                 painterImage.drawText(i - p, 27, QString::number(i));
            }
        }

        painterImage.translate(-40, 40);
        for(int i = 0; i <= ui->spinHeight->value(); i += 2){
            painterImage.drawLine(0, i, 10, i);
            if (i % 20 == 0){
                painterImage.drawLine(0, i, 15, i);

                //painterImage.rotate(90);
                painterImage.drawText(17, i + 4, QString::number(i));
                //painterImage.rotate(-90);
            }
        }
        imageLabel = new QLabel;
        imageLabel->setPixmap(*image);

        ui->scrollArea_2->setWidget(imageLabel);
        ui->pushButton->setEnabled(true);
    }
}

//Timer
void MainWindow::processOneThing(){
    //ui->label_2->setVisible(true);
    qInfo() << "KEK";
    time+=1;
    qInfo() << time/60 << time % 60;
    QString t;
    if (time / 60 < 10) t += "0"; t += QString::number(time/60);
    t += ":";
    if (time % 60 < 10) t += "0"; t += QString::number(time%60);

    int pr = ui->progressBar->value();
    if (pr > 0 && stime == 0){
        double k = 100/pr;
        stime = k * time;
    }
    t += " / ";

    if (stime / 60 < 10) t += "0"; t += QString::number(stime/60);
    t += ":";
    if (stime % 60 < 10) t += "0"; t += QString::number(stime%60);

    ui->label_2->setText(t);

}


//Button to add a Rectangle
void MainWindow::on_pushButton_2_clicked()
{
    QDialog dialog(this);
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);
    dialog.setWindowTitle("Rectangle");
    // Add some text above the fields
    dialog.setFixedWidth(520);
    form.addRow(new QLabel("Add item"));

    // Add the lineEdits with their respective labels

    QLineEdit *Nameline = new QLineEdit(&dialog);
    form.addRow("Name", Nameline);

    QSpinBox *Widthline = new QSpinBox(&dialog);
    Widthline->setMaximum(500);
    form.addRow("Width", Widthline);

    QSpinBox *Heightline = new QSpinBox(&dialog);
    Heightline->setMaximum(500);
    form.addRow("Height", Heightline);

    QSpinBox *Countline = new QSpinBox(&dialog);
    Countline->setMaximum(100);
    form.addRow("Count", Countline);

    QColorDialog *colorDialog = new QColorDialog(&dialog);
    colorDialog->setWindowFlags(Qt::SubWindow);
    colorDialog->setOption(QColorDialog::NoButtons);
    colorDialog->setCurrentColor("#000");
//    colorDialog->setFixedWidth(500);
    form.addRow(colorDialog);
    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));





    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {

        //--------------------
        PackedListItem *itm = new PackedListItem(Nameline->text(), Widthline->value(), Heightline->value(), Countline->value(), colorDialog->currentColor());

        addNewToList(itm);
    }
}


//Void adding to PaintedList at right
void MainWindow::addNewToList(PackedListItem* itm){


    QPixmap* image = new QPixmap(QSize(itm->Width, itm->Height));
    QPaintEngine* eng = image->paintEngine();
    if(eng) {
        image->fill(Qt::white);
        //QPainter painter(ui->centralwidget);
        QPainter painterImage(image);
        //painter.setPen(QPen(Qt::black, 0.09));
        painterImage.setPen(QPen(itm->Color, 0.09));
        //    painter.;
        //painter.scale(2, 2);
        painterImage.scale(1, 1);
        QPainterPath path;
        path.addPolygon(itm->polygon);

        QBrush brush;
            brush.setColor(itm->Color);
            brush.setStyle(Qt::SolidPattern);
        painterImage.fillPath(path, brush);
    }

    itm->setIcon(*image);
    itm->setSizeHint(QSize(itm->sizeHint().width(), 90));
    ui->listWidget->setIconSize(QSize(80, image->width()));
    ui->listWidget->addItem(itm);


    //---------------
    QHBoxLayout *HLay = new QHBoxLayout();
    QVBoxLayout *HLay2 = new QVBoxLayout();
    QVBoxLayout *VLay1 = new QVBoxLayout();
    VLay1->setAlignment(Qt::AlignRight);
    QPushButton *b1 = new QPushButton;
    QPixmap delBtnIcon(":/assets/stop.png");
    b1->setIcon(delBtnIcon);
    b1->setIconSize(QSize(20, 20));
    b1->setStyleSheet("background-color: transparent; background-position:center center;");
    b1->setFixedSize(QSize(20, 20));

    QPushButton *b2 = new QPushButton;
    QPixmap copyBtnIcon(":/assets/copy.png");
    b2->setIcon(copyBtnIcon);
    b2->setIconSize(QSize(20, 20));
    b2->setStyleSheet("background-color: transparent; background-position:center center;");
    b2->setFixedSize(QSize(20, 20));

    QPushButton *b3 = new QPushButton;
    QPixmap editBtnIcon(":/assets/edit.png");
    b3->setIcon(editBtnIcon);
    b3->setIconSize(QSize(20, 20));
    b3->setStyleSheet("background-color: transparent; background-position:center center;");
    b3->setFixedSize(QSize(20, 20));

    QLabel *n = new QLabel((itm->Name));
    n->setStyleSheet("background: url(':/assets/tag.png') no-repeat left center; padding-left: 20px; vertical-align:middle;");

    QLabel *w = new QLabel(QString::number(itm->Width));
    w->setStyleSheet("background: url(':/assets/width.png') no-repeat left center; padding-left: 20px; vertical-align:middle;");

    QLabel *h = new QLabel(QString::number(itm->Height));
    h->setStyleSheet("background: url(':/assets/height.png') no-repeat; background-position: left center; padding-left: 20px; vertical-align:middle;");
    QLabel *c= new QLabel("Count:" + QString::number(itm->Count));

    QLabel *Msh= new QLabel("Scale:" + QString::number(itm->msh));

    HLay2->addWidget(n);
    if (!itm->isPoly) HLay2->addWidget(w);
    if (!itm->isPoly) HLay2->addWidget(h);
    if(itm->isPoly) HLay2->addWidget(Msh);
    HLay2->addWidget(c);


    VLay1->addWidget(b1);
    VLay1->addWidget(b2);
    VLay1->addWidget(b3);
    HLay->addLayout(HLay2);
    HLay->addLayout(VLay1);

    QWidget * twoButtonWidget = new QWidget();


    twoButtonWidget->setLayout( HLay );

    ui->listWidget->setItemWidget(itm, twoButtonWidget);

    connect(b1, SIGNAL(clicked()), this, SLOT(removeFromList()));
    connect(b2, SIGNAL(clicked()), this, SLOT(copyFromList()));
    connect(b3, SIGNAL(clicked()), this, SLOT(editFromList()));
}


//Remove from PaintedList
void MainWindow::removeFromList(){
    QPoint globalCursorPos = ui->listWidget->mapFromGlobal(QCursor::pos());
    qInfo() <<globalCursorPos<< ui->listWidget->itemAt(globalCursorPos);
    PackedListItem *it = dynamic_cast<PackedListItem*>(ui->listWidget->itemAt(globalCursorPos));

    qInfo() << it->Name;

    delete it;
}


//Copy From PaintedList
void MainWindow::copyFromList(){
    QPoint globalCursorPos = ui->listWidget->mapFromGlobal(QCursor::pos());
    qInfo() <<globalCursorPos<< ui->listWidget->itemAt(globalCursorPos);
    PackedListItem *it = dynamic_cast<PackedListItem*>(ui->listWidget->itemAt(globalCursorPos));

    PackedListItem *newIt = new PackedListItem(it);
    addNewToList(newIt);

}


//Edit From PaintedList
void MainWindow::editFromList(){
    QPoint globalCursorPos = ui->listWidget->mapFromGlobal(QCursor::pos());
    qInfo() <<globalCursorPos<< ui->listWidget->itemAt(globalCursorPos);
    PackedListItem *itm = dynamic_cast<PackedListItem*>(ui->listWidget->itemAt(globalCursorPos));

    QDialog dialog(this);
    dialog.setWindowTitle("Редактирование элемента");
    dialog.setFixedWidth(520);
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);


    // Add some text above the fields
    form.addRow(new QLabel("Добавление элемента"));

    // Add the lineEdits with their respective labels

    PolygonInputerWidget *k;
    QSpinBox *mash;
    if (itm->isPoly){
        k = new PolygonInputerWidget(&dialog);
        mash = new QSpinBox(&dialog);
        k->setFixedHeight(200);
        k->updatep(itm->polygon, itm->msh);
        form.addRow(k);
        dialog.setFixedWidth(520);
        // Add some text above the fields

        // Add the lineEdits with their respective labels

        mash->setMaximum(999);
        mash->setValue(itm->msh);
        form.addRow("Scale (1:X)", mash);
    }

    QLineEdit *Nameline = new QLineEdit(&dialog);
    Nameline->setText(itm->Name);
    form.addRow("Name", Nameline);
    qInfo() << itm->isPoly;

    QSpinBox *Widthline;

    if (!itm->isPoly){
        Widthline = new QSpinBox(&dialog);
        if (!itm->isPoly) Widthline->setParent(&dialog);
        Widthline->setMaximum(500);
        Widthline->setValue(itm->Width);
        if (!itm->isPoly) form.addRow("Width", Widthline);
    }


    QSpinBox *Heightline;
    if (!itm->isPoly) {
        Heightline = new QSpinBox(&dialog);
        Heightline->setMaximum(500);
        Heightline->setValue(itm->Height);
        form.addRow("Height", Heightline);
    }

    QSpinBox *Countline = new QSpinBox(&dialog);
    Countline->setMaximum(100);
    Countline->setValue(itm->Count);
    form.addRow("Count", Countline);


    QColorDialog *colorDialog = new QColorDialog(&dialog);
    colorDialog->setWindowFlags(Qt::SubWindow);
    colorDialog->setOption(QColorDialog::NoButtons);
    colorDialog->setCurrentColor(itm->Color);
//    colorDialog->setFixedWidth(500);
    form.addRow(colorDialog);

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));





    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {

        //--------------------
        //PackedListItem *nitm = new PackedListItem(Nameline->text(), Widthline->value(), Heightline->value(), Countline->value());

        itm->Name = Nameline->text();
        itm->Color = colorDialog->currentColor();
        if (!itm->isPoly) itm->Width = Widthline->value();
        if (!itm->isPoly) itm->Height = Heightline->value();
        if (itm->isPoly) itm->msh = mash->value();
        itm->Count = Countline->value();

        if (!itm->isPoly) itm->updatePolygon();
        else itm->polygon = k->getPolygon(itm->msh);

        QPixmap* image = new QPixmap(QSize(itm->Width, itm->Height));
        QPaintEngine* eng = image->paintEngine();
        if(eng) {
            image->fill(Qt::white);
            //QPainter painter(ui->centralwidget);
            QPainter painterImage(image);
            //painter.setPen(QPen(Qt::black, 0.09));
            painterImage.setPen(QPen(itm->Color, 0.09));
            //    painter.;
            //painter.scale(2, 2);
            painterImage.scale(1, 1);
            QPainterPath path;
            path.addPolygon(itm->polygon);

            QBrush brush;
                brush.setColor(itm->Color);
                brush.setStyle(Qt::SolidPattern);
            painterImage.fillPath(path, brush);
        }

        itm->setIcon(*image);
        itm->setSizeHint(QSize(itm->sizeHint().width(), 90));
        ui->listWidget->setIconSize(QSize(80, image->width()));
        ui->listWidget->addItem(itm);


        //---------------
        QHBoxLayout *HLay = new QHBoxLayout();
        QVBoxLayout *HLay2 = new QVBoxLayout();
        QVBoxLayout *VLay1 = new QVBoxLayout();
        VLay1->setAlignment(Qt::AlignRight);
        QPushButton *b1 = new QPushButton;
        QPixmap delBtnIcon(":/assets/stop.png");
        b1->setIcon(delBtnIcon);
        b1->setIconSize(QSize(20, 20));
        b1->setStyleSheet("background-color: transparent; background-position:center center;");
        b1->setFixedSize(QSize(20, 20));

        QPushButton *b2 = new QPushButton;
        QPixmap copyBtnIcon(":/assets/copy.png");
        b2->setIcon(copyBtnIcon);
        b2->setIconSize(QSize(20, 20));
        b2->setStyleSheet("background-color: transparent; background-position:center center;");
        b2->setFixedSize(QSize(20, 20));

        QPushButton *b3 = new QPushButton;
        QPixmap editBtnIcon(":/assets/edit.png");
        b3->setIcon(editBtnIcon);
        b3->setIconSize(QSize(20, 20));
        b3->setStyleSheet("background-color: transparent; background-position:center center;");
        b3->setFixedSize(QSize(20, 20));

        QLabel *n = new QLabel((itm->Name));
        n->setStyleSheet("background: url(':/assets/tag.png') no-repeat left center; padding-left: 20px; vertical-align:middle;");

        QLabel *w = new QLabel(QString::number(itm->Width));
        w->setStyleSheet("background: url(':/assets/width.png') no-repeat left center; padding-left: 20px; vertical-align:middle;");

        QLabel *h = new QLabel(QString::number(itm->Height));
        h->setStyleSheet("background: url(':/assets/height.png') no-repeat; background-position: left center; padding-left: 20px; vertical-align:middle;");
        QLabel *c= new QLabel("Count:" + QString::number(itm->Count));

        QLabel *Msh= new QLabel("Scale:" + QString::number(itm->msh));

        HLay2->addWidget(n);
        if (!itm->isPoly) HLay2->addWidget(w);
        if (!itm->isPoly) HLay2->addWidget(h);
        if(itm->isPoly) HLay2->addWidget(Msh);
        HLay2->addWidget(c);


        VLay1->addWidget(b1);
        VLay1->addWidget(b2);
        VLay1->addWidget(b3);
        HLay->addLayout(HLay2);
        HLay->addLayout(VLay1);

        QWidget * twoButtonWidget = new QWidget();


        twoButtonWidget->setLayout( HLay );

        ui->listWidget->setItemWidget(itm, twoButtonWidget);

        connect(b1, SIGNAL(clicked()), this, SLOT(removeFromList()));
        connect(b2, SIGNAL(clicked()), this, SLOT(copyFromList()));
        connect(b3, SIGNAL(clicked()), this, SLOT(editFromList()));




//        ui->listWidget->;
    }
}


//save to JSON
void MainWindow::on_action_JSON_triggered()
{
    QJsonArray arr;

    for(int i = 0; i < ui->listWidget->count(); ++i){
        QJsonObject obj;
        PackedListItem *it = dynamic_cast<PackedListItem*>(ui->listWidget->item(i));
        obj.insert("isRect", !it->isPoly);
        obj.insert("Name", it->Name);
        obj.insert("Count", it->Count);

        if (it->isPoly){

            QJsonArray js;
            for(auto p : it->polygon){
                QJsonObject a = {{"x", p.x()}, {"y", p.y()}};
                js.push_back(a);
            }
            obj.insert("Scale", it->msh);
            obj.insert("Poly", js);

        }else{
            obj.insert("Width", it->Width);
            obj.insert("Height", it->Height);
        }


        arr.push_back(obj);
    }
    QJsonDocument doc;
    doc.setArray(arr);
    QString fileName = QFileDialog::getSaveFileName(this,
                                                            tr("Сохранить файл настроек"), "",
                                                            tr("JSON (*.json);"));
    QFile file(fileName);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
          {
              // We're going to streaming text to the file
              QTextStream stream(&file);

              stream << doc.toJson();

              file.close();
              qDebug() << "Writing finished";
          }

}



//load from JSON
void MainWindow::on_action_5_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                            tr("Открыь файл настроек"), "",
                                                            tr("JSON (*.json);"));
    QFile file(fileName);

     if(file.open(QIODevice::ReadOnly | QIODevice::Text))
     {
         QString raw = file.readAll();

         QJsonDocument doc = QJsonDocument::fromJson(raw.toUtf8());

         QJsonArray arr = doc.array();

         ui->listWidget->clear();
         for(auto i : arr){
            QJsonObject obj = i.toObject();
            PackedListItem *itm;

            if (obj["isRect"].toBool()){
                itm = new PackedListItem(obj["Name"].toString(), obj["Width"].toInt(), obj["Height"].toInt(), obj["Count"].toInt());
            }else{
                QPolygon p;
                QJsonArray a = obj["Poly"].toArray();
                for(auto l : a){
                    QPoint ps(l.toObject()["x"].toInt(), l.toObject()["y"].toInt());
                    p.push_back(ps);
                }
                itm = new PackedListItem(obj["Name"].toString(), obj["Count"].toInt(), p);
                itm->msh = obj["Scale"].toInt();            }

            addNewToList(itm);
         }



     }


}


//save as image
void MainWindow::on_saveAsImage_triggered()
{
    if (imageLabel == nullptr) return;
    if (imageLabel && imageLabel->pixmap() != nullptr){


        QPixmap* image = new QPixmap(imageLabel->pixmap()->size());
        QPaintEngine* eng = image->paintEngine();
        if(eng) {
            image->fill(Qt::white);
            //QPainter painter(ui->centralwidget);
            QPainter painterImage(image);
            //painter.setPen(QPen(Qt::black, 0.09));
            painterImage.setPen(QPen(Qt::black, 0.09));

            painterImage.drawImage(0, 0, imageLabel->pixmap()->toImage());
        }

        qInfo() << *image;
        //p.fill(Qt::white);
        QString fileName = QFileDialog::getSaveFileName(this,
                                                                tr("Сохранить файл настроек"), "",
                                                                tr("JPG (*.jpg);;PNG (*.png);"));

        image->save(fileName);

    }
}


// Print
void MainWindow::on_Print_triggered()
{

    if (imageLabel && imageLabel->pixmap()){




        QPrinter printer;

        printer.setPaperSize(QPrinter::A4);
        printer.setFullPage(true);
        QPrintDialog *dialog = new QPrintDialog(&printer);
        dialog->setWindowTitle("Print Document");

        if (dialog->exec() != QDialog::Accepted)
            return;
        //QPainter painter(ui->centralwidget);
        QPainter painter;
        painter.begin(&printer);
        QPixmap img = *imageLabel->pixmap();
        QMatrix rm;
        rm.rotate(90);
        img = img.transformed(rm);
        painter.drawImage(0, 0, img.toImage());

        painter.end();
    }

}


// Button to adding Polygon
void MainWindow::on_addPolygon_clicked()
{
    QDialog dialog(this);
    dialog.setFixedWidth(500);
    dialog.setWindowTitle("Polygon");
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);
    form.addRow(new QLabel("Item settings"));

    PolygonInputerWidget *k = new PolygonInputerWidget(&dialog);
    k->setFixedHeight(200);
    form.addRow(k);
    // Add some text above the fields

    // Add the lineEdits with their respective labels

    QSpinBox *mash = new QSpinBox(&dialog);
    mash->setMaximum(999);
    mash->setValue(10);
    form.addRow("Scale (1:X)", mash);

    QLineEdit *Nameline = new QLineEdit(&dialog);
    form.addRow("Name", Nameline);

    QSpinBox *Countline = new QSpinBox(&dialog);
    Countline->setMaximum(100);
    form.addRow("Count", Countline);

    QColorDialog *colorDialog = new QColorDialog(&dialog);
    colorDialog->setWindowFlags(Qt::SubWindow);
    colorDialog->setOption(QColorDialog::NoButtons);
    colorDialog->setCurrentColor("#000");
    form.addRow(colorDialog);

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));





    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted) {
        qInfo() << k->getPolygon(mash->value());

        //--------------------
        PackedListItem *itm = new PackedListItem(Nameline->text(), Countline->value(), k->getPolygon(mash->value()), colorDialog->currentColor());
        itm->msh = mash->value();
        itm->isPoly = 1;
        addNewToList(itm);
    }

}



//About Modal Window
void MainWindow::on_aboutDev_triggered()
{

    QDialog *p = new QDialog(this);
    p->setFixedWidth(520);
    QVBoxLayout info(p);
    QLabel msg;

    p->setWindowTitle("Про разработчиков");



    msg.setText("<p align=\"center\"><b>Разработчики</b></p><ul style=\"list-style:none; margin-left: 74px\">"
                "<li>Бугайчук Юрий | Программист | <a href=\"mailto:bugaychuck@gmail.com\">bugaychuck@gmail.com</a></li>"
                "<li>Чепец Иван | Программист | <a href=\"mailto:ivanchepets14@gmail.com\">ivanchepets14@gmail.com</a></li> "
                "<li>Макин Андрей | Программист | <a href=\"mailto:makintoshka1522@gmail.com\">makintoshka1522@gmail.com</a></li>"
                "<li>Сохет Лея | Математик | <a href=\"mailto:leasokhet@gmail.com\">leasokhet@gmail.com</a></li>"
                "<li>Пенцова Мария | Математик | <a href=\"mailto:marija2718@gmail.com\">marija2718@gmail.com</a></li></ul>"
                "<hr><p align=\"center\"><b>Руководитель проекта</b></p><p align=\"center\">Процай Наталья Тимофеевна</p><hr>"
                "<p style=\"padding: 0; margin: 0; text-align:center;\"><img src=\":/assets/logo.png\"> </p>"
                "<p align=\"center\" style=\"padding: 0; margin: 0;text-align:center;\">Национальный технический университет <br> \"Харьковский политехнический институт\"</p>"
                "<p style=\"text-align:center;\">&copy; 2020</p>");

    info.addWidget(&msg);

    p->exec();
}

// Scale up
void MainWindow::on_pushButton__up_clicked()
{
    GlobalScale += 0.5;
    handleFinished();
}


// Scale Down
void MainWindow::on_pushButton_down_clicked()
{
    GlobalScale -= 0.5;
    handleFinished();
}


// Instruction modal window
void MainWindow::on_action_triggered()
{
    QDialog *p = new QDialog(this);
    p->setFixedWidth(800);
     p->setFixedHeight(800);
    QVBoxLayout info(p);
    p->setWindowTitle("Краткая инструкция");
   // QTextBrowser msg;

    QWebEngineView page;
   // msg.setSource(QUrl(QLatin1String("qrc:/assets/help/help.html")));

    page.load(QUrl(QLatin1String("qrc:/assets/help/help.html")));
    info.addWidget(&page);
    p->exec();
}
