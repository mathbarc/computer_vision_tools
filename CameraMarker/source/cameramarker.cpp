#include "cameramarker.h"
#include "ui_cameramarker.h"
#include "dialog_world_point.hpp"
#include <QMessageBox>
#include <QFileDialog>
#include <iostream>
#include <QKeyEvent>
#include <QKeySequence>
#include <opencv2/core/core.hpp>

CameraMarker::CameraMarker(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CameraMarker)
{
    ui->setupUi(this);
    this->setMouseTracking(true);
    this->ui->scrollArea->setVisible(true);

}

CameraMarker::~CameraMarker()
{
    delete ui;
}

void CameraMarker::on_actionAbrir_Video_triggered()
{
    QString path = QFileDialog::getOpenFileName(0,"Abrir Vídeo", "", "MP4 (*.mp4);;MPEG (*.mpg *.mpeg *.MPG *.MPEG);;AVI (*.avi *.AVI)");
    emit openVideo(path);

}

void CameraMarker::showImage(QImage img)
{
    int x = ((this->ui->scrollAreaWidgetContents->width() - img.width())/2);
    int y = ((this->ui->scrollAreaWidgetContents->height() - img.height())/2);

    if(x>0)
    {
        this->ui->label_frame->move(x,y);
    }
    else
    {
        this->ui->label_frame->move(0,0);
    }


    this->ui->label_frame->resize(img.width(),img.height());
    this->ui->label_frame->setPixmap(QPixmap::fromImage(img));

}

void CameraMarker::keyPressEvent(QKeyEvent *event)
{
    this->focusPolicy();

    if(event->matches(QKeySequence::Open)){
        QString path = QFileDialog::getOpenFileName(0,"Abrir Vídeo", "", "MP4 (*.mp4);;MPEG (*.mpg *.mpeg *.MPG *.MPEG);;AVI (*.avi *.AVI)");
        emit openVideo(path);
    }

    if(event->matches(QKeySequence::Close)|| event->key()==Qt::Key_Escape){
        emit this->close();
    }

    if(event->key() == Qt::Key_Space){
        emit playPauseVideo();
    }

}

void CameraMarker::mousePressEvent(QMouseEvent* event)
{
    QPoint p = this->ui->label_frame->mapFrom(this,event->pos());
    QSize s = this->ui->label_frame->size();
    if(p.x() >= 0 && p.y() >= 0 && p.x() < s.width() && p.y() < s.height())
    {
        cv::Point2d imagePoint;
        cv::Point3d worldPoint;

        imagePoint.x = p.x();
        imagePoint.y = p.y();

        worldPoint = DialogWorldPoint::getWorldPoint(this);
        CalibrationMarker cm(imagePoint, worldPoint);
        emit addCalibrationMarker(cm);
    }
}

void CameraMarker::insertOnTable(CalibrationMarker cm)
{
    int lastRow = this->ui->tableWidget->rowCount();
    this->ui->tableWidget->insertRow(lastRow);

    this->ui->tableWidget->setItem(lastRow,0,
                                   new QTableWidgetItem (QString::number(cm.imagePoint.x)));
    this->ui->tableWidget->setItem(lastRow,1,
                                   new QTableWidgetItem (QString::number(cm.imagePoint.y)));
    this->ui->tableWidget->setItem(lastRow,2,
                                   new QTableWidgetItem (QString::number(cm.worldPoint.x)));
    this->ui->tableWidget->setItem(lastRow,3,
                                   new QTableWidgetItem (QString::number(cm.worldPoint.y)));
    this->ui->tableWidget->setItem(lastRow,4,
                                   new QTableWidgetItem (QString::number(cm.worldPoint.z)));


}

void CameraMarker::on_actionGerar_Calib_Arq_triggered()
{

    if(this->ui->tableWidget->rowCount()!=0)
    {
        QString file = QFileDialog::getSaveFileName(0,"Salvar Arquivo", "Calib", "XML (*.xml);;YML (*.yml)");
        std::cout<<file.toStdString()<<std::endl;

        if(file!="")
            emit genCalibFile(file);
    }
}


void CameraMarker::on_actionGerar_Homo_Arq_triggered()
{
    if(this->ui->tableWidget->rowCount()!=0)
    {
        QString file = QFileDialog::getSaveFileName(0,"Salvar Arquivo", "Homography", "XML (*.xml);;YML (*.yml)");
        std::cout<<file.toStdString()<<std::endl;

        if(file!="" && this->ui->tableWidget->rowCount()!=0)
            emit genHomoDirFile(file);
    }
}

void CameraMarker::on_actionGerar_arquivo_de_homografia_inversa_triggered()
{
    if(this->ui->tableWidget->rowCount()!=0)
    {
        QString file = QFileDialog::getSaveFileName(0,"Salvar Arquivo", "Homography", "XML (*.xml);;YML (*.yml)");
        std::cout<<file.toStdString()<<std::endl;

        if(file!="" && this->ui->tableWidget->rowCount()!=0)
            emit genHomoInvFile(file);
    }
}

