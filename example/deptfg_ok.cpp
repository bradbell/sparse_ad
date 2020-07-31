# include <src/fun_obj/fun_deptfg.hpp>
# include "fun_base_ok.hpp"

bool deptfg_ok(void)
{   bool ok = true;
    //
    // number of sub-intervals in the k-stage collocation
    size_t  nx = 4;
    size_t  ny = 4;
    //
    // function object for this test case
    fun_deptfg fun_obj(nx, ny);
    //
    ok &= f2cpp_ok(fun_obj);
    ok &= adolc_ok(fun_obj);
    ok &= cppad_ok(fun_obj);
    ok &= subgraph_ok(fun_obj);
    ok &= cppadcg_ok(fun_obj);
    //
    return ok;
}
