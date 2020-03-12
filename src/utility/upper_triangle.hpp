// vim: set expandtab:
# ifndef SRC_UTILITY_UPPER_TRIANGLE_HPP
# define SRC_UTILITY_UPPER_TRIANGLE_HPP

# include <cppad/utility/sparse_rc.hpp>
# include <src/typedef.hpp>

CppAD::sparse_rc<s_vector> upper_triangle(
    const CppAD::sparse_rc<s_vector>& pattern
);

# endif
