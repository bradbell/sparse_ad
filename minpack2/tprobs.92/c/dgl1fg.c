/* minpack2/tprobs.92/dgl1fg.f -- translated by f2c (version 20190311).
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

/* Subroutine */ int dgl1fg_(integer *n, doublereal *x, doublereal *f, 
	doublereal *fgrad, char *task, doublereal *t, ftnlen task_len)
{
    /* System generated locals */
    integer i__1;
    doublereal d__1, d__2, d__3, d__4, d__5, d__6, d__7, d__8, d__9;

    /* Builtin functions */
    double atan(doublereal);
    integer s_cmp(char *, char *, ftnlen, ftnlen);
    double sqrt(doublereal);

    /* Local variables */
    doublereal c__;
    integer i__;
    doublereal f1, f2, f3, h1, h2;
    integer n1, n2;
    doublereal ec, dn, em, ds, pi, fac, hcn, hcs, tcn, tcs, hbar, penn, pens, 
	    temp, gamma, betan, betas, alphan, alphas;

/*     ********** */

/*     Subroutine dgl1fg */

/*     This subroutine computes the function and gradient of the */
/*     Inhomogeneous Superconductors (One Dimensional Ginzburg-Landau) */
/*     problem based on the work of J. Garner and R. Benedek. This */
/*     problem arises in the solution of the Ginzburg-Landau equations */
/*     for inhomogeneous superconductors in the absence of magnetic */
/*     fields. The one-dimensional system under consideration consists */
/*     of alternating layers of lead and tin. The problem is discretized */
/*     by considering a finite element approximation over the space of */
/*     piecewise linear functions on the domain (-d,d) where 2d is the */
/*     width of the interval. The problem depends on temperature, t, in */
/*     Kelvin. For lead and tin, values of t in the interval (3.73,7.32) */
/*     are of interest.  A typical value is t = 5. */

/*     The subroutine statement is: */

/*       subroutine dgl1fg(n,x,f,fgrad,task,t) */

/*     where */

/*       n is an integer variable. */
/*         On entry n is the number of subintervals in the domain. */
/*         On exit n is unchanged. */

/*       x is a double precision array of dimension n. */
/*         On entry x specifies the vector x if task = 'F', 'G', or 'FG'. */
/*            Otherwise x need not be specified. */
/*         On exit x is unchanged if task = 'F', 'G', or 'FG'. Otherwise */
/*            x is set according to task. */

/*       f is a double precision variable. */
/*         On entry f need not be specified. */
/*         On exit f is set to the function evaluated at x if task = 'F' */
/*            or 'FG'. */

/*       fgrad is a double precision array of dimension n. */
/*         On entry fgrad need not be specified. */
/*         On exit fgrad contains the gradient evaluated at x if */
/*            task = 'G' or 'FG'. */

/*       task is a character variable. */
/*         On entry task specifies the action of the subroutine: */

/*            task               action */
/*            ----               ------ */
/*             'F'     Evaluate the function at x. */
/*             'G'     Evaluate the gradient at x. */
/*             'FG'    Evaluate the function and the gradient at x. */
/*             'XS'    Set x to the standard starting point xs. */

/*         On exit task is unchanged. */

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
    --fgrad;
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
/*     Compute the standard starting point if task = 'XS'. */
    if (s_cmp(task, "XS", task_len, (ftnlen)2) == 0) {
	temp = sqrt((betas + betan) / ((abs(alphas) + abs(alphan)) * 2.));
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    x[i__] = temp;
/* L10: */
	}
	return 0;
    }
    if (s_cmp(task, "F", task_len, (ftnlen)1) == 0 || s_cmp(task, "FG", 
	    task_len, (ftnlen)2) == 0) {
	f1 = 0.;
	f2 = 0.;
	f3 = 0.;
    }
    if (s_cmp(task, "G", task_len, (ftnlen)1) == 0 || s_cmp(task, "FG", 
	    task_len, (ftnlen)2) == 0) {
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    fgrad[i__] = 0.;
/* L20: */
	}
    }
/*     Evaluate the function over the intervals (-d, -ds), (-ds, ds), */
/*     and (ds, d) if task = 'F' or task = 'FG'. */
    if (s_cmp(task, "F", task_len, (ftnlen)1) == 0 || s_cmp(task, "FG", 
	    task_len, (ftnlen)2) == 0) {
	i__1 = n1;
	for (i__ = 1; i__ <= i__1; ++i__) {
/* Computing 2nd power */
	    d__1 = x[i__ + 1];
/* Computing 2nd power */
	    d__2 = x[i__];
/* Computing 4th power */
	    d__3 = x[i__ + 1], d__3 *= d__3;
/* Computing 3rd power */
	    d__4 = x[i__ + 1];
/* Computing 2nd power */
	    d__5 = x[i__ + 1];
/* Computing 2nd power */
	    d__6 = x[i__];
/* Computing 3rd power */
	    d__7 = x[i__];
/* Computing 4th power */
	    d__8 = x[i__], d__8 *= d__8;
/* Computing 2nd power */
	    d__9 = (x[i__ + 1] - x[i__]) / h1;
	    f1 = f1 + alphan / 3. * (d__1 * d__1 + x[i__ + 1] * x[i__] + d__2 
		    * d__2) + betan / 10. * (d__3 * d__3 + d__4 * (d__4 * 
		    d__4) * x[i__] + d__5 * d__5 * (d__6 * d__6) + x[i__ + 1] 
		    * (d__7 * (d__7 * d__7)) + d__8 * d__8) + gamma * (d__9 * 
		    d__9);
/* L30: */
	}
	i__1 = n1 + n2;
	for (i__ = n1 + 1; i__ <= i__1; ++i__) {
/* Computing 2nd power */
	    d__1 = x[i__ + 1];
/* Computing 2nd power */
	    d__2 = x[i__];
/* Computing 4th power */
	    d__3 = x[i__ + 1], d__3 *= d__3;
/* Computing 3rd power */
	    d__4 = x[i__ + 1];
/* Computing 2nd power */
	    d__5 = x[i__ + 1];
/* Computing 2nd power */
	    d__6 = x[i__];
/* Computing 3rd power */
	    d__7 = x[i__];
/* Computing 4th power */
	    d__8 = x[i__], d__8 *= d__8;
/* Computing 2nd power */
	    d__9 = (x[i__ + 1] - x[i__]) / h2;
	    f2 = f2 + alphas / 3. * (d__1 * d__1 + x[i__ + 1] * x[i__] + d__2 
		    * d__2) + betas / 10. * (d__3 * d__3 + d__4 * (d__4 * 
		    d__4) * x[i__] + d__5 * d__5 * (d__6 * d__6) + x[i__ + 1] 
		    * (d__7 * (d__7 * d__7)) + d__8 * d__8) + gamma * (d__9 * 
		    d__9);
/* L40: */
	}
	i__1 = *n - 1;
	for (i__ = n1 + n2 + 1; i__ <= i__1; ++i__) {
/* Computing 2nd power */
	    d__1 = x[i__ + 1];
/* Computing 2nd power */
	    d__2 = x[i__];
/* Computing 4th power */
	    d__3 = x[i__ + 1], d__3 *= d__3;
/* Computing 3rd power */
	    d__4 = x[i__ + 1];
/* Computing 2nd power */
	    d__5 = x[i__ + 1];
/* Computing 2nd power */
	    d__6 = x[i__];
/* Computing 3rd power */
	    d__7 = x[i__];
/* Computing 4th power */
	    d__8 = x[i__], d__8 *= d__8;
/* Computing 2nd power */
	    d__9 = (x[i__ + 1] - x[i__]) / h1;
	    f3 = f3 + alphan / 3. * (d__1 * d__1 + x[i__ + 1] * x[i__] + d__2 
		    * d__2) + betan / 10. * (d__3 * d__3 + d__4 * (d__4 * 
		    d__4) * x[i__] + d__5 * d__5 * (d__6 * d__6) + x[i__ + 1] 
		    * (d__7 * (d__7 * d__7)) + d__8 * d__8) + gamma * (d__9 * 
		    d__9);
/* L50: */
	}
/*        Special case for the right subinterval where x(n+1) = x(1). */
/* Computing 2nd power */
	d__1 = x[1];
/* Computing 2nd power */
	d__2 = x[*n];
/* Computing 4th power */
	d__3 = x[1], d__3 *= d__3;
/* Computing 3rd power */
	d__4 = x[1];
/* Computing 2nd power */
	d__5 = x[1];
/* Computing 2nd power */
	d__6 = x[*n];
/* Computing 3rd power */
	d__7 = x[*n];
/* Computing 4th power */
	d__8 = x[*n], d__8 *= d__8;
/* Computing 2nd power */
	d__9 = (x[1] - x[*n]) / h1;
	f3 = f3 + alphan / 3. * (d__1 * d__1 + x[1] * x[*n] + d__2 * d__2) + 
		betan / 10. * (d__3 * d__3 + d__4 * (d__4 * d__4) * x[*n] + 
		d__5 * d__5 * (d__6 * d__6) + x[1] * (d__7 * (d__7 * d__7)) + 
		d__8 * d__8) + gamma * (d__9 * d__9);
	*f = h1 * f1 + h2 * f2 + h1 * f3;
	if (s_cmp(task, "F", task_len, (ftnlen)1) == 0) {
	    return 0;
	}
    }
/*     Evaluate the gradient over the intervals (-d, -ds), (-ds, ds), */
/*     and (ds, d). */
    if (s_cmp(task, "G", task_len, (ftnlen)1) == 0 || s_cmp(task, "FG", 
	    task_len, (ftnlen)2) == 0) {
	i__1 = n1;
	for (i__ = 1; i__ <= i__1; ++i__) {
/* Computing 3rd power */
	    d__1 = x[i__ + 1];
/* Computing 2nd power */
	    d__2 = x[i__ + 1];
/* Computing 2nd power */
	    d__3 = x[i__];
/* Computing 3rd power */
	    d__4 = x[i__];
	    fgrad[i__] += h1 * (alphan / 3. * (x[i__ + 1] + x[i__] * 2.) + 
		    betan / 10. * (d__1 * (d__1 * d__1) + d__2 * d__2 * 2. * 
		    x[i__] + x[i__ + 1] * 3. * (d__3 * d__3) + d__4 * (d__4 * 
		    d__4) * 4.) - gamma * (2. / h1) * ((x[i__ + 1] - x[i__]) /
		     h1));
/* Computing 3rd power */
	    d__1 = x[i__ + 1];
/* Computing 2nd power */
	    d__2 = x[i__ + 1];
/* Computing 2nd power */
	    d__3 = x[i__];
/* Computing 3rd power */
	    d__4 = x[i__];
	    fgrad[i__ + 1] += h1 * (alphan / 3. * (x[i__ + 1] * 2. + x[i__]) 
		    + betan / 10. * (d__1 * (d__1 * d__1) * 4. + d__2 * d__2 *
		     3. * x[i__] + x[i__ + 1] * 2. * (d__3 * d__3) + d__4 * (
		    d__4 * d__4)) + gamma * (2. / h1) * ((x[i__ + 1] - x[i__])
		     / h1));
/* L60: */
	}
	i__1 = n1 + n2;
	for (i__ = n1 + 1; i__ <= i__1; ++i__) {
/* Computing 3rd power */
	    d__1 = x[i__ + 1];
/* Computing 2nd power */
	    d__2 = x[i__ + 1];
/* Computing 2nd power */
	    d__3 = x[i__];
/* Computing 3rd power */
	    d__4 = x[i__];
	    fgrad[i__] += h2 * (alphas / 3. * (x[i__ + 1] + x[i__] * 2.) + 
		    betas / 10. * (d__1 * (d__1 * d__1) + d__2 * d__2 * 2. * 
		    x[i__] + x[i__ + 1] * 3. * (d__3 * d__3) + d__4 * (d__4 * 
		    d__4) * 4.) - gamma * (2. / h2) * ((x[i__ + 1] - x[i__]) /
		     h2));
/* Computing 3rd power */
	    d__1 = x[i__ + 1];
/* Computing 2nd power */
	    d__2 = x[i__ + 1];
/* Computing 2nd power */
	    d__3 = x[i__];
/* Computing 3rd power */
	    d__4 = x[i__];
	    fgrad[i__ + 1] += h2 * (alphas / 3. * (x[i__ + 1] * 2. + x[i__]) 
		    + betas / 10. * (d__1 * (d__1 * d__1) * 4. + d__2 * d__2 *
		     3. * x[i__] + x[i__ + 1] * 2. * (d__3 * d__3) + d__4 * (
		    d__4 * d__4)) + gamma * (2. / h2) * ((x[i__ + 1] - x[i__])
		     / h2));
/* L70: */
	}
	i__1 = *n - 1;
	for (i__ = n1 + n2 + 1; i__ <= i__1; ++i__) {
/* Computing 3rd power */
	    d__1 = x[i__ + 1];
/* Computing 2nd power */
	    d__2 = x[i__ + 1];
/* Computing 2nd power */
	    d__3 = x[i__];
/* Computing 3rd power */
	    d__4 = x[i__];
	    fgrad[i__] += h1 * (alphan / 3. * (x[i__ + 1] + x[i__] * 2.) + 
		    betan / 10. * (d__1 * (d__1 * d__1) + d__2 * d__2 * 2. * 
		    x[i__] + x[i__ + 1] * 3. * (d__3 * d__3) + d__4 * (d__4 * 
		    d__4) * 4.) - gamma * (2. / h1) * ((x[i__ + 1] - x[i__]) /
		     h1));
/* Computing 3rd power */
	    d__1 = x[i__ + 1];
/* Computing 2nd power */
	    d__2 = x[i__ + 1];
/* Computing 2nd power */
	    d__3 = x[i__];
/* Computing 3rd power */
	    d__4 = x[i__];
	    fgrad[i__ + 1] += h1 * (alphan / 3. * (x[i__ + 1] * 2. + x[i__]) 
		    + betan / 10. * (d__1 * (d__1 * d__1) * 4. + d__2 * d__2 *
		     3. * x[i__] + x[i__ + 1] * 2. * (d__3 * d__3) + d__4 * (
		    d__4 * d__4)) + gamma * (2. / h1) * ((x[i__ + 1] - x[i__])
		     / h1));
/* L80: */
	}
/*        Special case for the right subinterval where x(n+1) = x(1). */
/* Computing 3rd power */
	d__1 = x[1];
/* Computing 2nd power */
	d__2 = x[1];
/* Computing 2nd power */
	d__3 = x[*n];
/* Computing 3rd power */
	d__4 = x[*n];
	fgrad[*n] += h1 * (alphan / 3. * (x[1] + x[*n] * 2.) + betan / 10. * (
		d__1 * (d__1 * d__1) + d__2 * d__2 * 2. * x[*n] + x[1] * 3. * 
		(d__3 * d__3) + d__4 * (d__4 * d__4) * 4.) - gamma * (2. / h1)
		 * ((x[1] - x[*n]) / h1));
/* Computing 3rd power */
	d__1 = x[1];
/* Computing 2nd power */
	d__2 = x[1];
/* Computing 2nd power */
	d__3 = x[*n];
/* Computing 3rd power */
	d__4 = x[*n];
	fgrad[1] += h1 * (alphan / 3. * (x[1] * 2. + x[*n]) + betan / 10. * (
		d__1 * (d__1 * d__1) * 4. + d__2 * d__2 * 3. * x[*n] + x[1] * 
		2. * (d__3 * d__3) + d__4 * (d__4 * d__4)) + gamma * (2. / h1)
		 * ((x[1] - x[*n]) / h1));
	return 0;
    }
    return 0;
} /* dgl1fg_ */

