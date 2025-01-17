#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main(int argc, char **argv)
{
    if(argc < 1)
    {
        std::cout << "Usage:\n" << argv[0] << " <path_to_image>" << std::endl;
        return -1;
    }

    std::cout << "Path to image: " << argv[1];

    cv::Mat img = cv::imread(argv[1], cv::IMREAD_ANYCOLOR | cv::IMREAD_ANYDEPTH);

    if(img.empty())
    {
        std::cout << "Image " << argv[1] << " not able to open" << std::endl;
        return -2;
    }

    std::cout << "Image size: " << img.size() << std::endl;
    std::cout << "Image format: " << img.depth() << std::endl;

    cv::namedWindow(argv[1], cv::WINDOW_NORMAL);
    cv::imshow(argv[1], img);
    cv::waitKey();

    return 0;
}
