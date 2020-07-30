// vim: set expandtab:

/*
{xsrst_begin upper_triangle}

.. include:: ../preamble.rst

{xsrst_spell
    cpp
}

Extract Upper Triangle From A CppAD Sparsity Pattern
####################################################

Syntax
******
*upper* =  ``upper_triangle`` ( *pattern* )

Prototype
*********
{xsrst_file
    // BEGIN_PROTOTYPE
    // END_PROTOTYPE
}

pattern
*******
The a CppAD sparsity pattern.

upper
*****
Is the CppAD sparsity pattern for the upper triangle of the
matrix; i.e., only elements with index ( *i* , *j* ) such that
*i* <= *j* are included in *upper* .

{xsrst_end upper_triangle}
*/

# include "upper_triangle.hpp"

// BEGIN_PROTOTYPE
CppAD::sparse_rc<s_vector> upper_triangle(
    const CppAD::sparse_rc<s_vector>& pattern )
// END_PROTOTYPE
{   const size_t    nr  = pattern.nr();
    const size_t    nc  = pattern.nc();
    const s_vector& row = pattern.row();
    const s_vector& col = pattern.col();
    //
    // nnz
    size_t nnz = 0;
    for(size_t k = 0; k < pattern.nnz(); ++k)
        if( row[k] <= col[k] )
            ++nnz;
    //
    // upper
    CppAD::sparse_rc<s_vector> upper(nr, nc, nnz);
    size_t ell = 0;
    for(size_t k = 0; k < pattern.nnz(); ++k)
        if( row[k] <= col[k] )
            upper.set(ell++, row[k], col[k]);
    //
    return upper;
}
