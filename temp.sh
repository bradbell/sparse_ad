#! /bin/bash -e
git checkout bin/run_speed.sh
list='
    bin/run_speed.sh
    src/speed/speed.xsrst
    bin/devel.sh
    doc.xsrst
    bin/bin.xsrst
'
for file in $list
do
    echo "git show master:$file > $file"
    git show master:$file > $file
done
#
sed -i bin/run_speed.sh \
    -e 's|\*build_type\* --correct|*build_type* ``--correct``|' \
    -e 's|``build/src/speed/`` \*name\* - \*mmdd\* ``.csv``|``build/src/speed/``\\ *name-mmdd*\\ ``.csv``|'
#
sed -i bin/devel.sh \
    -e '/set expand_tab/d'
#
sed -i doc.xsrst \
    -e 's|``build`` . \*build_type\*|``build.``\\ *build_type*|' \
    -e 's|-- ``correct``|``--correct``|' \
    -e 's|correct`` - \*mmdd\*|correct-``\\ *mmdd*|' \
    -e 's|speed`` - \*mmdd\*|speed-``\\ *mmdd*|'
#
sed -i src/speed/speed.xsrst \
    -e 's|\\$|\\\\|'
#
bin/run_xsrst.sh
echo 'temp.sh: OK'
exit 0
