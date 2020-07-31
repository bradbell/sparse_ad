# include <cstring>
# include <cmath>
# include <cppad/cg/cppadcg.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <src/fun_obj/fun_base.hpp>
# include <src/utility/check_sparse.hpp>
# include <src/utility/cppadcg_library.hpp>

bool cppadcg_ok(fun_base& fun_obj)
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
    // function argument x and result fvec
    cg_vector cg_x(n);
    cppadcg_vector acg_x(n), acg_fvec(m);
    for(size_t j = 0; j < n; ++j)
        acg_x[j] = cg_x[j] = xs[j];
    //
    // Declare acg_x = x as the independent variables
    CppAD::Independent(acg_x);
    //
    // acg_fvec = f(x)
    fun_obj.fun(acg_x, acg_fvec);
    //
    // cg_double version of f(x)
    CppAD::ADFun<cg_double> cg_f(acg_x, acg_fvec);
    //
    // cppadcg_double version of f(x)
    CppAD::ADFun<cppadcg_double, cg_double> acg_f;
    acg_f = cg_f.base2ad();
    //
    if( m == 1 )
    {   // Hessian sparisty pattern
        b_vector select_domain(n), select_range(1);
        for(size_t j = 0; j < n; ++j)
            select_domain[j] = true;
        select_range[0] = true;
        bool internal_bool = false;
        CppAD::sparse_rc<s_vector> pattern_hes;
        cg_f.for_hes_sparsity(
            select_domain, select_range, internal_bool, pattern_hes
        );
        //
        // sparse Hessian
        CppAD::sparse_rcv<s_vector, cppadcg_vector> acg_sparse_hes(pattern_hes);
        //
        // record computation of sparse Hessian H(x) = f''(x)
        std::string coloring = "colpack.symmetric";
        CppAD::sparse_hes_work work;
        cppadcg_vector acg_w(1);
        acg_w[0] = 1.0;
        CppAD::Independent(acg_x);
        acg_f.sparse_hes(
            acg_x, acg_w, acg_sparse_hes, pattern_hes, coloring, work
        );
        CppAD::ADFun<cg_double> cg_H(acg_x, acg_sparse_hes.val());
        //
        // create a object library corresponding to H(x)
        cppadcg_library sparse_hes_val(cg_H, "sparse_hes_val");
        //
        // compute the values in the sparse Hessian using the library
        d_vector val = sparse_hes_val(xs);
        //
        // create a sparse Matrix corresponding to the values
        CppAD::sparse_rcv<s_vector, d_vector> sparse_hes(pattern_hes);
        for(size_t k = 0; k < sparse_hes.nnz(); ++k)
            sparse_hes.set(k, val[k]);
        //
        // check the sparse Hessian computed using the library
        d_vector dense_hes( n * n );
        bool have_hes = fun_obj.hes(xs, dense_hes);
        ok &= have_hes;
        std::string print_label = "cppadcg did not expect";
        bool print_done;
        ok &= check_sparse(sparse_hes, dense_hes, print_label, print_done);
    }
    else
    {
        //
        // Jacobian sparsity pattern
        CppAD::sparse_rc<s_vector> pattern_in, pattern_jac;
        pattern_in.resize(n, n, n);
        for(size_t k = 0; k < n; ++k)
            pattern_in.set(k, k, k);
        bool transpose     = false;
        bool dependency    = false;
        bool internal_bool = false;
        cg_f.for_jac_sparsity(
            pattern_in, transpose, dependency, internal_bool, pattern_jac
        );
        //
        // sparse Jacobain
        CppAD::sparse_rcv<s_vector, cppadcg_vector> acg_sparse_jac(pattern_jac);
        //
        // record computation of sparse Jacobian J(x) = f'(x)
        const std::string coloring = "colpack";
        CppAD::sparse_jac_work work;
        CppAD::Independent(acg_x);
        acg_f.sparse_jac_rev(
            acg_x, acg_sparse_jac, pattern_jac, coloring, work
        );
        CppAD::ADFun<cg_double> cg_J(acg_x, acg_sparse_jac.val());
        //
        // create library corresponding to J(x)
        cppadcg_library sparse_jac_val(cg_J, "sparse_jac_val");
        //
        // compute the values in the sparse Jacobian using the library
        d_vector val = sparse_jac_val(xs);
        //
        // create a sparse Matrix corresponding to he values
        CppAD::sparse_rcv<s_vector, d_vector> sparse_jac(pattern_jac);
        for(size_t k = 0; k < sparse_jac.nnz(); ++k)
            sparse_jac.set(k, val[k]);
        //
        // check the sparse Jacobian computed using the library
        d_vector dense_jac(m * n);
        bool have_jac = fun_obj.jac(xs, dense_jac);
        ok &= have_jac;
        std::string print_label = "cppadcg did not expect";
        bool print_done;
        ok &= check_sparse(sparse_jac, dense_jac, print_label, print_done);
    }
    //
    return ok;
}
