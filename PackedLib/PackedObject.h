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
        if (!v) return;

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
        if (a > b) std::swap(a, b);
        //qInfo() << a << b << v->size();
        int fi = -1;
        for(int i =  a; i <= b; ++i){
            //qInfo()<< i << v->at(i).polygon;
            for(QPoint j : v->at(i).polygon){
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

        for(auto itp : *v){
            bool k = false;
            for(auto j : itp.polygon){
                if (j.x() > H){
                    k = true;
                    break;
                }
            }
            QPolygon p = itp.polygon;
            if (!k) {
                QTransform matrix;
                matrix.rotate(90);
//                matrix.rotate(180, Qt::XAxis);
                p = matrix.map(p);
            }

            int mnx = 10000;
            int mny = 10000;
            for(auto i: p){
                mnx = std::min(mnx, i.x());
                mny = std::min(mny, i.y());
            }

            QPolygon tp;

            int inp;

            for(inp = 0; inp < p.size(); ++inp){
                if (p[inp].x() == mnx) break;
            }

            for(int i  = inp; i < p.size(); ++i){
                tp.push_back(p[i]);
            }

            for(int i = 0; i < inp; ++i){
                tp.push_back(p[i]);
            }

            p = tp;



            p.translate(-mnx, -mny);
           // qInfo() << mnx << mny << "MNSS";

            mnx = 10000;
            mny = 10000;

           // qInfo() << "next";

            newV->push_back(PackedObject(p, itp.number, itp.Name, itp.Color));



        }

        return newV;
    }

    PackedObjectContainer* reverseObjectsH(int H){
        PackedObjectContainer *newV = new PackedObjectContainer;

        for(auto itp : *v){
            bool k = false;
            for(auto j : itp.polygon){
                if (j.x() > H){
                    k = true;
                    break;
                }
            }
            QPolygon p = itp.polygon;
            if (!k) {
                QTransform matrix;
                matrix.rotate(180, Qt::YAxis);
                p = matrix.map(p);
            }
            int mnx = 10000;
            int mny = 10000;
            for(auto i: p){
                mnx = std::min(mnx, i.x());
                mny = std::min(mny, i.y());
            }

            QPolygon tp;

            int inp;

            for(inp = 0; inp < p.size(); ++inp){
                if (p[inp].x() == mnx) break;
            }

            for(int i  = inp; i < p.size(); ++i){
                tp.push_back(p[i]);
            }

            for(int i = 0; i < inp; ++i){
                tp.push_back(p[i]);
            }

            p = tp;



            p.translate(-mnx, -mny);
          //  qInfo() << mnx << mny << "MNSS";

            mnx = 10000;
            mny = 10000;

          //  qInfo() << "next";

            newV->push_back(PackedObject(p, itp.number, itp.Name, itp.Color));

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
            if (i.x() < 0) return 1;
            if (i.y() < 0 || i.y() > H) return 1;
        }
        return 0;
    }

    ~PackedLib(){
        delete topV;
        population.clear();
    }


    // Fit func
public:
    void fit(PackedObjectContainer *v){
        PackedObjectContainer *newV = new PackedObjectContainer;
        std::set<std::pair<int, int>> q;
        q.insert({0, 0});
        for(int i = 1; i < H; ++i){
            q.insert({0, i});
        }

//        for(int i = 1; i < 50; ++i){
//            q.insert({0, i});
//        }

        for(int k = 0; k < v->size(); ++k){
            bool flag = 0;
            for(auto to : q){
                int i = to.first;
                int j = to.second;

                PackedObject p = v->at(k);

                int mnx = 99999;
                int mny = 99999;
                for(auto i : p.polygon){
                    mnx = std::min(mnx, i.x());
                    mny = std::min(mny, i.y());
                }
                p.polygon.translate(-mnx, -mny);
                p.polygon.translate(i, j);

                if (!newV->contains(p.polygon) && !isOut(p.polygon)){
                    flag = 1;
                    newV->push_back(p);
                   // qInfo() << p.polygon.size() << " | SIIIZE";
//                    for(auto i : p.polygon){

//                        q.insert({i.x(), i.y()});
//                        q.insert({i.x(), 0});
//                    }

                    for(int i = 0; i < p.polygon.size() - 1; ++i){
                        int x1 = p.polygon[i].x();
                        int x2 = p.polygon[i + 1].x();
                        if (x1 > x2) std::swap(x1,x2);

                        int y1 = p.polygon[i].y();
                        int y2 = p.polygon[i + 1].y();
                        if (y1 > y2) std::swap(y1, y2);

                        if (x1 == x2){
                            for(int ii = y1; ii <= y2; ii += 10){
                                q.insert({x1, ii});

                            }
                             q.insert({x1, 0});
                        }else{
                            for(int ii = x1; ii <= x2; ii += 10){
                                q.insert({ii, y1});
                                q.insert({ii, 0});
                            }
                        }
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
        n  = 20;
        if (v->size() == 1) return;

        n /= 2;
        population.push_back(v);
        for(int i = 0; i < n; ++i){
            population.push_back(v->nextPopulation());
           // qInfo() << fitFumc(population[i]);
        }

        v = v->reverseObjects(H);
        population.push_back(v);
        for(int i = 0; i < n; ++i){
            population.push_back(v->nextPopulation());
        }


//        QVector<PackedObjectContainer*> population2;
//        QVector<PackedObjectContainer*> population3;

        v = v->reverseObjects(H);
        population.push_back(v);
        for(int i = 0; i < n ; ++i){
            population.push_back(v->nextPopulation());
        }

        v = v->reverseObjects(H);
        population.push_back(v);

        for(int i = 0; i < n ; ++i){
            population.push_back(v->nextPopulation());
        }

        v = v->reverseObjects(H);

        v = v->reverseObjectsH(H);
        population.push_back(v);

        for(int i = 0; i < n ; ++i){
            population.push_back(v->nextPopulation());
        }

        v = v->reverseObjects(H);
        population.push_back(v);

        for(int i = 0; i < n ; ++i){
            population.push_back(v->nextPopulation());
        }

        v = v->reverseObjects(H);
        population.push_back(v);
        for(int i = 0; i < n ; ++i){
            population.push_back(v->nextPopulation());
        }

        v = v->reverseObjects(H);
        population.push_back(v);

        for(int i = 0; i < n ; ++i){
            population.push_back(v->nextPopulation());
        }


        //qInfo () << population.size() << " Heheh";
        int sz = population.size();
        for(int i = 0; i < sz; ++i){
            int os1 = rand() % population.size();
            int os2 = rand() % population.size();
            PackedObjectContainer* first_parent = new PackedObjectContainer();
            PackedObjectContainer* second_parent = new PackedObjectContainer();

            for(auto j : *population[os1]){
                first_parent->push_back(j);
            }

            for(auto j : *population[os2]){
                second_parent->push_back(j);
            }
            //qInfo() << "Stage 1";


            int k = rand() % (population[os1]->size());
            for(int j = 0; j < k; ++j){
                int index2in1;
                for(index2in1 = 0; index2in1 < second_parent->size(); ++index2in1){
                   if (second_parent->at(j).number == second_parent->at(index2in1).number) break;
                }

                int index1in2;
                for(index1in2 = 0; index1in2 < first_parent->size(); ++index1in2){
                   if (first_parent->at(j).number == second_parent->at(index1in2).number) break;
                }

                PackedObject Temp = first_parent->at(j);
                first_parent->at(j) = (second_parent->at(j));
                second_parent->at(j) = (Temp);

                Temp = first_parent->at(index2in1);
                first_parent->at(index2in1) = second_parent->at(index1in2);
                second_parent->at(index1in2) = Temp;

            }
            //qInfo() << "Stage 2";

            population.push_back(first_parent);
            population.push_back(second_parent);

        }




         qInfo() << "Start";


        double ALL_OP = 500;
        double OP_COUNTER = 0;
        topV->fit = INT_MAX;
        int last = 999999;
        int k_kol = 0;

//        Parallel foreach \ using all cores
       std::for_each(
           std::execution::par_unseq,
           population.begin(),
           population.end(),
           [&](auto i)
           {
               fit(i);
           });


        for(int jj = 0; jj < ALL_OP; ++jj){

            OP_COUNTER++;





            std::sort(population.begin(), population.end(), cmp);



            topV = population[0];
            topV->fit = population[0]->fit;
            double avg = 0;
            QDebug dbg(QtDebugMsg);
            for(int i = 0; i < population.size(); ++i){
               // dbg << population[i]->fit &;
//                if (population[i]->fit % 10 != 0){
//                    topV = population[i];

//                    taskCompleted();
//                    return;
//                }
                avg += population[i]->fit;
            }
//            qInfo() << avg << "AVGD";
            avg /= static_cast<double>(population.size());

//            if (topV->fit > population[0]->fit){
//                topV->clear();
//                for(auto i : *population[0]){
//                    topV->push_back(i);

//                }

//                topV->fit = population[0]->fit;
////                last = topV->fit;
//            }

//            qInfo() << last << "LL";
            if (last == topV->fit){
                k_kol++;
            }else{
//                qInfo() <<" LOOOOL";
                last = topV -> fit;
                k_kol = 0;
            }


            if (k_kol > 50) {
                updateValue(100);
                setValue(100);

                break;
            }

            int OT = 20;
            for(int i = 0; i < OT; ++i){
                delete population.last();
                population.pop_back();
            }


            int os1 = rand() % population[0]->size();
            int os2 = rand() % population[0]->size();
            for(int i = 0; i < OT / 2; ++i){
                PackedObjectContainer* first_parent = new PackedObjectContainer();
                PackedObjectContainer* second_parent = new PackedObjectContainer();

                for(auto j : *population[os1]){
                    first_parent->push_back(j);
                }

                for(auto j : *population[os2]){
                    second_parent->push_back(j);
                }


                int k = rand() % (population[0]->size());
                for(int j = 0; j < k; ++j){
                    int index2in1;
                    for(index2in1 = 0; index2in1 < second_parent->size(); ++index2in1){
                       if (second_parent->at(j).number == second_parent->at(index2in1).number) break;
                    }

                    int index1in2;
                    for(index1in2 = 0; index1in2 < first_parent->size(); ++index1in2){
                       if (first_parent->at(j).number == second_parent->at(index1in2).number) break;
                    }

                    PackedObject Temp = first_parent->at(j);
                    first_parent->at(j) = (second_parent->at(j));
                    second_parent->at(j) = (Temp);

                    Temp = first_parent->at(index2in1);
                    first_parent->at(index2in1) = second_parent->at(index1in2);
                    second_parent->at(index1in2) = Temp;

                }


//                if (first_parent->fit == INT_MAX) {
//                    topV = first_parent;
//                    taskCompleted();
//                    return;
//                }

//                if (second_parent->fit == INT_MAX) {
//                    topV = second_parent;
//                    taskCompleted();
//                    return;
//                }

                int isMutation = rand() % 2;

                if (isMutation){
                    int isp = rand() % 2;
                    if (isp) first_parent = first_parent->nextPopulation();

                    int k = rand() % 5;
                    for(int jj = 0; jj < k; ++jj){
                       first_parent = first_parent->reverseObjects(H);
                    }
                    k = rand() % 2;
                    if(k){
                        first_parent = first_parent->reverseObjectsH(H);
                    }
                }

                isMutation = rand() % 2;

                if (isMutation){
                    int isp = rand() % 2;
                    if (isp) second_parent = second_parent->nextPopulation();

                    int k = rand() % 5;
                    for(int jj = 0; jj < k; ++jj){
                       second_parent = second_parent->reverseObjects(H);
                    }
                    k = rand() % 2;
                    if(k){
                        second_parent = second_parent->reverseObjectsH(H);
                    }
                }

                fit(first_parent);
                fit(second_parent);

//                qInfo() << first_parent->fit << " " << second_parent->fit << " | EEH";

                population.push_back(first_parent);
                population.push_back(second_parent);

            }





            updateValue(OP_COUNTER / ALL_OP * 100);
            setValue(OP_COUNTER / ALL_OP * 100);


            qInfo() << "NOW: " << topV->fit << " Cur k_kol: " << k_kol << " Pop size: " << population.size() << " AVG " << avg;

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
