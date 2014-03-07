#include "f2c.h"

/* Subroutine */ int checon_(char *uplo, integer *n, complex *a, integer *lda,
	 integer *ipiv, real *anorm, real *rcond, complex *work, integer *
	info)
{
/*  -- LAPACK routine (version 2.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       March 31, 1993   


    Purpose   
    =======   

    CHECON estimates the reciprocal of the condition number of a complex 
  
    Hermitian matrix A using the factorization A = U*D*U**H or   
    A = L*D*L**H computed by CHETRF.   

    An estimate is obtained for norm(inv(A)), and the reciprocal of the   
    condition number is computed as RCOND = 1 / (ANORM * norm(inv(A))).   

    Arguments   
    =========   

    UPLO    (input) CHARACTER*1   
            Specifies whether the details of the factorization are stored 
  
            as an upper or lower triangular matrix.   
            = 'U':  Upper triangular, form is A = U*D*U**H;   
            = 'L':  Lower triangular, form is A = L*D*L**H.   

    N       (input) INTEGER   
            The order of the matrix A.  N >= 0.   

    A       (input) COMPLEX array, dimension (LDA,N)   
            The block diagonal matrix D and the multipliers used to   
            obtain the factor U or L as computed by CHETRF.   

    LDA     (input) INTEGER   
            The leading dimension of the array A.  LDA >= max(1,N).   

    IPIV    (input) INTEGER array, dimension (N)   
            Details of the interchanges and the block structure of D   
            as determined by CHETRF.   

    ANORM   (input) REAL   
            The 1-norm of the original matrix A.   

    RCOND   (output) REAL   
            The reciprocal of the condition number of the matrix A,   
            computed as RCOND = 1/(ANORM * AINVNM), where AINVNM is an   
            estimate of the 1-norm of inv(A) computed in this routine.   

    WORK    (workspace) COMPLEX array, dimension (2*N)   

    INFO    (output) INTEGER   
            = 0:  successful exit   
            < 0:  if INFO = -i, the i-th argument had an illegal value   

    ===================================================================== 
  


       Test the input parameters.   

    
   Parameter adjustments   
       Function Body */
    /* Table of constant values */
    static integer c__1 = 1;
    
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    /* Local variables */
    static integer kase, i;
    extern logical lsame_(char *, char *);
    static logical upper;
    extern /* Subroutine */ int clacon_(integer *, complex *, complex *, real 
	    *, integer *), xerbla_(char *, integer *);
    static real ainvnm;
    extern /* Subroutine */ int chetrs_(char *, integer *, integer *, complex 
	    *, integer *, integer *, complex *, integer *, integer *);



#define IPIV(I) ipiv[(I)-1]
#define WORK(I) work[(I)-1]

#define A(I,J) a[(I)-1 + ((J)-1)* ( *lda)]

    *info = 0;
    upper = lsame_(uplo, "U");
    if (! upper && ! lsame_(uplo, "L")) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*lda < max(1,*n)) {
	*info = -4;
    } else if (*anorm < 0.f) {
	*info = -6;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("CHECON", &i__1);
	return 0;
    }

/*     Quick return if possible */

    *rcond = 0.f;
    if (*n == 0) {
	*rcond = 1.f;
	return 0;
    } else if (*anorm <= 0.f) {
	return 0;
    }

/*     Check that the diagonal matrix D is nonsingular. */

    if (upper) {

/*        Upper triangular storage: examine D from bottom to top */

	for (i = *n; i >= 1; --i) {
	    i__1 = i + i * a_dim1;
	    if (IPIV(i) > 0 && (A(i,i).r == 0.f && A(i,i).i == 0.f)) {
		return 0;
	    }
/* L10: */
	}
    } else {

/*        Lower triangular storage: examine D from top to bottom. */

	i__1 = *n;
	for (i = 1; i <= *n; ++i) {
	    i__2 = i + i * a_dim1;
	    if (IPIV(i) > 0 && (A(i,i).r == 0.f && A(i,i).i == 0.f)) {
		return 0;
	    }
/* L20: */
	}
    }

/*     Estimate the 1-norm of the inverse. */

    kase = 0;
L30:
    clacon_(n, &WORK(*n + 1), &WORK(1), &ainvnm, &kase);
    if (kase != 0) {

/*        Multiply by inv(L*D*L') or inv(U*D*U'). */

	chetrs_(uplo, n, &c__1, &A(1,1), lda, &IPIV(1), &WORK(1), n, 
		info);
	goto L30;
    }

/*     Compute the estimate of the reciprocal condition number. */

    if (ainvnm != 0.f) {
	*rcond = 1.f / ainvnm / *anorm;
    }

    return 0;

/*     End of CHECON */

} /* checon_ */

