#include "controller.hpp"
#include <QMessageBox>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>



Controller::Controller()
    : videoThread(NULL)
{
    qRegisterMetaType<cv::Mat>("cv::Mat");
}


QImage Controller::cvToQImage(cv::Mat img){
    static const cv::Vec3b max(255,255,255);

//    for (int i = 0; i < this->markers.image.size(); i++)
//    {
//        cv::Vec3b cor = img.at<cv::Vec3b>(markers.image[i]);
//        if(cor[0] == 0 && cor[1] == 255 && cor[2] == 0)
//            cv::circle(img,markers.image[i],3,cv::Scalar(255,0,0),CV_FILLED);
//        else
//            cv::circle(img,markers.image[i],3,cv::Scalar(0,255,0),CV_FILLED);
//    }

    QImage to_show(img.cols, img.rows, QImage::Format_ARGB32);
    cv::Vec3b* v;
    QRgb* v_show;
    for(int i = 0; i<img.rows; i++)
    {
        v = img.ptr<cv::Vec3b>(i);
        v_show = (QRgb*)to_show.scanLine(i);
        for(int j = 0; j<img.cols; j++)
        {
            v_show[j] = qRgba(v[j][2],v[j][1],v[j][0],255);
        }
    }
    return to_show;
}

void Controller::showImage(cv::Mat img)
{
    img.copyTo(this->recentFrame);
    emit showImage(cvToQImage(img));
}

Controller::~Controller()
{
    if(this->videoThread!=NULL)
    {
        this->videoThread->requestInterruption();
        this->videoThread->wait();
        delete this->videoThread;
    }
}


void Controller::openVideo(QString path)
{
    std::string pathVideo = path.toStdString();
    try{
        if(this->videoThread!=NULL)
        {
            this->videoThread->requestInterruption();
            this->videoThread->wait();

            delete this->videoThread;
            this->videoThread=NULL;
        }
        this->videoThread = new VideoProcessor(pathVideo);
        this->videoThread->start();
    }
    catch(std::string message){
        QMessageBox::warning(nullptr,"Problema na abertura do vídeo",QString(message.c_str()));
        this->videoThread=NULL;
    }


}


void Controller::close(){
    if(this->videoThread!=NULL)
    {
        this->videoThread->requestInterruption();
        this->videoThread->wait();
        delete this->videoThread;
        this->videoThread = NULL;
    }
}
