# include <src/utility/check_sparse.hpp>
# include <src/utility/upper_triangle.hpp>
# include <src/fun_obj/fun_base.hpp>
# include <src/typedef.hpp>

# include "global.hpp"

namespace {
    // subgraph function object for this problem
    CppAD::ADFun<double>  fun_;
    // gradient object for this problem
    CppAD::ADFun<double>  grad_;
    // subgraph sparsity results
    CppAD::sparse_rcv<s_vector, d_vector> subset_;
    // warning message has been printed
    bool printed_ = false;
}
// -----------------------------------------------------------------------
// setup_subgraph
extern void test_subgraph_jac(size_t repeat);
extern void test_subgraph_hes(size_t repeat);
void setup_subgraph(void)
{
    size_t n   = global_x.size();
    size_t m   = global_problem_ptr->size_range();
    //
    // fun_
    cppad_vector ax(n), afvec(m);
    for(size_t j = 0; j < n; ++j)
        ax[j] = global_x[j];
    CppAD::Independent(ax);
    global_problem_ptr->fun(ax, afvec);
    fun_.Dependent(ax, afvec);
    if( global_optimize )
    {   fun_.optimize("no_cumulative_sum_op collision_limit=500");
        if( fun_.exceed_collision_limit() )
        {   std::cerr << "subgraph: fun_: collision limit execeeded\n";
            std::exit(1);
        }
    }
    //
    bool transpose = false;
    if( m == 1 )
    {   // Computing Hessian
        //
        // grad(x) =  f'(x)
        CppAD::ADFun<cppad_double, double> af = fun_.base2ad();
        cppad_vector aw(1);
        aw[0] = 1.0;
        for(size_t j = 0; j < n; ++j)
            ax[j] = global_x[j];
        CppAD::Independent(ax);
        af.Forward(0, ax);
        cppad_vector agrad = af.Reverse(1, aw);
        grad_.Dependent(ax, agrad);
        if( global_optimize )
        {   grad_.optimize("no_cumulative_sum_op collision_limit=500");
            if( grad_.exceed_collision_limit() )
            {   std::cerr << "subgraph: collision limit execeeded\n";
                std::exit(1);
            }
        }
        //
        // Compute Jacobian sparsity pattern for grad'(x), which is a
        // Hessian sparsity pattern for g(x).
        b_vector select_domain(n), select_range(n);
        for(size_t j = 0; j < n; ++j)
        {   select_domain[j] = true;
            select_range[j]  = true;
        }
        CppAD::sparse_rc<s_vector> pattern_hes;
        grad_.subgraph_sparsity(
            select_domain, select_range, transpose, pattern_hes
        );
        // upper triangle version of hessian sparsity pattern
        CppAD::sparse_rc<s_vector> upper = upper_triangle(pattern_hes);
        subset_ =
            CppAD::sparse_rcv<s_vector, d_vector>( upper );

    }
    else
    {   // Jacobian sparsity pattern for f(x)
        b_vector select_domain(n), select_range(m);
        for(size_t j = 0; j < n; ++j)
            select_domain[j] = true;
        for(size_t i = 0; i < m; ++i)
            select_range[i] = true;
        CppAD::sparse_rc<s_vector> pattern_jac;
        fun_.subgraph_sparsity(
            select_domain, select_range, transpose, pattern_jac
        );
        subset_ =
            CppAD::sparse_rcv<s_vector, d_vector>( pattern_jac );
    }
}
// -----------------------------------------------------------------------
// test_subgraph_jac
void test_subgraph_jac(size_t repeat)
{
    assert( global_problem_ptr->size_range() > 1 );
    assert( ! global_colpack );
    assert( ! global_indirect );
    assert( ! global_onepass );
    assert( global_reverse );
    //
    size_t n = global_x.size();
    size_t m = global_problem_ptr->size_range();
    for(size_t rep = 0; rep < repeat; ++rep)
    {   if( global_setup )
            setup_subgraph();
        fun_.subgraph_jac_rev(global_x, subset_);
    }
    global_nnz = subset_.nnz();
    if( global_correct )
    {   // compute dense version of Jacobian
        d_vector dense_jac( m * n );
        global_problem_ptr->jac(global_x, dense_jac);
        //
        // compare to sparse version of Jacobian
        std::string print_label = "";
        bool print_done;
        if( ! printed_ )
            print_label = "subgraph did not expect";
        global_correct_ok = check_sparse(
            subset_, dense_jac, print_label, print_done
        );
        if( print_done )
            printed_ = true;
    }
}
// -----------------------------------------------------------------------
// test_subgraph_hes
void test_subgraph_hes(size_t repeat)
{
    assert( global_problem_ptr->size_range() == 1 );
    assert( global_reverse );
    assert( ! global_colpack );
    assert( ! global_indirect );
    assert( ! global_onepass );
    //
    size_t n = global_x.size();
    for(size_t rep = 0; rep < repeat; ++rep)
    {   if( global_setup )
            setup_subgraph();
       assert( grad_.size_var() > 0 );
        // evaluate the Hessian of f
        grad_.subgraph_jac_rev(global_x, subset_);
    }
    global_nnz = subset_.nnz();
    if( global_correct )
    {
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
            print_label = "subgraph did not expect";
        global_correct_ok = check_sparse(
            subset_, dense_hes, print_label, print_done
        );
        if( print_done )
            printed_ = true;
    }
}
