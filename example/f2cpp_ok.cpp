// vim: set expandtab:

# include <cstring>
# include <cmath>
# include <src/fun_obj/fun_base.hpp>

bool f2cpp_ok(fun_base& fun_obj)
{   bool ok = true;
    //
    // the standard starting point
    d_vector xs = fun_obj.start();
    //
    // number of variables
    size_t n = xs.size();
    //
    // x = xs
    d_vector x = xs;
    //
    // step size used for central difference approximation of derivative
    double step = 1e-5;
    //
    // size of range space
    const size_t m = fun_obj.size_range();
    //
    if( m == 1 )
    {   // gradient vector
        d_vector fgrad( n );
        bool have_grad = fun_obj.grad(xs, fgrad);
        ok &= have_grad;
        //
        // function result at plus and minus points
        d_vector fplus(1), fminus(1);
        //
        // loop over the components of the domain vector x
        double max_rel_err = 0.0;
        for(size_t j = 0; j < n; ++j)
        {   // fplus = f( xs + step * e_j )
            x[j] = xs[j] + step;
            fun_obj.fun(x, fplus);
            //
            // fminus = f( xs - step * e_j )
            x[j] = xs[j] - step;
            fun_obj.fun(x, fminus);
            //
            // finite difference approximation for derivative
            double value   = fgrad[j];
            double apx     = (fplus[0] - fminus[0]) / (2 * step);
            double rel_err = std::fabs( 1.0 - apx / value );
            if( value == 0.0 )
                ok &= apx == 0.0;
            else
                max_rel_err = std::max(rel_err, max_rel_err);
            //
            x[j] = xs[j];
        }
        ok &= max_rel_err <= 1e-5;
    }
    else
    {   // Jacobian vector
        ok &= m > 1;
        d_vector fjac( m * n );
        bool have_jac = fun_obj.jac(xs, fjac);
        ok &= have_jac;
        //
        // function result vector at plus and minus points
        d_vector fplus(m), fminus(m);
        //
        // loop over the components of the domain vector x
        double max_rel_err = 0.0;
        for(size_t j = 0; j < n; ++j)
        {   // fplus = f( xs + step * e_j )
            x[j] = xs[j] + step;
            fun_obj.fun(x, fplus);
            //
            // fminus = f( xs - step * e_j )
            x[j] = xs[j] - step;
            fun_obj.fun(x, fminus);
            //
            // loop over the components of the range vector f
            for(size_t i = 0; i < m; ++i)
            {   // finite difference approximation for derivative
                double value   = fjac[ i + j * m ];
                double apx     = (fplus[i] - fminus[i]) / (2 * step);
                double rel_err = std::fabs( 1.0 - apx / value );
                if( value == 0.0 )
                    ok &= apx == 0.0;
                else
                    max_rel_err = std::max(rel_err, max_rel_err);
            }
            x[j] = xs[j];
        }
        ok &= max_rel_err <= 1e-5;
    }
    //
    return ok;
}
