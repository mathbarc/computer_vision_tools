#include "calibration_processor.hpp"
#include <iostream>
#include <opencv2/calib3d/calib3d.hpp>
#include <QMessageBox>

CalibrationProcessor::CalibrationProcessor(std::string path, CalibrationData input, cv::Size size, CalibrationType method)
    : path(path)
    , input(input)
    , method(method)
    , size(size)
{ }

void CalibrationProcessor::run()
{

    std::cout<<"Starting Calibration"<<std::endl;
    switch (method)
    {
        case CalibrationType::ZHENG:
                this->zhengCalibration();
            break;
        case CalibrationType::HOMOGRAPHY_DIR:
        case CalibrationType::HOMOGRAPHY_INV:
                this->homography();
            break;
        default:
            break;
    }

}

void CalibrationProcessor::zhengCalibration()
{

    if(this->input.image.size()<4){
        return;
    }

    std::vector< std::vector<cv::Point2f> > image;
    std::vector< std::vector<cv::Point3f> > world;

    std::vector< cv::Mat > rvecs, tvecs;

    image.push_back(this->input.image);
    world.push_back(this->input.world);

    cv::Mat calibrationMatrix, distortionCoeff;

    cv::calibrateCamera(world, image, this->size, calibrationMatrix,
                        distortionCoeff, rvecs, tvecs,
                        cv::CALIB_ZERO_TANGENT_DIST );

    cv::FileStorage file(path, cv::FileStorage::WRITE);
    file<<"type"<<"zheng";
    file<<"calibration"<<calibrationMatrix;
    file<<"distortion_coefficients"<<distortionCoeff;
    file.release();

}


void CalibrationProcessor::homography()
{
    cv::Mat mask;
    cv::Mat homographyMatrix;

    if(method==CalibrationType::HOMOGRAPHY_DIR)
    {
        homographyMatrix = cv::findHomography(this->input.image,this->input.worldToPoint2d(), mask);
    }
    else if(method==CalibrationType::HOMOGRAPHY_INV)
    {
        homographyMatrix = cv::findHomography(this->input.worldToPoint2d(),this->input.image, mask);
    }
    cv::FileStorage file(path, cv::FileStorage::WRITE);
    file<<"type"<<"homography";
    file<<"homography"<<homographyMatrix;
    file<<"mask"<<mask;
    file.release();
}
