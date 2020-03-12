// :vim set expandtab:
# ifndef SRC_UTILITY_ADOLC2SPARSE_RCV_HPP
# define SRC_UTILITY_ADOLC2SPARSE_RCV_HPP

# include <cppad/utility/sparse_rcv.hpp>
# include <src/typedef.hpp>

CppAD::sparse_rcv<s_vector, d_vector> adolc2sparse_rcv(
    int                                    nr            ,
    int                                    nc            ,
    int                                    nnz           ,
    const unsigned int*                    rind          ,
    const unsigned int*                    cind          ,
    const double*                          values
);

# endif
