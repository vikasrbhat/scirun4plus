#include "f2c.h"

/* Subroutine */ int clar2v_(integer *n, complex *x, complex *y, complex *z, 
	integer *incx, real *c, complex *s, integer *incc)
{
/*  -- LAPACK auxiliary routine (version 2.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       February 29, 1992   


    Purpose   
    =======   

    CLAR2V applies a vector of complex plane rotations with real cosines 
  
    from both sides to a sequence of 2-by-2 complex Hermitian matrices,   
    defined by the elements of the vectors x, y and z. For i = 1,2,...,n 
  

       (       x(i)  z(i) ) :=   
       ( conjg(z(i)) y(i) )   

         (  c(i) conjg(s(i)) ) (       x(i)  z(i) ) ( c(i) -conjg(s(i)) ) 
  
         ( -s(i)       c(i)  ) ( conjg(z(i)) y(i) ) ( s(i)        c(i)  ) 
  

    Arguments   
    =========   

    N       (input) INTEGER   
            The number of plane rotations to be applied.   

    X       (input/output) COMPLEX array, dimension (1+(N-1)*INCX)   
            The vector x; the elements of x are assumed to be real.   

    Y       (input/output) COMPLEX array, dimension (1+(N-1)*INCX)   
            The vector y; the elements of y are assumed to be real.   

    Z       (input/output) COMPLEX array, dimension (1+(N-1)*INCX)   
            The vector z.   

    INCX    (input) INTEGER   
            The increment between elements of X, Y and Z. INCX > 0.   

    C       (input) REAL array, dimension (1+(N-1)*INCC)   
            The cosines of the plane rotations.   

    S       (input) COMPLEX array, dimension (1+(N-1)*INCC)   
            The sines of the plane rotations.   

    INCC    (input) INTEGER   
            The increment between elements of C and S. INCC > 0.   

    ===================================================================== 
  


    
   Parameter adjustments   
       Function Body */
    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1;
    complex q__1, q__2, q__3, q__4, q__5;
    /* Builtin functions */
    double r_imag(complex *);
    void r_cnjg(complex *, complex *);
    /* Local variables */
    static integer i;
    static complex t2, t3, t4;
    static real t5, t6;
    static integer ic;
    static real ci;
    static complex si;
    static integer ix;
    static real xi, yi;
    static complex zi;
    static real t1i, t1r, sii, zii, sir, zir;


#define S(I) s[(I)-1]
#define C(I) c[(I)-1]
#define Z(I) z[(I)-1]
#define Y(I) y[(I)-1]
#define X(I) x[(I)-1]


    ix = 1;
    ic = 1;
    i__1 = *n;
    for (i = 1; i <= *n; ++i) {
	i__2 = ix;
	xi = X(ix).r;
	i__2 = ix;
	yi = Y(ix).r;
	i__2 = ix;
	zi.r = Z(ix).r, zi.i = Z(ix).i;
	zir = zi.r;
	zii = r_imag(&zi);
	ci = C(ic);
	i__2 = ic;
	si.r = S(ic).r, si.i = S(ic).i;
	sir = si.r;
	sii = r_imag(&si);
	t1r = sir * zir - sii * zii;
	t1i = sir * zii + sii * zir;
	q__1.r = ci * zi.r, q__1.i = ci * zi.i;
	t2.r = q__1.r, t2.i = q__1.i;
	r_cnjg(&q__3, &si);
	q__2.r = xi * q__3.r, q__2.i = xi * q__3.i;
	q__1.r = t2.r - q__2.r, q__1.i = t2.i - q__2.i;
	t3.r = q__1.r, t3.i = q__1.i;
	r_cnjg(&q__2, &t2);
	q__3.r = yi * si.r, q__3.i = yi * si.i;
	q__1.r = q__2.r + q__3.r, q__1.i = q__2.i + q__3.i;
	t4.r = q__1.r, t4.i = q__1.i;
	t5 = ci * xi + t1r;
	t6 = ci * yi - t1r;
	i__2 = ix;
	d__1 = ci * t5 + (sir * t4.r + sii * r_imag(&t4));
	X(ix).r = d__1, X(ix).i = 0.f;
	i__2 = ix;
	d__1 = ci * t6 - (sir * t3.r - sii * r_imag(&t3));
	Y(ix).r = d__1, Y(ix).i = 0.f;
	i__2 = ix;
	q__2.r = ci * t3.r, q__2.i = ci * t3.i;
	r_cnjg(&q__4, &si);
	q__5.r = t6, q__5.i = t1i;
	q__3.r = q__4.r * q__5.r - q__4.i * q__5.i, q__3.i = q__4.r * q__5.i 
		+ q__4.i * q__5.r;
	q__1.r = q__2.r + q__3.r, q__1.i = q__2.i + q__3.i;
	Z(ix).r = q__1.r, Z(ix).i = q__1.i;
	ix += *incx;
	ic += *incc;
/* L10: */
    }
    return 0;

/*     End of CLAR2V */

} /* clar2v_ */

