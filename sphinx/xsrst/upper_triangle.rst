!!!!!!!!!!!!!!
upper_triangle
!!!!!!!!!!!!!!

.. include:: ../preamble.rst

.. meta::
   :keywords: upper_triangle, extract, upper, triangle, from, a, cppad, sparsity, pattern

.. index:: upper_triangle, extract, upper, triangle, from, a, cppad, sparsity, pattern

.. _upper_triangle:

Extract Upper Triangle From A CppAD Sparsity Pattern
####################################################
- :ref:`upper_triangle.syntax`
- :ref:`upper_triangle.prototype`
- :ref:`upper_triangle.pattern`
- :ref:`upper_triangle.upper`

.. meta::
   :keywords: syntax

.. index:: syntax

.. _upper_triangle.syntax:

Syntax
******
*upper* =  ``upper_triangle`` ( *pattern* )

.. meta::
   :keywords: prototype

.. index:: prototype

.. _upper_triangle.prototype:

Prototype
*********

.. literalinclude:: ../../src/utility/upper_triangle.cpp
    :lines: 39-40
    :language: cpp

.. meta::
   :keywords: pattern

.. index:: pattern

.. _upper_triangle.pattern:

pattern
*******
The a CppAD sparsity pattern.

.. meta::
   :keywords: upper

.. index:: upper

.. _upper_triangle.upper:

upper
*****
Is the CppAD sparsity pattern for the upper triangle of the
matrix; i.e., only elements with index ( *i* , *j* ) such that
*i* <= *j* are included in *upper* .

----

xsrst input file: ``src/utility/upper_triangle.cpp``
