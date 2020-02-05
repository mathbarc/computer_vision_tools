#include "Video.h"
#include <iostream>
#include <vector>
#include <string>

using namespace cv;
using namespace std;

vector<Video*> videos;
vector<VideoWriter*> videosOut;

void atualizar(int position, void* v){
	Video* vi = (Video*) v;
	vi->atualizar(position);
}


int main(int argc, char* args[]){

	cout<<argc<<endl;

	if(argc<2){
		cout<<"São necessários pelo menos dois vídeos para realizar a sincronização. "<<endl;
		return -2;
	}
	for(unsigned int i = 1; i<argc; i++){
		try{
			cout<<args[i]<<" "<<i<<endl;
			videos.push_back(new Video(args[i]));
			createTrackbar("Nº Frame: ",args[i],&videos[videos.size()-1]->trackbar,videos[videos.size()-1]->nframes,atualizar,(void*)videos[videos.size()-1]);
		}
		catch(string s){
			cout<<s<<endl;
		}
	}

	bool continuar = true;
	char key;
	vector<VideoWriter*> out;
	stringstream name;
	for(int i = 0; i<videos.size();i++){
        name << i << ".avi";
        out.push_back(new VideoWriter(name.str(),VideoWriter::fourcc('D','I','V','X'),videos[i]->fps,videos[i]->resolucao));
		name.str("");
		name.clear();
	}
	Mat* m = new Mat[out.size()];
	do{
		key = waitKey(0);
		switch(key){
			case 27: continuar=false;
				 break;
			case 's':
			case 'S':
				 do{
                    for(int i = 0; (i<out.size())&&continuar ;i++)
                    {
						m[i] = videos[i]->nextFrame();
						if(m[i].empty())
							continuar=false;

					}
					if(continuar){
						for(int i = 0; i<out.size();i++){
							out[i]->write(m[i]);
						}	
					}
					if(waitKey(1)==27)
						continuar=false;
				 }while(continuar);
				 
				 for(int i =0; i<out.size(); i++){
					out[i]->release();
				 }
				 
				 break;
			default:
				break;
		}

	}while(continuar);


	cout<<"ok"<<endl;
	destroyAllWindows();
	videos.clear();
	out.clear();
}
