# ifndef SRC_UTILITY_CHECK_SPARSE_HPP
# define SRC_UTILITY_CHECK_SPARSE_HPP

# include <src/typedef.hpp>

bool check_sparse(
    const CppAD::sparse_rcv<s_vector, d_vector>& sparse_matrix ,
    const d_vector&                              dense_matrix  ,
    const std::string&                           print_label   ,
    bool&                                        print_done
);

# endif
