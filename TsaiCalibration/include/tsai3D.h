/**************************
* Calibration    TSAI3D          *
*                                                    *
* Marcelo Gattass                    *
* Manuel E. L. Fernandez    *
* Jul06,2006                            *
**************************/
#ifndef __TSAI3D_H 
#define __TSAI3D_H

#if    defined(__cplusplus)
extern "C" {
#endif

#include <stdio.h>
#include <math.h>
#include <malloc.h>

// This library implemented a simplified "C" version of 
// Levenberg-Marquardt method for Nonlinear optimization 
#include "cminpack.h"
#include "matrix.h"

// Define names of camera parameters knew to different camera models
// - cameras
#define PHOTOMETRICS_STAR_I            0
#define GENERAL_IMAGING_MOS5300 1
#define PANASONIC_GP_MF702            2
#define SONY_XC75                                3
#define SONY_XC77                                4
#define SONY_XC57                                5
// - webcams
#define CREATIVE_WEBCAM_NX_PRO            6
#define LOGITECH_QUICKCAM_PRO_4000    7
// - standard and own cameras
#define PINHOLE_CAMERA                    8
#define NEW_CAMERA                            9

/**************************************************************************\
* Camera parameters are usually the fixed parameters of the given camera     *
* system, typically obtained from manufacturers specifications.                        *
*                                                                                                                                                    *
* Cy and Cy (the center of radial lens distortion), may be calibrated            *
* separately or as part of the coplanar/non-coplanar calibration.                    *
* The same with sx (x scale uncertainty factor).                                                    *
\*************************************************************************/
struct tsai_camera_parameters {
    double    Ncx;    /* [sel]     Number of sensor elements in camera's x direction */
    double    Nfx;    /* [pix]     Number of pixels in frame grabber's x direction   */
    double    dx;        /* [mm/sel]  X dimension of camera's sensor element (in mm)    */
    double    dy;        /* [mm/sel]  Y dimension of camera's sensor element (in mm)    */
    double    dpx;    /* [mm/pix]  Effective X dimension of pixel in frame grabber   */
    double    dpy;    /* [mm/pix]  Effective Y dimension of pixel in frame grabber   */
    double    Cx;        /* [pix]     Z axis intercept of camera coordinate system      */
    double    Cy;        /* [pix]     Z axis intercept of camera coordinate system      */
    double    sx;        /* []        Scale factor to compensate for any error in dpx   */
    
    int is_load_paremeters; /*      flag to indicate that all parameters have been loaded  */
};

/*******************************************************************************\
* Calibration data consists of the (x,y,z) world coordinates of a feature                *
* point    (in mm) and the corresponding coordinates (Xf,Yf) (in pixels) of the        *
* feature point in the image.  Point count is the number of points in the data    *
* set.                                                                                                                                                    *
*                                                                                                                                                                *
* Coplanar calibration:                                                                                                                    *
*                                                                                                                                                                *
* For coplanar calibration the z world coordinate of the data must be zero.            *
* In addition, for numerical stability the coordinates of the feature points        *
* should be placed at some arbitrary distance from the origin (0,0,0) of the        *
* world coordinate system.  Finally, the plane containing the feature points        *
* should not be parallel to the imaging plane.  A relative angle of 30 degrees    *
* is recommended.                                                                                                                                *
*                                                                                                                                                                *
* Non-coplanar calibration:                                                                                                            *
*                                                                                                                                                                *
* For non - coplanar calibration the data must not lie in a single plane.                    *
*                                                                                                                                                                *
\*******************************************************************************/
struct tsai_calibration_data {
    int      point_count;    /* [points]      */
    double    *xw;                /* [mm]        */
    double    *yw;                /* [mm]        */
    double    *zw;                /* [mm]        */
    double    *Xf;                /* [pix]       */
    double    *Yf;                /* [pix]       */
};

/*********************************************************\
* Calibration constants are the model constants that are    * 
* determined from the calibration data.                                        *
\*********************************************************/
struct tsai_calibration_constants 
{
    double    f;        /* [mm]          */
    double    kappa1;        /* [1/mm^2]      */
    double    p1;        /* [1/mm]        */
    double    p2;        /* [1/mm]        */
    double    Tx;        /* [mm]          */
    double    Ty;        /* [mm]          */
    double    Tz;        /* [mm]          */
    double    Rx;        /* [rad]     */
    double    Ry;        /* [rad]     */
    double    Rz;        /* [rad]     */
    double    r1;        /* []            */
    double    r2;        /* []            */
    double    r3;        /* []            */
    double    r4;        /* []            */
    double    r5;        /* []            */
    double    r6;        /* []            */
    double    r7;        /* []            */
    double    r8;        /* []            */
    double    r9;        /* []            */
    
    int is_load_constants; /*      flag to indicate that all parameters have been loaded  */
};

/*******************************************\
* Calibration funtions                                            *
\*******************************************/
/************************************************************************/
/**
* Initialization of camera parameters.
* @param[in] CameraModel     ID to some camera models of which we know your camera parameters.
* @param[in] Ncx - [sel]     Number of sensor elements in camera's x direction.
* @param[in] Nfx - [pix]     Number of pixels in frame grabber's x direction.
* @param[in] dx - [mm/sel]  X dimension of camera's sensor element (in mm).
* @param[in] dy - [mm/sel]  Y dimension of camera's sensor element (in mm).
* @param[in] Cx - [pix]     Z axis intercept of camera coordinate system.
* @param[in] Cy - [pix]     Z axis intercept of camera coordinate system.
* @param[in] sx - []        Scale factor to compensate for any error in dpx.
* @param[out] cp - struct to store camera parameters and depend on model of camera 
*                                     defined, if you needed can define your own camera parameters.
*                                     .
*/
void   tsai3D_define_camera_parameters(int CameraModel, 
                                                                            double Ncx,    double Nfx,
                                                                            double dx,   double dy,
                                                                            double Cx,    double Cy, double sx,
                                                                            struct tsai_camera_parameters *cp);

/**
* Get calibration camera using Tsai Non-coplanar method.
* @param[in] numberPoints - Number of points used to get camera calibration.
* @param[in] modelPoints  - World coordinates of pattern markers used to calibrate the camera.
* @param[in] imagePoints  - Image coordinates of pattern markers used to calibrate the camera.
* @param[in] cp - struct to store camera parameters and depend on model of camera 
*                                     defined, if you needed can define your own camera parameters.
* @param[out] A - Matrix of Intrinsic parameters. 
* @param[out] K - Matrix of Extrinsic parameters. 
* @param[out] distortion - value of first component of radial distortion k1. 
*/
int  tsai3D_calibration(  int numberPoints, double* modelPoints, double* imagePoints,
                                                    double *A, double *K, double *distortion, 
                                                    struct tsai_camera_parameters *cp,
                                                    struct tsai_calibration_constants *cc);

#if defined(__cplusplus)
}
#endif

#endif  /* __tsai3D_H */

/* tsai3D.H */
