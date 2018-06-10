#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>
std::fstream out;
int position;

cv::Mat img;
std::vector<cv::Rect> selections;


void paint(cv::Mat& im, cv::Rect roi)
{
    for(int i = 0; i<selections.size(); i++)
    {
        cv::rectangle(im, selections[i], cv::Scalar(0,0,255));
    }
    cv::rectangle(im, roi, cv::Scalar(0,255,0));
}

void mouse_callback(int event, int x, int y, int, void*)
{
    static cv::Point2d p1;
    static cv::Point2d p2;
    static bool pressed = false;

//    std::cout<<p1<<", "<<p2<<std::endl;
    if(event == cv::EVENT_LBUTTONDOWN)
    {
        p1.x = x;
        p1.y = y;
        pressed = true;
    }
    else if(pressed)
    {
        if(event == cv::EVENT_MOUSEMOVE)
        {
            p2.x = x;
            p2.y = y;
            cv::Mat show = img.clone();
            cv::Rect roi(p2,p1);
            paint(show, roi);
            cv::imshow("Video", show);
        }
        if(event == cv::EVENT_LBUTTONUP)
        {
            p2.x = x;
            p2.y = y;
            cv::Rect roi(p2,p1);
            selections.push_back(roi);
            out<<position<<", "<<roi.x<<", "<<roi.y<<", "<<roi.width<<", "<<roi.height<<std::endl;
            pressed = false;
        }
    }
}


int main(int argc, char** argv)
{
    if(argc<4)
    {
        return -1;
    }

    cv::VideoCapture cap(argv[1]);
    int step = std::atoi(argv[2]);



    int maxPosition = cap.get(CV_CAP_PROP_FRAME_COUNT);
    out.open(argv[3], std::fstream::out);

    cv::namedWindow("Video", cv::WINDOW_NORMAL);
    cv::setMouseCallback("Video",mouse_callback);
    for(position = 0; position <= maxPosition; position += step)
    {
        cap.set(CV_CAP_PROP_POS_FRAMES, position);
        cap >> img;
        if(img.empty())
            break;
        cv::imshow("Video", img);
        cv::waitKey(0);
        selections.clear();
    }
    out.close();


    return 0;
}
