#include "video_processor.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <QImage>
#include <iostream>

VideoProcessor::VideoProcessor(std::string id, std::string path_to_video)
    : id(id)
{
    this->cap.open(path_to_video);
    if(!this->cap.isOpened()){
        throw std::string("Não foi possível abrir o vídeo.");
    }
    this->waitPeriod = 1e6/(this->cap.get(cv::CAP_PROP_FPS));
    this->pauseStream = true;
}

VideoProcessor::VideoProcessor(std::string id, int index, double fps)
    : id(id)
{
    #if WIN32
    this->cap.open(index, cv::CAP_DSHOW);
    #else
    this->cap.open(index, cv::CAP_V4L);
    #endif
    if(!this->cap.isOpened()){
        throw std::string("Não foi possível abrir o vídeo.");
    }

    this->cap.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
    this->cap.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);
    this->cap.set(cv::CAP_PROP_FPS, fps);
    this->cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M','J','P','G'));
    this->cap.set(cv::CAP_PROP_EXPOSURE, -4);
    this->cap.set(cv::CAP_PROP_AUTO_EXPOSURE, 0);
    this->cap.set(cv::CAP_PROP_BRIGHTNESS, 100);
    this->cap.set(cv::CAP_PROP_AUTOFOCUS, 0);
    this->cap.set(cv::CAP_PROP_FOCUS, 0);

    this->waitPeriod = 1e6/fps;
    this->pauseStream = false;
}

void VideoProcessor::pause()
{
    this->pauseStream = true;
}
void VideoProcessor::play()
{
    this->pauseStream = false;
}

void VideoProcessor::next()
{
    this->cap >> this->frame;

    emit showImage(this->frame);

}

void VideoProcessor::previous()
{
    double fcount = this->cap.get(cv::CAP_PROP_FRAME_COUNT);
    fcount-=2;
    this->cap.get(cv::CAP_PROP_FRAME_COUNT);
    this->cap.set(cv::CAP_PROP_FRAME_COUNT,fcount);

    this->cap >> this->frame;

    emit showImage(this->frame);

}

void VideoProcessor::capture()
{
    this->cap >> this->frame;

    emit captureImage(this->id, this->frame);
}

void VideoProcessor::run()
{
    this->cap >> this->frame;
    while(!this->frame.empty()){
        emit showImage(frame);
        emit showImageWithId(this->id, frame);
        QThread::usleep(this->waitPeriod);
        this->cap.read(this->frame);
        while(this->pauseStream)
        {
            if(this->isInterruptionRequested()){
                return;
            }
        }
        if(this->isInterruptionRequested()){
            return;
        }
    }
}

VideoProcessor::~VideoProcessor()
{
    this->requestInterruption();
    this->wait();

    this->cap.release();
//    std::cout<<"Encerrando Thread de captura"<<std::endl;
}

void VideoProcessor::playPauseVideo(){
    this->pauseStream = !this->pauseStream;
}

