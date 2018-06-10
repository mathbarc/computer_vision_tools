#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

int main(int argc, char** argv){
	if(argc<2){
		std::cout<<"Número insuficiente de argumentos"<<std::endl;
	}
	cv::VideoCapture cap(argv[1]);

    std::cout<<"Resolução: "<<cap.get(CV_CAP_PROP_FRAME_WIDTH)<<"x"<<cap.get(CV_CAP_PROP_FRAME_HEIGHT)<<std::endl;
    int fourcc = cap.get(CV_CAP_PROP_FOURCC);
    std::cout<<"Codec: "<<((uchar*)&fourcc)[0]<<((uchar*)&fourcc)[1]<<((uchar*)&fourcc)[2]<<((uchar*)&fourcc)[3]<<std::endl;
    std::cout<<"FPS: "<<cap.get(CV_CAP_PROP_FPS)<<std::endl;
    std::cout<<"Quantidade de Frames: "<<cap.get(CV_CAP_PROP_FRAME_COUNT)<<std::endl;
    int secs = cap.get(CV_CAP_PROP_FRAME_COUNT)/cap.get(CV_CAP_PROP_FPS);
    std::cout<<"Duração: "<<(secs/3600)<<":"<<((secs%3600)/60)<<":"<<(secs%60)<<std::endl;

	return 0;
}
