#! /bin/bash -e
# vim: set expandtab:
hash='5c86b1803d1ce47dfce5b20c58a64c6cf19a61e0'
version='20200523'
# --------------------------------------------------------------------------
echo_eval() {
    echo $*
    eval $*
}
# --------------------------------------------------------------------------
if [ "$0" != 'bin/get_cppad.sh' ]
then
    echo "bin/get_cppad.sh: must be executed from its parent directory"
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
# --------------------------------------------------------------------------
# debug_which
if [ "$build_type" == 'debug' ]
then
    debug_which='debug_all'
elif [ "$build_type" == "release" ]
then
    debug_which='debug_none'
else
    echo 'bin/get_cppad: build_type in bin/run_cmake not debug or release'
    exit 1
fi
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
# checkout out cppad.git
if [ ! -e cppad.git ]
then
    echo_eval git clone https://github.com/coin-or/CppAD.git cppad.git
fi
echo_eval cd cppad.git
echo_eval git checkout master
echo_eval git pull
echo_eval git checkout --quiet $hash
if ! grep "^SET(cppad_version *\"$version\" *)" CMakeLists.txt > /dev/null
then
    echo 'bin/get_cppad.sh: hash and version at top of file do not agree.'
    echo "verion in bin/get_cppad.sh=$version"
    file=$(pwd)/CMakeList.txt
    echo "version in $file"
    grep "^SET(cppad_version *\"[0-9]*\" *)" CMakeLists.txt
    exit 1
fi
# --------------------------------------------------------------------------
# buld and install cppad
if [ -e build ]
then
    echo_eval rm -r build
fi
echo_eval mkdir build
echo_eval cd build
echo_eval cmake \
    -D cppad_prefix="$prefix" \
    -D colpack_prefix="$prefix" \
    -D cmake_install_libdirs="lib\;lib64" \
    -D cppad_debug_which="$debug_which" \
    ..
echo_eval make install
# -----------------------------------------------------------------------------
echo 'get_cppad.sh: OK'
exit 0
