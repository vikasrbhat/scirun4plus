#include "f2c.h"

/* Subroutine */ int cungr2_(integer *m, integer *n, integer *k, complex *a, 
	integer *lda, complex *tau, complex *work, integer *info)
{
/*  -- LAPACK routine (version 2.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    CUNGR2 generates an m by n complex matrix Q with orthonormal rows,   
    which is defined as the last m rows of a product of k elementary   
    reflectors of order n   

          Q  =  H(1)' H(2)' . . . H(k)'   

    as returned by CGERQF.   

    Arguments   
    =========   

    M       (input) INTEGER   
            The number of rows of the matrix Q. M >= 0.   

    N       (input) INTEGER   
            The number of columns of the matrix Q. N >= M.   

    K       (input) INTEGER   
            The number of elementary reflectors whose product defines the 
  
            matrix Q. M >= K >= 0.   

    A       (input/output) COMPLEX array, dimension (LDA,N)   
            On entry, the (m-k+i)-th row must contain the vector which   
            defines the elementary reflector H(i), for i = 1,2,...,k, as 
  
            returned by CGERQF in the last k rows of its array argument   
            A.   
            On exit, the m-by-n matrix Q.   

    LDA     (input) INTEGER   
            The first dimension of the array A. LDA >= max(1,M).   

    TAU     (input) COMPLEX array, dimension (K)   
            TAU(i) must contain the scalar factor of the elementary   
            reflector H(i), as returned by CGERQF.   

    WORK    (workspace) COMPLEX array, dimension (M)   

    INFO    (output) INTEGER   
            = 0: successful exit   
            < 0: if INFO = -i, the i-th argument has an illegal value   

    ===================================================================== 
  


       Test the input arguments   

    
   Parameter adjustments   
       Function Body */
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3;
    complex q__1, q__2;
    /* Builtin functions */
    void r_cnjg(complex *, complex *);
    /* Local variables */
    static integer i, j, l;
    extern /* Subroutine */ int cscal_(integer *, complex *, complex *, 
	    integer *), clarf_(char *, integer *, integer *, complex *, 
	    integer *, complex *, complex *, integer *, complex *);
    static integer ii;
    extern /* Subroutine */ int clacgv_(integer *, complex *, integer *), 
	    xerbla_(char *, integer *);


#define TAU(I) tau[(I)-1]
#define WORK(I) work[(I)-1]

#define A(I,J) a[(I)-1 + ((J)-1)* ( *lda)]

    *info = 0;
    if (*m < 0) {
	*info = -1;
    } else if (*n < *m) {
	*info = -2;
    } else if (*k < 0 || *k > *m) {
	*info = -3;
    } else if (*lda < max(1,*m)) {
	*info = -5;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("CUNGR2", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*m <= 0) {
	return 0;
    }

    if (*k < *m) {

/*        Initialise rows 1:m-k to rows of the unit matrix */

	i__1 = *n;
	for (j = 1; j <= *n; ++j) {
	    i__2 = *m - *k;
	    for (l = 1; l <= *m-*k; ++l) {
		i__3 = l + j * a_dim1;
		A(l,j).r = 0.f, A(l,j).i = 0.f;
/* L10: */
	    }
	    if (j > *n - *m && j <= *n - *k) {
		i__2 = *m - *n + j + j * a_dim1;
		A(*m-*n+j,j).r = 1.f, A(*m-*n+j,j).i = 0.f;
	    }
/* L20: */
	}
    }

    i__1 = *k;
    for (i = 1; i <= *k; ++i) {
	ii = *m - *k + i;

/*        Apply H(i)' to A(1:m-k+i,1:n-k+i) from the right */

	i__2 = *n - *m + ii - 1;
	clacgv_(&i__2, &A(ii,1), lda);
	i__2 = ii + (*n - *m + ii) * a_dim1;
	A(ii,*n-*m+ii).r = 1.f, A(ii,*n-*m+ii).i = 0.f;
	i__2 = ii - 1;
	i__3 = *n - *m + ii;
	r_cnjg(&q__1, &TAU(i));
	clarf_("Right", &i__2, &i__3, &A(ii,1), lda, &q__1, &A(1,1), lda, &WORK(1));
	i__2 = *n - *m + ii - 1;
	i__3 = i;
	q__1.r = -(doublereal)TAU(i).r, q__1.i = -(doublereal)TAU(i).i;
	cscal_(&i__2, &q__1, &A(ii,1), lda);
	i__2 = *n - *m + ii - 1;
	clacgv_(&i__2, &A(ii,1), lda);
	i__2 = ii + (*n - *m + ii) * a_dim1;
	r_cnjg(&q__2, &TAU(i));
	q__1.r = 1.f - q__2.r, q__1.i = 0.f - q__2.i;
	A(ii,*n-*m+ii).r = q__1.r, A(ii,*n-*m+ii).i = q__1.i;

/*        Set A(m-k+i,n-k+i+1:n) to zero */

	i__2 = *n;
	for (l = *n - *m + ii + 1; l <= *n; ++l) {
	    i__3 = ii + l * a_dim1;
	    A(ii,l).r = 0.f, A(ii,l).i = 0.f;
/* L30: */
	}
/* L40: */
    }
    return 0;

/*     End of CUNGR2 */

} /* cungr2_ */

