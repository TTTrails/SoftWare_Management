#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDialog>
#include <QFileDialog>
#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include <QLabel>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)   
    , ui(new Ui::MainWindow)
    , isPicOpen(false)
{
    ui->setupUi(this);


    ui->label->installEventFilter(this);//在label‘中添加事件过滤器
    layerViewModel=new ImageLayerObserver();
    connect(this,&MainWindow::AddNewLayer,layerViewModel,&ImageLayerObserver::OnAddLayer);
    connect(layerViewModel,&ImageLayerObserver::NewLayer,this,&MainWindow::AddItemInList);
    connect(this,&MainWindow::listUpdate,layerViewModel,&ImageLayerObserver::OnIndexUpdate);
    connect(ui->up,&QAction::triggered,this,&MainWindow::UplayerItem);
    connect(ui->down,&QAction::triggered,this,&MainWindow::DownlayerItem);
    connect(ui->delete_2,&QAction::triggered,this,&MainWindow::RemoveLayer);


}


MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)   //用过滤器eventFilter（）拦截QLabel中的QEvent::Paint事件
{
    if(watched ==ui->label && event->type() == QEvent::Paint)
        paint();

    return QWidget::eventFilter(watched,event);
}

void MainWindow::paint(){//重新构建paint函数
    ShowBlendingImage();
//    //1 创建画家对象
//    QPainter painter(this);
//    //2 获取绘图所在矩形区域，当前是在frame窗口

//    QRect rect = ui->frame->frameRect();



//    //坐标值平移，让rect和painter使用相同的坐标系,在这个ui设计中会有很多的问题
//    rect.translate(ui->centralwidget->pos()+ui->frame->pos());


//    //3 构建要绘制的图形对象
//    QImage image(filePath);
//    if(image.isNull())
//        qDebug()<<"open failed";
//    else{
//        isPicOpen=true;
//    }
//    //4 使用painter将image图片画到rect
//    painter.drawImage(rect,image);


}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    ui->label->repaint();
}

void MainWindow::UplayerItem()
{
    int index1 = ui->listWidget->currentRow();
    if(index1 == 0)
        return;
    else
    {
        QListWidgetItem *item =CopyItem(ui->listWidget->currentItem(),index1-1);
        ui->listWidget->takeItem(index1+1);
        ui->listWidget->setCurrentRow(index1);
        ui->listWidget->show();
        emit listUpdate(ui->listWidget);
    }

}

void MainWindow::DownlayerItem()
{
    int index1 = ui->listWidget->currentRow();
    if(index1 == (ui->listWidget->count()-1))
        return;
    else
    {
        QListWidgetItem *item =CopyItem(ui->listWidget->currentItem(),index1+2);
        ui->listWidget->takeItem(index1);
        ui->listWidget->setCurrentRow(index1);
        ui->listWidget->show();
        emit listUpdate(ui->listWidget);
    }

}

void MainWindow::ShowBlendingImage()
{
    //1 创建画家对象
    QPainter painter(ui->label);
    //2 获取绘图所在矩形区域，当前是在label窗口
    ui->label->setPixmap(QPixmap::fromImage(ImageLayerManager::getImageLayerManager().BlendAllLayers()));//在label中显示图片,实际功能是重设label的大小让label出现滚动条,
    //QRect rect = ui->label->frameRect();



    //坐标值平移，让rect和painter使用相同的坐标系,在这个ui设计中会有很多的问题
    //rect.translate(ui->centralwidget->pos()+ui->label->pos());


    //4 使用painter将image图片画到label
    painter.drawPixmap(0,0,QPixmap::fromImage(ImageLayerManager::getImageLayerManager().BlendAllLayers()));
    ui->label->resize(1920,1080);
}

void MainWindow::AddItemInList(ImageLayer* layer)
{
    QListWidget *LayerList=ui->listWidget;
    QListWidgetItem *item=new QListWidgetItem(LayerList,0);

    item->setSizeHint(QSize(100,100));
    QWidget *w = new QWidget(LayerList);
    QHBoxLayout *layout=new QHBoxLayout(w);
    QLabel *label=new QLabel(w);
    label->setText(layer->getLayerName());
    QLabel *img=new QLabel(w);
    img->clear();
    QPixmap picture=QPixmap::fromImage(layer->getMainImage());
    img->setScaledContents(true);
    img->setPixmap(picture);
    picture.scaled(img->size(), Qt::KeepAspectRatio);
    layout->addWidget(img);
    layout->addWidget(label);
    w->setLayout(layout);
    w->show();
    LayerList->setItemWidget(item,w);
    LayerList->show();
    emit listUpdate(ui->listWidget);
}
void MainWindow::RemoveLayer()
{
    int row = ui->listWidget->currentRow();
    layerViewModel->OnRemoveLayer(row);
    ui->listWidget->takeItem(row);
    emit listUpdate(ui->listWidget);
}

QListWidgetItem* MainWindow::CopyItem(QListWidgetItem *layer,int index)
{
    QString name;
    QPixmap image;
    QWidget* wig=ui->listWidget->itemWidget(layer);
    if(wig!=nullptr){
        QList<QLabel*> labelList=wig->findChildren<QLabel*>();
        foreach(QLabel *label,labelList){
            if(label->pixmap().isNull()){
                name=label->text();
            }
            else{
                image=label->pixmap();
            }
        }
    }
    QListWidgetItem *item=new QListWidgetItem();
    ui->listWidget->insertItem(index,item);
    item->setSizeHint(QSize(100,100));
    QWidget *w = new QWidget(ui->listWidget);
    QHBoxLayout *layout=new QHBoxLayout(w);
    QLabel *label=new QLabel(w);
    label->setText(name);
    QLabel *img=new QLabel(w);
    img->clear();
    img->setScaledContents(true);
    img->setPixmap(image);
    image.scaled(img->size(), Qt::KeepAspectRatio);
    layout->addWidget(img);
    layout->addWidget(label);
    w->setLayout(layout);
    w->show();
    ui->listWidget->setItemWidget(item,w);
    return item;
}




void MainWindow::on_action_openPic_triggered()
{
    QString path = QFileDialog::getOpenFileName(this,"打开图片","C:\\");
    qDebug()<<path;
    filePath=path;
    emit AddNewLayer(QImage(filePath));
}

void MainWindow::on_action_grey_to_binary_triggered()
{
    if(isPicOpen){
        QDialog* dlg=new QDialog(this);
        dlg->resize(800,600);
        dlg->show();
        dlg->setAttribute(Qt::WA_DeleteOnClose);



    }else {
        //图片都没打开处理个屁
        QMessageBox::critical(this,"error","图片没打开");
    }


}
