#! /bin/python3
# vim: set expandtab:
#
# $begin c2hpp.py$$ $newlinech #$$
# $spell
#   dir
#   gitignore
#   minpack
#   tprobs
#   dficfj
#   diff
#   hpp
#   py
#   doublereal
#   ftnlen
#   f2clib
#   adolc
#   cppad
#   cppadcg
# $$
#
# $section Create a C++ Template Version of an f2c File$$
#
# $head Syntax$$
# $codei%bin/c2hpp.py %dir%/c/%name%.c
# %$$
#
# $head Working Directory$$
# see $cref/working directory/bin/Working Directory/$$.
#
# $head dir, name$$
# The command line argument $icode%dir%/c/%name%.c%$$
# is an existing C file name,
# relative to the current working directory,
# and created by $cref f2c.sh$$.
# The $icode name$$ part does not contain any $code /$$ characters; i.e.,
# all the $code /$$ characters are in the $icode dir$$ part of the
# command line argument.
#
# $head Result$$
# The file $icode%dir%/hpp/%name%.hpp%$$ is a C++ version of the C file.
# The $code f2c$$ data types
# $code doublereal$$, $code integer$$, and $code ftnlen$$
# are converted to be template parameters in this include file.
#
# $head Warning$$
# This conversion has only been tested for the files in the
# $code minpack2/tprobs.92/c$$ directory and the $cref/types/typedef.hpp/$$
# $code double$$, $code adolc_double$$, $code cppad_double$$,
# $code cppadcg_double$$.
# New files and types may require modifications to the script
# $code bin/c2hpp.py$$
# and new library files may need to be added to $code f2clib.hpp$$; see
# $codei%
#   minpack2/tprobs.92/hpp/f2clib.hpp
# %$$
#
# $head Example$$
# If one is in the git root directory, the command
# $codei%
#   bin/c2hpp.py minpack2/tprobs.92/c/dficfj.c
# %$$
# will overwrite the git repository copy of $code dficfj.hpp$$.
# The result file should be the same which you can check using
# $codei%
#   git diff minpack2/tprobs.92/hpp/dficfj.hpp
# %$$
#
# $end
# -----------------------------------------------------------------------------
import sys
import re
# -----------------------------------------------------------------------------
if sys.argv[0] != 'bin/c2hpp.py' :
    msg = 'must execut bin/c2hpp.py from its parent directory'
    sys.exit(msg)
if len(sys.argv) != 2 :
    msg   = 'usage: bin/chpp.py c_file'
    msg  += '\nConverts an f2c c_file to a C++ include file'
    sys.exit(msg)
c_file = sys.argv[1]
# -----------------------------------------------------------------------------
# hpp_file and expected_procedure_name
pattern  = r'/c/(.*)\.c'
match    = re.search(pattern, c_file)
if match == None :
    msg  = "cannot find '/c/*.c' in c_file"
    sys.exit(msg)
hpp_file = c_file[0 : match.start(0) ] + '/hpp/' + match.group(1) + '.hpp'
expected_procedure_name = match.group(1)
#
# guard_name
guard_name = hpp_file.upper().replace('.', '_').replace('/','_')
#
# read c_file
file_obj   = open(c_file, 'r')
file_data  = file_obj.read()
file_obj.close()
#
# Remove from top of tile to the include f2c.h statement.
pattern   = r'\n# *include *"f2c\.h"\s*'
match     = re.search(pattern, file_data)
file_data = file_data[ match.end(0) : ]
#
# remove all occurances of /* Subroutine */
file_data = file_data.replace('/* Subroutine */ ', '')
#
# remove underbar an end of name for this procedure
pattern   = r'(int|integer) *(\w+)_\('
match     = re.search(pattern, file_data)
if match == None :
    msg = 'cannot find name for this procedure'
    sys.exit(msg)
start    = match.start()
end      = match.end()
ret_type = match.group(1)
name     = match.group(2)
if name != expected_procedure_name :
    msg  = 'expected procedure name ' + expected_procedure_name + '\n'
    msg += 'found procedure name ' + match.group(2)
    sys.exit(1)
replace   = ret_type + ' ' + name + '('
file_data = file_data[0 : start] + replace + file_data[end : ]
#
# cppadcg_double requires conversion of constructor from double to
# assignment to a double
pattern  = r' doublereal +([a-z_]+)\[([0-9]+)\] *= *{([^}]*)};'
start    = 0
match    = re.search(pattern, file_data[start : ])
while match != None :
    array_name = match.group(1)
    array_size = match.group(2)
    end        = start + match.end()
    start      = start + match.start()
    replace    = ' doublereal ' + array_name +'[' + array_size + '];\n'
    #
    array_size = int( array_size)
    array_list = match.group(3)
    array_list = array_list.replace(',' , ' ')
    array_list = array_list.split()
    if len(array_list) != array_size :
        msg  = 'c2hpp.py: array size not equal to array length.\n'
        msg += 'Detected in ' + c_file  + ' at:\n'
        msg += match.group(0)
        sys.exit(msg)
    for i in range(array_size) :
        replace += array_name + '[' + str(i) + '] = ' + array_list[i] + ';\n'
    file_data = file_data[0 : start] + replace + file_data[end : ]
    start     = start + len(replace)
    match    = re.search(pattern, file_data[start : ])
#
# convert style for comments at the end of a line
pattern   = r'/\*(.*)\*/ *\n'
replace   = r'//\1\n'
file_data = re.sub(pattern, replace, file_data);
#
# Remove three extra spaces after // at beginning of a line.
# This is a result of convert '^c' -> '/* '
pattern   = r'\n//    '
replace   = r'\n// '
file_data = re.sub(pattern, replace, file_data)
#
# change blank lines in block comments to start with //
pattern   = r'\n\n//'
replace   = r'\n//\n//'
file_data = re.sub(pattern, replace, file_data)
#
# Remove C style declarations of s_copy
pattern  = r'\n.*s_copy\(char *\*, char *\*, *ftnlen, *ftnlen\); *\n'
replace  = r'\n'
file_data = re.sub(pattern, replace, file_data)
#
# Remove C style declarations of s_cmp
pattern  = r'\n.*s_cmp\(char *\*, char *\*, *ftnlen, *ftnlen\); *\n'
replace  = r'\n'
file_data = re.sub(pattern, replace, file_data)
#
# Remove C style declarations of d_sign
pattern  = r'\n *double d_sign\(doublereal [*], doublereal [*]\);'
replace  = ''
file_data = re.sub(pattern, replace, file_data)
#
# Remove C style declarations unary math functions
pattern  = r' [a-z]+\(doublereal\),'
replace  = ''
file_data = re.sub(pattern, replace, file_data)
pattern  = r'\n *double +[a-z]+\(doublereal\);'
replace  = ''
file_data = re.sub(pattern, replace, file_data)
#
# abs is a macro in f2c.h, use AD version of abs function
pattern   = r'([^a-z_])abs\('
replace   = r'\1fabs('
file_data = re.sub(pattern, replace, file_data)
#
# Add includes and template declaration at the top of the source code
include  = '# include <cmath>\n'
include += '# include <cppad/cg/cppadcg.hpp>\n'
include += '# include <adolc/adolc.h>\n'
include += '# include "f2clib.hpp"\n'
template = 'template <class doublereal, class integer, class ftnlen>\n'
file_data = include + '\n' + template + file_data
#
# add include guard
guard  = '# ifndef ' + guard_name + '\n'
guard += '# define ' + guard_name + '\n'
file_data = guard + '\n' + file_data + '\n\n# endif\n'
#
# lastly, remove spaces at end of lines
pattern = r' +\n'
replace = r'\n'
file_data = re.sub(pattern, replace, file_data)
#
# write hpp_file
file_obj   = open(hpp_file, 'w')
file_obj.write(file_data)
file_obj.close()
#
print('c2hpp.py: OK')
sys.exit(0)
