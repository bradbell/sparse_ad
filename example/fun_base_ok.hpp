# ifndef EXAMPLE_FUN_BASE_OK_HPP
# define EXAMPLE_FUN_BASE_OK_HPP

# include <src/fun_obj/fun_base.hpp>

extern bool f2cpp_ok(fun_base& fun_obj);
extern bool adolc_ok(fun_base& fun_obj);
extern bool cppad_ok(fun_base& fun_obj);
extern bool subgraph_ok(fun_base& fun_obj);
extern bool cppadcg_ok(fun_base& fun_obj);

# endif
