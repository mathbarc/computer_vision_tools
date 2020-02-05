#include "controller.h"
#include <QMessageBox>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>



Controller::Controller()
    : window(new CameraMarker(NULL))
    , video_thread(NULL)
    , calib(NULL)
{
    qRegisterMetaType<cv::Mat>("cv::Mat");
    this->window->show();
    connect(this->window,SIGNAL(openVideo(QString)), this, SLOT(openVideo(QString)));
    connect(this->window, SIGNAL(genCalibFile(QString)),this, SLOT(genCalibFile(QString)));
    connect(this->window, SIGNAL(genHomoDirFile(QString)),this, SLOT(genHomoDirFile(QString)));
    connect(this->window, SIGNAL(genHomoInvFile(QString)),this, SLOT(genHomoInvFile(QString)));
    connect(this->window, SIGNAL(destroyed()), this, SLOT(close()));
    connect(this->window, SIGNAL(addCalibrationMarker(CalibrationMarker)),this, SLOT(addCalibrationMarker(CalibrationMarker)));
    connect(this, SIGNAL(insertOnTable(CalibrationMarker)), this->window, SLOT(insertOnTable(CalibrationMarker)));
}


QImage Controller::cvToQImage(cv::Mat img){
    static const cv::Vec3b max(255,255,255);

    for (int i = 0; i < this->markers.image.size(); i++)
    {
        cv::Vec3b cor = img.at<cv::Vec3b>(markers.image[i]);
        if(cor[0] == 0 && cor[1] == 255 && cor[2] == 0)
            cv::circle(img,markers.image[i],3,cv::Scalar(255,0,0),cv::FILLED);
        else
            cv::circle(img,markers.image[i],3,cv::Scalar(0,255,0),cv::FILLED);
    }

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
    if(this->window!=NULL)
    {
        delete this->window;
    }
    if(this->video_thread!=NULL)
    {
        this->video_thread->requestInterruption();
        this->video_thread->wait();
        delete this->video_thread;
    }
}


void Controller::openVideo(QString path)
{
    std::string pathVideo = path.toStdString();
    try{
        if(this->video_thread!=NULL)
        {
            this->video_thread->requestInterruption();
            this->video_thread->wait();
            disconnect(this->window, SIGNAL(playPauseVideo()), this->video_thread, SLOT(playPauseVideo()));
            disconnect(this->video_thread,SIGNAL(showImage(cv::Mat)),this,SLOT(showImage(cv::Mat)));
            disconnect(this,SIGNAL(showImage(QImage)),this->window,SLOT(showImage(QImage)));

            delete this->video_thread;
            this->video_thread=NULL;
        }
        this->video_thread = new VideoProcessor(pathVideo);
        connect(this->video_thread,SIGNAL(showImage(cv::Mat)),this,SLOT(showImage(cv::Mat)));
        connect(this,SIGNAL(showImage(QImage)),this->window,SLOT(showImage(QImage)));
        connect(this->window, SIGNAL(playPauseVideo()), this->video_thread, SLOT(playPauseVideo()));
        this->video_thread->start();
    }
    catch(std::string message){
        QMessageBox::warning(this->window,"Problema na abertura do vídeo",QString(message.c_str()));
        this->video_thread=NULL;
    }


}

void Controller::addCalibrationMarker(CalibrationMarker cm)
{
    cv::Point3d wP = cm.worldPoint;
    if(wP.x != -1 && wP.y != -1 && wP.z != -1)
    {
        this->markers.image.push_back(cm.imagePoint);
        this->markers.world.push_back(cm.worldPoint);
        QImage to_show = cvToQImage(this->recentFrame);
        emit showImage(to_show);
        emit insertOnTable(cm);
    }
}

void Controller::genCalibFile(QString path)
{
    if(this->calib!=NULL)
    {
        this->calib->wait();
        delete this->calib;
    }

    this->calib = new CalibrationProcessor(path.toStdString(), this->markers, this->recentFrame.size(),
                                   CalibrationProcessor::CalibrationType::ZHENG);

    calib->start();
}

void Controller::genHomoDirFile(QString path)
{
    if(this->calib!=NULL)
    {
        this->calib->wait();
        delete this->calib;
    }

    this->calib = new CalibrationProcessor(path.toStdString(), this->markers, this->recentFrame.size(),
                               CalibrationProcessor::CalibrationType::HOMOGRAPHY_DIR);
    this->calib->start();
}

void Controller::genHomoInvFile(QString path)
{
    if(this->calib!=NULL)
    {
        this->calib->wait();
        delete this->calib;
    }

    this->calib = new CalibrationProcessor(path.toStdString(), this->markers, this->recentFrame.size(),
                               CalibrationProcessor::CalibrationType::HOMOGRAPHY_INV);
    this->calib->start();
}

void Controller::close(){
    if(this->video_thread!=NULL)
    {
        this->video_thread->requestInterruption();
        this->video_thread->wait();
        delete this->video_thread;
        this->video_thread = NULL;
    }
    if(this->calib!=NULL)
    {
        this->calib->wait();
        delete this->calib;
        this->calib=NULL;
    }
}
