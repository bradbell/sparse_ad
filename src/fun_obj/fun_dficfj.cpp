// vim: set expandtab:
# include <dficfj.hpp>
# include "fun_dficfj.hpp"

// ----------------------------------------------------------------------------
// constructor
fun_dficfj::fun_dficfj(size_t n_int)
:
n_int_( n_int )        ,
n_( 8 * n_int )        ,
ldfjac_( n_ )          ,
r_( 1.0 )              ,
x_( n_ )               ,
adolc_x_( n_ )         ,
cppad_x_( n_ )         ,
cppadcg_x_( n_ )       ,
fvec_( n_ )            ,
adolc_fvec_( n_ )      ,
cppad_fvec_( n_ )      ,
cppadcg_fvec_( n_ )    ,
fjac_( n_ )            ,
adolc_fjac_( n_ )      ,
cppad_fjac_( n_ )      ,
cppadcg_fjac_( n_ )
{ }
// ----------------------------------------------------------------------------
// range
size_t fun_dficfj::size_range(void)
{   return n_; }
// ----------------------------------------------------------------------------
// start
CppAD::vector<double> fun_dficfj::start(void)
{
    // task string
    int task_len = 2;
    c_vector task(size_t(task_len + 1));
    strcpy(task.data(), "XS");
    //
    int n      = int( n_ );
    int ldfjac = int( ldfjac_ );
    int n_int  = int( n_int_ );
    double r   = r_;
    dficfj<double, int, ftnlen>(
        &n,
        x_.data(),
        fvec_.data(),
        fjac_.data(),
        &ldfjac,
        task.data(),
        &r,
        &n_int,
        task_len
    );
    //
    return x_;
}
// ----------------------------------------------------------------------------
// double fun
void fun_dficfj::fun(d_vector& x, d_vector& fun_out)
{   assert( fun_out.size() == n_ );
    //
    // task string
    int task_len = 1;
    c_vector task(size_t(task_len + 1));
    strcpy(task.data(), "F");
    //
    int n      = int( n_ );
    int ldfjac = int( ldfjac_ );
    int n_int  = int( n_int_ );
    double r   = r_;
    dficfj<double, int, ftnlen>(
        &n,
        x.data(),
        fun_out.data(),
        fjac_.data(),
        &ldfjac,
        task.data(),
        &r,
        &n_int,
        task_len
    );
    return;
}
// ----------------------------------------------------------------------------
// adolc_double fun
void fun_dficfj::fun(adolc_vector& x, adolc_vector& fun_out)
{   assert( fun_out.size() == n_ );
    //
    // task string
    int task_len = 1;
    c_vector task(size_t(task_len + 1));
    strcpy(task.data(), "F");
    //
    int n            = int( n_ );
    int ldfjac       = int( ldfjac_ );
    int n_int        = int( n_int_ );
    adolc_double r   = r_;
    dficfj<adolc_double, int, ftnlen>(
        &n,
        x.data(),
        fun_out.data(),
        adolc_fjac_.data(),
        &ldfjac,
        task.data(),
        &r,
        &n_int,
        task_len
    );
    return;
}
// ----------------------------------------------------------------------------
// cppad_double fun
void fun_dficfj::fun(cppad_vector& x, cppad_vector& fun_out)
{   assert( fun_out.size() == n_ );
    //
    // task string
    int task_len = 1;
    c_vector task(size_t(task_len + 1));
    strcpy(task.data(), "F");
    //
    int n            = int( n_ );
    int ldfjac       = int( ldfjac_ );
    int n_int        = int( n_int_ );
    cppad_double r   = r_;
    dficfj<cppad_double, int, ftnlen>(
        &n,
        x.data(),
        fun_out.data(),
        cppad_fjac_.data(),
        &ldfjac,
        task.data(),
        &r,
        &n_int,
        task_len
    );
    return;
}
// ----------------------------------------------------------------------------
// cppadcg_double fun
void fun_dficfj::fun(cppadcg_vector& x, cppadcg_vector& fun_out)
{   assert( fun_out.size() == n_ );
    //
    // task string
    int task_len = 1;
    c_vector task(size_t(task_len + 1));
    strcpy(task.data(), "F");
    //
    int n            = int( n_ );
    int ldfjac       = int( ldfjac_ );
    int n_int        = int( n_int_ );
    cppadcg_double r;
    r                = r_;
    if( CppAD::IdenticalZero(r) )
        std::cout << "r is identically zero";
    dficfj<cppadcg_double, int, ftnlen>(
        &n,
        x.data(),
        fun_out.data(),
        cppadcg_fjac_.data(),
        &ldfjac,
        task.data(),
        &r,
        &n_int,
        task_len
    );
    return;
}
// ----------------------------------------------------------------------------
// double jac
bool fun_dficfj::jac(d_vector& x, d_vector& jac_out)
{   assert( jac_out.size() == n_* n_ );
    //
    // task string
    int task_len = 1;
    c_vector task(size_t(task_len + 1));
    strcpy(task.data(), "J");
    //
    int n      = int( n_ );
    int ldfjac = int( ldfjac_ );
    int n_int  = int( n_int_ );
    double r   = r_;
    dficfj<double, int, ftnlen>(
        &n,
        x.data(),
        fvec_.data(),
        jac_out.data(),
        &ldfjac,
        task.data(),
        &r,
        &n_int,
        task_len
    );
    // return true because Jacobian is defined for this function
    return true;
}
// ----------------------------------------------------------------------------
// double grad
bool fun_dficfj::grad(d_vector& x, d_vector& grad_out)
{   return false; }
// ----------------------------------------------------------------------------
// double hes
bool fun_dficfj::hes(d_vector& x, d_vector& hes_out)
{   return false; }
