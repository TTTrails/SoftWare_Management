#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private:
//绘图事件处理，虚函数
    void paintEvent(QPaintEvent*);

private:
    Ui::MainWindow *ui;
//文件路径
    QString filePath;
};
#endif // MAINWINDOW_H
