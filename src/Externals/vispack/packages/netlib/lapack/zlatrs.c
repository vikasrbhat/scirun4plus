#include "f2c.h"

/* Subroutine */ int zlatrs_(char *uplo, char *trans, char *diag, char *
	normin, integer *n, doublecomplex *a, integer *lda, doublecomplex *x, 
	doublereal *scale, doublereal *cnorm, integer *info)
{
/*  -- LAPACK auxiliary routine (version 2.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       June 30, 1992   


    Purpose   
    =======   

    ZLATRS solves one of the triangular systems   

       A * x = s*b,  A**T * x = s*b,  or  A**H * x = s*b,   

    with scaling to prevent overflow.  Here A is an upper or lower   
    triangular matrix, A**T denotes the transpose of A, A**H denotes the 
  
    conjugate transpose of A, x and b are n-element vectors, and s is a   
    scaling factor, usually less than or equal to 1, chosen so that the   
    components of x will be less than the overflow threshold.  If the   
    unscaled problem will not cause overflow, the Level 2 BLAS routine   
    ZTRSV is called. If the matrix A is singular (A(j,j) = 0 for some j), 
  
    then s is set to 0 and a non-trivial solution to A*x = 0 is returned. 
  

    Arguments   
    =========   

    UPLO    (input) CHARACTER*1   
            Specifies whether the matrix A is upper or lower triangular. 
  
            = 'U':  Upper triangular   
            = 'L':  Lower triangular   

    TRANS   (input) CHARACTER*1   
            Specifies the operation applied to A.   
            = 'N':  Solve A * x = s*b     (No transpose)   
            = 'T':  Solve A**T * x = s*b  (Transpose)   
            = 'C':  Solve A**H * x = s*b  (Conjugate transpose)   

    DIAG    (input) CHARACTER*1   
            Specifies whether or not the matrix A is unit triangular.   
            = 'N':  Non-unit triangular   
            = 'U':  Unit triangular   

    NORMIN  (input) CHARACTER*1   
            Specifies whether CNORM has been set or not.   
            = 'Y':  CNORM contains the column norms on entry   
            = 'N':  CNORM is not set on entry.  On exit, the norms will   
                    be computed and stored in CNORM.   

    N       (input) INTEGER   
            The order of the matrix A.  N >= 0.   

    A       (input) COMPLEX*16 array, dimension (LDA,N)   
            The triangular matrix A.  If UPLO = 'U', the leading n by n   
            upper triangular part of the array A contains the upper   
            triangular matrix, and the strictly lower triangular part of 
  
            A is not referenced.  If UPLO = 'L', the leading n by n lower 
  
            triangular part of the array A contains the lower triangular 
  
            matrix, and the strictly upper triangular part of A is not   
            referenced.  If DIAG = 'U', the diagonal elements of A are   
            also not referenced and are assumed to be 1.   

    LDA     (input) INTEGER   
            The leading dimension of the array A.  LDA >= max (1,N).   

    X       (input/output) COMPLEX*16 array, dimension (N)   
            On entry, the right hand side b of the triangular system.   
            On exit, X is overwritten by the solution vector x.   

    SCALE   (output) DOUBLE PRECISION   
            The scaling factor s for the triangular system   
               A * x = s*b,  A**T * x = s*b,  or  A**H * x = s*b.   
            If SCALE = 0, the matrix A is singular or badly scaled, and   
            the vector x is an exact or approximate solution to A*x = 0. 
  

    CNORM   (input or output) DOUBLE PRECISION array, dimension (N)   

            If NORMIN = 'Y', CNORM is an input argument and CNORM(j)   
            contains the norm of the off-diagonal part of the j-th column 
  
            of A.  If TRANS = 'N', CNORM(j) must be greater than or equal 
  
            to the infinity-norm, and if TRANS = 'T' or 'C', CNORM(j)   
            must be greater than or equal to the 1-norm.   

            If NORMIN = 'N', CNORM is an output argument and CNORM(j)   
            returns the 1-norm of the offdiagonal part of the j-th column 
  
            of A.   

    INFO    (output) INTEGER   
            = 0:  successful exit   
            < 0:  if INFO = -k, the k-th argument had an illegal value   

    Further Details   
    ======= =======   

    A rough bound on x is computed; if that is less than overflow, ZTRSV 
  
    is called, otherwise, specific code is used which checks for possible 
  
    overflow or divide-by-zero at every operation.   

    A columnwise scheme is used for solving A*x = b.  The basic algorithm 
  
    if A is lower triangular is   

         x[1:n] := b[1:n]   
         for j = 1, ..., n   
              x(j) := x(j) / A(j,j)   
              x[j+1:n] := x[j+1:n] - x(j) * A[j+1:n,j]   
         end   

    Define bounds on the components of x after j iterations of the loop: 
  
       M(j) = bound on x[1:j]   
       G(j) = bound on x[j+1:n]   
    Initially, let M(0) = 0 and G(0) = max{x(i), i=1,...,n}.   

    Then for iteration j+1 we have   
       M(j+1) <= G(j) / | A(j+1,j+1) |   
       G(j+1) <= G(j) + M(j+1) * | A[j+2:n,j+1] |   
              <= G(j) ( 1 + CNORM(j+1) / | A(j+1,j+1) | )   

    where CNORM(j+1) is greater than or equal to the infinity-norm of   
    column j+1 of A, not counting the diagonal.  Hence   

       G(j) <= G(0) product ( 1 + CNORM(i) / | A(i,i) | )   
                    1<=i<=j   
    and   

       |x(j)| <= ( G(0) / |A(j,j)| ) product ( 1 + CNORM(i) / |A(i,i)| ) 
  
                                     1<=i< j   

    Since |x(j)| <= M(j), we use the Level 2 BLAS routine ZTRSV if the   
    reciprocal of the largest M(j), j=1,..,n, is larger than   
    max(underflow, 1/overflow).   

    The bound on x(j) is also used to determine when a step in the   
    columnwise method can be performed without fear of overflow.  If   
    the computed bound is greater than a large constant, x is scaled to   
    prevent overflow, but if the bound overflows, x is set to 0, x(j) to 
  
    1, and scale to 0, and a non-trivial solution to A*x = 0 is found.   

    Similarly, a row-wise scheme is used to solve A**T *x = b  or   
    A**H *x = b.  The basic algorithm for A upper triangular is   

         for j = 1, ..., n   
              x(j) := ( b(j) - A[1:j-1,j]' * x[1:j-1] ) / A(j,j)   
         end   

    We simultaneously compute two bounds   
         G(j) = bound on ( b(i) - A[1:i-1,i]' * x[1:i-1] ), 1<=i<=j   
         M(j) = bound on x(i), 1<=i<=j   

    The initial values are G(0) = 0, M(0) = max{b(i), i=1,..,n}, and we   
    add the constraint G(j) >= G(j-1) and M(j) >= M(j-1) for j >= 1.   
    Then the bound on x(j) is   

         M(j) <= M(j-1) * ( 1 + CNORM(j) ) / | A(j,j) |   

              <= M(0) * product ( ( 1 + CNORM(i) ) / |A(i,i)| )   
                        1<=i<=j   

    and we can safely call ZTRSV if 1/M(n) and 1/G(n) are both greater   
    than max(underflow, 1/overflow).   

    ===================================================================== 
  


    
   Parameter adjustments   
       Function Body */
    /* Table of constant values */
    static integer c__1 = 1;
    static doublereal c_b36 = .5;
    
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3, i__4, i__5;
    doublereal d__1, d__2, d__3, d__4;
    doublecomplex z__1, z__2, z__3, z__4;
    /* Builtin functions */
    double d_imag(doublecomplex *);
    void d_cnjg(doublecomplex *, doublecomplex *);
    /* Local variables */
    static integer jinc;
    static doublereal xbnd;
    static integer imax;
    static doublereal tmax;
    static doublecomplex tjjs;
    static doublereal xmax, grow;
    static integer i, j;
    extern /* Subroutine */ int dscal_(integer *, doublereal *, doublereal *, 
	    integer *);
    extern logical lsame_(char *, char *);
    static doublereal tscal;
    static doublecomplex uscal;
    static integer jlast;
    static doublecomplex csumj;
    extern /* Double Complex */ VOID zdotc_(doublecomplex *, integer *, 
	    doublecomplex *, integer *, doublecomplex *, integer *);
    static logical upper;
    extern /* Double Complex */ VOID zdotu_(doublecomplex *, integer *, 
	    doublecomplex *, integer *, doublecomplex *, integer *);
    extern /* Subroutine */ int zaxpy_(integer *, doublecomplex *, 
	    doublecomplex *, integer *, doublecomplex *, integer *), ztrsv_(
	    char *, char *, char *, integer *, doublecomplex *, integer *, 
	    doublecomplex *, integer *), dlabad_(
	    doublereal *, doublereal *);
    extern doublereal dlamch_(char *);
    static doublereal xj;
    extern integer idamax_(integer *, doublereal *, integer *);
    extern /* Subroutine */ int xerbla_(char *, integer *), zdscal_(
	    integer *, doublereal *, doublecomplex *, integer *);
    static doublereal bignum;
    extern integer izamax_(integer *, doublecomplex *, integer *);
    extern /* Double Complex */ VOID zladiv_(doublecomplex *, doublecomplex *,
	     doublecomplex *);
    static logical notran;
    static integer jfirst;
    extern doublereal dzasum_(integer *, doublecomplex *, integer *);
    static doublereal smlnum;
    static logical nounit;
    static doublereal rec, tjj;



#define X(I) x[(I)-1]
#define CNORM(I) cnorm[(I)-1]

#define A(I,J) a[(I)-1 + ((J)-1)* ( *lda)]

    *info = 0;
    upper = lsame_(uplo, "U");
    notran = lsame_(trans, "N");
    nounit = lsame_(diag, "N");

/*     Test the input parameters. */

    if (! upper && ! lsame_(uplo, "L")) {
	*info = -1;
    } else if (! notran && ! lsame_(trans, "T") && ! lsame_(trans, 
	    "C")) {
	*info = -2;
    } else if (! nounit && ! lsame_(diag, "U")) {
	*info = -3;
    } else if (! lsame_(normin, "Y") && ! lsame_(normin, "N"))
	     {
	*info = -4;
    } else if (*n < 0) {
	*info = -5;
    } else if (*lda < max(1,*n)) {
	*info = -7;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("ZLATRS", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	return 0;
    }

/*     Determine machine dependent parameters to control overflow. */

    smlnum = dlamch_("Safe minimum");
    bignum = 1. / smlnum;
    dlabad_(&smlnum, &bignum);
    smlnum /= dlamch_("Precision");
    bignum = 1. / smlnum;
    *scale = 1.;

    if (lsame_(normin, "N")) {

/*        Compute the 1-norm of each column, not including the diagona
l. */

	if (upper) {

/*           A is upper triangular. */

	    i__1 = *n;
	    for (j = 1; j <= *n; ++j) {
		i__2 = j - 1;
		CNORM(j) = dzasum_(&i__2, &A(1,j), &c__1);
/* L10: */
	    }
	} else {

/*           A is lower triangular. */

	    i__1 = *n - 1;
	    for (j = 1; j <= *n-1; ++j) {
		i__2 = *n - j;
		CNORM(j) = dzasum_(&i__2, &A(j+1,j), &c__1);
/* L20: */
	    }
	    CNORM(*n) = 0.;
	}
    }

/*     Scale the column norms by TSCAL if the maximum element in CNORM is 
  
       greater than BIGNUM/2. */

    imax = idamax_(n, &CNORM(1), &c__1);
    tmax = CNORM(imax);
    if (tmax <= bignum * .5) {
	tscal = 1.;
    } else {
	tscal = .5 / (smlnum * tmax);
	dscal_(n, &tscal, &CNORM(1), &c__1);
    }

/*     Compute a bound on the computed solution vector to see if the   
       Level 2 BLAS routine ZTRSV can be used. */

    xmax = 0.;
    i__1 = *n;
    for (j = 1; j <= *n; ++j) {
/* Computing MAX */
	i__2 = j;
	d__3 = xmax, d__4 = (d__1 = X(j).r / 2., abs(d__1)) + (d__2 = 
		d_imag(&X(j)) / 2., abs(d__2));
	xmax = max(d__3,d__4);
/* L30: */
    }
    xbnd = xmax;

    if (notran) {

/*        Compute the growth in A * x = b. */

	if (upper) {
	    jfirst = *n;
	    jlast = 1;
	    jinc = -1;
	} else {
	    jfirst = 1;
	    jlast = *n;
	    jinc = 1;
	}

	if (tscal != 1.) {
	    grow = 0.;
	    goto L60;
	}

	if (nounit) {

/*           A is non-unit triangular.   

             Compute GROW = 1/G(j) and XBND = 1/M(j).   
             Initially, G(0) = max{x(i), i=1,...,n}. */

	    grow = .5 / max(xbnd,smlnum);
	    xbnd = grow;
	    i__1 = jlast;
	    i__2 = jinc;
	    for (j = jfirst; jinc < 0 ? j >= jlast : j <= jlast; j += jinc) {

/*              Exit the loop if the growth factor is too smal
l. */

		if (grow <= smlnum) {
		    goto L60;
		}

		i__3 = j + j * a_dim1;
		tjjs.r = A(j,j).r, tjjs.i = A(j,j).i;
		tjj = (d__1 = tjjs.r, abs(d__1)) + (d__2 = d_imag(&tjjs), abs(
			d__2));

		if (tjj >= smlnum) {

/*                 M(j) = G(j-1) / abs(A(j,j))   

   Computing MIN */
		    d__1 = xbnd, d__2 = min(1.,tjj) * grow;
		    xbnd = min(d__1,d__2);
		} else {

/*                 M(j) could overflow, set XBND to 0. */

		    xbnd = 0.;
		}

		if (tjj + CNORM(j) >= smlnum) {

/*                 G(j) = G(j-1)*( 1 + CNORM(j) / abs(A(j,
j)) ) */

		    grow *= tjj / (tjj + CNORM(j));
		} else {

/*                 G(j) could overflow, set GROW to 0. */

		    grow = 0.;
		}
/* L40: */
	    }
	    grow = xbnd;
	} else {

/*           A is unit triangular.   

             Compute GROW = 1/G(j), where G(0) = max{x(i), i=1,...
,n}.   

   Computing MIN */
	    d__1 = 1., d__2 = .5 / max(xbnd,smlnum);
	    grow = min(d__1,d__2);
	    i__2 = jlast;
	    i__1 = jinc;
	    for (j = jfirst; jinc < 0 ? j >= jlast : j <= jlast; j += jinc) {

/*              Exit the loop if the growth factor is too smal
l. */

		if (grow <= smlnum) {
		    goto L60;
		}

/*              G(j) = G(j-1)*( 1 + CNORM(j) ) */

		grow *= 1. / (CNORM(j) + 1.);
/* L50: */
	    }
	}
L60:

	;
    } else {

/*        Compute the growth in A**T * x = b  or  A**H * x = b. */

	if (upper) {
	    jfirst = 1;
	    jlast = *n;
	    jinc = 1;
	} else {
	    jfirst = *n;
	    jlast = 1;
	    jinc = -1;
	}

	if (tscal != 1.) {
	    grow = 0.;
	    goto L90;
	}

	if (nounit) {

/*           A is non-unit triangular.   

             Compute GROW = 1/G(j) and XBND = 1/M(j).   
             Initially, M(0) = max{x(i), i=1,...,n}. */

	    grow = .5 / max(xbnd,smlnum);
	    xbnd = grow;
	    i__1 = jlast;
	    i__2 = jinc;
	    for (j = jfirst; jinc < 0 ? j >= jlast : j <= jlast; j += jinc) {

/*              Exit the loop if the growth factor is too smal
l. */

		if (grow <= smlnum) {
		    goto L90;
		}

/*              G(j) = max( G(j-1), M(j-1)*( 1 + CNORM(j) ) ) 
*/

		xj = CNORM(j) + 1.;
/* Computing MIN */
		d__1 = grow, d__2 = xbnd / xj;
		grow = min(d__1,d__2);

		i__3 = j + j * a_dim1;
		tjjs.r = A(j,j).r, tjjs.i = A(j,j).i;
		tjj = (d__1 = tjjs.r, abs(d__1)) + (d__2 = d_imag(&tjjs), abs(
			d__2));

		if (tjj >= smlnum) {

/*                 M(j) = M(j-1)*( 1 + CNORM(j) ) / abs(A(
j,j)) */

		    if (xj > tjj) {
			xbnd *= tjj / xj;
		    }
		} else {

/*                 M(j) could overflow, set XBND to 0. */

		    xbnd = 0.;
		}
/* L70: */
	    }
	    grow = min(grow,xbnd);
	} else {

/*           A is unit triangular.   

             Compute GROW = 1/G(j), where G(0) = max{x(i), i=1,...
,n}.   

   Computing MIN */
	    d__1 = 1., d__2 = .5 / max(xbnd,smlnum);
	    grow = min(d__1,d__2);
	    i__2 = jlast;
	    i__1 = jinc;
	    for (j = jfirst; jinc < 0 ? j >= jlast : j <= jlast; j += jinc) {

/*              Exit the loop if the growth factor is too smal
l. */

		if (grow <= smlnum) {
		    goto L90;
		}

/*              G(j) = ( 1 + CNORM(j) )*G(j-1) */

		xj = CNORM(j) + 1.;
		grow /= xj;
/* L80: */
	    }
	}
L90:
	;
    }

    if (grow * tscal > smlnum) {

/*        Use the Level 2 BLAS solve if the reciprocal of the bound on
   
          elements of X is not too small. */

	ztrsv_(uplo, trans, diag, n, &A(1,1), lda, &X(1), &c__1);
    } else {

/*        Use a Level 1 BLAS solve, scaling intermediate results. */

	if (xmax > bignum * .5) {

/*           Scale X so that its components are less than or equal
 to   
             BIGNUM in absolute value. */

	    *scale = bignum * .5 / xmax;
	    zdscal_(n, scale, &X(1), &c__1);
	    xmax = bignum;
	} else {
	    xmax *= 2.;
	}

	if (notran) {

/*           Solve A * x = b */

	    i__1 = jlast;
	    i__2 = jinc;
	    for (j = jfirst; jinc < 0 ? j >= jlast : j <= jlast; j += jinc) {

/*              Compute x(j) = b(j) / A(j,j), scaling x if nec
essary. */

		i__3 = j;
		xj = (d__1 = X(j).r, abs(d__1)) + (d__2 = d_imag(&X(j)), 
			abs(d__2));
		if (nounit) {
		    i__3 = j + j * a_dim1;
		    z__1.r = tscal * A(j,j).r, z__1.i = tscal * A(j,j).i;
		    tjjs.r = z__1.r, tjjs.i = z__1.i;
		} else {
		    tjjs.r = tscal, tjjs.i = 0.;
		    if (tscal == 1.) {
			goto L110;
		    }
		}
		tjj = (d__1 = tjjs.r, abs(d__1)) + (d__2 = d_imag(&tjjs), abs(
			d__2));
		if (tjj > smlnum) {

/*                    abs(A(j,j)) > SMLNUM: */

		    if (tjj < 1.) {
			if (xj > tjj * bignum) {

/*                          Scale x by 1/b(j). */

			    rec = 1. / xj;
			    zdscal_(n, &rec, &X(1), &c__1);
			    *scale *= rec;
			    xmax *= rec;
			}
		    }
		    i__3 = j;
		    zladiv_(&z__1, &X(j), &tjjs);
		    X(j).r = z__1.r, X(j).i = z__1.i;
		    i__3 = j;
		    xj = (d__1 = X(j).r, abs(d__1)) + (d__2 = d_imag(&X(j))
			    , abs(d__2));
		} else if (tjj > 0.) {

/*                    0 < abs(A(j,j)) <= SMLNUM: */

		    if (xj > tjj * bignum) {

/*                       Scale x by (1/abs(x(j)))*abs(
A(j,j))*BIGNUM   
                         to avoid overflow when dividi
ng by A(j,j). */

			rec = tjj * bignum / xj;
			if (CNORM(j) > 1.) {

/*                          Scale by 1/CNORM(j) to
 avoid overflow when   
                            multiplying x(j) times
 column j. */

			    rec /= CNORM(j);
			}
			zdscal_(n, &rec, &X(1), &c__1);
			*scale *= rec;
			xmax *= rec;
		    }
		    i__3 = j;
		    zladiv_(&z__1, &X(j), &tjjs);
		    X(j).r = z__1.r, X(j).i = z__1.i;
		    i__3 = j;
		    xj = (d__1 = X(j).r, abs(d__1)) + (d__2 = d_imag(&X(j))
			    , abs(d__2));
		} else {

/*                    A(j,j) = 0:  Set x(1:n) = 0, x(j) = 
1, and   
                      scale = 0, and compute a solution to
 A*x = 0. */

		    i__3 = *n;
		    for (i = 1; i <= *n; ++i) {
			i__4 = i;
			X(i).r = 0., X(i).i = 0.;
/* L100: */
		    }
		    i__3 = j;
		    X(j).r = 1., X(j).i = 0.;
		    xj = 1.;
		    *scale = 0.;
		    xmax = 0.;
		}
L110:

/*              Scale x if necessary to avoid overflow when ad
ding a   
                multiple of column j of A. */

		if (xj > 1.) {
		    rec = 1. / xj;
		    if (CNORM(j) > (bignum - xmax) * rec) {

/*                    Scale x by 1/(2*abs(x(j))). */

			rec *= .5;
			zdscal_(n, &rec, &X(1), &c__1);
			*scale *= rec;
		    }
		} else if (xj * CNORM(j) > bignum - xmax) {

/*                 Scale x by 1/2. */

		    zdscal_(n, &c_b36, &X(1), &c__1);
		    *scale *= .5;
		}

		if (upper) {
		    if (j > 1) {

/*                    Compute the update   
                         x(1:j-1) := x(1:j-1) - x(j) *
 A(1:j-1,j) */

			i__3 = j - 1;
			i__4 = j;
			z__2.r = -X(j).r, z__2.i = -X(j).i;
			z__1.r = tscal * z__2.r, z__1.i = tscal * z__2.i;
			zaxpy_(&i__3, &z__1, &A(1,j), &c__1, &X(1),
				 &c__1);
			i__3 = j - 1;
			i = izamax_(&i__3, &X(1), &c__1);
			i__3 = i;
			xmax = (d__1 = X(i).r, abs(d__1)) + (d__2 = d_imag(
				&X(i)), abs(d__2));
		    }
		} else {
		    if (j < *n) {

/*                    Compute the update   
                         x(j+1:n) := x(j+1:n) - x(j) *
 A(j+1:n,j) */

			i__3 = *n - j;
			i__4 = j;
			z__2.r = -X(j).r, z__2.i = -X(j).i;
			z__1.r = tscal * z__2.r, z__1.i = tscal * z__2.i;
			zaxpy_(&i__3, &z__1, &A(j+1,j), &c__1, &
				X(j + 1), &c__1);
			i__3 = *n - j;
			i = j + izamax_(&i__3, &X(j + 1), &c__1);
			i__3 = i;
			xmax = (d__1 = X(i).r, abs(d__1)) + (d__2 = d_imag(
				&X(i)), abs(d__2));
		    }
		}
/* L120: */
	    }

	} else if (lsame_(trans, "T")) {

/*           Solve A**T * x = b */

	    i__2 = jlast;
	    i__1 = jinc;
	    for (j = jfirst; jinc < 0 ? j >= jlast : j <= jlast; j += jinc) {

/*              Compute x(j) = b(j) - sum A(k,j)*x(k).   
                                      k<>j */

		i__3 = j;
		xj = (d__1 = X(j).r, abs(d__1)) + (d__2 = d_imag(&X(j)), 
			abs(d__2));
		uscal.r = tscal, uscal.i = 0.;
		rec = 1. / max(xmax,1.);
		if (CNORM(j) > (bignum - xj) * rec) {

/*                 If x(j) could overflow, scale x by 1/(2
*XMAX). */

		    rec *= .5;
		    if (nounit) {
			i__3 = j + j * a_dim1;
			z__1.r = tscal * A(j,j).r, z__1.i = tscal * A(j,j)
				.i;
			tjjs.r = z__1.r, tjjs.i = z__1.i;
		    } else {
			tjjs.r = tscal, tjjs.i = 0.;
		    }
		    tjj = (d__1 = tjjs.r, abs(d__1)) + (d__2 = d_imag(&tjjs), 
			    abs(d__2));
		    if (tjj > 1.) {

/*                       Divide by A(j,j) when scaling
 x if A(j,j) > 1.   

   Computing MIN */
			d__1 = 1., d__2 = rec * tjj;
			rec = min(d__1,d__2);
			zladiv_(&z__1, &uscal, &tjjs);
			uscal.r = z__1.r, uscal.i = z__1.i;
		    }
		    if (rec < 1.) {
			zdscal_(n, &rec, &X(1), &c__1);
			*scale *= rec;
			xmax *= rec;
		    }
		}

		csumj.r = 0., csumj.i = 0.;
		if (uscal.r == 1. && uscal.i == 0.) {

/*                 If the scaling needed for A in the dot 
product is 1,   
                   call ZDOTU to perform the dot product. 
*/

		    if (upper) {
			i__3 = j - 1;
			zdotu_(&z__1, &i__3, &A(1,j), &c__1, &X(1),
				 &c__1);
			csumj.r = z__1.r, csumj.i = z__1.i;
		    } else if (j < *n) {
			i__3 = *n - j;
			zdotu_(&z__1, &i__3, &A(j+1,j), &c__1, &
				X(j + 1), &c__1);
			csumj.r = z__1.r, csumj.i = z__1.i;
		    }
		} else {

/*                 Otherwise, use in-line code for the dot
 product. */

		    if (upper) {
			i__3 = j - 1;
			for (i = 1; i <= j-1; ++i) {
			    i__4 = i + j * a_dim1;
			    z__3.r = A(i,j).r * uscal.r - A(i,j).i * 
				    uscal.i, z__3.i = A(i,j).r * uscal.i + A(i,j).i * uscal.r;
			    i__5 = i;
			    z__2.r = z__3.r * X(i).r - z__3.i * X(i).i, 
				    z__2.i = z__3.r * X(i).i + z__3.i * X(
				    i).r;
			    z__1.r = csumj.r + z__2.r, z__1.i = csumj.i + 
				    z__2.i;
			    csumj.r = z__1.r, csumj.i = z__1.i;
/* L130: */
			}
		    } else if (j < *n) {
			i__3 = *n;
			for (i = j + 1; i <= *n; ++i) {
			    i__4 = i + j * a_dim1;
			    z__3.r = A(i,j).r * uscal.r - A(i,j).i * 
				    uscal.i, z__3.i = A(i,j).r * uscal.i + A(i,j).i * uscal.r;
			    i__5 = i;
			    z__2.r = z__3.r * X(i).r - z__3.i * X(i).i, 
				    z__2.i = z__3.r * X(i).i + z__3.i * X(
				    i).r;
			    z__1.r = csumj.r + z__2.r, z__1.i = csumj.i + 
				    z__2.i;
			    csumj.r = z__1.r, csumj.i = z__1.i;
/* L140: */
			}
		    }
		}

		z__1.r = tscal, z__1.i = 0.;
		if (uscal.r == z__1.r && uscal.i == z__1.i) {

/*                 Compute x(j) := ( x(j) - CSUMJ ) / A(j,
j) if 1/A(j,j)   
                   was not used to scale the dotproduct. 
*/

		    i__3 = j;
		    i__4 = j;
		    z__1.r = X(j).r - csumj.r, z__1.i = X(j).i - 
			    csumj.i;
		    X(j).r = z__1.r, X(j).i = z__1.i;
		    i__3 = j;
		    xj = (d__1 = X(j).r, abs(d__1)) + (d__2 = d_imag(&X(j))
			    , abs(d__2));
		    if (nounit) {
			i__3 = j + j * a_dim1;
			z__1.r = tscal * A(j,j).r, z__1.i = tscal * A(j,j)
				.i;
			tjjs.r = z__1.r, tjjs.i = z__1.i;
		    } else {
			tjjs.r = tscal, tjjs.i = 0.;
			if (tscal == 1.) {
			    goto L160;
			}
		    }

/*                    Compute x(j) = x(j) / A(j,j), scalin
g if necessary. */

		    tjj = (d__1 = tjjs.r, abs(d__1)) + (d__2 = d_imag(&tjjs), 
			    abs(d__2));
		    if (tjj > smlnum) {

/*                       abs(A(j,j)) > SMLNUM: */

			if (tjj < 1.) {
			    if (xj > tjj * bignum) {

/*                             Scale X by 1/ab
s(x(j)). */

				rec = 1. / xj;
				zdscal_(n, &rec, &X(1), &c__1);
				*scale *= rec;
				xmax *= rec;
			    }
			}
			i__3 = j;
			zladiv_(&z__1, &X(j), &tjjs);
			X(j).r = z__1.r, X(j).i = z__1.i;
		    } else if (tjj > 0.) {

/*                       0 < abs(A(j,j)) <= SMLNUM: */

			if (xj > tjj * bignum) {

/*                          Scale x by (1/abs(x(j)
))*abs(A(j,j))*BIGNUM. */

			    rec = tjj * bignum / xj;
			    zdscal_(n, &rec, &X(1), &c__1);
			    *scale *= rec;
			    xmax *= rec;
			}
			i__3 = j;
			zladiv_(&z__1, &X(j), &tjjs);
			X(j).r = z__1.r, X(j).i = z__1.i;
		    } else {

/*                       A(j,j) = 0:  Set x(1:n) = 0, 
x(j) = 1, and   
                         scale = 0 and compute a solut
ion to A**T *x = 0. */

			i__3 = *n;
			for (i = 1; i <= *n; ++i) {
			    i__4 = i;
			    X(i).r = 0., X(i).i = 0.;
/* L150: */
			}
			i__3 = j;
			X(j).r = 1., X(j).i = 0.;
			*scale = 0.;
			xmax = 0.;
		    }
L160:
		    ;
		} else {

/*                 Compute x(j) := x(j) / A(j,j) - CSUMJ i
f the dot   
                   product has already been divided by 1/A
(j,j). */

		    i__3 = j;
		    zladiv_(&z__2, &X(j), &tjjs);
		    z__1.r = z__2.r - csumj.r, z__1.i = z__2.i - csumj.i;
		    X(j).r = z__1.r, X(j).i = z__1.i;
		}
/* Computing MAX */
		i__3 = j;
		d__3 = xmax, d__4 = (d__1 = X(j).r, abs(d__1)) + (d__2 = 
			d_imag(&X(j)), abs(d__2));
		xmax = max(d__3,d__4);
/* L170: */
	    }

	} else {

/*           Solve A**H * x = b */

	    i__1 = jlast;
	    i__2 = jinc;
	    for (j = jfirst; jinc < 0 ? j >= jlast : j <= jlast; j += jinc) {

/*              Compute x(j) = b(j) - sum A(k,j)*x(k).   
                                      k<>j */

		i__3 = j;
		xj = (d__1 = X(j).r, abs(d__1)) + (d__2 = d_imag(&X(j)), 
			abs(d__2));
		uscal.r = tscal, uscal.i = 0.;
		rec = 1. / max(xmax,1.);
		if (CNORM(j) > (bignum - xj) * rec) {

/*                 If x(j) could overflow, scale x by 1/(2
*XMAX). */

		    rec *= .5;
		    if (nounit) {
			d_cnjg(&z__2, &A(j,j));
			z__1.r = tscal * z__2.r, z__1.i = tscal * z__2.i;
			tjjs.r = z__1.r, tjjs.i = z__1.i;
		    } else {
			tjjs.r = tscal, tjjs.i = 0.;
		    }
		    tjj = (d__1 = tjjs.r, abs(d__1)) + (d__2 = d_imag(&tjjs), 
			    abs(d__2));
		    if (tjj > 1.) {

/*                       Divide by A(j,j) when scaling
 x if A(j,j) > 1.   

   Computing MIN */
			d__1 = 1., d__2 = rec * tjj;
			rec = min(d__1,d__2);
			zladiv_(&z__1, &uscal, &tjjs);
			uscal.r = z__1.r, uscal.i = z__1.i;
		    }
		    if (rec < 1.) {
			zdscal_(n, &rec, &X(1), &c__1);
			*scale *= rec;
			xmax *= rec;
		    }
		}

		csumj.r = 0., csumj.i = 0.;
		if (uscal.r == 1. && uscal.i == 0.) {

/*                 If the scaling needed for A in the dot 
product is 1,   
                   call ZDOTC to perform the dot product. 
*/

		    if (upper) {
			i__3 = j - 1;
			zdotc_(&z__1, &i__3, &A(1,j), &c__1, &X(1),
				 &c__1);
			csumj.r = z__1.r, csumj.i = z__1.i;
		    } else if (j < *n) {
			i__3 = *n - j;
			zdotc_(&z__1, &i__3, &A(j+1,j), &c__1, &
				X(j + 1), &c__1);
			csumj.r = z__1.r, csumj.i = z__1.i;
		    }
		} else {

/*                 Otherwise, use in-line code for the dot
 product. */

		    if (upper) {
			i__3 = j - 1;
			for (i = 1; i <= j-1; ++i) {
			    d_cnjg(&z__4, &A(i,j));
			    z__3.r = z__4.r * uscal.r - z__4.i * uscal.i, 
				    z__3.i = z__4.r * uscal.i + z__4.i * 
				    uscal.r;
			    i__4 = i;
			    z__2.r = z__3.r * X(i).r - z__3.i * X(i).i, 
				    z__2.i = z__3.r * X(i).i + z__3.i * X(
				    i).r;
			    z__1.r = csumj.r + z__2.r, z__1.i = csumj.i + 
				    z__2.i;
			    csumj.r = z__1.r, csumj.i = z__1.i;
/* L180: */
			}
		    } else if (j < *n) {
			i__3 = *n;
			for (i = j + 1; i <= *n; ++i) {
			    d_cnjg(&z__4, &A(i,j));
			    z__3.r = z__4.r * uscal.r - z__4.i * uscal.i, 
				    z__3.i = z__4.r * uscal.i + z__4.i * 
				    uscal.r;
			    i__4 = i;
			    z__2.r = z__3.r * X(i).r - z__3.i * X(i).i, 
				    z__2.i = z__3.r * X(i).i + z__3.i * X(
				    i).r;
			    z__1.r = csumj.r + z__2.r, z__1.i = csumj.i + 
				    z__2.i;
			    csumj.r = z__1.r, csumj.i = z__1.i;
/* L190: */
			}
		    }
		}

		z__1.r = tscal, z__1.i = 0.;
		if (uscal.r == z__1.r && uscal.i == z__1.i) {

/*                 Compute x(j) := ( x(j) - CSUMJ ) / A(j,
j) if 1/A(j,j)   
                   was not used to scale the dotproduct. 
*/

		    i__3 = j;
		    i__4 = j;
		    z__1.r = X(j).r - csumj.r, z__1.i = X(j).i - 
			    csumj.i;
		    X(j).r = z__1.r, X(j).i = z__1.i;
		    i__3 = j;
		    xj = (d__1 = X(j).r, abs(d__1)) + (d__2 = d_imag(&X(j))
			    , abs(d__2));
		    if (nounit) {
			d_cnjg(&z__2, &A(j,j));
			z__1.r = tscal * z__2.r, z__1.i = tscal * z__2.i;
			tjjs.r = z__1.r, tjjs.i = z__1.i;
		    } else {
			tjjs.r = tscal, tjjs.i = 0.;
			if (tscal == 1.) {
			    goto L210;
			}
		    }

/*                    Compute x(j) = x(j) / A(j,j), scalin
g if necessary. */

		    tjj = (d__1 = tjjs.r, abs(d__1)) + (d__2 = d_imag(&tjjs), 
			    abs(d__2));
		    if (tjj > smlnum) {

/*                       abs(A(j,j)) > SMLNUM: */

			if (tjj < 1.) {
			    if (xj > tjj * bignum) {

/*                             Scale X by 1/ab
s(x(j)). */

				rec = 1. / xj;
				zdscal_(n, &rec, &X(1), &c__1);
				*scale *= rec;
				xmax *= rec;
			    }
			}
			i__3 = j;
			zladiv_(&z__1, &X(j), &tjjs);
			X(j).r = z__1.r, X(j).i = z__1.i;
		    } else if (tjj > 0.) {

/*                       0 < abs(A(j,j)) <= SMLNUM: */

			if (xj > tjj * bignum) {

/*                          Scale x by (1/abs(x(j)
))*abs(A(j,j))*BIGNUM. */

			    rec = tjj * bignum / xj;
			    zdscal_(n, &rec, &X(1), &c__1);
			    *scale *= rec;
			    xmax *= rec;
			}
			i__3 = j;
			zladiv_(&z__1, &X(j), &tjjs);
			X(j).r = z__1.r, X(j).i = z__1.i;
		    } else {

/*                       A(j,j) = 0:  Set x(1:n) = 0, 
x(j) = 1, and   
                         scale = 0 and compute a solut
ion to A**H *x = 0. */

			i__3 = *n;
			for (i = 1; i <= *n; ++i) {
			    i__4 = i;
			    X(i).r = 0., X(i).i = 0.;
/* L200: */
			}
			i__3 = j;
			X(j).r = 1., X(j).i = 0.;
			*scale = 0.;
			xmax = 0.;
		    }
L210:
		    ;
		} else {

/*                 Compute x(j) := x(j) / A(j,j) - CSUMJ i
f the dot   
                   product has already been divided by 1/A
(j,j). */

		    i__3 = j;
		    zladiv_(&z__2, &X(j), &tjjs);
		    z__1.r = z__2.r - csumj.r, z__1.i = z__2.i - csumj.i;
		    X(j).r = z__1.r, X(j).i = z__1.i;
		}
/* Computing MAX */
		i__3 = j;
		d__3 = xmax, d__4 = (d__1 = X(j).r, abs(d__1)) + (d__2 = 
			d_imag(&X(j)), abs(d__2));
		xmax = max(d__3,d__4);
/* L220: */
	    }
	}
	*scale /= tscal;
    }

/*     Scale the column norms by 1/TSCAL for return. */

    if (tscal != 1.) {
	d__1 = 1. / tscal;
	dscal_(n, &d__1, &CNORM(1), &c__1);
    }

    return 0;

/*     End of ZLATRS */

} /* zlatrs_ */

