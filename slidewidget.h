#ifndef SLIDEWIDGET_H
#define SLIDEWIDGET_H

#include <QWidget>

namespace Ui {
class SlideWidget;
}

class SlideWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SlideWidget(QWidget *parent = 0);
    ~SlideWidget();
    Ui::SlideWidget *ui;
protected:
    void changeEvent(QEvent *e);

private:

};

#endif // SLIDEWIDGET_H
