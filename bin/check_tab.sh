#! /bin/bash -e
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
    echo $*
    eval $*
}
# -----------------------------------------------------------------------------
if [ "$0" != 'bin/check_tab.sh' ]
then
    echo 'must execut bin/check_tab.sh from its parent directory'
    exit 1
fi
# -----------------------------------------------------------------------------
# ignore: files created by f2c (have the form /c/name.c)
list=`git ls-files | sed \
    -e '/\/c\/[^/]*$/d' \
    -e '/\/hpp\/[^/]*$/d' \
    -e '/^sphinx\/Makefile$/d' \
`
ok='yes'
for file in $list
do
    if grep $'\t' $file > /dev/null
    then
        echo "$file has a tabs"
        read -p "Try to convert them to spaces [y/n] ?" response
        if [ "$response" == 'y' ]
        then
            echo_eval tab2space.sh $file
        elif [ "$response" != 'n' ]
        then
            echo 'response was not y or n'
            exit 1
        fi
        ok='no'
    fi
done
if [ "$ok" == 'no' ]
then
    echo 'check_tab.sh: Error'
    exit 1
fi
echo 'check_tab.sh: OK'
exit 0
