#include "f2c.h"

/* Subroutine */ int ztrexc_(char *compq, integer *n, doublecomplex *t, 
	integer *ldt, doublecomplex *q, integer *ldq, integer *ifst, integer *
	ilst, integer *info)
{
/*  -- LAPACK routine (version 2.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       March 31, 1993   


    Purpose   
    =======   

    ZTREXC reorders the Schur factorization of a complex matrix   
    A = Q*T*Q**H, so that the diagonal element of T with row index IFST   
    is moved to row ILST.   

    The Schur form T is reordered by a unitary similarity transformation 
  
    Z**H*T*Z, and optionally the matrix Q of Schur vectors is updated by 
  
    postmultplying it with Z.   

    Arguments   
    =========   

    COMPQ   (input) CHARACTER*1   
            = 'V':  update the matrix Q of Schur vectors;   
            = 'N':  do not update Q.   

    N       (input) INTEGER   
            The order of the matrix T. N >= 0.   

    T       (input/output) COMPLEX*16 array, dimension (LDT,N)   
            On entry, the upper triangular matrix T.   
            On exit, the reordered upper triangular matrix.   

    LDT     (input) INTEGER   
            The leading dimension of the array T. LDT >= max(1,N).   

    Q       (input/output) COMPLEX*16 array, dimension (LDQ,N)   
            On entry, if COMPQ = 'V', the matrix Q of Schur vectors.   
            On exit, if COMPQ = 'V', Q has been postmultiplied by the   
            unitary transformation matrix Z which reorders T.   
            If COMPQ = 'N', Q is not referenced.   

    LDQ     (input) INTEGER   
            The leading dimension of the array Q.  LDQ >= max(1,N).   

    IFST    (input) INTEGER   
    ILST    (input) INTEGER   
            Specify the reordering of the diagonal elements of T:   
            The element with row index IFST is moved to row ILST by a   
            sequence of transpositions between adjacent elements.   
            1 <= IFST <= N; 1 <= ILST <= N.   

    INFO    (output) INTEGER   
            = 0:  successful exit   
            < 0:  if INFO = -i, the i-th argument had an illegal value   

    ===================================================================== 
  


       Decode and test the input parameters.   

    
   Parameter adjustments   
       Function Body */
    /* Table of constant values */
    static integer c__1 = 1;
    
    /* System generated locals */
    integer q_dim1, q_offset, t_dim1, t_offset, i__1, i__2, i__3;
    doublecomplex z__1;
    /* Builtin functions */
    void d_cnjg(doublecomplex *, doublecomplex *);
    /* Local variables */
    static doublecomplex temp;
    extern /* Subroutine */ int zrot_(integer *, doublecomplex *, integer *, 
	    doublecomplex *, integer *, doublereal *, doublecomplex *);
    static integer k;
    extern logical lsame_(char *, char *);
    static logical wantq;
    static integer m1, m2, m3;
    static doublereal cs;
    static doublecomplex t11, t22, sn;
    extern /* Subroutine */ int xerbla_(char *, integer *), zlartg_(
	    doublecomplex *, doublecomplex *, doublereal *, doublecomplex *, 
	    doublecomplex *);




#define T(I,J) t[(I)-1 + ((J)-1)* ( *ldt)]
#define Q(I,J) q[(I)-1 + ((J)-1)* ( *ldq)]

    *info = 0;
    wantq = lsame_(compq, "V");
    if (! lsame_(compq, "N") && ! wantq) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*ldt < max(1,*n)) {
	*info = -4;
    } else if (*ldq < 1 || wantq && *ldq < max(1,*n)) {
	*info = -6;
    } else if (*ifst < 1 || *ifst > *n) {
	*info = -7;
    } else if (*ilst < 1 || *ilst > *n) {
	*info = -8;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("ZTREXC", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 1 || *ifst == *ilst) {
	return 0;
    }

    if (*ifst < *ilst) {

/*        Move the IFST-th diagonal element forward down the diagonal.
 */

	m1 = 0;
	m2 = -1;
	m3 = 1;
    } else {

/*        Move the IFST-th diagonal element backward up the diagonal. 
*/

	m1 = -1;
	m2 = 0;
	m3 = -1;
    }

    i__1 = *ilst + m2;
    i__2 = m3;
    for (k = *ifst + m1; m3 < 0 ? k >= *ilst+m2 : k <= *ilst+m2; k += m3) {

/*        Interchange the k-th and (k+1)-th diagonal elements. */

	i__3 = k + k * t_dim1;
	t11.r = T(k,k).r, t11.i = T(k,k).i;
	i__3 = k + 1 + (k + 1) * t_dim1;
	t22.r = T(k+1,k+1).r, t22.i = T(k+1,k+1).i;

/*        Determine the transformation to perform the interchange. */

	z__1.r = t22.r - t11.r, z__1.i = t22.i - t11.i;
	zlartg_(&T(k,k+1), &z__1, &cs, &sn, &temp);

/*        Apply transformation to the matrix T. */

	if (k + 2 <= *n) {
	    i__3 = *n - k - 1;
	    zrot_(&i__3, &T(k,k+2), ldt, &T(k+1,k+2), ldt, &cs, &sn);
	}
	i__3 = k - 1;
	d_cnjg(&z__1, &sn);
	zrot_(&i__3, &T(1,k), &c__1, &T(1,k+1), &
		c__1, &cs, &z__1);

	i__3 = k + k * t_dim1;
	T(k,k).r = t22.r, T(k,k).i = t22.i;
	i__3 = k + 1 + (k + 1) * t_dim1;
	T(k+1,k+1).r = t11.r, T(k+1,k+1).i = t11.i;

	if (wantq) {

/*           Accumulate transformation in the matrix Q. */

	    d_cnjg(&z__1, &sn);
	    zrot_(n, &Q(1,k), &c__1, &Q(1,k+1), &
		    c__1, &cs, &z__1);
	}

/* L10: */
    }

    return 0;

/*     End of ZTREXC */

} /* ztrexc_ */
