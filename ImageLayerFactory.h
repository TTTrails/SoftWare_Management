#ifndef IMAGELAYERFACTORY_H
#define IMAGELAYERFACTORY_H
#include"ImageLayer.h"
class ImageLayerFactory{
private:

    ImageLayerFactory(){}
    ~ImageLayerFactory(){};
public:
    static ImageLayerFactory& getImageLayerFactory(){
        static ImageLayerFactory imageLayerFactory;
        return imageLayerFactory;
    }
    static ImageLayer* ProductImageLayer(QImage image){
        static int num=0;
        static int prio=0;
        return new ImageLayer(prio++,image,"图层"+QString::number(num++));
    }
};

#endif // IMAGELAYERFACTORY_H
