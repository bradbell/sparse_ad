// vim: set expandtab:
# ifndef SRC_SPEED_TEST_HPP
# define SRC_SPEED_TEST_HPP
/*
{xsrst_begin speed_test}

.. include:: ../preamble.rst

{xsrst_spell
    hes
    jac
    ptr
    globals
    nnz
}

Setup a Speed Test Problem
##########################

Syntax
******

| ``test_`` *implement* ``_jac`` ( *repeat* )

``%test_%implement%_hes(%repeat%)%``

Prototype
*********

Jacobian
========
{xsrst_code hpp} */
extern void test_adolc_jac(size_t repeat);
extern void test_cppad_jac(size_t repeat);
extern void test_subgraph_jac(size_t repeat);
extern void test_cppadcg_jac(size_t repeat);
extern void test_subcg_jac(size_t repeat);
/* {xsrst_code}

Hessian
=======
{xsrst_code hpp} */
extern void test_adolc_hes(size_t repeat);
extern void test_cppad_hes(size_t repeat);
extern void test_subgraph_hes(size_t repeat);
extern void test_cppadcg_hes(size_t repeat);
extern void test_subcg_hes(size_t repeat);
/* {xsrst_code}

Purpose
*******
The Jacobian (Hessian) routines calculate the Jacobian (Hessian)
for objective corresponding to the problem specified by
:ref:`global_problem_ptr<speed_global.initialization.global_problem_ptr>`

implement
*********
is one of the following
``adolc`` ,
``cppad`` ,
``subgraph`` ,
``cppadcg`` ,
``subcg`` .

Setup
*****
The routine ``setup_`` *implement* () is called before
``test_`` *implement* ``_jac`` ( *repeat* ) or
``%test_%implement%_hes(%repeat%)%`` .

Globals
*******

Initialization
==============
The :ref:`initialization<speed_global.initialization>` global variables
are inputs and not changed by the setup routines.
In addition, the have the same values as during the call to
``setup_`` *implement* () .

m
=
We use *m* for the value ``global_problem_ptr->size_range()``
see :ref:`global_problem_ptr<speed_global.initialization.global_problem_ptr>`.
This is the dimension of the range space for :math:`f(x)`.

Jacobian
========
If *m* > 1 , the Jacobian test for this implementation is called.

Hessian
=======
If *m* = 1 , the Hessian test for this implementation is called.

global_nnz
==========
The input value of this global does not matter.
Upon return, it has been set to the number of non-zeros in the sparsity
pattern for the sparse Jacobian or Hessian that is calculated.

global_correct_ok
=================
The input value of this global does not matter.
If :ref:`global_correct<speed_global.initialization.global_correct>`
is true, ``global_correct_ok`` has been set to true (false)
if the result calculated by the last repetition of the test
passes (fails) the correctness test; see :ref:`check_sparse<check_sparse>`.

{xsrst_end speed_test}
*/

# endif
