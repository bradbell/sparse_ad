#! /bin/bash -e
# --------------------------------------------------------------------------
version='3.3.7'
web_page='https://gitlab.com/libeigen/eigen.git'
# --------------------------------------------------------------------------
echo_eval() {
    echo $*
    eval $*
}
# --------------------------------------------------------------------------
if [ "$0" != 'bin/get_eigen.sh' ]
then
    echo "bin/get_eigen.sh: must be executed from its parent directory"
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
if [ ! -e eigen.git ]
then
    echo_eval git clone $web_page eigen.git
fi
echo_eval cd eigen.git
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
    ..
echo_eval make install
# -----------------------------------------------------------------------------
echo 'get_eigen.sh: OK'
exit 0
