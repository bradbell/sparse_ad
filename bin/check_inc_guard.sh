#! /bin/bash -e
# -----------------------------------------------------------------------------
if [ "$0" != 'bin/check_inc_guard.sh' ]
then
    echo 'must execut bin/check_inc_guard.sh from its parent directory'
    exit 1
fi
# -----------------------------------------------------------------------------
list=`git ls-files *.hpp`
found_error='no'
for file_name in $list
do
    guard_name=`sed -n -e '/^# *ifndef *[A-Z0-9_]*_HPP/p' $file_name |
        sed -e 's|^# *ifndef *||'`
    check_name=`echo $file_name | tr [a-z/.] [A-Z__]`
    if [ "$guard_name" != "$check_name" ]
    then
        if [ "$found_error" == 'no' ]
        then
            echo 'bin/check_inc_guard.sh: Erros in following files:'
            found_error='yes'
        fi
        echo $file_name
    fi
done
if [ "$found_error" == 'yes' ]
then
    exit 1
fi
# -----------------------------------------------------------------------------
echo 'bin/check_inc_guard.sh: OK'
exit 0
