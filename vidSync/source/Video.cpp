#include "Video.h"

using namespace cv;
using namespace std;


/*******************************************************************************************
 Função: Video::atualizar
 Parâmetros:
	posicao - um inteiro entre
 Retorno: N/A
 Descrição: 
********************************************************************************************/

void Video::atualizar(int posicao){
	this->video->set(CV_CAP_PROP_POS_FRAMES,posicao);
	*(this->video) >> this->frameAtual;
	imshow(this->janela, this->frameAtual);
}

/*******************************************************************************************
 Função: Video::Video
 Parâmetros:
	caminho - 
 Retorno: N/A
 Descrição: 
********************************************************************************************/

Video::Video(string caminho)
    : trackbar(0)
{
	this->video = new VideoCapture(caminho);
	if(!this->video->isOpened()){
		throw caminho+" não pode ser encontrado";
	}
	this->janela = caminho;
	*(this->video) >> this->frameAtual;
	namedWindow(caminho,WINDOW_NORMAL);
	this->nframes = this->video->get(CV_CAP_PROP_FRAME_COUNT);
	this->fps = this->video->get(CV_CAP_PROP_FPS);
	this->resolucao = Size((int) this->video->get(CV_CAP_PROP_FRAME_WIDTH),
                  	(int) this->video->get(CV_CAP_PROP_FRAME_HEIGHT));
	imshow(this->janela,this->frameAtual);	
}

/*******************************************************************************************
 Função: Video::nextFrame
 Parâmetros: N/A
 Retorno: this->frameAtual, cv::Mat
 Descrição: 
********************************************************************************************/

Mat Video::nextFrame(){
	*(this->video) >> this->frameAtual;
	if(!this->frameAtual.empty())
		imshow(this->janela,this->frameAtual);	
	return this->frameAtual;

}

Video::~Video(){
	this->video->release();
	delete(this->video);
}
