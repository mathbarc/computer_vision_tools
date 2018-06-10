/**************************
* Calibration    TSAI3D          *
*                                                    *
* Marcelo Gattass                    *
* Manuel E. L. Fernandez    *
* Jul06,2006                            *
**************************/
#include "tsai3D.h"

#define DEBUG 0
/***********************************************************************\
* Auxiliar global pointers                                                                                            *
* used only for internal calculations                                                                        * 
\***********************************************************************/
struct tsai_calibration_data *cd_temp; 
struct tsai_calibration_constants *cc_temp;
struct tsai_camera_parameters *cp_temp;
double *Xd,*Yd,*r_squared;
/***********************************************************************\
* Functions to allocate memory space to calibration variables                        *
\***********************************************************************/
void tsai3D_init_cp(struct tsai_camera_parameters *cp) {
    cp->Ncx = 0.0;        /* [sel] Number of sensor elements in camera's x direction */
    cp->Nfx = 0.0;        /* [pix] Number of pixels in frame grabber's x direction  */
    cp->dx  = 0.0;        /* [mm/sel] X dimension of camera's sensor element (in mm) */
    cp->dy  = 0.0;        /* [mm/sel] Y dimension of camera's sensor element (in mm) */
    cp->dpx = 0.0;        /* [mm/pix] Effective X dimension of pixel in frame grabber  */
    cp->dpy = 0.0;        /* [mm/pix] Effective Y dimension of pixel in frame grabber  */
    cp->Cx  = 0.0;        /* [pix] Z axis intercept of camera coordinate system  */
    cp->Cy  = 0.0;        /* [pix] Z axis intercept of camera coordinate system  */
    cp->sx  = 0.0;        /* [] Scale factor to compensate for any error in dpx  */

    cp->is_load_paremeters = 0; /*  flag to indicate that all parameters have been loaded */
}

void tsai3D_set_cp(struct tsai_camera_parameters *cp) {
    if(cp->is_load_paremeters == 1){
        cp->dpx = cp->dx * cp->Ncx / cp->Nfx;        /* [mm/pix] Effective X dimension of pixel in frame grabber  */
        cp->dpy = cp->dy;                                                /* [mm/pix] Effective Y dimension of pixel in frame grabber  */
        cp->sx  = 1.0;                                                    /* [] Scale factor to compensate for any error in dpx  */        
    };
}

void tsai3D_init_cd(struct tsai_calibration_data* cd){
    cd->point_count = 0;    /* [points]      */
    cd->xw = 0;    /* [mm]          */
    cd->yw = 0;    /* [mm]          */
    cd->zw = 0;    /* [mm]          */
    cd->Xf = 0;    /* [pix]         */
    cd->Yf = 0;    /* [pix]         */
}

void tsai3D_free_Memory_cd(struct tsai_calibration_data* cd){
    if(cd->point_count != 0){
        free(cd->xw);
        free(cd->yw);
        free(cd->zw);
        free(cd->Xf);
        free(cd->Yf);
        cd->point_count =0;
    };
}

int tsai3D_allocate_Memory_cd(struct tsai_calibration_data* cd , int set_nro_points)
{
    if(cd->point_count != 0){
        free(cd->xw);
        free(cd->yw);
        free(cd->zw);
        free(cd->Xf);
        free(cd->Yf);
    };
    cd->xw = (double*)malloc(sizeof(double)*set_nro_points);
    cd->yw = (double*)malloc(sizeof(double)*set_nro_points);
    cd->zw = (double*)malloc(sizeof(double)*set_nro_points);
    cd->Xf = (double*)malloc(sizeof(double)*set_nro_points);
    cd->Yf = (double*)malloc(sizeof(double)*set_nro_points);
    if(cd->xw && cd->yw && cd->zw && cd->Xf && cd->Yf){
        cd->point_count = set_nro_points;
        return 1;
    }
    else{
        return 0;
    };
}

void tsai3D_init_cc(struct tsai_calibration_constants* cc ){
    cc->f = 0.0;        /* [mm]          */
    cc->kappa1 = 0.0;        /* [1/mm^2]      */
    cc->p1 = 0.0;        /* [1/mm]        */
    cc->p2= 0.0;        /* [1/mm]        */
    cc->Tx= 0.0;        /* [mm]          */
    cc->Ty= 0.0;        /* [mm]          */
    cc->Tz= 0.0;        /* [mm]          */
    cc->Rx= 0.0;        /* [rad]     */
    cc->Ry= 0.0;        /* [rad]     */
    cc->Rz= 0.0;        /* [rad]     */
    cc->r1= 0.0;        /* []            */
    cc->r2= 0.0;        /* []            */
    cc->r3= 0.0;        /* []            */
    cc->r4= 0.0;        /* []            */
    cc->r5= 0.0;        /* []            */
    cc->r6= 0.0;        /* []            */
    cc->r7= 0.0;        /* []            */
    cc->r8= 0.0;        /* []            */
    cc->r9= 0.0;        /* []            */

    cc->is_load_constants = 0; /*      flag to indicate that all parameters have been loaded  */
}

/***********************************************************************\
* Math routines for Non-coplanar camera calibration                                                         *
\***********************************************************************/
void SINCOSf(double x, double* s, double* c)
{
    *s = (double)sin(x); *c = (double)cos(x);
};
double SQRf(double x){ 
    return (x*x);
}

int SIGNBITf(double x){ 
    if(x > 0.f) 
        return 0; 
    else 
        return 1;
}
/***********************************************************************\
* This routine solves for the roll, pitch and yaw angles (in radians)    *
* for a given orthonormal rotation matrix (from Richard P. Paul,        *
* Robot Manipulators: Mathematics, Programming and Control, p70).       *
* Note 1, should the rotation matrix not be orthonormal these will not  *
* be the "best fit" roll, pitch and yaw angles.                         *
* Note 2, there are actually two possible solutions for the matrix.     *
* The second solution can be found by adding 180 degrees to Rz before   *
* Ry and Rx are calculated.                                             *
\***********************************************************************/
void solve_RPY_transform (struct tsai_calibration_constants *cc)
{
    double    sg,cg;
    sg = 0.0; cg = 0.0;
    cc->Rz = (double)atan2 (cc->r4, cc->r1);
    SINCOSf(cc->Rz, &sg, &cg);
    cc->Ry = (double)atan2 (-cc->r7, cc->r1 * cg + cc->r4 * sg);
    cc->Rx = (double)atan2 (cc->r3 * sg - cc->r6 * cg, cc->r5 * cg - cc->r2 * sg);
}

/***********************************************************************\
* Functions to implement Tsai Non-Coplanar calibration                                    *
\***********************************************************************/
void tsai3D_compute_Xd_Yd_and_r_squared (struct tsai_calibration_data *cd,
                                                                                    struct tsai_camera_parameters *cp, 
                                                                                    double *Xd, double *Yd, double *r_squared)
{
    int    i;
    double  Xd_,Yd_;
    for(i=0; i<cd->point_count; i++){
        Xd[i] = Xd_ = cp->dpx * (cd->Xf[i] - cp->Cx) / cp->sx;    // [mm] 
        Yd[i] = Yd_ = cp->dpy * (cd->Yf[i] - cp->Cy);                        // [mm] 
        r_squared[i] = SQRf(Xd_) + SQRf(Yd_);                                        // [mm^2] 
    }
}

int  tsai3D_compute_U (struct tsai_calibration_data* cd, double *Xd, double *Yd, double *U)
{
    int      i,line;
    double   *A,*x,*b;
    
    A = (double*)malloc(sizeof(double)*(cd->point_count*7)); 
    x =  (double*)malloc(sizeof(double)*7); 
    b =  (double*)malloc(sizeof(double)*cd->point_count); 
    
    for (i = 0; i < cd->point_count; i++) {
        line = i*7;
        A[line + 0] = Yd[i] * cd->xw[i];
        A[line + 1] = Yd[i] * cd->yw[i];
        A[line + 2] = Yd[i] * cd->zw[i];
        A[line + 3] = Yd[i];
        A[line + 4] = -Xd[i] * cd->xw[i];
        A[line + 5] = -Xd[i] * cd->yw[i];
        A[line + 6] = -Xd[i] * cd->zw[i];
        b[i] = Xd[i];
    }
    mtxAx_b(A,cd->point_count,7,b,x);
    
    U[0] = x[0];
    U[1] = x[1];
    U[2] = x[2];
    U[3] = x[3];
    U[4] = x[4];
    U[5] = x[5];
    U[6] = x[6];

    return 1;
}

void tsai3D_compute_Tx_and_Ty (    struct tsai_calibration_data *cd,
                                                                struct tsai_calibration_constants *cc,
                                                                double *Xd, double *Yd, double *r_squared, double *U)
{
    int      i,far_point;
    double    Tx,Ty,Ty_squared,x,y,r1,r2,r3,r4,r5,r6,distance,far_distance;

    // first find the square of the magnitude of Ty 
    Ty_squared = 1.0 / (SQRf(U[4]) + SQRf(U[5]) + SQRf(U[6]));

    // find a point that is far from the image center 
    far_distance = 0;
    far_point = 0;
    for (i = 0; i < cd->point_count; i++){
        if ((distance = r_squared[i]) > far_distance) {
            far_point = i;
            far_distance = distance;
        }
    }

    // now find the sign for Ty 
    // start by assuming Ty > 0 
    Ty = (double)sqrt(Ty_squared);
    r1 = U[0] * Ty;
    r2 = U[1] * Ty;
    r3 = U[2] * Ty;
    Tx = U[3] * Ty;
    r4 = U[4] * Ty;
    r5 = U[5] * Ty;
    r6 = U[6] * Ty;
    x = r1 * cd->xw[far_point] + r2 * cd->yw[far_point] + r3 * cd->zw[far_point] + Tx;
    y = r4 * cd->xw[far_point] + r5 * cd->yw[far_point] + r6 * cd->zw[far_point] + Ty;

    // flip Ty if we guessed wrong 
    if ((SIGNBITf (x) != SIGNBITf (Xd[far_point])) ||
            (SIGNBITf (y) != SIGNBITf (Yd[far_point])))
        Ty = -Ty;

    // update the calibration constants 
    cc->Tx = U[3] * Ty;
    cc->Ty = Ty;
}

void tsai3D_compute_sx (    struct tsai_camera_parameters *cp,
                                                struct tsai_calibration_constants *cc,
                                                double *U)
{
    cp->sx = (double)sqrt(SQRf (U[0]) + SQRf (U[1]) + SQRf (U[2])) * (double)fabs (cc->Ty);    
}

void tsai3D_compute_R (    struct tsai_camera_parameters* cp,
                                                struct tsai_calibration_constants *cc, 
                                                double *U)
{
    double  r1,r2,r3,r4,r5,r6,r7,r8,r9;

    r1 = U[0] * cc->Ty / cp->sx;
    r2 = U[1] * cc->Ty / cp->sx;
    r3 = U[2] * cc->Ty / cp->sx;

    r4 = U[4] * cc->Ty;
    r5 = U[5] * cc->Ty;
    r6 = U[6] * cc->Ty;

    // use the outer product of the first two rows to get the last row 
    r7 = r2 * r6 - r3 * r5;
    r8 = r3 * r4 - r1 * r6;
    r9 = r1 * r5 - r2 * r4;

    // update the calibration constants 
    cc->r1 = r1;
    cc->r2 = r2;
    cc->r3 = r3;
    cc->r4 = r4;
    cc->r5 = r5;
    cc->r6 = r6;
    cc->r7 = r7;
    cc->r8 = r8;
    cc->r9 = r9;

    // fill in cc.Rx, cc.Ry and cc.Rz 
    solve_RPY_transform (cc);
}


void tsai3D_compute_better_R (    struct tsai_camera_parameters* cp,
                                                            struct tsai_calibration_constants *cc, 
                                                            double *U)
{
    double    r1,r2,r3,r4,r5,r6,r7,sa,ca,sb,cb,sg,cg;

    r1 = U[0] * cc->Ty / cp->sx;
    r2 = U[1] * cc->Ty / cp->sx;
    r3 = U[2] * cc->Ty / cp->sx;

    r4 = U[4] * cc->Ty;
    r5 = U[5] * cc->Ty;
    r6 = U[6] * cc->Ty;

    // use the outer product of the first two rows to get the last row 
    r7 = r2 * r6 - r3 * r5;

    // now find the RPY angles corresponding to the estimated rotation matrix 
    cc->Rz = (double)atan2 (r4, r1);

    SINCOSf(cc->Rz, &sg, &cg);

    cc->Ry = (double)atan2 (-r7, r1 * cg + r4 * sg);

    cc->Rx = (double)atan2 (r3 * sg - r6 * cg, r5 * cg - r2 * sg);

    SINCOSf(cc->Rx, &sa, &ca);

    SINCOSf(cc->Ry, &sb, &cb);

    // now generate a more orthonormal rotation matrix from the RPY angles 
    cc->r1 = cb * cg;
    cc->r2 = cg * sa * sb - ca * sg;
    cc->r3 = sa * sg + ca * cg * sb;
    cc->r4 = cb * sg;
    cc->r5 = sa * sb * sg + ca * cg;
    cc->r6 = ca * sb * sg - cg * sa;
    cc->r7 = -sb;
    cc->r8 = cb * sa;
    cc->r9 = ca * cb;
}

int  tsai3D_compute_approximate_f_and_Tz (    struct tsai_calibration_data *cd,
                                                                                    struct tsai_calibration_constants *cc, 
                                                                                    double *Yd)
{
    int            i,line;
    double    *A,*x,*b;
    
    A = (double*)malloc(sizeof(double)*(cd->point_count*2)); 
    x = (double*)malloc(sizeof(double)*2); 
    b = (double*)malloc(sizeof(double)*cd->point_count); 

    for (i = 0; i < cd->point_count; i++) {
        line = i*2;
        A[line + 0] = cc->r4 * cd->xw[i] + cc->r5 * cd->yw[i] + cc->r6 * cd->zw[i] + cc->Ty;
        A[line + 1] = -Yd[i];
        b[i] = (cc->r7 * cd->xw[i] + cc->r8 * cd->yw[i] + cc->r9 * cd->zw[i]) * Yd[i];
    }
    mtxAx_b(A,cd->point_count,2,b,x);
    // update the calibration constants 
    cc->f = x[0];
    cc->Tz = x[1];
    cc->kappa1 = 0.0;        // this is the assumption that our calculation was made under 
    return 1;
}


/************************************************************************/

void tsai3D_compute_exact_f_and_Tz_error_cminpack(int  m_ptr,int n_ptr,double *params,
                                                                                     double *err, int *erro)
{    // pointer to number of points to fit 
    // pointer to number of parameters 
    // vector of parameters 
    // vector of error from data 
    int       i;
    double    xc,yc,zc, Xu_1,Yu_1, Xu_2,Yu_2, distortion_factor,f,Tz,kappa1;
    double errAVG;

    f = params[0];
    Tz = params[1];
    kappa1 = params[2];

    errAVG = 0.0;

    for (i = 0; i < cd_temp->point_count; i++) {
        // convert from world coordinates to camera coordinates 
        xc = cc_temp->r1 * cd_temp->xw[i] + cc_temp->r2 * cd_temp->yw[i] + cc_temp->r3 * cd_temp->zw[i] + cc_temp->Tx;
        yc = cc_temp->r4 * cd_temp->xw[i] + cc_temp->r5 * cd_temp->yw[i] + cc_temp->r6 * cd_temp->zw[i] + cc_temp->Ty;
        zc = cc_temp->r7 * cd_temp->xw[i] + cc_temp->r8 * cd_temp->yw[i] + cc_temp->r9 * cd_temp->zw[i] + Tz;

        // convert from camera coordinates to undistorted sensor coordinates 
        Xu_1 = f * xc / zc;
        Yu_1 = f * yc / zc;

        // convert from distorted sensor coordinates to undistorted sensor coordinates 
        distortion_factor = 1 + kappa1 * r_squared[i];
        Xu_2 = Xd[i] * distortion_factor;
        Yu_2 = Yd[i] * distortion_factor;

        // record the error in the undistorted sensor coordinates 
        err[i] = hypot (Xu_1 - Xu_2, Yu_1 - Yu_2);
        errAVG+=err[i];
    };
    if(DEBUG)    
        printf("\nErro medio= %lf",errAVG/cd_temp->point_count);
}


void tsai3D_compute_exact_f_and_Tz_cminpack ()
{
    #define NPARAMS 3

    // Parameters needed by simplified "C" version of Levenberg-Marquardt method 

    int     m = cd_temp->point_count;
    int     n = NPARAMS;
    double  *x;    //    x[NPARAMS];
    int     *msk;  // msk[NPARAMS];
    double  *fvec; // fvec[m];
    double  tol = 1.0e-10;
    int     info;
    int     nfev;

    // allocate some workspace 
    if ( (x = (double*)malloc(NPARAMS * sizeof(double))) == NULL ) 
    {
        printf("malloc: Cannot allocate workspace x\n");
        return;
    }

    if ( (msk = (int*)malloc(NPARAMS * sizeof(int))) == NULL ) 
    {
        printf("malloc: Cannot allocate workspace msk\n");
        return;
    }

    if ( (fvec = (double*)malloc(m * sizeof(double))) == NULL ) 
    {
        printf("malloc: Cannot allocate workspace fvec\n");
        return;
    }

    // use the current calibration constants as an initial guess 
    x[0] = cc_temp->f;
    x[1] = cc_temp->Tz;
    x[2] = cc_temp->kappa1;

    // Here we call a function Levenberg-Marquardt Nonlinear optimization in "C" 
    // minpack simplified version
    if(lmdif0(tsai3D_compute_exact_f_and_Tz_error_cminpack,m,n,x,msk,fvec,tol,&info,&nfev)){
            printf("\nParameters error lmdif process \n");
            
    }
    else{
        printf("\nParameters successful lmdif process \n");
        cc_temp->f = x[0];
        cc_temp->Tz = x[1];
        cc_temp->kappa1 = x[2];
    }
    
    #undef NPARAMS
    return;
}


/*********************************************************************/
void  tsai3D_define_camera_parameters(int CameraModel, 
                                                                            double Ncx,    double Nfx,
                                                                            double dx,   double dy,
                                                                            double Cx,    double Cy, double sx,
                                                                            struct tsai_camera_parameters *cp){

    switch(CameraModel) {
    // - cameras
    case PHOTOMETRICS_STAR_I:
        cp->Ncx = 576;        /* [sel]        */
        cp->Nfx = 576;        /* [pix]        */
        cp->dx = 0.023;        /* [mm/sel]     */
        cp->dy = 0.023;        /* [mm/sel]     */
        cp->dpx = cp->dx * cp->Ncx / cp->Nfx;    /* [mm/pix]     */
        cp->dpy = cp->dy;        /* [mm/pix]     */
        /* something a bit more realistic */
        cp->Cx = 258.0;
        cp->Cy = 204.0;
        cp->sx = 1.0;        /* []         */
      break;
    
    case GENERAL_IMAGING_MOS5300:
        cp->Ncx = 649;        /* [sel]        */
        cp->Nfx = 512;        /* [pix]        */
        cp->dx = 0.015;        /* [mm/sel]     */
        cp->dy = 0.015;        /* [mm/sel]     */
        cp->dpx = cp->dx * cp->Ncx / cp->Nfx;    /* [mm/pix]     */
        cp->dpy = cp->dy;        /* [mm/pix]     */
        cp->Cx = 512 / 2;        /* [pix]        */
        cp->Cy = 480 / 2;        /* [pix]        */
        cp->sx = 1.0;        /* []         */
        break;

    case PANASONIC_GP_MF702:
        cp->Ncx = 649;        /* [sel]        */
        cp->Nfx = 512;        /* [pix]        */
        cp->dx = 0.015;        /* [mm/sel]     */
        cp->dy = 0.015;        /* [mm/sel]     */
        cp->dpx = cp->dx * cp->Ncx / cp->Nfx;    /* [mm/pix]     */
        cp->dpy = cp->dy;        /* [mm/pix]     */
        cp->Cx = 268;        /* [pix]        */
        cp->Cy = 248;        /* [pix]        */
        cp->sx = 1.078647;        /* []           */
        break;

    case SONY_XC75:
        cp->Ncx = 768;        /* [sel]        */
        cp->Nfx = 512;        /* [pix]        */
        cp->dx = 0.0084;        /* [mm/sel]     */
        cp->dy = 0.0098;        /* [mm/sel]     */
        cp->dpx = cp->dx * cp->Ncx / cp->Nfx;    /* [mm/pix]     */
        cp->dpy = cp->dy;        /* [mm/pix]     */
        cp->Cx = 512 / 2;        /* [pix]        */
        cp->Cy = 480 / 2;        /* [pix]        */
        cp->sx = 1.0;        /* []           */
        break;

    case SONY_XC77:
        cp->Ncx = 768;        /* [sel]        */
        cp->Nfx = 512;        /* [pix]        */
        cp->dx = 0.011;        /* [mm/sel]     */
        cp->dy = 0.013;        /* [mm/sel]     */
        cp->dpx = cp->dx * cp->Ncx / cp->Nfx;    /* [mm/pix]     */
        cp->dpy = cp->dy;        /* [mm/pix]     */
        cp->Cx = 512 / 2;        /* [pix]        */
        cp->Cy = 480 / 2;        /* [pix]        */
        cp->sx = 1.0;        /* []           */
        break;

    case SONY_XC57: 
        cp->Ncx = 510;               /* [sel]        */
        cp->Nfx = 512;               /* [pix]        */
        cp->dx = 0.017;              /* [mm/sel]     */
        cp->dy = 0.013;              /* [mm/sel]     */
        cp->dpx = cp->dx * cp->Ncx / cp->Nfx;   /* [mm/pix]     */
        cp->dpy = cp->dy;             /* [mm/pix]     */
        cp->Cx = 512 / 2;            /* [pix]        */
        cp->Cy = 480 / 2;            /* [pix]        */
        cp->sx = 1.107914;           /* []           */
        break;
    // - webcams
    case LOGITECH_QUICKCAM_PRO_4000: 
        cp->Ncx = 659;               /* [sel]        */
        cp->Nfx = 640;               /* [pix]        */
        cp->dx = 0.0056;              /* [mm/sel]     */
        cp->dy = 0.0056;              /* [mm/sel]     */
        cp->dpx = cp->dx * cp->Ncx / cp->Nfx;   /* [mm/pix]     */
        cp->dpy = cp->dy;             /* [mm/pix]     */
        cp->Cx = 640 / 2;            /* [pix]        */
        cp->Cy = 480 / 2;            /* [pix]        */
        cp->sx = 1.0;           /* []           */
        break;
    
    case CREATIVE_WEBCAM_NX_PRO: 
        cp->Ncx = 642;               /* [sel]        */
        cp->Nfx = 640;               /* [pix]        */
        cp->dx = 0.008;              /* [mm/sel]     */
        cp->dy = 0.008;              /* [mm/sel]     */
        cp->dpx = cp->dx * cp->Ncx / cp->Nfx;   /* [mm/pix]     */
        cp->dpy = cp->dy;             /* [mm/pix]     */
        cp->Cx = 640 / 2;            /* [pix]        */
        cp->Cy = 480 / 2;            /* [pix]        */
        cp->sx = 1.0;           /* []           */
        break;
    // - Default camera 
    case PINHOLE_CAMERA: 
        cp->Ncx = Cx*2.0;               /* [sel]        */
        cp->Nfx = Cx*2.0;               /* [pix]        */
        cp->dx = 1.0;              /* [mm/sel]     */
        cp->dy = 1.0;              /* [mm/sel]     */
        cp->dpx = cp->dx * cp->Ncx / cp->Nfx;   /* [mm/pix]     */
        cp->dpy = cp->dy;             /* [mm/pix]     */
        cp->Cx = Cx;            /* [pix]        */
        cp->Cy = Cy;            /* [pix]        */
        cp->sx = 1.0;           /* []           */
        break;
    case NEW_CAMERA: 
        cp->Ncx = Ncx;               /* [sel]        */
        cp->Nfx = Nfx;               /* [pix]        */
        cp->dx = dx;              /* [mm/sel]     */
        cp->dy = dy;              /* [mm/sel]     */
        cp->dpx = cp->dx * cp->Ncx / cp->Nfx;   /* [mm/pix]     */
        cp->dpy = cp->dy;             /* [mm/pix]     */
        cp->Cx = Cx;            /* [pix]        */
        cp->Cy = Cy;            /* [pix]        */
        cp->sx = 1.0;           /* []           */
        break;

    default:
        printf("\nInvalid camera selection");
    }

}

/* Function to get calibration Tsai process */
int   tsai3D_calibration (    int numberPoints, double* modelPoints, double* imagePoints,
                                                        double *A, double *K, double *distortion, 
                                                        struct tsai_camera_parameters *cp,
                                                        struct tsai_calibration_constants *cc)
{
    int  i;
    double *U;
    // External variables
    U = 0; Xd = 0;     Yd = 0;    r_squared = 0;
    
    // Linked reference of tsai_object to internal pointer
    cp_temp = cp;
    cd_temp = (struct tsai_calibration_data*)malloc(sizeof(struct tsai_calibration_data)*1);
    tsai3D_init_cd(cd_temp);
    tsai3D_allocate_Memory_cd(cd_temp, numberPoints);

    cc_temp = cc;

    for(i=0;i<numberPoints;i++) {
        cd_temp->Xf[i] = imagePoints[2*i + 0];
        cd_temp->Yf[i] = imagePoints[2*i + 1];
        cd_temp->xw[i] = modelPoints[3*i + 0];
        cd_temp->yw[i] = modelPoints[3*i + 1];
        cd_temp->zw[i] = modelPoints[3*i + 2];
    };
    cd_temp->point_count = numberPoints;

    // Init process of Tsai Non-coplanar calibration if have 
    // pattern points captured for calibration camera.
    if(numberPoints > 0 )
    {
        Xd                = (double*)malloc(sizeof(double)*numberPoints); 
        Yd                = (double*)malloc(sizeof(double)*numberPoints);
        r_squared = (double*)malloc(sizeof(double)*numberPoints);
        U                    = (double*)malloc(sizeof(double)*7);
        tsai3D_compute_Xd_Yd_and_r_squared(cd_temp,cp_temp,Xd,Yd,r_squared);
        if(tsai3D_compute_U(cd_temp,Xd,Yd,U))
        {
            tsai3D_compute_Tx_and_Ty(cd_temp,cc_temp,Xd,Yd,r_squared,U);
            tsai3D_compute_sx(cp_temp,cc_temp,U);
            tsai3D_compute_Xd_Yd_and_r_squared(cd_temp,cp_temp,Xd,Yd,r_squared);
            tsai3D_compute_better_R(cp_temp, cc_temp, U);
            tsai3D_compute_approximate_f_and_Tz(cd_temp,cc_temp,Yd);
            if (cc_temp->f < 0.f) 
            {
                /* try the other solution for the orthonormal matrix */
                cc_temp->r3 = -cc_temp->r3;
                cc_temp->r6 = -cc_temp->r6;
                cc_temp->r7 = -cc_temp->r7;
                cc_temp->r8 = -cc_temp->r8;
                solve_RPY_transform(cc_temp);
                tsai3D_compute_approximate_f_and_Tz(cd_temp,cc_temp,Yd);
                if (cc_temp->f < 0.f){
                    printf ("\nError - possible handedness problem with data\n");
                    tsai3D_free_Memory_cd(cd_temp);
                    free(cd_temp);
                    free(U); free(Xd); free(Yd); free(r_squared);
                    return 0;
                };
            };        
            tsai3D_compute_exact_f_and_Tz_cminpack();
            printf ("\n-- Calibration Tsai Non-coplanar process successful --\n");
            tsai3D_free_Memory_cd(cd_temp);
            free(cd_temp);
            free(U); free(Xd); free(Yd); free(r_squared);
            A[0] = cc_temp->f;    A[1] = 0.0;                    A[2] = cp_temp->Cx;
            A[3] = 0.0;                    A[4] = cc_temp->f;    A[5] = cp_temp->Cy;
            A[6] = 0.0;                    A[7] = 0.0;                    A[8] = 1.0;

            K[0] = cc_temp->r1;    K[1] = cc_temp->r2;    K[2]  = cc_temp->r3; K[3]  = cc_temp->Tx; 
            K[4] = cc_temp->r4;    K[5] = cc_temp->r5;    K[6]  = cc_temp->r6; K[7]  = cc_temp->Ty; 
            K[8] = cc_temp->r7;    K[9] = cc_temp->r8;    K[10] = cc_temp->r9; K[11] = cc_temp->Tz; 
            
            *distortion = cc_temp->kappa1;    

            return 1;
        }
        else{
            tsai3D_free_Memory_cd(cd_temp);
            free(cd_temp);
            free(U); free(Xd); free(Yd); free(r_squared);
            return 0;
        };
    }
    else{
        tsai3D_free_Memory_cd(cd_temp);
        free(cd_temp);
        return 0;
    };
}
