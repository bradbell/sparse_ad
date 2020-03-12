// vim: set expandtab:

# include <src/fun_obj/fun_dgl1fg.hpp>
# include "fun_base_ok.hpp"

bool dgl1fg_ok(void)
{   bool ok = true;
    //
    // number of sub-intervals in the k-stage collocation
    size_t  n = 20;
    //
    // function object for this test case
    fun_dgl1fg fun_obj(n);
    //
    ok &= f2cpp_ok(fun_obj);
    ok &= adolc_ok(fun_obj);
    ok &= cppad_ok(fun_obj);
    ok &= subgraph_ok(fun_obj);
    ok &= cppadcg_ok(fun_obj);
    //
    return ok;
}
