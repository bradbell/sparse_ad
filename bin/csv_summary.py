#! /bin/python3
#
# {xsrst_comment_ch #}
#
# {xsrst_begin csv_summary_py}
#
# .. include:: ../preamble.rst
#
# {xsrst_spell
#   py
#   onepass
#   nnz
#   true true
#   false false
# }
#
# Summarize A CSV File's Speed Results
# ####################################
#
# Syntax
# ******
#
# | ``bin/csv_summary.py`` *csv_file* *problem* *setup*
#
#
# csv_file
# ********
# This is name of a csv file created buy :ref:`run_speed.sh<run_speed_sh>`.
# It must begin with ``csv/``; i.e., it must be in the ``csv`` directory.
# It must end with ``.csv``.
#
# problem
# *******
# This is the :ref:`speed.problem` we are summarizing the results for and
# must be one of the following:
# ``dficfj``, ``dierfj``, ``deptfg``, ``dgl1fg``.
#
# setup
# *****
# This must be ``true`` or ``false`` and indicates in the
# :ref:``speed.--setup`` time is included for this summary.
#
# Example Output
# **************
# Below is example output by this program (where the seconds column has been
# truncated to 3 after the decimal::
#
#   problem    = deptfg
#   setup      = false
#   onepass    = false
#   correct    = false
#   n          = 3600
#   m          = 1
#   KB        implement colpack   indirect  optimize  reverse   nnz       sec
#   6199      subgraph  false     false     true      true      10680     0.011
#   33603     cppad     true      false     true      true      10680     0.018
#   15059     adolc     true      true      false     false     14161     0.038
#
# {xsrst_end csv_summary_py}
# -----------------------------------------------------------------------------
import sys
import csv
# -----------------------------------------------------------------------------
if sys.argv[0] != 'bin/csv_summary.py' :
    msg = 'must execut bin/csv_summary.py from its parent directory'
    sys.exit(msg)
# -----------------------------------------------------------------------------
# problem_set
problem_set = { 'dficfj', 'dierfj', 'deptfg', 'dgl1fg' }
# -----------------------------------------------------------------------------
# csv_file, problem, setup
if len(sys.argv) != 4 :
    msg   = 'usage: bin/csv_summary.py csv_file problem setup'
    msg  += '\nSummarizes bin/run_speed.sh results for one problem'
    sys.exit(msg)
csv_file = sys.argv[1]
problem  = sys.argv[2]
setup    = sys.argv[3]
#
if not csv_file.startswith('csv/') :
    msg = 'csv_summary.py: csv_file does not begin with csv/'
    sys.exit(msg)
if not csv_file.endswith('.csv') :
    msg = 'csv_summary.py: csv_file does not end with .csv'
    sys.exit(msg)
if not problem in problem_set :
    msg  = 'csv_summary.py: problem not one of following:\n'
    msg += ', '.join(problem_set)
    sys.exit(msg)
if not setup in [ 'true', 'false' ] :
    msg = 'csv_summary.py: setup is not true or false'
    sys.exit(msg)
# -----------------------------------------------------------------------------
# selected_data
fp        = open(csv_file, 'r')
reader    = csv.DictReader(fp)
selected_data  = list()
for row in reader :
    assert row['setup'] in [ 'true', 'false' ]
    assert row['problem'] in problem_set
    if row['setup'] == setup and row['problem'] == problem :
        selected_data.append(row)
fp.close()
# -----------------------------------------------------------------------------
# sorted_data
sorted_data = sorted(selected_data, key = lambda row : float(row['sec']) )
# -----------------------------------------------------------------------------
# fastest_data
implement_set = set()
fastest_data  = list()
for row in sorted_data :
    if not row['implement'] in implement_set :
        fastest_data.append(row)
        implement_set.add( row['implement'] )
# -----------------------------------------------------------------------------
# constant_column, variable_column
constant_column = list()
variable_column = list()
for key in sorted_data[0] :
    constant = True
    value    = fastest_data[0][key]
    for row in fastest_data :
        constant = constant and row[key] == value
    if constant :
        constant_column.append(key)
    else :
        variable_column.append(key)
# -----------------------------------------------------------------------------
# print fastest_data
for key in constant_column :
    line = '{:10} = {}'.format(key, fastest_data[0][key] )
    print(line)
line = ''
for key in variable_column :
    line += '{:10}'.format(key)
print(line)
for row in fastest_data :
    line = ''
    for key in variable_column :
        line += '{:10}'.format( row[key] )
    print(line)
# -----------------------------------------------------------------------------
print('csv_summary.py: OK')
sys.exit(0)
