# ifndef MINPACK2_TPROBS_92_HPP_MIN_HPP
# define MINPACK2_TPROBS_92_HPP_MIN_HPP

// Brad Bell converted this version of min from the f2c.h definiton:
// #define min(a,b) ((a) <= (b) ? (a) : (b))
// on 2019-02-27. One could add specializations that use conditional assignments
// for cppad_double and adolc_double.

template <class Type> Type min(const Type& a, const Type& b)
{	return ((a) <= (b) ? (a) : (b)); }

# endif
