#include "f2c.h"

/* Subroutine */ int zgghrd_(char *compq, char *compz, integer *n, integer *
	ilo, integer *ihi, doublecomplex *a, integer *lda, doublecomplex *b, 
	integer *ldb, doublecomplex *q, integer *ldq, doublecomplex *z, 
	integer *ldz, integer *info)
{
/*  -- LAPACK routine (version 2.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    ZGGHRD reduces a pair of complex matrices (A,B) to generalized upper 
  
    Hessenberg form using unitary transformations, where A is a   
    general matrix and B is upper triangular:  Q' * A * Z = H and   
    Q' * B * Z = T, where H is upper Hessenberg, T is upper triangular,   
    and Q and Z are unitary, and ' means conjugate transpose.   

    The unitary matrices Q and Z are determined as products of Givens   
    rotations.  They may either be formed explicitly, or they may be   
    postmultiplied into input matrices Q1 and Z1, so that   

         Q1 * A * Z1' = (Q1*Q) * H * (Z1*Z)'   
         Q1 * B * Z1' = (Q1*Q) * T * (Z1*Z)'   

    Arguments   
    =========   

    COMPQ   (input) CHARACTER*1   
            = 'N': do not compute Q;   
            = 'I': Q is initialized to the unit matrix, and the   
                   unitary matrix Q is returned;   
            = 'V': Q must contain a unitary matrix Q1 on entry,   
                   and the product Q1*Q is returned.   

    COMPZ   (input) CHARACTER*1   
            = 'N': do not compute Q;   
            = 'I': Q is initialized to the unit matrix, and the   
                   unitary matrix Q is returned;   
            = 'V': Q must contain a unitary matrix Q1 on entry,   
                   and the product Q1*Q is returned.   

    N       (input) INTEGER   
            The order of the matrices A and B.  N >= 0.   

    ILO     (input) INTEGER   
    IHI     (input) INTEGER   
            It is assumed that A is already upper triangular in rows and 
  
            columns 1:ILO-1 and IHI+1:N.  ILO and IHI are normally set   
            by a previous call to ZGGBAL; otherwise they should be set   
            to 1 and N respectively.   
            1 <= ILO <= IHI <= N, if N > 0; ILO=1 and IHI=0, if N=0.   

    A       (input/output) COMPLEX*16 array, dimension (LDA, N)   
            On entry, the N-by-N general matrix to be reduced.   
            On exit, the upper triangle and the first subdiagonal of A   
            are overwritten with the upper Hessenberg matrix H, and the   
            rest is set to zero.   

    LDA     (input) INTEGER   
            The leading dimension of the array A.  LDA >= max(1,N).   

    B       (input/output) COMPLEX*16 array, dimension (LDB, N)   
            On entry, the N-by-N upper triangular matrix B.   
            On exit, the upper triangular matrix T = Q' B Z.  The   
            elements below the diagonal are set to zero.   

    LDB     (input) INTEGER   
            The leading dimension of the array B.  LDB >= max(1,N).   

    Q       (input/output) COMPLEX*16 array, dimension (LDQ, N)   
            If COMPQ='N':  Q is not referenced.   
            If COMPQ='I':  on entry, Q need not be set, and on exit it   
                           contains the unitary matrix Q, where Q'   
                           is the product of the Givens transformations   
                           which are applied to A and B on the left.   
            If COMPQ='V':  on entry, Q must contain a unitary matrix   
                           Q1, and on exit this is overwritten by Q1*Q.   

    LDQ     (input) INTEGER   
            The leading dimension of the array Q.   
            LDQ >= N if COMPQ='V' or 'I'; LDQ >= 1 otherwise.   

    Z       (input/output) COMPLEX*16 array, dimension (LDZ, N)   
            If COMPZ='N':  Z is not referenced.   
            If COMPZ='I':  on entry, Z need not be set, and on exit it   
                           contains the unitary matrix Z, which is   
                           the product of the Givens transformations   
                           which are applied to A and B on the right.   
            If COMPZ='V':  on entry, Z must contain a unitary matrix   
                           Z1, and on exit this is overwritten by Z1*Z.   

    LDZ     (input) INTEGER   
            The leading dimension of the array Z.   
            LDZ >= N if COMPZ='V' or 'I'; LDZ >= 1 otherwise.   

    INFO    (output) INTEGER   
            = 0:  successful exit.   
            < 0:  if INFO = -i, the i-th argument had an illegal value.   

    Further Details   
    ===============   

    This routine reduces A to Hessenberg and B to triangular form by   
    an unblocked reduction, as described in _VISMatrix_Computations_,   
    by Golub and van Loan (Johns Hopkins Press).   

    ===================================================================== 
  


       Decode COMPQ   

    
   Parameter adjustments   
       Function Body */
    /* Table of constant values */
    static doublecomplex c_b1 = {1.,0.};
    static doublecomplex c_b2 = {0.,0.};
    static integer c__1 = 1;
    
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, q_dim1, q_offset, z_dim1, 
	    z_offset, i__1, i__2, i__3;
    doublecomplex z__1;
    /* Builtin functions */
    void d_cnjg(doublecomplex *, doublecomplex *);
    /* Local variables */
    static integer jcol, jrow;
    extern /* Subroutine */ int zrot_(integer *, doublecomplex *, integer *, 
	    doublecomplex *, integer *, doublereal *, doublecomplex *);
    static doublereal c;
    static doublecomplex s;
    extern logical lsame_(char *, char *);
    static doublecomplex ctemp;
    extern /* Subroutine */ int xerbla_(char *, integer *);
    static integer icompq, icompz;
    extern /* Subroutine */ int zlaset_(char *, integer *, integer *, 
	    doublecomplex *, doublecomplex *, doublecomplex *, integer *), zlartg_(doublecomplex *, doublecomplex *, doublereal *, 
	    doublecomplex *, doublecomplex *);
    static logical ilq, ilz;




#define A(I,J) a[(I)-1 + ((J)-1)* ( *lda)]
#define B(I,J) b[(I)-1 + ((J)-1)* ( *ldb)]
#define Q(I,J) q[(I)-1 + ((J)-1)* ( *ldq)]
#define Z(I,J) z[(I)-1 + ((J)-1)* ( *ldz)]

    if (lsame_(compq, "N")) {
	ilq = FALSE_;
	icompq = 1;
    } else if (lsame_(compq, "V")) {
	ilq = TRUE_;
	icompq = 2;
    } else if (lsame_(compq, "I")) {
	ilq = TRUE_;
	icompq = 3;
    } else {
	icompq = 0;
    }

/*     Decode COMPZ */

    if (lsame_(compz, "N")) {
	ilz = FALSE_;
	icompz = 1;
    } else if (lsame_(compz, "V")) {
	ilz = TRUE_;
	icompz = 2;
    } else if (lsame_(compz, "I")) {
	ilz = TRUE_;
	icompz = 3;
    } else {
	icompz = 0;
    }

/*     Test the input parameters. */

    *info = 0;
    if (icompq <= 0) {
	*info = -1;
    } else if (icompz <= 0) {
	*info = -2;
    } else if (*n < 0) {
	*info = -3;
    } else if (*ilo < 1) {
	*info = -4;
    } else if (*ihi > *n || *ihi < *ilo - 1) {
	*info = -5;
    } else if (*lda < max(1,*n)) {
	*info = -7;
    } else if (*ldb < max(1,*n)) {
	*info = -9;
    } else if (ilq && *ldq < *n || *ldq < 1) {
	*info = -11;
    } else if (ilz && *ldz < *n || *ldz < 1) {
	*info = -13;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("ZGGHRD", &i__1);
	return 0;
    }

/*     Initialize Q and Z if desired. */

    if (icompq == 3) {
	zlaset_("Full", n, n, &c_b2, &c_b1, &Q(1,1), ldq);
    }
    if (icompz == 3) {
	zlaset_("Full", n, n, &c_b2, &c_b1, &Z(1,1), ldz);
    }

/*     Quick return if possible */

    if (*n <= 1) {
	return 0;
    }

/*     Zero out lower triangle of B */

    i__1 = *n - 1;
    for (jcol = 1; jcol <= *n-1; ++jcol) {
	i__2 = *n;
	for (jrow = jcol + 1; jrow <= *n; ++jrow) {
	    i__3 = jrow + jcol * b_dim1;
	    B(jrow,jcol).r = 0., B(jrow,jcol).i = 0.;
/* L10: */
	}
/* L20: */
    }

/*     Reduce A and B */

    i__1 = *ihi - 2;
    for (jcol = *ilo; jcol <= *ihi-2; ++jcol) {

	i__2 = jcol + 2;
	for (jrow = *ihi; jrow >= jcol+2; --jrow) {

/*           Step 1: rotate rows JROW-1, JROW to kill A(JROW,JCOL)
 */

	    i__3 = jrow - 1 + jcol * a_dim1;
	    ctemp.r = A(jrow-1,jcol).r, ctemp.i = A(jrow-1,jcol).i;
	    zlartg_(&ctemp, &A(jrow,jcol), &c, &s, &A(jrow-1,jcol));
	    i__3 = jrow + jcol * a_dim1;
	    A(jrow,jcol).r = 0., A(jrow,jcol).i = 0.;
	    i__3 = *n - jcol;
	    zrot_(&i__3, &A(jrow-1,jcol+1), lda, &A(jrow,jcol+1), lda, &c, &s);
	    i__3 = *n + 2 - jrow;
	    zrot_(&i__3, &B(jrow-1,jrow-1), ldb, &B(jrow,jrow-1), ldb, &c, &s);
	    if (ilq) {
		d_cnjg(&z__1, &s);
		zrot_(n, &Q(1,jrow-1), &c__1, &Q(1,jrow), &c__1, &c, &z__1);
	    }

/*           Step 2: rotate columns JROW, JROW-1 to kill B(JROW,JR
OW-1) */

	    i__3 = jrow + jrow * b_dim1;
	    ctemp.r = B(jrow,jrow).r, ctemp.i = B(jrow,jrow).i;
	    zlartg_(&ctemp, &B(jrow,jrow-1), &c, &s, &B(jrow,jrow));
	    i__3 = jrow + (jrow - 1) * b_dim1;
	    B(jrow,jrow-1).r = 0., B(jrow,jrow-1).i = 0.;
	    zrot_(ihi, &A(1,jrow), &c__1, &A(1,jrow-1), &c__1, &c, &s);
	    i__3 = jrow - 1;
	    zrot_(&i__3, &B(1,jrow), &c__1, &B(1,jrow-1), &c__1, &c, &s);
	    if (ilz) {
		zrot_(n, &Z(1,jrow), &c__1, &Z(1,jrow-1), &c__1, &c, &s);
	    }
/* L30: */
	}
/* L40: */
    }

    return 0;

/*     End of ZGGHRD */

} /* zgghrd_ */

