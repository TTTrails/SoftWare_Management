#include"ImageLayerObserver.h"

void ImageLayerObserver::OnAddLayer(QImage img){
    emit NewLayer(ImageLayerManager::getImageLayerManager().AddLayer(img));
    ImageLayerManager::getImageLayerManager().DebugOut();
}
void ImageLayerObserver::OnIndexUpdate(QListWidget* LayerList){
    int row=0;
    while(row<(LayerList->count()))
    {
        QListWidgetItem* item=LayerList->item(row);
        QWidget* wig=LayerList->itemWidget(item);
        if(wig!=nullptr){
            QList<QLabel*> labelList=wig->findChildren<QLabel*>();
            foreach(QLabel *label,labelList){
                if(label->pixmap().isNull()){
                    QString name=label->text();
                    int oldPri=ImageLayerManager::getImageLayerManager().getLayer(name)->getPriority();
                    if(oldPri!=row){
                        ImageLayerManager::getImageLayerManager().getLayer(name)->setPriority(row);
                    }

                }
            }
        }
        row++;
    }
    ImageLayerManager::getImageLayerManager().DebugOut();

}
void ImageLayerObserver::OnRemoveLayer(int priority){
    ImageLayerManager::getImageLayerManager().RemoveLayer(priority);
    ImageLayerManager::getImageLayerManager().DebugOut();
}
