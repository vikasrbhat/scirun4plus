#include "f2c.h"

/* Subroutine */ int slaed4_(integer *n, integer *i, real *d, real *z, real *
	delta, real *rho, real *dlam, integer *info)
{
/*  -- LAPACK routine (version 2.0) --   
       Univ. of Tennessee, Oak Ridge National Lab, Argonne National Lab, 
  
       Courant Institute, NAG Ltd., and Rice University   
       September 30, 1994   


    Purpose   
    =======   

    This subroutine computes the I-th updated eigenvalue of a symmetric   
    rank-one modification to a diagonal matrix whose elements are   
    given in the array d, and that   

               D(i) < D(j)  for  i < j   

    and that RHO > 0.  This is arranged by the calling routine, and is   
    no loss in generality.  The rank-one modified system is thus   

               diag( D )  +  RHO *  Z * Z_transpose.   

    where we assume the Euclidean norm of Z is 1.   

    The method consists of approximating the rational functions in the   
    secular equation by simpler interpolating rational functions.   

    Arguments   
    =========   

    N      (input) INTEGER   
           The length of all arrays.   

    I      (input) INTEGER   
           The index of the eigenvalue to be computed.  1 <= I <= N.   

    D      (input) REAL array, dimension (N)   
           The original eigenvalues.  It is assumed that they are in   
           order, D(I) < D(J)  for I < J.   

    Z      (input) REAL array, dimension (N)   
           The components of the updating vector.   

    DELTA  (output) REAL array, dimension (N)   
           If N .ne. 1, DELTA contains (D(j) - lambda_I) in its  j-th   
           component.  If N = 1, then DELTA(1) = 1.  The vector DELTA   
           contains the information necessary to construct the   
           eigenvectors.   

    RHO    (input) REAL   
           The scalar in the symmetric updating formula.   

    DLAM   (output) REAL   
           The computed lambda_I, the I-th updated eigenvalue.   

    INFO   (output) INTEGER   
           = 0:  successful exit   
           > 0:  if INFO = 1, the updating process failed.   

    Internal Parameters   
    ===================   

    Logical variable ORGATI (origin-at-i?) is used for distinguishing   
    whether D(i) or D(i+1) is treated as the origin.   

              ORGATI = .true.    origin at i   
              ORGATI = .false.   origin at i+1   

     Logical variable SWTCH3 (switch-for-3-poles?) is for noting   
     if we are working with THREE poles!   

     MAXIT is the maximum number of iterations allowed for each   
     eigenvalue.   

    ===================================================================== 
  


       Since this routine is called in an inner loop, we do no argument   
       checking.   

       Quick return for N=1 and 2.   

    
   Parameter adjustments   
       Function Body */
    /* System generated locals */
    integer i__1;
    real r__1;
    /* Builtin functions */
    double sqrt(doublereal);
    /* Local variables */
    static real dphi, dpsi;
    static integer iter;
    static real temp, prew, temp1, a, b, c;
    static integer j;
    static real w;
    static integer niter;
    static logical swtch;
    extern /* Subroutine */ int slaed5_(integer *, real *, real *, real *, 
	    real *, real *), slaed6_(integer *, logical *, real *, real *, 
	    real *, real *, real *, integer *);
    static logical swtch3;
    static integer ii;
    static real dw;
    extern doublereal slamch_(char *);
    static real zz[3];
    static logical orgati;
    static real erretm, rhoinv;
    static integer ip1;
    static real del, eta, phi, eps, tau, psi;
    static integer iim1, iip1;


#define DELTA(I) delta[(I)-1]
#define Z(I) z[(I)-1]
#define D(I) d[(I)-1]


    *info = 0;
    if (*n == 1) {

/*         Presumably, I=1 upon entry */

	*dlam = D(1) + *rho * Z(1) * Z(1);
	DELTA(1) = 1.f;
	return 0;
    }
    if (*n == 2) {
	slaed5_(i, &D(1), &Z(1), &DELTA(1), rho, dlam);
	return 0;
    }

/*     Compute machine epsilon */

    eps = slamch_("Epsilon");
    rhoinv = 1.f / *rho;

/*     The case I = N */

    if (*i == *n) {

/*        Initialize some basic variables */

	ii = *n - 1;
	niter = 1;

/*        Calculate initial guess */

	temp = *rho / 2.f;

/*        If ||Z||_2 is not one, then TEMP should be set to   
          RHO * ||Z||_2^2 / TWO */

	i__1 = *n;
	for (j = 1; j <= *n; ++j) {
	    DELTA(j) = D(j) - D(*i) - temp;
/* L10: */
	}

	psi = 0.f;
	i__1 = *n - 2;
	for (j = 1; j <= *n-2; ++j) {
	    psi += Z(j) * Z(j) / DELTA(j);
/* L20: */
	}

	c = rhoinv + psi;
	w = c + Z(ii) * Z(ii) / DELTA(ii) + Z(*n) * Z(*n) / DELTA(*n);

	if (w <= 0.f) {
	    temp = Z(*n - 1) * Z(*n - 1) / (D(*n) - D(*n - 1) + *rho) + Z(*n) 
		    * Z(*n) / *rho;
	    if (c <= temp) {
		tau = *rho;
	    } else {
		del = D(*n) - D(*n - 1);
		a = -(doublereal)c * del + Z(*n - 1) * Z(*n - 1) + Z(*n) * Z(*
			n);
		b = Z(*n) * Z(*n) * del;
		if (a < 0.f) {
		    tau = b * 2.f / (sqrt(a * a + b * 4.f * c) - a);
		} else {
		    tau = (a + sqrt(a * a + b * 4.f * c)) / (c * 2.f);
		}
	    }

/*           It can be proved that   
                 D(N)+RHO/2 <= LAMBDA(N) < D(N)+TAU <= D(N)+RHO */

	} else {
	    del = D(*n) - D(*n - 1);
	    a = -(doublereal)c * del + Z(*n - 1) * Z(*n - 1) + Z(*n) * Z(*n);
	    b = Z(*n) * Z(*n) * del;
	    if (a < 0.f) {
		tau = b * 2.f / (sqrt(a * a + b * 4.f * c) - a);
	    } else {
		tau = (a + sqrt(a * a + b * 4.f * c)) / (c * 2.f);
	    }

/*           It can be proved that   
                 D(N) < D(N)+TAU < LAMBDA(N) < D(N)+RHO/2 */

	}

	i__1 = *n;
	for (j = 1; j <= *n; ++j) {
	    DELTA(j) = D(j) - D(*i) - tau;
/* L30: */
	}

/*        Evaluate PSI and the derivative DPSI */

	dpsi = 0.f;
	psi = 0.f;
	erretm = 0.f;
	i__1 = ii;
	for (j = 1; j <= ii; ++j) {
	    temp = Z(j) / DELTA(j);
	    psi += Z(j) * temp;
	    dpsi += temp * temp;
	    erretm += psi;
/* L40: */
	}
	erretm = dabs(erretm);

/*        Evaluate PHI and the derivative DPHI */

	temp = Z(*n) / DELTA(*n);
	phi = Z(*n) * temp;
	dphi = temp * temp;
	erretm = (-(doublereal)phi - psi) * 8.f + erretm - phi + rhoinv + 
		dabs(tau) * (dpsi + dphi);

	w = rhoinv + phi + psi;

/*        Test for convergence */

	if (dabs(w) <= eps * erretm) {
	    *dlam = D(*i) + tau;
	    goto L250;
	}

/*        Calculate the new step */

	++niter;
	c = w - DELTA(*n - 1) * dpsi - DELTA(*n) * dphi;
	a = (DELTA(*n - 1) + DELTA(*n)) * w - DELTA(*n - 1) * DELTA(*n) * (
		dpsi + dphi);
	b = DELTA(*n - 1) * DELTA(*n) * w;
	if (c < 0.f) {
	    c = dabs(c);
	}
	if (c == 0.f) {
/*          ETA = B/A */
	    eta = *rho - tau;
	} else if (a >= 0.f) {
	    eta = (a + sqrt((r__1 = a * a - b * 4.f * c, dabs(r__1)))) / (c * 
		    2.f);
	} else {
	    eta = b * 2.f / (a - sqrt((r__1 = a * a - b * 4.f * c, dabs(r__1))
		    ));
	}

/*        Note, eta should be positive if w is negative, and   
          eta should be negative otherwise. However,   
          if for some reason caused by roundoff, eta*w > 0,   
          we simply use one Newton step instead. This way   
          will guarantee eta*w < 0. */

	if (w * eta > 0.f) {
	    eta = -(doublereal)w / (dpsi + dphi);
	}
	temp = tau + eta;
	if (temp > *rho) {
	    eta = *rho - tau;
	}
	i__1 = *n;
	for (j = 1; j <= *n; ++j) {
	    DELTA(j) -= eta;
/* L50: */
	}

	tau += eta;

/*        Evaluate PSI and the derivative DPSI */

	dpsi = 0.f;
	psi = 0.f;
	erretm = 0.f;
	i__1 = ii;
	for (j = 1; j <= ii; ++j) {
	    temp = Z(j) / DELTA(j);
	    psi += Z(j) * temp;
	    dpsi += temp * temp;
	    erretm += psi;
/* L60: */
	}
	erretm = dabs(erretm);

/*        Evaluate PHI and the derivative DPHI */

	temp = Z(*n) / DELTA(*n);
	phi = Z(*n) * temp;
	dphi = temp * temp;
	erretm = (-(doublereal)phi - psi) * 8.f + erretm - phi + rhoinv + 
		dabs(tau) * (dpsi + dphi);

	w = rhoinv + phi + psi;

/*        Main loop to update the values of the array   DELTA */

	iter = niter + 1;

	for (niter = iter; niter <= 20; ++niter) {

/*           Test for convergence */

	    if (dabs(w) <= eps * erretm) {
		*dlam = D(*i) + tau;
		goto L250;
	    }

/*           Calculate the new step */

	    c = w - DELTA(*n - 1) * dpsi - DELTA(*n) * dphi;
	    a = (DELTA(*n - 1) + DELTA(*n)) * w - DELTA(*n - 1) * DELTA(*n) * 
		    (dpsi + dphi);
	    b = DELTA(*n - 1) * DELTA(*n) * w;
	    if (a >= 0.f) {
		eta = (a + sqrt((r__1 = a * a - b * 4.f * c, dabs(r__1)))) / (
			c * 2.f);
	    } else {
		eta = b * 2.f / (a - sqrt((r__1 = a * a - b * 4.f * c, dabs(
			r__1))));
	    }

/*           Note, eta should be positive if w is negative, and   
             eta should be negative otherwise. However,   
             if for some reason caused by roundoff, eta*w > 0,   
             we simply use one Newton step instead. This way   
             will guarantee eta*w < 0. */

	    if (w * eta > 0.f) {
		eta = -(doublereal)w / (dpsi + dphi);
	    }
	    temp = tau + eta;
	    if (temp <= 0.f) {
		eta /= 2.f;
	    }
	    i__1 = *n;
	    for (j = 1; j <= *n; ++j) {
		DELTA(j) -= eta;
/* L70: */
	    }

	    tau += eta;

/*           Evaluate PSI and the derivative DPSI */

	    dpsi = 0.f;
	    psi = 0.f;
	    erretm = 0.f;
	    i__1 = ii;
	    for (j = 1; j <= ii; ++j) {
		temp = Z(j) / DELTA(j);
		psi += Z(j) * temp;
		dpsi += temp * temp;
		erretm += psi;
/* L80: */
	    }
	    erretm = dabs(erretm);

/*           Evaluate PHI and the derivative DPHI */

	    temp = Z(*n) / DELTA(*n);
	    phi = Z(*n) * temp;
	    dphi = temp * temp;
	    erretm = (-(doublereal)phi - psi) * 8.f + erretm - phi + rhoinv + 
		    dabs(tau) * (dpsi + dphi);

	    w = rhoinv + phi + psi;
/* L90: */
	}

/*        Return with INFO = 1, NITER = MAXIT and not converged */

	*info = 1;
	*dlam = D(*i) + tau;
	goto L250;

/*        End for the case I = N */

    } else {

/*        The case for I < N */

	niter = 1;
	ip1 = *i + 1;

/*        Calculate initial guess */

	temp = (D(ip1) - D(*i)) / 2.f;
	i__1 = *n;
	for (j = 1; j <= *n; ++j) {
	    DELTA(j) = D(j) - D(*i) - temp;
/* L100: */
	}

	psi = 0.f;
	i__1 = *i - 1;
	for (j = 1; j <= *i-1; ++j) {
	    psi += Z(j) * Z(j) / DELTA(j);
/* L110: */
	}

	phi = 0.f;
	i__1 = *i + 2;
	for (j = *n; j >= *i+2; --j) {
	    phi += Z(j) * Z(j) / DELTA(j);
/* L120: */
	}
	c = rhoinv + psi + phi;
	w = c + Z(*i) * Z(*i) / DELTA(*i) + Z(ip1) * Z(ip1) / DELTA(ip1);

	if (w > 0.f) {

/*           d(i)< the ith eigenvalue < (d(i)+d(i+1))/2   

             We choose d(i) as origin. */

	    orgati = TRUE_;
	    del = D(ip1) - D(*i);
	    a = c * del + Z(*i) * Z(*i) + Z(ip1) * Z(ip1);
	    b = Z(*i) * Z(*i) * del;
	    if (a > 0.f) {
		tau = b * 2.f / (a + sqrt((r__1 = a * a - b * 4.f * c, dabs(
			r__1))));
	    } else {
		tau = (a - sqrt((r__1 = a * a - b * 4.f * c, dabs(r__1)))) / (
			c * 2.f);
	    }
	} else {

/*           (d(i)+d(i+1))/2 <= the ith eigenvalue < d(i+1)   

             We choose d(i+1) as origin. */

	    orgati = FALSE_;
	    del = D(ip1) - D(*i);
	    a = c * del - Z(*i) * Z(*i) - Z(ip1) * Z(ip1);
	    b = Z(ip1) * Z(ip1) * del;
	    if (a < 0.f) {
		tau = b * 2.f / (a - sqrt((r__1 = a * a + b * 4.f * c, dabs(
			r__1))));
	    } else {
		tau = -(doublereal)(a + sqrt((r__1 = a * a + b * 4.f * c, 
			dabs(r__1)))) / (c * 2.f);
	    }
	}

	if (orgati) {
	    i__1 = *n;
	    for (j = 1; j <= *n; ++j) {
		DELTA(j) = D(j) - D(*i) - tau;
/* L130: */
	    }
	} else {
	    i__1 = *n;
	    for (j = 1; j <= *n; ++j) {
		DELTA(j) = D(j) - D(ip1) - tau;
/* L140: */
	    }
	}
	if (orgati) {
	    ii = *i;
	} else {
	    ii = *i + 1;
	}
	iim1 = ii - 1;
	iip1 = ii + 1;

/*        Evaluate PSI and the derivative DPSI */

	dpsi = 0.f;
	psi = 0.f;
	erretm = 0.f;
	i__1 = iim1;
	for (j = 1; j <= iim1; ++j) {
	    temp = Z(j) / DELTA(j);
	    psi += Z(j) * temp;
	    dpsi += temp * temp;
	    erretm += psi;
/* L150: */
	}
	erretm = dabs(erretm);

/*        Evaluate PHI and the derivative DPHI */

	dphi = 0.f;
	phi = 0.f;
	i__1 = iip1;
	for (j = *n; j >= iip1; --j) {
	    temp = Z(j) / DELTA(j);
	    phi += Z(j) * temp;
	    dphi += temp * temp;
	    erretm += phi;
/* L160: */
	}

	w = rhoinv + phi + psi;

/*        W is the value of the secular function with   
          its ii-th element removed. */

	swtch3 = FALSE_;
	if (orgati) {
	    if (w < 0.f) {
		swtch3 = TRUE_;
	    }
	} else {
	    if (w > 0.f) {
		swtch3 = TRUE_;
	    }
	}
	if (ii == 1 || ii == *n) {
	    swtch3 = FALSE_;
	}

	temp = Z(ii) / DELTA(ii);
	dw = dpsi + dphi + temp * temp;
	temp = Z(ii) * temp;
	w += temp;
	erretm = (phi - psi) * 8.f + erretm + rhoinv * 2.f + dabs(temp) * 3.f 
		+ dabs(tau) * dw;

/*        Test for convergence */

	if (dabs(w) <= eps * erretm) {
	    if (orgati) {
		*dlam = D(*i) + tau;
	    } else {
		*dlam = D(ip1) + tau;
	    }
	    goto L250;
	}

/*        Calculate the new step */

	++niter;
	if (! swtch3) {
	    if (orgati) {
/* Computing 2nd power */
		r__1 = Z(*i) / DELTA(*i);
		c = w - DELTA(ip1) * dw - (D(*i) - D(ip1)) * (r__1 * r__1);
	    } else {
/* Computing 2nd power */
		r__1 = Z(ip1) / DELTA(ip1);
		c = w - DELTA(*i) * dw - (D(ip1) - D(*i)) * (r__1 * r__1);
	    }
	    a = (DELTA(*i) + DELTA(ip1)) * w - DELTA(*i) * DELTA(ip1) * dw;
	    b = DELTA(*i) * DELTA(ip1) * w;
	    if (c == 0.f) {
		if (a == 0.f) {
		    if (orgati) {
			a = Z(*i) * Z(*i) + DELTA(ip1) * DELTA(ip1) * (dpsi + 
				dphi);
		    } else {
			a = Z(ip1) * Z(ip1) + DELTA(*i) * DELTA(*i) * (dpsi + 
				dphi);
		    }
		}
		eta = b / a;
	    } else if (a <= 0.f) {
		eta = (a - sqrt((r__1 = a * a - b * 4.f * c, dabs(r__1)))) / (
			c * 2.f);
	    } else {
		eta = b * 2.f / (a + sqrt((r__1 = a * a - b * 4.f * c, dabs(
			r__1))));
	    }
	} else {

/*           Interpolation using THREE most relevant poles */

	    temp = rhoinv + psi + phi;
	    if (orgati) {
		temp1 = Z(iim1) / DELTA(iim1);
		temp1 *= temp1;
		c = temp - DELTA(iip1) * (dpsi + dphi) - (D(iim1) - D(iip1)) *
			 temp1;
		zz[0] = Z(iim1) * Z(iim1);
		zz[2] = DELTA(iip1) * DELTA(iip1) * (dpsi - temp1 + dphi);
	    } else {
		temp1 = Z(iip1) / DELTA(iip1);
		temp1 *= temp1;
		c = temp - DELTA(iim1) * (dpsi + dphi) - (D(iip1) - D(iim1)) *
			 temp1;
		zz[0] = DELTA(iim1) * DELTA(iim1) * (dpsi + (dphi - temp1));
		zz[2] = Z(iip1) * Z(iip1);
	    }
	    zz[1] = Z(ii) * Z(ii);
	    slaed6_(&niter, &orgati, &c, &DELTA(iim1), zz, &w, &eta, info);
	    if (*info != 0) {
		goto L250;
	    }
	}

/*        Note, eta should be positive if w is negative, and   
          eta should be negative otherwise. However,   
          if for some reason caused by roundoff, eta*w > 0,   
          we simply use one Newton step instead. This way   
          will guarantee eta*w < 0. */

	if (w * eta >= 0.f) {
	    eta = -(doublereal)w / dw;
	}
	temp = tau + eta;
	del = (D(ip1) - D(*i)) / 2.f;
	if (orgati) {
	    if (temp >= del) {
		eta = del - tau;
	    }
	    if (temp <= 0.f) {
		eta /= 2.f;
	    }
	} else {
	    if (temp <= -(doublereal)del) {
		eta = -(doublereal)del - tau;
	    }
	    if (temp >= 0.f) {
		eta /= 2.f;
	    }
	}

	prew = w;

/* L170: */
	i__1 = *n;
	for (j = 1; j <= *n; ++j) {
	    DELTA(j) -= eta;
/* L180: */
	}

/*        Evaluate PSI and the derivative DPSI */

	dpsi = 0.f;
	psi = 0.f;
	erretm = 0.f;
	i__1 = iim1;
	for (j = 1; j <= iim1; ++j) {
	    temp = Z(j) / DELTA(j);
	    psi += Z(j) * temp;
	    dpsi += temp * temp;
	    erretm += psi;
/* L190: */
	}
	erretm = dabs(erretm);

/*        Evaluate PHI and the derivative DPHI */

	dphi = 0.f;
	phi = 0.f;
	i__1 = iip1;
	for (j = *n; j >= iip1; --j) {
	    temp = Z(j) / DELTA(j);
	    phi += Z(j) * temp;
	    dphi += temp * temp;
	    erretm += phi;
/* L200: */
	}

	temp = Z(ii) / DELTA(ii);
	dw = dpsi + dphi + temp * temp;
	temp = Z(ii) * temp;
	w = rhoinv + phi + psi + temp;
	erretm = (phi - psi) * 8.f + erretm + rhoinv * 2.f + dabs(temp) * 3.f 
		+ (r__1 = tau + eta, dabs(r__1)) * dw;

	swtch = FALSE_;
	if (orgati) {
	    if (-(doublereal)w > dabs(prew) / 10.f) {
		swtch = TRUE_;
	    }
	} else {
	    if (w > dabs(prew) / 10.f) {
		swtch = TRUE_;
	    }
	}

	tau += eta;

/*        Main loop to update the values of the array   DELTA */

	iter = niter + 1;

	for (niter = iter; niter <= 20; ++niter) {

/*           Test for convergence */

	    if (dabs(w) <= eps * erretm) {
		if (orgati) {
		    *dlam = D(*i) + tau;
		} else {
		    *dlam = D(ip1) + tau;
		}
		goto L250;
	    }

/*           Calculate the new step */

	    if (! swtch3) {
		if (! swtch) {
		    if (orgati) {
/* Computing 2nd power */
			r__1 = Z(*i) / DELTA(*i);
			c = w - DELTA(ip1) * dw - (D(*i) - D(ip1)) * (r__1 * 
				r__1);
		    } else {
/* Computing 2nd power */
			r__1 = Z(ip1) / DELTA(ip1);
			c = w - DELTA(*i) * dw - (D(ip1) - D(*i)) * (r__1 * 
				r__1);
		    }
		} else {
		    temp = Z(ii) / DELTA(ii);
		    if (orgati) {
			dpsi += temp * temp;
		    } else {
			dphi += temp * temp;
		    }
		    c = w - DELTA(*i) * dpsi - DELTA(ip1) * dphi;
		}
		a = (DELTA(*i) + DELTA(ip1)) * w - DELTA(*i) * DELTA(ip1) * 
			dw;
		b = DELTA(*i) * DELTA(ip1) * w;
		if (c == 0.f) {
		    if (a == 0.f) {
			if (! swtch) {
			    if (orgati) {
				a = Z(*i) * Z(*i) + DELTA(ip1) * DELTA(ip1) * 
					(dpsi + dphi);
			    } else {
				a = Z(ip1) * Z(ip1) + DELTA(*i) * DELTA(*i) * 
					(dpsi + dphi);
			    }
			} else {
			    a = DELTA(*i) * DELTA(*i) * dpsi + DELTA(ip1) * 
				    DELTA(ip1) * dphi;
			}
		    }
		    eta = b / a;
		} else if (a <= 0.f) {
		    eta = (a - sqrt((r__1 = a * a - b * 4.f * c, dabs(r__1))))
			     / (c * 2.f);
		} else {
		    eta = b * 2.f / (a + sqrt((r__1 = a * a - b * 4.f * c, 
			    dabs(r__1))));
		}
	    } else {

/*              Interpolation using THREE most relevant poles 
*/

		temp = rhoinv + psi + phi;
		if (swtch) {
		    c = temp - DELTA(iim1) * dpsi - DELTA(iip1) * dphi;
		    zz[0] = DELTA(iim1) * DELTA(iim1) * dpsi;
		    zz[2] = DELTA(iip1) * DELTA(iip1) * dphi;
		} else {
		    if (orgati) {
			temp1 = Z(iim1) / DELTA(iim1);
			temp1 *= temp1;
			c = temp - DELTA(iip1) * (dpsi + dphi) - (D(iim1) - D(
				iip1)) * temp1;
			zz[0] = Z(iim1) * Z(iim1);
			zz[2] = DELTA(iip1) * DELTA(iip1) * (dpsi - temp1 + 
				dphi);
		    } else {
			temp1 = Z(iip1) / DELTA(iip1);
			temp1 *= temp1;
			c = temp - DELTA(iim1) * (dpsi + dphi) - (D(iip1) - D(
				iim1)) * temp1;
			zz[0] = DELTA(iim1) * DELTA(iim1) * (dpsi + (dphi - 
				temp1));
			zz[2] = Z(iip1) * Z(iip1);
		    }
		}
		slaed6_(&niter, &orgati, &c, &DELTA(iim1), zz, &w, &eta, info)
			;
		if (*info != 0) {
		    goto L250;
		}
	    }

/*           Note, eta should be positive if w is negative, and   
             eta should be negative otherwise. However,   
             if for some reason caused by roundoff, eta*w > 0,   
             we simply use one Newton step instead. This way   
             will guarantee eta*w < 0. */

	    if (w * eta >= 0.f) {
		eta = -(doublereal)w / dw;
	    }
	    temp = tau + eta;
	    del = (D(ip1) - D(*i)) / 2.f;
	    if (orgati) {
		if (temp >= del) {
		    eta = del - tau;
		}
		if (temp <= 0.f) {
		    eta /= 2.f;
		}
	    } else {
		if (temp <= -(doublereal)del) {
		    eta = -(doublereal)del - tau;
		}
		if (temp >= 0.f) {
		    eta /= 2.f;
		}
	    }

	    i__1 = *n;
	    for (j = 1; j <= *n; ++j) {
		DELTA(j) -= eta;
/* L210: */
	    }

	    tau += eta;
	    prew = w;

/*           Evaluate PSI and the derivative DPSI */

	    dpsi = 0.f;
	    psi = 0.f;
	    erretm = 0.f;
	    i__1 = iim1;
	    for (j = 1; j <= iim1; ++j) {
		temp = Z(j) / DELTA(j);
		psi += Z(j) * temp;
		dpsi += temp * temp;
		erretm += psi;
/* L220: */
	    }
	    erretm = dabs(erretm);

/*           Evaluate PHI and the derivative DPHI */

	    dphi = 0.f;
	    phi = 0.f;
	    i__1 = iip1;
	    for (j = *n; j >= iip1; --j) {
		temp = Z(j) / DELTA(j);
		phi += Z(j) * temp;
		dphi += temp * temp;
		erretm += phi;
/* L230: */
	    }

	    temp = Z(ii) / DELTA(ii);
	    dw = dpsi + dphi + temp * temp;
	    temp = Z(ii) * temp;
	    w = rhoinv + phi + psi + temp;
	    erretm = (phi - psi) * 8.f + erretm + rhoinv * 2.f + dabs(temp) * 
		    3.f + dabs(tau) * dw;
	    if (w * prew > 0.f && dabs(w) > dabs(prew) / 10.f) {
		swtch = ! swtch;
	    }

/* L240: */
	}

/*        Return with INFO = 1, NITER = MAXIT and not converged */

	*info = 1;
	if (orgati) {
	    *dlam = D(*i) + tau;
	} else {
	    *dlam = D(ip1) + tau;
	}

    }

L250:
    return 0;

/*     End of SLAED4 */

} /* slaed4_ */

