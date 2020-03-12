# ifndef MINPACK2_TPROBS_92_HPP_D_SIGN_HPP
# define MINPACK2_TPROBS_92_HPP_D_SIGN_HPP

// Brad Bell converted this version of d_sign to a C++ header file on
// 2019-01-25. See the difference of this file from ../c/d_sign.c

template <class doublereal>
doublereal d_sign(doublereal *a, doublereal *b)
{
	doublereal x = (*a >= 0 ? *a : - *a);
	return( *b >= 0 ? x : -x);
}

# endif
