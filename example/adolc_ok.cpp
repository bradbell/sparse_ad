// vim: set expandtab:

# include <cstring>
# include <cmath>
# include <adolc/adolc.h>
# include <adolc/adolc_sparse.h>
# include <cppad/speed/uniform_01.hpp>
# include <src/fun_obj/fun_base.hpp>
# include <src/utility/adolc2sparse_rcv.hpp>
# include <src/utility/check_sparse.hpp>

bool adolc_ok(fun_base& fun_obj)
{   bool ok = true;
    //
    // the standard starting point
    d_vector xs = fun_obj.start();
    //
    // number of independent and dependent variables
    size_t n = xs.size();
    size_t m = fun_obj.size_range();
    //
    // perturb the starting point with random noise
    d_vector noise(n);
    CppAD::uniform_01(n, noise);
    for(size_t j = 0; j < n; ++j)
        xs[j] = xs[j] + (noise[j] - 0.5) / 10.0;
    //
    // function argument and result in adouble
    adolc_vector ax(n), afvec(m);
    //
    // trace_on
    short tag  = 0; // tape identifier
    trace_on(tag);
    //
    // Declare ax = x as the independent variables
    for(size_t j = 0; j < n; ++j)
        ax[j] <<= xs[j];
    //
    // afvec = f(x)
    fun_obj.fun(ax, afvec);
    //
    // Declare fvec as the dependent variables
    d_vector fvec(n);
    for(size_t i = 0; i < m; ++i)
        afvec[i] >>= fvec[i];
    //
    // trace_off
    trace_off();
    //
    if( m == 1 )
    {   // Evaluate Hessian of f using ADOLC sparse_hess
        int repeat = 0, nnz;
        unsigned int *rind = nullptr, *cind = nullptr;
        double *values = nullptr;
        //
        int options[2];
        options[0] = 0; // safe mode (default)
        // options[0] = 1; tight mode
        options[1] = 0; // indirect recovery (default)
        // options[1] = 1; direct recovery
        //
        sparse_hess(
            tag,       // the tape corresponding to the function f
            int(n),    // number of dependent variables
            repeat,    // indicates repeated calls with same sparsity pattern
            xs.data(), // independent variables where evaluating Jacobian
            &nnz,      // # non-zeros in sparsity pattern (non-repeat output)
            &rind,     // row indices (non-repeat output)
            &cind,     // column indices (non-repeat output)
            &values,   // value of non-zeros in Hessian (output)
            options    // control options
        );
        //
        // compute dense version of upper triangle of Hessian
        d_vector dense_hes( n * n );
        bool have_hes = fun_obj.hes(xs, dense_hes);
        ok &= have_hes;
        for(size_t i = 0; i < n; ++i)
        {   for(size_t j = 0; j < i; ++j)
                dense_hes[i + n * j] = 0.0;
        }
        //
        // compare to sparse version of Hessian
        CppAD::sparse_rcv<s_vector, d_vector> sparse_hes = adolc2sparse_rcv(
            int(n), int(n), nnz, rind, cind, values
        );
        std::string print_label = "adolc did not expect";
        bool print_done;
        ok &= check_sparse(sparse_hes, dense_hes, print_label, print_done);
    }
    else
    {   ok &= m > 1;
        //
        // Evaluate the Jacobian of f using ADOLC sparse_jac
        // (row major order)
        int repeat = 0, nnz;
        unsigned int *rind = nullptr, *cind = nullptr;
        double *values = nullptr;
        //
        int options[4];
        // method for computing sparsity patterns
        options[0] = 0; // propagation of index domains
        // options[0] = 1; propagation of bit patterns
        //
        // test the computational graph control flow
        options[1] = 0; // safe mode (default)
        // options[1] = 1; tight mode
        //
        // choice of forward and reverse mode
        options[2] = 0; // automatic detection (default)
        // options[2] = 1; forward mode
        // options[2] = 2; reverse mode
        //
        // type of compression
        // options[3] = 0; column compression (default)
        options[3] = 1; // row compression
        //
        sparse_jac(
            tag,       // the tape corresponding to the function f
            int(m),    // number of dependent variables
            int(n),    // number of independent variables
            repeat,    // indicates repeated calls with same sparsity pattern
            xs.data(), // independent variables where evaluating Jacobian
            &nnz,      // # non-zeros in sparsity pattern (non-repeat output)
            &rind,     // row indices (non-repeat output)
            &cind,     // column indices (non-repeat output)
            &values,   // value of non-zeros in Jacobian (output)
            options    // control options
        );
        // check Jacobian
        d_vector dense_jac(m * n);
        bool have_jac = fun_obj.jac(xs, dense_jac);
        ok &= have_jac;
        CppAD::sparse_rcv<s_vector, d_vector> sparse_jac = adolc2sparse_rcv(
            int(n), int(n), nnz, rind, cind, values
        );
        std::string print_label = "adolc did not expect";
        bool print_done;
        ok &= check_sparse(sparse_jac, dense_jac, print_label, print_done);
        //
        free(rind);
        free(cind);
        free(values);
    }
    return ok;
}
