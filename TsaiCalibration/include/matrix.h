#ifndef _MATRIX_H_
#define _MATRIX_H_
/******************
 * Simple Math     *
 *                 *
 * Marcelo Gattass *
 * Dec04,2005      *
 ******************/

#if    defined(__cplusplus)
extern "C" {
#endif

/*
 * Vectors are represented in ANSI C as double*
 * Matrices are stored as vectors in a line by line fashion.
 * i.e., the ij element of a matrix Amxn (m rowns and n columns) is 
 * stored in the i*n+j  position of the vector that stores the 
 * matrix.
 *
 * The ideia is to enable simple matrix creations such as:
   double a[2*3] = {  1., 2., 7.,
                     -1., 5., 4.};
 * This ANSI C statment declares and stores the 2x3 matrix:
 *     |  1. 2. 7. |
 *     | -1  5. 4. |
 */
 
/*
 * Computes the solution for the linear equations [a]{x}={b}
 * using Gauss method.
 *
 * input: 
 * [a]  Coefficient matrix (stored as a vector in a line by line mode).
 *  n   number of equations.
 * {b}  Constant vector.
 * 
 * output:
 * {b}  Solution {x}.
 *
 * @return 0 => no solution or infinity; 1=> one solution.
 */
int mtxGaussAxb (double* a, int n, double* b);

/*
 * Computes the LU decomposition of a matrix [a]nxn
 * [A]=[L][U]
 *
 * input:
 *   [a] = a nxn square matrix stored as a vector in a line by line mode.
 *    n  =  dimension of [a].
 *
 * output:
 *   [a] = represented as [L] and [U} in the same matrix.
 *   {rp} = row permutation.
 *   *d = +1 ,number of row interchanges is even (-1 = odd).
 */
int mtxDecompLU ( double* a, int n, int *rp, double* d, double* scale );

/*
 *  Solves the set of n linear equations [A]{x} = {b}
 *  where the matrix for this function is de LU decompostion
 *  determined by the function mtxDecompLU.
 *
 *  input:
 *  [a] is not as the matrix A but rather as its LU decomposition.
 *   n  =  dimension of [a].
 *  {rp} = row permutation (determined by the function mtxDecompLU).
 *  {b} is the right hand side vector {b}.
 *
 *  output:
 *  {b} stores the solution {x}
 *
 *  Note:
 *  [a], n, and {rp} are not modified by this function and can be
 *  used for successive calls with different right-hand sides b. 
 *  This routine takes into account he possibility that {b} may begin 
 *  with many zero elements, so it it efficient for use in matrix
 *  inversion.
 */
void mtxBackSubLU( double* a, int n, int* rp, double* b );

/*
 *  Computes the determinant of a matrix [A]
 *  where the matrix for this function is the LU decomposition
 *  determined by the function mtxDecompLU.
 *
 *  input:
 *  [a] is not as the matrix A but rather as its LU decomposition.
 *   d = +1 ,number of row permutation in the decomposition was even (-1 = odd).
 *   n  =  dimension of [a].
 *
 *  @return:
 *  determinant of [a].
 */
double mtxDetLU( double* a, double d, int n);
 
/*
 * Computes the Singular Value Decomposition of a matrix [a]
 * i.e.: [a]=[u][d][v]T
 * input: 
 *   [a] = a mxn matrix (stored as a vector in a line by line mode).
 *    m  = number of rows of matrix [a].
 *    n  = number of columns of matrix [a].
 * 
 * output:
 *   [u] = a mxn matrix (columns are orthogonal vectors)(stored as a vector in a line by line mode).
 *   {d} = 1xn matrix that contains the elements of the diagonal matrix [d](nxn).
 *   [v] = a nxn orthogonal matrix(stored as a vector in a line by line mode).
 *
 * temporary space:
 *  {tmp}= a temporary 1xn vector (space used by the function).
 *
 */ 
int mtxSVD(double* a, int m, int n, double* u, double* d, double* v, double* tmp);

/*
 * Finds a non trivial solution for the system Ax=0
 * A mxn, m>n and rank(A)< n (better if = n-1).
 *
 * The return value indicates the error. He is the ratio from the smalest  
 * to the largest singular value of A. If rank(A) < n
 * this value should be zero (~=TOL for the implementation).
 * input: 
 *   [a] = a mxn matrix (stored as a vector in a line by line mode).
 *    m  = number of rows of matrix [a].
 *    n  = number of coluns of matrix [a].
 * temporary space:
 *  {tmp}= a temporary 1xm vector (space used by the function).
 *
 * output:
 *   {x} = 1xm solution vector.
 *   [u] = a mxn matrix (columns are orthogonal vectors)(stored as a vector in a line by line mode).
 *   {d} = 1xn matrix that contains the elements of the diagonal matrix [d](nxn).
 *   [v] = a nxn orthogonal matrix(stored as a vector in a line by line mode).
 * 
 */ 
double mtxSVDAx0(double* a, int m, int n, double* x, double* u, double* d, double* v, double* tmp);

/*
 * Finds a solution for the system Ax=b
 * A mxn, m>n.
 * input: 
 *   [a] = a mxn matrix (stored as a vector in a line by line mode).
 *    m  = number of rows of matrix [a].
 *    n  = number of coluns of matrix [a].
 *   {b} = 1xm right hand side vector.
 * temporary space:
 *  {tmp}= a temporary 1xm vector (space used by the function).
 *
 * output:
 *   {x} = 1xm solution vector.
 *   [u] = a mxn matrix (columns are orthogonal vectors)(stored as a vector in a line by line mode).
 *   {d} = 1xn matrix that contains the elements of the diagonal matrix [d](nxn).
 *   [v] = a nxn orthogonal matrix(stored as a vector in a line by line mode).
 * 
 */ 
void mtxSVDAxb(double* a, int m, int n, double* x, double* b, double* u, double* d, double* v, double* tmp);

/* 
 * Add the tensor product of the vector {v} (i.e., {v}{v}T) 
 * to the matrix [A].  
 * [A]+={v}{v}T
 *
 */
void mtxAddMatVecTensor(double* a, double* v, int n);

/* {x}=[A]{b}    Dimensions: [A]=mxn, {b}=m and {x}=n. */
void mtxAb(double* a, double* b, int m, int n, double* x);

/* {x}=[A]T{b}    Dimensions: [A]=mxn, {b}=m and {x}=n. */
void mtxAtb(double* a, double* b, int m, int n, double* x);

/* [X]=[A][B]    Dimensions: [A]=mxp, [B]=pxn and [X]=mxn. */
void mtxAB(double* a, double* b, int m, int p, int n, double* x);

/* [X]=[A][B]T    Dimensions: [A]=mxp, [B]=nxp and [X]=mxn. */
void mtxABt(double* a, double* b, int m, int p, int n, double* x);

/*  [X]=[A]T[B]    Dimensions: [A]=mxp, [B]=mxn and [X]=pxn */
void mtxAtB(double* a, double* b, int m, int p, int n, double* x);

/*  [X]=[A]+[B]    Dimensions: [A]=mxn, [B]=mxn and [X]=mxn */
void mtxAddMat(double* a, double* b, int m, int n, double* x);

/*  [X]=[A]-[B]    Dimensions: [A]=mxn, [B]=mxn and [X]=mxn */
void mtxSubMat(double* a, double* b, int m, int n, double* x);

/*  [X]=s[A]    Dimensions: [A]=mxn and [X]=mxn */
void mtxScaMat(double* a, double s,int m, int n, double* x);

/*  [X]=[A]T    Dimensions: [A]=mxn and [X]=nxm */
void mtxAt(double* a, int m, int n, double* x);

/*  [X]=[A]    Dimensions: [A]=mxn and [X]=mxn */
void mtxMatCopy(double* a, int m, int n, double* x);

/*  {x}={v}+{u}  Dimensions: {v}=m, {u}=m and {x}=m */
void mtxAddVec(double* u, double* v, int m, double* x);

/*  {x}={v}-{u}  Dimensions: {v}=m, {u}=m and {x}=m */
void mtxSubVec(double* u, double* v, int m, double* x);

/*  {x}=s{u}  Dimensions: {u}=m and {x}=m */
void mtxScaVec(double* u, int m,  double s, double* x);

/*  s={v}.{u}  Dimensions: {v}=m and  {u}=m */
double mtxDotProd(double* u, double* v, int m);

/* normalize {v} and returns the norm */
double mtxNormalizeVector(int n, double* v);

/* {x}={v} Dimensions: {v}={x}=m */
void mtxVecCopy(double* v, int m, double* x);

/* {x}=col(A) */
void mtxCol(double* A,int col,  int m, int n, double* x);

/* Print matrix */
void mtxShowMat(char*  title, double* a, int m, int n);

/* Print vector */
void mtxShowVec(char*  title, double* v, int n);

void mtxLUdcmp(double **a,int n,int *indx,double *d);
void mtxLUbksb(double **a,int n,int *indx,double *b);
/*
* Computes the nxn matrix [X] such that [A][X]=[I]
* , then [X] is the inverse of [A].  
*(uses LU decomposition).
*/ 
void mtxAXI_nxn(double *a, int n, double *x);
/*
* Computes the mxn matrix [X] such that [A][X]=[I]
* , then [X] is the inverse of [A].  
*/
void mtxAxI_mxn(double *A, int m, int n, double *X);

/*
* Acha uma solucao x para o Sistema Ax=b
* A nxn, via :
*                                        [A]x =            b
*                                [At][A]x =    [At]b
*     inv([At][A])    [At][A]x =    inv([At][A])[At]b    
*                                        [I]x =  inv([At][A])[At]b    
*/ 
void mtxAx_b(double *A, int m, int n, double *b, double *x);

#if defined(__cplusplus)
}
#endif

#endif

