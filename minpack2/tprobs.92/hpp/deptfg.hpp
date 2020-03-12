# ifndef MINPACK2_TPROBS_92_HPP_DEPTFG_HPP
# define MINPACK2_TPROBS_92_HPP_DEPTFG_HPP

# include <cmath>
# include <cppad/cg/cppadcg.hpp>
# include <adolc/adolc.h>
# include "f2clib.hpp"

template <class doublereal, class integer, class ftnlen>
int deptfg(integer *nx, integer *ny, doublereal *x,
	doublereal *f, doublereal *fgrad, char *task, doublereal *c__, ftnlen
	task_len)
{
    // System generated locals
    integer i__1, i__2, i__3, i__4;
    doublereal d__1, d__2;

    // Builtin functions

    // Local variables
    integer i__, j, k;
    doublereal v, vb, hx, hy, vl, vr, vt, area, flin, dvdx, dvdy, temp, cdiv3,
	     temp1, fquad;
//
//  **********
//
//  Subroutine deptfg
//
//  This subroutine computes the function and gradient of the
//  Elastic-Plastic Torsion problem as formulated by, eg., Glowinski.
//  This problem arises in the determination of the stress field on an
//  infinitely long cylindrical bar subject to a torsional couple
//  characterized by an angle of twist per unit length c. A finite
//  element approximation to the torsion problem is obtained by using
//  piecewise linear basis functions over a triangulation of the unit
//  square.
//
//  The subroutine statement is:
//
//    subroutine deptfg(nx,ny,x,f,fgrad,task,c)
//
//  where
//
//    nx is an integer variable.
//      On entry nx is the number of grid points in the first
//         coordinate direction.
//      On exit nx is unchanged.
//
//    ny is an integer variable.
//      On entry ny is the number of grid points in the second
//         coordinate direction.
//      On exit ny is unchanged.
//
//    x is a double precision array of dimension nx*ny.
//      On entry x specifies the vector x if task = 'F', 'G', or 'FG'.
//         Otherwise x need not be specified.
//      On exit x is unchanged if task = 'F', 'G', or 'FG'. Otherwise
//         x is set according to task.
//
//    f is a double precision variable.
//      On entry f need not be specified.
//      On exit f is set to the function evaluated at x if task = 'F'
//         or 'FG'.
//
//    fgrad is a double precision array of dimension nx*ny.
//      On entry fgrad need not be specified.
//      On exit fgrad contains the gradient evaluated at x if
//         task = 'G' or 'FG'.
//
//    task is a character variable.
//      On entry task specifies the action of the subroutine:
//
//         task               action
//         ----               ------
//          'F'     Evaluate the function at x.
//          'G'     Evaluate the gradient at x.
//          'FG'    Evaluate the function and the gradient at x.
//          'XS'    Set x to the standard starting point xs.
//          'XL'    Set x to the lower bound xl.
//          'XU'    Set x to the upper bound xu.
//
//      On exit task is unchanged.
//
//    c is a double precision variable.
//      On entry c is the angle of twist per unit length.
//      On exit c is unchanged.
//
//  MINPACK-2 Project. October 1992.
//  Argonne National Laboratory and University of Minnesota.
//  Brett M. Averick and Jorge J. More'.
//
//  **********
    // Parameter adjustments
    --fgrad;
    --x;

    // Function Body
    hx = 1. / (doublereal) (*nx + 1);
    hy = 1. / (doublereal) (*ny + 1);
    area = hx * .5 * hy;
    cdiv3 = *c__ / 3.;
//  Compute a lower bound for x if task = 'XL' or an upper bound if
//  task = 'XU'.
    if (s_cmp(task, "XL", task_len, (ftnlen)2) == 0 || s_cmp(task, "XU",
	    task_len, (ftnlen)2) == 0) {
	if (s_cmp(task, "XL", task_len, (ftnlen)2) == 0) {
	    temp1 = -1.;
	}
	if (s_cmp(task, "XU", task_len, (ftnlen)2) == 0) {
	    temp1 = 1.;
	}
	i__1 = *ny;
	for (j = 1; j <= i__1; ++j) {
// Computing MIN
	    i__2 = j, i__3 = *ny - j + 1;
	    temp = (doublereal) min(i__2,i__3) * hy;
	    i__2 = *nx;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		k = *nx * (j - 1) + i__;
// Computing MIN
// Computing MIN
		i__3 = i__, i__4 = *nx - i__ + 1;
		d__2 = (doublereal) min(i__3,i__4) * hx;
		d__1 = min(d__2,temp);
		x[k] = d_sign(&d__1, &temp1);
// L10:
	    }
// L20:
	}
	return 0;
    }
//  Compute the standard starting point if task = 'XS'.
    if (s_cmp(task, "XS", task_len, (ftnlen)2) == 0) {
	i__1 = *ny;
	for (j = 1; j <= i__1; ++j) {
// Computing MIN
	    i__2 = j, i__3 = *ny - j + 1;
	    temp = (doublereal) min(i__2,i__3) * hy;
	    i__2 = *nx;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		k = *nx * (j - 1) + i__;
// Computing MIN
// Computing MIN
		i__3 = i__, i__4 = *nx - i__ + 1;
		d__1 = (doublereal) min(i__3,i__4) * hx;
		x[k] = min(d__1,temp);
// L30:
	    }
// L40:
	}
	return 0;
    }
//  Compute the function if task = 'F', the gradient if task = 'G', or
//  both if task = 'FG'.
    if (s_cmp(task, "F", task_len, (ftnlen)1) == 0 || s_cmp(task, "FG",
	    task_len, (ftnlen)2) == 0) {
	fquad = 0.;
	flin = 0.;
    }
    if (s_cmp(task, "G", task_len, (ftnlen)1) == 0 || s_cmp(task, "FG",
	    task_len, (ftnlen)2) == 0) {
	i__1 = *nx * *ny;
	for (k = 1; k <= i__1; ++k) {
	    fgrad[k] = 0.;
// L50:
	}
    }
//  Computation of the function and the gradient over the lower
//  triangular elements.
    i__1 = *ny;
    for (j = 0; j <= i__1; ++j) {
	i__2 = *nx;
	for (i__ = 0; i__ <= i__2; ++i__) {
	    k = *nx * (j - 1) + i__;
	    v = 0.;
	    vr = 0.;
	    vt = 0.;
	    if (i__ >= 1 && j >= 1) {
		v = x[k];
	    }
	    if (i__ < *nx && j > 0) {
		vr = x[k + 1];
	    }
	    if (i__ > 0 && j < *ny) {
		vt = x[k + *nx];
	    }
	    dvdx = (vr - v) / hx;
	    dvdy = (vt - v) / hy;
	    if (s_cmp(task, "F", task_len, (ftnlen)1) == 0 || s_cmp(task,
		    "FG", task_len, (ftnlen)2) == 0) {
// Computing 2nd power
		d__1 = dvdx;
// Computing 2nd power
		d__2 = dvdy;
		fquad = fquad + d__1 * d__1 + d__2 * d__2;
		flin -= cdiv3 * (v + vr + vt);
	    }
	    if (s_cmp(task, "G", task_len, (ftnlen)1) == 0 || s_cmp(task,
		    "FG", task_len, (ftnlen)2) == 0) {
		if (i__ != 0 && j != 0) {
		    fgrad[k] = fgrad[k] - dvdx / hx - dvdy / hy - cdiv3;
		}
		if (i__ != *nx && j != 0) {
		    fgrad[k + 1] = fgrad[k + 1] + dvdx / hx - cdiv3;
		}
		if (i__ != 0 && j != *ny) {
		    fgrad[k + *nx] = fgrad[k + *nx] + dvdy / hy - cdiv3;
		}
	    }
// L60:
	}
// L70:
    }
//  Computation of the function and the gradient over the upper
//  triangular elements.
    i__1 = *ny + 1;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *nx + 1;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    k = *nx * (j - 1) + i__;
	    vb = 0.;
	    vl = 0.;
	    v = 0.;
	    if (i__ <= *nx && j > 1) {
		vb = x[k - *nx];
	    }
	    if (i__ > 1 && j <= *ny) {
		vl = x[k - 1];
	    }
	    if (i__ <= *nx && j <= *ny) {
		v = x[k];
	    }
	    dvdx = (v - vl) / hx;
	    dvdy = (v - vb) / hy;
	    if (s_cmp(task, "F", task_len, (ftnlen)1) == 0 || s_cmp(task,
		    "FG", task_len, (ftnlen)2) == 0) {
// Computing 2nd power
		d__1 = dvdx;
// Computing 2nd power
		d__2 = dvdy;
		fquad = fquad + d__1 * d__1 + d__2 * d__2;
		flin -= cdiv3 * (vb + vl + v);
	    }
	    if (s_cmp(task, "G", task_len, (ftnlen)1) == 0 || s_cmp(task,
		    "FG", task_len, (ftnlen)2) == 0) {
		if (i__ != *nx + 1 && j != 1) {
		    fgrad[k - *nx] = fgrad[k - *nx] - dvdy / hy - cdiv3;
		}
		if (i__ != 1 && j != *ny + 1) {
		    fgrad[k - 1] = fgrad[k - 1] - dvdx / hx - cdiv3;
		}
		if (i__ != *nx + 1 && j != *ny + 1) {
		    fgrad[k] = fgrad[k] + dvdx / hx + dvdy / hy - cdiv3;
		}
	    }
// L80:
	}
// L90:
    }
//  Scale the result.
    if (s_cmp(task, "F", task_len, (ftnlen)1) == 0 || s_cmp(task, "FG",
	    task_len, (ftnlen)2) == 0) {
	*f = area * (fquad * .5 + flin);
    }
    if (s_cmp(task, "G", task_len, (ftnlen)1) == 0 || s_cmp(task, "FG",
	    task_len, (ftnlen)2) == 0) {
	i__1 = *nx * *ny;
	for (k = 1; k <= i__1; ++k) {
	    fgrad[k] = area * fgrad[k];
// L100:
	}
    }
    return 0;
} // deptfg_



# endif
