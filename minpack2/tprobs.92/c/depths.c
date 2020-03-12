/* minpack2/tprobs.92/depths.f -- translated by f2c (version 20190311).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c.h"

/* Subroutine */ int depths_(integer *nx, integer *ny, doublereal *s, 
	doublereal *y)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    integer i__, j, k;
    doublereal v, vb, hx, hy, vl, vr, vt, area, hxhx, hyhy;

/*     ********** */

/*     Subroutine depths */

/*     This subroutine computes the product */

/*                            H*s = y */

/*     where H is the Hessian matrix for the Elastic Plastic Torsion */
/*     problem. */

/*     The subroutine statement is: */

/*       subroutine depths(nx,ny,s,y) */

/*     where */

/*       nx is an integer variable. */
/*         On entry nx is the number of grid points in the first */
/*            coordinate direction. */
/*         On exit nx is unchanged. */

/*       ny is an integer variable. */
/*         On entry ny is the number of grid points in the second */
/*            coordinate direction. */
/*         On exit ny is unchanged. */

/*       s is a double precision array of dimension nx*ny. */
/*         On entry s contains the vector s. */
/*         On exit s is unchanged. */

/*       y is a double precision array of dimension nx*ny. */
/*         On entry out need not be specified. */
/*         On exit y contains H*s. */


/*     MINPACK-2 Project. October 1992. */
/*     Argonne National Laboratory and University of Minnesota. */
/*     Brett M. Averick. */

/*     ********** */
    /* Parameter adjustments */
    --y;
    --s;

    /* Function Body */
    hx = 1. / (doublereal) (*nx + 1);
    hy = 1. / (doublereal) (*ny + 1);
    hxhx = 1. / (hx * hx);
    hyhy = 1. / (hy * hy);
    area = hx * .5 * hy;
    i__1 = *nx * *ny;
    for (k = 1; k <= i__1; ++k) {
	y[k] = 0.;
/* L10: */
    }
/*     Computation of H*s over the lower triangular elements. */
    i__1 = *ny;
    for (j = 0; j <= i__1; ++j) {
	i__2 = *nx;
	for (i__ = 0; i__ <= i__2; ++i__) {
	    k = *nx * (j - 1) + i__;
	    v = 0.;
	    vr = 0.;
	    vt = 0.;
	    if (i__ != 0 && j != 0) {
		v = s[k];
	    }
	    if (i__ != *nx && j != 0) {
		vr = s[k + 1];
		y[k + 1] += hxhx * (vr - v);
	    }
	    if (i__ != 0 && j != *ny) {
		vt = s[k + *nx];
		y[k + *nx] += hyhy * (vt - v);
	    }
	    if (i__ != 0 && j != 0) {
		y[k] = y[k] + hxhx * (v - vr) + hyhy * (v - vt);
	    }
/* L20: */
	}
/* L30: */
    }
/*     Computation of H*s over the upper triangular elements. */
    i__1 = *ny + 1;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *nx + 1;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    k = *nx * (j - 1) + i__;
	    v = 0.;
	    vl = 0.;
	    vb = 0.;
	    if (i__ != *nx + 1 && j != *ny + 1) {
		v = s[k];
	    }
	    if (i__ != *nx + 1 && j != 1) {
		vb = s[k - *nx];
		y[k - *nx] += hyhy * (vb - v);
	    }
	    if (i__ != 1 && j != *ny + 1) {
		vl = s[k - 1];
		y[k - 1] += hxhx * (vl - v);
	    }
	    if (i__ != *nx + 1 && j != *ny + 1) {
		y[k] = y[k] + hxhx * (v - vl) + hyhy * (v - vb);
	    }
/* L40: */
	}
/* L50: */
    }
/*     Scale the result. */
    i__1 = *nx * *ny;
    for (k = 1; k <= i__1; ++k) {
	y[k] = area * y[k];
/* L60: */
    }
    return 0;
} /* depths_ */

