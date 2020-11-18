#include "video_processor.hpp"
#include "keyboard_event_filter.hpp"
#include <QObject>
#include <map>
#include <QApplication>
#include <chrono>
#include <iostream>

#include <ctime>
#include <list>
#include <vector>
#include <opencv2/core/core.hpp>
std::vector<std::string> imgPaths;

cv::FileStorage storage("stereo_calib.xml", cv::FileStorage::WRITE);

void show(std::string source, cv::Mat img)
{
    cv::imshow(source, img);
}

void capture(std::string source, cv::Mat img)
{

    static std::map<std::string, int> sourceCount;
    if(sourceCount.find(source)==sourceCount.end())
    {
        sourceCount[source] = 0;
    }

    std::string path = source+std::to_string(sourceCount[source])+".png";
    imgPaths.push_back(path);
    cv::imwrite(path, img);
    sourceCount[source]++;
}

void finish()
{
    cv::destroyAllWindows();
}



int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    VideoProcessor processor0("left", 0, 30);
    VideoProcessor processor1("right", 1, 30);
    KeyboardEventFilter* filter = new KeyboardEventFilter();



    QObject::connect(&processor0, &VideoProcessor::showImageWithId, &show);
    QObject::connect(&processor1, &VideoProcessor::showImageWithId, &show);

    QObject::connect(&processor0, &VideoProcessor::captureImage, &capture);
    QObject::connect(&processor1, &VideoProcessor::captureImage, &capture);

    QObject::connect(filter, SIGNAL(capture()), &processor0, SLOT(capture()));
    QObject::connect(filter, SIGNAL(capture()), &processor1, SLOT(capture()));

    QObject::connect(filter, SIGNAL(finish()), &processor0, SLOT(terminate()));
    QObject::connect(filter, SIGNAL(finish()), &processor1, SLOT(terminate()));
    QObject::connect(filter, &KeyboardEventFilter::finish, &finish);

    cv::namedWindow("left", cv::WINDOW_KEEPRATIO);
    cv::namedWindow("right", cv::WINDOW_KEEPRATIO);


    processor0.start();
    processor1.start();
    app.installEventFilter(filter);

    auto code = app.exec();
    storage << "imagelist" << imgPaths;
    storage.release();
    return code;
}
