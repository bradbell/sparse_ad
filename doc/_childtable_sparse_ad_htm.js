// Child table for section sparse_ad
document.write('\
<select onchange="sparse_ad_child(this)">\
<option>sparse_ad-&gt;</option>\
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
function sparse_ad_child(item)
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
