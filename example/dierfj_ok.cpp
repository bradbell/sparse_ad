// vim: set expandtab:

# include <src/fun_obj/fun_dierfj.hpp>
# include "fun_base_ok.hpp"

bool dierfj_ok(void)
{   bool ok = true;
    //
    // number of sub-intervals in the k-stage collocation
    size_t  n_int = 3;
    //
    // function object for this test case
    fun_dierfj fun_obj(n_int);
    //
    ok &= f2cpp_ok(fun_obj);
    ok &= adolc_ok(fun_obj);
    ok &= cppad_ok(fun_obj);
    ok &= subgraph_ok(fun_obj);
    ok &= cppadcg_ok(fun_obj);
    //
    return ok;
}
