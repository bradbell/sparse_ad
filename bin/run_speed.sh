#! /bin/bash -e
# {xsrst_comment_ch #}
# {xsrst_begin run_speed_sh}
# {xsrst_spell
#   yymmdd
# }
#
# .. include:: ../preamble.rst
#
# Run All the Jacobian and Hessian Speed Tests
# ############################################
#
# Purpose
# *******
# The :ref:`speed<speed>` program has many possible
# combinations of options.
# This script runs all the possible combinations
# and collects the results in one csv file.
#
# Syntax
# ******
# - ``bin/run_speed.sh`` *name* *build_type*
# - ``bin/run_speed.sh`` *name* *build_type* ``--correct``
#
# name
# ****
# The results for the speed tests are written to the file
#
# |tab| ``csv/``\ *name-yymmdd*\ ``.csv``
#
# where *yymmdd** is the current year, month and day.
#
# build_type
# **********
# This is either ``debug`` or release and determines which build
# is used to run the speed tests.
# The ``debug`` build is only intended for debugging and testing.
#
# --correct
# *********
# If ``--correct`` is present, the
# :ref:`--correct<speed.option_list.--correct>` option is included
# for all the speed tests.
# This is only intended for correctness testing.
#
# Implementation List
# *******************
# The code below selects which
# :ref:`implementations<speed.implement>` are included:
# {xsrst_code sh}
implement_list='adolc cppad subgraph cppadcg subcg'
implement_list='adolc cppad subgraph' # without code generation
# {xsrst_code}
# This choice is in ``bin/run_speed.sh`` and can be changed.
# Note that the ``cppadcg`` and ``subcg`` choices run the derivative code
# through a compiler before using it and the compliation can take a long time.
#
# Problem List
# ************
# The code below selects which
# :ref:`problems<speed.problem>` are included:
# {xsrst_code sh}
problem_list='dficfj dierfj deptfg dgl1fg'
# {xsrst_code}
# This code is in ``bin/run_speed.sh`` and can be changed.
#
# {xsrst_end run_speed_sh}
# --------------------------------------------------------------------------
echo_eval() {
    echo $*
    eval $*
}
# --------------------------------------------------------------------------
top_srcdir=$(pwd)
speed_program='../build/src/speed/speed'
run_speed() {
    echo "$top_srcdir/bin/massif.sh $speed_program dummy.csv $*"
    n_bytes=`$top_srcdir/bin/massif.sh $speed_program dummy.csv $*`
    echo "KB = $n_bytes / 1000"
    KB=`echo "$n_bytes 1000" | awk '{print int($1 / $2 + 0.5)}'`
    echo "$speed_program $csv_file KB=$KB $*"
    $speed_program $csv_file KB=$KB $*
}
# --------------------------------------------------------------------------
if [ "$0" != 'bin/run_speed.sh' ]
then
    echo "bin/run_speed.sh: must be executed from its parent directory"
    exit 1
fi
#
usage_error='no'
if  [ "$2" != 'debug' ] && [ "$2" != 'release' ]
then
    usage_error='yes'
fi
if  [ "$3" != '' ] && [ "$3" != '--correct' ]
then
    usage_error='yes'
fi
if [ "$usage_error" == 'yes' ]
then
cat << EOF

usage: bin/run_speed.sh name build_type [--correct]

Runs speed tests possible combinations of options except for --correct.
This creates the file csv/name-yymmdd.csv with results
where mm is the current month and add is the current day.
The build_type must be debug or release.
If --correct is present, the --correct option is included in the speed runs.
EOF
    exit 1
fi
name="$1"
build_type="$2"
correct="$3"
# --------------------------------------------------------------------------
yymmdd=`date +%y%m%d`
csv_file="$name-$yymmdd.csv"
if [ -e csv/$csv_file ]
then
    echo 'bin/run_speed.sh: The output file already exists, remove it ?'
    echo "    rm csv/$csv_file"
    exit 1
fi
# --------------------------------------------------------------------------
# set build_type as specified at top of this file
sed -i bin/run_cmake.sh -e "s|^build_type=.*|build_type='$build_type'|"
# configure
bin/run_cmake.sh
# make speed
cd build
make speed
# set csv to be the working directory
cd ../csv
# --------------------------------------------------------------------------
# set memory limits
#
total=`free | sed -n -e 's|^ *\([^ ]*\).*|\1|' -e '1,1p'`
if [ "$total" != 'total' ]
then
    echo 'bin/run_speed.sh: expected total to be first column'
    exit 1
fi
mem=`free -k | sed -n -e 's|^Mem: *\([^ ]*\) .*|\1|' -e '2,2p'`
swap=`free -k | sed -n -e 's|^Swap: *\([^ ]*\) .*|\1|' -e '3,3p'`
#
mem=`echo "$mem / 2" | bc`
swap=`echo "$swap / 2" | bc`
#
echo_eval ulimit -d $mem
echo_eval ulimit -v $swap
# --------------------------------------------------------------------------
#
count='0'
for just_count in 'true' 'false'
do
    total_count="$count"
    count='0'
    #
for implement in $implement_list
do
for problem in $problem_list
do
for setup in --setup   ''
do
for reverse in --reverse ''
do
for onepass in --onepass  ''
do
for optimize in --optimize ''
do
for indirect in --indirect ''
do
for colpack in --colpack ''
do
    case $problem in
        dficfj)
        hessian=''
        size=400
        ;;

        dierfj)
        hessian=''
        size=200
        ;;

        deptfg)
        hessian='--hessian'
        size=60
        ;;

        dgl1fg)
        hessian='--hessian'
        size=5000
        ;;
    esac
    if [ "$correct" == '--correct' ]
    then
        let "size = $size / 10"
    fi
    skip=''
    if [ "$hessian" == '--hessian' ] && [ "$onepass" == '--onepass' ]
    then
        skip='--skip'
    fi
    if [ "$implement" == 'subgraph' ] || [ "$implement" == 'subcg' ]
    then
        if [ "$reverse" == '' ] || [ "$onepass" == '--onepass' ]
        then
            skip='--skip'
        fi
        if [ "$indirect" == "--indirect" ] || [ "$colpack" == '--colpack' ]
        then
            skip='--skip'
        fi
    fi
    if [ "$implement" == 'cppad' ] || [ "$implement" == 'cppadcg' ]
    then
        if [ "$reverse" == '--reverse' ] && [ "$onepass" == '--onepass' ]
        then
            skip='--skip'
        fi
        if [ "$indirect" == "--indirect" ]
        then
            skip='--skip'
        fi
    fi
    if [ "$implement" == 'adolc' ]
    then
        if [ "$optimize" == '--optimize' ] || [ "$colpack" != '--colpack' ]
        then
            skip='--skip'
        fi
        if [ "$reverse" == '--reverse' ] && [ "$hessian" == '--hessian' ]
        then
            skip='--skip'
        fi
        if [ "$hessian" == '' ] && [ "$onepass" == '' ]
        then
            skip='--skip'
        fi
        if [ "$indirect" == "--indirect" ] && [ "$hessian" == '' ]
        then
            skip='--skip'
        fi
    fi
    if [ "$skip" == '' ]
    then
        let "count = $count + 1"
        if [ "$just_count" == 'false' ]
        then
            echo "run $count of $total_count ----------------------------------"
            run_speed $implement $problem $size $correct \
                $setup $reverse $onepass  $optimize $indirect $colpack
        fi
    fi
done
done
done
done
done
done
done
done
done
#
list='massif.err massif.out massif.tmp sparse_jac_val.so sparse_hes_val.so'
for file in $list
do
    if [ -e $file ]
    then
        echo_eval rm $file
    fi
done
# --------------------------------------------------------------------------
echo 'bin/run_speed.sh: OK'
exit 1
