# ifndef SRC_SPEED_GLOBAL_HPP
# define SRC_SPEED_GLOBAL_HPP

# include <src/typedef.hpp>

extern fun_base* global_problem_ptr;
extern size_t    global_size;
extern bool      global_colpack;
extern bool      global_indirect;
extern bool      global_optimize;
extern bool      global_setup;
extern bool      global_reverse;
extern bool      global_onepass;
extern bool      global_correct;
extern d_vector  global_x;

extern size_t    global_nnz;
extern bool      global_correct_ok;

# endif
