// vim: set expandtab:
# include <deptfg.hpp>
# include <minpack2/tprobs.92/c/depths.h>
# include "fun_deptfg.hpp"

// ---------------------------------------------------------------------------
// constructor
fun_deptfg::fun_deptfg(size_t nx, size_t ny)
:
nx_( nx )              ,
ny_( ny )              ,
n_( nx * ny )          ,
c_( 0.1 )              ,
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
size_t fun_deptfg::size_range(void)
{   return 1; }
// ---------------------------------------------------------------------------
// start
CppAD::vector<double> fun_deptfg::start(void)
{
    // task string
    int task_len = 2;
    c_vector task(size_t(task_len + 1));
    strcpy(task.data(), "XS");
    //
    int nx      = int( nx_ );
    int ny      = int( ny_ );
    double c    = c_;
    double f;
    deptfg<double, int, ftnlen>(
        &nx,
        &ny,
        x_.data(),
        &f,
        fgrad_.data(),
        task.data(),
        &c,
        task_len
    );
    //
    return x_;
}
// ----------------------------------------------------------------------------
// double fun
void fun_deptfg::fun(d_vector& x, d_vector& fun_out)
{   assert( fun_out.size() == 1 );
    //
    // task string
    int task_len = 1;
    c_vector task(size_t(task_len + 1));
    strcpy(task.data(), "F");
    //
    int nx      = int( nx_ );
    int ny      = int( ny_ );
    double c    = c_;
    double f;
    deptfg<double, int, ftnlen>(
        &nx,
        &ny,
        x.data(),
        &f,
        fgrad_.data(),
        task.data(),
        &c,
        task_len
    );
    fun_out[0] = f;
    //
    return;
}
// ----------------------------------------------------------------------------
// adolc_double fun
void fun_deptfg::fun(adolc_vector& x, adolc_vector& fun_out)
{   assert( fun_out.size() == 1 );
    //
    // task string
    int task_len = 1;
    c_vector task(size_t(task_len + 1));
    strcpy(task.data(), "F");
    //
    int nx         = int( nx_ );
    int ny         = int( ny_ );
    adolc_double c = c_;
    adolc_double f;
    deptfg<adolc_double, int, ftnlen>(
        &nx,
        &ny,
        x.data(),
        &f,
        adolc_fgrad_.data(),
        task.data(),
        &c,
        task_len
    );
    fun_out[0] = f;
    //
    return;
}
// ----------------------------------------------------------------------------
// cppad_double fun
void fun_deptfg::fun(cppad_vector& x, cppad_vector& fun_out)
{   assert( fun_out.size() == 1 );
    //
    // task string
    int task_len = 1;
    c_vector task(size_t(task_len + 1));
    strcpy(task.data(), "F");
    //
    int nx         = int( nx_ );
    int ny         = int( ny_ );
    cppad_double c = c_;
    cppad_double f;
    deptfg<cppad_double, int, ftnlen>(
        &nx,
        &ny,
        x.data(),
        &f,
        cppad_fgrad_.data(),
        task.data(),
        &c,
        task_len
    );
    fun_out[0] = f;
    //
    return;
}
// ----------------------------------------------------------------------------
// cppadcg_double fun
void fun_deptfg::fun(cppadcg_vector& x, cppadcg_vector& fun_out)
{   assert( fun_out.size() == 1 );
    //
    // task string
    int task_len = 1;
    c_vector task(size_t(task_len + 1));
    strcpy(task.data(), "F");
    //
    int nx         = int( nx_ );
    int ny         = int( ny_ );
    cppadcg_double c;
    c = c_;
    cppadcg_double f;
    deptfg<cppadcg_double, int, ftnlen>(
        &nx,
        &ny,
        x.data(),
        &f,
        cppadcg_fgrad_.data(),
        task.data(),
        &c,
        task_len
    );
    fun_out[0] = f;
    //
    return;
}
// ----------------------------------------------------------------------------
// double grad
bool fun_deptfg::grad(d_vector& x, d_vector& grad_out)
{   assert( grad_out.size() == n_ );
    //
    // task string
    int task_len = 1;
    c_vector task(size_t(task_len + 1));
    strcpy(task.data(), "G");
    //
    int nx      = int( nx_ );
    int ny      = int( ny_ );
    double c    = c_;
    double f;
    deptfg<double, int, ftnlen>(
        &nx,
        &ny,
        x.data(),
        &f,
        grad_out.data(),
        task.data(),
        &c,
        task_len
    );
    // return true because gradient is defined for this function
    return true;
}
// ----------------------------------------------------------------------------
// double jac
bool fun_deptfg::jac(d_vector& x, d_vector& jac_out)
{   return false; }
// ----------------------------------------------------------------------------
// double hes
bool fun_deptfg::hes(d_vector& x, d_vector& hes_out)
{   assert( hes_out.size() == n_ * n_ );
    //
    int nx      = int( nx_ );
    int ny      = int( ny_ );
    //
    d_vector s(n_), y(n_);
    for(size_t j = 0; j < n_; ++j)
        s[j] = 0;
    //
    for(size_t i = 0; i < n_; ++i)
    {   // evaluate the i-th row of H
        s[i] = 1.0;
        depths_(
            &nx,
            &ny,
            s.data(),
            y.data()
        );
        for(size_t j = 0; j < n_; j++)
            hes_out[ i * n_ + j ] = y[j];
        //
        s[i] = 0.0;
    }
    // return true because Hessian is defined for this function
    return true;
}
