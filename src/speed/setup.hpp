// vim: set expandtab:
# ifndef SRC_SPEED_SETUP_HPP
# define SRC_SPEED_SETUP_HPP
/*
$begin speed_setup$$
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
$codei%setup_%implement%()%$$

$head Prototype$$
$srccode%hpp% */
extern void setup_adolc(void);
extern void setup_cppad(void);
extern void setup_subgraph(void);
extern void setup_cppadcg(void);
extern void setup_subcg(void);
/* %$$

$head Purpose$$
Do as much of the calculation as possible, excluding knowing
the argument value $latex x$$ at which we are evaluating the Jacobian or
Hessian of $latex f(x)$$.

$head implement$$
is one of the following
$code adolc$$,
$code cppad$$,
$code subgraph$$,
$code cppadcg$$,
$code subcg$$.

$head Globals$$

$subhead Initialization$$
The $cref/initialization/speed_global/Initialization/$$ global variables
are inputs and not changed by the setup routines.

$subhead m$$
We use $icode m$$ for the value $code global_problem_ptr->size_range()$$
see $cref/global_problem_ptr/speed_global/Initialization/global_problem_ptr/$$.
This is the dimension of the range space for $latex f(x)$$.

$subhead Hessian$$
A problem is a Hessian test case if $icode%m% = 1%$$.
In the case $codei%test_%implement%_hes%$$ will be called to determine
the speed of evaluation of the Hessian.

$subhead Jacobian$$
A problem is a Jacobian test case if $icode%m% > 1%$$.
In the case $codei%test_%implement%_jac%$$ will be called to determine
the speed of evaluation of the Jacobian.

$subhead Not used$$
$cref/global_nnz/speed_global/global_nnz/$$ and
$cref/global_correct_ok/speed_global/global_correct_ok/$$
are not used by these routines.

$end
*/

# endif
