#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>


int main(int argc, char** argv)
{
    if(argc < 2)
	    return -1;

    #if WIN32
    cv::VideoCapture cap(std::atof(argv[1]), cv::CAP_DSHOW);
    #else
    cv::VideoCapture cap(std::atof(argv[1]), cv::CAP_V4L2);
    #endif
    
    cv::Mat frame;
    double focus = 30;
    
    cv::Size size(1920,1080);
    double fps = 30;

    std::cout<<cap.set(cv::CAP_PROP_FOURCC,cv::VideoWriter::fourcc('M','J','P','G'))<<std::endl;
    std::cout<<cap.set(cv::CAP_PROP_FRAME_WIDTH, size.width)<<std::endl;
    std::cout<<cap.set(cv::CAP_PROP_FRAME_HEIGHT, size.height)<<std::endl;
    std::cout<<cap.set(cv::CAP_PROP_FPS, fps)<<std::endl;
    std::cout<<cap.set(cv::CAP_PROP_AUTOFOCUS, 0)<<std::endl;
    std::cout<<cap.set(cv::CAP_PROP_FOCUS, focus)<<std::endl;
//    std::cout<<cap.set(cv::CAP_PROP_AUTO_EXPOSURE, 0)<<std::endl;
//    std::cout<<cap.set(cv::CAP_PROP_EXPOSURE, -7)<<std::endl;
    
    //std::cout<<cap.set(cv::CAP_PROP_FPS, 15)<<std::endl;

    std::cout<<"Frame size: "<<cap.get(cv::CAP_PROP_FRAME_WIDTH)<<", " << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << std::endl;
    std::cout<<"Camera FPS: "<<cap.get(cv::CAP_PROP_FPS)<<std::endl;
    std::cout<<"Camera Focus: "<<cap.get(cv::CAP_PROP_FOCUS)<<std::endl;
    char key;

    cv::VideoWriter vid("out.mp4", cv::VideoWriter::fourcc('A','V','C','1'), 30, size);

    cv::namedWindow("Frame", cv::WINDOW_NORMAL);
    do
    {
        cap >> frame;
        vid << frame;
        cv::imshow("Frame", frame);
        key = cv::waitKey(1);
        if(key == 'q')
        {
            focus+=5;
            if(focus >= 250)
                    focus = 250;
            cap.set(cv::CAP_PROP_FOCUS, focus);
        }
        else if(key == 'a')
        {
            focus-=5;
            if(focus<0)
                focus = 0;
            cap.set(cv::CAP_PROP_FOCUS, focus);
        }
        else if(key == 'z')
        {
            std::cout<<"Camera Focus: "<<cap.get(cv::CAP_PROP_FOCUS)<<std::endl;
        }
        else if(key == 'x')
        {
            std::cout<<"Exposure: "<<cap.get(cv::CAP_PROP_EXPOSURE)<<std::endl;
        }
    }
    while(!frame.empty()&&key!=27);

    cap.release();
    vid.release();
    
    return 0;

}
