#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QFileDialog>
#include <opencv.hpp>
#include <QVector>
#include <QImage>
#include <QPoint>
#define moveLeft  0
#define moveRight 1
#define moveUp    2
#define moveDown  3
#define moveStop  4
using namespace cv;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_OpenFile_clicked();

    void on_UpBtn_clicked();

    void on_LeftBtn_clicked();

    void on_DownBtn_clicked();

    void on_RightBtn_clicked();

    void on_StepLineEdit_editingFinished();

private:
    Ui::MainWindow *ui;
    //User Define Func and Var
public:
    Mat OriImage;
    Mat MovedImage;
    QImage dispImage;
    int x_distance;
    int step;
    int dir;
     QPoint xyoffset;
    QVector<QPoint>path;
    QImage convertMatToQImage(cv::Mat &mat);
    void dispLabelImage(QImage);
    void updataDisoffset(QPoint);
    void updataDir(int);
    Mat moveImage(Mat &InputImage,Mat &OutImage, QPoint offset);
};

#endif // MAINWINDOW_H
