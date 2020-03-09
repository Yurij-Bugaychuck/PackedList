#ifndef PackedObjectContainer_H
#define PackedObjectContainer_H

#include <ctime>
#include <random>
#include <algorithm>
#include <QPolygon>
#include<QVector>
#include<QTransform>
#include<QDebug>
struct PackedObject{
    int number = 0;
    QPolygon polygon;

    PackedObject(){

    }
    PackedObject(QPolygon Obj, int i){
        polygon = Obj;
        number = i;
    }
    PackedObject(const PackedObject &Obj){
        polygon = Obj.polygon;
        number = Obj.number;
    }
    void update(const PackedObject &Obj){
        polygon = Obj.polygon;
        number = Obj.number;
    }
    PackedObject& operator= (const PackedObject &Obj)
    {
        polygon = Obj.polygon;
        number = Obj.number;

        return *this;
    }
};

class PackedObjectContainer{
    QVector<PackedObject> *v;

public:
    int fit = 0;
    PackedObjectContainer(){
        v = new QVector<PackedObject>;
        v->clear();
    }



    void push_back(QPolygon Obj){
        v->push_back(PackedObject(Obj, v->size()));

    }
    void push_back(PackedObject obj){
        v->push_back(obj);
    }

    bool contains(QPolygon Obj){ // true = interseted
        for(int i = 0; i < v->size(); ++i){
            if (v->at(i).polygon.intersected(Obj).size() != 0) return 1;
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
    QVector<PackedObject>* getObject(){
        return v;
    }

    PackedObject& at(int index) const{
        return v->begin()[index];
    }

    QVector<PackedObject>::iterator begin(){
        return v->begin();
    }

    QVector<PackedObject>::iterator end(){
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
            for(QPoint j : v->takeAt(i).polygon){
                fi = std::max(fi, j.x());
            }
        }
        return fi;
    }

    PackedObjectContainer* reverseObjects(){
        PackedObjectContainer *newV = new PackedObjectContainer;

        for(auto i : *v){
            QPolygon p = i.polygon;
            QTransform matrix;
            matrix.rotate(90);
            matrix.rotate(180, Qt::XAxis);
            p = matrix.map(p);
            newV->push_back(PackedObject(p, i.number));
        }

        return newV;
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

    bool inline isOut(QPolygon &p){
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

                      PackedObject p = v->at(k);


                      p.polygon.translate(i, j);
                      if (!newV->contains(p.polygon) && !isOut(p.polygon)){

                          flag = 1;
                          newV->push_back(p);
                          //qInfo() << i << j;
                          break;
                      }
                      p.polygon.translate(-i, -j);


                   }
                   if (flag) break;
                  // qInfo() << "Ok - 5";
               }
           }

           v->update(newV);

           int fi = -1;
           for(auto i : *v->getObject()){
               for(QPoint j : i.polygon){
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
        QVector<PackedObjectContainer*> lpopulation;
        QVector<PackedObjectContainer*> rpopulation;
        for(int i = 0; i < n / 2; ++i){
            lpopulation.push_back(v->nextPopulation());


           // qInfo() << fitFumc(population[i]);
        }
        v = v->reverseObjects();
        for(int i = 0; i < n / 2; ++i){
            rpopulation.push_back(v->nextPopulation());

        }

        for(int i = 0; i < lpopulation.size(); ++i){
            int k = rand() % (lpopulation[0]->size() - 1);
            qInfo() << k << lpopulation[0]->size() << " | KKKKKKKKKKKKKKK";
            for(int j = 0; j < k; ++j){

                int index2in1;
                for(index2in1 = 0; index2in1 < lpopulation.size(); ++index2in1){
                   if (rpopulation[i]->at(j).number == lpopulation[i]->at(index2in1).number) break;
                }

                int index1in2;
                for(index1in2 = 0; index1in2 < lpopulation.size(); ++index1in2){
                   if (lpopulation[i]->at(j).number == rpopulation[i]->at(index1in2).number) break;
                }

                PackedObject Temp = lpopulation[i]->at(j);
                lpopulation[i]->at(j) = (rpopulation[i]->at(j));
                rpopulation[i]->at(j) = (Temp);

                qInfo() << lpopulation[i]->at(index2in1).number << " <-> " << rpopulation[i]->at(index1in2).number;

                Temp = lpopulation[i]->at(index2in1);
                lpopulation[i]->at(index2in1) = rpopulation[i]->at(index1in2);
                rpopulation[i]->at(index1in2) = Temp;


                qInfo() << lpopulation[i]->at(index2in1).number << " <-> " << rpopulation[i]->at(index1in2).number;


            }
            population.push_back(lpopulation[i]);
            population.push_back(rpopulation[i]);
        }

        for(auto i : population){
            fit(i);
        }

        std::sort(population.begin(), population.end(), cmp);



//        for(int i = 0; i < population.size(); ++i){
//            qInfo() << population[i]->fit << '\n' << "------------";
//            for(auto j : *population[i]){
//                qInfo() << j.number << " ";
//            }
//            qInfo() << "---------------------\n";

//        }

//        for(auto i : population){
//            qInfo() << i->fit;
//        }

    }

    PackedObjectContainer* Top(){
        return population[5];
    }




};


#endif // OBJECTI_H
