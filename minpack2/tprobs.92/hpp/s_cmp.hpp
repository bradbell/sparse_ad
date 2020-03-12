# ifndef MINPACK2_TPROBS_92_HPP_S_CMP_HPP
# define MINPACK2_TPROBS_92_HPP_S_CMP_HPP

// Brad Bell converted this version of s_cmp to a C++ header file on
// 2019-01-25. See the difference of this file from ../c/s_cmp.c

/* compare two strings */

template <class ftnlen>
int s_cmp(const char *a0, const char *b0, ftnlen la, ftnlen lb)
{
const unsigned char *a, *aend, *b, *bend;
a = (unsigned char *)a0;
b = (unsigned char *)b0;
aend = a + la;
bend = b + lb;

if(la <= lb)
	{
	while(a < aend)
		if(*a != *b)
			return( *a - *b );
		else
			{ ++a; ++b; }

	while(b < bend)
		if(*b != ' ')
			return( ' ' - *b );
		else	++b;
	}

else
	{
	while(b < bend)
		if(*a == *b)
			{ ++a; ++b; }
		else
			return( *a - *b );
	while(a < aend)
		if(*a != ' ')
			return(*a - ' ');
		else	++a;
	}
return(0);
}

# endif
