// This is an extra test the dficfj problem that is useful for debugging

# include <cstring>
# include <cmath>
# include <cppad/utility/vector.hpp>
# include <minpack2/tprobs.92/c/dficfj.h>
# include <dficfj.hpp>

bool dficfj_extra(void)
{   bool ok = true;
    using CppAD::vector;
    typedef int ftnlen;

    // number of sub-intervals in the k-stage collocation
    int  nint     = 3;
    int* nint_ptr = &nint;

    // number of variables
    int   n_i   = 8 * nint;
    int*  n_ptr = &n_i;
    size_t n_s  = size_t(n_i);

    // leading dimension in Jacobian matrix
    int   ldfjac_i   = n_i;
    int*  ldfjac_ptr = &ldfjac_i;
    size_t ldfjac_s  = size_t(ldfjac_i);

    // function argument vector
    vector<double> xs(n_s);
    double* xs_ptr = xs.data();

    // function result vector
    vector<double> fvec(n_s);
    double* fvec_ptr  = fvec.data();

    // Jacobian vector
    vector<double> fjac( ldfjac_s * n_s );
    double* fjac_ptr = fjac.data();

    // task string
    int task_len;
    vector<char> task(100);
    char* task_ptr = task.data();

    // Reynolds number
    double  r     = 1.0;
    double* r_ptr = &r;

    // vector used to check results
    vector<double> check;
    double* check_ptr;

    // -----------------------------------------------------------------------
    // xs = the standard starting point
    std::strcpy(task_ptr, "XS");
    task_len = 2;
    dficfj<double, int, ftnlen>(
        n_ptr,
        xs_ptr,
        fvec_ptr,
        fjac_ptr,
        ldfjac_ptr,
        task_ptr,
        r_ptr,
        nint_ptr,
        task_len
    );
    // check result using C version of routine
    check.resize(n_s);
    check_ptr = check.data();
    dficfj_(
        n_ptr,
        check_ptr,
        fvec_ptr,
        fjac_ptr,
        ldfjac_ptr,
        task_ptr,
        r_ptr,
        nint_ptr,
        task_len
    );
    for(size_t j = 0; j < n_s; ++j)
        ok &= xs[j] == check[j];

    // -----------------------------------------------------------------------
    // fvec = Function at starting point
    std::strcpy(task_ptr, "F");
    task_len = 1;
    dficfj<double, int, ftnlen>(
        n_ptr,
        xs_ptr,
        fvec_ptr,
        fjac_ptr,
        ldfjac_ptr,
        task_ptr,
        r_ptr,
        nint_ptr,
        task_len
    );
    // check result using C version of routine
    check.resize(n_s);
    check_ptr = check.data();
    dficfj_(
        n_ptr,
        xs_ptr,
        check_ptr,
        fjac_ptr,
        ldfjac_ptr,
        task_ptr,
        r_ptr,
        nint_ptr,
        task_len
    );
    for(size_t j = 0; j < n_s; ++j)
        ok &= fvec[j] == check[j];

    // -----------------------------------------------------------------------
    // fjac = Jacobian at starting point
    std::strcpy(task_ptr, "J");
    task_len = 1;
    dficfj<double, int, ftnlen>(
        n_ptr,
        xs_ptr,
        fvec_ptr,
        fjac_ptr,
        ldfjac_ptr,
        task_ptr,
        r_ptr,
        nint_ptr,
        task_len
    );
    // check result using C version of routine
    check.resize(ldfjac_s * n_s);
    check_ptr = check.data();
    dficfj_(
        n_ptr,
        xs_ptr,
        fvec_ptr,
        check_ptr,
        ldfjac_ptr,
        task_ptr,
        r_ptr,
        nint_ptr,
        task_len
    );
    for(size_t i = 0; i < n_s; ++i)
    {   for(size_t j = 0; j < n_s; ++j)
            ok &= fjac[ i + j * ldfjac_s ] == check[ i + j * ldfjac_s ];
    }

    //
    return ok;
}
