#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"ImageLayerObserver.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

//自动生成的槽函数
private slots:
    void on_action_openPic_triggered();

    void on_action_grey_to_binary_triggered();

private:
//绘图事件处理，虚函数
    void paint();
    void mousePressEvent(QMouseEvent* event);
    bool eventFilter(QObject *watched, QEvent *event);
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

};
#endif // MAINWINDOW_H
