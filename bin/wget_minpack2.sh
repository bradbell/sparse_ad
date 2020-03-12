#! /bin/bash -e
# vim: set expandtab:
#
# $begin wget_minpack2.sh$$ $newlinech #$$
# $spell
#   Minpack
#   Argonne
#   wget
#   gitignore
#   ftp.mcs.anl.gov
#   tprobs
#   dficfj
#   diff
# $$
#
# $section Get a Minpack2 File From Argonne National Laboratory$$
#
# $head Syntax$$
# $codei%bin/wget_minpack2.sh minpack2/%file
# %$$
#
# $head Working Directory$$
# see $cref/working directory/bin/Working Directory/$$.
#
# $head file$$
# The $icode file$$ part of the command line argument
# $codei%minpack2/%file%$$ is a valid file location
# relative to the Argonne National Laboratory copy of minpack2; i.e.,
# $codei%
#   ftp.mcs.anl.gov/pub/MINPACK-2%file%
# %$$
# is the a valid file location.
#
# $head Result$$
# The file is copied to the location $codei%minpack2/%file%$$
# relative to the current working directory (git root directory).
#
# $head Example$$
# If one is in the git root directory, the command
# $codei%
#   bin/wget_minpack2.sh minpack2/tprobs.92/dficfj.f
# %$$
# will overwrite the git repository copy of $code dficfj.f$$.
# The result file should be the same which you can check using
# $codei%
#   git diff minpack2/tprobs.92/dficfj.f
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
remote='ftp.mcs.anl.gov/pub/MINPACK-2'
if [ "$0" != 'bin/wget_minpack2.sh' ]
then
    echo 'must execut bin/wget_minpack2.sh from its parent directory'
    exit 1
fi
if [ "$1" == '' ] || [ "$2" != '' ]
then
    echo 'usage: bin/wget_minpack2 file'
    echo "Places a copy of $remote/file in file"
    echo 'where file begins with minpack2/'
    exit 1
fi
file="$1"
if ! echo $file | grep '^minpack2/' > /dev/null
then
    echo "$file does not begin with minpack2/"
    exit 1
fi
# -----------------------------------------------------------------------------
dir=`echo $file | sed -e 's|/[^/]*$||'`
if [ ! -d "$dir" ]
then
    echo "Must first create the directory $dir"
    exit 1
fi
local_file=`echo $file | sed -e 's|^minpack2/||'`
echo_eval wget $remote/$local_file -O minpack2/$local_file
echo 'wget_minpack2.sh: OK'
exit 0
