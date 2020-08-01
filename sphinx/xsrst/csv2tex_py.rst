!!!!!!!!!!
csv2tex_py
!!!!!!!!!!

.. include:: ../preamble.rst

.. meta::
   :keywords: csv2tex_py, extract, latex, tables, from, speed, test, csv, file

.. index:: csv2tex_py, extract, latex, tables, from, speed, test, csv, file

.. _csv2tex_py:

Extract Latex Tables From Speed Test Csv File
#############################################
- :ref:`csv2tex_py.syntax`
- :ref:`csv2tex_py.csv_file`
- :ref:`csv2tex_py.output_files`
    - :ref:`csv2tex_py.output_files.subset`
    - :ref:`csv2tex_py.output_files.problem`
    - :ref:`csv2tex_py.output_files.files`

.. meta::
   :keywords: syntax

.. index:: syntax

.. _csv2tex_py.syntax:

Syntax
******
``bin/csv2tex.py`` *csv_file*

.. meta::
   :keywords: csv, file

.. index:: csv, file

.. _csv2tex_py.csv_file:

Csv File
********
Each line of this input file corresponds to one run of the
:ref:`speed<speed>` program.

.. meta::
   :keywords: output, files

.. index:: output, files

.. _csv2tex_py.output_files:

Output Files
************

.. meta::
   :keywords: subset

.. index:: subset

.. _csv2tex_py.output_files.subset:

Subset
======
Each one is a latex table that corresponds to a subset of the input data
where *problem* , and *setup* have one value that corresponds
to the file name.
For each subset, only the combination of options that has the smallest
value in the *sec* column is included.

.. meta::
   :keywords: problem

.. index:: problem

.. _csv2tex_py.output_files.problem:

Problem
=======
The possible value for *problem* are
``deptfg`` , ``dierfj`` , ``dficfj`` .
We use *Problem* to denote the *problem*
with the first letter in upper case; e.g.,
``Deptfg`` is the *Problem* corresponding to ``deptfg`` .

.. meta::
   :keywords: files

.. index:: files

.. _csv2tex_py.output_files.files:

Files
=====
The output files are written in the ``build/tex`` directory.
For each of the possible problems, the output file
*Problem* ``YesSetup.tex`` ( *Problem* ``NoSetup.tex`` )
corresponds to *setup* true (false).

----

xsrst input file: ``bin/csv2tex.py``
