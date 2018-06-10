/**************************
* Calibration    TSAI3D          *
*                                                    *
* Marcelo Gattass                    *
* Manuel E. L. Fernandez    *
* Jul06,2006                            *
**************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
 
#include "tsai3D.h"

#define PRINT 0
#define MAX_POINTS 512

void saveIntrinsicData(char* name, double* A, double *K, double distortion,
                                                struct tsai_camera_parameters CameraParameters)
{ 
    FILE* fpcsv=fopen(name,"wt");

    fprintf(fpcsv,"Camera parameters:\n");
    fprintf(fpcsv,"Ncx :    %.8lf \t     Nfx: %.8lf    \n",CameraParameters.Ncx,CameraParameters.Nfx);
    fprintf(fpcsv,"Cell size (dx,dy):    (%.8lf,%.8lf) \n",CameraParameters.dx,CameraParameters.dx);
    fprintf(fpcsv,"Image center (Cx,Cy): (%.8lf, %.8lf) \n",CameraParameters.Cx,CameraParameters.Cy);      
    fprintf(fpcsv,"sx : %.8lf  \n\n",CameraParameters.sx);      

    fprintf(fpcsv,"Calibration results:\n");
    fprintf(fpcsv,"Intrinsic parameters:\n");
    fprintf(fpcsv,"    %.8lf \t    %.8lf    \t %.8lf \n",A[0],A[1],A[2]);
    fprintf(fpcsv,"    %.8lf \t    %.8lf    \t %.8lf \n",A[3],A[4],A[5]);
    fprintf(fpcsv,"    %.8lf \t    %.8lf    \t %.8lf \n\n",A[6],A[7],A[8]);      
    
    fprintf(fpcsv,"Extrinsic parameters:\n");
    fprintf(fpcsv,"    %.8lf \t    %.8lf    \t %.8lf  \t %.8lf \n",K[0],K[1],K[2],K[3]);
    fprintf(fpcsv,"    %.8lf \t    %.8lf    \t %.8lf  \t %.8lf \n",K[4],K[5],K[6],K[7]);
    fprintf(fpcsv,"    %.8lf \t    %.8lf    \t %.8lf  \t %.8lf \n\n",K[8],K[9],K[10],K[11]);      
    
    fprintf(fpcsv,"Distortion parameters:\n");
    fprintf(fpcsv,"    %.8lf \n",distortion);
    fclose(fpcsv);
}


int loadModel(double* modelPoints, double* imagePoints)
{
   int n=0;
   FILE* fpi = fopen("data.txt","rt");
   
   if (fpi==NULL) { printf("Arq error\n"); return 1; }

   for (n=0; !feof(fpi); n++ ) {
      fscanf(fpi,"%lf %lf %lf %lf %lf",
                &modelPoints[3*n],&modelPoints[3*n +1],&modelPoints[3*n +2],
                &imagePoints[2*n],&imagePoints[2*n +1]);
   }

   fclose(fpi); 
     n--;
   return n;
}

int main( )
{
   double modelPoints[3*MAX_POINTS];
     double imagePoints[2*MAX_POINTS];
     
   double A[3*3], K[3*4],distortion;
   int n;
   
     struct tsai_camera_parameters CameraParameters;
     struct tsai_calibration_constants CalibrationResults;

     n = loadModel(modelPoints, imagePoints);
     /****************** INIT CAMERA PARAMETERS *****************/
     /* You have the next list of camera parameters for using in configuration your camera:
     - cameras
        - PHOTOMETRICS_STAR_I
        - GENERAL_IMAGING_MOS5300 
        - PANASONIC_GP_MF702            
        - SONY_XC75                                
        - SONY_XC77                                
        - SONY_XC57                                
     - Webcams
        - CREATIVE_WEBCAM_NX_PRO            
        - LOGITECH_QUICKCAM_PRO_4000    
     - standard and own cameras
        - PINHOLE_CAMERA                    
        -    NEW_CAMERA                            
     Only for the last two option you need to indicate values for camera parameters, in the other types 
     you need put the ID of the camera like "PHOTOMETRICS_STAR_I" and the rest fill with "0" values.
   
     - Parameters (Camera Model  ,Ncx,Nfx,dx,dy, Cx,Cy, sx, struct tsai_camera_parameters *cp)
     
     In the "PINHOLE_CAMERA" you only need fill the values to center of images (width/2,height/2).
     In the "NEW_CAMERA" you fill the know values of the camera parameters that you will use.
     */
     tsai3D_define_camera_parameters(PHOTOMETRICS_STAR_I,0,0,0,0,0,0,0,&CameraParameters);
     printf("\n****************** GET TSAI 3D CALIBRATION *****************\n");
     if(tsai3D_calibration(n, modelPoints, imagePoints,A,K,&distortion,&CameraParameters,&CalibrationResults)){
          /*Save results in a file: Calib.txt*/
            saveIntrinsicData("Calib.txt",A,K,distortion,CameraParameters);
            printf("\n\nCamera parameters:\n");
            printf("\nNcx :    %.8lf \t     Nfx: %.8lf    \n",CameraParameters.Ncx,CameraParameters.Nfx);
            printf("\nCell size (dx,dy):    (%.8lf,%.8lf) \n",CameraParameters.dx,CameraParameters.dx);
            printf("\nImage center (Cx,Cy): (%.8lf, %.8lf) \n",CameraParameters.Cx,CameraParameters.Cy);      
            printf("\nsx : %.8lf  \n\n",CameraParameters.sx);      

            printf("\nCalibration results:\n");
            printf("\nIntrinsic parameters:\n");
            printf("%.8lf %.8lf     %.8lf \n",A[0],A[1],A[2]);
            printf("%.8lf %.8lf     %.8lf \n",A[3],A[4],A[5]);
            printf("%.8lf %.8lf     %.8lf \n\n",A[6],A[7],A[8]);      

            printf("\nExtrinsic parameters:\n");
            printf("%.8lf  %.8lf    %.8lf  %.8lf \n",K[0],K[1],K[2],K[3]);
            printf("%.8lf  %.8lf    %.8lf  %.8lf \n",K[4],K[5],K[6],K[7]);
            printf("%.8lf  %.8lf    %.8lf  %.8lf \n\n",K[8],K[9],K[10],K[11]);      

            printf("\nDistortion parameters: \t %.8lf ",distortion);
     }
     else{
         printf("\nERROR IN CALIBRATION PROCESS");
     };

     printf("\n\n");
     scanf("%c");
   return 0;
}





