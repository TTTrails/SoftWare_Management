#ifndef IMAGELAYERMANAGER_H
#define IMAGELAYERMANAGER_H
#include"ImageLayer.h"
#include"ImageLayerFactory.h"
#include<QDebug>
#include<initializer_list>
#include<qstring.h>
class ImageLayerManager{
private:
    QVector<ImageLayer*> *layers;

    ImageLayerManager(){
        layers=new QVector<ImageLayer*>();
    }
    ~ImageLayerManager(){
        for(int i=0;i<layers->size();i++){
            delete layers->at(i);
        }
        delete layers;
    };
public:
    static ImageLayerManager& getImageLayerManager(){
        static ImageLayerManager imageLayerManager;
        return imageLayerManager;
    }
    void sortLayers();
    QImage BlendAllLayers();
    ImageLayer* getLayer(QString LayerName);
    ImageLayer* getLayer(int priority);
    void AddLayer(ImageLayer* layer);
    ImageLayer* AddLayer(QImage img);
    void RemoveLayer(int priority);
    void RemoveLayer(QString Layername);
    bool isEmpty();
    void DebugOut();


};
#endif // IMAGELAYERMANAGER_H
