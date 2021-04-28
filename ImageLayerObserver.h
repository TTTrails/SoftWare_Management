#ifndef IMAGELAYEROBSERVER_H
#define IMAGELAYEROBSERVER_H
#include"ImageLayer.h"
#include"ImageLayerManager.h"
#include<QListWidget>
#include<QHBoxLayout>
#include<QLabel>
class ImageLayerObserver:public QObject{
    Q_OBJECT

public:
    void OnIndexUpdate(QListWidget* LayerList);
    void OnAddLayer(QImage img);
    void OnRemoveLayer(int priority);
signals:
    void NewLayer(ImageLayer* layer);
};

#endif // IMAGELAYEROBSERVER_H
