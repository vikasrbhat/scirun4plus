#include "f2c.h"

/* Subroutine */ int zlaed8_(integer *k, integer *n, integer *qsiz, 
	doublecomplex *q, integer *ldq, doublereal *d, doublereal *rho, 
	integer *cutpnt, doublereal *z, doublereal *dlamda, doublecomplex *q2,
	 integer *ldq2, doublereal *w, integer *indxp, integer *indx, integer 
	*indxq, integer *perm, integer *givptr, integer *givcol, doublereal *
	givnum, integer *info)
{
/*  -- LAPACK routine (version 2.0) --   
       Univ. of Tennessee, Oak Ridge National Lab, Argonne National Lab, 
  
       Courant Institute, NAG Ltd., and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    ZLAED8 merges the two sets of eigenvalues together into a single   
    sorted set.  Then it tries to deflate the size of the problem.   
    There are two ways in which deflation can occur:  when two or more   
    eigenvalues are close together or if there is a tiny element in the   
    Z vector.  For each such occurrence the order of the related secular 
  
    equation problem is reduced by one.   

    Arguments   
    =========   

    K      (output) INTEGER   
           Contains the number of non-deflated eigenvalues.   
           This is the order of the related secular equation.   

    N      (input) INTEGER   
           The dimension of the symmetric tridiagonal matrix.  N >= 0.   

    QSIZ   (input) INTEGER   
           The dimension of the unitary matrix used to reduce   
           the dense or band matrix to tridiagonal form.   
           QSIZ >= N if ICOMPQ = 1.   

    Q      (input/output) COMPLEX*16 array, dimension (LDQ,N)   
           On entry, Q contains the eigenvectors of the partially solved 
  
           system which has been previously updated in matrix   
           multiplies with other partially solved eigensystems.   
           On exit, Q contains the trailing (N-K) updated eigenvectors   
           (those which were deflated) in its last N-K columns.   

    LDQ    (input) INTEGER   
           The leading dimension of the array Q.  LDQ >= max( 1, N ).   

    D      (input/output) DOUBLE PRECISION array, dimension (N)   
           On entry, D contains the eigenvalues of the two submatrices to 
  
           be combined.  On exit, D contains the trailing (N-K) updated   
           eigenvalues (those which were deflated) sorted into increasing 
  
           order.   

    RHO    (input/output) DOUBLE PRECISION   
           Contains the off diagonal element associated with the rank-1   
           cut which originally split the two submatrices which are now   
           being recombined. RHO is modified during the computation to   
           the value required by DLAED3.   

    CUTPNT (input) INTEGER   
           Contains the location of the last eigenvalue in the leading   
           sub-matrix.  MIN(1,N) <= CUTPNT <= N.   

    Z      (input) DOUBLE PRECISION array, dimension (N)   
           On input this vector contains the updating vector (the last   
           row of the first sub-eigenvector matrix and the first row of   
           the second sub-eigenvector matrix).  The contents of Z are   
           destroyed during the updating process.   

    DLAMDA (output) DOUBLE PRECISION array, dimension (N)   
           Contains a copy of the first K eigenvalues which will be used 
  
           by DLAED3 to form the secular equation.   

    Q2     (output) COMPLEX*16 array, dimension (LDQ2,N)   
           If ICOMPQ = 0, Q2 is not referenced.  Otherwise,   
           Contains a copy of the first K eigenvectors which will be used 
  
           by DLAED7 in a matrix multiply (DGEMM) to update the new   
           eigenvectors.   

    LDQ2   (input) INTEGER   
           The leading dimension of the array Q2.  LDQ2 >= max( 1, N ).   

    W      (output) DOUBLE PRECISION array, dimension (N)   
           This will hold the first k values of the final   
           deflation-altered z-vector and will be passed to DLAED3.   

    INDXP  (workspace) INTEGER array, dimension (N)   
           This will contain the permutation used to place deflated   
           values of D at the end of the array. On output INDXP(1:K)   
           points to the nondeflated D-values and INDXP(K+1:N)   
           points to the deflated eigenvalues.   

    INDX   (workspace) INTEGER array, dimension (N)   
           This will contain the permutation used to sort the contents of 
  
           D into ascending order.   

    INDXQ  (input) INTEGER array, dimension (N)   
           This contains the permutation which separately sorts the two   
           sub-problems in D into ascending order.  Note that elements in 
  
           the second half of this permutation must first have CUTPNT   
           added to their values in order to be accurate.   

    PERM   (output) INTEGER array, dimension (N)   
           Contains the permutations (from deflation and sorting) to be   
           applied to each eigenblock.   

    GIVPTR (output) INTEGER   
           Contains the number of Givens rotations which took place in   
           this subproblem.   

    GIVCOL (output) INTEGER array, dimension (2, N)   
           Each pair of numbers indicates a pair of columns to take place 
  
           in a Givens rotation.   

    GIVNUM (output) DOUBLE PRECISION array, dimension (2, N)   
           Each number indicates the S value to be used in the   
           corresponding Givens rotation.   

    INFO   (output) INTEGER   
            = 0:  successful exit.   
            < 0:  if INFO = -i, the i-th argument had an illegal value.   

    ===================================================================== 
  


       Test the input parameters.   

       Parameter adjustments */
    /* Table of constant values */
    static doublereal c_b3 = -1.;
    static integer c__1 = 1;
    
    /* System generated locals */
    integer q_dim1, q_offset, q2_dim1, q2_offset, i__1;
    doublereal d__1;
    /* Builtin functions */
    double sqrt(doublereal);
    /* Local variables */
    static integer jlam, imax, jmax;
    static doublereal c;
    static integer i, j;
    static doublereal s, t;
    extern /* Subroutine */ int dscal_(integer *, doublereal *, doublereal *, 
	    integer *), dcopy_(integer *, doublereal *, integer *, doublereal 
	    *, integer *);
    static integer k2, n1, n2;
    extern /* Subroutine */ int zdrot_(integer *, doublecomplex *, integer *, 
	    doublecomplex *, integer *, doublereal *, doublereal *), zcopy_(
	    integer *, doublecomplex *, integer *, doublecomplex *, integer *)
	    ;
    extern doublereal dlapy2_(doublereal *, doublereal *), dlamch_(char *);
    static integer jp;
    extern integer idamax_(integer *, doublereal *, integer *);
    extern /* Subroutine */ int dlamrg_(integer *, integer *, doublereal *, 
	    integer *, integer *, integer *), xerbla_(char *, integer *), zlacpy_(char *, integer *, integer *, doublecomplex *, 
	    integer *, doublecomplex *, integer *);
    static integer n1p1;
    static doublereal eps, tau, tol;


    q_dim1 = *ldq;
    q_offset = q_dim1 + 1;
    q -= q_offset;
    --d;
    --z;
    --dlamda;
    q2_dim1 = *ldq2;
    q2_offset = q2_dim1 + 1;
    q2 -= q2_offset;
    --w;
    --indxp;
    --indx;
    --indxq;
    --perm;
    givcol -= 3;
    givnum -= 3;

    /* Function Body */
    *info = 0;

    if (*n < 0) {
	*info = -2;
    } else if (*qsiz < *n) {
	*info = -3;
    } else if (*ldq < max(1,*n)) {
	*info = -5;
    } else if (*cutpnt < min(1,*n) || *cutpnt > *n) {
	*info = -8;
    } else if (*ldq2 < max(1,*n)) {
	*info = -12;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("ZLAED8", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	return 0;
    }

    n1 = *cutpnt;
    n2 = *n - n1;
    n1p1 = n1 + 1;

    if (*rho < 0.) {
	dscal_(&n2, &c_b3, &z[n1p1], &c__1);
    }

/*     Normalize z so that norm(z) = 1 */

    t = 1. / sqrt(2.);
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	indx[j] = j;
/* L10: */
    }
    dscal_(n, &t, &z[1], &c__1);
    *rho = (d__1 = *rho * 2., abs(d__1));

/*     Sort the eigenvalues into increasing order */

    i__1 = *n;
    for (i = *cutpnt + 1; i <= i__1; ++i) {
	indxq[i] += *cutpnt;
/* L20: */
    }
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
	dlamda[i] = d[indxq[i]];
	w[i] = z[indxq[i]];
/* L30: */
    }
    i = 1;
    j = *cutpnt + 1;
    dlamrg_(&n1, &n2, &dlamda[1], &c__1, &c__1, &indx[1]);
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
	d[i] = dlamda[indx[i]];
	z[i] = w[indx[i]];
/* L40: */
    }

/*     Calculate the allowable deflation tolerance */

    imax = idamax_(n, &z[1], &c__1);
    jmax = idamax_(n, &d[1], &c__1);
    eps = dlamch_("Epsilon");
    tol = eps * 8. * (d__1 = d[jmax], abs(d__1));

/*     If the rank-1 modifier is small enough, no more needs to be done   
       -- except to reorganize Q so that its columns correspond with the 
  
       elements in D. */

    if (*rho * (d__1 = z[imax], abs(d__1)) <= tol) {
	*k = 0;
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    perm[j] = indxq[indx[j]];
	    zcopy_(qsiz, &q[perm[j] * q_dim1 + 1], &c__1, &q2[j * q2_dim1 + 1]
		    , &c__1);
/* L50: */
	}
	zlacpy_("A", qsiz, n, &q2[q2_dim1 + 1], ldq2, &q[q_dim1 + 1], ldq)
		;
	return 0;
    }

/*     If there are multiple eigenvalues then the problem deflates.  Here 
  
       the number of equal eigenvalues are found.  As each equal   
       eigenvalue is found, an elementary reflector is computed to rotate 
  
       the corresponding eigensubspace so that the corresponding   
       components of Z are zero in this new basis. */

    *k = 0;
    *givptr = 0;
    k2 = *n + 1;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	if (*rho * (d__1 = z[j], abs(d__1)) <= tol) {

/*           Deflate due to small z component. */

	    --k2;
	    indxp[k2] = j;
	    if (j == *n) {
		goto L100;
	    }
	} else {
	    jlam = j;
	    goto L70;
	}
/* L60: */
    }
L70:
    ++j;
    if (j > *n) {
	goto L90;
    }
    if (*rho * (d__1 = z[j], abs(d__1)) <= tol) {

/*        Deflate due to small z component. */

	--k2;
	indxp[k2] = j;
    } else {

/*        Check if eigenvalues are close enough to allow deflation. */

	s = z[jlam];
	c = z[j];

/*        Find sqrt(a**2+b**2) without overflow or   
          destructive underflow. */

	tau = dlapy2_(&c, &s);
	t = d[j] - d[jlam];
	c /= tau;
	s = -s / tau;
	if ((d__1 = t * c * s, abs(d__1)) <= tol) {

/*           Deflation is possible. */

	    z[j] = tau;
	    z[jlam] = 0.;

/*           Record the appropriate Givens rotation */

	    ++(*givptr);
	    givcol[(*givptr << 1) + 1] = indxq[indx[jlam]];
	    givcol[(*givptr << 1) + 2] = indxq[indx[j]];
	    givnum[(*givptr << 1) + 1] = c;
	    givnum[(*givptr << 1) + 2] = s;
	    zdrot_(qsiz, &q[indxq[indx[jlam]] * q_dim1 + 1], &c__1, &q[indxq[
		    indx[j]] * q_dim1 + 1], &c__1, &c, &s);
	    t = d[jlam] * c * c + d[j] * s * s;
	    d[j] = d[jlam] * s * s + d[j] * c * c;
	    d[jlam] = t;
	    --k2;
	    i = 1;
L80:
	    if (k2 + i <= *n) {
		if (d[jlam] < d[indxp[k2 + i]]) {
		    indxp[k2 + i - 1] = indxp[k2 + i];
		    indxp[k2 + i] = jlam;
		    ++i;
		    goto L80;
		} else {
		    indxp[k2 + i - 1] = jlam;
		}
	    } else {
		indxp[k2 + i - 1] = jlam;
	    }
	    jlam = j;
	} else {
	    ++(*k);
	    w[*k] = z[jlam];
	    dlamda[*k] = d[jlam];
	    indxp[*k] = jlam;
	    jlam = j;
	}
    }
    goto L70;
L90:

/*     Record the last eigenvalue. */

    ++(*k);
    w[*k] = z[jlam];
    dlamda[*k] = d[jlam];
    indxp[*k] = jlam;

L100:

/*     Sort the eigenvalues and corresponding eigenvectors into DLAMDA   
       and Q2 respectively.  The eigenvalues/vectors which were not   
       deflated go into the first K slots of DLAMDA and Q2 respectively, 
  
       while those which were deflated go into the last N - K slots. */

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	jp = indxp[j];
	dlamda[j] = d[jp];
	perm[j] = indxq[indx[jp]];
	zcopy_(qsiz, &q[perm[j] * q_dim1 + 1], &c__1, &q2[j * q2_dim1 + 1], &
		c__1);
/* L110: */
    }

/*     The deflated eigenvalues and their corresponding vectors go back   
       into the last N - K slots of D and Q respectively. */

    if (*k < *n) {
	i__1 = *n - *k;
	dcopy_(&i__1, &dlamda[*k + 1], &c__1, &d[*k + 1], &c__1);
	i__1 = *n - *k;
	zlacpy_("A", qsiz, &i__1, &q2[(*k + 1) * q2_dim1 + 1], ldq2, &q[(*k + 
		1) * q_dim1 + 1], ldq);
    }

    return 0;

/*     End of ZLAED8 */

} /* zlaed8_ */

