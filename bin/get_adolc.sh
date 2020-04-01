#! /bin/bash -e
# vim: set expandtab:
web_page='https://github.com/coin-or/ADOL-C.git'
version='releases/2.6.3'
# --------------------------------------------------------------------------
echo_eval() {
    echo $*
    eval $*
}
# --------------------------------------------------------------------------
if [ "$0" != 'bin/get_adolc.sh' ]
then
    echo "bin/get_adolc.sh: must be executed from its parent directory"
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
if [ ! -e adolc.git ]
then
    echo_eval git clone $web_page adolc.git
fi
echo_eval cd adolc.git
echo_eval git reset --hard
echo_eval git checkout --quiet $version
echo_eval autoreconf --force --install
#
# Patch to fix memory leak in sparse reverse mode
sed -i 'ADOL-C/src/fo_rev.c' \
    -e 's|^    myfree2(rpp_A);|    free(rpp_A[0]);\n&|'
# --------------------------------------------------------------------------
if [ -e build ]
then
    echo_eval rm -r build
fi
echo_eval mkdir build
echo_eval cd build
#
flags="--prefix=$prefix"
flags="$flags --with-colpack=$prefix"
flags="$flags --libdir=$prefix/$libdir"
flags="$flags --with-colpack=$prefix"
flags="$flags --enable-sparse --enable-static --disable-shared --enable-addexa"
if [ "$build_type" == 'debug' ]
then
    export CFLAGS='-g -O0'
    export CXXFLAGS='-g -O0'
    flags="$flags --with-cflags='-g -O0' --with-cxxflags='-g -O0'"
else
    export CFLAGS='-O2'
    export CXXFLAGS='-O2'
    flags="$flags --with-cflags=-O2 --with-cxxflags=-O2"
fi
echo_eval ../configure $flags
echo_eval make install
# -----------------------------------------------------------------------------
echo 'get_adolc.sh: OK'
exit 0
