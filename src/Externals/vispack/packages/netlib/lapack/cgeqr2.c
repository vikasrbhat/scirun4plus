#include "f2c.h"

/* Subroutine */ int cgeqr2_(integer *m, integer *n, complex *a, integer *lda,
	 complex *tau, complex *work, integer *info)
{
/*  -- LAPACK routine (version 2.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    CGEQR2 computes a QR factorization of a complex m by n matrix A:   
    A = Q * R.   

    Arguments   
    =========   

    M       (input) INTEGER   
            The number of rows of the matrix A.  M >= 0.   

    N       (input) INTEGER   
            The number of columns of the matrix A.  N >= 0.   

    A       (input/output) COMPLEX array, dimension (LDA,N)   
            On entry, the m by n matrix A.   
            On exit, the elements on and above the diagonal of the array 
  
            contain the min(m,n) by n upper trapezoidal matrix R (R is   
            upper triangular if m >= n); the elements below the diagonal, 
  
            with the array TAU, represent the unitary matrix Q as a   
            product of elementary reflectors (see Further Details).   

    LDA     (input) INTEGER   
            The leading dimension of the array A.  LDA >= max(1,M).   

    TAU     (output) COMPLEX array, dimension (min(M,N))   
            The scalar factors of the elementary reflectors (see Further 
  
            Details).   

    WORK    (workspace) COMPLEX array, dimension (N)   

    INFO    (output) INTEGER   
            = 0: successful exit   
            < 0: if INFO = -i, the i-th argument had an illegal value   

    Further Details   
    ===============   

    The matrix Q is represented as a product of elementary reflectors   

       Q = H(1) H(2) . . . H(k), where k = min(m,n).   

    Each H(i) has the form   

       H(i) = I - tau * v * v'   

    where tau is a complex scalar, and v is a complex vector with   
    v(1:i-1) = 0 and v(i) = 1; v(i+1:m) is stored on exit in A(i+1:m,i), 
  
    and tau in TAU(i).   

    ===================================================================== 
  


       Test the input arguments   

    
   Parameter adjustments   
       Function Body */
    /* Table of constant values */
    static integer c__1 = 1;
    
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3;
    complex q__1;
    /* Builtin functions */
    void r_cnjg(complex *, complex *);
    /* Local variables */
    static integer i, k;
    static complex alpha;
    extern /* Subroutine */ int clarf_(char *, integer *, integer *, complex *
	    , integer *, complex *, complex *, integer *, complex *), 
	    clarfg_(integer *, complex *, complex *, integer *, complex *), 
	    xerbla_(char *, integer *);



#define TAU(I) tau[(I)-1]
#define WORK(I) work[(I)-1]

#define A(I,J) a[(I)-1 + ((J)-1)* ( *lda)]

    *info = 0;
    if (*m < 0) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*lda < max(1,*m)) {
	*info = -4;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("CGEQR2", &i__1);
	return 0;
    }

    k = min(*m,*n);

    i__1 = k;
    for (i = 1; i <= k; ++i) {

/*        Generate elementary reflector H(i) to annihilate A(i+1:m,i) 
*/

	i__2 = *m - i + 1;
/* Computing MIN */
	i__3 = i + 1;
	clarfg_(&i__2, &A(i,i), &A(min(i+1,*m),i), &
		c__1, &TAU(i));
	if (i < *n) {

/*           Apply H(i)' to A(i:m,i+1:n) from the left */

	    i__2 = i + i * a_dim1;
	    alpha.r = A(i,i).r, alpha.i = A(i,i).i;
	    i__2 = i + i * a_dim1;
	    A(i,i).r = 1.f, A(i,i).i = 0.f;
	    i__2 = *m - i + 1;
	    i__3 = *n - i;
	    r_cnjg(&q__1, &TAU(i));
	    clarf_("Left", &i__2, &i__3, &A(i,i), &c__1, &q__1, &A(i,i+1), lda, &WORK(1));
	    i__2 = i + i * a_dim1;
	    A(i,i).r = alpha.r, A(i,i).i = alpha.i;
	}
/* L10: */
    }
    return 0;

/*     End of CGEQR2 */

} /* cgeqr2_ */

