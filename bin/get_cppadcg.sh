#! /bin/bash -e
# vim: set expandtab:
# --------------------------------------------------------------------------
# can use a git has, branch, or release tag
version='v2.4.2'
# --------------------------------------------------------------------------
echo_eval() {
    echo $*
    eval $*
}
# --------------------------------------------------------------------------
if [ "$0" != 'bin/get_cppadcg.sh' ]
then
    echo "bin/get_cppadcg.sh: must be executed from its parent directory"
    exit 1
fi
# --------------------------------------------------------------------------
# build_type
cmd=`grep '^build_type=' bin/run_cmake.sh`
eval $cmd
echo "build_type=$build_type"
#
# prefix
cmd=`grep '^prefix=' bin/run_cmake.sh`
eval $cmd
echo "prefix=$prefix"
#
# libdir
if [ -e /usr/lib64 ]
then
    libdir='lib64'
else
    libdir='lib'
fi
echo "libdir=$libdir"
# --------------------------------------------------------------------------
if [ "$build_type" != 'debug' ] && [ "$build_type" != "release" ]
then
    echo "$0: build_type in bin/run_cmake not debug or release"
    exit 1
fi
# --------------------------------------------------------------------------
# link the build directory
if [ ! -e build.$build_type ]
then
    mkdir build.$build_type
fi
if [ -e build ]
then
    rm build
fi
ln -s build.$build_type build
# --------------------------------------------------------------------------
# change into the build/external directory
if [ ! -e build/external ]
then
    mkdir build/external
fi
cd build/external
# --------------------------------------------------------------------------
if [ ! -e cppadcg.git ]
then
    echo_eval git clone \
        https://github.com/joaoleal/CppADCodeGen.git cppadcg.git
fi
echo_eval cd cppadcg.git
echo_eval git reset --hard
echo_eval git checkout master
echo_eval git pull
echo_eval git checkout --quiet $version
if [ -e build ]
then
    echo_eval rm -r build
fi
echo_eval mkdir build
echo_eval cd build
#
echo_eval cmake \
    -D CMAKE_INSTALL_PREFIX=$prefix \
    -D GOOGLETEST_GIT=ON \
    ..
echo_eval make install
# -----------------------------------------------------------------------------
echo 'get_cppadcg.sh: OK'
exit 0
