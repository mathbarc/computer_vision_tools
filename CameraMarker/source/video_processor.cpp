#include "video_processor.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QImage>
#include <iostream>

VideoProcessor::VideoProcessor(std::string path_to_video)
{
    this->cap.open(path_to_video);
    if(!this->cap.isOpened()){
        throw std::string("Não foi possível abrir o vídeo.");
    }
    this->waitPeriod = 1e6/(this->cap.get(CV_CAP_PROP_FPS));
    this->pauseStream = true;
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
    double fcount = this->cap.get(CV_CAP_PROP_FRAME_COUNT);
    fcount-=2;
    this->cap.get(CV_CAP_PROP_FRAME_COUNT);
    this->cap.set(CV_CAP_PROP_FRAME_COUNT,fcount);

    this->cap >> this->frame;

    emit showImage(this->frame);

}

void VideoProcessor::run()
{
    this->cap >> this->frame;
    while(!this->frame.empty()){
        emit showImage(frame);
        QThread::usleep(this->waitPeriod);
        this->cap >> this->frame;
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

VideoProcessor::~VideoProcessor(){
    this->cap.release();
    std::cout<<"Encerrando Thread de captura"<<std::endl;
}

void VideoProcessor::playPauseVideo(){
    this->pauseStream = !this->pauseStream;
}
