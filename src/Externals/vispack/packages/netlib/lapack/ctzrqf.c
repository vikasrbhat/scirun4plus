#include "f2c.h"

/* Subroutine */ int ctzrqf_(integer *m, integer *n, complex *a, integer *lda,
	 complex *tau, integer *info)
{
/*  -- LAPACK routine (version 2.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    CTZRQF reduces the M-by-N ( M<=N ) complex upper trapezoidal matrix A 
  
    to upper triangular form by means of unitary transformations.   

    The upper trapezoidal matrix A is factored as   

       A = ( R  0 ) * Z,   

    where Z is an N-by-N unitary matrix and R is an M-by-M upper   
    triangular matrix.   

    Arguments   
    =========   

    M       (input) INTEGER   
            The number of rows of the matrix A.  M >= 0.   

    N       (input) INTEGER   
            The number of columns of the matrix A.  N >= M.   

    A       (input/output) COMPLEX array, dimension (LDA,N)   
            On entry, the leading M-by-N upper trapezoidal part of the   
            array A must contain the matrix to be factorized.   
            On exit, the leading M-by-M upper triangular part of A   
            contains the upper triangular matrix R, and elements M+1 to   
            N of the first M rows of A, with the array TAU, represent the 
  
            unitary matrix Z as a product of M elementary reflectors.   

    LDA     (input) INTEGER   
            The leading dimension of the array A.  LDA >= max(1,M).   

    TAU     (output) COMPLEX array, dimension (M)   
            The scalar factors of the elementary reflectors.   

    INFO    (output) INTEGER   
            = 0: successful exit   
            < 0: if INFO = -i, the i-th argument had an illegal value   

    Further Details   
    ===============   

    The  factorization is obtained by Householder's method.  The kth   
    transformation matrix, Z( k ), whose conjugate transpose is used to   
    introduce zeros into the (m - k + 1)th row of A, is given in the form 
  

       Z( k ) = ( I     0   ),   
                ( 0  T( k ) )   

    where   

       T( k ) = I - tau*u( k )*u( k )',   u( k ) = (   1    ),   
                                                   (   0    )   
                                                   ( z( k ) )   

    tau is a scalar and z( k ) is an ( n - m ) element vector.   
    tau and z( k ) are chosen to annihilate the elements of the kth row   
    of X.   

    The scalar tau is returned in the kth element of TAU and the vector   
    u( k ) in the kth row of A, such that the elements of z( k ) are   
    in  a( k, m + 1 ), ..., a( k, n ). The elements of R are returned in 
  
    the upper triangular part of A.   

    Z is given by   

       Z =  Z( 1 ) * Z( 2 ) * ... * Z( m ).   

   ===================================================================== 
  


       Test the input parameters.   

    
   Parameter adjustments   
       Function Body */
    /* Table of constant values */
    static complex c_b1 = {1.f,0.f};
    static integer c__1 = 1;
    
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    complex q__1, q__2;
    /* Builtin functions */
    void r_cnjg(complex *, complex *);
    /* Local variables */
    static integer i, k;
    extern /* Subroutine */ int cgerc_(integer *, integer *, complex *, 
	    complex *, integer *, complex *, integer *, complex *, integer *);
    static complex alpha;
    extern /* Subroutine */ int cgemv_(char *, integer *, integer *, complex *
	    , complex *, integer *, complex *, integer *, complex *, complex *
	    , integer *), ccopy_(integer *, complex *, integer *, 
	    complex *, integer *), caxpy_(integer *, complex *, complex *, 
	    integer *, complex *, integer *);
    static integer m1;
    extern /* Subroutine */ int clarfg_(integer *, complex *, complex *, 
	    integer *, complex *), clacgv_(integer *, complex *, integer *), 
	    xerbla_(char *, integer *);



#define TAU(I) tau[(I)-1]

#define A(I,J) a[(I)-1 + ((J)-1)* ( *lda)]

    *info = 0;
    if (*m < 0) {
	*info = -1;
    } else if (*n < *m) {
	*info = -2;
    } else if (*lda < max(1,*m)) {
	*info = -4;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("CTZRQF", &i__1);
	return 0;
    }

/*     Perform the factorization. */

    if (*m == 0) {
	return 0;
    }
    if (*m == *n) {
	i__1 = *n;
	for (i = 1; i <= *n; ++i) {
	    i__2 = i;
	    TAU(i).r = 0.f, TAU(i).i = 0.f;
/* L10: */
	}
    } else {
/* Computing MIN */
	i__1 = *m + 1;
	m1 = min(i__1,*n);
	for (k = *m; k >= 1; --k) {

/*           Use a Householder reflection to zero the kth row of A
.   
             First set up the reflection. */

	    i__1 = k + k * a_dim1;
	    r_cnjg(&q__1, &A(k,k));
	    A(k,k).r = q__1.r, A(k,k).i = q__1.i;
	    i__1 = *n - *m;
	    clacgv_(&i__1, &A(k,m1), lda);
	    i__1 = k + k * a_dim1;
	    alpha.r = A(k,k).r, alpha.i = A(k,k).i;
	    i__1 = *n - *m + 1;
	    clarfg_(&i__1, &alpha, &A(k,m1), lda, &TAU(k));
	    i__1 = k + k * a_dim1;
	    A(k,k).r = alpha.r, A(k,k).i = alpha.i;
	    i__1 = k;
	    r_cnjg(&q__1, &TAU(k));
	    TAU(k).r = q__1.r, TAU(k).i = q__1.i;

	    i__1 = k;
	    if ((TAU(k).r != 0.f || TAU(k).i != 0.f) && k > 1) {

/*              We now perform the operation  A := A*P( k )'. 
  

                Use the first ( k - 1 ) elements of TAU to sto
re  a( k ),   
                where  a( k ) consists of the first ( k - 1 ) 
elements of   
                the  kth column  of  A.  Also  let  B  denote 
 the  first   
                ( k - 1 ) rows of the last ( n - m ) columns o
f A. */

		i__1 = k - 1;
		ccopy_(&i__1, &A(1,k), &c__1, &TAU(1), &c__1);

/*              Form   w = a( k ) + B*z( k )  in TAU. */

		i__1 = k - 1;
		i__2 = *n - *m;
		cgemv_("No transpose", &i__1, &i__2, &c_b1, &A(1,m1), lda, &A(k,m1), lda, &c_b1, &TAU(1), &
			c__1);

/*              Now form  a( k ) := a( k ) - conjg(tau)*w   
                and       B      := B      - conjg(tau)*w*z( k
 )'. */

		i__1 = k - 1;
		r_cnjg(&q__2, &TAU(k));
		q__1.r = -(doublereal)q__2.r, q__1.i = -(doublereal)q__2.i;
		caxpy_(&i__1, &q__1, &TAU(1), &c__1, &A(1,k), &
			c__1);
		i__1 = k - 1;
		i__2 = *n - *m;
		r_cnjg(&q__2, &TAU(k));
		q__1.r = -(doublereal)q__2.r, q__1.i = -(doublereal)q__2.i;
		cgerc_(&i__1, &i__2, &q__1, &TAU(1), &c__1, &A(k,m1), lda, &A(1,m1), lda);
	    }
/* L20: */
	}
    }

    return 0;

/*     End of CTZRQF */

} /* ctzrqf_ */

