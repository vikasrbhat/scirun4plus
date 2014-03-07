#include "f2c.h"

/* Subroutine */ int cgtsv_(integer *n, integer *nrhs, complex *dl, complex *
	d, complex *du, complex *b, integer *ldb, integer *info)
{
/*  -- LAPACK routine (version 2.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    CGTSV  solves the equation   

       A*X = B,   

    where A is an N-by-N tridiagonal matrix, by Gaussian elimination with 
  
    partial pivoting.   

    Note that the equation  A'*X = B  may be solved by interchanging the 
  
    order of the arguments DU and DL.   

    Arguments   
    =========   

    N       (input) INTEGER   
            The order of the matrix A.  N >= 0.   

    NRHS    (input) INTEGER   
            The number of right hand sides, i.e., the number of columns   
            of the matrix B.  NRHS >= 0.   

    DL      (input/output) COMPLEX array, dimension (N-1)   
            On entry, DL must contain the (n-1) subdiagonal elements of   
            A.   
            On exit, DL is overwritten by the (n-2) elements of the   
            second superdiagonal of the upper triangular matrix U from   
            the LU factorization of A, in DL(1), ..., DL(n-2).   

    D       (input/output) COMPLEX array, dimension (N)   
            On entry, D must contain the diagonal elements of A.   
            On exit, D is overwritten by the n diagonal elements of U.   

    DU      (input/output) COMPLEX array, dimension (N-1)   
            On entry, DU must contain the (n-1) superdiagonal elements   
            of A.   
            On exit, DU is overwritten by the (n-1) elements of the first 
  
            superdiagonal of U.   

    B       (input/output) COMPLEX array, dimension (LDB,NRHS)   
            On entry, the N-by-NRHS right hand side matrix B.   
            On exit, if INFO = 0, the N-by-NRHS solution matrix X.   

    LDB     (input) INTEGER   
            The leading dimension of the array B.  LDB >= max(1,N).   

    INFO    (output) INTEGER   
            = 0:  successful exit   
            < 0:  if INFO = -i, the i-th argument had an illegal value   
            > 0:  if INFO = i, U(i,i) is exactly zero, and the solution   
                  has not been computed.  The factorization has not been 
  
                  completed unless i = N.   

    ===================================================================== 
  


    
   Parameter adjustments   
       Function Body */
    /* System generated locals */
    integer b_dim1, b_offset, i__1, i__2, i__3, i__4, i__5, i__6, i__7;
    real r__1, r__2, r__3, r__4;
    complex q__1, q__2, q__3, q__4, q__5;
    /* Builtin functions */
    double r_imag(complex *);
    void c_div(complex *, complex *, complex *);
    /* Local variables */
    static complex temp, mult;
    static integer j, k;
    extern /* Subroutine */ int xerbla_(char *, integer *);


#define DL(I) dl[(I)-1]
#define D(I) d[(I)-1]
#define DU(I) du[(I)-1]

#define B(I,J) b[(I)-1 + ((J)-1)* ( *ldb)]

    *info = 0;
    if (*n < 0) {
	*info = -1;
    } else if (*nrhs < 0) {
	*info = -2;
    } else if (*ldb < max(1,*n)) {
	*info = -7;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("CGTSV ", &i__1);
	return 0;
    }

    if (*n == 0) {
	return 0;
    }

    i__1 = *n - 1;
    for (k = 1; k <= *n-1; ++k) {
	i__2 = k;
	if (DL(k).r == 0.f && DL(k).i == 0.f) {

/*           Subdiagonal is zero, no elimination is required. */

	    i__2 = k;
	    if (D(k).r == 0.f && D(k).i == 0.f) {

/*              Diagonal is zero: set INFO = K and return; a u
nique   
                solution can not be found. */

		*info = k;
		return 0;
	    }
	} else /* if(complicated condition) */ {
	    i__2 = k;
	    i__3 = k;
	    if ((r__1 = D(k).r, dabs(r__1)) + (r__2 = r_imag(&D(k)), dabs(
		    r__2)) >= (r__3 = DL(k).r, dabs(r__3)) + (r__4 = 
		    r_imag(&DL(k)), dabs(r__4))) {

/*           No row interchange required */

		c_div(&q__1, &DL(k), &D(k));
		mult.r = q__1.r, mult.i = q__1.i;
		i__2 = k + 1;
		i__3 = k + 1;
		i__4 = k;
		q__2.r = mult.r * DU(k).r - mult.i * DU(k).i, q__2.i = 
			mult.r * DU(k).i + mult.i * DU(k).r;
		q__1.r = D(k+1).r - q__2.r, q__1.i = D(k+1).i - q__2.i;
		D(k+1).r = q__1.r, D(k+1).i = q__1.i;
		i__2 = *nrhs;
		for (j = 1; j <= *nrhs; ++j) {
		    i__3 = k + 1 + j * b_dim1;
		    i__4 = k + 1 + j * b_dim1;
		    i__5 = k + j * b_dim1;
		    q__2.r = mult.r * B(k,j).r - mult.i * B(k,j).i, q__2.i =
			     mult.r * B(k,j).i + mult.i * B(k,j).r;
		    q__1.r = B(k+1,j).r - q__2.r, q__1.i = B(k+1,j).i - q__2.i;
		    B(k+1,j).r = q__1.r, B(k+1,j).i = q__1.i;
/* L10: */
		}
		if (k < *n - 1) {
		    i__2 = k;
		    DL(k).r = 0.f, DL(k).i = 0.f;
		}
	    } else {

/*           Interchange rows K and K+1 */

		c_div(&q__1, &D(k), &DL(k));
		mult.r = q__1.r, mult.i = q__1.i;
		i__2 = k;
		i__3 = k;
		D(k).r = DL(k).r, D(k).i = DL(k).i;
		i__2 = k + 1;
		temp.r = D(k+1).r, temp.i = D(k+1).i;
		i__2 = k + 1;
		i__3 = k;
		q__2.r = mult.r * temp.r - mult.i * temp.i, q__2.i = mult.r * 
			temp.i + mult.i * temp.r;
		q__1.r = DU(k).r - q__2.r, q__1.i = DU(k).i - q__2.i;
		D(k+1).r = q__1.r, D(k+1).i = q__1.i;
		if (k < *n - 1) {
		    i__2 = k;
		    i__3 = k + 1;
		    DL(k).r = DU(k+1).r, DL(k).i = DU(k+1).i;
		    i__2 = k + 1;
		    q__2.r = -(doublereal)mult.r, q__2.i = -(doublereal)
			    mult.i;
		    i__3 = k;
		    q__1.r = q__2.r * DL(k).r - q__2.i * DL(k).i, 
			    q__1.i = q__2.r * DL(k).i + q__2.i * DL(k)
			    .r;
		    DU(k+1).r = q__1.r, DU(k+1).i = q__1.i;
		}
		i__2 = k;
		DU(k).r = temp.r, DU(k).i = temp.i;
		i__2 = *nrhs;
		for (j = 1; j <= *nrhs; ++j) {
		    i__3 = k + j * b_dim1;
		    temp.r = B(k,j).r, temp.i = B(k,j).i;
		    i__3 = k + j * b_dim1;
		    i__4 = k + 1 + j * b_dim1;
		    B(k,j).r = B(k+1,j).r, B(k,j).i = B(k+1,j).i;
		    i__3 = k + 1 + j * b_dim1;
		    i__4 = k + 1 + j * b_dim1;
		    q__2.r = mult.r * B(k+1,j).r - mult.i * B(k+1,j).i, q__2.i =
			     mult.r * B(k+1,j).i + mult.i * B(k+1,j).r;
		    q__1.r = temp.r - q__2.r, q__1.i = temp.i - q__2.i;
		    B(k+1,j).r = q__1.r, B(k+1,j).i = q__1.i;
/* L20: */
		}
	    }
	}
/* L30: */
    }
    i__1 = *n;
    if (D(*n).r == 0.f && D(*n).i == 0.f) {
	*info = *n;
	return 0;
    }

/*     Back solve with the matrix U from the factorization. */

    i__1 = *nrhs;
    for (j = 1; j <= *nrhs; ++j) {
	i__2 = *n + j * b_dim1;
	c_div(&q__1, &B(*n,j), &D(*n));
	B(*n,j).r = q__1.r, B(*n,j).i = q__1.i;
	if (*n > 1) {
	    i__2 = *n - 1 + j * b_dim1;
	    i__3 = *n - 1 + j * b_dim1;
	    i__4 = *n - 1;
	    i__5 = *n + j * b_dim1;
	    q__3.r = DU(*n-1).r * B(*n,j).r - DU(*n-1).i * B(*n,j).i, q__3.i =
		     DU(*n-1).r * B(*n,j).i + DU(*n-1).i * B(*n,j).r;
	    q__2.r = B(*n-1,j).r - q__3.r, q__2.i = B(*n-1,j).i - q__3.i;
	    c_div(&q__1, &q__2, &D(*n - 1));
	    B(*n-1,j).r = q__1.r, B(*n-1,j).i = q__1.i;
	}
	for (k = *n - 2; k >= 1; --k) {
	    i__2 = k + j * b_dim1;
	    i__3 = k + j * b_dim1;
	    i__4 = k;
	    i__5 = k + 1 + j * b_dim1;
	    q__4.r = DU(k).r * B(k+1,j).r - DU(k).i * B(k+1,j).i, q__4.i =
		     DU(k).r * B(k+1,j).i + DU(k).i * B(k+1,j).r;
	    q__3.r = B(k,j).r - q__4.r, q__3.i = B(k,j).i - q__4.i;
	    i__6 = k;
	    i__7 = k + 2 + j * b_dim1;
	    q__5.r = DL(k).r * B(k+2,j).r - DL(k).i * B(k+2,j).i, q__5.i =
		     DL(k).r * B(k+2,j).i + DL(k).i * B(k+2,j).r;
	    q__2.r = q__3.r - q__5.r, q__2.i = q__3.i - q__5.i;
	    c_div(&q__1, &q__2, &D(k));
	    B(k,j).r = q__1.r, B(k,j).i = q__1.i;
/* L40: */
	}
/* L50: */
    }

    return 0;

/*     End of CGTSV */

} /* cgtsv_ */

