#ifndef IMAGELAYER_H
#define IMAGELAYER_H
#include<QString>
#include<QImage>
class ImageLayer{
private:
    int priority;
    QImage MainImage;
    QString LayerName;
public:

    ImageLayer(int pri,QImage img,QString name):priority(pri),MainImage(img),LayerName(name){}
    int getPriority();
    void setPriority(int value);
    QImage getMainImage();
    void setMainImage(QImage &value);
    QString getLayerName();
    void setLayerName(QString &value);
};

#endif // IMAGELAYER_H
