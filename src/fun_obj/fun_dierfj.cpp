// vim: set expandtab:
# include <dierfj.hpp>
# include "fun_dierfj.hpp"

// ----------------------------------------------------------------------------
// constructor
fun_dierfj::fun_dierfj(size_t n_int)
:
n_int_( n_int )        ,
n_( 15 * n_int + 3 )   ,
ldfjac_( n_ )          ,
a_( 1.0 )              ,
b_( 1.0 )              ,
c_( 1.0 )              ,
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
size_t fun_dierfj::size_range(void)
{   return n_; }
// ----------------------------------------------------------------------------
// start
CppAD::vector<double> fun_dierfj::start(void)
{
    // task string
    int task_len = 2;
    c_vector task(size_t(task_len + 1));
    strcpy(task.data(), "XS");
    //
    int n      = int( n_ );
    int ldfjac = int( ldfjac_ );
    int n_int  = int( n_int_ );
    double a   = a_;
    double b   = b_;
    double c   = c_;
    dierfj<double, int, ftnlen>(
        &n,
        x_.data(),
        fvec_.data(),
        fjac_.data(),
        &ldfjac,
        task.data(),
        &a,
        &b,
        &c,
        &n_int,
        task_len
    );
    //
    return x_;
}
// ----------------------------------------------------------------------------
// double fun
void fun_dierfj::fun(d_vector& x, d_vector& fun_out)
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
    double a   = a_;
    double b   = b_;
    double c   = c_;
    dierfj<double, int, ftnlen>(
        &n,
        x.data(),
        fun_out.data(),
        fjac_.data(),
        &ldfjac,
        task.data(),
        &a,
        &b,
        &c,
        &n_int,
        task_len
    );
    return;
}
// ----------------------------------------------------------------------------
// adolc_double fun
void fun_dierfj::fun(adolc_vector& x, adolc_vector& fun_out)
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
    adolc_double a   = a_;
    adolc_double b   = b_;
    adolc_double c   = c_;
    dierfj<adolc_double, int, ftnlen>(
        &n,
        x.data(),
        fun_out.data(),
        adolc_fjac_.data(),
        &ldfjac,
        task.data(),
        &a,
        &b,
        &c,
        &n_int,
        task_len
    );
    return;
}
// ----------------------------------------------------------------------------
// cppad_double fun
void fun_dierfj::fun(cppad_vector& x, cppad_vector& fun_out)
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
    cppad_double a   = a_;
    cppad_double b   = b_;
    cppad_double c   = c_;
    dierfj<cppad_double, int, ftnlen>(
        &n,
        x.data(),
        fun_out.data(),
        cppad_fjac_.data(),
        &ldfjac,
        task.data(),
        &a,
        &b,
        &c,
        &n_int,
        task_len
    );
    return;
}
// ----------------------------------------------------------------------------
// cppadcg_double fun
void fun_dierfj::fun(cppadcg_vector& x, cppadcg_vector& fun_out)
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
    cppadcg_double a, b, c;
    a = a_;
    b = b_;
    c = c_;
    dierfj<cppadcg_double, int, ftnlen>(
        &n,
        x.data(),
        fun_out.data(),
        cppadcg_fjac_.data(),
        &ldfjac,
        task.data(),
        &a,
        &b,
        &c,
        &n_int,
        task_len
    );
    return;
}
// ----------------------------------------------------------------------------
// double jac
bool fun_dierfj::jac(d_vector& x, d_vector& jac_out)
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
    double a   = a_;
    double b   = b_;
    double c   = c_;
    dierfj<double, int, ftnlen>(
        &n,
        x.data(),
        fvec_.data(),
        jac_out.data(),
        &ldfjac,
        task.data(),
        &a,
        &b,
        &c,
        &n_int,
        task_len
    );
    // return true because Jacobian is defined for this function
    return true;
}
// ----------------------------------------------------------------------------
// double grad
bool fun_dierfj::grad(d_vector& x, d_vector& grad_out)
{   return false; }
// ----------------------------------------------------------------------------
// double hes
bool fun_dierfj::hes(d_vector& x, d_vector& hes)
{   return false; }
