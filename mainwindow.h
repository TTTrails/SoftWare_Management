#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QFileDialog>
#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include <QPixmap>


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

private:
//绘图事件处理，虚函数
    void paintEvent(QPaintEvent*);

private:
    Ui::MainWindow *ui;
//文件路径
    QString filePath;
//是否打开图片
    bool isPicOpen;
//rect 区域
    QRect curRect;
//显示的Qimage对象
    QImage curImage;
};
#endif // MAINWINDOW_H
