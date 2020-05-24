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
// setup_cppadcg
void setup_cppadcg(void)
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
    if( global_optimize )
    {   cg_f.optimize("collision_limit=30");
        if( cg_f.exceed_collision_limit() )
        {   std::cerr << "cppadcg: collision limit execeeded\n";
            std::exit(1);
        }
    }
    //
    // acg_f
    CppAD::ADFun<cppadcg_double, cg_double> acg_f;
    acg_f = cg_f.base2ad();
    //
    if( m == 1 )
    {   // options that are Hessian specific
        std::string coloring = "cppad.symmetric";
        if( global_colpack )
            coloring = "colpack.symmetric";
        //
        // pattern_hes
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
        // upper triangle of Hessian
        CppAD::sparse_rc<s_vector> upper = upper_triangle(pattern_hes);
        //
        // acg_sparse_hes
        CppAD::sparse_rcv<s_vector, cppadcg_vector>
            acg_sparse_hes(upper);
        //
        // record computation of sparse Hessian H(x) = f''(x)
        CppAD::sparse_hes_work work;
        cppadcg_vector acg_w(1);
        acg_w[0] = 1.0;
        CppAD::Independent(acg_x);
        acg_f.sparse_hes(
            acg_x, acg_w, acg_sparse_hes, pattern_hes, coloring, work
        );
        CppAD::ADFun<cg_double> cg_H;
        cg_H.Dependent(acg_x, acg_sparse_hes.val());
        if( global_optimize )
        {   cg_H.optimize("collision_limit=30");
            if( cg_H.exceed_collision_limit() )
            {   std::cerr << "cppadcg: collision limit execeeded\n";
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
    {   // options that are Jacobian specific
        std::string coloring = "cppad";
        if( global_colpack )
            coloring = "colpack";
        size_t group_max = 1;
        if( global_onepass )
            group_max = 100;
        //
        // pattern_jac
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
        // acg_sparse_jac
        CppAD::sparse_rcv<s_vector, cppadcg_vector>
            acg_sparse_jac(pattern_jac);
        //
        // record computation of sparse Jacobian J(x) = f'(x)
        CppAD::sparse_jac_work work;
        CppAD::Independent(acg_x);
        if( global_reverse )
        {   acg_f.sparse_jac_rev(
                acg_x, acg_sparse_jac, pattern_jac, coloring, work
            );
        }
        else
        {
# ifndef NDEBUG
            size_t n_color =
# endif
            acg_f.sparse_jac_for(
                group_max,
                acg_x,
                acg_sparse_jac,
                pattern_jac,
                coloring,
                work
            );
            assert( ! global_onepass || n_color < group_max );
        }
        CppAD::ADFun<cg_double> cg_J;
        cg_J.Dependent(acg_x, acg_sparse_jac.val());
        if( global_optimize )
        {   cg_J.optimize("collision_limit=30");
            if( cg_J.exceed_collision_limit() )
            {   std::cerr << "cppadcg: collision limit execeeded\n";
                std::exit(1);
            }
        }
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
// test_cppadcg_jac
void test_cppadcg_jac(size_t repeat)
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
            setup_cppadcg();
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
            print_label = "cppadcg did not expect";
        global_correct_ok = check_sparse(
            subset_, dense_jac, print_label, print_done
        );
        if( print_done )
            printed_ = true;
    }
}
// -----------------------------------------------------------------------
// test_cppadcg_hes
void test_cppadcg_hes(size_t repeat)
{
    assert( global_problem_ptr->size_range() == 1 );
    assert( ! global_indirect );
    assert( ! global_onepass  );
    //
    size_t n = global_x.size();
    d_vector sparse_hes_val( subset_.nnz() );
    for(size_t rep = 0; rep < repeat; ++rep)
    {   if( global_setup )
            setup_cppadcg();
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
            print_label = "cppadcg did not expect";
        global_correct_ok = check_sparse(
            subset_, dense_hes, print_label, print_done
        );
        if( print_done )
            printed_ = true;
    }
}
