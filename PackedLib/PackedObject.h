#ifndef PackedObjectContainer_H
#define PackedObjectContainer_H

#include <ctime>
#include <random>
#include <algorithm>
#include <QPolygon>
#include<QVector>
#include<QTransform>
#include<QDebug>
class PackedObjectContainer{
    QVector<QPolygon> *v;

public:
    int fit = 0;
    PackedObjectContainer(){
        v = new QVector<QPolygon>;
        v->clear();
    }

    PackedObjectContainer(QVector<QPolygon> *v){
        v = new QVector<QPolygon>;
        this->v = v;
    }

    void push_back(QPolygon Obj){
        v->push_back(Obj);
    }

    bool contains(QPolygon Obj){ // true = interseted
        for(int i = 0; i < v->size(); ++i){
            if (v->at(i).intersected(Obj).size() != 0) return 1;
        }
        return 0;
    }

    int size(){
        return v->size();
    }
    void update(PackedObjectContainer* newV){
        fit = newV->fit;
        delete v;
        v = newV->getObject();
    }
    QVector<QPolygon>* getObject(){
        return v;
    }

    QPolygon at(const int index){
        return v->at(index);
    }

    QPolygon operator[] (const int index){
        return v->at(index);
    }

    QVector<QPolygon>::iterator begin(){
        return v->begin();
    }

    QVector<QPolygon>::iterator end(){
        return v->end();
    }

    PackedObjectContainer* nextPopulation(){
        PackedObjectContainer *newV = new PackedObjectContainer;

        for(auto i : *v){
            newV->push_back(i);
        }

        std::random_shuffle(newV->begin(), newV->end());


        return newV;
    }
    int fitFunc(int a, int b){
        if (a < 0 || b > v->size()) return -1;
        int fi = -1;
        for(int i =  a; i < b; ++i){
            for(QPoint j : v->at(i)){
                fi = std::max(fi, j.x());
            }
        }
        return fi;
    }


};


class PackedLib{
    int W;
    int H;
    QVector<PackedObjectContainer*> population;
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
    void fit(PackedObjectContainer *v){
        PackedObjectContainer *newV = new PackedObjectContainer;
   //        qInfo() << "Ok!";
           for(int k = 0; k < v->size(); ++k){
               bool flag = 0;
   //            qInfo() << "Ok - start";
               for(int i = 0; i <= W; ++i){
                   for(int j = 0; j <= H; ++j){

                      QPolygon p = v->at(k);
                      QTransform matrix;

                      p.translate(i, j);
                      if (!newV->contains(p) && !isOut(p)){

                          flag = 1;
                          newV->push_back(p);
                          //qInfo() << i << j;
                          break;
                      }
                      p.translate(-i, -j);

                      matrix.rotate(90);
                      matrix.rotate(180, Qt::XAxis);
                      p = matrix.map(p);
                      p.translate(i, j);
                      if (!newV->contains(p) && !isOut(p)){
                          flag = 1;
                          newV->push_back(p);
                          //qInfo() << i << j;
                          break;
                      }
                      p.translate(-i, -j);

                      matrix.rotate(90);
                      matrix.rotate(180, Qt::XAxis);
                      p = matrix.map(p);
                      p.translate(i, j);
                      if (!newV->contains(p) && !isOut(p)){
                          flag = 1;
                          newV->push_back(p);
                          //qInfo() << i << j;
                          break;
                      }
                      p.translate(-i, -j);

                      matrix.rotate(90);
                      matrix.rotate(180, Qt::XAxis);
                      p = matrix.map(p);
                      p.translate(i, j);
                      if (!newV->contains(p) && !isOut(p)){
                          flag = 1;
                          newV->push_back(p);
                         // qInfo() << i << j;
                          break;
                      }
                      p.translate(-i, -j);
                   }
                   if (flag) break;
                  // qInfo() << "Ok - 5";
               }
           }

           v->update(newV);

           int fi = -1;
           for(auto i : *v->getObject()){
               for(QPoint j : i){
                   fi = std::max(fi, j.x());
               }
           }

           v->fit = fi;

           return;
       }

    static bool cmp(PackedObjectContainer* a, PackedObjectContainer* b){
        return (a->fit < b->fit);
    }
    void initPopulation(PackedObjectContainer* v, int n){
        for(int i = 0; i < n; ++i){
            population.push_back(v->nextPopulation());
            fit(population[i]);
           // qInfo() << fitFumc(population[i]);
        }

        std::sort(population.begin(), population.end(), cmp);

//        for(int i = 0; i < population[0]->size(); ++i){
//            qInfo() << population[0]->at(i) << population[1]->at(i);
//        }
        for(int i = 0; i < population.size(); ++i){
            qInfo() << population[i]->fit << population[i]->fitFunc(0, population[i]->size());
        }
    }

    PackedObjectContainer* Top(){
        return population[0];
    }




};


#endif // OBJECTI_H
