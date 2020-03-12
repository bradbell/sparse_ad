// Child table for section utility
document.write('\
<select onchange="utility_child(this)">\
<option>utility-&gt;</option>\
<option>check_sparse</option>\
<option>adolc2sparse_rcv</option>\
<option>prt_sparse</option>\
<option>upper_triangle</option>\
<option>cppadcg_library</option>\
</select>\
');
function utility_child(item)
{	var child_list = [
		'check_sparse.htm',
		'adolc2sparse_rcv.htm',
		'prt_sparse.htm',
		'upper_triangle.htm',
		'cppadcg_library.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
