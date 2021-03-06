#include "f2c.h"

/* Subroutine */ int cpocon_(char *uplo, integer *n, complex *a, integer *lda,
	 real *anorm, real *rcond, complex *work, real *rwork, integer *info)
{
/*  -- LAPACK routine (version 2.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       March 31, 1993   


    Purpose   
    =======   

    CPOCON estimates the reciprocal of the condition number (in the   
    1-norm) of a complex Hermitian positive definite matrix using the   
    Cholesky factorization A = U**H*U or A = L*L**H computed by CPOTRF.   

    An estimate is obtained for norm(inv(A)), and the reciprocal of the   
    condition number is computed as RCOND = 1 / (ANORM * norm(inv(A))).   

    Arguments   
    =========   

    UPLO    (input) CHARACTER*1   
            = 'U':  Upper triangle of A is stored;   
            = 'L':  Lower triangle of A is stored.   

    N       (input) INTEGER   
            The order of the matrix A.  N >= 0.   

    A       (input) COMPLEX array, dimension (LDA,N)   
            The triangular factor U or L from the Cholesky factorization 
  
            A = U**H*U or A = L*L**H, as computed by CPOTRF.   

    LDA     (input) INTEGER   
            The leading dimension of the array A.  LDA >= max(1,N).   

    ANORM   (input) REAL   
            The 1-norm (or infinity-norm) of the Hermitian matrix A.   

    RCOND   (output) REAL   
            The reciprocal of the condition number of the matrix A,   
            computed as RCOND = 1/(ANORM * AINVNM), where AINVNM is an   
            estimate of the 1-norm of inv(A) computed in this routine.   

    WORK    (workspace) COMPLEX array, dimension (2*N)   

    RWORK   (workspace) REAL array, dimension (N)   

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
    integer a_dim1, a_offset, i__1;
    real r__1, r__2;
    /* Builtin functions */
    double r_imag(complex *);
    /* Local variables */
    static integer kase;
    static real scale;
    extern logical lsame_(char *, char *);
    static logical upper;
    extern /* Subroutine */ int clacon_(integer *, complex *, complex *, real 
	    *, integer *);
    static integer ix;
    extern integer icamax_(integer *, complex *, integer *);
    static real scalel;
    extern doublereal slamch_(char *);
    static real scaleu;
    extern /* Subroutine */ int xerbla_(char *, integer *);
    static real ainvnm;
    extern /* Subroutine */ int clatrs_(char *, char *, char *, char *, 
	    integer *, complex *, integer *, complex *, real *, real *, 
	    integer *), csrscl_(integer *, 
	    real *, complex *, integer *);
    static char normin[1];
    static real smlnum;



#define WORK(I) work[(I)-1]
#define RWORK(I) rwork[(I)-1]

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
	*info = -5;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("CPOCON", &i__1);
	return 0;
    }

/*     Quick return if possible */

    *rcond = 0.f;
    if (*n == 0) {
	*rcond = 1.f;
	return 0;
    } else if (*anorm == 0.f) {
	return 0;
    }

    smlnum = slamch_("Safe minimum");

/*     Estimate the 1-norm of inv(A). */

    kase = 0;
    *(unsigned char *)normin = 'N';
L10:
    clacon_(n, &WORK(*n + 1), &WORK(1), &ainvnm, &kase);
    if (kase != 0) {
	if (upper) {

/*           Multiply by inv(U'). */

	    clatrs_("Upper", "Conjugate transpose", "Non-unit", normin, n, &A(1,1), lda, &WORK(1), &scalel, &RWORK(1), info);
	    *(unsigned char *)normin = 'Y';

/*           Multiply by inv(U). */

	    clatrs_("Upper", "No transpose", "Non-unit", normin, n, &A(1,1), lda, &WORK(1), &scaleu, &RWORK(1), info);
	} else {

/*           Multiply by inv(L). */

	    clatrs_("Lower", "No transpose", "Non-unit", normin, n, &A(1,1), lda, &WORK(1), &scalel, &RWORK(1), info);
	    *(unsigned char *)normin = 'Y';

/*           Multiply by inv(L'). */

	    clatrs_("Lower", "Conjugate transpose", "Non-unit", normin, n, &A(1,1), lda, &WORK(1), &scaleu, &RWORK(1), info);
	}

/*        Multiply by 1/SCALE if doing so will not cause overflow. */

	scale = scalel * scaleu;
	if (scale != 1.f) {
	    ix = icamax_(n, &WORK(1), &c__1);
	    i__1 = ix;
	    if (scale < ((r__1 = WORK(ix).r, dabs(r__1)) + (r__2 = r_imag(&
		    WORK(ix)), dabs(r__2))) * smlnum || scale == 0.f) {
		goto L20;
	    }
	    csrscl_(n, &scale, &WORK(1), &c__1);
	}
	goto L10;
    }

/*     Compute the estimate of the reciprocal condition number. */

    if (ainvnm != 0.f) {
	*rcond = 1.f / ainvnm / *anorm;
    }

L20:
    return 0;

/*     End of CPOCON */

} /* cpocon_ */

