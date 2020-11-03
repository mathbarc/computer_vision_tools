#ifndef VIDEO_PROCESSOR
#define VIDEO_PROCESSOR

#include <QThread>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

class VideoProcessor : public QThread
{
    Q_OBJECT

    private:
        cv::VideoCapture cap;
        cv::Mat frame;
        double waitPeriod;
        bool pauseStream;
        std::string id;

    public:
        VideoProcessor(std::string id, std::string path_to_video);
        VideoProcessor(std::string id, int index, double fps);
        ~VideoProcessor();

    public slots:
        void pause();
        void play();
        void next();
        void previous();
        void run();
        void capture();
        void playPauseVideo();

    signals:
        void showImage(cv::Mat img);
        void showImageWithId(std::string id, cv::Mat img);
        void captureImage(std::string id, cv::Mat img);

};

#endif
