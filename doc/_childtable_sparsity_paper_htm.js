// Child table for section sparsity_paper
document.write('\
<select onchange="sparsity_paper_child(this)">\
<option>sparsity_paper-&gt;</option>\
<option>speed</option>\
<option>bin</option>\
<option>typedef.hpp</option>\
<option>fun_base.hpp</option>\
<option>utility</option>\
<option>_reference</option>\
<option>_index</option>\
<option>_search</option>\
<option>_external</option>\
</select>\
');
function sparsity_paper_child(item)
{	var child_list = [
		'speed.htm',
		'bin.htm',
		'typedef.hpp.htm',
		'fun_base.hpp.htm',
		'utility.htm',
		'_reference.htm',
		'_index.htm',
		'_search.htm',
		'_external.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
