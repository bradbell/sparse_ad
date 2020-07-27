#! /bin/bash -e
# vim: set expandtab:
# --------------------------------------------------------------------------
echo_eval() {
    echo $*
    eval $*
}
# --------------------------------------------------------------------------
if [ "$0" != 'bin/get_all.sh' ]
then
    echo "bin/get_all.sh: must be executed from its parent directory"
    exit 1
fi
# --------------------------------------------------------------------------
# $begin get_all.sh$$
#
# $section Local install of Packages Required by sparse_ad$$
#
# $head Syntax$$
# $codei%bin/get_all.sh%$$
#
# $head Implementation$$
# This script just executes the following scripts in order:
# $srccode%sh%
bin/get_colpack.sh  # Colpack
bin/get_adolc.sh    # ADOL-C:requires colpack
bin/get_eigen.sh    # Eigen
bin/get_cppad.sh    # CppAD
bin/get_cppadcg.sh  # CppADCodeGen: requires Eigen and CppAD
# %$$
#
# $end
#
# --------------------------------------------------------------------------
echo 'bin/get_all.sh: OK'
exit 1
