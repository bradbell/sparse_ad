// vim: set expandtab:

# include <cstring>
# include <cmath>
# include <cppad/speed/uniform_01.hpp>
# include <src/fun_obj/fun_base.hpp>
# include <src/utility/check_sparse.hpp>

bool subgraph_ok(fun_base& fun_obj)
{   bool ok = true;
    //
    // the standard starting point
    d_vector xs = fun_obj.start();
    //
    // number of independent and dependent variables
    size_t n = xs.size();
    size_t m = fun_obj.size_range();
    //
    // perturb the starting point with random noise
    d_vector noise(n);
    CppAD::uniform_01(n, noise);
    for(size_t j = 0; j < n; ++j)
        xs[j] = xs[j] + (noise[j] - 0.5) / 10.0;
    //
    // function argument and result in adouble
    cppad_vector ax(n), afvec(m);
    for(size_t j = 0; j < n; ++j)
        ax[j] = xs[j];
    //
    // Declare ax = x as the independent variables
    CppAD::Independent(ax);
    //
    // afvec = f(x)
    fun_obj.fun(ax, afvec);
    //
    // Declare fvec as the dependent variables
    CppAD::ADFun<double> f(ax, afvec);
    //
    if( m == 1 )
    {
        // create gp = g'(x) where g(x) = sum_i w_i f_i (x)
        CppAD::ADFun<cppad_double, double> af = f.base2ad();
        CppAD::Independent(ax);
        cppad_vector aw(1);
        aw[0] = 1.0;
        af.Forward(0, ax);
        cppad_vector agrad = af.Reverse(1, aw);
        CppAD::ADFun<double> gp(ax, agrad);
        //
        // compute Hessian of f as Jacobian of g using sub-graph method
        b_vector select_domain(n), select_range(n);
        CppAD::sparse_rcv<s_vector, d_vector> sparse_hes;
        for(size_t j = 0; j < n; ++j)
        {   select_domain[j]  = true;
            select_range[j]   = true;
        }
        gp.subgraph_jac_rev(select_domain, select_range, xs, sparse_hes);
        //
        // check Hessian
        d_vector dense_hes( n * n );
        bool have_hes = fun_obj.hes(xs, dense_hes);
        ok &= have_hes;
        std::string print_label = "cppad did not expect";
        bool print_done;
        ok &= check_sparse(sparse_hes, dense_hes, print_label, print_done);
    }
    else
    {   ok &= m > 1;
        //
        // compute Jacobian using sub-graph method
        b_vector select_domain(n), select_range(m);
        CppAD::sparse_rcv<s_vector, d_vector> sparse_jac;
        for(size_t j = 0; j < n; ++j)
            select_domain[j]  = true;
        for(size_t i = 0; i < m; ++i)
            select_range[i]  = true;
        f.subgraph_jac_rev(select_domain, select_range, xs, sparse_jac);
        //
        // check Jacobian
        d_vector dense_jac(n * n);
        bool have_jac = fun_obj.jac(xs, dense_jac);
        ok &= have_jac;
        std::string print_label = "subgraph did not expect";
        bool print_done;
        ok &= check_sparse(sparse_jac, dense_jac, print_label, print_done);
    }
    return ok;
}
