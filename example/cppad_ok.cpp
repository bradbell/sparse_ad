// vim: set expandtab:

# include <cstring>
# include <cmath>
# include <cppad/speed/uniform_01.hpp>
# include <src/fun_obj/fun_base.hpp>
# include <src/utility/check_sparse.hpp>

bool cppad_ok(fun_base& fun_obj)
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
    {   // sparse Hessian
        b_vector select_domain(n), select_range(1);
        for(size_t j = 0; j < n; ++j)
            select_domain[j] = true;
        select_range[0] = true;
        bool internal_bool = false;
        CppAD::sparse_rc<s_vector> pattern_hes;
        f.for_hes_sparsity(
            select_domain, select_range, internal_bool, pattern_hes
        );
        CppAD::sparse_rcv<s_vector, d_vector> sparse_hes( pattern_hes );
        std::string coloring = "colpack.symmetric";
        CppAD::sparse_hes_work work;
        d_vector w(1);
        w[0] = 1.0;
        f.sparse_hes(
            xs, w, sparse_hes, pattern_hes, coloring, work
        );
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
    {
        //
        // compute sparsity pattern for Jacobian
        CppAD::sparse_rc<s_vector> pattern_in, pattern_jac;
        pattern_in.resize(n, n, n);
        for(size_t k = 0; k < n; ++k)
            pattern_in.set(k, k, k);
        bool transpose     = false;
        bool dependency    = false;
        bool internal_bool = false;
        f.for_jac_sparsity(
            pattern_in, transpose, dependency, internal_bool, pattern_jac
        );
        //
        // evaluate the Jacobian of f
        CppAD::sparse_rcv<s_vector, d_vector> sparse_jac( pattern_jac );
        const std::string coloring = "colpack";
        CppAD::sparse_jac_work work;
        //
        f.sparse_jac_rev(xs, sparse_jac, pattern_jac, coloring, work);
        //
        // check Jacobian
        d_vector dense_jac(m * n);
        bool have_jac = fun_obj.jac(xs, dense_jac);
        ok &= have_jac;
        std::string print_label = "cppad did not expect";
        bool print_done;
        ok &= check_sparse(sparse_jac, dense_jac, print_label, print_done);
    }
    //
    return ok;
}
