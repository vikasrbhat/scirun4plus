#include "f2c.h"

/* Subroutine */ int chptrd_(char *uplo, integer *n, complex *ap, real *d, 
	real *e, complex *tau, integer *info)
{
/*  -- LAPACK routine (version 2.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    CHPTRD reduces a complex Hermitian matrix A stored in packed form to 
  
    real symmetric tridiagonal form T by a unitary similarity   
    transformation: Q**H * A * Q = T.   

    Arguments   
    =========   

    UPLO    (input) CHARACTER*1   
            = 'U':  Upper triangle of A is stored;   
            = 'L':  Lower triangle of A is stored.   

    N       (input) INTEGER   
            The order of the matrix A.  N >= 0.   

    AP      (input/output) COMPLEX array, dimension (N*(N+1)/2)   
            On entry, the upper or lower triangle of the Hermitian matrix 
  
            A, packed columnwise in a linear array.  The j-th column of A 
  
            is stored in the array AP as follows:   
            if UPLO = 'U', AP(i + (j-1)*j/2) = A(i,j) for 1<=i<=j;   
            if UPLO = 'L', AP(i + (j-1)*(2*n-j)/2) = A(i,j) for j<=i<=n. 
  
            On exit, if UPLO = 'U', the diagonal and first superdiagonal 
  
            of A are overwritten by the corresponding elements of the   
            tridiagonal matrix T, and the elements above the first   
            superdiagonal, with the array TAU, represent the unitary   
            matrix Q as a product of elementary reflectors; if UPLO   
            = 'L', the diagonal and first subdiagonal of A are over-   
            written by the corresponding elements of the tridiagonal   
            matrix T, and the elements below the first subdiagonal, with 
  
            the array TAU, represent the unitary matrix Q as a product   
            of elementary reflectors. See Further Details.   

    D       (output) REAL array, dimension (N)   
            The diagonal elements of the tridiagonal matrix T:   
            D(i) = A(i,i).   

    E       (output) REAL array, dimension (N-1)   
            The off-diagonal elements of the tridiagonal matrix T:   
            E(i) = A(i,i+1) if UPLO = 'U', E(i) = A(i+1,i) if UPLO = 'L'. 
  

    TAU     (output) COMPLEX array, dimension (N-1)   
            The scalar factors of the elementary reflectors (see Further 
  
            Details).   

    INFO    (output) INTEGER   
            = 0:  successful exit   
            < 0:  if INFO = -i, the i-th argument had an illegal value   

    Further Details   
    ===============   

    If UPLO = 'U', the matrix Q is represented as a product of elementary 
  
    reflectors   

       Q = H(n-1) . . . H(2) H(1).   

    Each H(i) has the form   

       H(i) = I - tau * v * v'   

    where tau is a complex scalar, and v is a complex vector with   
    v(i+1:n) = 0 and v(i) = 1; v(1:i-1) is stored on exit in AP,   
    overwriting A(1:i-1,i+1), and tau is stored in TAU(i).   

    If UPLO = 'L', the matrix Q is represented as a product of elementary 
  
    reflectors   

       Q = H(1) H(2) . . . H(n-1).   

    Each H(i) has the form   

       H(i) = I - tau * v * v'   

    where tau is a complex scalar, and v is a complex vector with   
    v(1:i) = 0 and v(i+1) = 1; v(i+2:n) is stored on exit in AP,   
    overwriting A(i+2:n,i), and tau is stored in TAU(i).   

    ===================================================================== 
  


       Test the input parameters   

    
   Parameter adjustments   
       Function Body */
    /* Table of constant values */
    static complex c_b2 = {0.f,0.f};
    static integer c__1 = 1;
    
    /* System generated locals */
    integer i__1, i__2, i__3;
    doublereal d__1;
    complex q__1, q__2, q__3, q__4;
    /* Local variables */
    static complex taui;
    extern /* Subroutine */ int chpr2_(char *, integer *, complex *, complex *
	    , integer *, complex *, integer *, complex *);
    static integer i;
    static complex alpha;
    extern /* Complex */ VOID cdotc_(complex *, integer *, complex *, integer 
	    *, complex *, integer *);
    extern logical lsame_(char *, char *);
    extern /* Subroutine */ int chpmv_(char *, integer *, complex *, complex *
	    , complex *, integer *, complex *, complex *, integer *), 
	    caxpy_(integer *, complex *, complex *, integer *, complex *, 
	    integer *);
    static integer i1;
    static logical upper;
    static integer ii;
    extern /* Subroutine */ int clarfg_(integer *, complex *, complex *, 
	    integer *, complex *), xerbla_(char *, integer *);
    static integer i1i1;



#define TAU(I) tau[(I)-1]
#define E(I) e[(I)-1]
#define D(I) d[(I)-1]
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
	xerbla_("CHPTRD", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*n <= 0) {
	return 0;
    }

    if (upper) {

/*        Reduce the upper triangle of A.   
          I1 is the index in AP of A(1,I+1). */

	i1 = *n * (*n - 1) / 2 + 1;
	i__1 = i1 + *n - 1;
	i__2 = i1 + *n - 1;
	d__1 = AP(i1+*n-1).r;
	AP(i1+*n-1).r = d__1, AP(i1+*n-1).i = 0.f;
	for (i = *n - 1; i >= 1; --i) {

/*           Generate elementary reflector H(i) = I - tau * v * v'
   
             to annihilate A(1:i-1,i+1) */

	    i__1 = i1 + i - 1;
	    alpha.r = AP(i1+i-1).r, alpha.i = AP(i1+i-1).i;
	    clarfg_(&i, &alpha, &AP(i1), &c__1, &taui);
	    i__1 = i;
	    E(i) = alpha.r;

	    if (taui.r != 0.f || taui.i != 0.f) {

/*              Apply H(i) from both sides to A(1:i,1:i) */

		i__1 = i1 + i - 1;
		AP(i1+i-1).r = 1.f, AP(i1+i-1).i = 0.f;

/*              Compute  y := tau * A * v  storing y in TAU(1:
i) */

		chpmv_(uplo, &i, &taui, &AP(1), &AP(i1), &c__1, &c_b2, &TAU(1)
			, &c__1);

/*              Compute  w := y - 1/2 * tau * (y'*v) * v */

		q__3.r = -.5f, q__3.i = 0.f;
		q__2.r = q__3.r * taui.r - q__3.i * taui.i, q__2.i = q__3.r * 
			taui.i + q__3.i * taui.r;
		cdotc_(&q__4, &i, &TAU(1), &c__1, &AP(i1), &c__1);
		q__1.r = q__2.r * q__4.r - q__2.i * q__4.i, q__1.i = q__2.r * 
			q__4.i + q__2.i * q__4.r;
		alpha.r = q__1.r, alpha.i = q__1.i;
		caxpy_(&i, &alpha, &AP(i1), &c__1, &TAU(1), &c__1);

/*              Apply the transformation as a rank-2 update: 
  
                   A := A - v * w' - w * v' */

		q__1.r = -1.f, q__1.i = 0.f;
		chpr2_(uplo, &i, &q__1, &AP(i1), &c__1, &TAU(1), &c__1, &AP(1)
			);

	    }
	    i__1 = i1 + i - 1;
	    i__2 = i;
	    AP(i1+i-1).r = E(i), AP(i1+i-1).i = 0.f;
	    i__1 = i + 1;
	    i__2 = i1 + i;
	    D(i+1) = AP(i1+i).r;
	    i__1 = i;
	    TAU(i).r = taui.r, TAU(i).i = taui.i;
	    i1 -= i;
/* L10: */
	}
	D(1) = AP(1).r;
    } else {

/*        Reduce the lower triangle of A. II is the index in AP of   
          A(i,i) and I1I1 is the index of A(i+1,i+1). */

	ii = 1;
	d__1 = AP(1).r;
	AP(1).r = d__1, AP(1).i = 0.f;
	i__1 = *n - 1;
	for (i = 1; i <= *n-1; ++i) {
	    i1i1 = ii + *n - i + 1;

/*           Generate elementary reflector H(i) = I - tau * v * v'
   
             to annihilate A(i+2:n,i) */

	    i__2 = ii + 1;
	    alpha.r = AP(ii+1).r, alpha.i = AP(ii+1).i;
	    i__2 = *n - i;
	    clarfg_(&i__2, &alpha, &AP(ii + 2), &c__1, &taui);
	    i__2 = i;
	    E(i) = alpha.r;

	    if (taui.r != 0.f || taui.i != 0.f) {

/*              Apply H(i) from both sides to A(i+1:n,i+1:n) 
*/

		i__2 = ii + 1;
		AP(ii+1).r = 1.f, AP(ii+1).i = 0.f;

/*              Compute  y := tau * A * v  storing y in TAU(i:
n-1) */

		i__2 = *n - i;
		chpmv_(uplo, &i__2, &taui, &AP(i1i1), &AP(ii + 1), &c__1, &
			c_b2, &TAU(i), &c__1);

/*              Compute  w := y - 1/2 * tau * (y'*v) * v */

		q__3.r = -.5f, q__3.i = 0.f;
		q__2.r = q__3.r * taui.r - q__3.i * taui.i, q__2.i = q__3.r * 
			taui.i + q__3.i * taui.r;
		i__2 = *n - i;
		cdotc_(&q__4, &i__2, &TAU(i), &c__1, &AP(ii + 1), &c__1);
		q__1.r = q__2.r * q__4.r - q__2.i * q__4.i, q__1.i = q__2.r * 
			q__4.i + q__2.i * q__4.r;
		alpha.r = q__1.r, alpha.i = q__1.i;
		i__2 = *n - i;
		caxpy_(&i__2, &alpha, &AP(ii + 1), &c__1, &TAU(i), &c__1);

/*              Apply the transformation as a rank-2 update: 
  
                   A := A - v * w' - w * v' */

		i__2 = *n - i;
		q__1.r = -1.f, q__1.i = 0.f;
		chpr2_(uplo, &i__2, &q__1, &AP(ii + 1), &c__1, &TAU(i), &c__1,
			 &AP(i1i1));

	    }
	    i__2 = ii + 1;
	    i__3 = i;
	    AP(ii+1).r = E(i), AP(ii+1).i = 0.f;
	    i__2 = i;
	    i__3 = ii;
	    D(i) = AP(ii).r;
	    i__2 = i;
	    TAU(i).r = taui.r, TAU(i).i = taui.i;
	    ii = i1i1;
/* L20: */
	}
	i__1 = *n;
	i__2 = ii;
	D(*n) = AP(ii).r;
    }

    return 0;

/*     End of CHPTRD */

} /* chptrd_ */

