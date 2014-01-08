#include "f2c.h"

/* Subroutine */ int dtrevc_(char *side, char *howmny, logical *select, 
	integer *n, doublereal *t, integer *ldt, doublereal *vl, integer *
	ldvl, doublereal *vr, integer *ldvr, integer *mm, integer *m, 
	doublereal *work, integer *info)
{
/*  -- LAPACK routine (version 2.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    DTREVC computes some or all of the right and/or left eigenvectors of 
  
    a real upper quasi-triangular matrix T.   

    The right eigenvector x and the left eigenvector y of T corresponding 
  
    to an eigenvalue w are defined by:   

                 T*x = w*x,     y'*T = w*y'   

    where y' denotes the conjugate transpose of the vector y.   

    If all eigenvectors are requested, the routine may either return the 
  
    matrices X and/or Y of right or left eigenvectors of T, or the   
    products Q*X and/or Q*Y, where Q is an input orthogonal   
    matrix. If T was obtained from the real-Schur factorization of an   
    original matrix A = Q*T*Q', then Q*X and Q*Y are the matrices of   
    right or left eigenvectors of A.   

    T must be in Schur canonical form (as returned by DHSEQR), that is,   
    block upper triangular with 1-by-1 and 2-by-2 diagonal blocks; each   
    2-by-2 diagonal block has its diagonal elements equal and its   
    off-diagonal elements of opposite sign.  Corresponding to each 2-by-2 
  
    diagonal block is a complex conjugate pair of eigenvalues and   
    eigenvectors; only one eigenvector of the pair is computed, namely   
    the one corresponding to the eigenvalue with positive imaginary part. 
  


    Arguments   
    =========   

    SIDE    (input) CHARACTER*1   
            = 'R':  compute right eigenvectors only;   
            = 'L':  compute left eigenvectors only;   
            = 'B':  compute both right and left eigenvectors.   

    HOWMNY  (input) CHARACTER*1   
            = 'A':  compute all right and/or left eigenvectors;   
            = 'B':  compute all right and/or left eigenvectors,   
                    and backtransform them using the input matrices   
                    supplied in VR and/or VL;   
            = 'S':  compute selected right and/or left eigenvectors,   
                    specified by the logical array SELECT.   

    SELECT  (input/output) LOGICAL array, dimension (N)   
            If HOWMNY = 'S', SELECT specifies the eigenvectors to be   
            computed.   
            If HOWMNY = 'A' or 'B', SELECT is not referenced.   
            To select the real eigenvector corresponding to a real   
            eigenvalue w(j), SELECT(j) must be set to .TRUE..  To select 
  
            the complex eigenvector corresponding to a complex conjugate 
  
            pair w(j) and w(j+1), either SELECT(j) or SELECT(j+1) must be 
  
            set to .TRUE.; then on exit SELECT(j) is .TRUE. and   
            SELECT(j+1) is .FALSE..   

    N       (input) INTEGER   
            The order of the matrix T. N >= 0.   

    T       (input) DOUBLE PRECISION array, dimension (LDT,N)   
            The upper quasi-triangular matrix T in Schur canonical form. 
  

    LDT     (input) INTEGER   
            The leading dimension of the array T. LDT >= max(1,N).   

    VL      (input/output) DOUBLE PRECISION array, dimension (LDVL,MM)   
            On entry, if SIDE = 'L' or 'B' and HOWMNY = 'B', VL must   
            contain an N-by-N matrix Q (usually the orthogonal matrix Q   
            of Schur vectors returned by DHSEQR).   
            On exit, if SIDE = 'L' or 'B', VL contains:   
            if HOWMNY = 'A', the matrix Y of left eigenvectors of T;   
            if HOWMNY = 'B', the matrix Q*Y;   
            if HOWMNY = 'S', the left eigenvectors of T specified by   
                             SELECT, stored consecutively in the columns 
  
                             of VL, in the same order as their   
                             eigenvalues.   
            A complex eigenvector corresponding to a complex eigenvalue   
            is stored in two consecutive columns, the first holding the   
            real part, and the second the imaginary part.   
            If SIDE = 'R', VL is not referenced.   

    LDVL    (input) INTEGER   
            The leading dimension of the array VL.  LDVL >= max(1,N) if   
            SIDE = 'L' or 'B'; LDVL >= 1 otherwise.   

    VR      (input/output) DOUBLE PRECISION array, dimension (LDVR,MM)   
            On entry, if SIDE = 'R' or 'B' and HOWMNY = 'B', VR must   
            contain an N-by-N matrix Q (usually the orthogonal matrix Q   
            of Schur vectors returned by DHSEQR).   
            On exit, if SIDE = 'R' or 'B', VR contains:   
            if HOWMNY = 'A', the matrix X of right eigenvectors of T;   
            if HOWMNY = 'B', the matrix Q*X;   
            if HOWMNY = 'S', the right eigenvectors of T specified by   
                             SELECT, stored consecutively in the columns 
  
                             of VR, in the same order as their   
                             eigenvalues.   
            A complex eigenvector corresponding to a complex eigenvalue   
            is stored in two consecutive columns, the first holding the   
            real part and the second the imaginary part.   
            If SIDE = 'L', VR is not referenced.   

    LDVR    (input) INTEGER   
            The leading dimension of the array VR.  LDVR >= max(1,N) if   
            SIDE = 'R' or 'B'; LDVR >= 1 otherwise.   

    MM      (input) INTEGER   
            The number of columns in the arrays VL and/or VR. MM >= M.   

    M       (output) INTEGER   
            The number of columns in the arrays VL and/or VR actually   
            used to store the eigenvectors.   
            If HOWMNY = 'A' or 'B', M is set to N.   
            Each selected real eigenvector occupies one column and each   
            selected complex eigenvector occupies two columns.   

    WORK    (workspace) DOUBLE PRECISION array, dimension (3*N)   

    INFO    (output) INTEGER   
            = 0:  successful exit   
            < 0:  if INFO = -i, the i-th argument had an illegal value   

    Further Details   
    ===============   

    The algorithm used in this program is basically backward (forward)   
    substitution, with scaling to make the the code robust against   
    possible overflow.   

    Each eigenvector is normalized so that the element of largest   
    magnitude has magnitude 1; here the magnitude of a complex number   
    (x,y) is taken to be |x| + |y|.   

    ===================================================================== 
  


       Decode and test the input parameters   

    
   Parameter adjustments   
       Function Body */
    /* Table of constant values */
    static logical c_false = FALSE_;
    static integer c__1 = 1;
    static doublereal c_b23 = 1.;
    static doublereal c_b26 = 0.;
    static integer c__2 = 2;
    static logical c_true = TRUE_;
    
    /* System generated locals */
    integer t_dim1, t_offset, vl_dim1, vl_offset, vr_dim1, vr_offset, i__1, 
	    i__2, i__3;
    doublereal d__1, d__2, d__3, d__4;
    /* Builtin functions */
    double sqrt(doublereal);
    /* Local variables */
    static doublereal beta, emax;
    static logical pair;
    extern doublereal ddot_(integer *, doublereal *, integer *, doublereal *, 
	    integer *);
    static logical allv;
    static integer ierr;
    static doublereal unfl, ovfl, smin;
    static logical over;
    static doublereal vmax;
    static integer jnxt, i, j, k;
    extern /* Subroutine */ int dscal_(integer *, doublereal *, doublereal *, 
	    integer *);
    static doublereal scale, x[4]	/* was [2][2] */;
    extern logical lsame_(char *, char *);
    extern /* Subroutine */ int dgemv_(char *, integer *, integer *, 
	    doublereal *, doublereal *, integer *, doublereal *, integer *, 
	    doublereal *, doublereal *, integer *);
    static doublereal remax;
    extern /* Subroutine */ int dcopy_(integer *, doublereal *, integer *, 
	    doublereal *, integer *);
    static logical leftv, bothv;
    extern /* Subroutine */ int daxpy_(integer *, doublereal *, doublereal *, 
	    integer *, doublereal *, integer *);
    static doublereal vcrit;
    static logical somev;
    static integer j1, j2, n2;
    static doublereal xnorm;
    extern /* Subroutine */ int dlaln2_(logical *, integer *, integer *, 
	    doublereal *, doublereal *, doublereal *, integer *, doublereal *,
	     doublereal *, doublereal *, integer *, doublereal *, doublereal *
	    , doublereal *, integer *, doublereal *, doublereal *, integer *),
	     dlabad_(doublereal *, doublereal *);
    static integer ii, ki;
    extern doublereal dlamch_(char *);
    static integer ip, is;
    static doublereal wi;
    extern integer idamax_(integer *, doublereal *, integer *);
    static doublereal wr;
    extern /* Subroutine */ int xerbla_(char *, integer *);
    static doublereal bignum;
    static logical rightv;
    static doublereal smlnum, rec, ulp;



#define SELECT(I) select[(I)-1]
#define WORK(I) work[(I)-1]

#define T(I,J) t[(I)-1 + ((J)-1)* ( *ldt)]
#define VL(I,J) vl[(I)-1 + ((J)-1)* ( *ldvl)]
#define VR(I,J) vr[(I)-1 + ((J)-1)* ( *ldvr)]

    bothv = lsame_(side, "B");
    rightv = lsame_(side, "R") || bothv;
    leftv = lsame_(side, "L") || bothv;

    allv = lsame_(howmny, "A");
    over = lsame_(howmny, "B") || lsame_(howmny, "O");
    somev = lsame_(howmny, "S");

    *info = 0;
    if (! rightv && ! leftv) {
	*info = -1;
    } else if (! allv && ! over && ! somev) {
	*info = -2;
    } else if (*n < 0) {
	*info = -4;
    } else if (*ldt < max(1,*n)) {
	*info = -6;
    } else if (*ldvl < 1 || leftv && *ldvl < *n) {
	*info = -8;
    } else if (*ldvr < 1 || rightv && *ldvr < *n) {
	*info = -10;
    } else {

/*        Set M to the number of columns required to store the selecte
d   
          eigenvectors, standardize the array SELECT if necessary, and
   
          test MM. */

	if (somev) {
	    *m = 0;
	    pair = FALSE_;
	    i__1 = *n;
	    for (j = 1; j <= *n; ++j) {
		if (pair) {
		    pair = FALSE_;
		    SELECT(j) = FALSE_;
		} else {
		    if (j < *n) {
			if (T(j+1,j) == 0.) {
			    if (SELECT(j)) {
				++(*m);
			    }
			} else {
			    pair = TRUE_;
			    if (SELECT(j) || SELECT(j + 1)) {
				SELECT(j) = TRUE_;
				*m += 2;
			    }
			}
		    } else {
			if (SELECT(*n)) {
			    ++(*m);
			}
		    }
		}
/* L10: */
	    }
	} else {
	    *m = *n;
	}

	if (*mm < *m) {
	    *info = -11;
	}
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("DTREVC", &i__1);
	return 0;
    }

/*     Quick return if possible. */

    if (*n == 0) {
	return 0;
    }

/*     Set the constants to control overflow. */

    unfl = dlamch_("Safe minimum");
    ovfl = 1. / unfl;
    dlabad_(&unfl, &ovfl);
    ulp = dlamch_("Precision");
    smlnum = unfl * (*n / ulp);
    bignum = (1. - ulp) / smlnum;

/*     Compute 1-norm of each column of strictly upper triangular   
       part of T to control overflow in triangular solver. */

    WORK(1) = 0.;
    i__1 = *n;
    for (j = 2; j <= *n; ++j) {
	WORK(j) = 0.;
	i__2 = j - 1;
	for (i = 1; i <= j-1; ++i) {
	    WORK(j) += (d__1 = T(i,j), abs(d__1));
/* L20: */
	}
/* L30: */
    }

/*     Index IP is used to specify the real or complex eigenvalue:   
         IP = 0, real eigenvalue,   
              1, first of conjugate complex pair: (wr,wi)   
             -1, second of conjugate complex pair: (wr,wi) */

    n2 = *n << 1;

    if (rightv) {

/*        Compute right eigenvectors. */

	ip = 0;
	is = *m;
	for (ki = *n; ki >= 1; --ki) {

	    if (ip == 1) {
		goto L130;
	    }
	    if (ki == 1) {
		goto L40;
	    }
	    if (T(ki,ki-1) == 0.) {
		goto L40;
	    }
	    ip = -1;

L40:
	    if (somev) {
		if (ip == 0) {
		    if (! SELECT(ki)) {
			goto L130;
		    }
		} else {
		    if (! SELECT(ki - 1)) {
			goto L130;
		    }
		}
	    }

/*           Compute the KI-th eigenvalue (WR,WI). */

	    wr = T(ki,ki);
	    wi = 0.;
	    if (ip != 0) {
		wi = sqrt((d__1 = T(ki,ki-1), abs(d__1))) * 
			sqrt((d__2 = T(ki-1,ki), abs(d__2)));
	    }
/* Computing MAX */
	    d__1 = ulp * (abs(wr) + abs(wi));
	    smin = max(d__1,smlnum);

	    if (ip == 0) {

/*              Real right eigenvector */

		WORK(ki + *n) = 1.;

/*              Form right-hand side */

		i__1 = ki - 1;
		for (k = 1; k <= ki-1; ++k) {
		    WORK(k + *n) = -T(k,ki);
/* L50: */
		}

/*              Solve the upper quasi-triangular system:   
                   (T(1:KI-1,1:KI-1) - WR)*X = SCALE*WORK. */

		jnxt = ki - 1;
		for (j = ki - 1; j >= 1; --j) {
		    if (j > jnxt) {
			goto L60;
		    }
		    j1 = j;
		    j2 = j;
		    jnxt = j - 1;
		    if (j > 1) {
			if (T(j,j-1) != 0.) {
			    j1 = j - 1;
			    jnxt = j - 2;
			}
		    }

		    if (j1 == j2) {

/*                    1-by-1 diagonal block */

			dlaln2_(&c_false, &c__1, &c__1, &smin, &c_b23, &T(j,j), ldt, &c_b23, &c_b23, &WORK(j + *
				n), n, &wr, &c_b26, x, &c__2, &scale, &xnorm, 
				&ierr);

/*                    Scale X(1,1) to avoid overflow w
hen updating   
                      the right-hand side. */

			if (xnorm > 1.) {
			    if (WORK(j) > bignum / xnorm) {
				x[0] /= xnorm;
				scale /= xnorm;
			    }
			}

/*                    Scale if necessary */

			if (scale != 1.) {
			    dscal_(&ki, &scale, &WORK(*n + 1), &c__1);
			}
			WORK(j + *n) = x[0];

/*                    Update right-hand side */

			i__1 = j - 1;
			d__1 = -x[0];
			daxpy_(&i__1, &d__1, &T(1,j), &c__1, &WORK(
				*n + 1), &c__1);

		    } else {

/*                    2-by-2 diagonal block */

			dlaln2_(&c_false, &c__2, &c__1, &smin, &c_b23, &T(j-1,j-1), ldt, &c_b23, &c_b23, &
				WORK(j - 1 + *n), n, &wr, &c_b26, x, &c__2, &
				scale, &xnorm, &ierr);

/*                    Scale X(1,1) and X(2,1) to avoid
 overflow when   
                      updating the right-hand side. */

			if (xnorm > 1.) {
/* Computing MAX */
			    d__1 = WORK(j - 1), d__2 = WORK(j);
			    beta = max(d__1,d__2);
			    if (beta > bignum / xnorm) {
				x[0] /= xnorm;
				x[1] /= xnorm;
				scale /= xnorm;
			    }
			}

/*                    Scale if necessary */

			if (scale != 1.) {
			    dscal_(&ki, &scale, &WORK(*n + 1), &c__1);
			}
			WORK(j - 1 + *n) = x[0];
			WORK(j + *n) = x[1];

/*                    Update right-hand side */

			i__1 = j - 2;
			d__1 = -x[0];
			daxpy_(&i__1, &d__1, &T(1,j-1), &c__1, 
				&WORK(*n + 1), &c__1);
			i__1 = j - 2;
			d__1 = -x[1];
			daxpy_(&i__1, &d__1, &T(1,j), &c__1, &WORK(
				*n + 1), &c__1);
		    }
L60:
		    ;
		}

/*              Copy the vector x or Q*x to VR and normalize. 
*/

		if (! over) {
		    dcopy_(&ki, &WORK(*n + 1), &c__1, &VR(1,is), &
			    c__1);

		    ii = idamax_(&ki, &VR(1,is), &c__1);
		    remax = 1. / (d__1 = VR(ii,is), abs(d__1));
		    dscal_(&ki, &remax, &VR(1,is), &c__1);

		    i__1 = *n;
		    for (k = ki + 1; k <= *n; ++k) {
			VR(k,is) = 0.;
/* L70: */
		    }
		} else {
		    if (ki > 1) {
			i__1 = ki - 1;
			dgemv_("N", n, &i__1, &c_b23, &VR(1,1), ldvr, &
				WORK(*n + 1), &c__1, &WORK(ki + *n), &VR(1,ki), &c__1);
		    }

		    ii = idamax_(n, &VR(1,ki), &c__1);
		    remax = 1. / (d__1 = VR(ii,ki), abs(d__1));
		    dscal_(n, &remax, &VR(1,ki), &c__1);
		}

	    } else {

/*              Complex right eigenvector.   

                Initial solve   
                  [ (T(KI-1,KI-1) T(KI-1,KI) ) - (WR + I* WI)]
*X = 0.   
                  [ (T(KI,KI-1)   T(KI,KI)   )               ]
 */

		if ((d__1 = T(ki-1,ki), abs(d__1)) >= (d__2 = T(ki,ki-1), abs(d__2))) {
		    WORK(ki - 1 + *n) = 1.;
		    WORK(ki + n2) = wi / T(ki-1,ki);
		} else {
		    WORK(ki - 1 + *n) = -wi / T(ki,ki-1);
		    WORK(ki + n2) = 1.;
		}
		WORK(ki + *n) = 0.;
		WORK(ki - 1 + n2) = 0.;

/*              Form right-hand side */

		i__1 = ki - 2;
		for (k = 1; k <= ki-2; ++k) {
		    WORK(k + *n) = -WORK(ki - 1 + *n) * T(k,ki-1);
		    WORK(k + n2) = -WORK(ki + n2) * T(k,ki);
/* L80: */
		}

/*              Solve upper quasi-triangular system:   
                (T(1:KI-2,1:KI-2) - (WR+i*WI))*X = SCALE*(WORK
+i*WORK2) */

		jnxt = ki - 2;
		for (j = ki - 2; j >= 1; --j) {
		    if (j > jnxt) {
			goto L90;
		    }
		    j1 = j;
		    j2 = j;
		    jnxt = j - 1;
		    if (j > 1) {
			if (T(j,j-1) != 0.) {
			    j1 = j - 1;
			    jnxt = j - 2;
			}
		    }

		    if (j1 == j2) {

/*                    1-by-1 diagonal block */

			dlaln2_(&c_false, &c__1, &c__2, &smin, &c_b23, &T(j,j), ldt, &c_b23, &c_b23, &WORK(j + *
				n), n, &wr, &wi, x, &c__2, &scale, &xnorm, &
				ierr);

/*                    Scale X(1,1) and X(1,2) to avoid
 overflow when   
                      updating the right-hand side. */

			if (xnorm > 1.) {
			    if (WORK(j) > bignum / xnorm) {
				x[0] /= xnorm;
				x[2] /= xnorm;
				scale /= xnorm;
			    }
			}

/*                    Scale if necessary */

			if (scale != 1.) {
			    dscal_(&ki, &scale, &WORK(*n + 1), &c__1);
			    dscal_(&ki, &scale, &WORK(n2 + 1), &c__1);
			}
			WORK(j + *n) = x[0];
			WORK(j + n2) = x[2];

/*                    Update the right-hand side */

			i__1 = j - 1;
			d__1 = -x[0];
			daxpy_(&i__1, &d__1, &T(1,j), &c__1, &WORK(
				*n + 1), &c__1);
			i__1 = j - 1;
			d__1 = -x[2];
			daxpy_(&i__1, &d__1, &T(1,j), &c__1, &WORK(
				n2 + 1), &c__1);

		    } else {

/*                    2-by-2 diagonal block */

			dlaln2_(&c_false, &c__2, &c__2, &smin, &c_b23, &T(j-1,j-1), ldt, &c_b23, &c_b23, &
				WORK(j - 1 + *n), n, &wr, &wi, x, &c__2, &
				scale, &xnorm, &ierr);

/*                    Scale X to avoid overflow when u
pdating   
                      the right-hand side. */

			if (xnorm > 1.) {
/* Computing MAX */
			    d__1 = WORK(j - 1), d__2 = WORK(j);
			    beta = max(d__1,d__2);
			    if (beta > bignum / xnorm) {
				rec = 1. / xnorm;
				x[0] *= rec;
				x[2] *= rec;
				x[1] *= rec;
				x[3] *= rec;
				scale *= rec;
			    }
			}

/*                    Scale if necessary */

			if (scale != 1.) {
			    dscal_(&ki, &scale, &WORK(*n + 1), &c__1);
			    dscal_(&ki, &scale, &WORK(n2 + 1), &c__1);
			}
			WORK(j - 1 + *n) = x[0];
			WORK(j + *n) = x[1];
			WORK(j - 1 + n2) = x[2];
			WORK(j + n2) = x[3];

/*                    Update the right-hand side */

			i__1 = j - 2;
			d__1 = -x[0];
			daxpy_(&i__1, &d__1, &T(1,j-1), &c__1, 
				&WORK(*n + 1), &c__1);
			i__1 = j - 2;
			d__1 = -x[1];
			daxpy_(&i__1, &d__1, &T(1,j), &c__1, &WORK(
				*n + 1), &c__1);
			i__1 = j - 2;
			d__1 = -x[2];
			daxpy_(&i__1, &d__1, &T(1,j-1), &c__1, 
				&WORK(n2 + 1), &c__1);
			i__1 = j - 2;
			d__1 = -x[3];
			daxpy_(&i__1, &d__1, &T(1,j), &c__1, &WORK(
				n2 + 1), &c__1);
		    }
L90:
		    ;
		}

/*              Copy the vector x or Q*x to VR and normalize. 
*/

		if (! over) {
		    dcopy_(&ki, &WORK(*n + 1), &c__1, &VR(1,is-1), &c__1);
		    dcopy_(&ki, &WORK(n2 + 1), &c__1, &VR(1,is), &
			    c__1);

		    emax = 0.;
		    i__1 = ki;
		    for (k = 1; k <= ki; ++k) {
/* Computing MAX */
			d__3 = emax, d__4 = (d__1 = VR(k,is-1)
				, abs(d__1)) + (d__2 = VR(k,is), 
				abs(d__2));
			emax = max(d__3,d__4);
/* L100: */
		    }

		    remax = 1. / emax;
		    dscal_(&ki, &remax, &VR(1,is-1), &c__1);
		    dscal_(&ki, &remax, &VR(1,is), &c__1);

		    i__1 = *n;
		    for (k = ki + 1; k <= *n; ++k) {
			VR(k,is-1) = 0.;
			VR(k,is) = 0.;
/* L110: */
		    }

		} else {

		    if (ki > 2) {
			i__1 = ki - 2;
			dgemv_("N", n, &i__1, &c_b23, &VR(1,1), ldvr, &
				WORK(*n + 1), &c__1, &WORK(ki - 1 + *n), &VR(1,ki-1), &c__1);
			i__1 = ki - 2;
			dgemv_("N", n, &i__1, &c_b23, &VR(1,1), ldvr, &
				WORK(n2 + 1), &c__1, &WORK(ki + n2), &VR(1,ki), &c__1);
		    } else {
			dscal_(n, &WORK(ki - 1 + *n), &VR(1,ki-1), &c__1);
			dscal_(n, &WORK(ki + n2), &VR(1,ki), &
				c__1);
		    }

		    emax = 0.;
		    i__1 = *n;
		    for (k = 1; k <= *n; ++k) {
/* Computing MAX */
			d__3 = emax, d__4 = (d__1 = VR(k,ki-1)
				, abs(d__1)) + (d__2 = VR(k,ki), 
				abs(d__2));
			emax = max(d__3,d__4);
/* L120: */
		    }
		    remax = 1. / emax;
		    dscal_(n, &remax, &VR(1,ki-1), &c__1);
		    dscal_(n, &remax, &VR(1,ki), &c__1);
		}
	    }

	    --is;
	    if (ip != 0) {
		--is;
	    }
L130:
	    if (ip == 1) {
		ip = 0;
	    }
	    if (ip == -1) {
		ip = 1;
	    }
/* L140: */
	}
    }

    if (leftv) {

/*        Compute left eigenvectors. */

	ip = 0;
	is = 1;
	i__1 = *n;
	for (ki = 1; ki <= *n; ++ki) {

	    if (ip == -1) {
		goto L250;
	    }
	    if (ki == *n) {
		goto L150;
	    }
	    if (T(ki+1,ki) == 0.) {
		goto L150;
	    }
	    ip = 1;

L150:
	    if (somev) {
		if (! SELECT(ki)) {
		    goto L250;
		}
	    }

/*           Compute the KI-th eigenvalue (WR,WI). */

	    wr = T(ki,ki);
	    wi = 0.;
	    if (ip != 0) {
		wi = sqrt((d__1 = T(ki,ki+1), abs(d__1))) * 
			sqrt((d__2 = T(ki+1,ki), abs(d__2)));
	    }
/* Computing MAX */
	    d__1 = ulp * (abs(wr) + abs(wi));
	    smin = max(d__1,smlnum);

	    if (ip == 0) {

/*              Real left eigenvector. */

		WORK(ki + *n) = 1.;

/*              Form right-hand side */

		i__2 = *n;
		for (k = ki + 1; k <= *n; ++k) {
		    WORK(k + *n) = -T(ki,k);
/* L160: */
		}

/*              Solve the quasi-triangular system:   
                   (T(KI+1:N,KI+1:N) - WR)'*X = SCALE*WORK */

		vmax = 1.;
		vcrit = bignum;

		jnxt = ki + 1;
		i__2 = *n;
		for (j = ki + 1; j <= *n; ++j) {
		    if (j < jnxt) {
			goto L170;
		    }
		    j1 = j;
		    j2 = j;
		    jnxt = j + 1;
		    if (j < *n) {
			if (T(j+1,j) != 0.) {
			    j2 = j + 1;
			    jnxt = j + 2;
			}
		    }

		    if (j1 == j2) {

/*                    1-by-1 diagonal block   

                      Scale if necessary to avoid over
flow when forming   
                      the right-hand side. */

			if (WORK(j) > vcrit) {
			    rec = 1. / vmax;
			    i__3 = *n - ki + 1;
			    dscal_(&i__3, &rec, &WORK(ki + *n), &c__1);
			    vmax = 1.;
			    vcrit = bignum;
			}

			i__3 = j - ki - 1;
			WORK(j + *n) -= ddot_(&i__3, &T(ki+1,j), 
				&c__1, &WORK(ki + 1 + *n), &c__1);

/*                    Solve (T(J,J)-WR)'*X = WORK */

			dlaln2_(&c_false, &c__1, &c__1, &smin, &c_b23, &T(j,j), ldt, &c_b23, &c_b23, &WORK(j + *
				n), n, &wr, &c_b26, x, &c__2, &scale, &xnorm, 
				&ierr);

/*                    Scale if necessary */

			if (scale != 1.) {
			    i__3 = *n - ki + 1;
			    dscal_(&i__3, &scale, &WORK(ki + *n), &c__1);
			}
			WORK(j + *n) = x[0];
/* Computing MAX */
			d__2 = (d__1 = WORK(j + *n), abs(d__1));
			vmax = max(d__2,vmax);
			vcrit = bignum / vmax;

		    } else {

/*                    2-by-2 diagonal block   

                      Scale if necessary to avoid over
flow when forming   
                      the right-hand side.   

   Computing MAX */
			d__1 = WORK(j), d__2 = WORK(j + 1);
			beta = max(d__1,d__2);
			if (beta > vcrit) {
			    rec = 1. / vmax;
			    i__3 = *n - ki + 1;
			    dscal_(&i__3, &rec, &WORK(ki + *n), &c__1);
			    vmax = 1.;
			    vcrit = bignum;
			}

			i__3 = j - ki - 1;
			WORK(j + *n) -= ddot_(&i__3, &T(ki+1,j), 
				&c__1, &WORK(ki + 1 + *n), &c__1);

			i__3 = j - ki - 1;
			WORK(j + 1 + *n) -= ddot_(&i__3, &T(ki+1,j+1), &c__1, &WORK(ki + 1 + *n), &c__1);

/*                    Solve   
                        [T(J,J)-WR   T(J,J+1)     ]'* 
X = SCALE*( WORK1 )   
                        [T(J+1,J)    T(J+1,J+1)-WR]   
          ( WORK2 ) */

			dlaln2_(&c_true, &c__2, &c__1, &smin, &c_b23, &T(j,j), ldt, &c_b23, &c_b23, &WORK(j + *
				n), n, &wr, &c_b26, x, &c__2, &scale, &xnorm, 
				&ierr);

/*                    Scale if necessary */

			if (scale != 1.) {
			    i__3 = *n - ki + 1;
			    dscal_(&i__3, &scale, &WORK(ki + *n), &c__1);
			}
			WORK(j + *n) = x[0];
			WORK(j + 1 + *n) = x[1];

/* Computing MAX */
			d__3 = (d__1 = WORK(j + *n), abs(d__1)), d__4 = (d__2 
				= WORK(j + 1 + *n), abs(d__2)), d__3 = max(
				d__3,d__4);
			vmax = max(d__3,vmax);
			vcrit = bignum / vmax;

		    }
L170:
		    ;
		}

/*              Copy the vector x or Q*x to VL and normalize. 
*/

		if (! over) {
		    i__2 = *n - ki + 1;
		    dcopy_(&i__2, &WORK(ki + *n), &c__1, &VL(ki,is), &c__1);

		    i__2 = *n - ki + 1;
		    ii = idamax_(&i__2, &VL(ki,is), &c__1) + ki - 
			    1;
		    remax = 1. / (d__1 = VL(ii,is), abs(d__1));
		    i__2 = *n - ki + 1;
		    dscal_(&i__2, &remax, &VL(ki,is), &c__1);

		    i__2 = ki - 1;
		    for (k = 1; k <= ki-1; ++k) {
			VL(k,is) = 0.;
/* L180: */
		    }

		} else {

		    if (ki < *n) {
			i__2 = *n - ki;
			dgemv_("N", n, &i__2, &c_b23, &VL(1,ki+1), ldvl, &WORK(ki + 1 + *n), &c__1, &WORK(
				ki + *n), &VL(1,ki), &c__1);
		    }

		    ii = idamax_(n, &VL(1,ki), &c__1);
		    remax = 1. / (d__1 = VL(ii,ki), abs(d__1));
		    dscal_(n, &remax, &VL(1,ki), &c__1);

		}

	    } else {

/*              Complex left eigenvector.   

                 Initial solve:   
                   ((T(KI,KI)    T(KI,KI+1) )' - (WR - I* WI))
*X = 0.   
                   ((T(KI+1,KI) T(KI+1,KI+1))                )
 */

		if ((d__1 = T(ki,ki+1), abs(d__1)) >= (d__2 = 
			T(ki+1,ki), abs(d__2))) {
		    WORK(ki + *n) = wi / T(ki,ki+1);
		    WORK(ki + 1 + n2) = 1.;
		} else {
		    WORK(ki + *n) = 1.;
		    WORK(ki + 1 + n2) = -wi / T(ki+1,ki);
		}
		WORK(ki + 1 + *n) = 0.;
		WORK(ki + n2) = 0.;

/*              Form right-hand side */

		i__2 = *n;
		for (k = ki + 2; k <= *n; ++k) {
		    WORK(k + *n) = -WORK(ki + *n) * T(ki,k);
		    WORK(k + n2) = -WORK(ki + 1 + n2) * T(ki+1,k)
			    ;
/* L190: */
		}

/*              Solve complex quasi-triangular system:   
                ( T(KI+2,N:KI+2,N) - (WR-i*WI) )*X = WORK1+i*W
ORK2 */

		vmax = 1.;
		vcrit = bignum;

		jnxt = ki + 2;
		i__2 = *n;
		for (j = ki + 2; j <= *n; ++j) {
		    if (j < jnxt) {
			goto L200;
		    }
		    j1 = j;
		    j2 = j;
		    jnxt = j + 1;
		    if (j < *n) {
			if (T(j+1,j) != 0.) {
			    j2 = j + 1;
			    jnxt = j + 2;
			}
		    }

		    if (j1 == j2) {

/*                    1-by-1 diagonal block   

                      Scale if necessary to avoid over
flow when   
                      forming the right-hand side elem
ents. */

			if (WORK(j) > vcrit) {
			    rec = 1. / vmax;
			    i__3 = *n - ki + 1;
			    dscal_(&i__3, &rec, &WORK(ki + *n), &c__1);
			    i__3 = *n - ki + 1;
			    dscal_(&i__3, &rec, &WORK(ki + n2), &c__1);
			    vmax = 1.;
			    vcrit = bignum;
			}

			i__3 = j - ki - 2;
			WORK(j + *n) -= ddot_(&i__3, &T(ki+2,j), 
				&c__1, &WORK(ki + 2 + *n), &c__1);
			i__3 = j - ki - 2;
			WORK(j + n2) -= ddot_(&i__3, &T(ki+2,j), 
				&c__1, &WORK(ki + 2 + n2), &c__1);

/*                    Solve (T(J,J)-(WR-i*WI))*(X11+i*
X12)= WK+I*WK2 */

			d__1 = -wi;
			dlaln2_(&c_false, &c__1, &c__2, &smin, &c_b23, &T(j,j), ldt, &c_b23, &c_b23, &WORK(j + *
				n), n, &wr, &d__1, x, &c__2, &scale, &xnorm, &
				ierr);

/*                    Scale if necessary */

			if (scale != 1.) {
			    i__3 = *n - ki + 1;
			    dscal_(&i__3, &scale, &WORK(ki + *n), &c__1);
			    i__3 = *n - ki + 1;
			    dscal_(&i__3, &scale, &WORK(ki + n2), &c__1);
			}
			WORK(j + *n) = x[0];
			WORK(j + n2) = x[2];
/* Computing MAX */
			d__3 = (d__1 = WORK(j + *n), abs(d__1)), d__4 = (d__2 
				= WORK(j + n2), abs(d__2)), d__3 = max(d__3,
				d__4);
			vmax = max(d__3,vmax);
			vcrit = bignum / vmax;

		    } else {

/*                    2-by-2 diagonal block   

                      Scale if necessary to avoid over
flow when forming   
                      the right-hand side elements.   

   Computing MAX */
			d__1 = WORK(j), d__2 = WORK(j + 1);
			beta = max(d__1,d__2);
			if (beta > vcrit) {
			    rec = 1. / vmax;
			    i__3 = *n - ki + 1;
			    dscal_(&i__3, &rec, &WORK(ki + *n), &c__1);
			    i__3 = *n - ki + 1;
			    dscal_(&i__3, &rec, &WORK(ki + n2), &c__1);
			    vmax = 1.;
			    vcrit = bignum;
			}

			i__3 = j - ki - 2;
			WORK(j + *n) -= ddot_(&i__3, &T(ki+2,j), 
				&c__1, &WORK(ki + 2 + *n), &c__1);

			i__3 = j - ki - 2;
			WORK(j + n2) -= ddot_(&i__3, &T(ki+2,j), 
				&c__1, &WORK(ki + 2 + n2), &c__1);

			i__3 = j - ki - 2;
			WORK(j + 1 + *n) -= ddot_(&i__3, &T(ki+2,j+1), &c__1, &WORK(ki + 2 + *n), &c__1);

			i__3 = j - ki - 2;
			WORK(j + 1 + n2) -= ddot_(&i__3, &T(ki+2,j+1), &c__1, &WORK(ki + 2 + n2), &c__1);

/*                    Solve 2-by-2 complex linear equa
tion   
                        ([T(j,j)   T(j,j+1)  ]'-(wr-i*
wi)*I)*X = SCALE*B   
                        ([T(j+1,j) T(j+1,j+1)]        
     ) */

			d__1 = -wi;
			dlaln2_(&c_true, &c__2, &c__2, &smin, &c_b23, &T(j,j), ldt, &c_b23, &c_b23, &WORK(j + *
				n), n, &wr, &d__1, x, &c__2, &scale, &xnorm, &
				ierr);

/*                    Scale if necessary */

			if (scale != 1.) {
			    i__3 = *n - ki + 1;
			    dscal_(&i__3, &scale, &WORK(ki + *n), &c__1);
			    i__3 = *n - ki + 1;
			    dscal_(&i__3, &scale, &WORK(ki + n2), &c__1);
			}
			WORK(j + *n) = x[0];
			WORK(j + n2) = x[2];
			WORK(j + 1 + *n) = x[1];
			WORK(j + 1 + n2) = x[3];
/* Computing MAX */
			d__1 = abs(x[0]), d__2 = abs(x[2]), d__1 = max(d__1,
				d__2), d__2 = abs(x[1]), d__1 = max(d__1,d__2)
				, d__2 = abs(x[3]), d__1 = max(d__1,d__2);
			vmax = max(d__1,vmax);
			vcrit = bignum / vmax;

		    }
L200:
		    ;
		}

/*              Copy the vector x or Q*x to VL and normalize. 
  

   L210: */
		if (! over) {
		    i__2 = *n - ki + 1;
		    dcopy_(&i__2, &WORK(ki + *n), &c__1, &VL(ki,is), &c__1);
		    i__2 = *n - ki + 1;
		    dcopy_(&i__2, &WORK(ki + n2), &c__1, &VL(ki,is+1), &c__1);

		    emax = 0.;
		    i__2 = *n;
		    for (k = ki; k <= *n; ++k) {
/* Computing MAX */
			d__3 = emax, d__4 = (d__1 = VL(k,is), abs(
				d__1)) + (d__2 = VL(k,is+1), 
				abs(d__2));
			emax = max(d__3,d__4);
/* L220: */
		    }
		    remax = 1. / emax;
		    i__2 = *n - ki + 1;
		    dscal_(&i__2, &remax, &VL(ki,is), &c__1);
		    i__2 = *n - ki + 1;
		    dscal_(&i__2, &remax, &VL(ki,is+1), &c__1)
			    ;

		    i__2 = ki - 1;
		    for (k = 1; k <= ki-1; ++k) {
			VL(k,is) = 0.;
			VL(k,is+1) = 0.;
/* L230: */
		    }
		} else {
		    if (ki < *n - 1) {
			i__2 = *n - ki - 1;
			dgemv_("N", n, &i__2, &c_b23, &VL(1,ki+2), ldvl, &WORK(ki + 2 + *n), &c__1, &WORK(
				ki + *n), &VL(1,ki), &c__1);
			i__2 = *n - ki - 1;
			dgemv_("N", n, &i__2, &c_b23, &VL(1,ki+2), ldvl, &WORK(ki + 2 + n2), &c__1, &WORK(
				ki + 1 + n2), &VL(1,ki+1), &
				c__1);
		    } else {
			dscal_(n, &WORK(ki + *n), &VL(1,ki), &
				c__1);
			dscal_(n, &WORK(ki + 1 + n2), &VL(1,ki+1), &c__1);
		    }

		    emax = 0.;
		    i__2 = *n;
		    for (k = 1; k <= *n; ++k) {
/* Computing MAX */
			d__3 = emax, d__4 = (d__1 = VL(k,ki), abs(
				d__1)) + (d__2 = VL(k,ki+1), 
				abs(d__2));
			emax = max(d__3,d__4);
/* L240: */
		    }
		    remax = 1. / emax;
		    dscal_(n, &remax, &VL(1,ki), &c__1);
		    dscal_(n, &remax, &VL(1,ki+1), &c__1);

		}

	    }

	    ++is;
	    if (ip != 0) {
		++is;
	    }
L250:
	    if (ip == -1) {
		ip = 0;
	    }
	    if (ip == 1) {
		ip = -1;
	    }

/* L260: */
	}

    }

    return 0;

/*     End of DTREVC */

} /* dtrevc_ */

