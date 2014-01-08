#include "f2c.h"

/* Subroutine */ int cptsvx_(char *fact, integer *n, integer *nrhs, real *d, 
	complex *e, real *df, complex *ef, complex *b, integer *ldb, complex *
	x, integer *ldx, real *rcond, real *ferr, real *berr, complex *work, 
	real *rwork, integer *info)
{
/*  -- LAPACK routine (version 2.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    CPTSVX uses the factorization A = L*D*L**H to compute the solution   
    to a complex system of linear equations A*X = B, where A is an   
    N-by-N Hermitian positive definite tridiagonal matrix and X and B   
    are N-by-NRHS matrices.   

    Error bounds on the solution and a condition estimate are also   
    provided.   

    Description   
    ===========   

    The following steps are performed:   

    1. If FACT = 'N', the matrix A is factored as A = L*D*L**H, where L   
       is a unit lower bidiagonal matrix and D is diagonal.  The   
       factorization can also be regarded as having the form   
       A = U**H*D*U.   

    2. The factored form of A is used to compute the condition number   
       of the matrix A.  If the reciprocal of the condition number is   
       less than machine precision, steps 3 and 4 are skipped.   

    3. The system of equations is solved for X using the factored form   
       of A.   

    4. Iterative refinement is applied to improve the computed solution   
       matrix and calculate error bounds and backward error estimates   
       for it.   

    Arguments   
    =========   

    FACT    (input) CHARACTER*1   
            Specifies whether or not the factored form of the matrix   
            A is supplied on entry.   
            = 'F':  On entry, DF and EF contain the factored form of A.   
                    D, E, DF, and EF will not be modified.   
            = 'N':  The matrix A will be copied to DF and EF and   
                    factored.   

    N       (input) INTEGER   
            The order of the matrix A.  N >= 0.   

    NRHS    (input) INTEGER   
            The number of right hand sides, i.e., the number of columns   
            of the matrices B and X.  NRHS >= 0.   

    D       (input) REAL array, dimension (N)   
            The n diagonal elements of the tridiagonal matrix A.   

    E       (input) COMPLEX array, dimension (N-1)   
            The (n-1) subdiagonal elements of the tridiagonal matrix A.   

    DF      (input or output) REAL array, dimension (N)   
            If FACT = 'F', then DF is an input argument and on entry   
            contains the n diagonal elements of the diagonal matrix D   
            from the L*D*L**H factorization of A.   
            If FACT = 'N', then DF is an output argument and on exit   
            contains the n diagonal elements of the diagonal matrix D   
            from the L*D*L**H factorization of A.   

    EF      (input or output) COMPLEX array, dimension (N-1)   
            If FACT = 'F', then EF is an input argument and on entry   
            contains the (n-1) subdiagonal elements of the unit   
            bidiagonal factor L from the L*D*L**H factorization of A.   
            If FACT = 'N', then EF is an output argument and on exit   
            contains the (n-1) subdiagonal elements of the unit   
            bidiagonal factor L from the L*D*L**H factorization of A.   

    B       (input) COMPLEX array, dimension (LDB,NRHS)   
            The N-by-NRHS right hand side matrix B.   

    LDB     (input) INTEGER   
            The leading dimension of the array B.  LDB >= max(1,N).   

    X       (output) COMPLEX array, dimension (LDX,NRHS)   
            If INFO = 0, the N-by-NRHS solution matrix X.   

    LDX     (input) INTEGER   
            The leading dimension of the array X.  LDX >= max(1,N).   

    RCOND   (output) REAL   
            The reciprocal condition number of the matrix A.  If RCOND   
            is less than the machine precision (in particular, if   
            RCOND = 0), the matrix is singular to working precision.   
            This condition is indicated by a return code of INFO > 0,   
            and the solution and error bounds are not computed.   

    FERR    (output) REAL array, dimension (NRHS)   
            The forward error bound for each solution vector   
            X(j) (the j-th column of the solution matrix X).   
            If XTRUE is the true solution corresponding to X(j), FERR(j) 
  
            is an estimated upper bound for the magnitude of the largest 
  
            element in (X(j) - XTRUE) divided by the magnitude of the   
            largest element in X(j).   

    BERR    (output) REAL array, dimension (NRHS)   
            The componentwise relative backward error of each solution   
            vector X(j) (i.e., the smallest relative change in any   
            element of A or B that makes X(j) an exact solution).   

    WORK    (workspace) COMPLEX array, dimension (N)   

    RWORK   (workspace) REAL array, dimension (N)   

    INFO    (output) INTEGER   
            = 0:  successful exit   
            < 0:  if INFO = -i, the i-th argument had an illegal value   
            > 0:  if INFO = i, and i is   
                  <= N  the leading minor of order i of A is not   
                  positive definite, so the factorization could not be   
                  completed unless i = N, and the solution and error   
                  bounds could not be computed.   
                  = N+1 RCOND is less than machine precision.  The   
                  factorization has been completed, but the matrix is   
                  singular to working precision, and the solution and   
                  error bounds have not been computed.   

    ===================================================================== 
  


       Test the input parameters.   

    
   Parameter adjustments   
       Function Body */
    /* Table of constant values */
    static integer c__1 = 1;
    
    /* System generated locals */
    integer b_dim1, b_offset, x_dim1, x_offset, i__1;
    /* Local variables */
    extern logical lsame_(char *, char *);
    static real anorm;
    extern /* Subroutine */ int ccopy_(integer *, complex *, integer *, 
	    complex *, integer *), scopy_(integer *, real *, integer *, real *
	    , integer *);
    extern doublereal slamch_(char *), clanht_(char *, integer *, 
	    real *, complex *);
    static logical nofact;
    extern /* Subroutine */ int clacpy_(char *, integer *, integer *, complex 
	    *, integer *, complex *, integer *), xerbla_(char *, 
	    integer *), cptcon_(integer *, real *, complex *, real *, 
	    real *, real *, integer *), cptrfs_(char *, integer *, integer *, 
	    real *, complex *, real *, complex *, complex *, integer *, 
	    complex *, integer *, real *, real *, complex *, real *, integer *
	    ), cpttrf_(integer *, real *, complex *, integer *), 
	    cpttrs_(char *, integer *, integer *, real *, complex *, complex *
	    , integer *, integer *);



#define D(I) d[(I)-1]
#define E(I) e[(I)-1]
#define DF(I) df[(I)-1]
#define EF(I) ef[(I)-1]
#define FERR(I) ferr[(I)-1]
#define BERR(I) berr[(I)-1]
#define WORK(I) work[(I)-1]
#define RWORK(I) rwork[(I)-1]

#define B(I,J) b[(I)-1 + ((J)-1)* ( *ldb)]
#define X(I,J) x[(I)-1 + ((J)-1)* ( *ldx)]

    *info = 0;
    nofact = lsame_(fact, "N");
    if (! nofact && ! lsame_(fact, "F")) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*nrhs < 0) {
	*info = -3;
    } else if (*ldb < max(1,*n)) {
	*info = -9;
    } else if (*ldx < max(1,*n)) {
	*info = -11;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("CPTSVX", &i__1);
	return 0;
    }

    if (nofact) {

/*        Compute the L*D*L' (or U'*D*U) factorization of A. */

	scopy_(n, &D(1), &c__1, &DF(1), &c__1);
	if (*n > 1) {
	    i__1 = *n - 1;
	    ccopy_(&i__1, &E(1), &c__1, &EF(1), &c__1);
	}
	cpttrf_(n, &DF(1), &EF(1), info);

/*        Return if INFO is non-zero. */

	if (*info != 0) {
	    if (*info > 0) {
		*rcond = 0.f;
	    }
	    return 0;
	}
    }

/*     Compute the norm of the matrix A. */

    anorm = clanht_("1", n, &D(1), &E(1));

/*     Compute the reciprocal of the condition number of A. */

    cptcon_(n, &DF(1), &EF(1), &anorm, rcond, &RWORK(1), info);

/*     Return if the matrix is singular to working precision. */

    if (*rcond < slamch_("Epsilon")) {
	*info = *n + 1;
	return 0;
    }

/*     Compute the solution vectors X. */

    clacpy_("Full", n, nrhs, &B(1,1), ldb, &X(1,1), ldx);
    cpttrs_("Lower", n, nrhs, &DF(1), &EF(1), &X(1,1), ldx, info);

/*     Use iterative refinement to improve the computed solutions and   
       compute error bounds and backward error estimates for them. */

    cptrfs_("Lower", n, nrhs, &D(1), &E(1), &DF(1), &EF(1), &B(1,1), ldb,
	     &X(1,1), ldx, &FERR(1), &BERR(1), &WORK(1), &RWORK(1), info);

    return 0;

/*     End of CPTSVX */

} /* cptsvx_ */

