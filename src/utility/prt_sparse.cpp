// vim: set expandtab:
# include "prt_sparse.hpp"

/*
{xsrst_begin prt_sparse}

.. include:: ../preamble.rst

{xsrst_spell
    cpp
    prt
}

Print A CppAD Sparsity Pattern or Sparse Matrix
###############################################

Syntax
******

| ``prt_sparse`` ( *name* , *pattern* )
| ``prt_sparse`` ( *name* , *matrix* )

Prototype
*********
{xsrst_file
    // BEGIN_PATTERN_PROTOTYPE
    // END_PATTERN_PROTOTYPE
}
{xsrst_file
    // BEGIN_MATRIX_PROTOTYPE
    // END_MATRIX_PROTOTYPE
}

name
****
Is the name for the matrix.

pattern
*******
Is the sparsity pattern that is printed.
In this case the row and column indices
corresponding to possibly non-zero values are printed.

matrix
******
Is the sparse matrix that is printed.
In this case the possibly non-zero values are printed
along with the corresponding row and column indices.

{xsrst_end prt_sparse}
*/

// BEGIN_PATTERN_PROTOTYPE
void prt_sparse(
    const std::string&                name    ,
    const CppAD::sparse_rc<s_vector>& pattern )
// END_PATTERN_PROTOTYPE
{   s_vector        row_major = pattern.row_major();
    const s_vector& row       = pattern.row();
    const s_vector& col       = pattern.col();
    size_t r_previous = pattern.nr();
    for(size_t k = 0; k < pattern.nnz(); ++k)
    {   size_t r = row[ row_major[k] ];
        size_t c = col[ row_major[k] ];
        if( k == 0 )
           std::cout << name << " =";
        if( r != r_previous )
            std::cout << "\nrow = " << r << " , col = " << c;
        else
            std::cout << " , " << c;
        r_previous = r;
    }
    std::cout << "\n";
}

// BEGIN_MATRIX_PROTOTYPE
void prt_sparse(
    const std::string&                           name   ,
    const CppAD::sparse_rcv<s_vector, d_vector>& matrix )
// END_MATRIX_PROTOTYPE
{   s_vector        row_major = matrix.row_major();
    const s_vector& row       = matrix.row();
    const s_vector& col       = matrix.col();
    const d_vector& val       = matrix.val();
    size_t r_previous = matrix.nr();
    for(size_t k = 0; k < matrix.nnz(); ++k)
    {   size_t r = row[ row_major[k] ];
        size_t c = col[ row_major[k] ];
        double v = val[ row_major[k] ];
        if( k == 0 )
           std::cout << name << " =";
        if( r != r_previous )
        {   std::cout << "\nrow = " << r << ": (col,val) = ";
            std::cout << "(" << c << " , " << v << ")";
        }
        else
            std::cout << ", (" << c << " , " << v << ")";
        //
        r_previous = r;
    }
    std::cout << "\n";
}
