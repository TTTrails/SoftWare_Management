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


  
    layerViewModel=new ImageLayerObserver();
      
    //myPaint=new MyPaint(ui->frame);
    myPaint=new MyPaint(ui->centralwidget);
    //myPaint->stackUnder(ui->frame);
    //这都是写死了的，实在不明智
    //myPaint->setGeometry(QRect(-1, -1, 511, 561));
    myPaint->setGeometry(QRect(9, 79, 511, 561));

    //myPaint->setFrameShape(QFrame::Box);
    //myPaint->setFrameShadow(QFrame::Sunken);


    rMenu = new QMenu(this);//创建右键菜单
    //rMenu->addAction(tr("保存  \tCtrl+S"), this, SLOT(SavePic()));//添加菜单动作
    rMenu->addAction(tr("退出  \tALT+F4"), this, SLOT(close()));//添加菜单动作
    rMenu->setStyleSheet("background-color:rgb(199,237,204)");//设置背景色


    //连接信号与槽函数
    connect(ui->action_draw_pencil, SIGNAL(triggered()), myPaint, SLOT(Lines()));
    connect(ui->action_draw_rectangle, SIGNAL(triggered()), myPaint, SLOT(Rects()));
    connect(ui->action_draw_ellipse, SIGNAL(triggered()), myPaint, SLOT(Ellipses()));
    connect(ui->action_draw_line, SIGNAL(triggered()), myPaint, SLOT(Line()));
    connect(ui->action_draw_text, SIGNAL(triggered()), myPaint, SLOT(Texts()));
      
    //zyk的信号与槽连接  函数指针写法
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

void MainWindow::contextMenuEvent(QContextMenuEvent *)  //右键菜单事件
{
    rMenu->exec(cursor().pos());//在光标位置弹出菜单
}

//int MainWindow::YUVConvertToRGB(){
//    unsigned char* data =curImage.bits();
//    unsigned char* RGBData;
//    int width=curImage.width();
//    int height=curImage.height();
//    int bytePerLine=(width*24+31)/8;
//    //存储处理后的数据
//    RGBData=new unsigned char [bytePerLine*height];
//    //unsigned char r,g,b,y;

//    for(int i=0;i<height;i++)
//    {
//        for(int j=0;j<width;j++)
//        {

//        }
//    }
//    QImage RGBImage(RGBData,width,height,bytePerLine,QImage::Format_RGB888);
//    curImage=RGBImage;

//    return 0;
//}

int MainWindow::RGBConvertToYUV(){
    unsigned char* data =curImage.bits();
    unsigned char* YUVData;
    int width=curImage.width();
    int height=curImage.height();
    int bytePerLine=(width*24+31)/8;
    //存储处理后的数据
    YUVData=new unsigned char [bytePerLine*height];
    unsigned char r,g,b,y;

    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            r=*(data+2);
            g=*(data+1);
            b=*data;

            y=YUVData[i * bytePerLine + j * 3]= 0.3*r + 0.59*g + 0.11*b;
            YUVData[i*bytePerLine+j*3+1]=(0.87*(0.493*((int)b - y)) + 127);
            YUVData[i*bytePerLine+j*3+2]=(0.8*(0.877*((int)r - y)) + 127);

            data+=4;

        }
    }
    QImage YUVImage(YUVData,width,height,bytePerLine,QImage::Format_RGB888);

    curImage=YUVImage;

    return 0;
}


void MainWindow::paintEvent(QPaintEvent *){

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
    ui->frame->repaint();
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


//    QPixmap pix;
//    pix.load(filePath);//加载图片
//    QPainter painter(&myPaint->_pixmap);

    //1 创建画家对象
    //QPainter painter(this);
    QPainter painter(myPaint);
    //2 获取绘图所在矩形区域，当前是在frame窗口

    //QRect rect = ui->frame->frameRect();
    QRect rect=myPaint->rect();
    //坐标值平移，让rect和painter使用相同的坐标系,在这个ui设计中会有很多的问题
    //rect.translate(ui->centralwidget->pos()+ui->frame->pos());


    //使用painter将image图片画到rect
    //painter.drawImage(rect,curImage);


    //4 使用painter将image图片画到rect
    painter.drawImage(rect,ImageLayerManager::getImageLayerManager().BlendAllLayers());
}


void MainWindow::on_action_openPic_triggered()
{
    QString path = QFileDialog::getOpenFileName(this,"打开图片","C:\\");
    if(path.isEmpty())
    {
        QMessageBox::warning(this,"未选择文件","请选择图片！");
        return;
     }
    //qDebug()<<path;
    filePath=path;
    //构建要绘制的图形对象
    //QImage* image=new QImage(filePath);
    QImage image(filePath);
    if(image.isNull())
        qDebug()<<"open failed";
    else{
        isPicOpen=true;
        curImage=image;
    }
  
    //zyk
    emit AddNewLayer(QImage(filePath));
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






void MainWindow::on_action_grey_to_binary_triggered()
{
    if(isPicOpen){

        int width = curImage.width();
        int height = curImage.height();
        int bytePerLine = curImage.bytesPerLine();    // 每一行的字节数
        unsigned char *data = curImage.bits();
        unsigned char *binarydata = new unsigned char[bytePerLine * height];

        unsigned char g = 0;
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                g = *(data + i * bytePerLine + j*3);
                if(int(g) >= 128)
                {
                    binarydata[ i * bytePerLine + j] = 0xFF;
                }
                else
                {
                    binarydata[ i * bytePerLine + j] = 0x00;
                }
            }
        }
        QImage binaryImage(binarydata, width, height, bytePerLine, QImage::Format_Grayscale8);
        curImage=binaryImage;
        update();



    }else {

        QMessageBox::critical(this,"error","图片没打开");
    }







void MainWindow::on_action_color_to_grey_triggered()
{
    if(isPicOpen){
        //QPixmap pixmap(QPixmap::fromImage(curImage));
        unsigned char* data =curImage.bits();
        unsigned char* greyData;
        int width=curImage.width();
        int height=curImage.height();
        int bytePerLine=(width*24+31)/8;
        //存储处理后的数据
        greyData=new unsigned char [bytePerLine*height];
        unsigned char r,g,b;

        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                r=*(data+2);
                g=*(data+1);
                b=*data;

                greyData[i * bytePerLine + j * 3] = (r * 30 + g*59 +b*11)/100;
                greyData[i*bytePerLine+j*3+1]=(r*30+g*59+b*11)/100;
                greyData[i*bytePerLine+j*3+2]=(r*30+g*59+b*11)/100;

                data+=4;

            }
        }
        QImage greyImage(greyData,width,height,bytePerLine,QImage::Format_RGB888);
        //QPixmap pixmap2(QPixmap::fromImage (greyImage));
        curImage=greyImage;
        update();
    }else {

        QMessageBox::critical(this,"error","图片没打开");
    }
}
void MainWindow::on_action_inverse_color_triggered()
{
    if(isPicOpen){

        unsigned char* data =curImage.bits();

        int width=curImage.width();
        int height=curImage.height();
        int bytePerLine=(width*24+31)/8;
        //存储处理后的数据
        unsigned char* inverseData=new unsigned char [bytePerLine*height];

        unsigned char r,g,b;

        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                r=*(data+2);
                g=*(data+1);
                b=*data;

                inverseData[i*bytePerLine+j*3]=255-(int)b;
                inverseData[i*bytePerLine+j*3+1]=255-int(g);
                inverseData[i*bytePerLine+j*3+2]=255-int(r);

                data+=4;

            }
        }
        QImage inverseImage(inverseData,width,height,bytePerLine,QImage::Format_RGB888);
        curImage=inverseImage;
        update();
    }else {

        QMessageBox::critical(this,"error","图片没打开");
    }
}
void MainWindow::on_action_histogram_equalization_triggered()
{
    RGBConvertToYUV();
    update();
}


//画图方面咯
void MainWindow::on_action_draw_pencil_triggered()
{

}

void MainWindow::on_action_draw_line_triggered()
{

}

void MainWindow::on_action_draw_ellipse_triggered()
{

}

void MainWindow::on_action_draw_rectangle_triggered()
{

}

void MainWindow::on_action_draw_text_triggered()
{

}


