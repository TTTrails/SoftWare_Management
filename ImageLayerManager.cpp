#include"ImageLayerManager.h"
void ImageLayerManager::sortLayers(){
    ImageLayer* temp;
    for (int i = 0; i < layers->size()-1; i++) {
        for (int j = 0; j < layers->size() -1- i; j++) {
            if ((layers->at(j)->getPriority() > layers->at(j + 1)->getPriority())) {
                temp = (*layers)[j];
                (*layers)[j] = (*layers)[j+1];
                (*layers)[j+1] = temp;
            }
        }
    }
}
QImage ImageLayerManager::BlendAllLayers(){
    if(isEmpty())
        return QImage();

    QImage oldImg1,oldImg2;
    oldImg1=layers->at(0)->getMainImage();
    if(layers->size()==1){
        return oldImg1;
    }
    else{
        sortLayers();
        qDebug()<<layers->at(0)->getLayerName();
        qDebug()<<layers->at(0)->getMainImage();
        return oldImg1;
//        for(int i=1;i<layers->size();i++){
//            QImage oldImg2=layers->at(i)->getMainImage();
//            QImage *newImage = new QImage(oldImg1.width(), oldImg1.height(), QImage::Format_ARGB32);
//            newImage->fill(0);
//            QColor oldColor1,oldColor2;
//            int r,g,b;
////混合图层方式
//            for(int x=0; x<newImage->width(); x++){
//                for(int y=0; y<newImage->height(); y++){
//                    oldColor1 = QColor(oldImg1.pixel(x,y));
//                    oldColor2 = QColor(oldImg2.pixel(x,y));
//                    r = (oldColor1.red() +  oldColor2.red())\2;
//                    g = (oldColor1.green() +  oldColor2.green())\2;
//                    b = (oldColor1.blue() +  oldColor2.blue())\2;

//                    //we check if the new values are between 0 and 255

//                    r = qBound(0, r, 255);
//                    g = qBound(0, g, 255);
//                    b = qBound(0, g, 255);

//                    newImage->setPixel(x,y, qRgb(r,g,b));
//              }
//            }
//            oldImg1=*newImage;
//       }
//        qDebug()<<layers->at(0)->getLayerName();
//        return oldImg1;
    }
}
ImageLayer* ImageLayerManager::getLayer(QString LayerName)
{
    if(isEmpty())
        return NULL;
    for(auto layer=layers->begin();layer!=layers->end();layer++)
        if((*layer)->getLayerName()==LayerName){
            return (*layer);
        }
    return NULL;
}

ImageLayer* ImageLayerManager::getLayer(int priority)
{
    if(isEmpty())
        return NULL;
    for(auto layer=layers->begin();layer!=layers->end();layer++)
        if((*layer)->getPriority()==priority){
            return (*layer);
        }
    return NULL;
}


void ImageLayerManager::AddLayer(ImageLayer* layer)
{
    layers->append(layer);
}
ImageLayer* ImageLayerManager::AddLayer(QImage img)
{
    ImageLayer* newLayer=ImageLayerFactory::getImageLayerFactory().ProductImageLayer(img);
    layers->append(newLayer);
    return newLayer;
}
void ImageLayerManager::RemoveLayer(int priority)
{
    if(isEmpty())
        return;
    for(auto i=layers->begin();i!=layers->end();i++ ){
        if((*i)->getPriority()==priority){
            layers->erase(i);
        }
    }
}

void ImageLayerManager::RemoveLayer(QString Layername)
{
    if(isEmpty())
        return;
    for(auto i=layers->begin();i!=layers->end();i++ ){
        if((*i)->getLayerName()==Layername){
            layers->erase(i);
        }
    }
}

bool ImageLayerManager::isEmpty()
{
    if(layers->empty())
        return true;
    else
        return false;
}

void ImageLayerManager::DebugOut()
{
    if(isEmpty())
        qDebug()<<"layer is empty";
    for(auto i=layers->begin();i!=layers->end();i++ )
        qDebug()<<(*i)->getLayerName()<<(*i)->getPriority();
}

