#include "mainwindow.h"
#include "./ui_mainwindow.h"



enum DrawType{
    PENCIL_DRAW_TYPE=1,
    RECTANGLE_DRAW_TYPE,
    ELLIPSE_DRAW_TYPE,
    LINE_DRAW_TYPE,
    TEXT_DRAW_TYPE,
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)   
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    ui->label->installEventFilter(this);//在label中添加事件过滤器
    ui->scrollAreaWidgetContents->setMinimumSize(QSize(400, 400));
    //画图功能
    isPicOpen = false; //初始未打开图片
    lpress = false;//初始鼠标左键未按下
    drawType = 0;//初始为什么都不画
    drag = 0;//默认非拖拽模式
    begin = pos();//拖拽的参考坐标，方便计算位移
    tEdit = new QTextEdit(this);//初始化文本输入框
    tEdit->hide();//隐藏
    setMouseTracking(true);//开启鼠标实时追踪，监听鼠标移动事件，默认只有按下时才监听
  
    layerViewModel=new ImageLayerObserver();
      
    rMenu = new QMenu(this);//创建右键菜单
    //rMenu->addAction(tr("保存  \tCtrl+S"), this, SLOT(SavePic()));//添加菜单动作
    rMenu->addAction(tr("退出  \tALT+F4"), this, SLOT(close()));//添加菜单动作
    rMenu->setStyleSheet("background-color:rgb(199,237,204)");//设置背景色


    //zyk的信号与槽连接  函数指针写法
    connect(this,&MainWindow::AddNewLayer,layerViewModel,&ImageLayerObserver::OnAddLayer);
    connect(layerViewModel,&ImageLayerObserver::NewLayer,this,&MainWindow::AddItemInList);
    connect(this,&MainWindow::listUpdate,layerViewModel,&ImageLayerObserver::OnIndexUpdate);

      
}


MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)//用过滤器拦截QLabel中的QEvent::Paint事件
{
    if(watched==ui->label&&event->type()==QEvent::Paint)
        ShowBlendingImage();
    return  QWidget::eventFilter(watched,event);
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
}

void MainWindow::GenerateDrawImage(){


    //    if(openflag == 0)//不是打开图片的，每一次新建一个空白的画布
    //    {
    //        pixmap = QPixmap(size());//新建pixmap
    //        pixmap.fill(Qt::white);//背景色填充为白色
    //    }

        //QPixmap pix = pixmap;//以_pixmap作为画布

        QImage img(300,300,QImage::Format_RGB32);
        img.fill(Qt::white);
        QPainter painter(&img);

        painter.setPen(QPen(Qt::blue));

        unsigned int i1=0,i2=0,i3=0,i4=0,i5=0;//各种图形的索引

        for(int c = 0;c<shape.size();++c)//控制用户当前所绘图形总数
        {
            if(shape.at(c) == 1)//线条
            {
                  const QVector<QPoint>& line = lines.at(i1++);//取出一条线条
                  for(int j=0; j<line.size()-1; ++j)//将线条的所有线段描绘出
                  {
                      painter.drawLine(line.at(j), line.at(j+1));
                  }
            }
            else if(shape.at(c) == 2)//矩形
            {
                 painter.drawRect(rects.at(i2++));
            }
            else if(shape.at(c) == 3)//椭圆
            {
                painter.drawEllipse(ellipse.at(i3++));
            }
            else if(shape.at(c) == 4)//直线
            {
                painter.drawLine(line.at(i4).topLeft(),line.at(i4).bottomRight());
                i4++;
            }
            else if(shape.at(c) == 5)//文本
            {
                painter.drawText(tpoint.at(i5),text.at(i5));
                i5++;
            }
        }
        //p.end();
        //p.begin(this);//将当前窗体作为画布
        //p.drawPixmap(0,0, );//将pixmap画到窗体
        //p.drawImage(0,0,img);
        emit AddNewLayer(img);
}


void MainWindow::ShowBlendingImage()
{


//    QPixmap pix;
//    pix.load(filePath);//加载图片
//    QPainter painter(&myPaint->_pixmap);

    //1 创建画家对象
    //QPainter painter(this);
    //QPainter painter(ui->frame);

    //2 获取绘图所在矩形区域，当前是在frame窗口

    //QRect rect = ui->frame->frameRect();
    //QRect rect=ui->label->rect();
    //坐标值平移，让rect和painter使用相同的坐标系,在这个ui设计中会有很多的问题
    //rect.translate(ui->centralwidget->pos()+ui->label->pos());


    //使用painter将image图片画到rect
    //painter.drawImage(rect,curImage);


    //4 使用painter将image图片画到rect
    //painter.drawImage(rect,ImageLayerManager::getImageLayerManager().BlendAllLayers());

    QPainter painter(ui->label);
    //pixmap.scaled(QSize(ui->label->width(),ui->label->height()),Qt::KeepAspectRatioByExpanding);
    ui->label->setPixmap(QPixmap::fromImage(ImageLayerManager::getImageLayerManager().BlendAllLayers()));

    //painter.drawImage(0,0,ImageLayerManager::getImageLayerManager().BlendAllLayers());
    painter.drawPixmap(0,0,QPixmap::fromImage(ImageLayerManager::getImageLayerManager().BlendAllLayers()));
    //ui->label->resize(1920,1080);

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

    //QSize qSize(300,300);
    //QImage image();

    QImage image(filePath);

    //image=image.scaled(1920,1080,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    //qDebug()<<"Image Size:"<<image.size();
    if(image.isNull())
        qDebug()<<"open failed";
    else{
        isPicOpen=true;
        curImage=image;
    }

    //zyk
    emit AddNewLayer(QImage(filePath));
    update();
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
    drawType = PENCIL_DRAW_TYPE;//铅笔
    tEdit->hide();//文本框隐藏
}

void MainWindow::on_action_draw_line_triggered()
{
    drawType=LINE_DRAW_TYPE;//线
    tEdit->hide();//文本框隐藏

}

void MainWindow::on_action_draw_ellipse_triggered()
{
    drawType=ELLIPSE_DRAW_TYPE;
    tEdit->hide();
}

void MainWindow::on_action_draw_rectangle_triggered()
{
    drawType=RECTANGLE_DRAW_TYPE;
    tEdit->hide();

}

void MainWindow::on_action_draw_text_triggered()
{
    drawType=TEXT_DRAW_TYPE;
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)//当鼠标左键按下
    {
        if(drawType == 1)//线条(铅笔)
        {
            lpress = true;//左键按下标志
            QVector<QPoint> line;//鼠标按下，新的线条开始
            lines.append(line);//将新线条添加到线条集合
            QVector<QPoint>& lastLine = lines.last();//拿到新线条
            lastLine.append(e->pos());//记录鼠标的坐标(新线条的开始坐标)
            shape.append(1);
        }
        else if(drawType == 2)//矩形
        {
            lpress = true;//左键按下标志
            if(!drag)//非拖拽模式
            {
                QRect rect;//鼠标按下，矩形开始
                rects.append(rect);//将新矩形添加到矩形集合
                QRect& lastRect = rects.last();//拿到新矩形
                lastRect.setTopLeft(e->pos());//记录鼠标的坐标(新矩形的左上角坐标)
                 shape.append(2);
            }
            else if(rects.last().contains(e->pos()))//拖拽模式、如果在矩形内按下
            {
                begin = e->pos();//记录拖拽开始的坐标位置,方便计算位移

            }

        }
        else if(drawType == 3)//椭圆
        {
            lpress = true;//左键按下标志
            if(!drag)//非拖拽模式
            {
                QRect rect;//鼠标按下，椭圆开始
                ellipse.append(rect);//将新椭圆添加到椭圆集合
                QRect& lastEllipse = ellipse.last();//拿到新椭圆
                lastEllipse.setTopLeft(e->pos());//记录鼠标的坐标(新椭圆的左上角坐标)
                shape.append(3);
            }
            else if(ellipse.last().contains(e->pos()))//如果在椭圆内按下
            {
                begin = e->pos();//记录拖拽开始的坐标位置

            }
        }
        else if(drawType == 4)//直线
        {
            lpress = true;//左键按下标志
            QRect rect;//鼠标按下，直线一端开始
            line.append(rect);//将新直线添加到直线集合
            QRect& lastLine = line.last();//拿到新直线
            lastLine.setTopLeft(e->pos());//记录鼠标的坐标(新直线开始一端坐标)
            shape.append(4);
        }
        else if(drawType == 5)//文字
        {
            update();//触发窗体重绘
            QPoint p;//鼠标按下，文字开始
            tpoint.append(p);//将文字坐标添加到文字位置集合
            QPoint& lastp = tpoint.last();//拿到新文字
            lastp = e->pos();//记录鼠标的坐标
            tEdit->setGeometry(lastp.x()-6,lastp.y()-17,90,90);//设置文本框的位置、大小

            tEdit->show();//显示文本输入框
            text.append("");//添加文本到文本集合
            tEdit->clear();//因为全局只有一个，所以使用之前要清空
            shape.append(5);
        }

    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(drag && ((drawType == 2 && rects.last().contains(e->pos()))
            || (drawType == 3 && ellipse.last().contains(e->pos()) )))
    {
        setCursor(Qt::SizeAllCursor);//拖拽模式下，并且在拖拽图形中，将光标形状改为十字
    }
    else
    {
        setCursor(Qt::ArrowCursor);//恢复原始光标形状
        drag = 0;
    }

    if(lpress)
    {
        if(drawType == 1)//铅笔画线
        {
            if(lines.size()<=0) return;//线条集合为空，不画线
            QVector<QPoint>& lastLine = lines.last();//最后添加的线条，就是最新画的
            lastLine.append(e->pos());//记录鼠标的坐标(线条的轨迹)
            //update();//触发窗体重绘
        }
        else if(drawType == 2)
        {
            if(drag == 0)//非拖拽
            {
                QRect& lastRect = rects.last();//拿到新矩形
                lastRect.setBottomRight(e->pos());//更新矩形的右下角坐标
            }
            else//拖拽模式
            {
                QRect& lastRect = rects.last();//拿到最后添加的矩形
                if(lastRect.contains(e->pos()))//在矩形的内部
                {
                    int dx = e->pos().x()-begin.x();//横向移动x
                    int dy = e->pos().y()-begin.y();//纵向移动y
                    lastRect = lastRect.adjusted(dx,dy,dx,dy);//更新矩形的位置
                    begin = e->pos();//刷新拖拽点起始坐标
                }

            }
           // update();//触发窗体重绘

        }
        else if(drawType == 3)
        {
            if(drag == 0)//非拖拽
            {
                QRect& lastEllipse = ellipse.last();//拿到新椭圆
                lastEllipse.setBottomRight(e->pos());//更新椭圆的右下角坐标)

            }
            else//拖拽
            {
                QRect& lastEllipse = ellipse.last();//拿到最后添加的矩形
                if(lastEllipse.contains(e->pos()))//在椭圆内部
                {
                    int dx = e->pos().x()-begin.x();//横向移动x
                    int dy = e->pos().y()-begin.y();//纵向移动y
                    lastEllipse = lastEllipse.adjusted(dx,dy,dx,dy);
                    begin = e->pos();//刷新拖拽点起始坐标
                }

            }
            //update();//触发窗体重绘
        }
        else if(drawType == 4)
        {
            QRect& lastLine = line.last();//拿到新直线
            lastLine.setBottomRight(e->pos());//更新直线另一端)
            //update();//触发窗体重绘
        }
        //GenerateDrawImage();
    }


}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if(lpress)
    {
        if(drawType == 1)
        {
            QVector<QPoint>& lastLine = lines.last();//最后添加的线条，就是最新画的
            lastLine.append(e->pos());//记录线条的结束坐标
            lpress = false;//标志左键释放
        }
        else if(drawType == 2)
        {
            QRect& lastRect = rects.last();//拿到新矩形
            if(!drag)
            {
                lastRect.setBottomRight(e->pos());//不是拖拽时，更新矩形的右下角坐标)
                //刚画完矩形，将光标设置到新矩形的中心位置，并进入拖拽模式
                this->cursor().setPos(this->cursor().pos().x()-lastRect.width()/2,this->cursor().pos().y()-lastRect.height()/2);
                drag = 1;

            }
            lpress = false;

        }
        else if(drawType == 3)
        {
            QRect& lastEllipse = ellipse.last();//拿到新椭圆
            if(!drag)
            {
                lastEllipse.setBottomRight(e->pos());//不是拖拽时，更新椭圆的右下角坐标)
                //刚画完椭圆，将光标设置到新椭圆的中心位置，并进入拖拽模式
                this->cursor().setPos(this->cursor().pos().x()-lastEllipse.width()/2,this->cursor().pos().y()-lastEllipse.height()/2);
                drag = 1;

            }
            lpress = false;
        }
        else if(drawType == 4)
        {
            QRect& lastLine = line.last();//拿到新矩形
            lastLine.setBottomRight(e->pos());//更新矩形的右下角坐标)
            lpress = false;

        }
        GenerateDrawImage();
    }
}

void MainWindow::AddTexts()//当输入框文本改变时调用
{
    QString& last = text.last();//拿到最后一个文本
    last = tEdit->toPlainText();//将输入框文本作为文本
}



void MainWindow::on_button_upLayer_clicked()
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

void MainWindow::on_button_downLayer_clicked()
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

void MainWindow::on_button_removeLayer_clicked()
{
    int row = ui->listWidget->currentRow();
    layerViewModel->OnRemoveLayer(row);
    ui->listWidget->takeItem(row);
    emit listUpdate(ui->listWidget);
}
