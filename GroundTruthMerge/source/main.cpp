#include <opencv2/core/core.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>


int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        return -1;
    }
    if(argc%2!=1)
    {
        return -2;
    }

    std::map<int, std::vector<cv::Point> > marks;

    std::cout<< "Parsing files" << std::endl;

    for(int i = 1; i<argc; i+=2)
    {
        std::cout<< "Parsing "<<i<<": "<< argv[i] <<" "<<argv[i+1]<<std::endl;
        cv::FileStorage hFile(argv[i+1], cv::FileStorage::READ);

        cv::Mat1d homography;

        hFile["homography"] >> homography;

        hFile.release();

        std::fstream file(argv[i], std::fstream::in);


        std::string line;

        while(std::getline(file,line))
        {

            std::stringstream ss(line);

            std::string cell;

            std::getline(ss,cell);
            int frame = std::stoi(cell);

            std::getline(ss,cell);
            int x = std::stoi(cell);

            std::getline(ss,cell);
            int y = std::stoi(cell);

            std::getline(ss,cell);
            int width = std::stoi(cell);

            std::getline(ss,cell);
            int height = std::stoi(cell);

            cv::Mat1d p(3,1);


            p.at<double>(0,0) = x + width/2.0;
            p.at<double>(1,0) = y + height/2.0;
            p.at<double>(2,0) = 1.0;

            cv::Mat1d pq(3,1);

            pq = homography * p;

            marks[frame].emplace_back(cv::Point(pq(0,0)/pq(2,0), pq(1,0)/pq(2,0)));

        }

        file.close();
    }

    std::cout<<"Removing repetitions"<<std::endl;

    for(auto& m : marks)
    {
        auto& v = m.second;
        cv::Point ref;
        for(int i = 0; i<v.size(); i++)
        {
            ref = v[i];
            for(int j = i+1; j < v.size(); j++)
            {
                if(ref.x == v[j].x && ref.y == v[j].y)
                {
                    v.erase(v.begin()+j);
                    j--;
                }
            }

        }
    }


    std::cout<<"Generating output file"<<std::endl;
    std::fstream out("out.csv", std::fstream::out);


    for(auto& m : marks)
    {
        int f = m.first;
        auto& v = m.second;

        for(const cv::Point& p : v )
        {
            out<< f << " " << p.x << " " << p.y << std::endl;
        }
    }

    out.close();

    std::cout<<"Done"<<std::endl;



    return 0;
}
