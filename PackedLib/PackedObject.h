#ifndef PACKEDOBJECT_H
#define PACKEDOBJECT_H

#include <QRect>
#include<QVector>

class PackedObjectI{

public:

    bool virtual contains(QRect obj);

    QVector<QPoint> virtual getCoords();

};


class PackedObjectRect : public PackedObjectI{
    QRect *Obj;

public:
    PackedObjectRect(int W, int H){
        Obj = new QRect(0, 0, W, H);
    };

    bool contains(QRect obj) override{
        return Obj->contains(obj);
    }

    QVector<QPoint> getCoords() override {
        QVector<QPoint> Coords;

        return Coords;
    };


};



class PackedObject{

public:




};
#endif // OBJECTI_H
