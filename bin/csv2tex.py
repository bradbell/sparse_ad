#! /usr/bin/python3
'''
{xsrst_begin csv2tex_py}

.. include:: ../preamble.rst

{xsrst_spell
    py
}

Extract Latex Tables From Speed Test Csv File
#############################################

Syntax
******
``bin/csv2tex.py`` *csv_file*

Csv File
********
Each line of this input file corresponds to one run of the
:ref:`speed<speed>` program.

Output Files
************

Subset
======
Each one is a latex table that corresponds to a subset of the input data
where *problem* , and *setup* have one value that corresponds
to the file name.
For each subset, only the combination of options that has the smallest
value in the *sec* column is included.

Problem
=======
The possible value for *problem* are
``deptfg`` , ``dierfj`` , ``dficfj`` .
We use *Problem* to denote the *problem*
with the first letter in upper case; e.g.,
``Deptfg`` is the *Problem* corresponding to ``deptfg`` .

Files
=====
The output files are written in the ``build/tex`` directory.
For each of the possible problems, the output file
*Problem*\ ``YesSetup.tex`` ( *Problem*\ ``NoSetup.tex`` )
corresponds to *setup* true (false).

{xsrst_end csv2tex_py}
'''
# -----------------------------------------------------------------------------
import sys
import csv
import re
#
usage = 'usage: bin/csv2tex.py csv_file'
if len( sys.argv ) != 2 :
    sys.exit(usage)
#
csv_file = sys.argv[1]
# -----------------------------------------------------------------------------
# read data
csvfile = open(csv_file, 'r')
reader  = csv.DictReader(csvfile)
data    = list()
for row in reader :
    data.append(row)
# -----------------------------------------------------------------------------
# separate camel case words
def space_camel_case(text) :
    pattern  = r'([a-z])([A-Z])'
    replace  = r'\1 \2'
    return re.sub(pattern, replace, text)
# -----------------------------------------------------------------------------
def write_table(table_name, restrict) :
    #
    # col_list, n_col
    header    = 'KB,method,problem,setup,'
    header   += 'colpack,indirect,optimize,reverse,onepass,'
    header   += 'n,m,nnz,sec'
    col_list  = header.split(',')
    n_col     = len(col_list)
    #
    # row_list
    # only include rows that satisfy the restriction condition
    row_list = list()
    for row in data :
        # make sure not taking time for correctness test
        assert row['correct'] == 'false'
        #
        include = True
        for col in restrict :
            include = include and row[col] in restrict[col]
        if include :
            row_list.append(row)
    #
    # row_sorted
    # sort row_list by sec (time)
    row_sorted = sorted(row_list, key= lambda row: float(row['sec']) )
    #
    # row_list
    # for each method, only include row with smallest (best) time
    method_list = set()
    row_list    = list()
    for row in row_sorted :
         if row['method'] not in method_list :
            method_list.add( row['method'] )
            row_list.append( row )
    #
    # constant_col
    constant_col = n_col * [ True ]
    row_zero  = row_list[0]
    for row in row_list :
        for j in range(n_col) :
            key = col_list[j]
            constant_col[j] = constant_col[j] and row[key] == row_zero[key]
    #
    # variable_col, n_variable
    variable_col = list()
    for j in range(n_col) :
        if not constant_col[j] :
            variable_col.append( col_list[j] )
    n_variable = len(variable_col)
    #
    # open output file
    file_name  = 'build/tex/' + table_name + '.tex'
    tex_file = open(file_name, 'w')
    #
    # caption
    caption = ''
    for j in range(n_col) :
        if constant_col[j] :
            last_constant = j
    for j in range(n_col) :
        if constant_col[j] :
            name  = col_list[j]
            value = row_zero[name]
            text  = '\\textit{' + name + '}=\\texttt{' + value + '}'
            if j < last_constant :
                text += ',\n'
            else :
                text += '\n'
            caption += text
    #
    # \begin{table}[tbhp]
    # { \footnotesize
    # \caption{caption}
    # \label{Tabletable_name}
    # \begin{center}
    # \begin{tabular}{r...r}
    text  = '{ \\footnotesize\n'
    text += '\\caption{' + caption + '}\n'
    text += '\\label{Table' + table_name + '}\n'
    text += '\\begin{center}\n'
    text += '\\begin{tabular}{' + ( n_variable * 'r' ) + '}\n'
    tex_file.write(text)
    #
    # name_1 & ... & name_p \\
    text = ''
    for j in range(n_variable) :
        if j > 0 :
            text += ' & '
        text += variable_col[j]
    text += '\\\\\n'
    tex_file.write( text )
    n_row      = len(row_list)
    for i in range(n_row) :
        row  = row_list[i]
        text = ''
        for j in range(n_variable) :
            name  = variable_col[j]
            value = row[name]
            if name == 'sec' :
                value = "{0:.7f}".format( float(value) )
            if j > 0 :
                text += ' & '
            text += value
        if i < n_row - 1 :
            text += ' \\\\'
        text += '\n'
        tex_file.write(text)
    #
    # \end{tabular}
    # \end{center}
    # }
    # \end{table}
    text  = '\\end{tabular}\n'
    text += '\\end{center}\n'
    text += '}\n'
    tex_file.write(text)
    #
    tex_file.close()
# -----------------------------------------------------------------------------
# for Problem = Dficfj, Dierfj, Deptfg create the tables
#   ProblemNoSetup.tex and ProblemYesSetup.tex
for problem in [ 'dficfj', 'dierfj', 'deptfg', 'dgl1fg' ] :
    #
    # problem with no setup
    restrict = {
        'problem'  : [ problem ]  ,
        'correct'  : [ 'false'  ] ,
        'setup'    : [ 'false'  ] ,
    }
    Problem = problem[0].upper() + problem[1:]
    table_name = Problem + 'NoSetup'
    write_table(table_name, restrict)
    #
    # problem with setup
    restrict[ 'setup' ] = [ 'true' ]
    table_name = Problem + 'YesSetup'
    write_table(table_name, restrict)
#
# -----------------------------------------------------------------------------
print('bin/csv2tex.py: OK\n')
sys.exit(0)
