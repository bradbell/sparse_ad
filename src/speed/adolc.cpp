// vim: set expandtab:
# include <adolc/adolc.h>
# include <adolc/adolc_sparse.h>
# include <src/utility/adolc2sparse_rcv.hpp>
# include <src/fun_obj/fun_base.hpp>
# include <src/utility/check_sparse.hpp>

# include "global.hpp"

namespace {
    //
    // adolc function object for this problme
    short                 tag_;
    // warning message has been printed
    bool                  printed_ = false;
    // adolc sparisty and compression results
    unsigned int          *rind_   = nullptr;
    unsigned int          *cind_   = nullptr;
    double                *values_ = nullptr;
    int                    repeat_ = 0;
    int                    nnz_    = 0;
}
// -----------------------------------------------------------------------
static void eval_jac(void)
{
    int n  = int(global_x.size());
    int m  = int( global_problem_ptr->size_range() );
    // index sparsity, safe mode, (not used), column compression
    int options[4] = {0, 0, 0, 0};
    if( global_reverse )
    {   // row compression
        options[3] = 1;
    }
# ifndef NDEBUG
    int flag =
# endif
    sparse_jac(
        tag_,
        m,
        n,
        repeat_,
        global_x.data(),
        &nnz_,
        &rind_,
        &cind_,
        &values_,
        options
    );
    assert( rind_   != nullptr );
    assert( cind_   != nullptr );
    assert( values_ != nullptr );
    repeat_    = 1;
    global_nnz = size_t(nnz_);
    assert( flag >= 0 );
}
// -----------------------------------------------------------------------
static void eval_hes(void)
{
    int n  = int(global_x.size());
    // safe mode, indirect recovery
    int options[2] = {0, 0};
    if( ! global_indirect )
    {   // direct recovery
        options[1] = 1;
    }
# ifndef NDEBUG
    int flag =
# endif
    sparse_hess(
        tag_,
        n,
        repeat_,
        global_x.data(),
        &nnz_,
        &rind_,
        &cind_,
        &values_,
        options
    );
    assert( rind_   != nullptr );
    assert( cind_   != nullptr );
    assert( values_ != nullptr );
    repeat_    = 1;
    global_nnz = size_t(nnz_);
    assert( flag >= 0 );
}

// -----------------------------------------------------------------------
// setup_adolc
void setup_adolc(void)
{   assert( rind_  == nullptr );
    assert( cind_  == nullptr );
    assert( values_ == nullptr );
    assert( repeat_ == 0 );
    assert( nnz_    == 0 );
    //
    size_t n   = global_x.size();
    size_t m   = global_problem_ptr->size_range();
    tag_ = 0;
    trace_on(tag_);
    //
    adolc_vector ax(n), afvec(m);
    for(size_t j = 0; j < n; ++j)
        ax[j] <<= global_x[j];
    global_problem_ptr->fun(ax, afvec);
    d_vector fvec(m);
    for(size_t i = 0; i < m; ++i)
        afvec[i] >>= fvec[i];
    trace_off();
    //
    // compute sparsity pattern, compression, and derivative
    if( m == 1 )
        eval_hes();
    else
        eval_jac();
}
// -----------------------------------------------------------------------
// test_adolc_jac
void test_adolc_jac(size_t repeat)
{
    assert( global_problem_ptr->size_range() > 1 );
    assert( global_colpack );
    assert( ! global_indirect );
    assert( ! global_optimize );
    assert( global_onepass );
    //
    int n  = int(global_x.size());
    int m  = int( global_problem_ptr->size_range() );
    for(size_t rep = 0; rep < repeat; ++rep)
    {   if( global_setup )
        {  free(rind_);
            free(cind_);
            free(values_);
            rind_   = nullptr;
            cind_   = nullptr;
            values_ = nullptr;
            //
            repeat_ = 0;
            nnz_    = 0;
            setup_adolc();
        }
        else
            eval_jac();
        if( global_correct & (rep == repeat-1) )
        {   // compute dense version of Jacobian
            d_vector dense_jac( size_t(m * n) );
            global_problem_ptr->jac(global_x, dense_jac);
            //
            // compare to sparse version of Jacobian
            CppAD::sparse_rcv<s_vector, d_vector>
            sparse_jac = adolc2sparse_rcv(
                m, n, nnz_, rind_, cind_, values_
            );
            std::string print_label = "";
            bool print_done;
            if( ! printed_ )
                print_label = "adolc did not expect";
            global_correct_ok = check_sparse(
                sparse_jac, dense_jac, print_label, print_done
            );
            if( print_done )
                printed_ = true;
        }
    }
}
// -----------------------------------------------------------------------
// test_adolc_hes
void test_adolc_hes(size_t repeat)
{
    assert( global_problem_ptr->size_range() == 1 );
    assert( global_colpack );
    assert( ! global_optimize );
    assert( ! global_reverse  );
    assert( ! global_onepass );
    //
    int n  = int(global_x.size());
    for(size_t rep = 0; rep < repeat; ++rep)
    {   if( global_setup )
        {   free(rind_);
            free(cind_);
            free(values_);
            rind_   = nullptr;
            cind_   = nullptr;
            values_ = nullptr;
            repeat_ = 0;
            nnz_    = 0;
            setup_adolc();
        }
        else
            eval_hes();
        if( global_correct & (rep == repeat-1) )
        {   // compute dense version of upper triangle of Hessian
            d_vector dense_hes( size_t(n * n) );
            global_problem_ptr->hes(global_x, dense_hes);
            for(int i = 0; i < n; ++i)
            {   for(int j = 0; j < i; ++j)
                    dense_hes[ i + n * j] = 0.0;
            }
            //
            // compare to sparse version of Hessian
            CppAD::sparse_rcv<s_vector, d_vector>
            sparse_hes = adolc2sparse_rcv(
                n, n, nnz_, rind_, cind_, values_
            );
            std::string print_label = "";
            bool print_done;
            if( ! printed_ )
                print_label = "adolc did not expect";
            global_correct_ok = check_sparse(
                sparse_hes, dense_hes, print_label, print_done
            );
            if( print_done )
                printed_ = true;
        }
    }
}
