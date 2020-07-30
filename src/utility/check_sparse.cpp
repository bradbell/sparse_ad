// vim: set expandtab:
# include <limits>
# include "check_sparse.hpp"

/*
{xsrst_begin check_sparse}

.. include:: ../preamble.rst

{xsrst_spell
    cpp
    nr
    nc
}

Compare A CppAD Sparse Matrix and a Dense Matrix
################################################

Syntax
******

| *ok* =  ``check_sparse`` (
| |tab| *sparse_matrix* , *dense_matrix* , *print_label* , *print_done*
| )

Prototype
*********
{xsrst_file
    // BEGIN_PROTOTYPE
    // END_PROTOTYPE
}

sparse_matrix
*************
A CppAD sparse representation of the matrix.

dense_matrix
************
A dense representation of the matrix as a vector of with size
*nr* * *nc* and in column major order; i.e.,
the value of the ( *i* , *j* ) element of the matrix is
*dense_matrix* [ *i* + * *nr* * *j* ] .

print_label
***********
A message is printed if *print_label* is non-empty
and one of the possibly non-zero values in the sparse matrix
has value zero in *dense_matrix* .
The message printed is

| |tab| ``:`` *print_label* ``matrix`` ( *i* , *j* ) == 0.0 ``:``

Note this is still correct if the corresponding sparse matrix value
is also zero.

print_done
**********
The return value for this argument is true (false)
if the message discussed above is (is not) printed.

ok
**
The return value *ok* is true if the following two conditions hold:

#.  All of the zero values in *dense_matrix*
    correspond to zeros in the sparse matrix.
#.  The relative error for all of the non-zero values in
    *dense_matrix* is less than 200 time machine epsilon for double.

{xsrst_end check_sparse}
*/

// BEGIN_PROTOTYPE
bool check_sparse(
    const CppAD::sparse_rcv<s_vector, d_vector>& sparse_matrix ,
    const d_vector&                              dense_matrix  ,
    const std::string&                           print_label   ,
    bool&                                        print_done    )
// END_PROTOTYPE
{   bool ok     = true;
    print_done  = false;
    //
    double eps200 = 200.0 * std::numeric_limits<double>::epsilon();
    const size_t nr          = sparse_matrix.nr();
    const size_t nc          = sparse_matrix.nc();
    const size_t nnz         = sparse_matrix.nnz();
    const s_vector& row      = sparse_matrix.row();
    const s_vector& col      = sparse_matrix.col();
    const d_vector& val      = sparse_matrix.val();
    const s_vector row_major = sparse_matrix.row_major();
    //
    size_t k = 0;
    size_t r = nr;
    size_t c = nc;
    double v = 0.0;
    if( k < nnz )
    {   r = row[ row_major[k] ];
        c = col[ row_major[k] ];
        v = val[ row_major[k] ];
    }
    for(size_t i = 0; i < nr; ++i)
    {   for(size_t j = 0; j < nc; ++j)
        {   double d = dense_matrix[i +  nr * j];
            if( i == r && j == c )
            {   if( d == 0.0 )
                {   if( ! print_done && print_label != "" )
                    {   std::cout << ":" << print_label << " matrix("
                        << i << "," << j << ") == 0.0:";
                        print_done = true;
                    }
                    ok &= v == 0.0;
                }
                else
                {   ok &= std::fabs(1.0 - v / d) < eps200;
                }
                ++k;
                if( k < nnz )
                {   r = row[ row_major[k] ];
                    c = col[ row_major[k] ];
                    v = val[ row_major[k] ];
                }
            }
            else
            {   ok &= d == 0.0;
            }
        }
    }
    ok &= k == nnz;
    return ok;
}
