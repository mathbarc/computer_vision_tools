#ifndef VIDEO_CPP
#define VIDEO_CPP 1
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

using namespace cv;
using namespace std;

class Video{
	private:
		string janela; 			//Nome da janela no qual o vídeo é exibido
		VideoCapture* video; 		//Vídeo de entrada
	public:
		Mat frameAtual; 		//Último frame obtido
		float fps;			//Quantidade de frames por segundo no vídeo
		unsigned int nframes;		//Quantidade total de frames no vídeo
                Size resolucao;			//Resolução do vídeo
                int trackbar;
		
		Video(string caminho);		//Método criador da classe
		~Video();			//Método destrutor
		void atualizar(int posicao);	//Método para atualizar o frame exibido a partir da posição do trackbar
		Mat nextFrame();		//Método para obter o próximo frame do vídeo
		
};

#endif
