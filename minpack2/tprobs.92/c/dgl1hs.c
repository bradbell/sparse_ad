/* minpack2/tprobs.92/dgl1hs.f -- translated by f2c (version 20190311).
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

/* Subroutine */ int dgl1hs_(integer *n, doublereal *x, doublereal *s, 
	doublereal *y, doublereal *t)
{
    /* System generated locals */
    integer i__1;
    doublereal d__1, d__2, d__3, d__4;

    /* Builtin functions */
    double atan(doublereal);

    /* Local variables */
    doublereal c__;
    integer i__;
    doublereal h1, h2;
    integer n1, n2;
    doublereal ec, dn, em, ds, pi, fac, hcn, hcs, tcn, tcs, hbar, penn, pens, 
	    gamma, betan, betas, alphan, alphas;

/*     ********** */

/*     Subroutine dglhs */

/*     This subroutine computes the product */

/*                            H(x)*s = y */

/*     where H(x) is the Hessian for the One-Dimensional Ginzburg-Landau */
/*     problem. */

/*     The subroutine statement is: */

/*       subroutine dgl1hs(n,x,s,y,t) */

/*     where */

/*       n is an integer variable. */
/*         On entry n is the number of variables. */
/*         On exit n is unchanged. */

/*       x is a double precision array of dimension n. */
/*         On entry x specifies the vector x. */
/*         On exit x is unchanged. */

/*       y is a double precision array of dimension n. */
/*         On entry out need not be specified. */
/*         On exit y contains H*s. */

/*       s is a double precision array of dimension n. */
/*         On entry s contains the vector s. */
/*         On exit s is unchanged. */

/*       t is a double precision variable. */
/*         On entry t is a temperature in (3.73,7.32). */
/*         On exit t is unchanged. */

/*     MINPACK-2 Project. October 1992. */
/*     Argonne National Laboratory and University of Minnesota. */
/*     Brett M. Averick and Jorge J. More'. */

/*     ********** */
/*     Initialization. */
/*     Set electron mass (grams), speed of light (cm/sec), and */
/*     electronic charge (esu). */
    /* Parameter adjustments */
    --y;
    --s;
    --x;

    /* Function Body */
    em = 9.11e-28;
    c__ = 2.99e10;
    ec = 4.8e-10;
/*     Set length of a half-layer of lead and tin (10**3-angstroms), */
/*     d = ds + dn. */
    ds = 1.;
    dn = 2.2;
/*     Set critical temperature for lead and tin (Kelvin). */
    tcs = 7.32;
    tcn = 3.73;
/*     Set critical magnetic field for lead and tin at zero */
/*     temperature (gauss). */
    hcs = 803.;
    hcn = 309.;
/*     Set penetration depth for lead and tin at zero temperature */
/*     (cm). */
    pens = 3.7e-6;
    penn = 3.4e-6;
/*     Compute pi. */
    pi = atan(1.) * 4.;
/*     Set initial values for temperature dependent constants alphas, */
/*     alphan (ergs), and betas, betan (ergs-cm**3). */
/* Computing 2nd power */
    d__1 = ec / c__;
/* Computing 2nd power */
    d__2 = hcs;
/* Computing 2nd power */
    d__3 = pens;
    alphas = d__1 * d__1 / em * -2. * (d__2 * d__2) * (d__3 * d__3);
/* Computing 2nd power */
    d__1 = ec / c__;
/* Computing 2nd power */
    d__2 = hcn;
/* Computing 2nd power */
    d__3 = penn;
    alphan = d__1 * d__1 / em * -2. * (d__2 * d__2) * (d__3 * d__3);
/* Computing 2nd power */
    d__2 = ec / c__;
/* Computing 2nd power */
    d__1 = d__2 * d__2 / em;
/* Computing 2nd power */
    d__3 = hcs;
/* Computing 4th power */
    d__4 = pens, d__4 *= d__4;
    betas = pi * 16. * (d__1 * d__1) * (d__3 * d__3) * (d__4 * d__4);
/* Computing 2nd power */
    d__2 = ec / c__;
/* Computing 2nd power */
    d__1 = d__2 * d__2 / em;
/* Computing 2nd power */
    d__3 = hcn;
/* Computing 4th power */
    d__4 = penn, d__4 *= d__4;
    betan = pi * 16. * (d__1 * d__1) * (d__3 * d__3) * (d__4 * d__4);
/* Computing 2nd power */
    d__1 = *t / tcs;
/* Computing 2nd power */
    d__2 = *t / tcs;
    alphas *= (1. - d__1 * d__1) / (d__2 * d__2 + 1.);
/* Computing 2nd power */
    d__1 = *t / tcn;
/* Computing 2nd power */
    d__2 = *t / tcn;
    alphan *= (1. - d__1 * d__1) / (d__2 * d__2 + 1.);
/* Computing 2nd power */
    d__2 = *t / tcs;
/* Computing 2nd power */
    d__1 = d__2 * d__2 + 1.;
    betas /= d__1 * d__1;
/* Computing 2nd power */
    d__2 = *t / tcn;
/* Computing 2nd power */
    d__1 = d__2 * d__2 + 1.;
    betan /= d__1 * d__1;
/*     Set Planck's constant (erg-sec). */
    hbar = 1.05459e-27;
/*     Set temperature dependent constant gamma (erg-cm**2). */
/* Computing 2nd power */
    d__1 = hbar;
    gamma = d__1 * d__1 / (em * 4.);
/*     Scale temperature dependent constants to the same units. */
/*     This makes the order parameter dimensionless. */
    fac = 1e6;
/* Computing 3rd power */
    d__1 = fac;
    alphas *= d__1 * (d__1 * d__1);
/* Computing 3rd power */
    d__1 = fac;
    alphan *= d__1 * (d__1 * d__1);
/* Computing 6th power */
    d__1 = fac, d__1 *= d__1;
    betas *= d__1 * (d__1 * d__1);
/* Computing 6th power */
    d__1 = fac, d__1 *= d__1;
    betan *= d__1 * (d__1 * d__1);
/* Computing 5th power */
    d__1 = fac, d__2 = d__1, d__1 *= d__1;
    gamma *= d__2 * (d__1 * d__1);
/*     Compute the number of subintervals in (-d,-ds), in (-ds,ds), */
/*     and in (ds,d). */
    n1 = *n / 4;
    n2 = *n - (n1 << 1);
    h1 = dn / (doublereal) n1;
    h2 = ds * 2. / (doublereal) n2;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	y[i__] = 0.;
/* L10: */
    }
/*     Evaluate H*s over the intervals (-d, -ds), (-ds, ds), */
/*     and (ds, d). */
    i__1 = n1;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* Computing 2nd power */
	d__1 = x[i__ + 1];
/* Computing 2nd power */
	d__2 = x[i__];
/* Computing 2nd power */
	d__3 = x[i__ + 1];
/* Computing 2nd power */
	d__4 = x[i__];
	y[i__] = y[i__] + h1 * (alphan * 2. / 3. + betan / 10. * (d__1 * d__1 
		* 2. + x[i__ + 1] * 6. * x[i__] + d__2 * d__2 * 12.) + gamma *
		 2. / h1 / h1) * s[i__] + h1 * (alphan / 3. + betan / 10. * (
		d__3 * d__3 * 3. + x[i__ + 1] * 4. * x[i__] + d__4 * d__4 * 
		3.) - gamma * 2. / h1 / h1) * s[i__ + 1];
/* Computing 2nd power */
	d__1 = x[i__ + 1];
/* Computing 2nd power */
	d__2 = x[i__];
/* Computing 2nd power */
	d__3 = x[i__ + 1];
/* Computing 2nd power */
	d__4 = x[i__];
	y[i__ + 1] = y[i__ + 1] + h1 * (alphan / 3. + betan / 10. * (d__1 * 
		d__1 * 3. + x[i__ + 1] * 4. * x[i__] + d__2 * d__2 * 3.) - 
		gamma * 2. / h1 / h1) * s[i__] + h1 * (alphan * 2. / 3. + 
		betan / 10. * (d__3 * d__3 * 12. + x[i__ + 1] * 6. * x[i__] + 
		d__4 * d__4 * 2.) + gamma * 2. / h1 / h1) * s[i__ + 1];
/* L20: */
    }
    i__1 = n1 + n2;
    for (i__ = n1 + 1; i__ <= i__1; ++i__) {
/* Computing 2nd power */
	d__1 = x[i__ + 1];
/* Computing 2nd power */
	d__2 = x[i__];
/* Computing 2nd power */
	d__3 = x[i__ + 1];
/* Computing 2nd power */
	d__4 = x[i__];
	y[i__] = y[i__] + h2 * (alphas * 2. / 3. + betas / 10. * (d__1 * d__1 
		* 2. + x[i__ + 1] * 6. * x[i__] + d__2 * d__2 * 12.) + gamma *
		 2. / h2 / h2) * s[i__] + h2 * (alphas / 3. + betas / 10. * (
		d__3 * d__3 * 3. + x[i__ + 1] * 4. * x[i__] + d__4 * d__4 * 
		3.) - gamma * 2. / h2 / h2) * s[i__ + 1];
/* Computing 2nd power */
	d__1 = x[i__ + 1];
/* Computing 2nd power */
	d__2 = x[i__];
/* Computing 2nd power */
	d__3 = x[i__ + 1];
/* Computing 2nd power */
	d__4 = x[i__];
	y[i__ + 1] = y[i__ + 1] + h2 * (alphas / 3. + betas / 10. * (d__1 * 
		d__1 * 3. + x[i__ + 1] * 4. * x[i__] + d__2 * d__2 * 3.) - 
		gamma * 2. / h2 / h2) * s[i__] + h2 * (alphas * 2. / 3. + 
		betas / 10. * (d__3 * d__3 * 12. + x[i__ + 1] * 6. * x[i__] + 
		d__4 * d__4 * 2.) + gamma * 2. / h2 / h2) * s[i__ + 1];
/* L30: */
    }
    i__1 = *n - 1;
    for (i__ = n1 + n2 + 1; i__ <= i__1; ++i__) {
/* Computing 2nd power */
	d__1 = x[i__ + 1];
/* Computing 2nd power */
	d__2 = x[i__];
/* Computing 2nd power */
	d__3 = x[i__ + 1];
/* Computing 2nd power */
	d__4 = x[i__];
	y[i__] = y[i__] + h1 * (alphan * 2. / 3. + betan / 10. * (d__1 * d__1 
		* 2. + x[i__ + 1] * 6. * x[i__] + d__2 * d__2 * 12.) + gamma *
		 2. / h1 / h1) * s[i__] + h1 * (alphan / 3. + betan / 10. * (
		d__3 * d__3 * 3. + x[i__ + 1] * 4. * x[i__] + d__4 * d__4 * 
		3.) - gamma * 2. / h1 / h1) * s[i__ + 1];
/* Computing 2nd power */
	d__1 = x[i__ + 1];
/* Computing 2nd power */
	d__2 = x[i__];
/* Computing 2nd power */
	d__3 = x[i__ + 1];
/* Computing 2nd power */
	d__4 = x[i__];
	y[i__ + 1] = y[i__ + 1] + h1 * (alphan / 3. + betan / 10. * (d__1 * 
		d__1 * 3. + x[i__ + 1] * 4. * x[i__] + d__2 * d__2 * 3.) - 
		gamma * 2. / h1 / h1) * s[i__] + h1 * (alphan * 2. / 3. + 
		betan / 10. * (d__3 * d__3 * 12. + x[i__ + 1] * 6. * x[i__] + 
		d__4 * d__4 * 2.) + gamma * 2. / h1 / h1) * s[i__ + 1];
/* L40: */
    }
/*     Special case for the right subinterval where x(n+1) = x(1). */
/* Computing 2nd power */
    d__1 = x[1];
/* Computing 2nd power */
    d__2 = x[*n];
/* Computing 2nd power */
    d__3 = x[1];
/* Computing 2nd power */
    d__4 = x[*n];
    y[1] = y[1] + h1 * (alphan / 3. + betan / 10. * (d__1 * d__1 * 3. + x[1] *
	     4. * x[*n] + d__2 * d__2 * 3.) - gamma * 2. / h1 / h1) * s[*n] + 
	    h1 * (alphan * 2. / 3. + betan / 10. * (d__3 * d__3 * 12. + x[1] *
	     6. * x[*n] + d__4 * d__4 * 2.) + gamma * 2. / h1 / h1) * s[1];
/* Computing 2nd power */
    d__1 = x[1];
/* Computing 2nd power */
    d__2 = x[*n];
/* Computing 2nd power */
    d__3 = x[1];
/* Computing 2nd power */
    d__4 = x[*n];
    y[*n] = y[*n] + h1 * (alphan * 2. / 3. + betan / 10. * (d__1 * d__1 * 2. 
	    + x[1] * 6. * x[*n] + d__2 * d__2 * 12.) + gamma * 2. / h1 / h1) *
	     s[*n] + h1 * (alphan / 3. + betan / 10. * (d__3 * d__3 * 3. + x[
	    1] * 4. * x[*n] + d__4 * d__4 * 3.) - gamma * 2. / h1 / h1) * s[1]
	    ;
    return 0;
} /* dgl1hs_ */

