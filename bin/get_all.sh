#! /bin/bash -e
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
# {xsrst_comment_ch #}
#
# {xsrst_begin get_all_sh}
#
# .. include:: ../preamble.rst
#
# Local install of Packages Required by sparse_ad
# ###############################################
#
# Syntax
# ******
# ``bin/get_all.sh``
#
# Implementation
# **************
# This script just executes the following scripts in order:
# {xsrst_code sh}
bin/get_colpack.sh  # Colpack
bin/get_adolc.sh    # ADOL-C:requires colpack
bin/get_eigen.sh    # Eigen
bin/get_cppad.sh    # CppAD
bin/get_cppadcg.sh  # CppADCodeGen: requires Eigen and CppAD
bin/get_xsrst.sh    # Only needed to build sparse_ad documentation
# {xsrst_code}
#
# {xsrst_end get_all_sh}
#
# --------------------------------------------------------------------------
echo 'bin/get_all.sh: OK'
exit 1
