// vim: set expandtab:
# include <dgl1fg.hpp>
# include <minpack2/tprobs.92/c/dgl1hs.h>
# include "fun_dgl1fg.hpp"

// ---------------------------------------------------------------------------
// constructor
fun_dgl1fg::fun_dgl1fg(size_t n)
:
n_( n )                ,
t_( 5.0 )              ,
x_( n_ )               ,
adolc_x_( n_ )         ,
cppad_x_( n_ )         ,
cppadcg_x_( n_ )       ,
fgrad_( n_ )           ,
adolc_fgrad_( n_ )     ,
cppad_fgrad_( n_ )     ,
cppadcg_fgrad_( n_ )
{ }
// ---------------------------------------------------------------------------
// range
size_t fun_dgl1fg::size_range(void)
{   return 1; }
// ---------------------------------------------------------------------------
// start
CppAD::vector<double> fun_dgl1fg::start(void)
{
    // task string
    int task_len = 2;
    c_vector task(size_t(task_len + 1));
    strcpy(task.data(), "XS");
    //
    int n       = int( n_ );
    double t    = t_;
    double f;
    dgl1fg<double, int, ftnlen>(
        &n,
        x_.data(),
        &f,
        fgrad_.data(),
        task.data(),
        &t,
        task_len
    );
    //
    return x_;
}
// ----------------------------------------------------------------------------
// double fun
void fun_dgl1fg::fun(d_vector& x, d_vector& fun_out)
{   assert( fun_out.size() == 1 );
    //
    // task string
    int task_len = 1;
    c_vector task(size_t(task_len + 1));
    strcpy(task.data(), "F");
    //
    int n      = int( n_ );
    double t   = t_;
    double f;
    dgl1fg<double, int, ftnlen>(
        &n,
        x.data(),
        &f,
        fgrad_.data(),
        task.data(),
        &t,
        task_len
    );
    fun_out[0] = f;
    //
    return;
}
// ----------------------------------------------------------------------------
// adolc_double fun
void fun_dgl1fg::fun(adolc_vector& x, adolc_vector& fun_out)
{   assert( fun_out.size() == 1 );
    //
    // task string
    int task_len = 1;
    c_vector task(size_t(task_len + 1));
    strcpy(task.data(), "F");
    //
    int n          = int( n_ );
    adolc_double t = t_;
    adolc_double f;
    dgl1fg<adolc_double, int, ftnlen>(
        &n,
        x.data(),
        &f,
        adolc_fgrad_.data(),
        task.data(),
        &t,
        task_len
    );
    fun_out[0] = f;
    //
    return;
}
// ----------------------------------------------------------------------------
// cppad_double fun
void fun_dgl1fg::fun(cppad_vector& x, cppad_vector& fun_out)
{   assert( fun_out.size() == 1 );
    //
    // task string
    int task_len = 1;
    c_vector task(size_t(task_len + 1));
    strcpy(task.data(), "F");
    //
    int n          = int( n_ );
    cppad_double t = t_;
    cppad_double f;
    dgl1fg<cppad_double, int, ftnlen>(
        &n,
        x.data(),
        &f,
        cppad_fgrad_.data(),
        task.data(),
        &t,
        task_len
    );
    fun_out[0] = f;
    //
    return;
}
// ----------------------------------------------------------------------------
// cppadcg_double fun
void fun_dgl1fg::fun(cppadcg_vector& x, cppadcg_vector& fun_out)
{   assert( fun_out.size() == 1 );
    //
    // task string
    int task_len = 1;
    c_vector task(size_t(task_len + 1));
    strcpy(task.data(), "F");
    //
    int n          = int( n_ );
    cppadcg_double t;
    t = t_;
    cppadcg_double f;
    dgl1fg<cppadcg_double, int, ftnlen>(
        &n,
        x.data(),
        &f,
        cppadcg_fgrad_.data(),
        task.data(),
        &t,
        task_len
    );
    fun_out[0] = f;
    //
    return;
}
// ----------------------------------------------------------------------------
// double grad
bool fun_dgl1fg::grad(d_vector& x, d_vector& grad_out)
{   assert( grad_out.size() == n_ );
    //
    // task string
    int task_len = 1;
    c_vector task(size_t(task_len + 1));
    strcpy(task.data(), "G");
    //
    int n      = int( n_ );
    double t   = t_;
    double f;
    dgl1fg<double, int, ftnlen>(
        &n,
        x.data(),
        &f,
        grad_out.data(),
        task.data(),
        &t,
        task_len
    );
    // return true because gradient is defined for this function
    return true;
}
// ----------------------------------------------------------------------------
// double jac
bool fun_dgl1fg::jac(d_vector& x, d_vector& jac_out)
{   return false; }
// ----------------------------------------------------------------------------
// double hes
bool fun_dgl1fg::hes(d_vector& x, d_vector& hes_out)
{   assert( hes_out.size() == n_ * n_ );
    //
    int n       = int( n_ );
    double t    = t_;
    //
    d_vector s(n_), y(n_);
    for(size_t j = 0; j < n_; ++j)
        s[j] = 0;
    //
    for(size_t i = 0; i < n_; ++i)
    {   // evaluate the i-th row of H
        s[i] = 1.0;
        dgl1hs_(
            &n,
            x.data(),
            s.data(),
            y.data(),
            &t
        );
        for(size_t j = 0; j < n_; j++)
            hes_out[ i * n_ + j ] = y[j];
        //
        s[i] = 0.0;
    }
    // return true because Hessian is defined for this function
    return true;
}
