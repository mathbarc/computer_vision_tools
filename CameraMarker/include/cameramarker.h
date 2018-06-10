#ifndef CAMERAMARKER_H
#define CAMERAMARKER_H

#include <QtWidgets/QMainWindow>
#include <opencv2/core/core.hpp>
#include "data_types.hpp"

namespace Ui{
    class CameraMarker;
}

class CameraMarker : public QMainWindow
{
	Q_OBJECT

    public:
        explicit CameraMarker(QWidget *parent = 0);
        ~CameraMarker();
        std::vector<QPoint> points;


    signals:
        void openVideo(QString path);
        void genCalibFile(QString path);
        void genHomoDirFile(QString path);
        void genHomoInvFile(QString path);
        void playPauseVideo();
        void addCalibrationMarker(CalibrationMarker cm);

    public slots:
        void showImage(QImage img);
        void insertOnTable(CalibrationMarker cm);

    private slots:
        void on_actionAbrir_Video_triggered();
        void on_actionGerar_Calib_Arq_triggered();
        void on_actionGerar_Homo_Arq_triggered();
        void on_actionGerar_arquivo_de_homografia_inversa_triggered();

    private:
        Ui::CameraMarker* ui;

    protected:
        void keyPressEvent(QKeyEvent* event);
        void mousePressEvent(QMouseEvent* event);
};

#endif // CAMERAMARKER_H
