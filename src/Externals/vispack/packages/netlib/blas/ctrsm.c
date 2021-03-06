
/*  -- translated by f2c (version 19940927).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static complex c_b1 = {1.f,0.f};

/* Subroutine */ int ctrsm_(char *side, char *uplo, char *transa, char *diag, 
	integer *m, integer *n, complex *alpha, complex *a, integer *lda, 
	complex *b, integer *ldb)
{


    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, i__1, i__2, i__3, i__4, i__5, 
	    i__6, i__7;
    complex q__1, q__2, q__3;

    /* Builtin functions */
    void c_div(complex *, complex *, complex *), r_cnjg(complex *, complex *);

    /* Local variables */
    static integer info;
    static complex temp;
    static integer i, j, k;
    extern logical lsame_(char *, char *);
    static logical lside;
    static integer nrowa;
    static logical upper;
    extern /* Subroutine */ int xerbla_(char *, integer *);
    static logical noconj, nounit;


/*  Purpose   
    =======   

    CTRSM  solves one of the matrix equations   

       op( A )*X = alpha*B,   or   X*op( A ) = alpha*B,   

    where alpha is a scalar, X and B are m by n matrices, A is a unit, or 
  
    non-unit,  upper or lower triangular matrix  and  op( A )  is one  of 
  

       op( A ) = A   or   op( A ) = A'   or   op( A ) = conjg( A' ).   

    The matrix X is overwritten on B.   

    Parameters   
    ==========   

    SIDE   - CHARACTER*1.   
             On entry, SIDE specifies whether op( A ) appears on the left 
  
             or right of X as follows:   

                SIDE = 'L' or 'l'   op( A )*X = alpha*B.   

                SIDE = 'R' or 'r'   X*op( A ) = alpha*B.   

             Unchanged on exit.   

    UPLO   - CHARACTER*1.   
             On entry, UPLO specifies whether the matrix A is an upper or 
  
             lower triangular matrix as follows:   

                UPLO = 'U' or 'u'   A is an upper triangular matrix.   

                UPLO = 'L' or 'l'   A is a lower triangular matrix.   

             Unchanged on exit.   

    TRANSA - CHARACTER*1.   
             On entry, TRANSA specifies the form of op( A ) to be used in 
  
             the matrix multiplication as follows:   

                TRANSA = 'N' or 'n'   op( A ) = A.   

                TRANSA = 'T' or 't'   op( A ) = A'.   

                TRANSA = 'C' or 'c'   op( A ) = conjg( A' ).   

             Unchanged on exit.   

    DIAG   - CHARACTER*1.   
             On entry, DIAG specifies whether or not A is unit triangular 
  
             as follows:   

                DIAG = 'U' or 'u'   A is assumed to be unit triangular.   

                DIAG = 'N' or 'n'   A is not assumed to be unit   
                                    triangular.   

             Unchanged on exit.   

    M      - INTEGER.   
             On entry, M specifies the number of rows of B. M must be at 
  
             least zero.   
             Unchanged on exit.   

    N      - INTEGER.   
             On entry, N specifies the number of columns of B.  N must be 
  
             at least zero.   
             Unchanged on exit.   

    ALPHA  - COMPLEX         .   
             On entry,  ALPHA specifies the scalar  alpha. When  alpha is 
  
             zero then  A is not referenced and  B need not be set before 
  
             entry.   
             Unchanged on exit.   

    A      - COMPLEX          array of DIMENSION ( LDA, k ), where k is m 
  
             when  SIDE = 'L' or 'l'  and is  n  when  SIDE = 'R' or 'r'. 
  
             Before entry  with  UPLO = 'U' or 'u',  the  leading  k by k 
  
             upper triangular part of the array  A must contain the upper 
  
             triangular matrix  and the strictly lower triangular part of 
  
             A is not referenced.   
             Before entry  with  UPLO = 'L' or 'l',  the  leading  k by k 
  
             lower triangular part of the array  A must contain the lower 
  
             triangular matrix  and the strictly upper triangular part of 
  
             A is not referenced.   
             Note that when  DIAG = 'U' or 'u',  the diagonal elements of 
  
             A  are not referenced either,  but are assumed to be  unity. 
  
             Unchanged on exit.   

    LDA    - INTEGER.   
             On entry, LDA specifies the first dimension of A as declared 
  
             in the calling (sub) program.  When  SIDE = 'L' or 'l'  then 
  
             LDA  must be at least  max( 1, m ),  when  SIDE = 'R' or 'r' 
  
             then LDA must be at least max( 1, n ).   
             Unchanged on exit.   

    B      - COMPLEX          array of DIMENSION ( LDB, n ).   
             Before entry,  the leading  m by n part of the array  B must 
  
             contain  the  right-hand  side  matrix  B,  and  on exit  is 
  
             overwritten by the solution matrix  X.   

    LDB    - INTEGER.   
             On entry, LDB specifies the first dimension of B as declared 
  
             in  the  calling  (sub)  program.   LDB  must  be  at  least 
  
             max( 1, m ).   
             Unchanged on exit.   


    Level 3 Blas routine.   

    -- Written on 8-February-1989.   
       Jack Dongarra, Argonne National Laboratory.   
       Iain Duff, AERE Harwell.   
       Jeremy Du Croz, Numerical Algorithms Group Ltd.   
       Sven Hammarling, Numerical Algorithms Group Ltd.   



       Test the input parameters.   

    
   Parameter adjustments   
       Function Body */

#define A(I,J) a[(I)-1 + ((J)-1)* ( *lda)]
#define B(I,J) b[(I)-1 + ((J)-1)* ( *ldb)]

    lside = lsame_(side, "L");
    if (lside) {
	nrowa = *m;
    } else {
	nrowa = *n;
    }
    noconj = lsame_(transa, "T");
    nounit = lsame_(diag, "N");
    upper = lsame_(uplo, "U");

    info = 0;
    if (! lside && ! lsame_(side, "R")) {
	info = 1;
    } else if (! upper && ! lsame_(uplo, "L")) {
	info = 2;
    } else if (! lsame_(transa, "N") && ! lsame_(transa, "T") 
	    && ! lsame_(transa, "C")) {
	info = 3;
    } else if (! lsame_(diag, "U") && ! lsame_(diag, "N")) {
	info = 4;
    } else if (*m < 0) {
	info = 5;
    } else if (*n < 0) {
	info = 6;
    } else if (*lda < max(1,nrowa)) {
	info = 9;
    } else if (*ldb < max(1,*m)) {
	info = 11;
    }
    if (info != 0) {
	xerbla_("CTRSM ", &info);
	return 0;
    }

/*     Quick return if possible. */

    if (*n == 0) {
	return 0;
    }

/*     And when  alpha.eq.zero. */

    if (alpha->r == 0.f && alpha->i == 0.f) {
	i__1 = *n;
	for (j = 1; j <= *n; ++j) {
	    i__2 = *m;
	    for (i = 1; i <= *m; ++i) {
		i__3 = i + j * b_dim1;
		B(i,j).r = 0.f, B(i,j).i = 0.f;
/* L10: */
	    }
/* L20: */
	}
	return 0;
    }

/*     Start the operations. */

    if (lside) {
	if (lsame_(transa, "N")) {

/*           Form  B := alpha*inv( A )*B. */

	    if (upper) {
		i__1 = *n;
		for (j = 1; j <= *n; ++j) {
		    if (alpha->r != 1.f || alpha->i != 0.f) {
			i__2 = *m;
			for (i = 1; i <= *m; ++i) {
			    i__3 = i + j * b_dim1;
			    i__4 = i + j * b_dim1;
			    q__1.r = alpha->r * B(i,j).r - alpha->i * B(i,j)
				    .i, q__1.i = alpha->r * B(i,j).i + 
				    alpha->i * B(i,j).r;
			    B(i,j).r = q__1.r, B(i,j).i = q__1.i;
/* L30: */
			}
		    }
		    for (k = *m; k >= 1; --k) {
			i__2 = k + j * b_dim1;
			if (B(k,j).r != 0.f || B(k,j).i != 0.f) {
			    if (nounit) {
				i__2 = k + j * b_dim1;
				c_div(&q__1, &B(k,j), &A(k,k));
				B(k,j).r = q__1.r, B(k,j).i = q__1.i;
			    }
			    i__2 = k - 1;
			    for (i = 1; i <= k-1; ++i) {
				i__3 = i + j * b_dim1;
				i__4 = i + j * b_dim1;
				i__5 = k + j * b_dim1;
				i__6 = i + k * a_dim1;
				q__2.r = B(k,j).r * A(i,k).r - B(k,j).i * 
					A(i,k).i, q__2.i = B(k,j).r * A(i,k).i + B(k,j).i * A(i,k).r;
				q__1.r = B(i,j).r - q__2.r, q__1.i = B(i,j)
					.i - q__2.i;
				B(i,j).r = q__1.r, B(i,j).i = q__1.i;
/* L40: */
			    }
			}
/* L50: */
		    }
/* L60: */
		}
	    } else {
		i__1 = *n;
		for (j = 1; j <= *n; ++j) {
		    if (alpha->r != 1.f || alpha->i != 0.f) {
			i__2 = *m;
			for (i = 1; i <= *m; ++i) {
			    i__3 = i + j * b_dim1;
			    i__4 = i + j * b_dim1;
			    q__1.r = alpha->r * B(i,j).r - alpha->i * B(i,j)
				    .i, q__1.i = alpha->r * B(i,j).i + 
				    alpha->i * B(i,j).r;
			    B(i,j).r = q__1.r, B(i,j).i = q__1.i;
/* L70: */
			}
		    }
		    i__2 = *m;
		    for (k = 1; k <= *m; ++k) {
			i__3 = k + j * b_dim1;
			if (B(k,j).r != 0.f || B(k,j).i != 0.f) {
			    if (nounit) {
				i__3 = k + j * b_dim1;
				c_div(&q__1, &B(k,j), &A(k,k));
				B(k,j).r = q__1.r, B(k,j).i = q__1.i;
			    }
			    i__3 = *m;
			    for (i = k + 1; i <= *m; ++i) {
				i__4 = i + j * b_dim1;
				i__5 = i + j * b_dim1;
				i__6 = k + j * b_dim1;
				i__7 = i + k * a_dim1;
				q__2.r = B(k,j).r * A(i,k).r - B(k,j).i * 
					A(i,k).i, q__2.i = B(k,j).r * A(i,k).i + B(k,j).i * A(i,k).r;
				q__1.r = B(i,j).r - q__2.r, q__1.i = B(i,j)
					.i - q__2.i;
				B(i,j).r = q__1.r, B(i,j).i = q__1.i;
/* L80: */
			    }
			}
/* L90: */
		    }
/* L100: */
		}
	    }
	} else {

/*           Form  B := alpha*inv( A' )*B   
             or    B := alpha*inv( conjg( A' ) )*B. */

	    if (upper) {
		i__1 = *n;
		for (j = 1; j <= *n; ++j) {
		    i__2 = *m;
		    for (i = 1; i <= *m; ++i) {
			i__3 = i + j * b_dim1;
			q__1.r = alpha->r * B(i,j).r - alpha->i * B(i,j).i, 
				q__1.i = alpha->r * B(i,j).i + alpha->i * B(i,j).r;
			temp.r = q__1.r, temp.i = q__1.i;
			if (noconj) {
			    i__3 = i - 1;
			    for (k = 1; k <= i-1; ++k) {
				i__4 = k + i * a_dim1;
				i__5 = k + j * b_dim1;
				q__2.r = A(k,i).r * B(k,j).r - A(k,i).i * 
					B(k,j).i, q__2.i = A(k,i).r * B(k,j).i + A(k,i).i * B(k,j).r;
				q__1.r = temp.r - q__2.r, q__1.i = temp.i - 
					q__2.i;
				temp.r = q__1.r, temp.i = q__1.i;
/* L110: */
			    }
			    if (nounit) {
				c_div(&q__1, &temp, &A(i,i));
				temp.r = q__1.r, temp.i = q__1.i;
			    }
			} else {
			    i__3 = i - 1;
			    for (k = 1; k <= i-1; ++k) {
				r_cnjg(&q__3, &A(k,i));
				i__4 = k + j * b_dim1;
				q__2.r = q__3.r * B(k,j).r - q__3.i * B(k,j)
					.i, q__2.i = q__3.r * B(k,j).i + 
					q__3.i * B(k,j).r;
				q__1.r = temp.r - q__2.r, q__1.i = temp.i - 
					q__2.i;
				temp.r = q__1.r, temp.i = q__1.i;
/* L120: */
			    }
			    if (nounit) {
				r_cnjg(&q__2, &A(i,i));
				c_div(&q__1, &temp, &q__2);
				temp.r = q__1.r, temp.i = q__1.i;
			    }
			}
			i__3 = i + j * b_dim1;
			B(i,j).r = temp.r, B(i,j).i = temp.i;
/* L130: */
		    }
/* L140: */
		}
	    } else {
		i__1 = *n;
		for (j = 1; j <= *n; ++j) {
		    for (i = *m; i >= 1; --i) {
			i__2 = i + j * b_dim1;
			q__1.r = alpha->r * B(i,j).r - alpha->i * B(i,j).i, 
				q__1.i = alpha->r * B(i,j).i + alpha->i * B(i,j).r;
			temp.r = q__1.r, temp.i = q__1.i;
			if (noconj) {
			    i__2 = *m;
			    for (k = i + 1; k <= *m; ++k) {
				i__3 = k + i * a_dim1;
				i__4 = k + j * b_dim1;
				q__2.r = A(k,i).r * B(k,j).r - A(k,i).i * 
					B(k,j).i, q__2.i = A(k,i).r * B(k,j).i + A(k,i).i * B(k,j).r;
				q__1.r = temp.r - q__2.r, q__1.i = temp.i - 
					q__2.i;
				temp.r = q__1.r, temp.i = q__1.i;
/* L150: */
			    }
			    if (nounit) {
				c_div(&q__1, &temp, &A(i,i));
				temp.r = q__1.r, temp.i = q__1.i;
			    }
			} else {
			    i__2 = *m;
			    for (k = i + 1; k <= *m; ++k) {
				r_cnjg(&q__3, &A(k,i));
				i__3 = k + j * b_dim1;
				q__2.r = q__3.r * B(k,j).r - q__3.i * B(k,j)
					.i, q__2.i = q__3.r * B(k,j).i + 
					q__3.i * B(k,j).r;
				q__1.r = temp.r - q__2.r, q__1.i = temp.i - 
					q__2.i;
				temp.r = q__1.r, temp.i = q__1.i;
/* L160: */
			    }
			    if (nounit) {
				r_cnjg(&q__2, &A(i,i));
				c_div(&q__1, &temp, &q__2);
				temp.r = q__1.r, temp.i = q__1.i;
			    }
			}
			i__2 = i + j * b_dim1;
			B(i,j).r = temp.r, B(i,j).i = temp.i;
/* L170: */
		    }
/* L180: */
		}
	    }
	}
    } else {
	if (lsame_(transa, "N")) {

/*           Form  B := alpha*B*inv( A ). */

	    if (upper) {
		i__1 = *n;
		for (j = 1; j <= *n; ++j) {
		    if (alpha->r != 1.f || alpha->i != 0.f) {
			i__2 = *m;
			for (i = 1; i <= *m; ++i) {
			    i__3 = i + j * b_dim1;
			    i__4 = i + j * b_dim1;
			    q__1.r = alpha->r * B(i,j).r - alpha->i * B(i,j)
				    .i, q__1.i = alpha->r * B(i,j).i + 
				    alpha->i * B(i,j).r;
			    B(i,j).r = q__1.r, B(i,j).i = q__1.i;
/* L190: */
			}
		    }
		    i__2 = j - 1;
		    for (k = 1; k <= j-1; ++k) {
			i__3 = k + j * a_dim1;
			if (A(k,j).r != 0.f || A(k,j).i != 0.f) {
			    i__3 = *m;
			    for (i = 1; i <= *m; ++i) {
				i__4 = i + j * b_dim1;
				i__5 = i + j * b_dim1;
				i__6 = k + j * a_dim1;
				i__7 = i + k * b_dim1;
				q__2.r = A(k,j).r * B(i,k).r - A(k,j).i * 
					B(i,k).i, q__2.i = A(k,j).r * B(i,k).i + A(k,j).i * B(i,k).r;
				q__1.r = B(i,j).r - q__2.r, q__1.i = B(i,j)
					.i - q__2.i;
				B(i,j).r = q__1.r, B(i,j).i = q__1.i;
/* L200: */
			    }
			}
/* L210: */
		    }
		    if (nounit) {
			c_div(&q__1, &c_b1, &A(j,j));
			temp.r = q__1.r, temp.i = q__1.i;
			i__2 = *m;
			for (i = 1; i <= *m; ++i) {
			    i__3 = i + j * b_dim1;
			    i__4 = i + j * b_dim1;
			    q__1.r = temp.r * B(i,j).r - temp.i * B(i,j).i, 
				    q__1.i = temp.r * B(i,j).i + temp.i * B(i,j).r;
			    B(i,j).r = q__1.r, B(i,j).i = q__1.i;
/* L220: */
			}
		    }
/* L230: */
		}
	    } else {
		for (j = *n; j >= 1; --j) {
		    if (alpha->r != 1.f || alpha->i != 0.f) {
			i__1 = *m;
			for (i = 1; i <= *m; ++i) {
			    i__2 = i + j * b_dim1;
			    i__3 = i + j * b_dim1;
			    q__1.r = alpha->r * B(i,j).r - alpha->i * B(i,j)
				    .i, q__1.i = alpha->r * B(i,j).i + 
				    alpha->i * B(i,j).r;
			    B(i,j).r = q__1.r, B(i,j).i = q__1.i;
/* L240: */
			}
		    }
		    i__1 = *n;
		    for (k = j + 1; k <= *n; ++k) {
			i__2 = k + j * a_dim1;
			if (A(k,j).r != 0.f || A(k,j).i != 0.f) {
			    i__2 = *m;
			    for (i = 1; i <= *m; ++i) {
				i__3 = i + j * b_dim1;
				i__4 = i + j * b_dim1;
				i__5 = k + j * a_dim1;
				i__6 = i + k * b_dim1;
				q__2.r = A(k,j).r * B(i,k).r - A(k,j).i * 
					B(i,k).i, q__2.i = A(k,j).r * B(i,k).i + A(k,j).i * B(i,k).r;
				q__1.r = B(i,j).r - q__2.r, q__1.i = B(i,j)
					.i - q__2.i;
				B(i,j).r = q__1.r, B(i,j).i = q__1.i;
/* L250: */
			    }
			}
/* L260: */
		    }
		    if (nounit) {
			c_div(&q__1, &c_b1, &A(j,j));
			temp.r = q__1.r, temp.i = q__1.i;
			i__1 = *m;
			for (i = 1; i <= *m; ++i) {
			    i__2 = i + j * b_dim1;
			    i__3 = i + j * b_dim1;
			    q__1.r = temp.r * B(i,j).r - temp.i * B(i,j).i, 
				    q__1.i = temp.r * B(i,j).i + temp.i * B(i,j).r;
			    B(i,j).r = q__1.r, B(i,j).i = q__1.i;
/* L270: */
			}
		    }
/* L280: */
		}
	    }
	} else {

/*           Form  B := alpha*B*inv( A' )   
             or    B := alpha*B*inv( conjg( A' ) ). */

	    if (upper) {
		for (k = *n; k >= 1; --k) {
		    if (nounit) {
			if (noconj) {
			    c_div(&q__1, &c_b1, &A(k,k));
			    temp.r = q__1.r, temp.i = q__1.i;
			} else {
			    r_cnjg(&q__2, &A(k,k));
			    c_div(&q__1, &c_b1, &q__2);
			    temp.r = q__1.r, temp.i = q__1.i;
			}
			i__1 = *m;
			for (i = 1; i <= *m; ++i) {
			    i__2 = i + k * b_dim1;
			    i__3 = i + k * b_dim1;
			    q__1.r = temp.r * B(i,k).r - temp.i * B(i,k).i, 
				    q__1.i = temp.r * B(i,k).i + temp.i * B(i,k).r;
			    B(i,k).r = q__1.r, B(i,k).i = q__1.i;
/* L290: */
			}
		    }
		    i__1 = k - 1;
		    for (j = 1; j <= k-1; ++j) {
			i__2 = j + k * a_dim1;
			if (A(j,k).r != 0.f || A(j,k).i != 0.f) {
			    if (noconj) {
				i__2 = j + k * a_dim1;
				temp.r = A(j,k).r, temp.i = A(j,k).i;
			    } else {
				r_cnjg(&q__1, &A(j,k));
				temp.r = q__1.r, temp.i = q__1.i;
			    }
			    i__2 = *m;
			    for (i = 1; i <= *m; ++i) {
				i__3 = i + j * b_dim1;
				i__4 = i + j * b_dim1;
				i__5 = i + k * b_dim1;
				q__2.r = temp.r * B(i,k).r - temp.i * B(i,k)
					.i, q__2.i = temp.r * B(i,k).i + 
					temp.i * B(i,k).r;
				q__1.r = B(i,j).r - q__2.r, q__1.i = B(i,j)
					.i - q__2.i;
				B(i,j).r = q__1.r, B(i,j).i = q__1.i;
/* L300: */
			    }
			}
/* L310: */
		    }
		    if (alpha->r != 1.f || alpha->i != 0.f) {
			i__1 = *m;
			for (i = 1; i <= *m; ++i) {
			    i__2 = i + k * b_dim1;
			    i__3 = i + k * b_dim1;
			    q__1.r = alpha->r * B(i,k).r - alpha->i * B(i,k)
				    .i, q__1.i = alpha->r * B(i,k).i + 
				    alpha->i * B(i,k).r;
			    B(i,k).r = q__1.r, B(i,k).i = q__1.i;
/* L320: */
			}
		    }
/* L330: */
		}
	    } else {
		i__1 = *n;
		for (k = 1; k <= *n; ++k) {
		    if (nounit) {
			if (noconj) {
			    c_div(&q__1, &c_b1, &A(k,k));
			    temp.r = q__1.r, temp.i = q__1.i;
			} else {
			    r_cnjg(&q__2, &A(k,k));
			    c_div(&q__1, &c_b1, &q__2);
			    temp.r = q__1.r, temp.i = q__1.i;
			}
			i__2 = *m;
			for (i = 1; i <= *m; ++i) {
			    i__3 = i + k * b_dim1;
			    i__4 = i + k * b_dim1;
			    q__1.r = temp.r * B(i,k).r - temp.i * B(i,k).i, 
				    q__1.i = temp.r * B(i,k).i + temp.i * B(i,k).r;
			    B(i,k).r = q__1.r, B(i,k).i = q__1.i;
/* L340: */
			}
		    }
		    i__2 = *n;
		    for (j = k + 1; j <= *n; ++j) {
			i__3 = j + k * a_dim1;
			if (A(j,k).r != 0.f || A(j,k).i != 0.f) {
			    if (noconj) {
				i__3 = j + k * a_dim1;
				temp.r = A(j,k).r, temp.i = A(j,k).i;
			    } else {
				r_cnjg(&q__1, &A(j,k));
				temp.r = q__1.r, temp.i = q__1.i;
			    }
			    i__3 = *m;
			    for (i = 1; i <= *m; ++i) {
				i__4 = i + j * b_dim1;
				i__5 = i + j * b_dim1;
				i__6 = i + k * b_dim1;
				q__2.r = temp.r * B(i,k).r - temp.i * B(i,k)
					.i, q__2.i = temp.r * B(i,k).i + 
					temp.i * B(i,k).r;
				q__1.r = B(i,j).r - q__2.r, q__1.i = B(i,j)
					.i - q__2.i;
				B(i,j).r = q__1.r, B(i,j).i = q__1.i;
/* L350: */
			    }
			}
/* L360: */
		    }
		    if (alpha->r != 1.f || alpha->i != 0.f) {
			i__2 = *m;
			for (i = 1; i <= *m; ++i) {
			    i__3 = i + k * b_dim1;
			    i__4 = i + k * b_dim1;
			    q__1.r = alpha->r * B(i,k).r - alpha->i * B(i,k)
				    .i, q__1.i = alpha->r * B(i,k).i + 
				    alpha->i * B(i,k).r;
			    B(i,k).r = q__1.r, B(i,k).i = q__1.i;
/* L370: */
			}
		    }
/* L380: */
		}
	    }
	}
    }

    return 0;

/*     End of CTRSM . */

} /* ctrsm_ */

