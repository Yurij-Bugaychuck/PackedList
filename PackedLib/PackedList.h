#ifndef PACKEDLISTITEM_H
#define PACKEDLISTITEM_H
#include<QPixmap>
#include<PackedLib/PackedObject.h>
#include<QListWidget>
class PackedListItem : public QListWidgetItem
{
public:
    QPixmap *poly;
    QString Name = "Ok";
    int Width = 50;
    int Height = 50;
    int Count;
    bool isPoly = 0;
    int msh = 0;
    QPolygon polygon;

    PackedListItem(){

    }
    PackedListItem(QString name, int w, int h, int count){
        Name = name;
        Width = w;
        Height = h;
        Count = count;
        polygon.putPoints(0, 4, 0, 0, 0, h, w, h, w, 0);
    }
    PackedListItem(QString name, int count, QPolygon p){
        Name = name;
        Count = count;
        polygon = p;
        isPoly = 1;
    }
    void updatePolygon(){
        polygon.clear();
        polygon.putPoints(0, 4, 0, 0, 0, Height, Width, Height, Width, 0);
    }
    PackedListItem(PackedListItem* it){
        Name = it->Name;
        Width = it->Width;
        Height = it->Height;
        Count = it->Count;
        polygon = it->polygon;
        isPoly = it->isPoly;
        msh = it->msh;
    }
    PackedListItem& operator=(PackedListItem &it){
        Name = it.Name;
        Width = it.Width;
        Height = it.Height;
        Count = it.Count;
        polygon = it.polygon;
        isPoly = it.isPoly;
        msh = it.msh;

        return *this;
    }
};

#endif // PACKEDLISTITEM_H
