!!!!!!!!!!!!!!
csv_summary_py
!!!!!!!!!!!!!!

.. include:: ../preamble.rst

.. meta::
   :keywords: csv_summary_py, summarize, a, csv, file's, speed, results

.. index:: csv_summary_py, summarize, a, csv, file's, speed, results

.. _csv_summary_py:

Summarize A CSV File's Speed Results
####################################
.. contents::
   :local:

.. meta::
   :keywords: syntax

.. index:: syntax

.. _csv_summary_py.syntax:

Syntax
******

| ``bin/csv_summary.py`` *csv_file* *problem* *setup*

.. meta::
   :keywords: csv_file

.. index:: csv_file

.. _csv_summary_py.csv_file:

csv_file
********
This is name of a csv file created buy :ref:`run_speed.sh<run_speed_sh>`.
It must begin with ``csv/``; i.e., it must be in the ``csv`` directory.
It must end with ``.csv``.

.. meta::
   :keywords: problem

.. index:: problem

.. _csv_summary_py.problem:

problem
*******
This is the :ref:`speed.problem` we are summarizing the results for and
must be one of the following:
``dficfj``, ``dierfj``, ``deptfg``, ``dgl1fg``.

.. meta::
   :keywords: setup

.. index:: setup

.. _csv_summary_py.setup:

setup
*****
This must be ``true`` or ``false`` and indicates in the
:ref:``speed.--setup`` time is included for this summary.

.. meta::
   :keywords: example, output

.. index:: example, output

.. _csv_summary_py.example_output:

Example Output
**************
Below is example output by this program (where the seconds column has been
truncated to 3 after the decimal::

  problem    = deptfg
  setup      = false
  onepass    = false
  correct    = false
  n          = 3600
  m          = 1
  KB        implement colpack   indirect  optimize  reverse   nnz       sec
  6199      subgraph  false     false     true      true      10680     0.011
  33603     cppad     true      false     true      true      10680     0.018
  15059     adolc     true      true      false     false     14161     0.038

----

xsrst input file: ``bin/csv_summary.py``
