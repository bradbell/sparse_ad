# ifndef SRC_FUN_OBJ_FUN_DEPTFG_HPP
# define SRC_FUN_OBJ_FUN_DEPTFG_HPP

# include "fun_base.hpp"

// declare deptfg function interface
class fun_deptfg : public fun_base {

private:
    // number of grid ponts in first coordinate direction
    const size_t nx_;
    // number of grid ponts in second coordinate direction
    const size_t ny_;
    // number of variables
    const size_t n_;
    // angle of the twist per unit length
    const double c_;
    // arugment vector
    d_vector x_;
    adolc_vector adolc_x_;
    cppad_vector cppad_x_;
    cppadcg_vector cppadcg_x_;
    // gradient vector
    d_vector fgrad_;
    adolc_vector adolc_fgrad_;
    cppad_vector cppad_fgrad_;
    cppadcg_vector cppadcg_fgrad_;

public:
    // constructor
    fun_deptfg(size_t nx, size_t ny);

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
