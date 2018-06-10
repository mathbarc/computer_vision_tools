#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>


int main(int argc, char** argv)
{

    if(argc<2)
        return -1;


    std::cout<<"Opening "<<argv[1]<<std::endl;
    cv::VideoCapture cap(argv[1]);
    if(!cap.isOpened())
        return -2;

    double count=0;
    cv::Mat3b f;
    cv::Mat3d frame;

    cap >> f;
    cv::Mat3d acumulator = cv::Mat3d::zeros(f.size());
    while(!f.empty())
    {
        f.convertTo(frame,CV_64FC3);
        acumulator += frame;
        count++;
        cap >>f;
    }
    cap.release();
    acumulator /= count;
    acumulator.convertTo(f,CV_8UC3);

    cv::namedWindow("result",cv::WINDOW_AUTOSIZE);
    cv::imwrite("bg.png",f);
    cv::imshow("result",f);
    cv::waitKey(0);
    return 0;
}
