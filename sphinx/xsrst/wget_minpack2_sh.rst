!!!!!!!!!!!!!!!!
wget_minpack2_sh
!!!!!!!!!!!!!!!!

.. include:: ../preamble.rst

.. meta::
   :keywords: wget_minpack2_sh, get, a, minpack2, file, from, argonne, national, laboratory

.. index:: wget_minpack2_sh, get, a, minpack2, file, from, argonne, national, laboratory

.. _wget_minpack2_sh:

Get a Minpack2 File From Argonne National Laboratory
####################################################
- :ref:`wget_minpack2_sh.syntax`
- :ref:`wget_minpack2_sh.working_directory`
- :ref:`wget_minpack2_sh.file`
- :ref:`wget_minpack2_sh.result`
- :ref:`wget_minpack2_sh.example`

.. meta::
   :keywords: syntax

.. index:: syntax

.. _wget_minpack2_sh.syntax:

Syntax
******

| ``bin/wget_minpack2.sh minpack2`` / *file*

.. meta::
   :keywords: working, directory

.. index:: working, directory

.. _wget_minpack2_sh.working_directory:

Working Directory
*****************
see :ref:`working_directory<bin.working_directory>`.

.. meta::
   :keywords: file

.. index:: file

.. _wget_minpack2_sh.file:

file
****
The *file* part of the command line argument
``minpack2`` / *file* is a valid file location
relative to the Argonne National Laboratory copy of minpack2; i.e.,

| |tab| ``ftp.mcs.anl.gov/pub/MINPACK-2`` *file*

is the a valid file location.

.. meta::
   :keywords: result

.. index:: result

.. _wget_minpack2_sh.result:

Result
******
The file is copied to the location ``minpack2`` / *file*
relative to the current working directory (git root directory).

.. meta::
   :keywords: example

.. index:: example

.. _wget_minpack2_sh.example:

Example
*******
If one is in the git root directory, the command

| |tab| ``bin/wget_minpack2.sh minpack2/tprobs.92/dficfj.f``

will overwrite the git repository copy of ``dficfj.f`` .
The result file should be the same which you can check using

| |tab| ``git diff minpack2/tprobs.92/dficfj.f``

----

xsrst input file: ``bin/wget_minpack2.sh``
