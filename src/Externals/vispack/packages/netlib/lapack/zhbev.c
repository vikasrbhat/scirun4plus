#include "f2c.h"

/* Subroutine */ int zhbev_(char *jobz, char *uplo, integer *n, integer *kd, 
	doublecomplex *ab, integer *ldab, doublereal *w, doublecomplex *z, 
	integer *ldz, doublecomplex *work, doublereal *rwork, integer *info)
{
/*  -- LAPACK driver routine (version 2.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    ZHBEV computes all the eigenvalues and, optionally, eigenvectors of   
    a complex Hermitian band matrix A.   

    Arguments   
    =========   

    JOBZ    (input) CHARACTER*1   
            = 'N':  Compute eigenvalues only;   
            = 'V':  Compute eigenvalues and eigenvectors.   

    UPLO    (input) CHARACTER*1   
            = 'U':  Upper triangle of A is stored;   
            = 'L':  Lower triangle of A is stored.   

    N       (input) INTEGER   
            The order of the matrix A.  N >= 0.   

    KD      (input) INTEGER   
            The number of superdiagonals of the matrix A if UPLO = 'U',   
            or the number of subdiagonals if UPLO = 'L'.  KD >= 0.   

    AB      (input/output) COMPLEX*16 array, dimension (LDAB, N)   
            On entry, the upper or lower triangle of the Hermitian band   
            matrix A, stored in the first KD+1 rows of the array.  The   
            j-th column of A is stored in the j-th column of the array AB 
  
            as follows:   
            if UPLO = 'U', AB(kd+1+i-j,j) = A(i,j) for max(1,j-kd)<=i<=j; 
  
            if UPLO = 'L', AB(1+i-j,j)    = A(i,j) for j<=i<=min(n,j+kd). 
  

            On exit, AB is overwritten by values generated during the   
            reduction to tridiagonal form.  If UPLO = 'U', the first   
            superdiagonal and the diagonal of the tridiagonal matrix T   
            are returned in rows KD and KD+1 of AB, and if UPLO = 'L',   
            the diagonal and first subdiagonal of T are returned in the   
            first two rows of AB.   

    LDAB    (input) INTEGER   
            The leading dimension of the array AB.  LDAB >= KD + 1.   

    W       (output) DOUBLE PRECISION array, dimension (N)   
            If INFO = 0, the eigenvalues in ascending order.   

    Z       (output) COMPLEX*16 array, dimension (LDZ, N)   
            If JOBZ = 'V', then if INFO = 0, Z contains the orthonormal   
            eigenvectors of the matrix A, with the i-th column of Z   
            holding the eigenvector associated with W(i).   
            If JOBZ = 'N', then Z is not referenced.   

    LDZ     (input) INTEGER   
            The leading dimension of the array Z.  LDZ >= 1, and if   
            JOBZ = 'V', LDZ >= max(1,N).   

    WORK    (workspace) COMPLEX*16 array, dimension (N)   

    RWORK   (workspace) DOUBLE PRECISION array, dimension (max(1,3*N-2)) 
  

    INFO    (output) INTEGER   
            = 0:  successful exit.   
            < 0:  if INFO = -i, the i-th argument had an illegal value.   
            > 0:  if INFO = i, the algorithm failed to converge; i   
                  off-diagonal elements of an intermediate tridiagonal   
                  form did not converge to zero.   

    ===================================================================== 
  


       Test the input parameters.   

    
   Parameter adjustments   
       Function Body */
    /* Table of constant values */
    static doublereal c_b11 = 1.;
    static integer c__1 = 1;
    
    /* System generated locals */
    integer ab_dim1, ab_offset, z_dim1, z_offset, i__1;
    doublereal d__1;
    /* Builtin functions */
    double sqrt(doublereal);
    /* Local variables */
    static integer inde;
    static doublereal anrm;
    static integer imax;
    static doublereal rmin, rmax;
    extern /* Subroutine */ int dscal_(integer *, doublereal *, doublereal *, 
	    integer *);
    static doublereal sigma;
    extern logical lsame_(char *, char *);
    static integer iinfo;
    static logical lower, wantz;
    extern doublereal dlamch_(char *);
    static integer iscale;
    static doublereal safmin;
    extern doublereal zlanhb_(char *, char *, integer *, integer *, 
	    doublecomplex *, integer *, doublereal *);
    extern /* Subroutine */ int xerbla_(char *, integer *);
    static doublereal bignum;
    extern /* Subroutine */ int dsterf_(integer *, doublereal *, doublereal *,
	     integer *), zlascl_(char *, integer *, integer *, doublereal *, 
	    doublereal *, integer *, integer *, doublecomplex *, integer *, 
	    integer *), zhbtrd_(char *, char *, integer *, integer *, 
	    doublecomplex *, integer *, doublereal *, doublereal *, 
	    doublecomplex *, integer *, doublecomplex *, integer *);
    static integer indrwk;
    static doublereal smlnum;
    extern /* Subroutine */ int zsteqr_(char *, integer *, doublereal *, 
	    doublereal *, doublecomplex *, integer *, doublereal *, integer *);
    static doublereal eps;



#define W(I) w[(I)-1]
#define WORK(I) work[(I)-1]
#define RWORK(I) rwork[(I)-1]

#define AB(I,J) ab[(I)-1 + ((J)-1)* ( *ldab)]
#define Z(I,J) z[(I)-1 + ((J)-1)* ( *ldz)]

    wantz = lsame_(jobz, "V");
    lower = lsame_(uplo, "L");

    *info = 0;
    if (! (wantz || lsame_(jobz, "N"))) {
	*info = -1;
    } else if (! (lower || lsame_(uplo, "U"))) {
	*info = -2;
    } else if (*n < 0) {
	*info = -3;
    } else if (*kd < 0) {
	*info = -4;
    } else if (*ldab < *kd + 1) {
	*info = -6;
    } else if (*ldz < 1 || wantz && *ldz < *n) {
	*info = -9;
    }

    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("ZHBEV ", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	return 0;
    }

    if (*n == 1) {
	i__1 = ab_dim1 + 1;
	W(1) = AB(1,1).r;
	if (wantz) {
	    i__1 = z_dim1 + 1;
	    Z(1,1).r = 1., Z(1,1).i = 0.;
	}
	return 0;
    }

/*     Get machine constants. */

    safmin = dlamch_("Safe minimum");
    eps = dlamch_("Precision");
    smlnum = safmin / eps;
    bignum = 1. / smlnum;
    rmin = sqrt(smlnum);
    rmax = sqrt(bignum);

/*     Scale matrix to allowable range, if necessary. */

    anrm = zlanhb_("M", uplo, n, kd, &AB(1,1), ldab, &RWORK(1));
    iscale = 0;
    if (anrm > 0. && anrm < rmin) {
	iscale = 1;
	sigma = rmin / anrm;
    } else if (anrm > rmax) {
	iscale = 1;
	sigma = rmax / anrm;
    }
    if (iscale == 1) {
	if (lower) {
	    zlascl_("B", kd, kd, &c_b11, &sigma, n, n, &AB(1,1), ldab, 
		    info);
	} else {
	    zlascl_("Q", kd, kd, &c_b11, &sigma, n, n, &AB(1,1), ldab, 
		    info);
	}
    }

/*     Call ZHBTRD to reduce Hermitian band matrix to tridiagonal form. */

    inde = 1;
    zhbtrd_(jobz, uplo, n, kd, &AB(1,1), ldab, &W(1), &RWORK(inde), &Z(1,1), ldz, &WORK(1), &iinfo);

/*     For eigenvalues only, call DSTERF.  For eigenvectors, call ZSTEQR. 
*/

    if (! wantz) {
	dsterf_(n, &W(1), &RWORK(inde), info);
    } else {
	indrwk = inde + *n;
	zsteqr_(jobz, n, &W(1), &RWORK(inde), &Z(1,1), ldz, &RWORK(
		indrwk), info);
    }

/*     If matrix was scaled, then rescale eigenvalues appropriately. */

    if (iscale == 1) {
	if (*info == 0) {
	    imax = *n;
	} else {
	    imax = *info - 1;
	}
	d__1 = 1. / sigma;
	dscal_(&imax, &d__1, &W(1), &c__1);
    }

    return 0;

/*     End of ZHBEV */

} /* zhbev_ */

