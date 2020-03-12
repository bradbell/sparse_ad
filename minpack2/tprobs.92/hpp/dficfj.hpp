# ifndef MINPACK2_TPROBS_92_HPP_DFICFJ_HPP
# define MINPACK2_TPROBS_92_HPP_DFICFJ_HPP

# include <cmath>
# include <cppad/cg/cppadcg.hpp>
# include <adolc/adolc.h>
# include "f2clib.hpp"

template <class doublereal, class integer, class ftnlen>
int dficfj(integer *n, doublereal *x, doublereal *fvec,
	doublereal *fjac, integer *ldfjac, char *task, doublereal *r__,
	integer *nint, ftnlen task_len)
{
    // Initialized data

    static doublereal rho[4];
rho[0] = .0694318413734436035;
rho[1] = .330009490251541138;
rho[2] = .66999053955078125;
rho[3] = .930568158626556396;


    // System generated locals
    integer fjac_dim1, fjac_offset, i__1, i__2;

    // Builtin functions

    // Local variables
    doublereal h__;
    integer i__, j, k, m;
    doublereal w[5], nf, hm, dw[40]	/* was [5][8] */, xt;
    integer eqn, var;
    doublereal rhnfhk[1280]	/* was [4][8][8][5] */, rhoijh;
//
//  **********
//
//  Subroutine dficfj
//
//  This subroutine computes the function and Jacobian matrix of the
//  Flow in a Channel problem. This problem arises in the analysis
//  of fluid injection through one side of a vertical channel.
//
//  The problem is modeled by the ordinary differential equation
//
//              u'''' = R*(u'*u'' - u*u''')
//
//  with boundary conditions
//
//              u(0) = u'(0) = 0
//              u(1) = 1, u'(1) = 0.
//
//  In this formulation R is the Reynolds number, u is the potential
//  function, and u' is the tangential velocity of the fluid.
//  The boundary value problem is discretized by a k-stage collocation
//  scheme to obtain a system of nonlinear equations.
//
//  The subroutine statement is:
//
//    subroutine dficfj(n,x,fvec,fjac,ldfjac,task,r,nint)
//
//  where
//
//    n is an integer variable.
//      On entry n is the number of variables.
//         For the flow in a channel problem n = 8*nint.
//      On exit n is unchanged.
//
//    x is a double precision array of dimension n.
//      On entry x specifies the vector x if task = 'F', 'J', or 'FJ'.
//         Otherwise x need not be specified.
//      On exit x is unchanged if task = 'F', 'J', or 'FJ'. Otherwise
//         x is set according to task.
//
//    fvec is a double precision array of dimension n.
//      On entry fvec need not be specified.
//      On exit fvec contains the function evaluated at x if
//         task = 'F' or 'FJ'.
//
//    fjac is a double precision array of dimension (ldfjac,n).
//      On entry fjac need not be specified.
//      On exit fjac contains the Jacobian matrix evaluated at x if
//         task = 'J' or 'FJ'.
//
//    ldfjac is an integer variable.
//       On entry ldfjac is the leading dimension of fjac.
//       On exit ldfjac is unchanged.
//
//    task is a character variable.
//      On entry task specifies the action of the subroutine:
//
//         task               action
//         ----               ------
//          'F'     Evaluate the function at x.
//          'J'     Evaluate the Jacobian matrix at x.
//          'FJ'    Evaluate the function and the Jacobian at x.
//          'XS'    Set x to the standard starting point xs.
//
//      On exit task is unchanged.
//
//    r is a double precision variable.
//      On entry r is the Reynolds number.
//      On exit r is unchanged.
//
//    nint is an integer variable.
//      On entry nint is the number of subintervals in the
//         k-stage collocation.
//      On exit nint is unchanged.
//
//  MINPACK-2 Project. October 1992.
//  Argonne National Laboratory and University of Minnesota.
//  Brett M. Averick.
//
//  **********
    // Parameter adjustments
    --fvec;
    --x;
    fjac_dim1 = *ldfjac;
    fjac_offset = 1 + fjac_dim1;
    fjac -= fjac_offset;

    // Function Body
//  Check input arguments for errors.
    if (*nint <= 0) {
	s_copy(task, "ERROR: NINT MUST BE .GT. 0", task_len, (ftnlen)26);
    }
    if (*n != *nint << 3) {
	s_copy(task, "ERROR: N MUST .EQ. 8*NINT", task_len, (ftnlen)25);
    }
    if (s_cmp(task, "ERROR", (ftnlen)5, (ftnlen)5) == 0) {
	return 0;
    }
//  Initialization.
    h__ = 1. / (doublereal) (*nint);
//  Compute the standard starting point if task = 'XS'
    if (s_cmp(task, "XS", task_len, (ftnlen)2) == 0) {
//     The standard starting point corresponds to the solution of the
//     flow in a channel problem with R = 0.
	xt = 0.;
	i__1 = *nint;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    var = i__ - 1 << 3;
	    x[var + 1] = xt * xt * (3. - xt * 2.);
	    x[var + 2] = xt * 6. * (1. - xt);
	    x[var + 3] = (1. - xt * 2.) * 6.;
	    x[var + 4] = -12.;
	    for (j = 1; j <= 4; ++j) {
		x[var + 4 + j] = 0.;
// L10:
	    }
	    xt += h__;
// L20:
	}
	return 0;
    }
//  Store all possible combinations of rho, h, and n factorial.
    hm = 1.;
    for (m = 0; m <= 4; ++m) {
	for (i__ = 1; i__ <= 4; ++i__) {
	    rhoijh = hm;
	    for (j = 0; j <= 7; ++j) {
		nf = 1.;
		for (k = 0; k <= 7; ++k) {
		    rhnfhk[i__ + (j + (k + (m << 3) << 3) << 2) - 1] = rhoijh
			    / nf;
		    nf *= (doublereal) (k + 1);
// L30:
		}
		rhoijh *= rho[i__ - 1];
// L40:
	    }
// L50:
	}
	hm *= h__;
// L60:
    }
//  Evaluate the function if task = 'F', the Jacobian matrix if
//  task = 'J', or both if task = 'FJ'.
//  Initialize arrays.
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	if (s_cmp(task, "F", task_len, (ftnlen)1) == 0 || s_cmp(task, "FJ",
		task_len, (ftnlen)2) == 0) {
	    fvec[j] = 0.;
	}
	if (s_cmp(task, "J", task_len, (ftnlen)1) == 0 || s_cmp(task, "FJ",
		task_len, (ftnlen)2) == 0) {
	    i__2 = *n;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		fjac[i__ + j * fjac_dim1] = 0.;
// L70:
	    }
	}
// L80:
    }
    for (k = 1; k <= 8; ++k) {
	for (j = 1; j <= 5; ++j) {
	    dw[j + k * 5 - 6] = 0.;
// L90:
	}
// L100:
    }
//  Set up the boundary equations at t = 0.  u(0) = 0, u'(0) = 0.
    if (s_cmp(task, "F", task_len, (ftnlen)1) == 0 || s_cmp(task, "FJ",
	    task_len, (ftnlen)2) == 0) {
	fvec[1] = x[1];
	fvec[2] = x[2];
    }
    if (s_cmp(task, "J", task_len, (ftnlen)1) == 0 || s_cmp(task, "FJ",
	    task_len, (ftnlen)2) == 0) {
	fjac[fjac_dim1 + 1] = 1.;
	fjac[(fjac_dim1 << 1) + 2] = 1.;
    }
    i__1 = *nint;
    for (i__ = 1; i__ <= i__1; ++i__) {
	var = i__ - 1 << 3;
//     Set up the collocation equations.
	eqn = var + 2;
	for (k = 1; k <= 4; ++k) {
	    for (m = 1; m <= 5; ++m) {
		w[m - 1] = 0.;
		for (j = m; j <= 4; ++j) {
		    w[m - 1] += rhnfhk[k + (j - m + (j - m + (j - m << 3) <<
			    3) << 2) - 1] * x[var + j];
		    dw[m + j * 5 - 6] = rhnfhk[k + (j - m + (j - m + (j - m <<
			     3) << 3) << 2) - 1];
// L110:
		}
		for (j = 1; j <= 4; ++j) {
		    w[m - 1] += rhnfhk[k + (j + 4 - m + (j + 4 - m + (4 - m +
			    1 << 3) << 3) << 2) - 1] * x[var + 4 + j];
		    dw[m + (j + 4) * 5 - 6] = rhnfhk[k + (j + 4 - m + (j + 4
			    - m + (4 - m + 1 << 3) << 3) << 2) - 1];
// L120:
		}
// L130:
	    }
	    if (s_cmp(task, "F", task_len, (ftnlen)1) == 0 || s_cmp(task,
		    "FJ", task_len, (ftnlen)2) == 0) {
		fvec[eqn + k] = w[4] - *r__ * (w[1] * w[2] - w[0] * w[3]);
	    }
	    if (s_cmp(task, "J", task_len, (ftnlen)1) == 0 || s_cmp(task,
		    "FJ", task_len, (ftnlen)2) == 0) {
		for (j = 1; j <= 8; ++j) {
		    fjac[eqn + k + (var + j) * fjac_dim1] = dw[j * 5 - 1] - *
			    r__ * (dw[j * 5 - 4] * w[2] + w[1] * dw[j * 5 - 3]
			     - dw[j * 5 - 5] * w[3] - w[0] * dw[j * 5 - 2]);
// L140:
		}
	    }
// L150:
	}
//     Set up the continuity equations.
	eqn = var + 6;
	for (m = 1; m <= 4; ++m) {
	    w[m - 1] = 0.;
	    for (j = m; j <= 4; ++j) {
		w[m - 1] += rhnfhk[(j - m + (j - m << 3)) * 32] * x[var + j];
		dw[m + j * 5 - 6] = rhnfhk[(j - m + (j - m << 3)) * 32];
// L160:
	    }
	    for (j = 1; j <= 4; ++j) {
		w[m - 1] += rhnfhk[(j + 4 - m + (4 - m + 1 << 3)) * 32] * x[
			var + 4 + j];
		dw[m + (j + 4) * 5 - 6] = rhnfhk[(j + 4 - m + (4 - m + 1 << 3)
			) * 32];
// L170:
	    }
// L180:
	}
	if (i__ == *nint) {
	    goto L230;
	}
	if (s_cmp(task, "F", task_len, (ftnlen)1) == 0 || s_cmp(task, "FJ",
		task_len, (ftnlen)2) == 0) {
	    for (m = 1; m <= 4; ++m) {
		fvec[eqn + m] = x[var + 8 + m] - w[m - 1];
// L190:
	    }
	}
	if (s_cmp(task, "J", task_len, (ftnlen)1) == 0 || s_cmp(task, "FJ",
		task_len, (ftnlen)2) == 0) {
	    for (m = 1; m <= 4; ++m) {
		fjac[eqn + m + (var + 8 + m) * fjac_dim1] = 1.;
		for (j = 1; j <= 8; ++j) {
		    fjac[eqn + m + (var + j) * fjac_dim1] = -dw[m + j * 5 - 6]
			    ;
// L200:
		}
// L210:
	    }
	}
// L220:
    }
//  Set up the boundary equations at t = 1.  u(1) = 1, u'(1) = 0.
L230:
    if (s_cmp(task, "F", task_len, (ftnlen)1) == 0 || s_cmp(task, "FJ",
	    task_len, (ftnlen)2) == 0) {
	fvec[*n - 1] = w[0] - 1.;
	fvec[*n] = w[1];
    }
    if (s_cmp(task, "J", task_len, (ftnlen)1) == 0 || s_cmp(task, "FJ",
	    task_len, (ftnlen)2) == 0) {
	var = *n - 8;
	for (j = 1; j <= 8; ++j) {
	    fjac[*n - 1 + (var + j) * fjac_dim1] = dw[j * 5 - 5];
	    fjac[*n + (var + j) * fjac_dim1] = dw[j * 5 - 4];
// L240:
	}
    }
    return 0;
} // dficfj_



# endif
