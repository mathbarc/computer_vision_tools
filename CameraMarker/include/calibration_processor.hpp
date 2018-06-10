#ifndef CALIBRATION_PROCESSOR_HPP
#define CALIBRATION_PROCESSOR_HPP

#include <QThread>
#include <vector>
#include "data_types.hpp"

class CalibrationProcessor : public QThread
{
    Q_OBJECT
    public:
        enum class CalibrationType
        {
            HOMOGRAPHY_DIR = 2,
            HOMOGRAPHY_INV = 1,
            ZHENG = 0
        };
        CalibrationProcessor(std::string path, CalibrationData input, cv::Size size, CalibrationType method);


    private:
        CalibrationData input;
        std::string path;
        CalibrationType method;
        cv::Size size;

        void zhengCalibration();
        void homography();

    public slots:
        void run();

};

#endif // CALIBRATION_PROCESSOR_HPP
