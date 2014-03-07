#include "f2c.h"

/* Subroutine */ int shseqr_(char *job, char *compz, integer *n, integer *ilo,
	 integer *ihi, real *h, integer *ldh, real *wr, real *wi, real *z, 
	integer *ldz, real *work, integer *lwork, integer *info)
{
/*  -- LAPACK routine (version 2.0) --   
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,   
       Courant Institute, Argonne National Lab, and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    SHSEQR computes the eigenvalues of a real upper Hessenberg matrix H   
    and, optionally, the matrices T and Z from the Schur decomposition   
    H = Z T Z**T, where T is an upper quasi-triangular matrix (the Schur 
  
    form), and Z is the orthogonal matrix of Schur vectors.   

    Optionally Z may be postmultiplied into an input orthogonal matrix Q, 
  
    so that this routine can give the Schur factorization of a matrix A   
    which has been reduced to the Hessenberg form H by the orthogonal   
    matrix Q:  A = Q*H*Q**T = (QZ)*T*(QZ)**T.   

    Arguments   
    =========   

    JOB     (input) CHARACTER*1   
            = 'E':  compute eigenvalues only;   
            = 'S':  compute eigenvalues and the Schur form T.   

    COMPZ   (input) CHARACTER*1   
            = 'N':  no Schur vectors are computed;   
            = 'I':  Z is initialized to the unit matrix and the matrix Z 
  
                    of Schur vectors of H is returned;   
            = 'V':  Z must contain an orthogonal matrix Q on entry, and   
                    the product Q*Z is returned.   

    N       (input) INTEGER   
            The order of the matrix H.  N >= 0.   

    ILO     (input) INTEGER   
    IHI     (input) INTEGER   
            It is assumed that H is already upper triangular in rows   
            and columns 1:ILO-1 and IHI+1:N. ILO and IHI are normally   
            set by a previous call to SGEBAL, and then passed to SGEHRD   
            when the matrix output by SGEBAL is reduced to Hessenberg   
            form. Otherwise ILO and IHI should be set to 1 and N   
            respectively.   
            1 <= ILO <= IHI <= N, if N > 0; ILO=1 and IHI=0, if N=0.   

    H       (input/output) REAL array, dimension (LDH,N)   
            On entry, the upper Hessenberg matrix H.   
            On exit, if JOB = 'S', H contains the upper quasi-triangular 
  
            matrix T from the Schur decomposition (the Schur form);   
            2-by-2 diagonal blocks (corresponding to complex conjugate   
            pairs of eigenvalues) are returned in standard form, with   
            H(i,i) = H(i+1,i+1) and H(i+1,i)*H(i,i+1) < 0. If JOB = 'E', 
  
            the contents of H are unspecified on exit.   

    LDH     (input) INTEGER   
            The leading dimension of the array H. LDH >= max(1,N).   

    WR      (output) REAL array, dimension (N)   
    WI      (output) REAL array, dimension (N)   
            The real and imaginary parts, respectively, of the computed   
            eigenvalues. If two eigenvalues are computed as a complex   
            conjugate pair, they are stored in consecutive elements of   
            WR and WI, say the i-th and (i+1)th, with WI(i) > 0 and   
            WI(i+1) < 0. If JOB = 'S', the eigenvalues are stored in the 
  
            same order as on the diagonal of the Schur form returned in   
            H, with WR(i) = H(i,i) and, if H(i:i+1,i:i+1) is a 2-by-2   
            diagonal block, WI(i) = sqrt(H(i+1,i)*H(i,i+1)) and   
            WI(i+1) = -WI(i).   

    Z       (input/output) REAL array, dimension (LDZ,N)   
            If COMPZ = 'N': Z is not referenced.   
            If COMPZ = 'I': on entry, Z need not be set, and on exit, Z   
            contains the orthogonal matrix Z of the Schur vectors of H.   
            If COMPZ = 'V': on entry Z must contain an N-by-N matrix Q,   
            which is assumed to be equal to the unit matrix except for   
            the submatrix Z(ILO:IHI,ILO:IHI); on exit Z contains Q*Z.   
            Normally Q is the orthogonal matrix generated by SORGHR after 
  
            the call to SGEHRD which formed the Hessenberg matrix H.   

    LDZ     (input) INTEGER   
            The leading dimension of the array Z.   
            LDZ >= max(1,N) if COMPZ = 'I' or 'V'; LDZ >= 1 otherwise.   

    WORK    (workspace) REAL array, dimension (N)   

    LWORK   (input) INTEGER   
            This argument is currently redundant.   

    INFO    (output) INTEGER   
            = 0:  successful exit   
            < 0:  if INFO = -i, the i-th argument had an illegal value   
            > 0:  if INFO = i, SHSEQR failed to compute all of the   
                  eigenvalues in a total of 30*(IHI-ILO+1) iterations;   
                  elements 1:ilo-1 and i+1:n of WR and WI contain those   
                  eigenvalues which have been successfully computed.   

    ===================================================================== 
  


       Decode and test the input parameters   

    
   Parameter adjustments   
       Function Body */
    /* Table of constant values */
    static real c_b9 = 0.f;
    static real c_b10 = 1.f;
    static integer c__4 = 4;
    static integer c_n1 = -1;
    static integer c__2 = 2;
    static integer c__8 = 8;
    static integer c__15 = 15;
    static logical c_false = FALSE_;
    static integer c__1 = 1;
    
    /* System generated locals */
    address a__1[2];
    integer h_dim1, h_offset, z_dim1, z_offset, i__1, i__2, i__3[2], i__4, 
	    i__5;
    real r__1, r__2;
    char ch__1[2];
    /* Builtin functions   
       Subroutine */ int s_cat(char *, char **, integer *, integer *, ftnlen);
    /* Local variables */
    static integer maxb;
    static real absw;
    static integer ierr;
    static real unfl, temp, ovfl;
    static integer i, j, k, l;
    static real s[225]	/* was [15][15] */, v[16];
    extern logical lsame_(char *, char *);
    extern /* Subroutine */ int sscal_(integer *, real *, real *, integer *);
    static integer itemp;
    extern /* Subroutine */ int sgemv_(char *, integer *, integer *, real *, 
	    real *, integer *, real *, integer *, real *, real *, integer *);
    static integer i1, i2;
    static logical initz, wantt;
    extern /* Subroutine */ int scopy_(integer *, real *, integer *, real *, 
	    integer *);
    static logical wantz;
    extern doublereal slapy2_(real *, real *);
    static integer ii, nh;
    extern /* Subroutine */ int slabad_(real *, real *);
    static integer nr, ns, nv;
    extern doublereal slamch_(char *);
    static real vv[16];
    extern /* Subroutine */ int xerbla_(char *, integer *);
    extern integer ilaenv_(integer *, char *, char *, integer *, integer *, 
	    integer *, integer *, ftnlen, ftnlen);
    extern /* Subroutine */ int slarfg_(integer *, real *, real *, integer *, 
	    real *);
    extern integer isamax_(integer *, real *, integer *);
    extern doublereal slanhs_(char *, integer *, real *, integer *, real *);
    extern /* Subroutine */ int slahqr_(logical *, logical *, integer *, 
	    integer *, integer *, real *, integer *, real *, real *, integer *
	    , integer *, real *, integer *, integer *), slacpy_(char *, 
	    integer *, integer *, real *, integer *, real *, integer *), slaset_(char *, integer *, integer *, real *, real *, 
	    real *, integer *), slarfx_(char *, integer *, integer *, 
	    real *, real *, real *, integer *, real *);
    static real smlnum;
    static integer itn;
    static real tau;
    static integer its;
    static real ulp, tst1;



#define WR(I) wr[(I)-1]
#define WI(I) wi[(I)-1]
#define WORK(I) work[(I)-1]

#define H(I,J) h[(I)-1 + ((J)-1)* ( *ldh)]
#define Z(I,J) z[(I)-1 + ((J)-1)* ( *ldz)]

    wantt = lsame_(job, "S");
    initz = lsame_(compz, "I");
    wantz = initz || lsame_(compz, "V");

    *info = 0;
    if (! lsame_(job, "E") && ! wantt) {
	*info = -1;
    } else if (! lsame_(compz, "N") && ! wantz) {
	*info = -2;
    } else if (*n < 0) {
	*info = -3;
    } else if (*ilo < 1 || *ilo > max(1,*n)) {
	*info = -4;
    } else if (*ihi < min(*ilo,*n) || *ihi > *n) {
	*info = -5;
    } else if (*ldh < max(1,*n)) {
	*info = -7;
    } else if (*ldz < 1 || wantz && *ldz < max(1,*n)) {
	*info = -11;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("SHSEQR", &i__1);
	return 0;
    }

/*     Initialize Z, if necessary */

    if (initz) {
	slaset_("Full", n, n, &c_b9, &c_b10, &Z(1,1), ldz);
    }

/*     Store the eigenvalues isolated by SGEBAL. */

    i__1 = *ilo - 1;
    for (i = 1; i <= *ilo-1; ++i) {
	WR(i) = H(i,i);
	WI(i) = 0.f;
/* L10: */
    }
    i__1 = *n;
    for (i = *ihi + 1; i <= *n; ++i) {
	WR(i) = H(i,i);
	WI(i) = 0.f;
/* L20: */
    }

/*     Quick return if possible. */

    if (*n == 0) {
	return 0;
    }
    if (*ilo == *ihi) {
	WR(*ilo) = H(*ilo,*ilo);
	WI(*ilo) = 0.f;
	return 0;
    }

/*     Set rows and columns ILO to IHI to zero below the first   
       subdiagonal. */

    i__1 = *ihi - 2;
    for (j = *ilo; j <= *ihi-2; ++j) {
	i__2 = *n;
	for (i = j + 2; i <= *n; ++i) {
	    H(i,j) = 0.f;
/* L30: */
	}
/* L40: */
    }
    nh = *ihi - *ilo + 1;

/*     Determine the order of the multi-shift QR algorithm to be used.   

   Writing concatenation */
    i__3[0] = 1, a__1[0] = job;
    i__3[1] = 1, a__1[1] = compz;
    s_cat(ch__1, a__1, i__3, &c__2, 2L);
    ns = ilaenv_(&c__4, "SHSEQR", ch__1, n, ilo, ihi, &c_n1, 6L, 2L);
/* Writing concatenation */
    i__3[0] = 1, a__1[0] = job;
    i__3[1] = 1, a__1[1] = compz;
    s_cat(ch__1, a__1, i__3, &c__2, 2L);
    maxb = ilaenv_(&c__8, "SHSEQR", ch__1, n, ilo, ihi, &c_n1, 6L, 2L);
    if (ns <= 2 || ns > nh || maxb >= nh) {

/*        Use the standard double-shift algorithm */

	slahqr_(&wantt, &wantz, n, ilo, ihi, &H(1,1), ldh, &WR(1), &WI(1)
		, ilo, ihi, &Z(1,1), ldz, info);
	return 0;
    }
    maxb = max(3,maxb);
/* Computing MIN */
    i__1 = min(ns,maxb);
    ns = min(i__1,15);

/*     Now 2 < NS <= MAXB < NH.   

       Set machine-dependent constants for the stopping criterion.   
       If norm(H) <= sqrt(OVFL), overflow should not occur. */

    unfl = slamch_("Safe minimum");
    ovfl = 1.f / unfl;
    slabad_(&unfl, &ovfl);
    ulp = slamch_("Precision");
    smlnum = unfl * (nh / ulp);

/*     I1 and I2 are the indices of the first row and last column of H   
       to which transformations must be applied. If eigenvalues only are 
  
       being computed, I1 and I2 are set inside the main loop. */

    if (wantt) {
	i1 = 1;
	i2 = *n;
    }

/*     ITN is the total number of multiple-shift QR iterations allowed. */

    itn = nh * 30;

/*     The main loop begins here. I is the loop index and decreases from 
  
       IHI to ILO in steps of at most MAXB. Each iteration of the loop   
       works with the active submatrix in rows and columns L to I.   
       Eigenvalues I+1 to IHI have already converged. Either L = ILO or   
       H(L,L-1) is negligible so that the matrix splits. */

    i = *ihi;
L50:
    l = *ilo;
    if (i < *ilo) {
	goto L170;
    }

/*     Perform multiple-shift QR iterations on rows and columns ILO to I 
  
       until a submatrix of order at most MAXB splits off at the bottom   
       because a subdiagonal element has become negligible. */

    i__1 = itn;
    for (its = 0; its <= itn; ++its) {

/*        Look for a single small subdiagonal element. */

	i__2 = l + 1;
	for (k = i; k >= l+1; --k) {
	    tst1 = (r__1 = H(k-1,k-1), dabs(r__1)) + (r__2 = 
		    H(k,k), dabs(r__2));
	    if (tst1 == 0.f) {
		i__4 = i - l + 1;
		tst1 = slanhs_("1", &i__4, &H(l,l), ldh, &WORK(1));
	    }
/* Computing MAX */
	    r__2 = ulp * tst1;
	    if ((r__1 = H(k,k-1), dabs(r__1)) <= dmax(r__2,
		    smlnum)) {
		goto L70;
	    }
/* L60: */
	}
L70:
	l = k;
	if (l > *ilo) {

/*           H(L,L-1) is negligible. */

	    H(l,l-1) = 0.f;
	}

/*        Exit from loop if a submatrix of order <= MAXB has split off
. */

	if (l >= i - maxb + 1) {
	    goto L160;
	}

/*        Now the active submatrix is in rows and columns L to I. If 
  
          eigenvalues only are being computed, only the active submatr
ix   
          need be transformed. */

	if (! wantt) {
	    i1 = l;
	    i2 = i;
	}

	if (its == 20 || its == 30) {

/*           Exceptional shifts. */

	    i__2 = i;
	    for (ii = i - ns + 1; ii <= i; ++ii) {
		WR(ii) = ((r__1 = H(ii,ii-1), dabs(r__1)) + (
			r__2 = H(ii,ii), dabs(r__2))) * 1.5f;
		WI(ii) = 0.f;
/* L80: */
	    }
	} else {

/*           Use eigenvalues of trailing submatrix of order NS as 
shifts. */

	    slacpy_("Full", &ns, &ns, &H(i-ns+1,i-ns+1), 
		    ldh, s, &c__15);
	    slahqr_(&c_false, &c_false, &ns, &c__1, &ns, s, &c__15, &WR(i - 
		    ns + 1), &WI(i - ns + 1), &c__1, &ns, &Z(1,1), ldz, &
		    ierr);
	    if (ierr > 0) {

/*              If SLAHQR failed to compute all NS eigenvalues
, use the   
                unconverged diagonal elements as the remaining
 shifts. */

		i__2 = ierr;
		for (ii = 1; ii <= ierr; ++ii) {
		    WR(i - ns + ii) = s[ii + ii * 15 - 16];
		    WI(i - ns + ii) = 0.f;
/* L90: */
		}
	    }
	}

/*        Form the first column of (G-w(1)) (G-w(2)) . . . (G-w(ns)) 
  
          where G is the Hessenberg submatrix H(L:I,L:I) and w is   
          the vector of shifts (stored in WR and WI). The result is   
          stored in the local array V. */

	v[0] = 1.f;
	i__2 = ns + 1;
	for (ii = 2; ii <= ns+1; ++ii) {
	    v[ii - 1] = 0.f;
/* L100: */
	}
	nv = 1;
	i__2 = i;
	for (j = i - ns + 1; j <= i; ++j) {
	    if (WI(j) >= 0.f) {
		if (WI(j) == 0.f) {

/*                 real shift */

		    i__4 = nv + 1;
		    scopy_(&i__4, v, &c__1, vv, &c__1);
		    i__4 = nv + 1;
		    r__1 = -(doublereal)WR(j);
		    sgemv_("No transpose", &i__4, &nv, &c_b10, &H(l,l), ldh, vv, &c__1, &r__1, v, &c__1);
		    ++nv;
		} else if (WI(j) > 0.f) {

/*                 complex conjugate pair of shifts */

		    i__4 = nv + 1;
		    scopy_(&i__4, v, &c__1, vv, &c__1);
		    i__4 = nv + 1;
		    r__1 = WR(j) * -2.f;
		    sgemv_("No transpose", &i__4, &nv, &c_b10, &H(l,l), ldh, v, &c__1, &r__1, vv, &c__1);
		    i__4 = nv + 1;
		    itemp = isamax_(&i__4, vv, &c__1);
/* Computing MAX */
		    r__2 = (r__1 = vv[itemp - 1], dabs(r__1));
		    temp = 1.f / dmax(r__2,smlnum);
		    i__4 = nv + 1;
		    sscal_(&i__4, &temp, vv, &c__1);
		    absw = slapy2_(&WR(j), &WI(j));
		    temp = temp * absw * absw;
		    i__4 = nv + 2;
		    i__5 = nv + 1;
		    sgemv_("No transpose", &i__4, &i__5, &c_b10, &H(l,l), ldh, vv, &c__1, &temp, v, &c__1);
		    nv += 2;
		}

/*              Scale V(1:NV) so that max(abs(V(i))) = 1. If V
 is zero,   
                reset it to the unit vector. */

		itemp = isamax_(&nv, v, &c__1);
		temp = (r__1 = v[itemp - 1], dabs(r__1));
		if (temp == 0.f) {
		    v[0] = 1.f;
		    i__4 = nv;
		    for (ii = 2; ii <= nv; ++ii) {
			v[ii - 1] = 0.f;
/* L110: */
		    }
		} else {
		    temp = dmax(temp,smlnum);
		    r__1 = 1.f / temp;
		    sscal_(&nv, &r__1, v, &c__1);
		}
	    }
/* L120: */
	}

/*        Multiple-shift QR step */

	i__2 = i - 1;
	for (k = l; k <= i-1; ++k) {

/*           The first iteration of this loop determines a reflect
ion G   
             from the vector V and applies it from left and right 
to H,   
             thus creating a nonzero bulge below the subdiagonal. 
  

             Each subsequent iteration determines a reflection G t
o   
             restore the Hessenberg form in the (K-1)th column, an
d thus   
             chases the bulge one step toward the bottom of the ac
tive   
             submatrix. NR is the order of G.   

   Computing MIN */
	    i__4 = ns + 1, i__5 = i - k + 1;
	    nr = min(i__4,i__5);
	    if (k > l) {
		scopy_(&nr, &H(k,k-1), &c__1, v, &c__1);
	    }
	    slarfg_(&nr, v, &v[1], &c__1, &tau);
	    if (k > l) {
		H(k,k-1) = v[0];
		i__4 = i;
		for (ii = k + 1; ii <= i; ++ii) {
		    H(ii,k-1) = 0.f;
/* L130: */
		}
	    }
	    v[0] = 1.f;

/*           Apply G from the left to transform the rows of the ma
trix in   
             columns K to I2. */

	    i__4 = i2 - k + 1;
	    slarfx_("Left", &nr, &i__4, v, &tau, &H(k,k), ldh, &
		    WORK(1));

/*           Apply G from the right to transform the columns of th
e   
             matrix in rows I1 to min(K+NR,I).   

   Computing MIN */
	    i__5 = k + nr;
	    i__4 = min(i__5,i) - i1 + 1;
	    slarfx_("Right", &i__4, &nr, v, &tau, &H(i1,k), ldh, &
		    WORK(1));

	    if (wantz) {

/*              Accumulate transformations in the matrix Z */

		slarfx_("Right", &nh, &nr, v, &tau, &Z(*ilo,k), 
			ldz, &WORK(1));
	    }
/* L140: */
	}

/* L150: */
    }

/*     Failure to converge in remaining number of iterations */

    *info = i;
    return 0;

L160:

/*     A submatrix of order <= MAXB in rows and columns L to I has split 
  
       off. Use the double-shift QR algorithm to handle it. */

    slahqr_(&wantt, &wantz, n, &l, &i, &H(1,1), ldh, &WR(1), &WI(1), ilo,
	     ihi, &Z(1,1), ldz, info);
    if (*info > 0) {
	return 0;
    }

/*     Decrement number of remaining iterations, and return to start of   
       the main loop with a new value of I. */

    itn -= its;
    i = l - 1;
    goto L50;

L170:
    return 0;

/*     End of SHSEQR */

} /* shseqr_ */

