// vim: set expandtab:
# ifndef SRC_SPEED_TEST_HPP
# define SRC_SPEED_TEST_HPP
/*
$begin speed_test$$
$spell
    hes
    jac
    Jacobian
    adolc
    cppad
    subgraph
    cppadcg
    subcg
    ptr
    Globals
    nnz
$$

$section Setup a Speed Test Problem$$


$head Syntax$$
$codei%test_%method%_jac(%repeat%)
%$$
$code%test_%method%_hes(%repeat%)%$$

$head Prototype$$

$subhead Jacobian$$
$srccode%hpp% */
extern void test_adolc_jac(size_t repeat);
extern void test_cppad_jac(size_t repeat);
extern void test_subgraph_jac(size_t repeat);
extern void test_cppadcg_jac(size_t repeat);
extern void test_subcg_jac(size_t repeat);
/* %$$

$subhead Hessian$$
$srccode%hpp% */
extern void test_adolc_hes(size_t repeat);
extern void test_cppad_hes(size_t repeat);
extern void test_subgraph_hes(size_t repeat);
extern void test_cppadcg_hes(size_t repeat);
extern void test_subcg_hes(size_t repeat);
/* %$$

$head Purpose$$
The Jacobian (Hessian) routines calculate the Jacobian (Hessian)
for objective corresponding to the problem specified by
$cref/global_problem_ptr/speed_global/Initialization/global_problem_ptr/$$

$head method$$
is one of the following
$code adolc$$,
$code cppad$$,
$code subgraph$$,
$code cppadcg$$,
$code subcg$$.

$head Setup$$
The routine $codei%setup_%method%()%$$ is called before
$codei%test_%method%_jac(%repeat%)%$$ or
$code%test_%method%_hes(%repeat%)%$$.

$head Globals$$

$subhead Initialization$$
The $cref/initialization/speed_global/Initialization/$$ global variables
are inputs and not changed by the setup routines.
In addition, the have the same values as during the call to
$codei%setup_%method%()%$$.

$subhead m$$
We use $icode m$$ for the value $code global_problem_ptr->size_range()$$
see $cref/global_problem_ptr/speed_global/Initialization/global_problem_ptr/$$.
This is the dimension of the range space for $latex f(x)$$.

$subhead Jacobian$$
If $icode%m% > 1%$$, the Jacobian test for this method is called.

$subhead Hessian$$
If $icode%m% = 1%$$, the Hessian test for this method is called.

$subhead global_nnz$$
The input value of this global does not matter.
Upon return, it has been set to the number of non-zeros in the sparsity
pattern for the sparse Jacobian or Hessian that is calculated.

$subhead global_correct_ok$$
The input value of this global does not matter.
If $cref/global_correct/speed_global/Initialization/global_correct/$$
is true, $code global_correct_ok$$ has been set to true (false)
if the result calculated by the last repetition of the test
passes (fails) the correctness test; see $cref check_sparse$$.


$end
*/

# endif
