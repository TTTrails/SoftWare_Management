#include"ImageLayer.h"

QImage ImageLayer::getMainImage()
{
    return MainImage;
}

void ImageLayer::setMainImage(QImage &value)
{
    MainImage = value;
}

QString ImageLayer::getLayerName()
{
    return LayerName==""?NULL:LayerName;
}

void ImageLayer::setLayerName(QString &value)
{
    LayerName = value;
}

int ImageLayer::getPriority()
{
    return priority<0?-1:priority;
}

void ImageLayer::setPriority(int value)
{
    priority = value;
}
