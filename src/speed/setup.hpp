// vim: set expandtab:
# ifndef SRC_SPEED_SETUP_HPP
# define SRC_SPEED_SETUP_HPP
/*
{xsrst_begin speed_setup}

.. include:: ../preamble.rst

{xsrst_spell
    hes
    jac
    adolc
    cppad
    subgraph
    cppadcg
    subcg
    ptr
    globals
    nnz
}

Setup a Speed Test Problem
##########################

Syntax
******
``setup_`` *implement* ()

Prototype
*********
{xsrst_code hpp} */
extern void setup_adolc(void);
extern void setup_cppad(void);
extern void setup_subgraph(void);
extern void setup_cppadcg(void);
extern void setup_subcg(void);
/* {xsrst_code}

Purpose
*******
Do as much of the calculation as possible, excluding knowing
the argument value :math:`x` at which we are evaluating the Jacobian or
Hessian of :math:`f(x)`.

implement
*********
is one of the following
``adolc`` ,
``cppad`` ,
``subgraph`` ,
``cppadcg`` ,
``subcg`` .

Globals
*******

Initialization
==============
The :ref:`initialization<speed_global.initialization>` global variables
are inputs and not changed by the setup routines.

m
=
We use *m* for the value ``global_problem_ptr->size_range()``
see :ref:`global_problem_ptr<speed_global.initialization.global_problem_ptr>`.
This is the dimension of the range space for :math:`f(x)`.

Hessian
=======
A problem is a Hessian test case if *m* = 1 .
In the case ``test_`` *implement* ``_hes`` will be called to determine
the speed of evaluation of the Hessian.

Jacobian
========
A problem is a Jacobian test case if *m* > 1 .
In the case ``test_`` *implement* ``_jac`` will be called to determine
the speed of evaluation of the Jacobian.

Not used
========
:ref:`global_nnz<speed_global.global_nnz>` and
:ref:`global_correct_ok<speed_global.global_correct_ok>`
are not used by these routines.

{xsrst_end speed_setup}
*/

# endif
