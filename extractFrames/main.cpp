#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <boost/filesystem.hpp>

int main(int argc, char** argv){
	if(argc<4){
		std::cout<<argv[0]<<" Video quantidade_frames caminho arquivo.xml"<<std::endl;
		exit(-1);
	}

    boost::filesystem::path videoPath(argv[1]);
    cv::VideoCapture video(argv[1]);

    int frame_bin = atoi(argv[2]);

	std::fstream file(argv[4],std::ios::out);
	file<<"<?xml version=\"1.0\"?>\n<opencv_storage>\n<images>";
	int i = 0;
	cv::Mat frame;
	int n = 0;
	while(1){
		std::stringstream ss;
        ss <<argv[3] <<"/"<< videoPath.stem().string() << "_"<< i << ".png";
		video>>frame;
		if(frame.empty())
			break;
		n++;
		file<<ss.str()<<"\n";
		cv::imwrite(ss.str(),frame);
		video.set(CV_CAP_PROP_POS_FRAMES,i);
		i+=frame_bin;
	}
	std::cout<<n<<" frames extraídos"<<std::endl;
	video.release();
	file<<"</images>\n</opencv_storage>";
	file.close();
	return 0;
}
