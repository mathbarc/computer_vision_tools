#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "video_processor.hpp"

#include <opencv2/core/core.hpp>
#include <QObject>
#include <vector>
#include <QImage>

class Controller : public QObject
{
	Q_OBJECT

	private:

        VideoProcessor* videoThread;
        QImage cvToQImage(cv::Mat img);
        cv::Mat recentFrame;

    private slots:
        void openVideo(QString path);
        void close();
        void showImage(cv::Mat img);

    signals:
        void showImage(QImage img);

	public:
        Controller();
        ~Controller();
};

#endif
