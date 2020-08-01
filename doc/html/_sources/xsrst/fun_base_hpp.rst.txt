!!!!!!!!!!!!
fun_base_hpp
!!!!!!!!!!!!

.. include:: ../preamble.rst

.. meta::
   :keywords: fun_base_hpp, base, class, for, test, function, objects

.. index:: fun_base_hpp, base, class, for, test, function, objects

.. _fun_base_hpp:

Base Class For Test Function Objects
####################################
- :ref:`fun_base_hpp.syntax`
- :ref:`fun_base_hpp.purpose`
- :ref:`fun_base_hpp.fun_obj`
- :ref:`fun_base_hpp.constructor`
- :ref:`fun_base_hpp.type_definitions`
- :ref:`fun_base_hpp.start`
    - :ref:`fun_base_hpp.start.x_out`
    - :ref:`fun_base_hpp.start.n`
- :ref:`fun_base_hpp.size_range`
    - :ref:`fun_base_hpp.size_range.m`
- :ref:`fun_base_hpp.fun`
    - :ref:`fun_base_hpp.fun.x`
    - :ref:`fun_base_hpp.fun.fun_out`
- :ref:`fun_base_hpp.jac`
    - :ref:`fun_base_hpp.jac.ok`
    - :ref:`fun_base_hpp.jac.x`
    - :ref:`fun_base_hpp.jac.jac_out`
- :ref:`fun_base_hpp.grad`
    - :ref:`fun_base_hpp.grad.ok`
    - :ref:`fun_base_hpp.grad.x`
    - :ref:`fun_base_hpp.grad.grad_out`
- :ref:`fun_base_hpp.hes`
    - :ref:`fun_base_hpp.hes.ok`
    - :ref:`fun_base_hpp.hes.x`
    - :ref:`fun_base_hpp.hes.hes_out`

.. meta::
   :keywords: syntax

.. index:: syntax

.. _fun_base_hpp.syntax:

Syntax
******

| ``# include <src/fun_obj/fun_base.hpp`` >
| *x_out* = *fun_obj* . ``start`` ()
| *m* = *fun_obj* . ``size_range`` ()
| *fun_obj* . ``fun`` ( *x* , *fun_out* )
| *ok* = *fun_obj* . ``jac`` ( *x* , *jac_out* )
| *ok* = *fun_obj* . ``grad`` ( *x* , *grad_out* )
| *ok* = *fun_obj* . ``hes`` ( *x* , *hes_out* )

.. meta::
   :keywords: purpose

.. index:: purpose

.. _fun_base_hpp.purpose:

Purpose
*******
Each of the test functions is a mapping
:math:`f : \B{R}^n \rightarrow \B{R}^m`.
It is defined using a class that is derived from the base class
``fun_base`` .

.. meta::
   :keywords: fun_obj

.. index:: fun_obj

.. _fun_base_hpp.fun_obj:

fun_obj
*******
We use the notation *fun_obj* for a derived class object
corresponding to the function :math:`f(x)`.

.. meta::
   :keywords: constructor

.. index:: constructor

.. _fun_base_hpp.constructor:

Constructor
***********
The constructor (and destructor) for a ``fun_base`` has no arguments.

.. meta::
   :keywords: type, definitions

.. index:: type, definitions

.. _fun_base_hpp.type_definitions:

Type Definitions
****************
The file :ref:`typedef_hpp<typedef_hpp>` is included by ``fun_base.hpp`` .

.. meta::
   :keywords: start

.. index:: start

.. _fun_base_hpp.start:

start
*****
This function returns the standard starting point for point for optimizing
the function. It has the following prototype

.. literalinclude:: ../../src/fun_obj/fun_base.hpp
    :lines: 225-225
    :language: cpp

.. meta::
   :keywords: x_out

.. index:: x_out

.. _fun_base_hpp.start.x_out:

x_out
=====
This vector is the starting point.

.. meta::
   :keywords: n

.. index:: n

.. _fun_base_hpp.start.n:

n
=
The dimension of the domain space is equal to the size of *x_out* .

.. meta::
   :keywords: size_range

.. index:: size_range

.. _fun_base_hpp.size_range:

size_range
**********
This function has the following prototype

.. literalinclude:: ../../src/fun_obj/fun_base.hpp
    :lines: 229-229
    :language: cpp

The corresponding virtual function must be defined by the derived class.

.. meta::
   :keywords: m

.. index:: m

.. _fun_base_hpp.size_range.m:

m
=
The return value *m* is the dimension of the range.

.. meta::
   :keywords: fun

.. index:: fun

.. _fun_base_hpp.fun:

fun
***
These functions function evaluate :math:`f(x)`.
They have the following prototypes

.. literalinclude:: ../../src/fun_obj/fun_base.hpp
    :lines: 233-236
    :language: cpp

The corresponding virtual functions must be defined by the derived class.

.. meta::
   :keywords: x

.. index:: x

.. _fun_base_hpp.fun.x:

x
=
The argument *x* has size *n* and specifies
the point at which the function is evaluated.
It is effectively ``const`` , but is not prototyped as such
in order to avoid the time to copy the vector to a temporary.

.. meta::
   :keywords: fun_out

.. index:: fun_out

.. _fun_base_hpp.fun.fun_out:

fun_out
=======
The return value *fun_out* has size *m* .
The input value of its elements does not matter.
Upon return, it contains the value of :math:`f(x)`.

.. meta::
   :keywords: jac

.. index:: jac

.. _fun_base_hpp.jac:

jac
***
This function evaluates :math:`f'(x)`
It has the following prototype

.. literalinclude:: ../../src/fun_obj/fun_base.hpp
    :lines: 240-240
    :language: cpp

The corresponding virtual functions must be defined by the derived class.

.. meta::
   :keywords: ok

.. index:: ok

.. _fun_base_hpp.jac.ok:

ok
==
If *m* == 1 , this function returns *ok* as false
and no other action is taken.
Otherwise *ok* is true.

.. meta::
   :keywords: x

.. index:: x

.. _fun_base_hpp.jac.x:

x
=
The argument *x* has size *n* and specifies
the point at which the function is evaluated.
It is effectively ``const`` , but is not prototyped as such
in order to avoid the time to copy the vector to a temporary.

.. meta::
   :keywords: jac_out

.. index:: jac_out

.. _fun_base_hpp.jac.jac_out:

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

.. meta::
   :keywords: grad

.. index:: grad

.. _fun_base_hpp.grad:

grad
****
This function evaluates :math:`f'(x)`.
It has the following prototype

.. literalinclude:: ../../src/fun_obj/fun_base.hpp
    :lines: 244-244
    :language: cpp

The corresponding virtual functions must be defined by the derived class.

.. meta::
   :keywords: ok

.. index:: ok

.. _fun_base_hpp.grad.ok:

ok
==
If *m* ``!= 1`` , this function returns *ok* as false
and no other action is taken.
Otherwise *ok* is true.

.. meta::
   :keywords: x

.. index:: x

.. _fun_base_hpp.grad.x:

x
=
The argument *x* has size *n* and specifies
the point at which the function is evaluated.
It is effectively ``const`` , but is not prototyped as such
in order to avoid the time to copy the vector to a temporary.

.. meta::
   :keywords: grad_out

.. index:: grad_out

.. _fun_base_hpp.grad.grad_out:

grad_out
========
The return value *grad_out* has size *n* .
The input value of its elements does not matter.
Upon return, it contains the value of :math:`f'(x)`.
To be specific,
for *j* = 0 , ... , *n* -1 ,
*grad_out* [ *j* ]
is the partial of :math:`f(x)` with respect to :math:`x_j`.

.. meta::
   :keywords: hes

.. index:: hes

.. _fun_base_hpp.hes:

hes
***
This function evaluates :math:`f^{(2)}(x)`.
It has the following prototype

.. literalinclude:: ../../src/fun_obj/fun_base.hpp
    :lines: 248-248
    :language: cpp

The corresponding virtual functions must be defined by the derived class.

.. meta::
   :keywords: ok

.. index:: ok

.. _fun_base_hpp.hes.ok:

ok
==
If *m* ``!= 1`` , this function returns *ok* as false
and no other action is taken.
Otherwise *ok* is true.

.. meta::
   :keywords: x

.. index:: x

.. _fun_base_hpp.hes.x:

x
=
The argument *x* has size *n* and specifies
the point at which the function is evaluated.
It is effectively ``const`` , but is not prototyped as such
in order to avoid the time to copy the vector to a temporary.

.. meta::
   :keywords: hes_out

.. index:: hes_out

.. _fun_base_hpp.hes.hes_out:

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

----

xsrst input file: ``src/fun_obj/fun_base.hpp``
