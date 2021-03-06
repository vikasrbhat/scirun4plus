#include "f2c.h"

/* Subroutine */ int ctrsyl_(char *trana, char *tranb, integer *isgn, integer 
	*m, integer *n, complex *a, integer *lda, complex *b, integer *ldb, 
	complex *c, integer *ldc, real *scale, integer *info)
{
/*  -- LAPACK routine (version 2.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       March 31, 1993   


    Purpose   
    =======   

    CTRSYL solves the complex Sylvester matrix equation:   

       op(A)*X + X*op(B) = scale*C or   
       op(A)*X - X*op(B) = scale*C,   

    where op(A) = A or A**H, and A and B are both upper triangular. A is 
  
    M-by-M and B is N-by-N; the right hand side C and the solution X are 
  
    M-by-N; and scale is an output scale factor, set <= 1 to avoid   
    overflow in X.   

    Arguments   
    =========   

    TRANA   (input) CHARACTER*1   
            Specifies the option op(A):   
            = 'N': op(A) = A    (No transpose)   
            = 'C': op(A) = A**H (Conjugate transpose)   

    TRANB   (input) CHARACTER*1   
            Specifies the option op(B):   
            = 'N': op(B) = B    (No transpose)   
            = 'C': op(B) = B**H (Conjugate transpose)   

    ISGN    (input) INTEGER   
            Specifies the sign in the equation:   
            = +1: solve op(A)*X + X*op(B) = scale*C   
            = -1: solve op(A)*X - X*op(B) = scale*C   

    M       (input) INTEGER   
            The order of the matrix A, and the number of rows in the   
            matrices X and C. M >= 0.   

    N       (input) INTEGER   
            The order of the matrix B, and the number of columns in the   
            matrices X and C. N >= 0.   

    A       (input) COMPLEX array, dimension (LDA,M)   
            The upper triangular matrix A.   

    LDA     (input) INTEGER   
            The leading dimension of the array A. LDA >= max(1,M).   

    B       (input) COMPLEX array, dimension (LDB,N)   
            The upper triangular matrix B.   

    LDB     (input) INTEGER   
            The leading dimension of the array B. LDB >= max(1,N).   

    C       (input/output) COMPLEX array, dimension (LDC,N)   
            On entry, the M-by-N right hand side matrix C.   
            On exit, C is overwritten by the solution matrix X.   

    LDC     (input) INTEGER   
            The leading dimension of the array C. LDC >= max(1,M)   

    SCALE   (output) REAL   
            The scale factor, scale, set <= 1 to avoid overflow in X.   

    INFO    (output) INTEGER   
            = 0: successful exit   
            < 0: if INFO = -i, the i-th argument had an illegal value   
            = 1: A and B have common or very close eigenvalues; perturbed 
  
                 values were used to solve the equation (but the matrices 
  
                 A and B are unchanged).   

    ===================================================================== 
  


       Decode and Test input parameters   

    
   Parameter adjustments   
       Function Body */
    /* Table of constant values */
    static integer c__1 = 1;
    
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, c_dim1, c_offset, i__1, i__2, 
	    i__3, i__4;
    real r__1, r__2;
    complex q__1, q__2, q__3, q__4;
    /* Builtin functions */
    double r_imag(complex *);
    void c_div(complex *, complex *, complex *), r_cnjg(complex *, complex *);
    /* Local variables */
    static real smin;
    static complex suml, sumr;
    static integer j, k, l;
    extern /* Complex */ VOID cdotc_(complex *, integer *, complex *, integer 
	    *, complex *, integer *);
    extern logical lsame_(char *, char *);
    extern /* Complex */ VOID cdotu_(complex *, integer *, complex *, integer 
	    *, complex *, integer *);
    static complex a11;
    static real db;
    extern /* Subroutine */ int slabad_(real *, real *);
    extern doublereal clange_(char *, integer *, integer *, complex *, 
	    integer *, real *);
    static complex x11;
    static real scaloc;
    extern doublereal slamch_(char *);
    extern /* Subroutine */ int csscal_(integer *, real *, complex *, integer 
	    *), xerbla_(char *, integer *);
    static real bignum;
    static logical notrna, notrnb;
    static real smlnum, da11;
    static complex vec;
    static real dum[1], eps, sgn;




#define A(I,J) a[(I)-1 + ((J)-1)* ( *lda)]
#define B(I,J) b[(I)-1 + ((J)-1)* ( *ldb)]
#define C(I,J) c[(I)-1 + ((J)-1)* ( *ldc)]

    notrna = lsame_(trana, "N");
    notrnb = lsame_(tranb, "N");

    *info = 0;
    if (! notrna && ! lsame_(trana, "T") && ! lsame_(trana, "C")) {
	*info = -1;
    } else if (! notrnb && ! lsame_(tranb, "T") && ! lsame_(tranb, 
	    "C")) {
	*info = -2;
    } else if (*isgn != 1 && *isgn != -1) {
	*info = -3;
    } else if (*m < 0) {
	*info = -4;
    } else if (*n < 0) {
	*info = -5;
    } else if (*lda < max(1,*m)) {
	*info = -7;
    } else if (*ldb < max(1,*n)) {
	*info = -9;
    } else if (*ldc < max(1,*m)) {
	*info = -11;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("CTRSYL", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*m == 0 || *n == 0) {
	return 0;
    }

/*     Set constants to control overflow */

    eps = slamch_("P");
    smlnum = slamch_("S");
    bignum = 1.f / smlnum;
    slabad_(&smlnum, &bignum);
    smlnum = smlnum * (real) (*m * *n) / eps;
    bignum = 1.f / smlnum;
/* Computing MAX */
    r__1 = smlnum, r__2 = eps * clange_("M", m, m, &A(1,1), lda, dum)
	    , r__1 = max(r__1,r__2), r__2 = eps * clange_("M", n, n, &B(1,1), ldb, dum);
    smin = dmax(r__1,r__2);
    *scale = 1.f;
    sgn = (real) (*isgn);

    if (notrna && notrnb) {

/*        Solve    A*X + ISGN*X*B = scale*C.   

          The (K,L)th block of X is determined starting from   
          bottom-left corner column by column by   

              A(K,K)*X(K,L) + ISGN*X(K,L)*B(L,L) = C(K,L) - R(K,L)   

          Where   
                      M                        L-1   
            R(K,L) = SUM [A(K,I)*X(I,L)] +ISGN*SUM [X(K,J)*B(J,L)].   
                    I=K+1                      J=1 */

	i__1 = *n;
	for (l = 1; l <= *n; ++l) {
	    for (k = *m; k >= 1; --k) {

		i__2 = *m - k;
/* Computing MIN */
		i__3 = k + 1;
/* Computing MIN */
		i__4 = k + 1;
		cdotu_(&q__1, &i__2, &A(k,min(k+1,*m)), lda, &C(min(k+1,*m),l), &c__1);
		suml.r = q__1.r, suml.i = q__1.i;
		i__2 = l - 1;
		cdotu_(&q__1, &i__2, &C(k,1), ldc, &B(1,l), 
			&c__1);
		sumr.r = q__1.r, sumr.i = q__1.i;
		i__2 = k + l * c_dim1;
		q__3.r = sgn * sumr.r, q__3.i = sgn * sumr.i;
		q__2.r = suml.r + q__3.r, q__2.i = suml.i + q__3.i;
		q__1.r = C(k,l).r - q__2.r, q__1.i = C(k,l).i - q__2.i;
		vec.r = q__1.r, vec.i = q__1.i;

		scaloc = 1.f;
		i__2 = k + k * a_dim1;
		i__3 = l + l * b_dim1;
		q__2.r = sgn * B(l,l).r, q__2.i = sgn * B(l,l).i;
		q__1.r = A(k,k).r + q__2.r, q__1.i = A(k,k).i + q__2.i;
		a11.r = q__1.r, a11.i = q__1.i;
		da11 = (r__1 = a11.r, dabs(r__1)) + (r__2 = r_imag(&a11), 
			dabs(r__2));
		if (da11 <= smin) {
		    a11.r = smin, a11.i = 0.f;
		    da11 = smin;
		    *info = 1;
		}
		db = (r__1 = vec.r, dabs(r__1)) + (r__2 = r_imag(&vec), dabs(
			r__2));
		if (da11 < 1.f && db > 1.f) {
		    if (db > bignum * da11) {
			scaloc = 1.f / db;
		    }
		}
		q__3.r = scaloc, q__3.i = 0.f;
		q__2.r = vec.r * q__3.r - vec.i * q__3.i, q__2.i = vec.r * 
			q__3.i + vec.i * q__3.r;
		c_div(&q__1, &q__2, &a11);
		x11.r = q__1.r, x11.i = q__1.i;

		if (scaloc != 1.f) {
		    i__2 = *n;
		    for (j = 1; j <= *n; ++j) {
			csscal_(m, &scaloc, &C(1,j), &c__1);
/* L10: */
		    }
		    *scale *= scaloc;
		}
		i__2 = k + l * c_dim1;
		C(k,l).r = x11.r, C(k,l).i = x11.i;

/* L20: */
	    }
/* L30: */
	}

    } else if (! notrna && notrnb) {

/*        Solve    A' *X + ISGN*X*B = scale*C.   

          The (K,L)th block of X is determined starting from   
          upper-left corner column by column by   

              A'(K,K)*X(K,L) + ISGN*X(K,L)*B(L,L) = C(K,L) - R(K,L)   

          Where   
                     K-1                         L-1   
            R(K,L) = SUM [A'(I,K)*X(I,L)] + ISGN*SUM [X(K,J)*B(J,L)] 
  
                     I=1                         J=1 */

	i__1 = *n;
	for (l = 1; l <= *n; ++l) {
	    i__2 = *m;
	    for (k = 1; k <= *m; ++k) {

		i__3 = k - 1;
		cdotc_(&q__1, &i__3, &A(1,k), &c__1, &C(1,l), &c__1);
		suml.r = q__1.r, suml.i = q__1.i;
		i__3 = l - 1;
		cdotu_(&q__1, &i__3, &C(k,1), ldc, &B(1,l), 
			&c__1);
		sumr.r = q__1.r, sumr.i = q__1.i;
		i__3 = k + l * c_dim1;
		q__3.r = sgn * sumr.r, q__3.i = sgn * sumr.i;
		q__2.r = suml.r + q__3.r, q__2.i = suml.i + q__3.i;
		q__1.r = C(k,l).r - q__2.r, q__1.i = C(k,l).i - q__2.i;
		vec.r = q__1.r, vec.i = q__1.i;

		scaloc = 1.f;
		r_cnjg(&q__2, &A(k,k));
		i__3 = l + l * b_dim1;
		q__3.r = sgn * B(l,l).r, q__3.i = sgn * B(l,l).i;
		q__1.r = q__2.r + q__3.r, q__1.i = q__2.i + q__3.i;
		a11.r = q__1.r, a11.i = q__1.i;
		da11 = (r__1 = a11.r, dabs(r__1)) + (r__2 = r_imag(&a11), 
			dabs(r__2));
		if (da11 <= smin) {
		    a11.r = smin, a11.i = 0.f;
		    da11 = smin;
		    *info = 1;
		}
		db = (r__1 = vec.r, dabs(r__1)) + (r__2 = r_imag(&vec), dabs(
			r__2));
		if (da11 < 1.f && db > 1.f) {
		    if (db > bignum * da11) {
			scaloc = 1.f / db;
		    }
		}

		q__3.r = scaloc, q__3.i = 0.f;
		q__2.r = vec.r * q__3.r - vec.i * q__3.i, q__2.i = vec.r * 
			q__3.i + vec.i * q__3.r;
		c_div(&q__1, &q__2, &a11);
		x11.r = q__1.r, x11.i = q__1.i;

		if (scaloc != 1.f) {
		    i__3 = *n;
		    for (j = 1; j <= *n; ++j) {
			csscal_(m, &scaloc, &C(1,j), &c__1);
/* L40: */
		    }
		    *scale *= scaloc;
		}
		i__3 = k + l * c_dim1;
		C(k,l).r = x11.r, C(k,l).i = x11.i;

/* L50: */
	    }
/* L60: */
	}

    } else if (! notrna && ! notrnb) {

/*        Solve    A'*X + ISGN*X*B' = C.   

          The (K,L)th block of X is determined starting from   
          upper-right corner column by column by   

              A'(K,K)*X(K,L) + ISGN*X(K,L)*B'(L,L) = C(K,L) - R(K,L) 
  

          Where   
                      K-1   
             R(K,L) = SUM [A'(I,K)*X(I,L)] +   
                      I=1   
                             N   
                       ISGN*SUM [X(K,J)*B'(L,J)].   
                            J=L+1 */

	for (l = *n; l >= 1; --l) {
	    i__1 = *m;
	    for (k = 1; k <= *m; ++k) {

		i__2 = k - 1;
		cdotc_(&q__1, &i__2, &A(1,k), &c__1, &C(1,l), &c__1);
		suml.r = q__1.r, suml.i = q__1.i;
		i__2 = *n - l;
/* Computing MIN */
		i__3 = l + 1;
/* Computing MIN */
		i__4 = l + 1;
		cdotc_(&q__1, &i__2, &C(k,min(l+1,*n)), ldc, &B(l,min(l+1,*n)), ldb);
		sumr.r = q__1.r, sumr.i = q__1.i;
		i__2 = k + l * c_dim1;
		r_cnjg(&q__4, &sumr);
		q__3.r = sgn * q__4.r, q__3.i = sgn * q__4.i;
		q__2.r = suml.r + q__3.r, q__2.i = suml.i + q__3.i;
		q__1.r = C(k,l).r - q__2.r, q__1.i = C(k,l).i - q__2.i;
		vec.r = q__1.r, vec.i = q__1.i;

		scaloc = 1.f;
		i__2 = k + k * a_dim1;
		i__3 = l + l * b_dim1;
		q__3.r = sgn * B(l,l).r, q__3.i = sgn * B(l,l).i;
		q__2.r = A(k,k).r + q__3.r, q__2.i = A(k,k).i + q__3.i;
		r_cnjg(&q__1, &q__2);
		a11.r = q__1.r, a11.i = q__1.i;
		da11 = (r__1 = a11.r, dabs(r__1)) + (r__2 = r_imag(&a11), 
			dabs(r__2));
		if (da11 <= smin) {
		    a11.r = smin, a11.i = 0.f;
		    da11 = smin;
		    *info = 1;
		}
		db = (r__1 = vec.r, dabs(r__1)) + (r__2 = r_imag(&vec), dabs(
			r__2));
		if (da11 < 1.f && db > 1.f) {
		    if (db > bignum * da11) {
			scaloc = 1.f / db;
		    }
		}

		q__3.r = scaloc, q__3.i = 0.f;
		q__2.r = vec.r * q__3.r - vec.i * q__3.i, q__2.i = vec.r * 
			q__3.i + vec.i * q__3.r;
		c_div(&q__1, &q__2, &a11);
		x11.r = q__1.r, x11.i = q__1.i;

		if (scaloc != 1.f) {
		    i__2 = *n;
		    for (j = 1; j <= *n; ++j) {
			csscal_(m, &scaloc, &C(1,j), &c__1);
/* L70: */
		    }
		    *scale *= scaloc;
		}
		i__2 = k + l * c_dim1;
		C(k,l).r = x11.r, C(k,l).i = x11.i;

/* L80: */
	    }
/* L90: */
	}

    } else if (notrna && ! notrnb) {

/*        Solve    A*X + ISGN*X*B' = C.   

          The (K,L)th block of X is determined starting from   
          bottom-left corner column by column by   

             A(K,K)*X(K,L) + ISGN*X(K,L)*B'(L,L) = C(K,L) - R(K,L)   

          Where   
                      M                          N   
            R(K,L) = SUM [A(K,I)*X(I,L)] + ISGN*SUM [X(K,J)*B'(L,J)] 
  
                    I=K+1                      J=L+1 */

	for (l = *n; l >= 1; --l) {
	    for (k = *m; k >= 1; --k) {

		i__1 = *m - k;
/* Computing MIN */
		i__2 = k + 1;
/* Computing MIN */
		i__3 = k + 1;
		cdotu_(&q__1, &i__1, &A(k,min(k+1,*m)), lda, &C(min(k+1,*m),l), &c__1);
		suml.r = q__1.r, suml.i = q__1.i;
		i__1 = *n - l;
/* Computing MIN */
		i__2 = l + 1;
/* Computing MIN */
		i__3 = l + 1;
		cdotc_(&q__1, &i__1, &C(k,min(l+1,*n)), ldc, &B(l,min(l+1,*n)), ldb);
		sumr.r = q__1.r, sumr.i = q__1.i;
		i__1 = k + l * c_dim1;
		r_cnjg(&q__4, &sumr);
		q__3.r = sgn * q__4.r, q__3.i = sgn * q__4.i;
		q__2.r = suml.r + q__3.r, q__2.i = suml.i + q__3.i;
		q__1.r = C(k,l).r - q__2.r, q__1.i = C(k,l).i - q__2.i;
		vec.r = q__1.r, vec.i = q__1.i;

		scaloc = 1.f;
		i__1 = k + k * a_dim1;
		r_cnjg(&q__3, &B(l,l));
		q__2.r = sgn * q__3.r, q__2.i = sgn * q__3.i;
		q__1.r = A(k,k).r + q__2.r, q__1.i = A(k,k).i + q__2.i;
		a11.r = q__1.r, a11.i = q__1.i;
		da11 = (r__1 = a11.r, dabs(r__1)) + (r__2 = r_imag(&a11), 
			dabs(r__2));
		if (da11 <= smin) {
		    a11.r = smin, a11.i = 0.f;
		    da11 = smin;
		    *info = 1;
		}
		db = (r__1 = vec.r, dabs(r__1)) + (r__2 = r_imag(&vec), dabs(
			r__2));
		if (da11 < 1.f && db > 1.f) {
		    if (db > bignum * da11) {
			scaloc = 1.f / db;
		    }
		}

		q__3.r = scaloc, q__3.i = 0.f;
		q__2.r = vec.r * q__3.r - vec.i * q__3.i, q__2.i = vec.r * 
			q__3.i + vec.i * q__3.r;
		c_div(&q__1, &q__2, &a11);
		x11.r = q__1.r, x11.i = q__1.i;

		if (scaloc != 1.f) {
		    i__1 = *n;
		    for (j = 1; j <= *n; ++j) {
			csscal_(m, &scaloc, &C(1,j), &c__1);
/* L100: */
		    }
		    *scale *= scaloc;
		}
		i__1 = k + l * c_dim1;
		C(k,l).r = x11.r, C(k,l).i = x11.i;

/* L110: */
	    }
/* L120: */
	}

    }

    return 0;

/*     End of CTRSYL */

} /* ctrsyl_ */

