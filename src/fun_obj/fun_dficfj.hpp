# ifndef SRC_FUN_OBJ_FUN_DFICFJ_HPP
# define SRC_FUN_OBJ_FUN_DFICFJ_HPP

# include "fun_base.hpp"

// declare dficfj function interface
class fun_dficfj : public fun_base {

private:
    // number of sub-intervals in the k-stage collocation
    const size_t n_int_;
    // number of variables
    const size_t n_;
    // leading dimension in Jacobian matrix
    const size_t ldfjac_;
    // Reynolds number
    const double r_;
    // arugment vector
    d_vector x_;
    adolc_vector adolc_x_;
    cppad_vector cppad_x_;
    cppadcg_vector cppadcg_x_;
    // function value vector
    d_vector fvec_;
    adolc_vector adolc_fvec_;
    cppad_vector cppad_fvec_;
    cppadcg_vector cppadcg_fvec_;
    // Jacobian vector
    d_vector fjac_;
    adolc_vector adolc_fjac_;
    cppad_vector cppad_fjac_;
    cppadcg_vector cppadcg_fjac_;

public:
    // constructor
    fun_dficfj(size_t n_int);

    // get starting point
    virtual d_vector start(void);

    // size of the range space
    virtual size_t size_range(void);

    // evaluate function
    virtual void fun(d_vector&     x, d_vector&     fun_out);
    virtual void fun(adolc_vector& x, adolc_vector& fun_out);
    virtual void fun(cppad_vector& x, cppad_vector& fun_out);
    virtual void fun(cppadcg_vector& x, cppadcg_vector& fun_out);

    // evaluate Jacobian
    virtual bool jac(d_vector& x, d_vector& jac_out);

    // evaluate gradient
    virtual bool grad(d_vector& x, d_vector& grad_out);

    // evaluate Hessian
    virtual bool hes(d_vector& x, d_vector& hes_out);
};

# endif
