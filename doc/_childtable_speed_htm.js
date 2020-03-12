// Child table for section speed
document.write('\
<select onchange="speed_child(this)">\
<option>speed-&gt;</option>\
<option>speed_global</option>\
<option>speed_setup</option>\
<option>speed_test</option>\
<option>speed_write_csv</option>\
</select>\
');
function speed_child(item)
{	var child_list = [
		'speed_global.htm',
		'speed_setup.htm',
		'speed_test.htm',
		'speed_write_csv.htm'
	];
	var index = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = child_list[index-1];
}
