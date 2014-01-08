#include "f2c.h"

/* Subroutine */ int clauum_(char *uplo, integer *n, complex *a, integer *lda,
	 integer *info)
{
/*  -- LAPACK auxiliary routine (version 2.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    CLAUUM computes the product U * U' or L' * L, where the triangular   
    factor U or L is stored in the upper or lower triangular part of   
    the array A.   

    If UPLO = 'U' or 'u' then the upper triangle of the result is stored, 
  
    overwriting the factor U in A.   
    If UPLO = 'L' or 'l' then the lower triangle of the result is stored, 
  
    overwriting the factor L in A.   

    This is the blocked form of the algorithm, calling Level 3 BLAS.   

    Arguments   
    =========   

    UPLO    (input) CHARACTER*1   
            Specifies whether the triangular factor stored in the array A 
  
            is upper or lower triangular:   
            = 'U':  Upper triangular   
            = 'L':  Lower triangular   

    N       (input) INTEGER   
            The order of the triangular factor U or L.  N >= 0.   

    A       (input/output) COMPLEX array, dimension (LDA,N)   
            On entry, the triangular factor U or L.   
            On exit, if UPLO = 'U', the upper triangle of A is   
            overwritten with the upper triangle of the product U * U';   
            if UPLO = 'L', the lower triangle of A is overwritten with   
            the lower triangle of the product L' * L.   

    LDA     (input) INTEGER   
            The leading dimension of the array A.  LDA >= max(1,N).   

    INFO    (output) INTEGER   
            = 0: successful exit   
            < 0: if INFO = -k, the k-th argument had an illegal value   

    ===================================================================== 
  


       Test the input parameters.   

    
   Parameter adjustments   
       Function Body */
    /* Table of constant values */
    static complex c_b1 = {1.f,0.f};
    static integer c__1 = 1;
    static integer c_n1 = -1;
    static real c_b21 = 1.f;
    
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3, i__4;
    /* Local variables */
    static integer i;
    extern /* Subroutine */ int cgemm_(char *, char *, integer *, integer *, 
	    integer *, complex *, complex *, integer *, complex *, integer *, 
	    complex *, complex *, integer *), cherk_(char *, 
	    char *, integer *, integer *, real *, complex *, integer *, real *
	    , complex *, integer *);
    extern logical lsame_(char *, char *);
    extern /* Subroutine */ int ctrmm_(char *, char *, char *, char *, 
	    integer *, integer *, complex *, complex *, integer *, complex *, 
	    integer *);
    static logical upper;
    extern /* Subroutine */ int clauu2_(char *, integer *, complex *, integer 
	    *, integer *);
    static integer ib, nb;
    extern /* Subroutine */ int xerbla_(char *, integer *);
    extern integer ilaenv_(integer *, char *, char *, integer *, integer *, 
	    integer *, integer *, ftnlen, ftnlen);




#define A(I,J) a[(I)-1 + ((J)-1)* ( *lda)]

    *info = 0;
    upper = lsame_(uplo, "U");
    if (! upper && ! lsame_(uplo, "L")) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*lda < max(1,*n)) {
	*info = -4;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("CLAUUM", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	return 0;
    }

/*     Determine the block size for this environment. */

    nb = ilaenv_(&c__1, "CLAUUM", uplo, n, &c_n1, &c_n1, &c_n1, 6L, 1L);

    if (nb <= 1 || nb >= *n) {

/*        Use unblocked code */

	clauu2_(uplo, n, &A(1,1), lda, info);
    } else {

/*        Use blocked code */

	if (upper) {

/*           Compute the product U * U'. */

	    i__1 = *n;
	    i__2 = nb;
	    for (i = 1; nb < 0 ? i >= *n : i <= *n; i += nb) {
/* Computing MIN */
		i__3 = nb, i__4 = *n - i + 1;
		ib = min(i__3,i__4);
		i__3 = i - 1;
		ctrmm_("Right", "Upper", "Conjugate transpose", "Non-unit", &
			i__3, &ib, &c_b1, &A(i,i), lda, &A(1,i), lda);
		clauu2_("Upper", &ib, &A(i,i), lda, info);
		if (i + ib <= *n) {
		    i__3 = i - 1;
		    i__4 = *n - i - ib + 1;
		    cgemm_("No transpose", "Conjugate transpose", &i__3, &ib, 
			    &i__4, &c_b1, &A(1,i+ib), lda, &A(i,i+ib), lda, &c_b1, &A(1,i), lda);
		    i__3 = *n - i - ib + 1;
		    cherk_("Upper", "No transpose", &ib, &i__3, &c_b21, &A(i,i+ib), lda, &c_b21, &A(i,i), lda);
		}
/* L10: */
	    }
	} else {

/*           Compute the product L' * L. */

	    i__2 = *n;
	    i__1 = nb;
	    for (i = 1; nb < 0 ? i >= *n : i <= *n; i += nb) {
/* Computing MIN */
		i__3 = nb, i__4 = *n - i + 1;
		ib = min(i__3,i__4);
		i__3 = i - 1;
		ctrmm_("Left", "Lower", "Conjugate transpose", "Non-unit", &
			ib, &i__3, &c_b1, &A(i,i), lda, &A(i,1), lda);
		clauu2_("Lower", &ib, &A(i,i), lda, info);
		if (i + ib <= *n) {
		    i__3 = i - 1;
		    i__4 = *n - i - ib + 1;
		    cgemm_("Conjugate transpose", "No transpose", &ib, &i__3, 
			    &i__4, &c_b1, &A(i+ib,i), lda, &A(i+ib,1), lda, &c_b1, &A(i,1), lda);
		    i__3 = *n - i - ib + 1;
		    cherk_("Lower", "Conjugate transpose", &ib, &i__3, &c_b21,
			     &A(i+ib,i), lda, &c_b21, &A(i,i), lda);
		}
/* L20: */
	    }
	}
    }

    return 0;

/*     End of CLAUUM */

} /* clauum_ */

