#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)   
    , ui(new Ui::MainWindow)
    , isPicOpen(false)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::paintEvent(QPaintEvent *){
    //1 创建画家对象
    QPainter painter(this);
    //2 获取绘图所在矩形区域，当前是在frame窗口

    QRect rect = ui->frame->frameRect();

    //坐标值平移，让rect和painter使用相同的坐标系,在这个ui设计中会有很多的问题
    rect.translate(ui->centralwidget->pos()+ui->frame->pos());


    //使用painter将image图片画到rect
    painter.drawImage(rect,curImage);

}

void MainWindow::on_action_openPic_triggered()
{
    QString path = QFileDialog::getOpenFileName(this,"打开图片","C:\\");
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
        QImage grayImage(greyData,width,height,bytePerLine,QImage::Format_RGB888);
        //QPixmap pixmap2(QPixmap::fromImage (greyImage));
        curImage=grayImage;
        update();
    }else {

        QMessageBox::critical(this,"error","图片没打开");
    }
}
void MainWindow::on_action_inverse_color_triggered()
{
    if(isPicOpen){
        //QPixmap pixmap(QPixmap::fromImage(curImage));
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
        //QPixmap pixmap2(QPixmap::fromImage (greyImage));
        curImage=inverseImage;
        update();
    }else {

        QMessageBox::critical(this,"error","图片没打开");
    }
}
void MainWindow::on_action_histogram_equalization_triggered()
{

}


