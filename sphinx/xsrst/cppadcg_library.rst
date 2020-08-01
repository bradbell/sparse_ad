!!!!!!!!!!!!!!!
cppadcg_library
!!!!!!!!!!!!!!!

.. include:: ../preamble.rst

.. meta::
   :keywords: cppadcg_library, cppadcodegen, dynamic, libraries

.. index:: cppadcg_library, cppadcodegen, dynamic, libraries

.. _cppadcg_library:

CppADCodeGen Dynamic Libraries
##############################
- :ref:`cppadcg_library.syntax`
- :ref:`cppadcg_library.prototype`
- :ref:`cppadcg_library.fun_name`
- :ref:`cppadcg_library.other_fun`
- :ref:`cppadcg_library.cg_fun`
- :ref:`cppadcg_library.file_name`
- :ref:`cppadcg_library.swap`
- :ref:`cppadcg_library.x`
- :ref:`cppadcg_library.y`

.. meta::
   :keywords: syntax

.. index:: syntax

.. _cppadcg_library.syntax:

Syntax
******

| ``cppadcg_library`` *fun_name* ( *cg_fun* , *file_name* )
| ``cppadcg_library`` *fun_name* ( *file_name* )
| ``cppadcg_library`` *fun_name* ()
| *fun_name* . ``swap`` ( *other_fun* )
| *y* = *fun_name* ( *x* )

.. meta::
   :keywords: prototype

.. index:: prototype

.. _cppadcg_library.prototype:

Prototype
*********

.. literalinclude:: ../../src/utility/cppadcg_library.cpp
    :lines: 90-92
    :language: cpp


.. literalinclude:: ../../src/utility/cppadcg_library.cpp
    :lines: 109-110
    :language: cpp


.. literalinclude:: ../../src/utility/cppadcg_library.cpp
    :lines: 125-125
    :language: cpp


.. literalinclude:: ../../src/utility/cppadcg_library.cpp
    :lines: 129-129
    :language: cpp


.. literalinclude:: ../../src/utility/cppadcg_library.cpp
    :lines: 135-135
    :language: cpp

.. meta::
   :keywords: fun_name

.. index:: fun_name

.. _cppadcg_library.fun_name:

fun_name
********
This is the name of the ``cppadcg_library`` object.

.. meta::
   :keywords: other_fun

.. index:: other_fun

.. _cppadcg_library.other_fun:

other_fun
*********
This is the name of another ``cppadcg_library`` object.

.. meta::
   :keywords: cg_fun

.. index:: cg_fun

.. _cppadcg_library.cg_fun:

cg_fun
******
This is a CppAD function object that corresponds to a function
:math:`f : \B{R}^n \rightarrow \B{R}^m`.
If this arguments is present in the constructor,
a new dynamic library is created.

.. meta::
   :keywords: file_name

.. index:: file_name

.. _cppadcg_library.file_name:

file_name
*********
This is the absolute or relative path for the
file that contains the dynamic library.
It does not include the files extension at the end that is used
for dynamic libraries on this system.
If *cg_fun* is not present in the constructor,
it must have been present in a previous constructor with the same
*file_name* .

.. meta::
   :keywords: swap

.. index:: swap

.. _cppadcg_library.swap:

swap
****
This exchanges the library in *fun_name* with the library in
*other_fun* .

.. meta::
   :keywords: x

.. index:: x

.. _cppadcg_library.x:

x
*
is a vector of size *n* specifying the argument value
at which the function will be evaluated.

.. meta::
   :keywords: y

.. index:: y

.. _cppadcg_library.y:

y
*
This return value has size *m* and is the value of :math:`f(x)`.

----

xsrst input file: ``src/utility/cppadcg_library.cpp``
