#include "f2c.h"

/* Subroutine */ int slasyf_(char *uplo, integer *n, integer *nb, integer *kb,
	 real *a, integer *lda, integer *ipiv, real *w, integer *ldw, integer 
	*info)
{
/*  -- LAPACK routine (version 2.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       February 29, 1992   


    Purpose   
    =======   

    SLASYF computes a partial factorization of a real symmetric matrix A 
  
    using the Bunch-Kaufman diagonal pivoting method. The partial   
    factorization has the form:   

    A  =  ( I  U12 ) ( A11  0  ) (  I    0   )  if UPLO = 'U', or:   
          ( 0  U22 ) (  0   D  ) ( U12' U22' )   

    A  =  ( L11  0 ) (  D   0  ) ( L11' L21' )  if UPLO = 'L'   
          ( L21  I ) (  0  A22 ) (  0    I   )   

    where the order of D is at most NB. The actual order is returned in   
    the argument KB, and is either NB or NB-1, or N if N <= NB.   

    SLASYF is an auxiliary routine called by SSYTRF. It uses blocked code 
  
    (calling Level 3 BLAS) to update the submatrix A11 (if UPLO = 'U') or 
  
    A22 (if UPLO = 'L').   

    Arguments   
    =========   

    UPLO    (input) CHARACTER*1   
            Specifies whether the upper or lower triangular part of the   
            symmetric matrix A is stored:   
            = 'U':  Upper triangular   
            = 'L':  Lower triangular   

    N       (input) INTEGER   
            The order of the matrix A.  N >= 0.   

    NB      (input) INTEGER   
            The maximum number of columns of the matrix A that should be 
  
            factored.  NB should be at least 2 to allow for 2-by-2 pivot 
  
            blocks.   

    KB      (output) INTEGER   
            The number of columns of A that were actually factored.   
            KB is either NB-1 or NB, or N if N <= NB.   

    A       (input/output) REAL array, dimension (LDA,N)   
            On entry, the symmetric matrix A.  If UPLO = 'U', the leading 
  
            n-by-n upper triangular part of A contains the upper   
            triangular part of the matrix A, and the strictly lower   
            triangular part of A is not referenced.  If UPLO = 'L', the   
            leading n-by-n lower triangular part of A contains the lower 
  
            triangular part of the matrix A, and the strictly upper   
            triangular part of A is not referenced.   
            On exit, A contains details of the partial factorization.   

    LDA     (input) INTEGER   
            The leading dimension of the array A.  LDA >= max(1,N).   

    IPIV    (output) INTEGER array, dimension (N)   
            Details of the interchanges and the block structure of D.   
            If UPLO = 'U', only the last KB elements of IPIV are set;   
            if UPLO = 'L', only the first KB elements are set.   

            If IPIV(k) > 0, then rows and columns k and IPIV(k) were   
            interchanged and D(k,k) is a 1-by-1 diagonal block.   
            If UPLO = 'U' and IPIV(k) = IPIV(k-1) < 0, then rows and   
            columns k-1 and -IPIV(k) were interchanged and D(k-1:k,k-1:k) 
  
            is a 2-by-2 diagonal block.  If UPLO = 'L' and IPIV(k) =   
            IPIV(k+1) < 0, then rows and columns k+1 and -IPIV(k) were   
            interchanged and D(k:k+1,k:k+1) is a 2-by-2 diagonal block.   

    W       (workspace) REAL array, dimension (LDW,NB)   

    LDW     (input) INTEGER   
            The leading dimension of the array W.  LDW >= max(1,N).   

    INFO    (output) INTEGER   
            = 0: successful exit   
            > 0: if INFO = k, D(k,k) is exactly zero.  The factorization 
  
                 has been completed, but the block diagonal matrix D is   
                 exactly singular.   

    ===================================================================== 
  


    
   Parameter adjustments   
       Function Body */
    /* Table of constant values */
    static integer c__1 = 1;
    static real c_b8 = -1.f;
    static real c_b9 = 1.f;
    
    /* System generated locals */
    integer a_dim1, a_offset, w_dim1, w_offset, i__1, i__2, i__3, i__4, i__5;
    real r__1, r__2, r__3;
    /* Builtin functions */
    double sqrt(doublereal);
    /* Local variables */
    static integer imax, jmax, j, k;
    static real t, alpha;
    extern logical lsame_(char *, char *);
    extern /* Subroutine */ int sscal_(integer *, real *, real *, integer *), 
	    sgemm_(char *, char *, integer *, integer *, integer *, real *, 
	    real *, integer *, real *, integer *, real *, real *, integer *), sgemv_(char *, integer *, integer *, real *, 
	    real *, integer *, real *, integer *, real *, real *, integer *);
    static integer kstep;
    extern /* Subroutine */ int scopy_(integer *, real *, integer *, real *, 
	    integer *), sswap_(integer *, real *, integer *, real *, integer *
	    );
    static real r1, d11, d21, d22;
    static integer jb, jj, kk, jp, kp;
    static real absakk;
    static integer kw;
    extern integer isamax_(integer *, real *, integer *);
    static real colmax, rowmax;
    static integer kkw;



#define IPIV(I) ipiv[(I)-1]

#define A(I,J) a[(I)-1 + ((J)-1)* ( *lda)]
#define W(I,J) w[(I)-1 + ((J)-1)* ( *ldw)]

    *info = 0;

/*     Initialize ALPHA for use in choosing pivot block size. */

    alpha = (sqrt(17.f) + 1.f) / 8.f;

    if (lsame_(uplo, "U")) {

/*        Factorize the trailing columns of A using the upper triangle
   
          of A and working backwards, and compute the matrix W = U12*D
   
          for use in updating A11   

          K is the main loop index, decreasing from N in steps of 1 or
 2   

          KW is the column of W which corresponds to column K of A */

	k = *n;
L10:
	kw = *nb + k - *n;

/*        Exit from loop */

	if (k <= *n - *nb + 1 && *nb < *n || k < 1) {
	    goto L30;
	}

/*        Copy column K of A to column KW of W and update it */

	scopy_(&k, &A(1,k), &c__1, &W(1,kw), &c__1);
	if (k < *n) {
	    i__1 = *n - k;
	    sgemv_("No transpose", &k, &i__1, &c_b8, &A(1,k+1),
		     lda, &W(k,kw+1), ldw, &c_b9, &W(1,kw), &c__1);
	}

	kstep = 1;

/*        Determine rows and columns to be interchanged and whether   
          a 1-by-1 or 2-by-2 pivot block will be used */

	absakk = (r__1 = W(k,kw), dabs(r__1));

/*        IMAX is the row-index of the largest off-diagonal element in
   
          column K, and COLMAX is its absolute value */

	if (k > 1) {
	    i__1 = k - 1;
	    imax = isamax_(&i__1, &W(1,kw), &c__1);
	    colmax = (r__1 = W(imax,kw), dabs(r__1));
	} else {
	    colmax = 0.f;
	}

	if (dmax(absakk,colmax) == 0.f) {

/*           Column K is zero: set INFO and continue */

	    if (*info == 0) {
		*info = k;
	    }
	    kp = k;
	} else {
	    if (absakk >= alpha * colmax) {

/*              no interchange, use 1-by-1 pivot block */

		kp = k;
	    } else {

/*              Copy column IMAX to column KW-1 of W and updat
e it */

		scopy_(&imax, &A(1,imax), &c__1, &W(1,kw-1), &c__1);
		i__1 = k - imax;
		scopy_(&i__1, &A(imax,imax+1), lda, &W(imax+1,kw-1), &c__1);
		if (k < *n) {
		    i__1 = *n - k;
		    sgemv_("No transpose", &k, &i__1, &c_b8, &A(1,k+1), lda, &W(imax,kw+1), 
			    ldw, &c_b9, &W(1,kw-1), &c__1)
			    ;
		}

/*              JMAX is the column-index of the largest off-di
agonal   
                element in row IMAX, and ROWMAX is its absolut
e value */

		i__1 = k - imax;
		jmax = imax + isamax_(&i__1, &W(imax+1,kw-1),
			 &c__1);
		rowmax = (r__1 = W(jmax,kw-1), dabs(r__1));
		if (imax > 1) {
		    i__1 = imax - 1;
		    jmax = isamax_(&i__1, &W(1,kw-1), &c__1);
/* Computing MAX */
		    r__2 = rowmax, r__3 = (r__1 = W(jmax,kw-1),
			     dabs(r__1));
		    rowmax = dmax(r__2,r__3);
		}

		if (absakk >= alpha * colmax * (colmax / rowmax)) {

/*                 no interchange, use 1-by-1 pivot block 
*/

		    kp = k;
		} else if ((r__1 = W(imax,kw-1), dabs(r__1)) >=
			 alpha * rowmax) {

/*                 interchange rows and columns K and IMAX
, use 1-by-1   
                   pivot block */

		    kp = imax;

/*                 copy column KW-1 of W to column KW */

		    scopy_(&k, &W(1,kw-1), &c__1, &W(1,kw), &c__1);
		} else {

/*                 interchange rows and columns K-1 and IM
AX, use 2-by-2   
                   pivot block */

		    kp = imax;
		    kstep = 2;
		}
	    }

	    kk = k - kstep + 1;
	    kkw = *nb + kk - *n;

/*           Updated column KP is already stored in column KKW of 
W */

	    if (kp != kk) {

/*              Copy non-updated column KK to column KP */

		A(kp,k) = A(kk,k);
		i__1 = k - 1 - kp;
		scopy_(&i__1, &A(kp+1,kk), &c__1, &A(kp,kp+1), lda);
		scopy_(&kp, &A(1,kk), &c__1, &A(1,kp), &
			c__1);

/*              Interchange rows KK and KP in last KK columns 
of A and W */

		i__1 = *n - kk + 1;
		sswap_(&i__1, &A(kk,kk), lda, &A(kp,kk),
			 lda);
		i__1 = *n - kk + 1;
		sswap_(&i__1, &W(kk,kkw), ldw, &W(kp,kkw), ldw);
	    }

	    if (kstep == 1) {

/*              1-by-1 pivot block D(k): column KW of W now ho
lds   

                W(k) = U(k)*D(k)   

                where U(k) is the k-th column of U   

                Store U(k) in column k of A */

		scopy_(&k, &W(1,kw), &c__1, &A(1,k), &
			c__1);
		r1 = 1.f / A(k,k);
		i__1 = k - 1;
		sscal_(&i__1, &r1, &A(1,k), &c__1);
	    } else {

/*              2-by-2 pivot block D(k): columns KW and KW-1 o
f W now   
                hold   

                ( W(k-1) W(k) ) = ( U(k-1) U(k) )*D(k)   

                where U(k) and U(k-1) are the k-th and (k-1)-t
h columns   
                of U */

		if (k > 2) {

/*                 Store U(k) and U(k-1) in columns k and 
k-1 of A */

		    d21 = W(k-1,kw);
		    d11 = W(k,kw) / d21;
		    d22 = W(k-1,kw-1) / d21;
		    t = 1.f / (d11 * d22 - 1.f);
		    d21 = t / d21;
		    i__1 = k - 2;
		    for (j = 1; j <= k-2; ++j) {
			A(j,k-1) = d21 * (d11 * W(j,kw-1) - W(j,kw));
			A(j,k) = d21 * (d22 * W(j,kw) - 
				W(j,kw-1));
/* L20: */
		    }
		}

/*              Copy D(k) to A */

		A(k-1,k-1) = W(k-1,kw-1);
		A(k-1,k) = W(k-1,kw);
		A(k,k) = W(k,kw);
	    }
	}

/*        Store details of the interchanges in IPIV */

	if (kstep == 1) {
	    IPIV(k) = kp;
	} else {
	    IPIV(k) = -kp;
	    IPIV(k - 1) = -kp;
	}

/*        Decrease K and return to the start of the main loop */

	k -= kstep;
	goto L10;

L30:

/*        Update the upper triangle of A11 (= A(1:k,1:k)) as   

          A11 := A11 - U12*D*U12' = A11 - U12*W'   

          computing blocks of NB columns at a time */

	i__1 = -(*nb);
	for (j = (k - 1) / *nb * *nb + 1; i__1 < 0 ? j >= 1 : j <= 1; j += 
		i__1) {
/* Computing MIN */
	    i__2 = *nb, i__3 = k - j + 1;
	    jb = min(i__2,i__3);

/*           Update the upper triangle of the diagonal block */

	    i__2 = j + jb - 1;
	    for (jj = j; jj <= j+jb-1; ++jj) {
		i__3 = jj - j + 1;
		i__4 = *n - k;
		sgemv_("No transpose", &i__3, &i__4, &c_b8, &A(j,k+1), lda, &W(jj,kw+1), ldw, &c_b9, 
			&A(j,jj), &c__1);
/* L40: */
	    }

/*           Update the rectangular superdiagonal block */

	    i__2 = j - 1;
	    i__3 = *n - k;
	    sgemm_("No transpose", "Transpose", &i__2, &jb, &i__3, &c_b8, &A(1,k+1), lda, &W(j,kw+1), ldw,
		     &c_b9, &A(1,j), lda);
/* L50: */
	}

/*        Put U12 in standard form by partially undoing the interchang
es   
          in columns k+1:n */

	j = k + 1;
L60:
	jj = j;
	jp = IPIV(j);
	if (jp < 0) {
	    jp = -jp;
	    ++j;
	}
	++j;
	if (jp != jj && j <= *n) {
	    i__1 = *n - j + 1;
	    sswap_(&i__1, &A(jp,j), lda, &A(jj,j), lda);
	}
	if (j <= *n) {
	    goto L60;
	}

/*        Set KB to the number of columns factorized */

	*kb = *n - k;

    } else {

/*        Factorize the leading columns of A using the lower triangle 
  
          of A and working forwards, and compute the matrix W = L21*D 
  
          for use in updating A22   

          K is the main loop index, increasing from 1 in steps of 1 or
 2 */

	k = 1;
L70:

/*        Exit from loop */

	if (k >= *nb && *nb < *n || k > *n) {
	    goto L90;
	}

/*        Copy column K of A to column K of W and update it */

	i__1 = *n - k + 1;
	scopy_(&i__1, &A(k,k), &c__1, &W(k,k), &c__1);
	i__1 = *n - k + 1;
	i__2 = k - 1;
	sgemv_("No transpose", &i__1, &i__2, &c_b8, &A(k,1), lda, &W(k,1), ldw, &c_b9, &W(k,k), &c__1);

	kstep = 1;

/*        Determine rows and columns to be interchanged and whether   
          a 1-by-1 or 2-by-2 pivot block will be used */

	absakk = (r__1 = W(k,k), dabs(r__1));

/*        IMAX is the row-index of the largest off-diagonal element in
   
          column K, and COLMAX is its absolute value */

	if (k < *n) {
	    i__1 = *n - k;
	    imax = k + isamax_(&i__1, &W(k+1,k), &c__1);
	    colmax = (r__1 = W(imax,k), dabs(r__1));
	} else {
	    colmax = 0.f;
	}

	if (dmax(absakk,colmax) == 0.f) {

/*           Column K is zero: set INFO and continue */

	    if (*info == 0) {
		*info = k;
	    }
	    kp = k;
	} else {
	    if (absakk >= alpha * colmax) {

/*              no interchange, use 1-by-1 pivot block */

		kp = k;
	    } else {

/*              Copy column IMAX to column K+1 of W and update
 it */

		i__1 = imax - k;
		scopy_(&i__1, &A(imax,k), lda, &W(k,k+1), &c__1);
		i__1 = *n - imax + 1;
		scopy_(&i__1, &A(imax,imax), &c__1, &W(imax,k+1), &c__1);
		i__1 = *n - k + 1;
		i__2 = k - 1;
		sgemv_("No transpose", &i__1, &i__2, &c_b8, &A(k,1), 
			lda, &W(imax,1), ldw, &c_b9, &W(k,k+1), &c__1);

/*              JMAX is the column-index of the largest off-di
agonal   
                element in row IMAX, and ROWMAX is its absolut
e value */

		i__1 = imax - k;
		jmax = k - 1 + isamax_(&i__1, &W(k,k+1), &c__1)
			;
		rowmax = (r__1 = W(jmax,k+1), dabs(r__1));
		if (imax < *n) {
		    i__1 = *n - imax;
		    jmax = imax + isamax_(&i__1, &W(imax+1,k+1), &c__1);
/* Computing MAX */
		    r__2 = rowmax, r__3 = (r__1 = W(jmax,k+1), 
			    dabs(r__1));
		    rowmax = dmax(r__2,r__3);
		}

		if (absakk >= alpha * colmax * (colmax / rowmax)) {

/*                 no interchange, use 1-by-1 pivot block 
*/

		    kp = k;
		} else if ((r__1 = W(imax,k+1), dabs(r__1)) >= 
			alpha * rowmax) {

/*                 interchange rows and columns K and IMAX
, use 1-by-1   
                   pivot block */

		    kp = imax;

/*                 copy column K+1 of W to column K */

		    i__1 = *n - k + 1;
		    scopy_(&i__1, &W(k,k+1), &c__1, &W(k,k), &c__1);
		} else {

/*                 interchange rows and columns K+1 and IM
AX, use 2-by-2   
                   pivot block */

		    kp = imax;
		    kstep = 2;
		}
	    }

	    kk = k + kstep - 1;

/*           Updated column KP is already stored in column KK of W
 */

	    if (kp != kk) {

/*              Copy non-updated column KK to column KP */

		A(kp,k) = A(kk,k);
		i__1 = kp - k - 1;
		scopy_(&i__1, &A(k+1,kk), &c__1, &A(kp,k+1), lda);
		i__1 = *n - kp + 1;
		scopy_(&i__1, &A(kp,kk), &c__1, &A(kp,kp), &c__1);

/*              Interchange rows KK and KP in first KK columns
 of A and W */

		sswap_(&kk, &A(kk,1), lda, &A(kp,1), lda);
		sswap_(&kk, &W(kk,1), ldw, &W(kp,1), ldw);
	    }

	    if (kstep == 1) {

/*              1-by-1 pivot block D(k): column k of W now hol
ds   

                W(k) = L(k)*D(k)   

                where L(k) is the k-th column of L   

                Store L(k) in column k of A */

		i__1 = *n - k + 1;
		scopy_(&i__1, &W(k,k), &c__1, &A(k,k), &
			c__1);
		if (k < *n) {
		    r1 = 1.f / A(k,k);
		    i__1 = *n - k;
		    sscal_(&i__1, &r1, &A(k+1,k), &c__1);
		}
	    } else {

/*              2-by-2 pivot block D(k): columns k and k+1 of 
W now hold   

                ( W(k) W(k+1) ) = ( L(k) L(k+1) )*D(k)   

                where L(k) and L(k+1) are the k-th and (k+1)-t
h columns   
                of L */

		if (k < *n - 1) {

/*                 Store L(k) and L(k+1) in columns k and 
k+1 of A */

		    d21 = W(k+1,k);
		    d11 = W(k+1,k+1) / d21;
		    d22 = W(k,k) / d21;
		    t = 1.f / (d11 * d22 - 1.f);
		    d21 = t / d21;
		    i__1 = *n;
		    for (j = k + 2; j <= *n; ++j) {
			A(j,k) = d21 * (d11 * W(j,k) - 
				W(j,k+1));
			A(j,k+1) = d21 * (d22 * W(j,k+1) - W(j,k));
/* L80: */
		    }
		}

/*              Copy D(k) to A */

		A(k,k) = W(k,k);
		A(k+1,k) = W(k+1,k);
		A(k+1,k+1) = W(k+1,k+1);
	    }
	}

/*        Store details of the interchanges in IPIV */

	if (kstep == 1) {
	    IPIV(k) = kp;
	} else {
	    IPIV(k) = -kp;
	    IPIV(k + 1) = -kp;
	}

/*        Increase K and return to the start of the main loop */

	k += kstep;
	goto L70;

L90:

/*        Update the lower triangle of A22 (= A(k:n,k:n)) as   

          A22 := A22 - L21*D*L21' = A22 - L21*W'   

          computing blocks of NB columns at a time */

	i__1 = *n;
	i__2 = *nb;
	for (j = k; *nb < 0 ? j >= *n : j <= *n; j += *nb) {
/* Computing MIN */
	    i__3 = *nb, i__4 = *n - j + 1;
	    jb = min(i__3,i__4);

/*           Update the lower triangle of the diagonal block */

	    i__3 = j + jb - 1;
	    for (jj = j; jj <= j+jb-1; ++jj) {
		i__4 = j + jb - jj;
		i__5 = k - 1;
		sgemv_("No transpose", &i__4, &i__5, &c_b8, &A(jj,1), 
			lda, &W(jj,1), ldw, &c_b9, &A(jj,jj)
			, &c__1);
/* L100: */
	    }

/*           Update the rectangular subdiagonal block */

	    if (j + jb <= *n) {
		i__3 = *n - j - jb + 1;
		i__4 = k - 1;
		sgemm_("No transpose", "Transpose", &i__3, &jb, &i__4, &c_b8, 
			&A(j+jb,1), lda, &W(j,1), ldw, &c_b9, 
			&A(j+jb,j), lda);
	    }
/* L110: */
	}

/*        Put L21 in standard form by partially undoing the interchang
es   
          in columns 1:k-1 */

	j = k - 1;
L120:
	jj = j;
	jp = IPIV(j);
	if (jp < 0) {
	    jp = -jp;
	    --j;
	}
	--j;
	if (jp != jj && j >= 1) {
	    sswap_(&j, &A(jp,1), lda, &A(jj,1), lda);
	}
	if (j >= 1) {
	    goto L120;
	}

/*        Set KB to the number of columns factorized */

	*kb = k - 1;

    }
    return 0;

/*     End of SLASYF */

} /* slasyf_ */

