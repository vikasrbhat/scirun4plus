
/*  -- translated by f2c (version 19940927).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int stbsv_(char *uplo, char *trans, char *diag, integer *n, 
	integer *k, real *a, integer *lda, real *x, integer *incx)
{


    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3, i__4;

    /* Local variables */
    static integer info;
    static real temp;
    static integer i, j, l;
    extern logical lsame_(char *, char *);
    static integer kplus1, ix, jx, kx;
    extern /* Subroutine */ int xerbla_(char *, integer *);
    static logical nounit;


/*  Purpose   
    =======   

    STBSV  solves one of the systems of equations   

       A*x = b,   or   A'*x = b,   

    where b and x are n element vectors and A is an n by n unit, or   
    non-unit, upper or lower triangular band matrix, with ( k + 1 )   
    diagonals.   

    No test for singularity or near-singularity is included in this   
    routine. Such tests must be performed before calling this routine.   

    Parameters   
    ==========   

    UPLO   - CHARACTER*1.   
             On entry, UPLO specifies whether the matrix is an upper or   
             lower triangular matrix as follows:   

                UPLO = 'U' or 'u'   A is an upper triangular matrix.   

                UPLO = 'L' or 'l'   A is a lower triangular matrix.   

             Unchanged on exit.   

    TRANS  - CHARACTER*1.   
             On entry, TRANS specifies the equations to be solved as   
             follows:   

                TRANS = 'N' or 'n'   A*x = b.   

                TRANS = 'T' or 't'   A'*x = b.   

                TRANS = 'C' or 'c'   A'*x = b.   

             Unchanged on exit.   

    DIAG   - CHARACTER*1.   
             On entry, DIAG specifies whether or not A is unit   
             triangular as follows:   

                DIAG = 'U' or 'u'   A is assumed to be unit triangular.   

                DIAG = 'N' or 'n'   A is not assumed to be unit   
                                    triangular.   

             Unchanged on exit.   

    N      - INTEGER.   
             On entry, N specifies the order of the matrix A.   
             N must be at least zero.   
             Unchanged on exit.   

    K      - INTEGER.   
             On entry with UPLO = 'U' or 'u', K specifies the number of   
             super-diagonals of the matrix A.   
             On entry with UPLO = 'L' or 'l', K specifies the number of   
             sub-diagonals of the matrix A.   
             K must satisfy  0 .le. K.   
             Unchanged on exit.   

    A      - REAL             array of DIMENSION ( LDA, n ).   
             Before entry with UPLO = 'U' or 'u', the leading ( k + 1 )   
             by n part of the array A must contain the upper triangular   
             band part of the matrix of coefficients, supplied column by 
  
             column, with the leading diagonal of the matrix in row   
             ( k + 1 ) of the array, the first super-diagonal starting at 
  
             position 2 in row k, and so on. The top left k by k triangle 
  
             of the array A is not referenced.   
             The following program segment will transfer an upper   
             triangular band matrix from conventional full matrix storage 
  
             to band storage:   

                   DO 20, J = 1, N   
                      M = K + 1 - J   
                      DO 10, I = MAX( 1, J - K ), J   
                         A( M + I, J ) = matrix( I, J )   
                10    CONTINUE   
                20 CONTINUE   

             Before entry with UPLO = 'L' or 'l', the leading ( k + 1 )   
             by n part of the array A must contain the lower triangular   
             band part of the matrix of coefficients, supplied column by 
  
             column, with the leading diagonal of the matrix in row 1 of 
  
             the array, the first sub-diagonal starting at position 1 in 
  
             row 2, and so on. The bottom right k by k triangle of the   
             array A is not referenced.   
             The following program segment will transfer a lower   
             triangular band matrix from conventional full matrix storage 
  
             to band storage:   

                   DO 20, J = 1, N   
                      M = 1 - J   
                      DO 10, I = J, MIN( N, J + K )   
                         A( M + I, J ) = matrix( I, J )   
                10    CONTINUE   
                20 CONTINUE   

             Note that when DIAG = 'U' or 'u' the elements of the array A 
  
             corresponding to the diagonal elements of the matrix are not 
  
             referenced, but are assumed to be unity.   
             Unchanged on exit.   

    LDA    - INTEGER.   
             On entry, LDA specifies the first dimension of A as declared 
  
             in the calling (sub) program. LDA must be at least   
             ( k + 1 ).   
             Unchanged on exit.   

    X      - REAL             array of dimension at least   
             ( 1 + ( n - 1 )*abs( INCX ) ).   
             Before entry, the incremented array X must contain the n   
             element right-hand side vector b. On exit, X is overwritten 
  
             with the solution vector x.   

    INCX   - INTEGER.   
             On entry, INCX specifies the increment for the elements of   
             X. INCX must not be zero.   
             Unchanged on exit.   


    Level 2 Blas routine.   

    -- Written on 22-October-1986.   
       Jack Dongarra, Argonne National Lab.   
       Jeremy Du Croz, Nag Central Office.   
       Sven Hammarling, Nag Central Office.   
       Richard Hanson, Sandia National Labs.   



       Test the input parameters.   

    
   Parameter adjustments   
       Function Body */
#define X(I) x[(I)-1]

#define A(I,J) a[(I)-1 + ((J)-1)* ( *lda)]

    info = 0;
    if (! lsame_(uplo, "U") && ! lsame_(uplo, "L")) {
	info = 1;
    } else if (! lsame_(trans, "N") && ! lsame_(trans, "T") &&
	     ! lsame_(trans, "C")) {
	info = 2;
    } else if (! lsame_(diag, "U") && ! lsame_(diag, "N")) {
	info = 3;
    } else if (*n < 0) {
	info = 4;
    } else if (*k < 0) {
	info = 5;
    } else if (*lda < *k + 1) {
	info = 7;
    } else if (*incx == 0) {
	info = 9;
    }
    if (info != 0) {
	xerbla_("STBSV ", &info);
	return 0;
    }

/*     Quick return if possible. */

    if (*n == 0) {
	return 0;
    }

    nounit = lsame_(diag, "N");

/*     Set up the start point in X if the increment is not unity. This   
       will be  ( N - 1 )*INCX  too small for descending loops. */

    if (*incx <= 0) {
	kx = 1 - (*n - 1) * *incx;
    } else if (*incx != 1) {
	kx = 1;
    }

/*     Start the operations. In this version the elements of A are   
       accessed by sequentially with one pass through A. */

    if (lsame_(trans, "N")) {

/*        Form  x := inv( A )*x. */

	if (lsame_(uplo, "U")) {
	    kplus1 = *k + 1;
	    if (*incx == 1) {
		for (j = *n; j >= 1; --j) {
		    if (X(j) != 0.f) {
			l = kplus1 - j;
			if (nounit) {
			    X(j) /= A(kplus1,j);
			}
			temp = X(j);
/* Computing MAX */
			i__2 = 1, i__3 = j - *k;
			i__1 = max(i__2,i__3);
			for (i = j - 1; i >= max(1,j-*k); --i) {
			    X(i) -= temp * A(l+i,j);
/* L10: */
			}
		    }
/* L20: */
		}
	    } else {
		kx += (*n - 1) * *incx;
		jx = kx;
		for (j = *n; j >= 1; --j) {
		    kx -= *incx;
		    if (X(jx) != 0.f) {
			ix = kx;
			l = kplus1 - j;
			if (nounit) {
			    X(jx) /= A(kplus1,j);
			}
			temp = X(jx);
/* Computing MAX */
			i__2 = 1, i__3 = j - *k;
			i__1 = max(i__2,i__3);
			for (i = j - 1; i >= max(1,j-*k); --i) {
			    X(ix) -= temp * A(l+i,j);
			    ix -= *incx;
/* L30: */
			}
		    }
		    jx -= *incx;
/* L40: */
		}
	    }
	} else {
	    if (*incx == 1) {
		i__1 = *n;
		for (j = 1; j <= *n; ++j) {
		    if (X(j) != 0.f) {
			l = 1 - j;
			if (nounit) {
			    X(j) /= A(1,j);
			}
			temp = X(j);
/* Computing MIN */
			i__3 = *n, i__4 = j + *k;
			i__2 = min(i__3,i__4);
			for (i = j + 1; i <= min(*n,j+*k); ++i) {
			    X(i) -= temp * A(l+i,j);
/* L50: */
			}
		    }
/* L60: */
		}
	    } else {
		jx = kx;
		i__1 = *n;
		for (j = 1; j <= *n; ++j) {
		    kx += *incx;
		    if (X(jx) != 0.f) {
			ix = kx;
			l = 1 - j;
			if (nounit) {
			    X(jx) /= A(1,j);
			}
			temp = X(jx);
/* Computing MIN */
			i__3 = *n, i__4 = j + *k;
			i__2 = min(i__3,i__4);
			for (i = j + 1; i <= min(*n,j+*k); ++i) {
			    X(ix) -= temp * A(l+i,j);
			    ix += *incx;
/* L70: */
			}
		    }
		    jx += *incx;
/* L80: */
		}
	    }
	}
    } else {

/*        Form  x := inv( A')*x. */

	if (lsame_(uplo, "U")) {
	    kplus1 = *k + 1;
	    if (*incx == 1) {
		i__1 = *n;
		for (j = 1; j <= *n; ++j) {
		    temp = X(j);
		    l = kplus1 - j;
/* Computing MAX */
		    i__2 = 1, i__3 = j - *k;
		    i__4 = j - 1;
		    for (i = max(1,j-*k); i <= j-1; ++i) {
			temp -= A(l+i,j) * X(i);
/* L90: */
		    }
		    if (nounit) {
			temp /= A(kplus1,j);
		    }
		    X(j) = temp;
/* L100: */
		}
	    } else {
		jx = kx;
		i__1 = *n;
		for (j = 1; j <= *n; ++j) {
		    temp = X(jx);
		    ix = kx;
		    l = kplus1 - j;
/* Computing MAX */
		    i__4 = 1, i__2 = j - *k;
		    i__3 = j - 1;
		    for (i = max(1,j-*k); i <= j-1; ++i) {
			temp -= A(l+i,j) * X(ix);
			ix += *incx;
/* L110: */
		    }
		    if (nounit) {
			temp /= A(kplus1,j);
		    }
		    X(jx) = temp;
		    jx += *incx;
		    if (j > *k) {
			kx += *incx;
		    }
/* L120: */
		}
	    }
	} else {
	    if (*incx == 1) {
		for (j = *n; j >= 1; --j) {
		    temp = X(j);
		    l = 1 - j;
/* Computing MIN */
		    i__1 = *n, i__3 = j + *k;
		    i__4 = j + 1;
		    for (i = min(*n,j+*k); i >= j+1; --i) {
			temp -= A(l+i,j) * X(i);
/* L130: */
		    }
		    if (nounit) {
			temp /= A(1,j);
		    }
		    X(j) = temp;
/* L140: */
		}
	    } else {
		kx += (*n - 1) * *incx;
		jx = kx;
		for (j = *n; j >= 1; --j) {
		    temp = X(jx);
		    ix = kx;
		    l = 1 - j;
/* Computing MIN */
		    i__4 = *n, i__1 = j + *k;
		    i__3 = j + 1;
		    for (i = min(*n,j+*k); i >= j+1; --i) {
			temp -= A(l+i,j) * X(ix);
			ix -= *incx;
/* L150: */
		    }
		    if (nounit) {
			temp /= A(1,j);
		    }
		    X(jx) = temp;
		    jx -= *incx;
		    if (*n - j >= *k) {
			kx -= *incx;
		    }
/* L160: */
		}
	    }
	}
    }

    return 0;

/*     End of STBSV . */

} /* stbsv_ */
