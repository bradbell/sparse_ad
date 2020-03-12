// Child table for section bin
document.write('\
<select onchange="bin_child(this)">\
<option>bin-&gt;</option>\
<option>wget_minpack2.sh</option>\
<option>f2c.sh</option>\
<option>c2hpp.py</option>\
<option>csv2tex.py</option>\
</select>\
');
function bin_child(item)
{	var child_list = [
		'wget_minpack2.sh.htm',
		'f2c.sh.htm',
		'c2hpp.py.htm',
		'csv2tex.py.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
