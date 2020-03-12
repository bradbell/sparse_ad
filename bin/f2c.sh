#! /bin/bash -e
# vim: set expandtab:
#
# $begin f2c.sh$$ $newlinech #$$
# $spell
#   dir
#   gitignore
#   minpack
#   tprobs
#   dficfj
#   diff
# $$
#
# $section Create a C Version of a Fortran File$$
#
# $head Syntax$$
# $codei%bin/f2c.sh %dir%/%name%.f
# %$$
#
# $head Working Directory$$
# see $cref/working directory/bin/Working Directory/$$.
#
# $head dir, name$$
# The command line argument $icode%dir%/%name%.f%$$
# is an existing Fortran file name
# relative to the current working directory.
# The $icode name$$ part does not contain any $code /$$ characters; i.e.,
# all the $code /$$ characters are in the $icode dir$$ part of the
# command line argument.
#
# $head Result$$
# The file $icode%dir%/c/%name%.c%$$ is a C version of the Fortran file
# (created by $code f2c$$).
#
# $head Example$$
# If one is in the git root directory, the command
# $codei%
#   bin/f2c.sh minpack2/tprobs.92/dficfj.f
# %$$
# will overwrite the git repository copy of $code dficfj.c$$.
# The result file should be the same which you can check using
# $codei%
#   git diff minpack2/tprobs.92/c/dficfj.c
# %$$
#
# $end
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
    echo $*
    eval $*
}
# -----------------------------------------------------------------------------
if [ "$0" != 'bin/f2c.sh' ]
then
    echo 'bin/f2c.sh must be executed from its parent directory'
    exit 1
fi
if [ "$1" == '' ] || [ "$2" != '' ]
then
    echo 'usage: bin/f2c.sh f_file'
    echo 'where f_file is a fortan source file'
    exit 1
fi
file="$1"
ext=`echo $file | sed -e 's|.*\.||'`
if [ "$ext" != 'f' ]
then
    echo "bin/f2c.sh: $file does not have .f extension"
    exit 1
fi
if [ ! -f $file ]
then
    echo "bin/f2c.sh: $file does not exist"
    exit 1
fi
dir=`echo $file | sed -e 's|/[^/]*$||'`
# -----------------------------------------------------------------------------
# -u:      Make the  default  type  of  a variable `undefined'
# -a:      Make local variables automatic rather than static unless they appear
#          in a DATA, EQUIVALENCE, NAMELIST, or SAVE statement.
# -ext:    Complain about f77(1) extensions.
# -i90:    Do not recognize  the  Fortran  90  bit-manipulation  intrinsics
#          btest, iand, ibclr, ibits, ibset, ieor, ior, ishft, and ishftc.
# -P:      Write a file.P of ANSI (or C++) prototypes  for  definitions  in
#          each input file.f or file.F.
# -r8:     Promote REAL to DOUBLE PRECISION, COMPLEX to DOUBLE COMPLEX.
# -ddir:   Write `.c' files in directory dir instead of the current  directory.
#
echo_eval f2c -u -a -ext -i90 -P -r8 -d$dir/c $file
name=`echo $file | sed -e 's|.*/||' -e 's|\.f$||'`
sed < $dir/c/$name.P > $dir/c/$name.h \
    -e 's|doublereal|double|g' \
    -e 's|integer|int|g' \
    -e 's|ftnlen|int|g' \
    -e '1,1s|^|extern "C" {\n|' \
    -e '$,$s|$|\n}|'
# -----------------------------------------------------------------------------
echo 'f2c.sh: OK'
exit 0
