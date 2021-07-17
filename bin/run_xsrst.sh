#! /bin/bash -e
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
    echo $*
    eval $*
}
# -----------------------------------------------------------------------------
if [ "$0" != "bin/run_xsrst.sh" ]
then
    echo "bin/run_xsrst.sh: must be executed from its parent directory"
    exit 1
fi
# -----------------------------------------------------------------------------
sphinx_dir='sphinx'
root_section='sparse_ad'
#
# prefix
cmd=`grep '^prefix=' bin/run_cmake.sh`
eval $cmd
echo "prefix=$prefix"
#
echo_eval $prefix/bin/xsrst.py html doc.xsrst $sphinx_dir spelling keyword
echo_eval cd $sphinx_dir
echo_eval make html
cat << EOF > _build/index.html
<html><head><script>
    window.location.href="html/xsrst/$root_section.html";
</script></head></html>
EOF
# -----------------------------------------------
echo 'run_xsrst.sh: OK'
exit 0
