// vim: set expandtab:
# ifndef SRC_TYPEDEF_HPP
# define SRC_TYPEDEF_HPP

# include <adolc/adolc.h>
# include <cppad/cg/cppadcg.hpp>

/*
$begin typedef.hpp$$
$spell
    ftnlen
    adolc
    cppad
    subgraph
    CppAD
    src
    typedef.hpp
    cppadcg
    cg
$$

$section Type Definitions$$

$head Syntax$$
$codei%# include <src/typedef.hpp>
%$$

$head Scalar Types$$

$subhead ftnlen$$
type used by f2c for fortran integers:
$srccode%hpp% */
typedef int                          ftnlen;
/* %$$

$subhead adolc_double$$
type used by adolc method to record operations:
$srccode%hpp% */
typedef adouble                      adolc_double;
/* %$$

$subhead cppad_double$$
type used by cppad and subgraph methods to record operations:
$srccode%hpp% */
typedef CppAD::AD<double>            cppad_double;
/* %$$

$subhead cg_double$$
type used by cppadcg methods to evaluate derivatives
$srccode%hpp% */
typedef CppAD::cg::CG<double>        cg_double;
/* %$$

$subhead cppadcg_double$$
type used by cppadcg methods to record operations:
$srccode%hpp% */
typedef CppAD::AD<cg_double>         cppadcg_double;
/* %$$

$head Vector Types$$
$codei%CppAD::vector<%T%>%$$ is a vector with elements of type $icode T$$ and
$codei%CppAD::vector<%T%>::data()%$$ is the corresponding $icode%T%*%$$
pointer.

$subhead d_vector$$
$srccode%hpp% */
typedef CppAD::vector<double>          d_vector;
/* %$$

$subhead c_vector$$
$srccode%hpp% */
typedef CppAD::vector<char>            c_vector;
/* %$$

$subhead s_vector$$
$srccode%hpp% */
typedef CppAD::vector<size_t>          s_vector;
/* %$$

$subhead b_vector$$
$srccode%hpp% */
typedef CppAD::vector<bool>            b_vector;
/* %$$

$subhead adolc_vector$$
$srccode%hpp% */
typedef CppAD::vector<adolc_double>    adolc_vector;
/* %$$

$subhead cppad_vector$$
$srccode%hpp% */
typedef CppAD::vector<cppad_double>    cppad_vector;

/* %$$

$subhead cg_vector$$
$srccode%hpp% */
typedef CppAD::vector<cg_double>       cg_vector;

/* %$$

$subhead cppadcg_vector$$
$srccode%hpp% */
typedef CppAD::vector<cppadcg_double>  cppadcg_vector;

/* %$$
$end
*/

# endif
