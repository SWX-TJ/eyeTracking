#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // init User Var
    x_distance =0;
    step  = 10;
    isFindWholeCircle = false;
    isSuccessFindObject = false;
    dir = moveStop;
    xyoffset.setX(0);
    xyoffset.setY(0);
    xyMatch.setX(0);
    xyMatch.setY(0);
    updataDisoffset(xyoffset);
    connect(this,SIGNAL(up_control()),ui->UpBtn,SLOT(click()));
    connect(this,SIGNAL(down_control()),ui->DownBtn,SLOT(click()));
    connect(this,SIGNAL(left_control()),ui->LeftBtn,SLOT(click()));
    connect(this,SIGNAL(right_control()),ui->RightBtn,SLOT(click()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_OpenFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("open image file"),
                "./", tr("Image files(*.bmp *.jpg );;All files (*.*)"));
    xyoffset.setX(0);
    xyoffset.setY(0);
    updataDisoffset(xyoffset);
    OriImage = imread(fileName.toStdString());
    MovedImage =  Mat(OriImage.rows,OriImage.cols,CV_8UC3,Scalar(0,0,0));
    dispImage = convertMatToQImage(OriImage);
    dispLabelImage(dispImage);
}

QImage MainWindow::convertMatToQImage(Mat &mat)
{
    QImage img;
    int nChannel=mat.channels();
    if(nChannel==3)
    {
        cv::cvtColor(mat,mat,CV_BGR2RGB);
        img = QImage((const unsigned char*)mat.data,mat.cols,mat.rows,QImage::Format_RGB888);
    }
    else if(nChannel==4)
    {
        img = QImage((const unsigned char*)mat.data,mat.cols,mat.rows,QImage::Format_ARGB32);
    }
    else if(nChannel==1)
    {
        img = QImage((const unsigned char*)mat.data,mat.cols,mat.rows,QImage::Format_Indexed8);
    }

    return img;
}

void MainWindow::dispLabelImage(QImage img)
{
    ui->DispLabel->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::updataDisoffset(QPoint xy_offset)
{
    ui->x_offset->setNum(xy_offset.x());
    ui->y_offset->setNum(xy_offset.y());
}

void MainWindow::upadateMatchXY(int x, int y)
{
    xyMatch.setX(x);
    xyMatch.setY(y);
    ui->x_label->setNum(x);
    ui->y_label->setNum(y);
}

void MainWindow::updataDir(int dispdir)
{
    switch (dispdir) {
    case moveUp:
        ui->DirLabel->setText("Up");
        break;
    case moveDown:
        ui->DirLabel->setText("Down");
        break;
    case moveLeft:
        ui->DirLabel->setText("Left");
        break;
    case moveRight:
        ui->DirLabel->setText("Right");
        break;
    default:
        break;
    }
}

Mat MainWindow::moveImage(Mat &InputImage,Mat &OutImage, QPoint offset)
{
    MovedImage =  Mat(OriImage.rows,OriImage.cols,CV_8UC3,Scalar(0,0,0));
    int x_dis = abs(offset.x());
    int y_dis = abs(offset.y());
    if(offset.x()>=0&&offset.y()>=0)
    {
        for (int i = y_dis; i<InputImage.rows; i++)
        {
            for (int j = x_dis; j<InputImage.cols; j++)
            {
                OutImage.at<Vec3b>(i, j) = InputImage.at<Vec3b>(i -y_dis, j-x_dis);
            }
        }
    }
    else if(offset.x()>=0&&offset.y()<0)
    {
        for (int i = 0; i<InputImage.rows-y_dis; i++)
        {
            for (int j = x_dis; j<InputImage.cols; j++)
            {
                OutImage.at<Vec3b>(i, j) = InputImage.at<Vec3b>(i +y_dis, j-x_dis);
            }
        }
    }
    else if(offset.x()<0&&offset.y()>=0)
    {
        for (int i = y_dis; i<InputImage.rows; i++)
        {
            for (int j = 0; j<InputImage.cols-x_dis; j++)
            {
                OutImage.at<Vec3b>(i, j) = InputImage.at<Vec3b>(i -y_dis, j+x_dis);
            }
        }
    }
    else if(offset.x()<0&&offset.y()<0)
    {
        for (int i = 0; i<InputImage.rows-y_dis; i++)
        {
            for (int j = 0; j<InputImage.cols-x_dis; j++)
            {
                OutImage.at<Vec3b>(i, j) = InputImage.at<Vec3b>(i +y_dis, j+x_dis);
            }
        }
    }
    return OutImage;
}

Point3f MainWindow::LeastSquareFittingCircle(vector<Point> temp_coordinates)
{
    float x1 = 0;
    float x2 = 0;
    float x3 = 0;
    float y1 = 0;
    float y2 = 0;
    float y3 = 0;
    float x1y1 = 0;
    float x1y2 = 0;
    float x2y1 = 0;
    int num;
    vector<Point>::iterator k;
    Point3f tempcircle;
    for (k = temp_coordinates.begin(); k != temp_coordinates.end(); k++)
    {

        x1 = x1 + (*k).x;
        x2 = x2 + (*k).x * (*k).x;
        x3 = x3 + (*k).x * (*k).x * (*k).x;
        y1 = y1 + (*k).y;
        y2 = y2 + (*k).y * (*k).y;
        y3 = y3 + (*k).y * (*k).y * (*k).y;
        x1y1 = x1y1 + (*k).x * (*k).y;
        x1y2 = x1y2 + (*k).x * (*k).y * (*k).y;
        x2y1 = x2y1 + (*k).x * (*k).x * (*k).y;
    }
    float C, D, E, G, H, a, b, c;
    num = temp_coordinates.size();
    C = num * x2 - x1 * x1;
    D = num * x1y1 - x1 * y1;
    E = num * x3 + num * x1y2 - x1 * (x2 + y2);
    G = num * y2 - y1 * y1;
    H = num * x2y1 + num * y3 - y1 * (x2 + y2);
    a = (H*D - E * G) / (C*G - D * D);
    b = (H*C - E * D) / (D*D - G * C);
    c = -(x2 + y2 + a * x1 + b * y1) / num;
    tempcircle.x = -a / 2; //圆心x坐标
    tempcircle.y = -b / 2;//圆心y坐标
    tempcircle.z = sqrt(a*a + b * b - 4 * c) / 2;//圆心半径
    return tempcircle;
}

Mat MainWindow::processImage(Mat InputImage, Mat outImage)
{
    //灰度化
    Mat  GrayImage;
    cvtColor(InputImage, GrayImage, COLOR_BGR2GRAY);
    //二值化
    Mat BinImage;
    threshold(GrayImage, BinImage, 0, 255, THRESH_OTSU);
    //找寻轮廓
    std::vector<std::vector<cv::Point>> contours,final_contours;
    cv::findContours(BinImage, contours,
                     RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    for (int i = 0; i < contours.size(); i++)
    {
        if (contours.at(i).size() >100&& contours.at(i).size()<700)
        {
            final_contours.push_back(contours.at(i));
        }
    }
    Mat contourImg = Mat(OriImage.rows, OriImage.cols,CV_8UC3,Scalar(0,0,0));
    cv::drawContours(contourImg, final_contours,
                     -1, cv::Scalar(255,255,255),2);
    Mat temp_houguImg;
    cvtColor(contourImg,temp_houguImg,COLOR_BGR2GRAY);
    vector<Vec3f> circles;
    HoughCircles(temp_houguImg, circles, CV_HOUGH_GRADIENT, 1.5, 220, 1, 100, 0, 0);
    if(circles.size()>0)
    {
        isFindWholeCircle = true;
    }
    else
    {
        isFindWholeCircle = false;
    }
    for (size_t i = 0; i < circles.size(); i++)
    {
        //参数定义
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        //绘制圆心
        circle(contourImg, center, 3, Scalar(0, 255, 0), -1, 8, 0);
        //绘制圆轮廓
        circle(contourImg, center, radius, Scalar(155, 50, 255), 3, 8, 0);
    }
    //最小二乘拟合圆
    for (int j = 0; j < final_contours.size(); j++)
    {
        Point3f circle_data = LeastSquareFittingCircle(final_contours.at(j));
        if(isFindWholeCircle)
        {
            if (3.14* circle_data.z*circle_data.z > 8000 && 3.14* circle_data.z*circle_data.z < 13000&&abs(cvRound(circles[0][0])-circle_data.x)<10)
            {
                circle(InputImage, Point(circle_data.x, circle_data.y), circle_data.z, Scalar(0, 255, 0), 2);
                circle(InputImage, Point(circle_data.x, circle_data.y), 2, Scalar(0, 0, 255), 4);
                upadateMatchXY(circle_data.x, circle_data.y);
                break;
            }
            else
            {
               // upadateMatchXY(-1,-1);
                // break;
            }
        }
        else
        {
            if (3.14* circle_data.z*circle_data.z > 8000 && 3.14* circle_data.z*circle_data.z < 13000)
            {
                circle(InputImage, Point(circle_data.x, circle_data.y), circle_data.z, Scalar(0, 255, 0), 2);
                circle(InputImage, Point(circle_data.x, circle_data.y), 2, Scalar(0, 0, 255), 4);
                upadateMatchXY(circle_data.x, circle_data.y);
                // break;
            }
            else
            {
               // upadateMatchXY(-1,-1);
                // break;
            }
        }

    }
    return outImage;
}


void MainWindow::on_UpBtn_clicked()
{
    xyoffset.setY(xyoffset.y()-step);
    if(abs(xyoffset.y())>OriImage.rows)
    {
        xyoffset.setY(-OriImage.rows);
    }
    updataDisoffset(xyoffset);
    MovedImage =  moveImage(OriImage,MovedImage,xyoffset);
    MovedImage = processImage(MovedImage,MovedImage);
    dispImage = convertMatToQImage(MovedImage);
    dispLabelImage(dispImage);
    dir = moveUp;
    updataDir(dir);
}

void MainWindow::on_DownBtn_clicked()
{
    xyoffset.setY(xyoffset.y()+step);
    if(abs(xyoffset.y())>OriImage.rows)
    {
        xyoffset.setY(OriImage.rows);
    }
    updataDisoffset(xyoffset);
    MovedImage =  moveImage(OriImage,MovedImage,xyoffset);
    MovedImage = processImage(MovedImage,MovedImage);
    dispImage = convertMatToQImage(MovedImage);
    dispLabelImage(dispImage);
    dir = moveDown;
    updataDir(dir);
}


void MainWindow::on_LeftBtn_clicked()
{
    xyoffset.setX(xyoffset.x()-step);
    if(abs(xyoffset.x())>OriImage.cols)
    {
        xyoffset.setX(-OriImage.cols);
    }
    updataDisoffset(xyoffset);
    MovedImage =  moveImage(OriImage,MovedImage,xyoffset);
    MovedImage = processImage(MovedImage,MovedImage);
    dispImage = convertMatToQImage(MovedImage);
    dispLabelImage(dispImage);
    dir = moveLeft;
    updataDir(dir);
}



void MainWindow::on_RightBtn_clicked()
{

    xyoffset.setX(xyoffset.x()+step);
    if(abs(xyoffset.x())>OriImage.cols)
    {
        xyoffset.setX(OriImage.cols);
    }
    updataDisoffset(xyoffset);
    MovedImage =  moveImage(OriImage,MovedImage,xyoffset);
    MovedImage = processImage(MovedImage,MovedImage);
    dispImage = convertMatToQImage(MovedImage);
    dispLabelImage(dispImage);
    dir = moveRight;
    updataDir(dir);
}

void MainWindow::on_StepLineEdit_editingFinished()
{
    step = ui->StepLineEdit->text().toInt();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        exit(EXIT_SUCCESS);
        break;
    case Qt::Key_W:
        up_control();
        break;
    case Qt::Key_S:
        down_control();
        break;
    case Qt::Key_A:
        left_control();
        break;
    case Qt::Key_D:
        right_control();
        break;
    default:
        break;
    }
}
