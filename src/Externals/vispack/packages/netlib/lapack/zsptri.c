#include "f2c.h"

/* Subroutine */ int zsptri_(char *uplo, integer *n, doublecomplex *ap, 
	integer *ipiv, doublecomplex *work, integer *info)
{
/*  -- LAPACK routine (version 2.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    ZSPTRI computes the inverse of a complex symmetric indefinite matrix 
  
    A in packed storage using the factorization A = U*D*U**T or   
    A = L*D*L**T computed by ZSPTRF.   

    Arguments   
    =========   

    UPLO    (input) CHARACTER*1   
            Specifies whether the details of the factorization are stored 
  
            as an upper or lower triangular matrix.   
            = 'U':  Upper triangular, form is A = U*D*U**T;   
            = 'L':  Lower triangular, form is A = L*D*L**T.   

    N       (input) INTEGER   
            The order of the matrix A.  N >= 0.   

    AP      (input/output) COMPLEX*16 array, dimension (N*(N+1)/2)   
            On entry, the block diagonal matrix D and the multipliers   
            used to obtain the factor U or L as computed by ZSPTRF,   
            stored as a packed triangular matrix.   

            On exit, if INFO = 0, the (symmetric) inverse of the original 
  
            matrix, stored as a packed triangular matrix. The j-th column 
  
            of inv(A) is stored in the array AP as follows:   
            if UPLO = 'U', AP(i + (j-1)*j/2) = inv(A)(i,j) for 1<=i<=j;   
            if UPLO = 'L',   
               AP(i + (j-1)*(2n-j)/2) = inv(A)(i,j) for j<=i<=n.   

    IPIV    (input) INTEGER array, dimension (N)   
            Details of the interchanges and the block structure of D   
            as determined by ZSPTRF.   

    WORK    (workspace) COMPLEX*16 array, dimension (N)   

    INFO    (output) INTEGER   
            = 0: successful exit   
            < 0: if INFO = -i, the i-th argument had an illegal value   
            > 0: if INFO = i, D(i,i) = 0; the matrix is singular and its 
  
                 inverse could not be computed.   

    ===================================================================== 
  


       Test the input parameters.   

    
   Parameter adjustments   
       Function Body */
    /* Table of constant values */
    static doublecomplex c_b1 = {1.,0.};
    static doublecomplex c_b2 = {0.,0.};
    static integer c__1 = 1;
    
    /* System generated locals */
    integer i__1, i__2, i__3;
    doublecomplex z__1, z__2, z__3;
    /* Builtin functions */
    void z_div(doublecomplex *, doublecomplex *, doublecomplex *);
    /* Local variables */
    static doublecomplex temp, akkp1, d;
    static integer j, k;
    static doublecomplex t;
    extern logical lsame_(char *, char *);
    static integer kstep;
    static logical upper;
    extern /* Subroutine */ int zcopy_(integer *, doublecomplex *, integer *, 
	    doublecomplex *, integer *);
    extern /* Double Complex */ VOID zdotu_(doublecomplex *, integer *, 
	    doublecomplex *, integer *, doublecomplex *, integer *);
    extern /* Subroutine */ int zswap_(integer *, doublecomplex *, integer *, 
	    doublecomplex *, integer *), zspmv_(char *, integer *, 
	    doublecomplex *, doublecomplex *, doublecomplex *, integer *, 
	    doublecomplex *, doublecomplex *, integer *);
    static doublecomplex ak;
    static integer kc, kp, kx;
    extern /* Subroutine */ int xerbla_(char *, integer *);
    static integer kcnext, kpc, npp;
    static doublecomplex akp1;



#define WORK(I) work[(I)-1]
#define IPIV(I) ipiv[(I)-1]
#define AP(I) ap[(I)-1]


    *info = 0;
    upper = lsame_(uplo, "U");
    if (! upper && ! lsame_(uplo, "L")) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("ZSPTRI", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	return 0;
    }

/*     Check that the diagonal matrix D is nonsingular. */

    if (upper) {

/*        Upper triangular storage: examine D from bottom to top */

	kp = *n * (*n + 1) / 2;
	for (*info = *n; *info >= 1; --(*info)) {
	    i__1 = kp;
	    if (IPIV(*info) > 0 && (AP(kp).r == 0. && AP(kp).i == 0.)) {
		return 0;
	    }
	    kp -= *info;
/* L10: */
	}
    } else {

/*        Lower triangular storage: examine D from top to bottom. */

	kp = 1;
	i__1 = *n;
	for (*info = 1; *info <= i__1; ++(*info)) {
	    i__2 = kp;
	    if (IPIV(*info) > 0 && (AP(kp).r == 0. && AP(kp).i == 0.)) {
		return 0;
	    }
	    kp = kp + *n - *info + 1;
/* L20: */
	}
    }
    *info = 0;

    if (upper) {

/*        Compute inv(A) from the factorization A = U*D*U'.   

          K is the main loop index, increasing from 1 to N in steps of
   
          1 or 2, depending on the size of the diagonal blocks. */

	k = 1;
	kc = 1;
L30:

/*        If K > N, exit from loop. */

	if (k > *n) {
	    goto L50;
	}

	kcnext = kc + k;
	if (IPIV(k) > 0) {

/*           1 x 1 diagonal block   

             Invert the diagonal block. */

	    i__1 = kc + k - 1;
	    z_div(&z__1, &c_b1, &AP(kc + k - 1));
	    AP(kc+k-1).r = z__1.r, AP(kc+k-1).i = z__1.i;

/*           Compute column K of the inverse. */

	    if (k > 1) {
		i__1 = k - 1;
		zcopy_(&i__1, &AP(kc), &c__1, &WORK(1), &c__1);
		i__1 = k - 1;
		z__1.r = -1., z__1.i = 0.;
		zspmv_(uplo, &i__1, &z__1, &AP(1), &WORK(1), &c__1, &c_b2, &
			AP(kc), &c__1);
		i__1 = kc + k - 1;
		i__2 = kc + k - 1;
		i__3 = k - 1;
		zdotu_(&z__2, &i__3, &WORK(1), &c__1, &AP(kc), &c__1);
		z__1.r = AP(kc+k-1).r - z__2.r, z__1.i = AP(kc+k-1).i - z__2.i;
		AP(kc+k-1).r = z__1.r, AP(kc+k-1).i = z__1.i;
	    }
	    kstep = 1;
	} else {

/*           2 x 2 diagonal block   

             Invert the diagonal block. */

	    i__1 = kcnext + k - 1;
	    t.r = AP(kcnext+k-1).r, t.i = AP(kcnext+k-1).i;
	    z_div(&z__1, &AP(kc + k - 1), &t);
	    ak.r = z__1.r, ak.i = z__1.i;
	    z_div(&z__1, &AP(kcnext + k), &t);
	    akp1.r = z__1.r, akp1.i = z__1.i;
	    z_div(&z__1, &AP(kcnext + k - 1), &t);
	    akkp1.r = z__1.r, akkp1.i = z__1.i;
	    z__3.r = ak.r * akp1.r - ak.i * akp1.i, z__3.i = ak.r * akp1.i + 
		    ak.i * akp1.r;
	    z__2.r = z__3.r - 1., z__2.i = z__3.i + 0.;
	    z__1.r = t.r * z__2.r - t.i * z__2.i, z__1.i = t.r * z__2.i + t.i 
		    * z__2.r;
	    d.r = z__1.r, d.i = z__1.i;
	    i__1 = kc + k - 1;
	    z_div(&z__1, &akp1, &d);
	    AP(kc+k-1).r = z__1.r, AP(kc+k-1).i = z__1.i;
	    i__1 = kcnext + k;
	    z_div(&z__1, &ak, &d);
	    AP(kcnext+k).r = z__1.r, AP(kcnext+k).i = z__1.i;
	    i__1 = kcnext + k - 1;
	    z__2.r = -akkp1.r, z__2.i = -akkp1.i;
	    z_div(&z__1, &z__2, &d);
	    AP(kcnext+k-1).r = z__1.r, AP(kcnext+k-1).i = z__1.i;

/*           Compute columns K and K+1 of the inverse. */

	    if (k > 1) {
		i__1 = k - 1;
		zcopy_(&i__1, &AP(kc), &c__1, &WORK(1), &c__1);
		i__1 = k - 1;
		z__1.r = -1., z__1.i = 0.;
		zspmv_(uplo, &i__1, &z__1, &AP(1), &WORK(1), &c__1, &c_b2, &
			AP(kc), &c__1);
		i__1 = kc + k - 1;
		i__2 = kc + k - 1;
		i__3 = k - 1;
		zdotu_(&z__2, &i__3, &WORK(1), &c__1, &AP(kc), &c__1);
		z__1.r = AP(kc+k-1).r - z__2.r, z__1.i = AP(kc+k-1).i - z__2.i;
		AP(kc+k-1).r = z__1.r, AP(kc+k-1).i = z__1.i;
		i__1 = kcnext + k - 1;
		i__2 = kcnext + k - 1;
		i__3 = k - 1;
		zdotu_(&z__2, &i__3, &AP(kc), &c__1, &AP(kcnext), &c__1);
		z__1.r = AP(kcnext+k-1).r - z__2.r, z__1.i = AP(kcnext+k-1).i - z__2.i;
		AP(kcnext+k-1).r = z__1.r, AP(kcnext+k-1).i = z__1.i;
		i__1 = k - 1;
		zcopy_(&i__1, &AP(kcnext), &c__1, &WORK(1), &c__1);
		i__1 = k - 1;
		z__1.r = -1., z__1.i = 0.;
		zspmv_(uplo, &i__1, &z__1, &AP(1), &WORK(1), &c__1, &c_b2, &
			AP(kcnext), &c__1);
		i__1 = kcnext + k;
		i__2 = kcnext + k;
		i__3 = k - 1;
		zdotu_(&z__2, &i__3, &WORK(1), &c__1, &AP(kcnext), &c__1);
		z__1.r = AP(kcnext+k).r - z__2.r, z__1.i = AP(kcnext+k).i - z__2.i;
		AP(kcnext+k).r = z__1.r, AP(kcnext+k).i = z__1.i;
	    }
	    kstep = 2;
	    kcnext = kcnext + k + 1;
	}

	kp = (i__1 = IPIV(k), abs(i__1));
	if (kp != k) {

/*           Interchange rows and columns K and KP in the leading 
  
             submatrix A(1:k+1,1:k+1) */

	    kpc = (kp - 1) * kp / 2 + 1;
	    i__1 = kp - 1;
	    zswap_(&i__1, &AP(kc), &c__1, &AP(kpc), &c__1);
	    kx = kpc + kp - 1;
	    i__1 = k - 1;
	    for (j = kp + 1; j <= k-1; ++j) {
		kx = kx + j - 1;
		i__2 = kc + j - 1;
		temp.r = AP(kc+j-1).r, temp.i = AP(kc+j-1).i;
		i__2 = kc + j - 1;
		i__3 = kx;
		AP(kc+j-1).r = AP(kx).r, AP(kc+j-1).i = AP(kx).i;
		i__2 = kx;
		AP(kx).r = temp.r, AP(kx).i = temp.i;
/* L40: */
	    }
	    i__1 = kc + k - 1;
	    temp.r = AP(kc+k-1).r, temp.i = AP(kc+k-1).i;
	    i__1 = kc + k - 1;
	    i__2 = kpc + kp - 1;
	    AP(kc+k-1).r = AP(kpc+kp-1).r, AP(kc+k-1).i = AP(kpc+kp-1).i;
	    i__1 = kpc + kp - 1;
	    AP(kpc+kp-1).r = temp.r, AP(kpc+kp-1).i = temp.i;
	    if (kstep == 2) {
		i__1 = kc + k + k - 1;
		temp.r = AP(kc+k+k-1).r, temp.i = AP(kc+k+k-1).i;
		i__1 = kc + k + k - 1;
		i__2 = kc + k + kp - 1;
		AP(kc+k+k-1).r = AP(kc+k+kp-1).r, AP(kc+k+k-1).i = AP(kc+k+kp-1).i;
		i__1 = kc + k + kp - 1;
		AP(kc+k+kp-1).r = temp.r, AP(kc+k+kp-1).i = temp.i;
	    }
	}

	k += kstep;
	kc = kcnext;
	goto L30;
L50:

	;
    } else {

/*        Compute inv(A) from the factorization A = L*D*L'.   

          K is the main loop index, increasing from 1 to N in steps of
   
          1 or 2, depending on the size of the diagonal blocks. */

	npp = *n * (*n + 1) / 2;
	k = *n;
	kc = npp;
L60:

/*        If K < 1, exit from loop. */

	if (k < 1) {
	    goto L80;
	}

	kcnext = kc - (*n - k + 2);
	if (IPIV(k) > 0) {

/*           1 x 1 diagonal block   

             Invert the diagonal block. */

	    i__1 = kc;
	    z_div(&z__1, &c_b1, &AP(kc));
	    AP(kc).r = z__1.r, AP(kc).i = z__1.i;

/*           Compute column K of the inverse. */

	    if (k < *n) {
		i__1 = *n - k;
		zcopy_(&i__1, &AP(kc + 1), &c__1, &WORK(1), &c__1);
		i__1 = *n - k;
		z__1.r = -1., z__1.i = 0.;
		zspmv_(uplo, &i__1, &z__1, &AP(kc + *n - k + 1), &WORK(1), &
			c__1, &c_b2, &AP(kc + 1), &c__1);
		i__1 = kc;
		i__2 = kc;
		i__3 = *n - k;
		zdotu_(&z__2, &i__3, &WORK(1), &c__1, &AP(kc + 1), &c__1);
		z__1.r = AP(kc).r - z__2.r, z__1.i = AP(kc).i - z__2.i;
		AP(kc).r = z__1.r, AP(kc).i = z__1.i;
	    }
	    kstep = 1;
	} else {

/*           2 x 2 diagonal block   

             Invert the diagonal block. */

	    i__1 = kcnext + 1;
	    t.r = AP(kcnext+1).r, t.i = AP(kcnext+1).i;
	    z_div(&z__1, &AP(kcnext), &t);
	    ak.r = z__1.r, ak.i = z__1.i;
	    z_div(&z__1, &AP(kc), &t);
	    akp1.r = z__1.r, akp1.i = z__1.i;
	    z_div(&z__1, &AP(kcnext + 1), &t);
	    akkp1.r = z__1.r, akkp1.i = z__1.i;
	    z__3.r = ak.r * akp1.r - ak.i * akp1.i, z__3.i = ak.r * akp1.i + 
		    ak.i * akp1.r;
	    z__2.r = z__3.r - 1., z__2.i = z__3.i + 0.;
	    z__1.r = t.r * z__2.r - t.i * z__2.i, z__1.i = t.r * z__2.i + t.i 
		    * z__2.r;
	    d.r = z__1.r, d.i = z__1.i;
	    i__1 = kcnext;
	    z_div(&z__1, &akp1, &d);
	    AP(kcnext).r = z__1.r, AP(kcnext).i = z__1.i;
	    i__1 = kc;
	    z_div(&z__1, &ak, &d);
	    AP(kc).r = z__1.r, AP(kc).i = z__1.i;
	    i__1 = kcnext + 1;
	    z__2.r = -akkp1.r, z__2.i = -akkp1.i;
	    z_div(&z__1, &z__2, &d);
	    AP(kcnext+1).r = z__1.r, AP(kcnext+1).i = z__1.i;

/*           Compute columns K-1 and K of the inverse. */

	    if (k < *n) {
		i__1 = *n - k;
		zcopy_(&i__1, &AP(kc + 1), &c__1, &WORK(1), &c__1);
		i__1 = *n - k;
		z__1.r = -1., z__1.i = 0.;
		zspmv_(uplo, &i__1, &z__1, &AP(kc + (*n - k + 1)), &WORK(1), &
			c__1, &c_b2, &AP(kc + 1), &c__1);
		i__1 = kc;
		i__2 = kc;
		i__3 = *n - k;
		zdotu_(&z__2, &i__3, &WORK(1), &c__1, &AP(kc + 1), &c__1);
		z__1.r = AP(kc).r - z__2.r, z__1.i = AP(kc).i - z__2.i;
		AP(kc).r = z__1.r, AP(kc).i = z__1.i;
		i__1 = kcnext + 1;
		i__2 = kcnext + 1;
		i__3 = *n - k;
		zdotu_(&z__2, &i__3, &AP(kc + 1), &c__1, &AP(kcnext + 2), &
			c__1);
		z__1.r = AP(kcnext+1).r - z__2.r, z__1.i = AP(kcnext+1).i - z__2.i;
		AP(kcnext+1).r = z__1.r, AP(kcnext+1).i = z__1.i;
		i__1 = *n - k;
		zcopy_(&i__1, &AP(kcnext + 2), &c__1, &WORK(1), &c__1);
		i__1 = *n - k;
		z__1.r = -1., z__1.i = 0.;
		zspmv_(uplo, &i__1, &z__1, &AP(kc + (*n - k + 1)), &WORK(1), &
			c__1, &c_b2, &AP(kcnext + 2), &c__1);
		i__1 = kcnext;
		i__2 = kcnext;
		i__3 = *n - k;
		zdotu_(&z__2, &i__3, &WORK(1), &c__1, &AP(kcnext + 2), &c__1);
		z__1.r = AP(kcnext).r - z__2.r, z__1.i = AP(kcnext).i - z__2.i;
		AP(kcnext).r = z__1.r, AP(kcnext).i = z__1.i;
	    }
	    kstep = 2;
	    kcnext -= *n - k + 3;
	}

	kp = (i__1 = IPIV(k), abs(i__1));
	if (kp != k) {

/*           Interchange rows and columns K and KP in the trailing
   
             submatrix A(k-1:n,k-1:n) */

	    kpc = npp - (*n - kp + 1) * (*n - kp + 2) / 2 + 1;
	    if (kp < *n) {
		i__1 = *n - kp;
		zswap_(&i__1, &AP(kc + kp - k + 1), &c__1, &AP(kpc + 1), &
			c__1);
	    }
	    kx = kc + kp - k;
	    i__1 = kp - 1;
	    for (j = k + 1; j <= kp-1; ++j) {
		kx = kx + *n - j + 1;
		i__2 = kc + j - k;
		temp.r = AP(kc+j-k).r, temp.i = AP(kc+j-k).i;
		i__2 = kc + j - k;
		i__3 = kx;
		AP(kc+j-k).r = AP(kx).r, AP(kc+j-k).i = AP(kx).i;
		i__2 = kx;
		AP(kx).r = temp.r, AP(kx).i = temp.i;
/* L70: */
	    }
	    i__1 = kc;
	    temp.r = AP(kc).r, temp.i = AP(kc).i;
	    i__1 = kc;
	    i__2 = kpc;
	    AP(kc).r = AP(kpc).r, AP(kc).i = AP(kpc).i;
	    i__1 = kpc;
	    AP(kpc).r = temp.r, AP(kpc).i = temp.i;
	    if (kstep == 2) {
		i__1 = kc - *n + k - 1;
		temp.r = AP(kc-*n+k-1).r, temp.i = AP(kc-*n+k-1).i;
		i__1 = kc - *n + k - 1;
		i__2 = kc - *n + kp - 1;
		AP(kc-*n+k-1).r = AP(kc-*n+kp-1).r, AP(kc-*n+k-1).i = AP(kc-*n+kp-1).i;
		i__1 = kc - *n + kp - 1;
		AP(kc-*n+kp-1).r = temp.r, AP(kc-*n+kp-1).i = temp.i;
	    }
	}

	k -= kstep;
	kc = kcnext;
	goto L60;
L80:
	;
    }

    return 0;

/*     End of ZSPTRI */

} /* zsptri_ */

