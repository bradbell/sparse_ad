// vim: set expandtab:
# include <src/typedef.hpp>
# include <src/fun_obj/fun_base.hpp>
# include <src/utility/check_sparse.hpp>
# include <src/utility/upper_triangle.hpp>

# include "global.hpp"

namespace {
    //
    // cppad function object for this problem
    CppAD::ADFun<double>                  fun_;
    // warning message has been printed
    bool                                  printed_ = false;
    // cppad sparsity and compression results
    CppAD::sparse_rcv<s_vector, d_vector> subset_;
    CppAD::sparse_jac_work                jac_work_;
    CppAD::sparse_hes_work                hes_work_;
    bool                                  work_empty_ = true;
}
// -----------------------------------------------------------------------
static void eval_jac(void)
{
    //
    size_t n = global_x.size();
    assert( global_problem_ptr->size_range() > 1 );
    //
    std::string coloring = "cppad";
    if( global_colpack )
        coloring = "colpack";
    //
    size_t group_max  = 1;
    if( global_onepass )
        group_max = 100;
    //
    CppAD::sparse_rc<s_vector> pattern_in, pattern_jac;
    if( work_empty_ )
    {   // calculate the sparsity pattern and compression
        pattern_in.resize(n, n, n);
        for(size_t k = 0; k < n; ++k)
            pattern_in.set(k, k, k);
        bool transpose     = false;
        bool dependency    = false;
        bool internal_bool = false;
        // Reverse mode sparsity should be better when m < n
        fun_.for_jac_sparsity(
            pattern_in,
            transpose,
            dependency,
            internal_bool,
            pattern_jac
        );
        subset_ =
            CppAD::sparse_rcv<s_vector, d_vector>( pattern_jac );
        //
        global_nnz = subset_.nnz();
    }
    //
    // evaluate the Jacobian of f
    if( global_reverse ) fun_.sparse_jac_rev(
        global_x, subset_, pattern_jac, coloring, jac_work_
    );
    else
    {
# ifndef NDEBUG
       size_t n_color =
# endif
        fun_.sparse_jac_for(
            group_max,
            global_x,
            subset_,
            pattern_jac,
            coloring,
            jac_work_
        );
        assert( ! global_onepass || n_color <= group_max );
    }
    work_empty_ = false;
}
// -----------------------------------------------------------------------
static void eval_hes(void)
{
    assert( global_problem_ptr->size_range() == 1 );
    assert( ! global_indirect );
    assert( ! global_onepass );
    //
    size_t n = global_x.size();
    std::string coloring = "cppad.symmetric";
    if( global_colpack )
        coloring = "colpack.symmetric";
    //
    CppAD::sparse_rc<s_vector> pattern_hes;
    if( work_empty_ )
    {   // calculate sparsity pattern
        b_vector select_domain(n), select_range(1);
        for(size_t j = 0; j < n; ++j)
            select_domain[j] = true;
        select_range[0] = true;
        bool internal_bool = false;
        if( global_reverse )
        {   bool transpose  = false;
            bool dependency = false;
            CppAD::sparse_rc<s_vector> pat_in(n, n, n), pat_out;
            for(size_t k = 0; k < n; ++k)
                pat_in.set(k, k, k);
            fun_.for_jac_sparsity(
                pat_in, transpose, dependency, internal_bool, pat_out
            );
            fun_.rev_hes_sparsity(
                select_range, transpose, internal_bool, pattern_hes
            );
            assert( fun_.size_forward_set() > 0 );
            fun_.size_forward_set(0);
            assert( fun_.size_forward_set() == 0 );
        }
        else
        {   fun_.for_hes_sparsity(
            select_domain, select_range, internal_bool, pattern_hes
            );
        }
        // upper triangle version of hessian sparsity pattern
        CppAD::sparse_rc<s_vector> upper = upper_triangle(pattern_hes);
        subset_ =
            CppAD::sparse_rcv<s_vector, d_vector>( upper );
        //
        // set global_nnz
        global_nnz = subset_.nnz();
    }
    // evaluate the Hessian of f
    d_vector w(1);
    w[0] = 1.0;
    fun_.sparse_hes(
        global_x, w, subset_, pattern_hes, coloring, hes_work_
    );
    work_empty_ = false;
}
// -----------------------------------------------------------------------
// setup_cppad
extern void test_cppad_jac(size_t repeat);
extern void test_cppad_hes(size_t repeat);
void setup_cppad(void)
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
    {   fun_.optimize("no_cumulative_sum_op collision_limit=100");
        if( fun_.exceed_collision_limit() )
        {   std::cerr << "cppad: fun_: collision limit execeeded\n";
            std::exit(1);
        }
    }
    //
    if( m == 1 )
        eval_hes();
    else
        eval_jac();
    //
    assert( ! work_empty_ );
}
// -----------------------------------------------------------------------
// test_cppad_jac
void test_cppad_jac(size_t repeat)
{
    assert( global_problem_ptr->size_range() > 1 );
    assert( ! global_indirect );
    assert( ! (global_onepass && global_reverse) );
    //
    size_t n = global_x.size();
    size_t m = global_problem_ptr->size_range();
    //
    for(size_t rep = 0; rep < repeat; ++rep)
    {   if( global_setup )
            setup_cppad();
        else
            eval_jac();
        //
        if( global_setup )
        {   jac_work_.clear();
            work_empty_ = true;
        }
    }
    if( global_correct )
    {   // compute dense version of Jacobian
        d_vector dense_jac( m * n );
        global_problem_ptr->jac(global_x, dense_jac);
        //
        // compare to sparse version of Jacobian
        std::string print_label = "";
        bool print_done;
        if( ! printed_ )
            print_label = "cppad did not expect";
        global_correct_ok = check_sparse(
            subset_, dense_jac, print_label, print_done
        );
        if( print_done )
            printed_ = true;
    }
}
// -----------------------------------------------------------------------
// test_cppad_hes
void test_cppad_hes(size_t repeat)
{
    assert( global_problem_ptr->size_range() == 1 );
    assert( ! global_indirect );
    assert( ! global_onepass );
    //
    size_t n = global_x.size();
    std::string coloring = "cppad.symmetric";
    if( global_colpack )
        coloring = "colpack.symmetric";
    //
    for(size_t rep = 0; rep < repeat; ++rep)
    {   if( global_setup )
            setup_cppad();
        else
            eval_hes();
        //
        if( global_setup )
        {   hes_work_.clear();
            work_empty_ = true;
        }
    }
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
            print_label = "cppad did not expect";
        global_correct_ok = check_sparse(
            subset_, dense_hes, print_label, print_done
        );
        if( print_done )
            printed_ = true;
    }
}
