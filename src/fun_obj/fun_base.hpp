// vim: set expandtab:
# ifndef SRC_FUN_OBJ_FUN_BASE_HPP
# define SRC_FUN_OBJ_FUN_BASE_HPP


/*
$begin fun_base.hpp$$
$spell
    obj
    const
    jac
    prototyped
    hes
    src
    hpp
$$

$section Base Class For Test Function Objects$$

$head Syntax$$
$codei%# include <src/fun_obj/fun_base.hpp>
%$$
$icode%x_out% = %fun_obj%.start()
%$$
$icode%m% = %fun_obj%.size_range()
%$$
$icode%fun_obj%.fun(%x%, %fun_out%)
%$$
$icode%ok% = %fun_obj%.jac(%x%, %jac_out%)
%$$
$icode%ok% = %fun_obj%.grad(%x%, %grad_out%)
%$$
$icode%ok% = %fun_obj%.hes(%x%, %hes_out%)
%$$


$head Purpose$$
Each of the test functions is a mapping
$latex f : \B{R}^n \rightarrow \B{R}^m$$.
It is defined using a class that is derived from the base class
$code fun_base$$.

$head fun_obj$$
We use the notation $icode fun_obj$$ for a derived class object
corresponding to the function $latex f(x)$$.

$head Constructor$$
The constructor (and destructor) for a $code fun_base$$ has no arguments.

$head Type Definitions$$
The file $cref typedef.hpp$$ is included by $code fun_base.hpp$$.

$head start$$
This function returns the standard starting point for point for optimizing
the function. It has the following prototype
$srcthisfile%
    0%// BEGIN_START%// END_START%1
%$$

$subhead x_out$$
This vector is the starting point.

$subhead n$$
The dimension of the domain space is equal to the size of $icode x_out$$.

$head size_range$$
This function has the following prototype
$srcthisfile%
    0%// BEGIN_SIZE_RANGE%// END_SIZE_RANGE%1
1%$$
The corresponding virtual function must be defined by the derived class.

$subhead m$$
The return value $icode m$$ is the dimension of the range.

$head fun$$
These functions function evaluate $latex f(x)$$.
They have the following prototypes
$srcthisfile%
    0%// BEGIN_FUN%// END_FUN%1
1%$$
The corresponding virtual functions must be defined by the derived class.

$subhead x$$
The argument $icode x$$ has size $icode n$$ and specifies
the point at which the function is evaluated.
It is effectively $code const$$, but is not prototyped as such
in order to avoid the time to copy the vector to a temporary.

$subhead fun_out$$
The return value $icode fun_out$$ has size $icode m$$.
The input value of its elements does not matter.
Upon return, it contains the value of $latex f(x)$$.

$head jac$$
This function evaluates $latex f'(x)$$
It has the following prototype
$srcthisfile%
    0%// BEGIN_JAC%// END_JAC%1
1%$$
The corresponding virtual functions must be defined by the derived class.

$subhead ok$$
If $icode%m% == 1%$$, this function returns $icode ok$$ as false
and no other action is taken.
Otherwise $icode ok$$ is true.

$subhead x$$
The argument $icode x$$ has size $icode n$$ and specifies
the point at which the function is evaluated.
It is effectively $code const$$, but is not prototyped as such
in order to avoid the time to copy the vector to a temporary.

$subhead jac_out$$
The return value $icode jac_out$$ has size $icode%m% * %n%$$.
The input value of its elements does not matter.
Upon return, it contains the value of $latex f'(x)$$.
To be specific,
for $icode%i% = 0 , %...%, %m%-1%$$,
$icode%j% = 0 , %...%, %n%-1%$$,
$icode%jac_out%[%i% + %m% * %j%]%$$
is the partial of $latex f_i (x)$$ with respect to $latex x_j$$.

$head grad$$
This function evaluates $latex f'(x)$$.
It has the following prototype
$srcthisfile%
    0%// BEGIN_GRAD%// END_GRAD%1
1%$$
The corresponding virtual functions must be defined by the derived class.

$subhead ok$$
If $icode%m% != 1%$$, this function returns $icode ok$$ as false
and no other action is taken.
Otherwise $icode ok$$ is true.

$subhead x$$
The argument $icode x$$ has size $icode n$$ and specifies
the point at which the function is evaluated.
It is effectively $code const$$, but is not prototyped as such
in order to avoid the time to copy the vector to a temporary.

$subhead grad_out$$
The return value $icode grad_out$$ has size $icode n$$.
The input value of its elements does not matter.
Upon return, it contains the value of $latex f'(x)$$.
To be specific,
for $icode%j% = 0 , %...%, %n%-1%$$,
$icode%grad_out%[%j%]%$$
is the partial of $latex f(x)$$ with respect to $latex x_j$$.

$head hes$$
This function evaluates $latex f^{(2)}(x)$$.
It has the following prototype
$srcthisfile%
    0%// BEGIN_HES%// END_HES%1
1%$$
The corresponding virtual functions must be defined by the derived class.

$subhead ok$$
If $icode%m% != 1%$$, this function returns $icode ok$$ as false
and no other action is taken.
Otherwise $icode ok$$ is true.

$subhead x$$
The argument $icode x$$ has size $icode n$$ and specifies
the point at which the function is evaluated.
It is effectively $code const$$, but is not prototyped as such
in order to avoid the time to copy the vector to a temporary.

$subhead hes_out$$
The return value $icode hes_out$$ has size $icode%n% * %n%$$.
The input value of its elements does not matter.
Upon return, it contains the value of $latex f^{(2)} (x)$$.
To be specific,
for $icode%j% = 0 , %...%, %n%-1%$$,
$icode%k% = 0 , %...%, %n%-1%$$,
$icode%hes_out%[%j% + %k% * %n%]%$$
is the second partial of $latex f(x)$$ with respect to
$latex x_j$$ and $latex x_k$$.


$end
*/

# include <src/typedef.hpp>

// a purely virtual base class for the interfact to a test function
class fun_base {

public:
    // constructor, destructor
    fun_base(void);
    ~fun_base(void);

// BEGIN_START
    virtual d_vector start(void) = 0 ;
// END_START

// BEGIN_SIZE_RANGE
    virtual size_t size_range(void) = 0;
// END_SIZE_RANGE

// BEGIN_FUN
    virtual void fun(d_vector&       x, d_vector&       fun_out) = 0 ;
    virtual void fun(adolc_vector&   x, adolc_vector&   fun_out) = 0 ;
    virtual void fun(cppad_vector&   x, cppad_vector&   fun_out) = 0 ;
    virtual void fun(cppadcg_vector& x, cppadcg_vector& fun_out) = 0 ;
// END_FUN

// BEGIN_JAC
    virtual bool jac(d_vector& x, d_vector& jac_out) = 0 ;
// END_JAC

// BEGIN_GRAD
    virtual bool grad(d_vector& x, d_vector& grad_out) = 0 ;
// END_GRAD

// BEGIN_HES
    virtual bool hes(d_vector& x, d_vector& hes_out) = 0 ;
// END_HES
};

# endif
