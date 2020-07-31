# ifndef SRC_TYPEDEF_HPP
# define SRC_TYPEDEF_HPP

# include <adolc/adolc.h>
# include <cppad/cg/cppadcg.hpp>

/*
{xsrst_begin typedef_hpp}

.. include:: ../preamble.rst

{xsrst_spell
    ftnlen
    typedef
    hpp
    cg
    adouble
}

Type Definitions
################

Syntax
******

| ``# include <src/typedef.hpp`` >

Scalar Types
************

ftnlen
======
type used by f2c for fortran integers:
{xsrst_code hpp} */
typedef int                          ftnlen;
/* {xsrst_code}

adolc_double
============
type used by adolc implementation to record operations:
{xsrst_code hpp} */
typedef adouble                      adolc_double;
/* {xsrst_code}

cppad_double
============
type used by cppad and subgraph implementations to record operations:
{xsrst_code hpp} */
typedef CppAD::AD<double>            cppad_double;
/* {xsrst_code}

cg_double
=========
type used by cppadcg implementations to evaluate derivatives
{xsrst_code hpp} */
typedef CppAD::cg::CG<double>        cg_double;
/* {xsrst_code}

cppadcg_double
==============
type used by cppadcg implementations to record operations:
{xsrst_code hpp} */
typedef CppAD::AD<cg_double>         cppadcg_double;
/* {xsrst_code}

Vector Types
************
``CppAD::vector`` < *T* > is a vector with elements of type *T* and
``CppAD::vector`` < *T* > ``::data`` () is the corresponding *T* *
pointer.

d_vector
========
{xsrst_code hpp} */
typedef CppAD::vector<double>          d_vector;
/* {xsrst_code}

c_vector
========
{xsrst_code hpp} */
typedef CppAD::vector<char>            c_vector;
/* {xsrst_code}

s_vector
========
{xsrst_code hpp} */
typedef CppAD::vector<size_t>          s_vector;
/* {xsrst_code}

b_vector
========
{xsrst_code hpp} */
typedef CppAD::vector<bool>            b_vector;
/* {xsrst_code}

adolc_vector
============
{xsrst_code hpp} */
typedef CppAD::vector<adolc_double>    adolc_vector;
/* {xsrst_code}

cppad_vector
============
{xsrst_code hpp} */
typedef CppAD::vector<cppad_double>    cppad_vector;

/* {xsrst_code}

cg_vector
=========
{xsrst_code hpp} */
typedef CppAD::vector<cg_double>       cg_vector;

/* {xsrst_code}

cppadcg_vector
==============
{xsrst_code hpp} */
typedef CppAD::vector<cppadcg_double>  cppadcg_vector;

/* {xsrst_code}
{xsrst_end typedef_hpp}
*/

# endif
