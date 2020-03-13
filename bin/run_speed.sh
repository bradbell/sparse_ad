#! /bin/bash -e
# vim: set expandtab:
method_list='adolc cppad subgraph cppadcg subcg'
problem_list='dficfj dierfj deptfg dgl1fg'
# --------------------------------------------------------------------------
echo_eval() {
    echo $*
    eval $*
}
# --------------------------------------------------------------------------
top_srcdir=`pwd`
run_speed() {
    echo "$top_srcdir/bin/massif.sh ./speed dummy.csv $*"
    n_bytes=`$top_srcdir/bin/massif.sh ./speed dummy.csv $*`
    echo "KB = $n_bytes / 1000"
    KB=`echo "$n_bytes 1000" | awk '{print int($1 / $2 + 0.5)}'`
    echo "./speed $csv_file KB=$KB $*"
    ./speed $csv_file KB=$KB $*
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
This creates the file build/src/speed/name-mmmdd.csv with results
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
mmdd=`date +%m%d`
csv_file="$name-$mmdd.csv"
if [ -e build/src/speed/$csv_file ]
then
    echo 'bin/run_speed.sh: The output file already exists, remove it ?'
    echo "    rm build/src/speed/$csv_file"
    exit 1
fi
# --------------------------------------------------------------------------
# set build_type as specified at top of this file
sed -i bin/run_cmake.sh -e "s|^build_type=.*|build_type='$build_type'|"
bin/run_cmake.sh
cd build/src/speed
make speed
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
for problem in $problem_list
do
for setup in --setup   ''
do
for reverse in --reverse ''
do
for onepass in --onepass  ''
do
for method in $method_list
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
    if [ "$method" == 'subgraph' ] || [ "$method" == 'subcg' ]
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
    if [ "$method" == 'cppad' ] || [ "$method" == 'cppadcg' ]
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
    if [ "$method" == 'adolc' ]
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
            run_speed $method $problem $size $correct \
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
