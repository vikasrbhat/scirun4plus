#include "f2c.h"

/* Subroutine */ int sspgv_(integer *itype, char *jobz, char *uplo, integer *
	n, real *ap, real *bp, real *w, real *z, integer *ldz, real *work, 
	integer *info)
{
/*  -- LAPACK driver routine (version 2.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    SSPGV computes all the eigenvalues and, optionally, the eigenvectors 
  
    of a real generalized symmetric-definite eigenproblem, of the form   
    A*x=(lambda)*B*x,  A*Bx=(lambda)*x,  or B*A*x=(lambda)*x.   
    Here A and B are assumed to be symmetric, stored in packed format,   
    and B is also positive definite.   

    Arguments   
    =========   

    ITYPE   (input) INTEGER   
            Specifies the problem type to be solved:   
            = 1:  A*x = (lambda)*B*x   
            = 2:  A*B*x = (lambda)*x   
            = 3:  B*A*x = (lambda)*x   

    JOBZ    (input) CHARACTER*1   
            = 'N':  Compute eigenvalues only;   
            = 'V':  Compute eigenvalues and eigenvectors.   

    UPLO    (input) CHARACTER*1   
            = 'U':  Upper triangles of A and B are stored;   
            = 'L':  Lower triangles of A and B are stored.   

    N       (input) INTEGER   
            The order of the matrices A and B.  N >= 0.   

    AP      (input/output) REAL array, dimension   
                              (N*(N+1)/2)   
            On entry, the upper or lower triangle of the symmetric matrix 
  
            A, packed columnwise in a linear array.  The j-th column of A 
  
            is stored in the array AP as follows:   
            if UPLO = 'U', AP(i + (j-1)*j/2) = A(i,j) for 1<=i<=j;   
            if UPLO = 'L', AP(i + (j-1)*(2*n-j)/2) = A(i,j) for j<=i<=n. 
  

            On exit, the contents of AP are destroyed.   

    BP      (input/output) REAL array, dimension (N*(N+1)/2)   
            On entry, the upper or lower triangle of the symmetric matrix 
  
            B, packed columnwise in a linear array.  The j-th column of B 
  
            is stored in the array BP as follows:   
            if UPLO = 'U', BP(i + (j-1)*j/2) = B(i,j) for 1<=i<=j;   
            if UPLO = 'L', BP(i + (j-1)*(2*n-j)/2) = B(i,j) for j<=i<=n. 
  

            On exit, the triangular factor U or L from the Cholesky   
            factorization B = U**T*U or B = L*L**T, in the same storage   
            format as B.   

    W       (output) REAL array, dimension (N)   
            If INFO = 0, the eigenvalues in ascending order.   

    Z       (output) REAL array, dimension (LDZ, N)   
            If JOBZ = 'V', then if INFO = 0, Z contains the matrix Z of   
            eigenvectors.  The eigenvectors are normalized as follows:   
            if ITYPE = 1 or 2, Z**T*B*Z = I;   
            if ITYPE = 3, Z**T*inv(B)*Z = I.   
            If JOBZ = 'N', then Z is not referenced.   

    LDZ     (input) INTEGER   
            The leading dimension of the array Z.  LDZ >= 1, and if   
            JOBZ = 'V', LDZ >= max(1,N).   

    WORK    (workspace) REAL array, dimension (3*N)   

    INFO    (output) INTEGER   
            = 0:  successful exit   
            < 0:  if INFO = -i, the i-th argument had an illegal value   
            > 0:  SPPTRF or SSPEV returned an error code:   
               <= N:  if INFO = i, SSPEV failed to converge;   
                      i off-diagonal elements of an intermediate   
                      tridiagonal form did not converge to zero.   
               > N:   if INFO = n + i, for 1 <= i <= n, then the leading 
  
                      minor of order i of B is not positive definite.   
                      The factorization of B could not be completed and   
                      no eigenvalues or eigenvectors were computed.   

    ===================================================================== 
  


       Test the input parameters.   

    
   Parameter adjustments   
       Function Body */
    /* Table of constant values */
    static integer c__1 = 1;
    
    /* System generated locals */
    integer z_dim1, z_offset, i__1;
    /* Local variables */
    static integer neig, j;
    extern logical lsame_(char *, char *);
    static char trans[1];
    static logical upper;
    extern /* Subroutine */ int sspev_(char *, char *, integer *, real *, 
	    real *, real *, integer *, real *, integer *);
    static logical wantz;
    extern /* Subroutine */ int stpmv_(char *, char *, char *, integer *, 
	    real *, real *, integer *), stpsv_(char *,
	     char *, char *, integer *, real *, real *, integer *), xerbla_(char *, integer *), spptrf_(char 
	    *, integer *, real *, integer *), sspgst_(integer *, char 
	    *, integer *, real *, real *, integer *);



#define AP(I) ap[(I)-1]
#define BP(I) bp[(I)-1]
#define W(I) w[(I)-1]
#define WORK(I) work[(I)-1]

#define Z(I,J) z[(I)-1 + ((J)-1)* ( *ldz)]

    wantz = lsame_(jobz, "V");
    upper = lsame_(uplo, "U");

    *info = 0;
    if (*itype < 0 || *itype > 3) {
	*info = -1;
    } else if (! (wantz || lsame_(jobz, "N"))) {
	*info = -2;
    } else if (! (upper || lsame_(uplo, "L"))) {
	*info = -3;
    } else if (*n < 0) {
	*info = -4;
    } else if (*ldz < 1 || wantz && *ldz < *n) {
	*info = -9;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("SSPGV ", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	return 0;
    }

/*     Form a Cholesky factorization of B. */

    spptrf_(uplo, n, &BP(1), info);
    if (*info != 0) {
	*info = *n + *info;
	return 0;
    }

/*     Transform problem to standard eigenvalue problem and solve. */

    sspgst_(itype, uplo, n, &AP(1), &BP(1), info);
    sspev_(jobz, uplo, n, &AP(1), &W(1), &Z(1,1), ldz, &WORK(1), info);

    if (wantz) {

/*        Backtransform eigenvectors to the original problem. */

	neig = *n;
	if (*info > 0) {
	    neig = *info - 1;
	}
	if (*itype == 1 || *itype == 2) {

/*           For A*x=(lambda)*B*x and A*B*x=(lambda)*x;   
             backtransform eigenvectors: x = inv(L)'*y or inv(U)*y
 */

	    if (upper) {
		*(unsigned char *)trans = 'N';
	    } else {
		*(unsigned char *)trans = 'T';
	    }

	    i__1 = neig;
	    for (j = 1; j <= neig; ++j) {
		stpsv_(uplo, trans, "Non-unit", n, &BP(1), &Z(1,j),
			 &c__1);
/* L10: */
	    }

	} else if (*itype == 3) {

/*           For B*A*x=(lambda)*x;   
             backtransform eigenvectors: x = L*y or U'*y */

	    if (upper) {
		*(unsigned char *)trans = 'T';
	    } else {
		*(unsigned char *)trans = 'N';
	    }

	    i__1 = neig;
	    for (j = 1; j <= neig; ++j) {
		stpmv_(uplo, trans, "Non-unit", n, &BP(1), &Z(1,j),
			 &c__1);
/* L20: */
	    }
	}
    }
    return 0;

/*     End of SSPGV */

} /* sspgv_ */

