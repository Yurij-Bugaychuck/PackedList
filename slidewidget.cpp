#include "slidewidget.h"
#include "ui_slidewidget.h"

SlideWidget::SlideWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SlideWidget)
{
    ui->setupUi(this);
}

SlideWidget::~SlideWidget()
{
    delete ui;
}

void SlideWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
