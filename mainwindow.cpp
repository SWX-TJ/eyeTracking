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
    step  = 1;
    dir = moveStop;
    xyoffset.setX(0);
    xyoffset.setY(0);
    updataDisoffset(xyoffset);
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
        for (int i = x_dis; i<InputImage.rows; i++)
        {
            for (int j = y_dis; j<InputImage.cols; j++)
            {
                OutImage.at<Vec3b>(i, j) = InputImage.at<Vec3b>(i -x_dis, j-y_dis);
            }
        }
    }
    else if(offset.x()>=0&&offset.y()<0)
    {
        for (int i = x_dis; i<InputImage.rows; i++)
        {
            for (int j = 0; j<InputImage.cols-y_dis; j++)
            {
                OutImage.at<Vec3b>(i, j) = InputImage.at<Vec3b>(i -x_dis, j+y_dis);
            }
        }
    }
    else if(offset.x()<0&&offset.y()>=0)
    {
        for (int i = 0; i<InputImage.rows-x_dis; i++)
        {
            for (int j = y_dis; j<InputImage.cols; j++)
            {
                OutImage.at<Vec3b>(i, j) = InputImage.at<Vec3b>(i +x_dis, j-y_dis);
            }
        }
    }
    else if(offset.x()<0&&offset.y()<0)
    {
        for (int i = 0; i<InputImage.rows-x_dis; i++)
        {
            for (int j = 0; j<InputImage.cols-y_dis; j++)
            {
                OutImage.at<Vec3b>(i, j) = InputImage.at<Vec3b>(i+x_dis, j+y_dis);
            }
        }
    }
    return OutImage;
}


void MainWindow::on_UpBtn_clicked()
{
    xyoffset.setX(xyoffset.x()-step);
    if(abs(xyoffset.x())>OriImage.rows)
    {
        xyoffset.setX(-OriImage.rows);
    }
    updataDisoffset(xyoffset);
    MovedImage =  moveImage(OriImage,MovedImage,xyoffset);
    dispImage = convertMatToQImage(MovedImage);
    dispLabelImage(dispImage);
    dir = moveUp;
    updataDir(dir);
}

void MainWindow::on_LeftBtn_clicked()
{
    xyoffset.setY(xyoffset.y()-step);
    if(abs(xyoffset.y())>OriImage.cols)
    {
        xyoffset.setY(-OriImage.cols);
    }
    updataDisoffset(xyoffset);
    MovedImage =  moveImage(OriImage,MovedImage,xyoffset);
    dispImage = convertMatToQImage(MovedImage);
    dispLabelImage(dispImage);
    dir = moveLeft;
    updataDir(dir);
}

void MainWindow::on_DownBtn_clicked()
{
    xyoffset.setX(xyoffset.x()+step);
    if(abs(xyoffset.x())>OriImage.rows)
    {
        xyoffset.setX(OriImage.rows);
    }
    updataDisoffset(xyoffset);
    MovedImage =  moveImage(OriImage,MovedImage,xyoffset);
    dispImage = convertMatToQImage(MovedImage);
    dispLabelImage(dispImage);
    dir = moveDown;
    updataDir(dir);
}

void MainWindow::on_RightBtn_clicked()
{
    xyoffset.setY(xyoffset.y()+step);
    if(abs(xyoffset.y())>OriImage.cols)
    {
        xyoffset.setY(OriImage.cols);
    }
    updataDisoffset(xyoffset);
    MovedImage =  moveImage(OriImage,MovedImage,xyoffset);
    dispImage = convertMatToQImage(MovedImage);
    dispLabelImage(dispImage);
    dir = moveRight;
    updataDir(dir);
}

void MainWindow::on_StepLineEdit_editingFinished()
{
    step = ui->StepLineEdit->text().toInt();
}
