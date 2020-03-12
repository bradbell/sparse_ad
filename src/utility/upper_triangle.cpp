// vim: set expandtab:

/*
$begin upper_triangle$$
$spell
    CppAD
$$

$section Extract Upper Triangle From A CppAD Sparsity Pattern$$

$head Syntax$$
$icode%upper% = upper_triangle(%pattern%)%$$

$head Prototype$$
$srcthisfile%
    0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1
%$$

$head pattern$$
The a CppAD sparsity pattern.

$head upper$$
Is the CppAD sparsity pattern for the upper triangle of the
matrix; i.e., only elements with index $codei%(%i%, %j%)%$$ such that
$icode%i% <= %j%$$ are included in $icode upper$$.

$end
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
