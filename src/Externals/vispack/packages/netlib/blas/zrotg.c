/* zrotg.f -- translated by f2c (version of 23 May 1992  14:18:33).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"
#define cdabs_ z_abs /*Fix to non-standard complex double abs */

/* Subroutine */ int zrotg_(doublecomplex *ca, doublecomplex *cb, doublereal *
	c, doublecomplex *s)
{
    /* System generated locals */
    doublereal d__1, d__2;
    doublecomplex z__1, z__2, z__3, z__4;

    /* Builtin functions */
    void z_div(doublecomplex *, doublecomplex *, doublecomplex *);
    double sqrt(doublereal);
    void d_cnjg(doublecomplex *, doublecomplex *);

    /* Local variables */
    static doublereal norm;
    extern doublereal cdabs_(doublecomplex *);
    static doublecomplex alpha;
    static doublereal scale;

    if (cdabs_(ca) != 0.) {
	goto L10;
    }
    *c = 0.;
    s->r = 1., s->i = 0.;
    ca->r = cb->r, ca->i = cb->i;
    goto L20;
L10:
    scale = cdabs_(ca) + cdabs_(cb);
    z__2.r = scale, z__2.i = 0.;
    z_div(&z__1, ca, &z__2);
/* Computing 2nd power */
    d__1 = cdabs_(&z__1);
    z__4.r = scale, z__4.i = 0.;
    z_div(&z__3, cb, &z__4);
/* Computing 2nd power */
    d__2 = cdabs_(&z__3);
    norm = scale * sqrt(d__1 * d__1 + d__2 * d__2);
    d__1 = cdabs_(ca);
    z__1.r = ca->r / d__1, z__1.i = ca->i / d__1;
    alpha.r = z__1.r, alpha.i = z__1.i;
    *c = cdabs_(ca) / norm;
    d_cnjg(&z__3, cb);
    z__2.r = alpha.r * z__3.r - alpha.i * z__3.i, z__2.i = alpha.r * z__3.i + 
	    alpha.i * z__3.r;
    z__1.r = z__2.r / norm, z__1.i = z__2.i / norm;
    s->r = z__1.r, s->i = z__1.i;
    z__1.r = norm * alpha.r, z__1.i = norm * alpha.i;
    ca->r = z__1.r, ca->i = z__1.i;
L20:
    return 0;
} /* zrotg_ */

