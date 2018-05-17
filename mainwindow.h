#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QFileDialog>
#include <opencv.hpp>
#include <QVector>
#include <QImage>
#include <QPoint>
#include <QKeyEvent>
#include <vector>
#include <QTime>
#define moveLeft  0
#define moveRight 1
#define moveUp    2
#define moveDown  3
#define moveStop  4
using namespace cv;
using namespace std;
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

    void on_AutoBtn_clicked();

private:
    Ui::MainWindow *ui;
 protected:
     void keyPressEvent(QKeyEvent *event);
signals:
     void up_control();
     void down_control();
     void left_control();
     void right_control();
    //User Define Func and Var
public:
    Mat OriImage;
    Mat MovedImage;
    QImage dispImage;
    int x_distance;
    int step;
    int dir;
    int Image_cent_x;
    int Image_cent_y;
    bool isFindWholeCircle;
    bool isSuccessFindObject;
    QPoint xyoffset;
    QPoint xyMatch;
    QVector<QPoint>path;
    int msec;
    QImage convertMatToQImage(cv::Mat &mat);
    void dispLabelImage(QImage);
    void updataDisoffset(QPoint);
    void upadateMatchXY(int x,int y);
    void updataDir(int);
    Mat moveImage(Mat &InputImage,Mat &OutImage, QPoint offset);//移动图像主函数
    Point3f LeastSquareFittingCircle(vector<Point> temp_coordinates);//最小二乘法拟合圆
    Mat processImage(Mat InputImage,Mat outImage);//处理图像主函数
    int selectCircle(Mat InputImage,vector<Point>,Point3f);//筛选识别到的最合理轮廓
    float DisP2P(Point,Point);//计算点到点的距离
    //定义一个延时可视化函数
    void m_sleep(unsigned int msec);
};

#endif // MAINWINDOW_H
