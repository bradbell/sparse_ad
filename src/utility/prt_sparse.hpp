# ifndef SRC_UTILITY_PRT_SPARSE_HPP
# define SRC_UTILITY_PRT_SPARSE_HPP

# include <cppad/utility/sparse_rcv.hpp>
# include <src/typedef.hpp>

extern void prt_sparse(
    const std::string&                name     ,
    const CppAD::sparse_rc<s_vector>& pattern
);

extern void prt_sparse(
    const std::string&                           name     ,
    const CppAD::sparse_rcv<s_vector, d_vector>& matrix
);

# endif
