#! /bin/bash -e
hash='90f184bb17c77506f8965c99a2257a37bae7da3e'
version='2021.5.15'
# --------------------------------------------------------------------------
echo_eval() {
    echo $*
    eval $*
}
# --------------------------------------------------------------------------
if [ "$0" != 'bin/get_xsrst.sh' ]
then
    echo "bin/get_xsrst.sh: must be executed from its parent directory"
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
if [ ! -e cppad_py.git ]
then
    echo_eval git clone https://github.com/bradbell/cppad_py.git cppad_py.git
fi
echo_eval cd cppad_py.git
echo_eval git checkout master
echo_eval git pull
echo_eval git checkout --quiet $hash
if ! grep "^SET(cppad_py_version *\"$version\" *)" CMakeLists.txt > /dev/null
then
    echo 'bin/get_xsrst.sh: hash and version at top of file do not agree.'
    echo "verion in bin/get_xsrst.sh=$version"
    file=$(pwd)/CMakeList.txt
    echo "version in $file"
    grep "^SET(cppad_py_version *\"[0-9]*\" *)" CMakeLists.txt
    exit 1
fi
# --------------------------------------------------------------------------
# install xsrst.py
if [ ! -e $prefix/bin ]
then
    echo_eval mkdir $prefix/bin
fi
echo_eval cp bin/xsrst.py $prefix/bin/xsrst.py
if ! which xsrst.py >& /dev/null
then
    echo 'bin/get_xsrst.sh: You need to execute following to use xsrst.py'
    echo "PATH=\$PATH:$prefix/bin"
fi
# -----------------------------------------------------------------------------
echo 'get_xsrst.sh: OK'
exit 0
