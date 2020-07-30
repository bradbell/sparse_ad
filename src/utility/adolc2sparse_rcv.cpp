// vim: set expandtab:

/*
{xsrst_begin adolc2sparse_rcv}

.. include:: ../preamble.rst

{xsrst_spell
    cpp
    rcv
    nr
    nc
    nnz
    cind
}

Convert An Adolc Sparse Matrix to CppAD Form
############################################

Syntax
******

| *sparse_matrix* =  ``adolc2sparse_rcv`` (
| |tab| *nr* , *nc* , *nnz* , *rind* , *cind* , *values*
| )

Prototype
*********
{xsrst_file
    // BEGIN_PROTOTYPE
    // END_PROTOTYPE
}

nr
**
number of rows in the matrix.

nc
**
number of columns in the matrix.

nnz
***
number of possibly non-zero entries in the matrix.

rind
****
The row indices for possibly non-zero values.

cind
****
The column indices for possibly non-zero values.

values
******
The possibly non-zero values.

sparse_matrix
*************
The cppad sparse representation for the matrix.

{xsrst_end adolc2sparse_rcv}
*/

# include <cppad/utility/vector.hpp>
# include "adolc2sparse_rcv.hpp"

// BEGIN_PROTOTYPE
CppAD::sparse_rcv<s_vector, d_vector> adolc2sparse_rcv(
    int                                    nr            ,
    int                                    nc            ,
    int                                    nnz           ,
    const unsigned int*                    rind          ,
    const unsigned int*                    cind          ,
    const double *                         values        )
// END_PROTOTYPE
{   size_t nr_size  = size_t(nr);
    size_t nc_size  = size_t(nc);
    size_t nnz_size = size_t(nnz);
    // pattern
    CppAD::sparse_rc<s_vector> pattern(nr_size, nc_size, nnz_size);
    for(size_t k = 0; k < nnz_size; ++k)
        pattern.set(k, size_t(rind[k]), size_t(cind[k]));
    // result
    CppAD::sparse_rcv<s_vector, d_vector> result( pattern );
    for(size_t k = 0; k < nnz_size; ++k)
        result.set(k, values[k]);
    //
    return result;
}
