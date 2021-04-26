#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDialog>
#include <QFileDialog>
#include <QPainter>
#include <QDebug>
#include <QMessageBox>

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


    //3 构建要绘制的图形对象
    QImage image(filePath);
    if(image.isNull())
        qDebug()<<"open failed";
    else{
        isPicOpen=true;
    }
    //4 使用painter将image图片画到rect
    painter.drawImage(rect,image);

}

void MainWindow::on_action_openPic_triggered()
{
    QString path = QFileDialog::getOpenFileName(this,"打开图片","C:\\");
    qDebug()<<path;
    filePath=path;
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
