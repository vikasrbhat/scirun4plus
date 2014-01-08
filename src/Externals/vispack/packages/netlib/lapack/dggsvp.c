#include "f2c.h"

/* Subroutine */ int dggsvp_(char *jobu, char *jobv, char *jobq, integer *m, 
	integer *p, integer *n, doublereal *a, integer *lda, doublereal *b, 
	integer *ldb, doublereal *tola, doublereal *tolb, integer *k, integer 
	*l, doublereal *u, integer *ldu, doublereal *v, integer *ldv, 
	doublereal *q, integer *ldq, integer *iwork, doublereal *tau, 
	doublereal *work, integer *info)
{
/*  -- LAPACK routine (version 2.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    DGGSVP computes orthogonal matrices U, V and Q such that   

                     N-K-L  K    L   
     U'*A*Q =     K ( 0    A12  A13 )  if M-K-L >= 0;   
                  L ( 0     0   A23 )   
              M-K-L ( 0     0    0  )   

                     N-K-L  K    L   
            =     K ( 0    A12  A13 )  if M-K-L < 0;   
                M-K ( 0     0   A23 )   

                   N-K-L  K    L   
     V'*B*Q =   L ( 0     0   B13 )   
              P-L ( 0     0    0  )   

    where the K-by-K matrix A12 and L-by-L matrix B13 are nonsingular   
    upper triangular; A23 is L-by-L upper triangular if M-K-L >= 0,   
    otherwise A23 is (M-K)-by-L upper trapezoidal.  K+L = the effective   
    numerical rank of the (M+P)-by-N matrix (A',B')'.  Z' denotes the   
    transpose of Z.   

    This decomposition is the preprocessing step for computing the   
    Generalized Singular Value Decomposition (GSVD), see subroutine   
    DGGSVD.   

    Arguments   
    =========   

    JOBU    (input) CHARACTER*1   
            = 'U':  Orthogonal matrix U is computed;   
            = 'N':  U is not computed.   

    JOBV    (input) CHARACTER*1   
            = 'V':  Orthogonal matrix V is computed;   
            = 'N':  V is not computed.   

    JOBQ    (input) CHARACTER*1   
            = 'Q':  Orthogonal matrix Q is computed;   
            = 'N':  Q is not computed.   

    M       (input) INTEGER   
            The number of rows of the matrix A.  M >= 0.   

    P       (input) INTEGER   
            The number of rows of the matrix B.  P >= 0.   

    N       (input) INTEGER   
            The number of columns of the matrices A and B.  N >= 0.   

    A       (input/output) DOUBLE PRECISION array, dimension (LDA,N)   
            On entry, the M-by-N matrix A.   
            On exit, A contains the triangular (or trapezoidal) matrix   
            described in the Purpose section.   

    LDA     (input) INTEGER   
            The leading dimension of the array A. LDA >= max(1,M).   

    B       (input/output) DOUBLE PRECISION array, dimension (LDB,N)   
            On entry, the P-by-N matrix B.   
            On exit, B contains the triangular matrix described in   
            the Purpose section.   

    LDB     (input) INTEGER   
            The leading dimension of the array B. LDB >= max(1,P).   

    TOLA    (input) DOUBLE PRECISION   
    TOLB    (input) DOUBLE PRECISION   
            TOLA and TOLB are the thresholds to determine the effective   
            numerical rank of matrix B and a subblock of A. Generally,   
            they are set to   
               TOLA = MAX(M,N)*norm(A)*MAZHEPS,   
               TOLB = MAX(P,N)*norm(B)*MAZHEPS.   
            The size of TOLA and TOLB may affect the size of backward   
            errors of the decomposition.   

    K       (output) INTEGER   
    L       (output) INTEGER   
            On exit, K and L specify the dimension of the subblocks   
            described in Purpose.   
            K + L = effective numerical rank of (A',B')'.   

    U       (output) DOUBLE PRECISION array, dimension (LDU,M)   
            If JOBU = 'U', U contains the orthogonal matrix U.   
            If JOBU = 'N', U is not referenced.   

    LDU     (input) INTEGER   
            The leading dimension of the array U. LDU >= max(1,M) if   
            JOBU = 'U'; LDU >= 1 otherwise.   

    V       (output) DOUBLE PRECISION array, dimension (LDV,M)   
            If JOBV = 'V', V contains the orthogonal matrix V.   
            If JOBV = 'N', V is not referenced.   

    LDV     (input) INTEGER   
            The leading dimension of the array V. LDV >= max(1,P) if   
            JOBV = 'V'; LDV >= 1 otherwise.   

    Q       (output) DOUBLE PRECISION array, dimension (LDQ,N)   
            If JOBQ = 'Q', Q contains the orthogonal matrix Q.   
            If JOBQ = 'N', Q is not referenced.   

    LDQ     (input) INTEGER   
            The leading dimension of the array Q. LDQ >= max(1,N) if   
            JOBQ = 'Q'; LDQ >= 1 otherwise.   

    IWORK   (workspace) INTEGER array, dimension (N)   

    TAU     (workspace) DOUBLE PRECISION array, dimension (N)   

    WORK    (workspace) DOUBLE PRECISION array, dimension (max(3*N,M,P)) 
  

    INFO    (output) INTEGER   
            = 0:  successful exit   
            < 0:  if INFO = -i, the i-th argument had an illegal value.   


    Further Details   
    ===============   

    The subroutine uses LAPACK subroutine DGEQPF for the QR factorization 
  
    with column pivoting to detect the effective numerical rank of the   
    a matrix. It may be replaced by a better rank determination strategy. 
  

    ===================================================================== 
  


       Test the input parameters   

    
   Parameter adjustments   
       Function Body */
    /* Table of constant values */
    static doublereal c_b12 = 0.;
    static doublereal c_b22 = 1.;
    
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, q_dim1, q_offset, u_dim1, 
	    u_offset, v_dim1, v_offset, i__1, i__2, i__3;
    doublereal d__1;
    /* Local variables */
    static integer i, j;
    extern logical lsame_(char *, char *);
    static logical wantq, wantu, wantv;
    extern /* Subroutine */ int dgeqr2_(integer *, integer *, doublereal *, 
	    integer *, doublereal *, doublereal *, integer *), dgerq2_(
	    integer *, integer *, doublereal *, integer *, doublereal *, 
	    doublereal *, integer *), dorg2r_(integer *, integer *, integer *,
	     doublereal *, integer *, doublereal *, doublereal *, integer *), 
	    dorm2r_(char *, char *, integer *, integer *, integer *, 
	    doublereal *, integer *, doublereal *, doublereal *, integer *, 
	    doublereal *, integer *), dormr2_(char *, char *, 
	    integer *, integer *, integer *, doublereal *, integer *, 
	    doublereal *, doublereal *, integer *, doublereal *, integer *), dgeqpf_(integer *, integer *, doublereal *, 
	    integer *, integer *, doublereal *, doublereal *, integer *), 
	    dlacpy_(char *, integer *, integer *, doublereal *, integer *, 
	    doublereal *, integer *), dlaset_(char *, integer *, 
	    integer *, doublereal *, doublereal *, doublereal *, integer *), xerbla_(char *, integer *), dlapmt_(logical *, 
	    integer *, integer *, doublereal *, integer *, integer *);
    static logical forwrd;



#define IWORK(I) iwork[(I)-1]
#define TAU(I) tau[(I)-1]
#define WORK(I) work[(I)-1]

#define A(I,J) a[(I)-1 + ((J)-1)* ( *lda)]
#define B(I,J) b[(I)-1 + ((J)-1)* ( *ldb)]
#define U(I,J) u[(I)-1 + ((J)-1)* ( *ldu)]
#define V(I,J) v[(I)-1 + ((J)-1)* ( *ldv)]
#define Q(I,J) q[(I)-1 + ((J)-1)* ( *ldq)]

    wantu = lsame_(jobu, "U");
    wantv = lsame_(jobv, "V");
    wantq = lsame_(jobq, "Q");
    forwrd = TRUE_;

    *info = 0;
    if (! (wantu || lsame_(jobu, "N"))) {
	*info = -1;
    } else if (! (wantv || lsame_(jobv, "N"))) {
	*info = -2;
    } else if (! (wantq || lsame_(jobq, "N"))) {
	*info = -3;
    } else if (*m < 0) {
	*info = -4;
    } else if (*p < 0) {
	*info = -5;
    } else if (*n < 0) {
	*info = -6;
    } else if (*lda < max(1,*m)) {
	*info = -8;
    } else if (*ldb < max(1,*p)) {
	*info = -10;
    } else if (*ldu < 1 || wantu && *ldu < *m) {
	*info = -16;
    } else if (*ldv < 1 || wantv && *ldv < *p) {
	*info = -18;
    } else if (*ldq < 1 || wantq && *ldq < *n) {
	*info = -20;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("DGGSVP", &i__1);
	return 0;
    }

/*     QR with column pivoting of B: B*P = V*( S11 S12 )   
                                             (  0   0  ) */

    i__1 = *n;
    for (i = 1; i <= *n; ++i) {
	IWORK(i) = 0;
/* L10: */
    }
    dgeqpf_(p, n, &B(1,1), ldb, &IWORK(1), &TAU(1), &WORK(1), info);

/*     Update A := A*P */

    dlapmt_(&forwrd, m, n, &A(1,1), lda, &IWORK(1));

/*     Determine the effective rank of matrix B. */

    *l = 0;
    i__1 = min(*p,*n);
    for (i = 1; i <= min(*p,*n); ++i) {
	if ((d__1 = B(i,i), abs(d__1)) > *tolb) {
	    ++(*l);
	}
/* L20: */
    }

    if (wantv) {

/*        Copy the details of V, and form V. */

	dlaset_("Full", p, p, &c_b12, &c_b12, &V(1,1), ldv);
	if (*p > 1) {
	    i__1 = *p - 1;
	    dlacpy_("Lower", &i__1, n, &B(2,1), ldb, &V(2,1), 
		    ldv);
	}
	i__1 = min(*p,*n);
	dorg2r_(p, p, &i__1, &V(1,1), ldv, &TAU(1), &WORK(1), info);
    }

/*     Clean up B */

    i__1 = *l - 1;
    for (j = 1; j <= *l-1; ++j) {
	i__2 = *l;
	for (i = j + 1; i <= *l; ++i) {
	    B(i,j) = 0.;
/* L30: */
	}
/* L40: */
    }
    if (*p > *l) {
	i__1 = *p - *l;
	dlaset_("Full", &i__1, n, &c_b12, &c_b12, &B(*l+1,1), ldb);
    }

    if (wantq) {

/*        Set Q = I and Update Q := Q*P */

	dlaset_("Full", n, n, &c_b12, &c_b22, &Q(1,1), ldq);
	dlapmt_(&forwrd, n, n, &Q(1,1), ldq, &IWORK(1));
    }

    if (*p >= *l && *n != *l) {

/*        RQ factorization of (S11 S12): ( S11 S12 ) = ( 0 S12 )*Z */

	dgerq2_(l, n, &B(1,1), ldb, &TAU(1), &WORK(1), info);

/*        Update A := A*Z' */

	dormr2_("Right", "Transpose", m, n, l, &B(1,1), ldb, &TAU(1), &A(1,1), lda, &WORK(1), info);

	if (wantq) {

/*           Update Q := Q*Z' */

	    dormr2_("Right", "Transpose", n, n, l, &B(1,1), ldb, &TAU(1),
		     &Q(1,1), ldq, &WORK(1), info);
	}

/*        Clean up B */

	i__1 = *n - *l;
	dlaset_("Full", l, &i__1, &c_b12, &c_b12, &B(1,1), ldb);
	i__1 = *n;
	for (j = *n - *l + 1; j <= *n; ++j) {
	    i__2 = *l;
	    for (i = j - *n + *l + 1; i <= *l; ++i) {
		B(i,j) = 0.;
/* L50: */
	    }
/* L60: */
	}

    }

/*     Let              N-L     L   
                  A = ( A11    A12 ) M,   

       then the following does the complete QR decomposition of A11:   

                A11 = U*(  0  T12 )*P1'   
                        (  0   0  ) */

    i__1 = *n - *l;
    for (i = 1; i <= *n-*l; ++i) {
	IWORK(i) = 0;
/* L70: */
    }
    i__1 = *n - *l;
    dgeqpf_(m, &i__1, &A(1,1), lda, &IWORK(1), &TAU(1), &WORK(1), info);

/*     Determine the effective rank of A11 */

    *k = 0;
/* Computing MIN */
    i__2 = *m, i__3 = *n - *l;
    i__1 = min(i__2,i__3);
    for (i = 1; i <= min(*m,*n-*l); ++i) {
	if ((d__1 = A(i,i), abs(d__1)) > *tola) {
	    ++(*k);
	}
/* L80: */
    }

/*     Update A12 := U'*A12, where A12 = A( 1:M, N-L+1:N )   

   Computing MIN */
    i__2 = *m, i__3 = *n - *l;
    i__1 = min(i__2,i__3);
    dorm2r_("Left", "Transpose", m, l, &i__1, &A(1,1), lda, &TAU(1), &A(1,*n-*l+1), lda, &WORK(1), info);

    if (wantu) {

/*        Copy the details of U, and form U */

	dlaset_("Full", m, m, &c_b12, &c_b12, &U(1,1), ldu);
	if (*m > 1) {
	    i__1 = *m - 1;
	    i__2 = *n - *l;
	    dlacpy_("Lower", &i__1, &i__2, &A(2,1), lda, &U(2,1)
		    , ldu);
	}
/* Computing MIN */
	i__2 = *m, i__3 = *n - *l;
	i__1 = min(i__2,i__3);
	dorg2r_(m, m, &i__1, &U(1,1), ldu, &TAU(1), &WORK(1), info);
    }

    if (wantq) {

/*        Update Q( 1:N, 1:N-L )  = Q( 1:N, 1:N-L )*P1 */

	i__1 = *n - *l;
	dlapmt_(&forwrd, n, &i__1, &Q(1,1), ldq, &IWORK(1));
    }

/*     Clean up A: set the strictly lower triangular part of   
       A(1:K, 1:K) = 0, and A( K+1:M, 1:N-L ) = 0. */

    i__1 = *k - 1;
    for (j = 1; j <= *k-1; ++j) {
	i__2 = *k;
	for (i = j + 1; i <= *k; ++i) {
	    A(i,j) = 0.;
/* L90: */
	}
/* L100: */
    }
    if (*m > *k) {
	i__1 = *m - *k;
	i__2 = *n - *l;
	dlaset_("Full", &i__1, &i__2, &c_b12, &c_b12, &A(*k+1,1), 
		lda);
    }

    if (*n - *l > *k) {

/*        RQ factorization of ( T11 T12 ) = ( 0 T12 )*Z1 */

	i__1 = *n - *l;
	dgerq2_(k, &i__1, &A(1,1), lda, &TAU(1), &WORK(1), info);

	if (wantq) {

/*           Update Q( 1:N,1:N-L ) = Q( 1:N,1:N-L )*Z1' */

	    i__1 = *n - *l;
	    dormr2_("Right", "Transpose", n, &i__1, k, &A(1,1), lda, &
		    TAU(1), &Q(1,1), ldq, &WORK(1), info);
	}

/*        Clean up A */

	i__1 = *n - *l - *k;
	dlaset_("Full", k, &i__1, &c_b12, &c_b12, &A(1,1), lda);
	i__1 = *n - *l;
	for (j = *n - *l - *k + 1; j <= *n-*l; ++j) {
	    i__2 = *k;
	    for (i = j - *n + *l + *k + 1; i <= *k; ++i) {
		A(i,j) = 0.;
/* L110: */
	    }
/* L120: */
	}

    }

    if (*m > *k) {

/*        QR factorization of A( K+1:M,N-L+1:N ) */

	i__1 = *m - *k;
	dgeqr2_(&i__1, l, &A(*k+1,*n-*l+1), lda, &TAU(1), &
		WORK(1), info);

	if (wantu) {

/*           Update U(:,K+1:M) := U(:,K+1:M)*U1 */

	    i__1 = *m - *k;
/* Computing MIN */
	    i__3 = *m - *k;
	    i__2 = min(i__3,*l);
	    dorm2r_("Right", "No transpose", m, &i__1, &i__2, &A(*k+1,*n-*l+1), lda, &TAU(1), &U(1,*k+1), ldu, &WORK(1), info);
	}

/*        Clean up */

	i__1 = *n;
	for (j = *n - *l + 1; j <= *n; ++j) {
	    i__2 = *m;
	    for (i = j - *n + *k + *l + 1; i <= *m; ++i) {
		A(i,j) = 0.;
/* L130: */
	    }
/* L140: */
	}

    }

    return 0;

/*     End of DGGSVP */

} /* dggsvp_ */

