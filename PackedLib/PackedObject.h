#ifndef PackedObjectContainer_H
#define PackedObjectContainer_H

#include <QPolygon>
#include<QVector>
#include<QTransform>
#include<QDebug>
class PackedObjectContainer{
    QVector<QPolygon> v;

public:
    PackedObjectContainer(){

    }

    void push_back(QPolygon Obj){
        v.push_back(Obj);
    }

    bool contains(QPolygon Obj){ // true = interseted
        for(int i = 0; i < v.size(); ++i){
            if (v[i].intersected(Obj).size() != 0) return 1;
        }
        return 0;
    }

    int size(){
        return v.size();
    }

    QVector<QPolygon> getObject(){
        return v;
    }

    QPolygon& operator[] (const int index){
        return v[index];
    }


};


class PackedLib{
    int W;
    int H;
public:
    PackedLib(int W, int H){
        this->W = W;
        this->H = H;
    }
    bool isOut(QPolygon &p){
        for(QPoint i : p){
            if (i.x() < 0 || i.x() > W) return 1;
            if (i.y() < 0 || i.y() > H) return 1;
        }
        return 0;
    }
    PackedObjectContainer fit(PackedObjectContainer v){
        PackedObjectContainer newV;

//        qInfo() << "Ok!";
        for(int k = 0; k < v.size(); ++k){
            bool flag = 0;
//            qInfo() << "Ok - start";
            for(int i = 0; i <= W; ++i){
                for(int j = 0; j <= H; ++j){

                   QPolygon p = v[k];
                   QTransform matrix;

                   p.translate(i, j);
                   if (!newV.contains(p) && !isOut(p)){

                       flag = 1;
                       newV.push_back(p);
                       qInfo() << i << j;
                       break;
                   }
                   p.translate(-i, -j);

                   matrix.rotate(90);
                   matrix.rotate(180, Qt::XAxis);
                   p = matrix.map(p);
                   p.translate(i, j);
                   if (!newV.contains(p) && !isOut(p)){
                       flag = 1;
                       newV.push_back(p);
                       qInfo() << i << j;
                       break;
                   }
                   p.translate(-i, -j);

                   matrix.rotate(90);
                   matrix.rotate(180, Qt::XAxis);
                   p = matrix.map(p);
                   p.translate(i, j);
                   if (!newV.contains(p) && !isOut(p)){
                       flag = 1;
                       newV.push_back(p);
                       qInfo() << i << j;
                       break;
                   }
                   p.translate(-i, -j);
                   matrix.rotate(90);
                   matrix.rotate(180, Qt::XAxis);
                   p = matrix.map(p);
                   p.translate(i, j);
                   if (!newV.contains(p) && !isOut(p)){
                       flag = 1;
                       newV.push_back(p);
                       qInfo() << i << j;
                       break;
                   }
                   p.translate(-i, -j);
                }
                if (flag) break;
                qInfo() << "Ok - 5";
            }
        }

        return newV;
    }


};


#endif // OBJECTI_H
