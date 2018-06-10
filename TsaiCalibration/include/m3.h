#ifndef _M3_H
#define _M3_H

#ifdef __cplusplus
extern "C" {
#endif
/******************** 
 * TAD 3x3 Matrix        *
 * Marcelo Gattass    *
 * May 20,2006            *
 *******************/

double m3Det(double* A);                            /* computes the determinant of the matrix [A]     */
double m3Trace(double* A);                          /* computes the trace of the matrix [A]           */
double m3Inv( double* A, double* Ainv );            /* computes the inverse,[Ainv], of the matrix [A] */ 
void m3Cross(double *a,double *b,double *c);                /* computes the cross product {c} = {a}x{b}       */
void m3MultAB(double* A, double* B, double* AB);    /* computes the matrix product [AB]=[A][B]        */
void m3MultAb(double* A, double* b, double* x);     /* computes the transformation {x}=[A]{b}         */
double m3SolvAxb(double* A, double* b, double* x);  /* computes the solution of [A]{x}={b}            */
void m3CopyAB(double* A, double* B);                                /* copy [A] to [B]                                  */
void m3PrintMat(char* text,double* A);              /* print in the console the matrix [A]            */
void m3PrintVet(char* text,double* v);              /* print in the console the vector {v}            */

#ifdef __cplusplus
}
#endif

#endif 