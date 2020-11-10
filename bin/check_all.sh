#! /bin/bash -e
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
    echo $*
    eval $*
}
# -----------------------------------------------------------------------------
if [ "$0" != 'bin/check_all.sh' ]
then
    echo 'must execut bin/check_all.sh from its parent directory'
    exit 1
fi
if [ "$1" != 'debug' ] && [ "$1" != 'release' ]
then
    echo 'usage: bin/check_all.sh build_type'
    echo 'where build_type is debug or release'
    exit 1
fi
build_type="$1"
# -----------------------------------------------------------------------------
if ! grep "^build_type='release'" bin/run_cmake.sh > /dev/null
then
    echo 'bin/check_all.sh: build_type in bin/run_cmake.sh must be release'
    echo 'when starting this script and is restored at end of this script.'
    exit 1
fi
if [ "$build_type" == 'debug' ]
then
    # change build_type in bin/run_cmake.sh
    sed -i bin/run_cmake.sh -e "s|^build_type=.*|build_type='debug'|"
fi
# -----------------------------------------------------------------------------
bin/check_tab.sh
bin/check_inc_guard.sh
# -----------------------------------------------------------------------------
# check wget_minpack.sh and f2c.sh results
dir='minpack2/tprobs.92'
list=`ls $dir/*.f`
change='no'
for file in $list
do
    echo "bin/wget_minpack2.sh $file >& /dev/null"
    if ! bin/wget_minpack2.sh $file >& /dev/null
    then
        echo 'bin/check_all.sh: bin/wget_minpack2.sh failed'
        exit 1
    fi
    if ! git diff --exit-code $file > /dev/null
    then
        echo "file changed using: bin/wget_minpack2.sh $name"
        change='yes'
    else
        echo "$file: OK"
    fi
    name=`echo $file | sed -e "s|$dir/||" -e 's|\.f$||'`
    bin/f2c.sh $file >& /dev/null
    if ! git diff --exit-code $dir/c/$name.c > /dev/null
    then
        echo "file changed using: bin/f2c.sh $file"
        change='yes'
    else
        echo "$dir/c/$name.c: OK"
    fi
    # Hessian routines names end in hs (C++ version not needed)
    if [[ "$name" =~ .*hs ]]
    then
        echo "bin/c2hpp.py: not converting $name"
    else
        bin/c2hpp.py $dir/c/$name.c >& /dev/null
        if ! git diff --exit-code $dir/hpp/$name.hpp > /dev/null
        then
            echo "file changed using: bin/c2hpp.py $dir/c/$name.c"
            change='yes'
        else
            echo "$dir/hpp/$name.hpp: OK"
        fi
    fi
done
if [ "$change" == 'yes' ]
then
    echo "At least one automatically generated file"
    echo "does not agree with its copy in the git repository."
    exit 1
fi
# -----------------------------------------------------------------------------
# check documentation runs
bin/run_xsrst.sh
# -----------------------------------------------------------------------------
# check example
bin/run_cmake.sh
pushd build
make check_example
popd
# -----------------------------------------------------------------------------
# check speed
bin/run_cmake.sh
pushd build/src/speed
make speed
if [ -e speed.csv ]
then
    echo_eval rm speed.csv
fi
problem_list='dficfj dierfj deptfg dgl1fg'
method_list='adolc cppad subgraph cppadcg subcg'
count='0'
for problem in $problem_list
do
    for method in $method_list
    do
        let "count = $count + 1"
        size='10'
        colpack=''
        onepass=''
        reverse=''
        if [ "$method" == 'adolc' ]
        then
            colpack='--colpack'
            if [[ "$problem" =~ .*fj$ ]]
            then
                onepass='--onepass'
            fi
        fi
        if [[ "$method" =~ sub.* ]]
        then
            reverse='--reverse'
        fi
        echo_eval ./speed speed.csv count=$count \
            $method $problem $size $colpack $onepass $reverse --correct
    done
done
popd
# -----------------------------------------------------------------------------
if [ "$build_type" == 'debug' ]
then
    # restore build_type in bin/run_cmake.sh
    sed -i bin/run_cmake.sh -e "s|^build_type=.*|build_type='release'|"
fi
# -----------------------------------------------------------------------------
echo 'check_all.sh: OK'
exit 0
