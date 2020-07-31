# ifndef SRC_FUN_OBJ_FUN_BASE_HPP
# define SRC_FUN_OBJ_FUN_BASE_HPP


/*
{xsrst_begin fun_base_hpp}

.. include:: ../preamble.rst

{xsrst_spell
    obj
    const
    jac
    prototyped
    hes
    hpp
    grad
    destructor
    typedef
}

Base Class For Test Function Objects
####################################

Syntax
******

| ``# include <src/fun_obj/fun_base.hpp`` >
| *x_out* = *fun_obj* . ``start`` ()
| *m* = *fun_obj* . ``size_range`` ()
| *fun_obj* . ``fun`` ( *x* , *fun_out* )
| *ok* = *fun_obj* . ``jac`` ( *x* , *jac_out* )
| *ok* = *fun_obj* . ``grad`` ( *x* , *grad_out* )
| *ok* = *fun_obj* . ``hes`` ( *x* , *hes_out* )

Purpose
*******
Each of the test functions is a mapping
:math:`f : \B{R}^n \rightarrow \B{R}^m`.
It is defined using a class that is derived from the base class
``fun_base`` .

fun_obj
*******
We use the notation *fun_obj* for a derived class object
corresponding to the function :math:`f(x)`.

Constructor
***********
The constructor (and destructor) for a ``fun_base`` has no arguments.

Type Definitions
****************
The file :ref:`typedef_hpp<typedef_hpp>` is included by ``fun_base.hpp`` .

start
*****
This function returns the standard starting point for point for optimizing
the function. It has the following prototype
{xsrst_file
    // BEGIN_START
    // END_START
}

x_out
=====
This vector is the starting point.

n
=
The dimension of the domain space is equal to the size of *x_out* .

size_range
**********
This function has the following prototype
{xsrst_file
    // BEGIN_SIZE_RANGE
    // END_SIZE_RANGE
}
The corresponding virtual function must be defined by the derived class.

m
=
The return value *m* is the dimension of the range.

fun
***
These functions function evaluate :math:`f(x)`.
They have the following prototypes
{xsrst_file
    // BEGIN_FUN
    // END_FUN
}
The corresponding virtual functions must be defined by the derived class.

x
=
The argument *x* has size *n* and specifies
the point at which the function is evaluated.
It is effectively ``const`` , but is not prototyped as such
in order to avoid the time to copy the vector to a temporary.

fun_out
=======
The return value *fun_out* has size *m* .
The input value of its elements does not matter.
Upon return, it contains the value of :math:`f(x)`.

jac
***
This function evaluates :math:`f'(x)`
It has the following prototype
{xsrst_file
    // BEGIN_JAC
    // END_JAC
}
The corresponding virtual functions must be defined by the derived class.

ok
==
If *m* == 1 , this function returns *ok* as false
and no other action is taken.
Otherwise *ok* is true.

x
=
The argument *x* has size *n* and specifies
the point at which the function is evaluated.
It is effectively ``const`` , but is not prototyped as such
in order to avoid the time to copy the vector to a temporary.

jac_out
=======
The return value *jac_out* has size *m* * *n* .
The input value of its elements does not matter.
Upon return, it contains the value of :math:`f'(x)`.
To be specific,
for *i* = 0 , ... , *m* -1 ,
*j* = 0 , ... , *n* -1 ,
*jac_out* [ *i* + *m* * *j* ]
is the partial of :math:`f_i (x)` with respect to :math:`x_j`.

grad
****
This function evaluates :math:`f'(x)`.
It has the following prototype
{xsrst_file
    // BEGIN_GRAD
    // END_GRAD
}
The corresponding virtual functions must be defined by the derived class.

ok
==
If *m* ``!= 1`` , this function returns *ok* as false
and no other action is taken.
Otherwise *ok* is true.

x
=
The argument *x* has size *n* and specifies
the point at which the function is evaluated.
It is effectively ``const`` , but is not prototyped as such
in order to avoid the time to copy the vector to a temporary.

grad_out
========
The return value *grad_out* has size *n* .
The input value of its elements does not matter.
Upon return, it contains the value of :math:`f'(x)`.
To be specific,
for *j* = 0 , ... , *n* -1 ,
*grad_out* [ *j* ]
is the partial of :math:`f(x)` with respect to :math:`x_j`.

hes
***
This function evaluates :math:`f^{(2)}(x)`.
It has the following prototype
{xsrst_file
    // BEGIN_HES
    // END_HES
}
The corresponding virtual functions must be defined by the derived class.

ok
==
If *m* ``!= 1`` , this function returns *ok* as false
and no other action is taken.
Otherwise *ok* is true.

x
=
The argument *x* has size *n* and specifies
the point at which the function is evaluated.
It is effectively ``const`` , but is not prototyped as such
in order to avoid the time to copy the vector to a temporary.

hes_out
=======
The return value *hes_out* has size *n* * *n* .
The input value of its elements does not matter.
Upon return, it contains the value of :math:`f^{(2)} (x)`.
To be specific,
for *j* = 0 , ... , *n* -1 ,
*k* = 0 , ... , *n* -1 ,
*hes_out* [ *j* + *k* * *n* ]
is the second partial of :math:`f(x)` with respect to
:math:`x_j` and :math:`x_k`.

{xsrst_end fun_base_hpp}
*/

# include <src/typedef.hpp>

// a purely virtual base class for the interfact to a test function
class fun_base {

public:
    // constructor, destructor
    fun_base(void);
    ~fun_base(void);

// BEGIN_START
    virtual d_vector start(void) = 0 ;
// END_START

// BEGIN_SIZE_RANGE
    virtual size_t size_range(void) = 0;
// END_SIZE_RANGE

// BEGIN_FUN
    virtual void fun(d_vector&       x, d_vector&       fun_out) = 0 ;
    virtual void fun(adolc_vector&   x, adolc_vector&   fun_out) = 0 ;
    virtual void fun(cppad_vector&   x, cppad_vector&   fun_out) = 0 ;
    virtual void fun(cppadcg_vector& x, cppadcg_vector& fun_out) = 0 ;
// END_FUN

// BEGIN_JAC
    virtual bool jac(d_vector& x, d_vector& jac_out) = 0 ;
// END_JAC

// BEGIN_GRAD
    virtual bool grad(d_vector& x, d_vector& grad_out) = 0 ;
// END_GRAD

// BEGIN_HES
    virtual bool hes(d_vector& x, d_vector& hes_out) = 0 ;
// END_HES
};

# endif
