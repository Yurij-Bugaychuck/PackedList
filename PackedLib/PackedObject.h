// Population item


#ifndef PackedObjectContainer_H
#define PackedObjectContainer_H

#include <ctime>
#include <random>
#include <algorithm>
#include <QPolygon>
#include<QVector>
#include<QTransform>
#include<QDebug>
#include<QSet>
#include<set>
#include <QProgressBar>
#include<PackedLib/PackedList.h>

#include <QObject>
#include <thread>
#include<execution>
#include<omp.h>
#include <QtConcurrent/QtConcurrent>
struct PackedObject{
    int number = 0;
    QPolygon polygon;
    QString Name = "default";
    QColor Color;
    PackedObject(){

    }
    PackedObject(QPolygon Obj, int i, QString name = "default", QColor color = "#000"){
        polygon = Obj;
        number = i;
        Name = name;
        Color = color;
    }
    PackedObject(PackedListItem Obj, int i){
        polygon = Obj.polygon;
        Name = Obj.Name;
        Color = Obj.Color;
        number = i;
    }
    PackedObject(const PackedObject &Obj){
        polygon = Obj.polygon;
        number = Obj.number;
        Name = Obj.Name;
        Color = Obj.Color;
    }
    void update(const PackedObject &Obj){
        polygon = Obj.polygon;
        number = Obj.number;
        Name = Obj.Name;
        Color = Obj.Color;
    }
    PackedObject& operator= (const PackedObject &Obj)
    {
        polygon = Obj.polygon;
        number = Obj.number;
        Name = Obj.Name;
        Color = Obj.Color;

        return *this;
    }
};


// Population
class PackedObjectContainer{
    QVector<PackedObject> *v;

public:
    ~PackedObjectContainer(){
        v->clear();
        delete v;
    }
    int fit = 0;
    PackedObjectContainer(){
        v = new QVector<PackedObject>;
        v->clear();
    }



    void push_back(QPolygon Obj){
        v->push_back(PackedObject(Obj, v->size()));
    }

    void push_back(PackedListItem Obj){
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


    // gen new population \ random funct
    PackedObjectContainer* nextPopulation(){
        PackedObjectContainer *newV = new PackedObjectContainer;

        for(auto i : *v){
            newV->push_back(i);
        }



        //std::random_shuffle(newV->begin(), newV->end());

        std::random_device rd;
        std::mt19937 g(rd());

        std::shuffle(newV->begin(), newV->end(), g);




        return newV;
    }

    // calc fit Func from a to b
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

    void clear(){
        v->clear();
    }

    // rotate Objects
    PackedObjectContainer* reverseObjects(int H){
        PackedObjectContainer *newV = new PackedObjectContainer;

        for(auto i : *v){
            bool k = false;
            for(auto j : i.polygon){
                if (j.x() > H){
                    k = true;
                    break;
                }
            }
            QPolygon p = i.polygon;
            if (!k) {
                QTransform matrix;
                matrix.rotate(90);
                matrix.rotate(180, Qt::XAxis);
                p = matrix.map(p);
            }

            newV->push_back(PackedObject(p, i.number, i.Name, i.Color));

        }

        return newV;
    }






};




// Population container
class PackedLib : public QObject{
    Q_OBJECT
    int W;
    int H;
    QVector<PackedObjectContainer*> population;
    PackedObjectContainer* topV = new PackedObjectContainer();
public:
//    QProgressBar* progressBar;
    explicit PackedLib(QObject *parent = 0);

    PackedLib(int W, int H){
        this->W = W;
        this->H = H;
    }

    bool inline isOut(QPolygon &p){
        for(QPoint i : p){
//            if (i.x() < 0 || i.x() > W) return 1;
            if (i.y() < 0 || i.y() > H) return 1;
        }
        return 0;
    }


    // Fit func
    void fit(PackedObjectContainer *v){
        PackedObjectContainer *newV = new PackedObjectContainer;
        std::set<std::pair<int, int>> q;
        q.insert({0, 0});

        for(int k = 0; k < v->size(); ++k){
            bool flag = 0;
            for(auto to : q){
                int i = to.first;
                int j = to.second;

                PackedObject p = v->at(k);

                p.polygon.translate(-p.polygon[0].x(), -p.polygon[0].y());
                p.polygon.translate(i, j);

                if (!newV->contains(p.polygon) && !isOut(p.polygon)){
                    flag = 1;
                    newV->push_back(p);
                   // qInfo() << p.polygon.size() << " | SIIIZE";
                    for(auto i : p.polygon){
                        q.insert({i.x(), i.y()});
                        q.insert({i.x(), 0});
                    }

                    q.erase({i, j});

                    break;
                }

            }
            if (!flag) {
                v->fit = INT_MAX;
                return;
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
    // comporator
    static bool cmp(PackedObjectContainer* a, PackedObjectContainer* b){
        return (a->fit < b->fit);
    }


    //Gen Population
    void initPopulation(PackedObjectContainer* v, int n){
        if (v->size() == 1) return;
        QVector<PackedObjectContainer*> lpopulation;
        QVector<PackedObjectContainer*> rpopulation;
        for(int i = 0; i < n / 2; ++i){
            lpopulation.push_back(v->nextPopulation());


           // qInfo() << fitFumc(population[i]);
        }

        v = v->reverseObjects(H);

        for(int i = 0; i < n / 2; ++i){
            rpopulation.push_back(v->nextPopulation());

        }

        for(int i = 0; i < lpopulation.size(); ++i){
            int k = rand() % (lpopulation[0]->size() - 1);
            //qInfo() << k << lpopulation[0]->size() << " | KKKKKKKKKKKKKKK";
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

                //qInfo() << lpopulation[i]->at(index2in1).number << " <-> " << rpopulation[i]->at(index1in2).number;

                Temp = lpopulation[i]->at(index2in1);
                lpopulation[i]->at(index2in1) = rpopulation[i]->at(index1in2);
                rpopulation[i]->at(index1in2) = Temp;


                //qInfo() << lpopulation[i]->at(index2in1).number << " <-> " << rpopulation[i]->at(index1in2).number;


            }
            population.push_back(lpopulation[i]);
            population.push_back(rpopulation[i]);
        }

        double ALL_OP = 250;
        double OP_COUNTER = 0;
        topV->fit = INT_MAX;
        int last = 999999;
        int k_kol = 0;
        for(int jj = 0; jj < ALL_OP; ++jj){

            OP_COUNTER++;
            for(int i = 0; i < 20; ++i){
                int os1 = rand() % (population.size() / 2 - 1);
                int os2 = (population.size() - rand() % (population.size() / 2 - 1)) - 1;

                int k = rand() % (population[0]->size() - 1);
               // qInfo() << jj << k << population[0]->size() << " | KKKKKKKKKKKKKKK";
                for(int j = 0; j < k; ++j){


                    int index2in1;
                    for(index2in1 = 0; index2in1 < lpopulation.size(); ++index2in1){
                       if (population[os2]->at(j).number == population[os2]->at(index2in1).number) break;
                    }

                    int index1in2;
                    for(index1in2 = 0; index1in2 < lpopulation.size(); ++index1in2){
                       if (population[os1]->at(j).number == population[os2]->at(index1in2).number) break;
                    }

                    PackedObject Temp = population[os1]->at(j);
                    population[os1]->at(j) = (population[os2]->at(j));
                    population[os2]->at(j) = (Temp);

                    Temp = population[os1]->at(index2in1);
                    population[os1]->at(index2in1) = population[os2]->at(index1in2);
                    population[os2]->at(index1in2) = Temp;

                }
            }

            // Parallel foreach \ using all cores
            std::for_each(
                std::execution::par_unseq,
                population.begin(),
                population.end(),
                [&](auto i)
                {
                    fit(i);

                });


            std::sort(population.begin(), population.end(), cmp);

            if (topV->fit > population[0]->fit){
                topV->clear();
                for(auto i : *population[0]){
                    topV->push_back(i);

                }

                topV->fit = population[0]->fit;
            }


            if (last >= topV->fit){
                k_kol++;
            }else{
                last = topV -> fit;
                k_kol = 0;
            }



            updateValue(OP_COUNTER / ALL_OP * 100);
            setValue(OP_COUNTER / ALL_OP * 100);

            if (k_kol > 20) {
                updateValue(100);
                setValue(100);

                break;
            }

        }


        taskCompleted();
    }

    PackedObjectContainer* Top(){
       // qInfo() << topV->fit << " KKK";
        return topV;
    }
    int pgrs = 0;
    void updateValue(int value){
        pgrs = value;
    }
    signals:
        void setValue(int);
        void taskCompleted();

public slots:


};



#endif // OBJECTI_H
