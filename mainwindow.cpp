#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QPainter>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_openPic_triggered()
{
    QString path = QFileDialog::getOpenFileName(this,"打开图片","C:\\");
    qDebug()<<path;
    filePath=path;


}

void MainWindow::paintEvent(QPaintEvent *){
    //1 创建画家对象
    QPainter painter(this);
    //2 获取绘图所在矩形区域，当前是在frame窗口
    QRect rect = ui->frame->frameRect();

    //坐标值平移，让rect和 painter使用相同的坐标系
    rect.translate(ui->frame->pos());

    //3 构建要绘制的图形对象
    QImage image(filePath);
    if(image.isNull())
        qDebug()<<"open failed";

    //4 使用painter将image图片画到rect
    painter.drawImage(rect,image);

}
