.. include:: ../preamble.rst

Table of Contents
*****************
:ref:`sparse_ad`

|
| :ref:`1<speed>` Run a Jacobian or Hessian Speed Test`
|
|  |space| :ref:`1.1<speed_global>` Speed Program Global Variables`
|  |space| :ref:`1.2<speed_setup>` Setup a Speed Test Problem`
|  |space| :ref:`1.3<speed_test>` Setup a Speed Test Problem`
|  |space| :ref:`1.4<speed_write_csv>` Writes One Row of the Speed Csv Output File`
|
| :ref:`2<bin>` Executable Scripts`
|
|  |space| :ref:`2.1<get_all_sh>` Local install of Packages Required by sparse_ad`
|  |space| :ref:`2.2<wget_minpack2_sh>` Get a Minpack2 File From Argonne National Laboratory`
|  |space| :ref:`2.3<f2c_sh>` Create a C Version of a Fortran File`
|  |space| :ref:`2.4<c2hpp_py>` Create a C++ Template Version of an f2c File`
|  |space| :ref:`2.5<csv2tex_py>` Extract Latex Tables From Speed Test Csv File`
|  |space| :ref:`2.6<run_speed_sh>` Run All the Jacobian and Hessian Speed Tests`
|
| :ref:`3<typedef_hpp>` Type Definitions`
| :ref:`4<fun_base_hpp>` Base Class For Test Function Objects`
| :ref:`5<utility>` Utility Routines`
|
|  |space| :ref:`5.1<check_sparse>` Compare A CppAD Sparse Matrix and a Dense Matrix`
|  |space| :ref:`5.2<adolc2sparse_rcv>` Convert An Adolc Sparse Matrix to CppAD Form`
|  |space| :ref:`5.3<prt_sparse>` Print A CppAD Sparsity Pattern or Sparse Matrix`
|  |space| :ref:`5.4<upper_triangle>` Extract Upper Triangle From A CppAD Sparsity Pattern`
|  |space| :ref:`5.5<cppadcg_library>` CppADCodeGen Dynamic Libraries`
|
| :ref:`6<release_notes>` Changes and Additions to sparse_ad`
|

Link to Index
*************
* :ref:`genindex`
