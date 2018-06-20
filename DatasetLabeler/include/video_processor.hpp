#ifndef VIDEO_PROCESSOR
#define VIDEO_PROCESSOR

#include <QThread>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class VideoProcessor : public QThread
{
    Q_OBJECT

    private:
        cv::VideoCapture cap;
        cv::Mat frame;
        double waitPeriod;
        bool pauseStream;

    public:
        VideoProcessor(std::string path_to_video);
        ~VideoProcessor();

    public slots:
        void pause();
        void play();
        void next();
        void previous();
        void run();

    signals:
        void showImage(cv::Mat img);

    public slots:
        void playPauseVideo();

};

#endif
