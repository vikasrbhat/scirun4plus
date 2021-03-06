#include "f2c.h"

/* Subroutine */ int cungql_(integer *m, integer *n, integer *k, complex *a, 
	integer *lda, complex *tau, complex *work, integer *lwork, integer *
	info)
{
/*  -- LAPACK routine (version 2.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    CUNGQL generates an M-by-N complex matrix Q with orthonormal columns, 
  
    which is defined as the last N columns of a product of K elementary   
    reflectors of order M   

          Q  =  H(k) . . . H(2) H(1)   

    as returned by CGEQLF.   

    Arguments   
    =========   

    M       (input) INTEGER   
            The number of rows of the matrix Q. M >= 0.   

    N       (input) INTEGER   
            The number of columns of the matrix Q. M >= N >= 0.   

    K       (input) INTEGER   
            The number of elementary reflectors whose product defines the 
  
            matrix Q. N >= K >= 0.   

    A       (input/output) COMPLEX array, dimension (LDA,N)   
            On entry, the (n-k+i)-th column must contain the vector which 
  
            defines the elementary reflector H(i), for i = 1,2,...,k, as 
  
            returned by CGEQLF in the last k columns of its array   
            argument A.   
            On exit, the M-by-N matrix Q.   

    LDA     (input) INTEGER   
            The first dimension of the array A. LDA >= max(1,M).   

    TAU     (input) COMPLEX array, dimension (K)   
            TAU(i) must contain the scalar factor of the elementary   
            reflector H(i), as returned by CGEQLF.   

    WORK    (workspace/output) COMPLEX array, dimension (LWORK)   
            On exit, if INFO = 0, WORK(1) returns the optimal LWORK.   

    LWORK   (input) INTEGER   
            The dimension of the array WORK. LWORK >= max(1,N).   
            For optimum performance LWORK >= N*NB, where NB is the   
            optimal blocksize.   

    INFO    (output) INTEGER   
            = 0:  successful exit   
            < 0:  if INFO = -i, the i-th argument has an illegal value   

    ===================================================================== 
  


       Test the input arguments   

    
   Parameter adjustments   
       Function Body */
    /* Table of constant values */
    static integer c__1 = 1;
    static integer c_n1 = -1;
    static integer c__3 = 3;
    static integer c__2 = 2;
    
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3, i__4, i__5;
    /* Local variables */
    static integer i, j, l, nbmin, iinfo;
    extern /* Subroutine */ int cung2l_(integer *, integer *, integer *, 
	    complex *, integer *, complex *, complex *, integer *);
    static integer ib, nb, kk;
    extern /* Subroutine */ int clarfb_(char *, char *, char *, char *, 
	    integer *, integer *, integer *, complex *, integer *, complex *, 
	    integer *, complex *, integer *, complex *, integer *);
    static integer nx;
    extern /* Subroutine */ int clarft_(char *, char *, integer *, integer *, 
	    complex *, integer *, complex *, complex *, integer *), xerbla_(char *, integer *);
    extern integer ilaenv_(integer *, char *, char *, integer *, integer *, 
	    integer *, integer *, ftnlen, ftnlen);
    static integer ldwork, iws;



#define TAU(I) tau[(I)-1]
#define WORK(I) work[(I)-1]

#define A(I,J) a[(I)-1 + ((J)-1)* ( *lda)]

    *info = 0;
    if (*m < 0) {
	*info = -1;
    } else if (*n < 0 || *n > *m) {
	*info = -2;
    } else if (*k < 0 || *k > *n) {
	*info = -3;
    } else if (*lda < max(1,*m)) {
	*info = -5;
    } else if (*lwork < max(1,*n)) {
	*info = -8;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("CUNGQL", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*n <= 0) {
	WORK(1).r = 1.f, WORK(1).i = 0.f;
	return 0;
    }

/*     Determine the block size. */

    nb = ilaenv_(&c__1, "CUNGQL", " ", m, n, k, &c_n1, 6L, 1L);
    nbmin = 2;
    nx = 0;
    iws = *n;
    if (nb > 1 && nb < *k) {

/*        Determine when to cross over from blocked to unblocked code.
   

   Computing MAX */
	i__1 = 0, i__2 = ilaenv_(&c__3, "CUNGQL", " ", m, n, k, &c_n1, 6L, 1L)
		;
	nx = max(i__1,i__2);
	if (nx < *k) {

/*           Determine if workspace is large enough for blocked co
de. */

	    ldwork = *n;
	    iws = ldwork * nb;
	    if (*lwork < iws) {

/*              Not enough workspace to use optimal NB:  reduc
e NB and   
                determine the minimum value of NB. */

		nb = *lwork / ldwork;
/* Computing MAX */
		i__1 = 2, i__2 = ilaenv_(&c__2, "CUNGQL", " ", m, n, k, &c_n1,
			 6L, 1L);
		nbmin = max(i__1,i__2);
	    }
	}
    }

    if (nb >= nbmin && nb < *k && nx < *k) {

/*        Use blocked code after the first block.   
          The last kk columns are handled by the block method.   

   Computing MIN */
	i__1 = *k, i__2 = (*k - nx + nb - 1) / nb * nb;
	kk = min(i__1,i__2);

/*        Set A(m-kk+1:m,1:n-kk) to zero. */

	i__1 = *n - kk;
	for (j = 1; j <= *n-kk; ++j) {
	    i__2 = *m;
	    for (i = *m - kk + 1; i <= *m; ++i) {
		i__3 = i + j * a_dim1;
		A(i,j).r = 0.f, A(i,j).i = 0.f;
/* L10: */
	    }
/* L20: */
	}
    } else {
	kk = 0;
    }

/*     Use unblocked code for the first or only block. */

    i__1 = *m - kk;
    i__2 = *n - kk;
    i__3 = *k - kk;
    cung2l_(&i__1, &i__2, &i__3, &A(1,1), lda, &TAU(1), &WORK(1), &iinfo)
	    ;

    if (kk > 0) {

/*        Use blocked code */

	i__1 = *k;
	i__2 = nb;
	for (i = *k - kk + 1; nb < 0 ? i >= *k : i <= *k; i += nb) {
/* Computing MIN */
	    i__3 = nb, i__4 = *k - i + 1;
	    ib = min(i__3,i__4);
	    if (*n - *k + i > 1) {

/*              Form the triangular factor of the block reflec
tor   
                H = H(i+ib-1) . . . H(i+1) H(i) */

		i__3 = *m - *k + i + ib - 1;
		clarft_("Backward", "Columnwise", &i__3, &ib, &A(1,*n-*k+i), lda, &TAU(i), &WORK(1), &ldwork);

/*              Apply H to A(1:m-k+i+ib-1,1:n-k+i-1) from the 
left */

		i__3 = *m - *k + i + ib - 1;
		i__4 = *n - *k + i - 1;
		clarfb_("Left", "No transpose", "Backward", "Columnwise", &
			i__3, &i__4, &ib, &A(1,*n-*k+i), lda,
			 &WORK(1), &ldwork, &A(1,1), lda, &WORK(ib + 1), 
			&ldwork);
	    }

/*           Apply H to rows 1:m-k+i+ib-1 of current block */

	    i__3 = *m - *k + i + ib - 1;
	    cung2l_(&i__3, &ib, &ib, &A(1,*n-*k+i), lda, &
		    TAU(i), &WORK(1), &iinfo);

/*           Set rows m-k+i+ib:m of current block to zero */

	    i__3 = *n - *k + i + ib - 1;
	    for (j = *n - *k + i; j <= *n-*k+i+ib-1; ++j) {
		i__4 = *m;
		for (l = *m - *k + i + ib; l <= *m; ++l) {
		    i__5 = l + j * a_dim1;
		    A(l,j).r = 0.f, A(l,j).i = 0.f;
/* L30: */
		}
/* L40: */
	    }
/* L50: */
	}
    }

    WORK(1).r = (real) iws, WORK(1).i = 0.f;
    return 0;

/*     End of CUNGQL */

} /* cungql_ */

