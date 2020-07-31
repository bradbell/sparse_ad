#! /bin/bash -e
# -----------------------------------------------------------------------------
# Parameters that can be changed
# build_type and prefix are used by all the get routines.
build_type='release'
prefix=`pwd`/build/prefix
extra_cxx_flags='-Wall -pedantic-errors -Wshadow -Wconversion -std=c++11'
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
    echo $*
    eval $*
}
# -----------------------------------------------------------------------------
if [ "$0" != 'bin/run_cmake.sh' ]
then
    echo 'bin/run_cmake.sh: must be run from its parent directory'
    exit 1
fi
# --------------------------------------------------------------------------
if [ "$build_type" != 'debug' ] && [ "$build_type" != "release" ]
then
    echo 'bin/run_cmake.sh: build_type is not debug or release'
    exit 1
fi
if [ ! -e build.$build_type ]
then
    mkdir build.$build_type
fi
if [ -e build ]
then
    rm build
fi
ln -s build.$build_type build
cd build
if [ -e CMakeCache.txt ]
then
    echo_eval rm CMakeCache.txt
fi
if [ -e CMakeFiles ]
then
    echo_eval rm -r CMakeFiles
fi
# --------------------------------------------------------------------------
# This is an example cmake command for this project
export CC=$(which clang)
export CXX=$(which clang++)
echo_eval cmake \
    -D CMAKE_VERBOSE_MAKEFILE='no' \
    -D CMAKE_BUILD_TYPE=\'$build_type\' \
    -D adolc_prefix=\'$prefix\' \
    -D colpack_prefix=\'$prefix\' \
    -D cppad_prefix=\'$prefix\' \
    -D extra_cxx_flags=\'$extra_cxx_flags\' \
    ..
