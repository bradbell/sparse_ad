// vim: set expandtab:
# include <cppad/cg/cppadcg.hpp>
# include <src/typedef.hpp>
# include <src/fun_obj/fun_base.hpp>
# include <src/utility/check_sparse.hpp>
# include <src/utility/cppadcg_library.hpp>
# include <src/utility/upper_triangle.hpp>

# include "global.hpp"

namespace {
    // cppadcg library object for this problem
    cppadcg_library                       lib_;
    // sparse matrix representation of result
    CppAD::sparse_rcv<s_vector, d_vector> subset_;
    // warning message has been printed
    bool                                  printed_ = false;
}
// -----------------------------------------------------------------------
// setup_subcg
void setup_subcg(void)
{
    size_t n   = global_x.size();
    size_t m   = global_problem_ptr->size_range();
    //
    // function argument x and result fvec
    cg_vector      cg_x(n);
    cppadcg_vector acg_x(n), acg_fvec(m);
    for(size_t j = 0; j < n; ++j)
        acg_x[j] = cg_x[j] = global_x[j];
    //
    // cg_f
    CppAD::Independent(acg_x);
    global_problem_ptr->fun(acg_x, acg_fvec);
    CppAD::ADFun<cg_double> cg_f;
    cg_f.Dependent(acg_x, acg_fvec);
    //
    // acg_f
    CppAD::ADFun<cppadcg_double, cg_double> acg_f;
    acg_f = cg_f.base2ad();
    //
    bool transpose = false;
    if( m == 1 )
    {   // grad(x) = f'(x)
        cppadcg_vector acg_w(1);
        acg_w[0] = 1.0;
        CppAD::Independent(acg_x);
        acg_f.Forward(0, acg_x);
        cppadcg_vector acg_g = acg_f.Reverse(1, acg_w);
        CppAD::ADFun<cg_double> cg_grad;
        cg_grad.Dependent(acg_x, acg_g);
        if( global_optimize )
        {   cg_grad.optimize("no_cumulative_sum_op collision_limit=100");
            if( cg_grad.exceed_collision_limit() )
            {   std::cerr << "subcg: cg_grad: collision limit execeeded\n";
                std::exit(1);
            }
        }
        CppAD::ADFun<cppadcg_double, cg_double> acg_grad;
        acg_grad = cg_grad.base2ad();
        //
        // Compute Jacobian sparsity pattern for grad'(x), which is a
        // Hessian sparsity pattern
        b_vector select_domain(n), select_range(n);
        for(size_t j = 0; j < n; ++j)
        {   select_domain[j] = true;
            select_range[j]  = true;
        }
        CppAD::sparse_rc<s_vector> pattern_hes;
        cg_grad.subgraph_sparsity(
            select_domain, select_range, transpose, pattern_hes
        );
        // upper triangle of Hessian
        CppAD::sparse_rc<s_vector> upper = upper_triangle(pattern_hes);
        //
        // acg_sparse_hes
        CppAD::sparse_rcv<s_vector, cppadcg_vector>
            acg_sparse_hes(upper);
        //
        // record computation of sparse Hessian H(x) = f''(x)
        CppAD::Independent(acg_x);
        acg_grad.subgraph_jac_rev(acg_x, acg_sparse_hes);
        CppAD::ADFun<cg_double> cg_H;
        cg_H.Dependent(acg_x, acg_sparse_hes.val());
        if( global_optimize )
        {   cg_H.optimize("no_cumulative_sum_op collision_limit=100");
            if( cg_H.exceed_collision_limit() )
            {   std::cerr << "subcg: cg_H: collision limit execeeded\n";
                std::exit(1);
            }
        }
        //
        // lib_
        cppadcg_library sparse_hes_val(cg_H, "sparse_hes_val");
        lib_.swap( sparse_hes_val );
        //
        // subset_
        subset_ = CppAD::sparse_rcv<s_vector, d_vector>(upper);
    }
    else
    {   // Jacobian sparsity pattern for f(x)
        b_vector select_domain(n), select_range(m);
        for(size_t j = 0; j < n; ++j)
            select_domain[j] = true;
        for(size_t i = 0; i < m; ++i)
            select_range[i] = true;
        CppAD::sparse_rc<s_vector> pattern_jac;
        cg_f.subgraph_sparsity(
            select_domain, select_range, transpose, pattern_jac
        );
        //
        // acg_sparse_jac
        CppAD::sparse_rcv<s_vector, cppadcg_vector>
            acg_sparse_jac(pattern_jac);
        //
        // record computation of sparse Jacobian J(x) = f'(x)
        CppAD::Independent(acg_x);
        acg_f.subgraph_jac_rev(acg_x, acg_sparse_jac);
        CppAD::ADFun<cg_double> cg_J;
        cg_J.Dependent(acg_x, acg_sparse_jac.val());
        //
        // lib_
        cppadcg_library sparse_jac_val(cg_J, "sparse_jac_val");
        lib_.swap( sparse_jac_val );
        //
        // subset_
        subset_ =
            CppAD::sparse_rcv<s_vector, d_vector>(pattern_jac);
    }
}
// -----------------------------------------------------------------------
// test_subcg_jac
void test_subcg_jac(size_t repeat)
{
    assert( global_problem_ptr->size_range() > 1 );
    assert( ! global_indirect );
    assert( ! (global_onepass && global_reverse) );
    //
    size_t n = global_x.size();
    size_t m = global_problem_ptr->size_range();
    d_vector sparse_jac_val( subset_.nnz() );
    for(size_t rep = 0; rep < repeat; ++rep)
    {   if( global_setup )
            setup_subcg();
        // evaluate the Jacobian of f
        sparse_jac_val = lib_(global_x);
    }
    global_nnz = subset_.nnz();
    if( global_correct )
    {   // sparse matrix version of Jacobian
        for(size_t k = 0; k < global_nnz; ++k)
            subset_.set(k, sparse_jac_val[k] );
        //
        // compute dense version of Jacobian
        d_vector dense_jac( m * n );
        global_problem_ptr->jac(global_x, dense_jac);
        //
        // compare to sparse version of Jacobian
        std::string print_label = "";
        bool print_done;
        if( ! printed_ )
            print_label = "subcg did not expect";
        global_correct_ok = check_sparse(
            subset_, dense_jac, print_label, print_done
        );
        if( print_done )
            printed_ = true;
    }
}
// -----------------------------------------------------------------------
// test_subcg_hes
void test_subcg_hes(size_t repeat)
{
    assert( global_problem_ptr->size_range() == 1 );
    assert( ! global_indirect );
    assert( ! global_onepass  );
    //
    size_t n = global_x.size();
    d_vector sparse_hes_val( subset_.nnz() );
    for(size_t rep = 0; rep < repeat; ++rep)
    {   if( global_setup )
            setup_subcg();
        // evaluate the Hessian of f
        sparse_hes_val = lib_(global_x);
    }
    global_nnz = subset_.nnz();
    if( global_correct )
    {   // sparse matrix version of Hessian
        for(size_t k = 0; k < global_nnz; ++k)
            subset_.set(k, sparse_hes_val[k] );
        //
        // compute dense version of upper triangle of Hessian
        d_vector dense_hes( n * n );
        global_problem_ptr->hes(global_x, dense_hes);
        for(size_t i = 0; i < n; ++i)
        {   for(size_t j = 0; j < i; ++j)
                dense_hes[ i + n * j] = 0.0;
        }
        //
        // compare to sparse version of Hessian
        std::string print_label = "";
        bool print_done;
        if( ! printed_ )
            print_label = "subcg did not expect";
        global_correct_ok = check_sparse(
            subset_, dense_hes, print_label, print_done
        );
        if( print_done )
            printed_ = true;
    }
}
