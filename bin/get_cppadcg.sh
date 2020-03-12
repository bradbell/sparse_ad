#! /bin/bash -e
# vim: set expandtab:
# --------------------------------------------------------------------------
# date corresponding to this commit is 20200126
version='505f1a2a1661a454a4e66ad07fcb4ca206e96d7e'
version='master'
version='v2.4.1'
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
echo_eval git checkout --quiet $version
# patch source code
sed -i include/cppad/cg/util.hpp \
    -e 's|^std::string readStringFromFile(|inline &|'
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
