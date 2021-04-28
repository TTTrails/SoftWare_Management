#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"ImageLayerObserver.h"
#include <QMainWindow>
#include <QDialog>
#include <QFileDialog>
#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include <QPixmap>
#include <QTextEdit>
#include "mypaint.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

//RGB to YUV
    int RGBConvertToYUV();
    int YUVConvertToRGB();
//自动生成的槽函数
private slots:
    void on_action_openPic_triggered();

    void on_action_grey_to_binary_triggered();

    void on_action_color_to_grey_triggered();

    void on_action_histogram_equalization_triggered();

    void on_action_inverse_color_triggered();

    void on_action_draw_pencil_triggered();

    void on_action_draw_line_triggered();

    void on_action_draw_ellipse_triggered();

    void on_action_draw_rectangle_triggered();

    void on_action_draw_text_triggered();

    //void AddTexts();//添加文字

private:
//绘图事件处理，虚函数


    void mousePressEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent*);//重写窗体重绘事件


    void contextMenuEvent(QContextMenuEvent *);//重写菜单事件
private:
    Ui::MainWindow *ui;
//文件路径
    QString filePath;
//是否打开图片
    bool isPicOpen;


private:
    ImageLayerObserver* layerViewModel;

private slots:
    void UplayerItem();
    void DownlayerItem();
signals:
    void AddNewLayer(QImage img);
    void listUpdate(QListWidget* list);
private:
    void ShowBlendingImage();
    void AddItemInList(ImageLayer* layer);
    void RemoveLayer();
    QListWidgetItem* CopyItem(QListWidgetItem *layer,int index);

//rect 区域
    QRect curRect;
//显示的Qimage对象
    QImage curImage;
//右键菜单
    QMenu * rMenu;

//绘图类
    MyPaint* myPaint;



};
#endif // MAINWINDOW_H
