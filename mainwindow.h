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
#include <QLabel>
#include <QMouseEvent>

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


    void GenerateDrawImage();
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

    void AddTexts();//添加文字


    //zyk
    void on_button_upLayer_clicked();

    void on_button_downLayer_clicked();

    void on_button_removeLayer_clicked();


protected:
//绘图事件,鼠标事件，菜单事件
    void paintEvent(QPaintEvent*);//重写窗体重绘事件

    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *);//重写鼠标释放事件
    void mouseMoveEvent(QMouseEvent *);//重写鼠标移动事件

    void contextMenuEvent(QContextMenuEvent *);//重写菜单事件

    bool eventFilter(QObject* watched,QEvent* event);


private:
    Ui::MainWindow *ui;
//文件路径
    QString filePath;
//是否打开图片
    bool isPicOpen;


    //zyk
    ImageLayerObserver* layerViewModel;

    //rect 区域
    QRect curRect;
    //显示的Qimage对象
    QImage curImage;
    //右键菜单
    QMenu * rMenu;

    //画图相关

    int lpress;//左键按下标志
    int drag;//拖拽标志
    int drawType;//描绘类型
    int openflag;//打开图片
    QTextEdit* tEdit;//初始化文本输入框

public:
    QPixmap pixmap;//画布图片

    QVector<QVector<QPoint> > lines;//线条集合(一条线条可包含多个线段)
    QVector<QRect> rects;//矩形集合
    QVector<QRect> ellipse;//椭圆集合
    QVector<QRect>  line;//直线集合
    QVector<QString>  text;//文字集合
    QVector<QPoint>  tpoint;//文字位置集合
    QVector<int>  shape;//图形类型集合，用于撤回功能
    QPoint begin;//鼠标按下坐标、用于最后一个图形移动






signals:
    void AddNewLayer(QImage img);
    void listUpdate(QListWidget* list);
private:
    void ShowBlendingImage();
    void AddItemInList(ImageLayer* layer);
    void RemoveLayer();
    QListWidgetItem* CopyItem(QListWidgetItem *layer,int index);

};
#endif // MAINWINDOW_H
