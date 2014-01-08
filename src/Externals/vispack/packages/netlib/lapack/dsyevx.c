#include "f2c.h"

/* Subroutine */ int dsyevx_(char *jobz, char *range, char *uplo, integer *n, 
	doublereal *a, integer *lda, doublereal *vl, doublereal *vu, integer *
	il, integer *iu, doublereal *abstol, integer *m, doublereal *w, 
	doublereal *z, integer *ldz, doublereal *work, integer *lwork, 
	integer *iwork, integer *ifail, integer *info)
{
/*  -- LAPACK driver routine (version 2.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    DSYEVX computes selected eigenvalues and, optionally, eigenvectors   
    of a real symmetric matrix A.  Eigenvalues and eigenvectors can be   
    selected by specifying either a range of values or a range of indices 
  
    for the desired eigenvalues.   

    Arguments   
    =========   

    JOBZ    (input) CHARACTER*1   
            = 'N':  Compute eigenvalues only;   
            = 'V':  Compute eigenvalues and eigenvectors.   

    RANGE   (input) CHARACTER*1   
            = 'A': all eigenvalues will be found.   
            = 'V': all eigenvalues in the half-open interval (VL,VU]   
                   will be found.   
            = 'I': the IL-th through IU-th eigenvalues will be found.   

    UPLO    (input) CHARACTER*1   
            = 'U':  Upper triangle of A is stored;   
            = 'L':  Lower triangle of A is stored.   

    N       (input) INTEGER   
            The order of the matrix A.  N >= 0.   

    A       (input/output) DOUBLE PRECISION array, dimension (LDA, N)   
            On entry, the symmetric matrix A.  If UPLO = 'U', the   
            leading N-by-N upper triangular part of A contains the   
            upper triangular part of the matrix A.  If UPLO = 'L',   
            the leading N-by-N lower triangular part of A contains   
            the lower triangular part of the matrix A.   
            On exit, the lower triangle (if UPLO='L') or the upper   
            triangle (if UPLO='U') of A, including the diagonal, is   
            destroyed.   

    LDA     (input) INTEGER   
            The leading dimension of the array A.  LDA >= max(1,N).   

    VL      (input) DOUBLE PRECISION   
    VU      (input) DOUBLE PRECISION   
            If RANGE='V', the lower and upper bounds of the interval to   
            be searched for eigenvalues. VL < VU.   
            Not referenced if RANGE = 'A' or 'I'.   

    IL      (input) INTEGER   
    IU      (input) INTEGER   
            If RANGE='I', the indices (in ascending order) of the   
            smallest and largest eigenvalues to be returned.   
            1 <= IL <= IU <= N, if N > 0; IL = 1 and IU = 0 if N = 0.   
            Not referenced if RANGE = 'A' or 'V'.   

    ABSTOL  (input) DOUBLE PRECISION   
            The absolute error tolerance for the eigenvalues.   
            An approximate eigenvalue is accepted as converged   
            when it is determined to lie in an interval [a,b]   
            of width less than or equal to   

                    ABSTOL + EPS *   max( |a|,|b| ) ,   

            where EPS is the machine precision.  If ABSTOL is less than   
            or equal to zero, then  EPS*|T|  will be used in its place,   
            where |T| is the 1-norm of the tridiagonal matrix obtained   
            by reducing A to tridiagonal form.   

            Eigenvalues will be computed most accurately when ABSTOL is   
            set to twice the underflow threshold 2*DLAMCH('S'), not zero. 
  
            If this routine returns with INFO>0, indicating that some   
            eigenvectors did not converge, try setting ABSTOL to   
            2*DLAMCH('S').   

            See "Computing Small Singular Values of Bidiagonal Matrices   
            with Guaranteed High Relative Accuracy," by Demmel and   
            Kahan, LAPACK Working Note #3.   

    M       (output) INTEGER   
            The total number of eigenvalues found.  0 <= M <= N.   
            If RANGE = 'A', M = N, and if RANGE = 'I', M = IU-IL+1.   

    W       (output) DOUBLE PRECISION array, dimension (N)   
            On normal exit, the first M elements contain the selected   
            eigenvalues in ascending order.   

    Z       (output) DOUBLE PRECISION array, dimension (LDZ, max(1,M))   
            If JOBZ = 'V', then if INFO = 0, the first M columns of Z   
            contain the orthonormal eigenvectors of the matrix A   
            corresponding to the selected eigenvalues, with the i-th   
            column of Z holding the eigenvector associated with W(i).   
            If an eigenvector fails to converge, then that column of Z   
            contains the latest approximation to the eigenvector, and the 
  
            index of the eigenvector is returned in IFAIL.   
            If JOBZ = 'N', then Z is not referenced.   
            Note: the user must ensure that at least max(1,M) columns are 
  
            supplied in the array Z; if RANGE = 'V', the exact value of M 
  
            is not known in advance and an upper bound must be used.   

    LDZ     (input) INTEGER   
            The leading dimension of the array Z.  LDZ >= 1, and if   
            JOBZ = 'V', LDZ >= max(1,N).   

    WORK    (workspace/output) DOUBLE PRECISION array, dimension (LWORK) 
  
            On exit, if INFO = 0, WORK(1) returns the optimal LWORK.   

    LWORK   (input) INTEGER   
            The length of the array WORK.  LWORK >= max(1,8*N).   
            For optimal efficiency, LWORK >= (NB+3)*N,   
            where NB is the blocksize for DSYTRD returned by ILAENV.   

    IWORK   (workspace) INTEGER array, dimension (5*N)   

    IFAIL   (output) INTEGER array, dimension (N)   
            If JOBZ = 'V', then if INFO = 0, the first M elements of   
            IFAIL are zero.  If INFO > 0, then IFAIL contains the   
            indices of the eigenvectors that failed to converge.   
            If JOBZ = 'N', then IFAIL is not referenced.   

    INFO    (output) INTEGER   
            = 0:  successful exit   
            < 0:  if INFO = -i, the i-th argument had an illegal value   
            > 0:  if INFO = i, then i eigenvectors failed to converge.   
                  Their indices are stored in array IFAIL.   

   ===================================================================== 
  


       Test the input parameters.   

    
   Parameter adjustments   
       Function Body */
    /* Table of constant values */
    static integer c__1 = 1;
    
    /* System generated locals */
    integer a_dim1, a_offset, z_dim1, z_offset, i__1, i__2;
    doublereal d__1, d__2;
    /* Builtin functions */
    double sqrt(doublereal);
    /* Local variables */
    static integer indd, inde;
    static doublereal anrm;
    static integer imax;
    static doublereal rmin, rmax;
    static integer lopt, itmp1, i, j, indee;
    extern /* Subroutine */ int dscal_(integer *, doublereal *, doublereal *, 
	    integer *);
    static doublereal sigma;
    extern logical lsame_(char *, char *);
    static integer iinfo;
    static char order[1];
    extern /* Subroutine */ int dcopy_(integer *, doublereal *, integer *, 
	    doublereal *, integer *), dswap_(integer *, doublereal *, integer 
	    *, doublereal *, integer *);
    static logical lower, wantz;
    static integer jj;
    extern doublereal dlamch_(char *);
    static logical alleig, indeig;
    static integer iscale, indibl;
    static logical valeig;
    extern /* Subroutine */ int dlacpy_(char *, integer *, integer *, 
	    doublereal *, integer *, doublereal *, integer *);
    static doublereal safmin;
    extern /* Subroutine */ int xerbla_(char *, integer *);
    static doublereal abstll, bignum;
    static integer indtau, indisp;
    extern /* Subroutine */ int dstein_(integer *, doublereal *, doublereal *,
	     integer *, doublereal *, integer *, integer *, doublereal *, 
	    integer *, doublereal *, integer *, integer *, integer *), 
	    dsterf_(integer *, doublereal *, doublereal *, integer *);
    static integer indiwo, indwkn;
    extern doublereal dlansy_(char *, char *, integer *, doublereal *, 
	    integer *, doublereal *);
    extern /* Subroutine */ int dstebz_(char *, char *, integer *, doublereal 
	    *, doublereal *, integer *, integer *, doublereal *, doublereal *,
	     doublereal *, integer *, integer *, doublereal *, integer *, 
	    integer *, doublereal *, integer *, integer *);
    static integer indwrk;
    extern /* Subroutine */ int dorgtr_(char *, integer *, doublereal *, 
	    integer *, doublereal *, doublereal *, integer *, integer *), dsteqr_(char *, integer *, doublereal *, doublereal *, 
	    doublereal *, integer *, doublereal *, integer *), 
	    dormtr_(char *, char *, char *, integer *, integer *, doublereal *
	    , integer *, doublereal *, doublereal *, integer *, doublereal *, 
	    integer *, integer *);
    static integer llwrkn, llwork, nsplit;
    static doublereal smlnum;
    extern /* Subroutine */ int dsytrd_(char *, integer *, doublereal *, 
	    integer *, doublereal *, doublereal *, doublereal *, doublereal *,
	     integer *, integer *);
    static doublereal eps, vll, vuu, tmp1;



#define W(I) w[(I)-1]
#define WORK(I) work[(I)-1]
#define IWORK(I) iwork[(I)-1]
#define IFAIL(I) ifail[(I)-1]

#define A(I,J) a[(I)-1 + ((J)-1)* ( *lda)]
#define Z(I,J) z[(I)-1 + ((J)-1)* ( *ldz)]

    lower = lsame_(uplo, "L");
    wantz = lsame_(jobz, "V");
    alleig = lsame_(range, "A");
    valeig = lsame_(range, "V");
    indeig = lsame_(range, "I");

    *info = 0;
    if (! (wantz || lsame_(jobz, "N"))) {
	*info = -1;
    } else if (! (alleig || valeig || indeig)) {
	*info = -2;
    } else if (! (lower || lsame_(uplo, "U"))) {
	*info = -3;
    } else if (*n < 0) {
	*info = -4;
    } else if (*lda < max(1,*n)) {
	*info = -6;
    } else if (valeig && *n > 0 && *vu <= *vl) {
	*info = -8;
    } else if (indeig && *il < 1) {
	*info = -9;
    } else if (indeig && (*iu < min(*n,*il) || *iu > *n)) {
	*info = -10;
    } else if (*ldz < 1 || wantz && *ldz < *n) {
	*info = -15;
    } else /* if(complicated condition) */ {
/* Computing MAX */
	i__1 = 1, i__2 = *n << 3;
	if (*lwork < max(i__1,i__2)) {
	    *info = -17;
	}
    }

    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("DSYEVX", &i__1);
	return 0;
    }

/*     Quick return if possible */

    *m = 0;
    if (*n == 0) {
	WORK(1) = 1.;
	return 0;
    }

    if (*n == 1) {
	WORK(1) = 7.;
	if (alleig || indeig) {
	    *m = 1;
	    W(1) = A(1,1);
	} else {
	    if (*vl < A(1,1) && *vu >= A(1,1)) {
		*m = 1;
		W(1) = A(1,1);
	    }
	}
	if (wantz) {
	    Z(1,1) = 1.;
	}
	return 0;
    }

/*     Get machine constants. */

    safmin = dlamch_("Safe minimum");
    eps = dlamch_("Precision");
    smlnum = safmin / eps;
    bignum = 1. / smlnum;
    rmin = sqrt(smlnum);
/* Computing MIN */
    d__1 = sqrt(bignum), d__2 = 1. / sqrt(sqrt(safmin));
    rmax = min(d__1,d__2);

/*     Scale matrix to allowable range, if necessary. */

    iscale = 0;
    abstll = *abstol;
    if (valeig) {
	vll = *vl;
	vuu = *vu;
    }
    anrm = dlansy_("M", uplo, n, &A(1,1), lda, &WORK(1));
    if (anrm > 0. && anrm < rmin) {
	iscale = 1;
	sigma = rmin / anrm;
    } else if (anrm > rmax) {
	iscale = 1;
	sigma = rmax / anrm;
    }
    if (iscale == 1) {
	if (lower) {
	    i__1 = *n;
	    for (j = 1; j <= *n; ++j) {
		i__2 = *n - j + 1;
		dscal_(&i__2, &sigma, &A(j,j), &c__1);
/* L10: */
	    }
	} else {
	    i__1 = *n;
	    for (j = 1; j <= *n; ++j) {
		dscal_(&j, &sigma, &A(1,j), &c__1);
/* L20: */
	    }
	}
	if (*abstol > 0.) {
	    abstll = *abstol * sigma;
	}
	if (valeig) {
	    vll = *vl * sigma;
	    vuu = *vu * sigma;
	}
    }

/*     Call DSYTRD to reduce symmetric matrix to tridiagonal form. */

    indtau = 1;
    inde = indtau + *n;
    indd = inde + *n;
    indwrk = indd + *n;
    llwork = *lwork - indwrk + 1;
    dsytrd_(uplo, n, &A(1,1), lda, &WORK(indd), &WORK(inde), &WORK(
	    indtau), &WORK(indwrk), &llwork, &iinfo);
    lopt = (integer) (*n * 3 + WORK(indwrk));

/*     If all eigenvalues are desired and ABSTOL is less than or equal to 
  
       zero, then call DSTERF or DORGTR and SSTEQR.  If this fails for   
       some eigenvalue, then try DSTEBZ. */

    if ((alleig || indeig && *il == 1 && *iu == *n) && *abstol <= 0.) {
	dcopy_(n, &WORK(indd), &c__1, &W(1), &c__1);
	indee = indwrk + (*n << 1);
	if (! wantz) {
	    i__1 = *n - 1;
	    dcopy_(&i__1, &WORK(inde), &c__1, &WORK(indee), &c__1);
	    dsterf_(n, &W(1), &WORK(indee), info);
	} else {
	    dlacpy_("A", n, n, &A(1,1), lda, &Z(1,1), ldz);
	    dorgtr_(uplo, n, &Z(1,1), ldz, &WORK(indtau), &WORK(indwrk), 
		    &llwork, &iinfo);
	    i__1 = *n - 1;
	    dcopy_(&i__1, &WORK(inde), &c__1, &WORK(indee), &c__1);
	    dsteqr_(jobz, n, &W(1), &WORK(indee), &Z(1,1), ldz, &WORK(
		    indwrk), info);
	    if (*info == 0) {
		i__1 = *n;
		for (i = 1; i <= *n; ++i) {
		    IFAIL(i) = 0;
/* L30: */
		}
	    }
	}
	if (*info == 0) {
	    *m = *n;
	    goto L40;
	}
	*info = 0;
    }

/*     Otherwise, call DSTEBZ and, if eigenvectors are desired, SSTEIN. */

    if (wantz) {
	*(unsigned char *)order = 'B';
    } else {
	*(unsigned char *)order = 'E';
    }
    indibl = 1;
    indisp = indibl + *n;
    indiwo = indisp + *n;
    dstebz_(range, order, n, &vll, &vuu, il, iu, &abstll, &WORK(indd), &WORK(
	    inde), m, &nsplit, &W(1), &IWORK(indibl), &IWORK(indisp), &WORK(
	    indwrk), &IWORK(indiwo), info);

    if (wantz) {
	dstein_(n, &WORK(indd), &WORK(inde), m, &W(1), &IWORK(indibl), &IWORK(
		indisp), &Z(1,1), ldz, &WORK(indwrk), &IWORK(indiwo), &
		IFAIL(1), info);

/*        Apply orthogonal matrix used in reduction to tridiagonal   
          form to eigenvectors returned by DSTEIN. */

	indwkn = inde;
	llwrkn = *lwork - indwkn + 1;
	dormtr_("L", uplo, "N", n, m, &A(1,1), lda, &WORK(indtau), &Z(1,1), ldz, &WORK(indwkn), &llwrkn, &iinfo);
    }

/*     If matrix was scaled, then rescale eigenvalues appropriately. */

L40:
    if (iscale == 1) {
	if (*info == 0) {
	    imax = *m;
	} else {
	    imax = *info - 1;
	}
	d__1 = 1. / sigma;
	dscal_(&imax, &d__1, &W(1), &c__1);
    }

/*     If eigenvalues are not in order, then sort them, along with   
       eigenvectors. */

    if (wantz) {
	i__1 = *m - 1;
	for (j = 1; j <= *m-1; ++j) {
	    i = 0;
	    tmp1 = W(j);
	    i__2 = *m;
	    for (jj = j + 1; jj <= *m; ++jj) {
		if (W(jj) < tmp1) {
		    i = jj;
		    tmp1 = W(jj);
		}
/* L50: */
	    }

	    if (i != 0) {
		itmp1 = IWORK(indibl + i - 1);
		W(i) = W(j);
		IWORK(indibl + i - 1) = IWORK(indibl + j - 1);
		W(j) = tmp1;
		IWORK(indibl + j - 1) = itmp1;
		dswap_(n, &Z(1,i), &c__1, &Z(1,j), &
			c__1);
		if (*info != 0) {
		    itmp1 = IFAIL(i);
		    IFAIL(i) = IFAIL(j);
		    IFAIL(j) = itmp1;
		}
	    }
/* L60: */
	}
    }

/*     Set WORK(1) to optimal workspace size.   

   Computing MAX */
    i__1 = *n * 7;
    WORK(1) = (doublereal) max(i__1,lopt);

    return 0;

/*     End of DSYEVX */

} /* dsyevx_ */

