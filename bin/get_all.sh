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
bin/get_colpack.sh
bin/get_adolc.sh    # requires colpack
bin/get_eigen.sh
bin/get_cppad.sh
bin/get_cppadcg.sh  # requires eigen and cppad
# --------------------------------------------------------------------------
echo 'bin/get_all.sh: OK'
exit 1
