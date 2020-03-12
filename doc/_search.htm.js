// ------------------------------------------------------------ 
// Copyright (C) Bradley M. Bell 1998-2015, All rights reserved 
// ------------------------------------------------------------ 
Keyword = 
[
{ tag: 'sparse_ad', title:'sparse_ad-20200311: Computing Sparse Jacobians and Hessians Using AD', other:' github system requirements build_type special check run speed tests' },
{ tag: 'speed', title:'Running Jacobian and Hessian Speed Tests', other:' syntax name_k=value_k method adolc cppad subgraph cppadcg subcg problem dficfj dierfj deptfg size option_list --colpack --indirect --optimize --setup --reverse --onepass --correct csv_file options nnz sec other output' },
{ tag: 'speed_global', title:'Speed Program Global Variables', other:' initialization global_problem_ptr global_size global_colpack global_indirect global_optimize global_setup global_reverse global_onepass global_correct global_x global_nnz global_correct_ok' },
{ tag: 'speed_setup', title:'Setup a Speed Test Problem', other:' syntax prototype purpose method globals initialization hessian jacobian not used' },
{ tag: 'speed_test', title:'Setup a Speed Test Problem', other:' syntax prototype jacobian hessian purpose method globals initialization global_nnz global_correct_ok' },
{ tag: 'speed_write_csv', title:'Writes One Row of the Speed Csv Output File', other:' syntax prototype csv_file header name value method problem options nnz sec' },
{ tag: 'bin', title:'Executable Scripts', other:' working directory' },
{ tag: 'wget_minpack2.sh', title:'Get a Minpack2 File From Argonne National Laboratory', other:' syntax working directory result example' },
{ tag: 'f2c.sh', title:'Create a C Version of a Fortran File', other:' syntax working directory name result example' },
{ tag: 'c2hpp.py', title:'Create a C++ Template Version of an f2c File', other:' syntax working directory name result warning example' },
{ tag: 'csv2tex.py', title:'Extract Latex Tables From Speed Test Csv File', other:' syntax output files subset problem' },
{ tag: 'typedef.hpp', title:'Type Definitions', other:' syntax scalar types ftnlen adolc_double cppad_double cg_double cppadcg_double vector d_vector c_vector s_vector b_vector adolc_vector cppad_vector cg_vector cppadcg_vector' },
{ tag: 'fun_base.hpp', title:'Base Class For Test Function Objects', other:' syntax purpose fun_obj constructor type definitions start x_out size_range m fun_out jac ok jac_out grad grad_out hes hes_out' },
{ tag: 'utility', title:'Utility Routines', other:'' },
{ tag: 'check_sparse', title:'Compare A CppAD Sparse Matrix and a Dense Matrix', other:' syntax prototype sparse_matrix dense_matrix print_label print_done ok' },
{ tag: 'adolc2sparse_rcv', title:'Convert An Adolc Sparse Matrix to CppAD Form', other:' syntax prototype nr nc nnz rind cind values sparse_matrix' },
{ tag: 'prt_sparse', title:'Print A CppAD Sparsity Pattern or Sparse Matrix', other:' syntax prototype name' },
{ tag: 'upper_triangle', title:'Extract Upper Triangle From A CppAD Sparsity Pattern', other:' syntax prototype' },
{ tag: 'cppadcg_library', title:'CppADCodeGen Dynamic Libraries', other:' syntax prototype fun_name other_fun cg_fun file_name swap' }
]

var MaxList = 100;
var Nstring = -1;
var Nkeyword = Keyword.length;
var Row2Tag  = [];
Initialize();

function Initialize()
{
	UpdateList();
	document.search.keywords.focus();
}
function UpdateList()
{
	var string  = document.search.keywords.value;
	if( Nstring == string.length )
		return;
	Nstring     = string.length;

	var word    = string.match(/\S+/g);
	var nword   = 0;
	if(word != null )
		nword   = word.length;

	var pattern = new Array(nword);
	for(var j = 0; j < nword; j++)
		pattern[j] = new RegExp(word[j], 'i');

	var nlist       = 0;
	var title_list  = '';
	var tag_list    = '';
	for(i = 0; (i < Nkeyword) && (nlist < MaxList) ; i++)
	{
		var match = true;
		for(j = 0; j < nword; j++)
		{	var flag = pattern[j].test(Keyword[i].tag);
			flag     = flag || pattern[j].test(Keyword[i].title);
			flag     = flag || pattern[j].test(Keyword[i].other);
			match    = match && flag;
		}
		if( match )
		{
			var tag    = Keyword[i].tag;
			var title  = Keyword[i].title
			title      = title.split(/\s+/);
			title      = title.join(' ');
			title_list = title_list + title + '\n';
			tag_list   = tag_list + tag + '\n'
			Row2Tag[nlist] = tag;
			nlist = nlist + 1;
		}
	}
	document.search.title_list.value = title_list;
	document.search.title_list.setAttribute('wrap', 'off');;
	document.search.title_list.readOnly = true;
	document.search.tag_list.value = tag_list;
	document.search.tag_list.setAttribute('wrap', 'off');;
	document.search.tag_list.readOnly = true;
}
function Choose(textarea)
{	var start_select = textarea.value.substring(0, textarea.selectionStart);
	var start_pos    = Math.max(0, start_select.lastIndexOf('\n') + 1);
	var length       = textarea.value.length;
	var end_select   = 
		textarea.value.substring(textarea.selectionEnd, length);
	var end_pos  = end_select.indexOf('\n');
	end_pos      = textarea.selectionEnd + end_pos;
	textarea.selectionStart = start_pos;
	textarea.selectionEnd   = end_pos;
	var row = start_select.split('\n').length - 1;
	var tag = Row2Tag[row];
	document.search.selection.value    = tag.toLowerCase();
	document.search.selection.readOnly = true;
	
	return true;
}
function Goto()
{  selection       = document.search.selection.value;
   if( selection != '' )
	    parent.location = selection + '.htm';
}
function CheckForReturn()
{
	var key = event.which;
	if( key == 13 ) Goto();
}
