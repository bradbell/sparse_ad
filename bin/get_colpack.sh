#! /bin/bash -e
version='v1.0.10'
# --------------------------------------------------------------------------
echo_eval() {
    echo $*
    eval $*
}
# --------------------------------------------------------------------------
if [ "$0" != 'bin/get_colpack.sh' ]
then
    echo "bin/get_colpack.sh: must be executed from its parent directory"
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
if [ "$build_type" == 'debug' ]
then
    export CFLAGS='-g -O0'
    export CXXFLAGS='-g -O0'
else
    export CFLAGS='-O2'
    export CXXFLAGS='-O2'
fi
# --------------------------------------------------------------------------
if [ ! -e colpack.git ]
then
    echo_eval git clone https://github.com/CSCsw/ColPack.git colpack.git
fi
echo_eval cd colpack.git
echo_eval git checkout --quiet $version
echo_eval autoreconf -i --force
#
echo_eval ./configure \
    --prefix=$prefix \
    --libdir=$prefix/$libdir \
    --enable-shared
make install
# -----------------------------------------------------------------------------
echo 'get_colpack.sh: OK'
exit 0
