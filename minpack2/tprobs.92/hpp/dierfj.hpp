# ifndef MINPACK2_TPROBS_92_HPP_DIERFJ_HPP
# define MINPACK2_TPROBS_92_HPP_DIERFJ_HPP

# include <cmath>
# include <cppad/cg/cppadcg.hpp>
# include <adolc/adolc.h>
# include "f2clib.hpp"

template <class doublereal, class integer, class ftnlen>
int dierfj(integer *n, doublereal *x, doublereal *fvec,
	doublereal *fjac, integer *ldfjac, char *task, doublereal *a,
	doublereal *b, doublereal *c__, integer *nint, ftnlen task_len)
{
    // Initialized data

    static doublereal rho[4];
rho[0] = .0694318413734436035;
rho[1] = .330009490251541138;
rho[2] = .66999053955078125;
rho[3] = .930568158626556396;

    static integer deg[3] = { 1,1,1 };
    static doublereal icond[3];
icond[0] = 0.;
icond[1] = 0.;
icond[2] = 0.;


    // System generated locals
    integer fjac_dim1, fjac_offset, i__1, i__2, i__3;

    // Builtin functions

    // Local variables
    integer e;
    doublereal h__;
    integer i__, j, k, l, m;
    doublereal w[6]	/* was [2][3] */, nf, hm, dw[30]	/* was [2][5][
	    3] */, xt;
    integer eqn[3], npi, var[3], ideg, ivar;
    doublereal rhnfhk[200]	/* was [4][5][5][2] */, rhoijh;
    integer sumdeg[4];

//
//  **********
//
//  Subroutine dierfj
//
//  This subroutine computes the function and Jacobian matrix of the
//  Incompressible Elastic Rod problem. This problem arises in the
//  analysis of a thin incompressible elastic rod clamped at the
//  origin and acted on by a vertical force Q, a horizontal force P,
//  and a torque M.
//
//  The Elastic Rod problem is modeled by the system of
//  ordinary differential equations
//
//                  x' = cos(theta)
//                  y' = sin(theta)
//                  theta' = Qx - Py + M
//
//  with the boundary conditions
//
//                  x(0) = 0, x(1) = a
//                  y(0) = 0, y(1) = b
//                  theta(0) = 0, theta(1) = c.
//
//  In this formulation (a,b) and c are the location and local angle
//  of the right end of the incompressable elastic rod of unit length.
//  The boundary value problem is discretized by a k-stage collocation
//  scheme to obtain a system of nonlinear equations.
//
//  The subroutine statement is:
//
//    subroutine dierfj(n,x,fvec,fjac,ldfjac,task,a,b,c,nint)
//
//  where
//
//    n is an integer variable.
//      On entry n is the number of variables.
//         For the Inverse Elastic Rod problem n must equal 15*nint+3.
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
//    a is a double precision variable.
//      On entry a is the X-coordinate of the right end of the rod.
//      On exit a is unchanged.
//
//    b is a double precision variable.
//      On entry b is the Y-coordinate of the right end of the rod.
//      On exit b is unchanged.
//
//    c is a double precision variable.
//      On entry c is the local angle at the right end of the rod.
//      On exit c is unchanged.
//
//    nint is an integer variable.
//      On entry nint is the number of subintervals in the
//         k-stage collocation.
//      On exit nint is unchanged.
//
//  MINPACK-2 Project. October 1992.
//  Argonne National Laboratory and University of Minnesota.
//  Brett M. Averick and Guo-Liang Xue
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
    if (*n != *nint * 15 + 3) {
	s_copy(task, "ERROR: N MUST .EQ. 15*NINT + 3", task_len, (ftnlen)30);
    }
    if (s_cmp(task, "ERROR", (ftnlen)5, (ftnlen)5) == 0) {
	return 0;
    }
//  Initialization.
    h__ = 1. / (doublereal) (*nint);
//  Store all possible combinations of rho, h, and n factorial.
    hm = 1.;
    for (m = 0; m <= 1; ++m) {
	for (i__ = 1; i__ <= 4; ++i__) {
	    rhoijh = hm;
	    for (j = 0; j <= 4; ++j) {
		nf = 1.;
		for (k = 0; k <= 4; ++k) {
		    rhnfhk[i__ + (j + (k + m * 5) * 5 << 2) - 1] = rhoijh /
			    nf;
		    nf *= (doublereal) (k + 1);
// L10:
		}
		rhoijh *= rho[i__ - 1];
// L20:
	    }
// L30:
	}
	hm *= h__;
// L40:
    }
    ideg = 0;
    sumdeg[0] = 0;
    for (i__ = 1; i__ <= 3; ++i__) {
	ideg += deg[i__ - 1];
	sumdeg[i__] = sumdeg[i__ - 1] + deg[i__ - 1];
// L50:
    }
    npi = ideg + 12;
//  Compute the standard starting point if task = 'XS'.
    if (s_cmp(task, "XS", task_len, (ftnlen)2) == 0) {
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    x[i__] = 0.;
// L60:
	}
	xt = 0.;
	ivar = 0;
	i__1 = *nint;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    x[ivar + 1] = xt;
	    x[ivar + 2] = 1.;
	    xt += h__;
	    ivar += npi;
// L70:
	}
	return 0;
    }
//  Evaluate the function if task = 'F', the Jacobian matrix if
//  task = 'J', or both if task = 'FJ'.
//  Initialize arrays.
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (s_cmp(task, "F", task_len, (ftnlen)1) == 0 || s_cmp(task, "FJ",
		task_len, (ftnlen)2) == 0) {
	    fvec[i__] = 0.;
	}
	if (s_cmp(task, "J", task_len, (ftnlen)1) == 0 || s_cmp(task, "FJ",
		task_len, (ftnlen)2) == 0) {
	    i__2 = *n;
	    for (j = 1; j <= i__2; ++j) {
		fjac[i__ + j * fjac_dim1] = 0.;
// L80:
	    }
	}
// L90:
    }
    for (k = 1; k <= 3; ++k) {
	for (j = 1; j <= 2; ++j) {
	    w[j + (k << 1) - 3] = 0.;
	    for (l = 1; l <= 5; ++l) {
		dw[j + (l + k * 5 << 1) - 13] = 0.;
// L100:
	    }
// L110:
	}
// L120:
    }
//  Satisfy initial conditions at t = 0.  yi(0) = icond(i).
    for (i__ = 1; i__ <= 3; ++i__) {
	if (s_cmp(task, "F", task_len, (ftnlen)1) == 0 || s_cmp(task, "FJ",
		task_len, (ftnlen)2) == 0) {
	    fvec[i__] = x[(i__ - 1 << 2) + sumdeg[i__ - 1] + 1] - icond[i__ -
		    1];
	}
	if (s_cmp(task, "J", task_len, (ftnlen)1) == 0 || s_cmp(task, "FJ",
		task_len, (ftnlen)2) == 0) {
	    fjac[i__ + ((i__ - 1 << 2) + sumdeg[i__ - 1] + 1) * fjac_dim1] =
		    1.;
	}
// L130:
    }
//  Set up the collocation equations.
    i__1 = *nint;
    for (i__ = 1; i__ <= i__1; ++i__) {
	for (k = 1; k <= 4; ++k) {
	    for (e = 1; e <= 3; ++e) {
		var[e - 1] = (i__ - 1) * npi + (e - 1 << 2) + sumdeg[e - 1];
		eqn[e - 1] = (i__ - 1) * npi + 3 + (e - 1 << 2);
		i__2 = deg[e - 1] + 1;
		for (m = 1; m <= i__2; ++m) {
		    w[m + (e << 1) - 3] = 0.;
		    i__3 = deg[e - 1];
		    for (j = m; j <= i__3; ++j) {
			w[m + (e << 1) - 3] += x[var[e - 1] + j] * rhnfhk[k +
				(j - m + (j - m + (j - m) * 5) * 5 << 2) - 1];
			dw[m + (j + e * 5 << 1) - 13] = rhnfhk[k + (j - m + (
				j - m + (j - m) * 5) * 5 << 2) - 1];
// L140:
		    }
		    for (j = 1; j <= 4; ++j) {
			w[m + (e << 1) - 3] += x[var[e - 1] + deg[e - 1] + j]
				* rhnfhk[k + (deg[e - 1] + j - m + (deg[e - 1]
				 + j - m + (deg[e - 1] - m + 1) * 5) * 5 << 2)
				 - 1];
			dw[m + (deg[e - 1] + j + e * 5 << 1) - 13] = rhnfhk[k
				+ (deg[e - 1] + j - m + (deg[e - 1] + j - m +
				(deg[e - 1] - m + 1) * 5) * 5 << 2) - 1];
// L150:
		    }
// L160:
		}
// L170:
	    }
	    if (s_cmp(task, "F", task_len, (ftnlen)1) == 0 || s_cmp(task,
		    "FJ", task_len, (ftnlen)2) == 0) {
		fvec[eqn[0] + k] = w[1] - cos(w[4]);
		fvec[eqn[1] + k] = w[3] - sin(w[4]);
		fvec[eqn[2] + k] = w[5] - x[*n - 2] * w[0] + x[*n - 1] * w[2]
			- x[*n];
	    }
	    if (s_cmp(task, "J", task_len, (ftnlen)1) == 0 || s_cmp(task,
		    "FJ", task_len, (ftnlen)2) == 0) {
		i__2 = deg[0] + 4;
		for (j = 1; j <= i__2; ++j) {
		    fjac[eqn[0] + k + (var[0] + j) * fjac_dim1] = dw[(j + 5 <<
			     1) - 11];
		    fjac[eqn[2] + k + (var[0] + j) * fjac_dim1] = -x[*n - 2] *
			     dw[(j + 5 << 1) - 12];
		    fjac[eqn[1] + k + (var[1] + j) * fjac_dim1] = dw[(j + 10
			    << 1) - 11];
		    fjac[eqn[2] + k + (var[1] + j) * fjac_dim1] = x[*n - 1] *
			    dw[(j + 10 << 1) - 12];
		    fjac[eqn[0] + k + (var[2] + j) * fjac_dim1] = sin(w[4]) *
			    dw[(j + 15 << 1) - 12];
		    fjac[eqn[1] + k + (var[2] + j) * fjac_dim1] = -cos(w[4]) *
			     dw[(j + 15 << 1) - 12];
		    fjac[eqn[2] + k + (var[2] + j) * fjac_dim1] = dw[(j + 15
			    << 1) - 11];
// L180:
		}
		fjac[eqn[2] + k + (*n - 2) * fjac_dim1] = -w[0];
		fjac[eqn[2] + k + (*n - 1) * fjac_dim1] = w[2];
		fjac[eqn[2] + k + *n * fjac_dim1] = -1.;
	    }
// L190:
	}
// L200:
    }
//  Set up the continuity equations.
    i__1 = *nint;
    for (i__ = 1; i__ <= i__1; ++i__) {
	for (e = 1; e <= 3; ++e) {
	    var[e - 1] = (i__ - 1) * npi + (e - 1 << 2) + sumdeg[e - 1];
	    eqn[e - 1] = (i__ - 1) * npi + 15 + sumdeg[e - 1];
	    i__2 = deg[e - 1];
	    for (m = 1; m <= i__2; ++m) {
		w[m + (e << 1) - 3] = 0.;
		i__3 = deg[e - 1];
		for (j = m; j <= i__3; ++j) {
		    w[m + (e << 1) - 3] += rhnfhk[(j - m + (j - m) * 5) * 20]
			    * x[var[e - 1] + j];
		    dw[m + (j + e * 5 << 1) - 13] = rhnfhk[(j - m + (j - m) *
			    5) * 20];
// L210:
		}
		for (j = 1; j <= 4; ++j) {
		    w[m + (e << 1) - 3] += x[var[e - 1] + deg[e - 1] + j] *
			    rhnfhk[(deg[e - 1] + j - m + (deg[e - 1] - m + 1)
			    * 5) * 20];
		    dw[m + (deg[e - 1] + j + e * 5 << 1) - 13] = rhnfhk[(deg[
			    e - 1] + j - m + (deg[e - 1] - m + 1) * 5) * 20];
// L220:
		}
// L230:
	    }
// L240:
	}
	if (i__ == *nint) {
	    goto L290;
	}
	for (e = 1; e <= 3; ++e) {
	    i__2 = deg[e - 1];
	    for (m = 1; m <= i__2; ++m) {
		if (s_cmp(task, "F", task_len, (ftnlen)1) == 0 || s_cmp(task,
			"FJ", task_len, (ftnlen)2) == 0) {
		    fvec[eqn[e - 1] + m] = x[var[e - 1] + npi + m] - w[m + (e
			    << 1) - 3];
		}
		if (s_cmp(task, "J", task_len, (ftnlen)1) == 0 || s_cmp(task,
			"FJ", task_len, (ftnlen)2) == 0) {
		    fjac[eqn[e - 1] + m + (var[e - 1] + npi + m) * fjac_dim1]
			    = 1.;
		    i__3 = deg[e - 1] + 4;
		    for (j = 1; j <= i__3; ++j) {
			fjac[eqn[e - 1] + m + (var[e - 1] + j) * fjac_dim1] =
				-dw[m + (j + e * 5 << 1) - 13];
// L250:
		    }
		}
// L260:
	    }
// L270:
	}
// L280:
    }
L290:
    if (s_cmp(task, "F", task_len, (ftnlen)1) == 0 || s_cmp(task, "FJ",
	    task_len, (ftnlen)2) == 0) {
	fvec[*n - 2] = w[0] - *a;
	fvec[*n - 1] = w[2] - *b;
	fvec[*n] = w[4] - *c__;
    }
    if (s_cmp(task, "J", task_len, (ftnlen)1) == 0 || s_cmp(task, "FJ",
	    task_len, (ftnlen)2) == 0) {
	var[0] = *n - npi - 3;
	var[1] = var[0] + 5;
	var[2] = var[1] + 5;
	for (j = 1; j <= 5; ++j) {
	    fjac[*n - 2 + (var[0] + j) * fjac_dim1] = dw[(j + 5 << 1) - 12];
	    fjac[*n - 1 + (var[1] + j) * fjac_dim1] = dw[(j + 10 << 1) - 12];
	    fjac[*n + (var[2] + j) * fjac_dim1] = dw[(j + 15 << 1) - 12];
// L300:
	}
    }
    return 0;
} // dierfj_



# endif
