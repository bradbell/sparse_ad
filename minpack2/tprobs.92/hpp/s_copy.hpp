# ifndef MINPACK2_TPROBS_92_HPP_S_COPY_HPP
# define MINPACK2_TPROBS_92_HPP_S_COPY_HPP

// Brad Bell converted this version of s_copy to a C++ header file on
// 2019-01-25. See the difference of this file from ../c/s_copy.c

/* Unless compiled with -DNO_OVERWRITE, this variant of s_copy allows the
 * target of an assignment to appear on its right-hand side (contrary
 * to the Fortran 77 Standard, but in accordance with Fortran 90),
 * as in  a(2:5) = a(4:7) .
 */

/* assign strings:  a = b */

template <class ftnlen>
void s_copy(char *a, const char *b, ftnlen la, ftnlen lb)
{
	register char *aend;
	const char *bend;

	aend = a + la;

	if(la <= lb)
#ifndef NO_OVERWRITE
		if (a <= b || a >= b + la)
#endif
			while(a < aend)
				*a++ = *b++;
#ifndef NO_OVERWRITE
		else
			for(b += la; a < aend; )
				*--aend = *--b;
#endif

	else {
		bend = b + lb;
#ifndef NO_OVERWRITE
		if (a <= b || a >= bend)
#endif
			while(b < bend)
				*a++ = *b++;
#ifndef NO_OVERWRITE
		else {
			a += lb;
			while(b < bend)
				*--a = *--bend;
			a += lb;
			}
#endif
		while(a < aend)
			*a++ = ' ';
		}
	}

# endif
