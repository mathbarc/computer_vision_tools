#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <pcl/visualization/cloud_viewer.h>

cv::VideoCapture cap(1, cv::CAP_OPENNI2);

void getPointCloud(pcl::visualization::PCLVisualizer& viewer)
{
    viewer.removeAllPointClouds();

    cv::Mat pointCloudFrame, frame;

    cap.grab();
    cap.retrieve(pointCloudFrame, cv::CAP_OPENNI_POINT_CLOUD_MAP);
    cap.retrieve(frame, cv::CAP_OPENNI_QVGA_60HZ);

    std::cout<<frame.type()<<" "<<frame.size()<<std::endl;

    pcl::visualization::CloudViewer::ColorCloud* cloud = new pcl::visualization::CloudViewer::ColorCloud();


    for(int i = 0; i<pointCloudFrame.rows; i++)
    {
        for(int j = 0; j<pointCloudFrame.cols; j++)
        {
            uchar color = frame.at<uchar>(i,j);
            pcl::PointXYZRGB point(color, color, color );

            cv::Vec3f coords = pointCloudFrame.at<cv::Vec3f>(i,j);
            point.x = coords[0];
            point.y = coords[1];
            point.z = coords[2];


            cloud->push_back(point);
        }
    }

    viewer.addPointCloud(pcl::visualization::CloudViewer::ColorCloud::ConstPtr(cloud));


}


int main(int argc, char** argv)
{


    pcl::visualization::CloudViewer viewer("point cloud");
    viewer.runOnVisualizationThread(getPointCloud);

    while(!viewer.wasStopped())
    {

    }

    cap.release();
//    vid.release();
    
    return 0;

}
