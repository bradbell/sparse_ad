#! /bin/bash -e
# This program can be run in any directory
# ----------------------------------------------------------------------------
if [ "$1" == '' ]
then
    echo 'usage: massif.sh program arguments'
    echo ''
    echo 'massif.err:   massif standard error output'
    echo 'massif.out:   massif standard output file'
    echo 'stdout:       average heap space size in bytes'
    echo 'This program can be exucted in any directory'
    exit 1
fi
for file in massif.err massif.out massif.prt
do
    if [ -e $file ]
    then
        rm $file
    fi
done
valgrind --tool=massif --massif-out-file=massif.out $* >& massif.err
#
n_sum=`grep '^mem_heap_B=' massif.out | wc -l`
sum=`grep '^mem_heap_B=' massif.out | \
    sed -e 's|^mem_heap_B=|+|' | \
    tr '\n' ' ' | \
    sed -e 's| ||g' -e 's|^+|(|' -e 's|$|)|'`
echo "$sum / $n_sum" | bc
exit 0
