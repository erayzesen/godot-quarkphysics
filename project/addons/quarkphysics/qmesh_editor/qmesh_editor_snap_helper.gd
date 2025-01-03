class_name QMeshSnapHelper extends RefCounted

#These Helper Functions were taken from: @bitwes - https://github.com/godotengine/godot-proposals/issues/10529#issuecomment-2501936526 
var found_all_editor_controls = false
var snap_enabled: bool = false
var snap_step : Vector2 = Vector2.ZERO
var snap_offset: Vector2 = Vector2.ZERO

var editor_controls = {
	dialog = null,
	grid_offset_x = null,
	grid_offset_y = null,
	grid_snap_x = null,
	grid_snap_y = null,
	snap_toggle = null
}


func _init():
	if(Engine.is_editor_hint()):
		setup_controls()
		update_values_from_editor()
		
		
		
		



func setup_controls():
	var main_screen = EditorInterface.get_editor_main_screen()
	editor_controls.snap_toggle = safe_get_descendant(main_screen, ["HBoxContainer", 12], Button)

	var snap_dia_grid_container = safe_get_descendant(main_screen, ["SnapDialog", "GridContainer"], GridContainer)
	editor_controls.grid_offset_x = safe_get_child(snap_dia_grid_container, 1, SpinBox)
	editor_controls.grid_offset_y = safe_get_child(snap_dia_grid_container, 2, SpinBox)
	editor_controls.grid_snap_x = safe_get_child(snap_dia_grid_container, 4, SpinBox)
	editor_controls.grid_snap_y = safe_get_child(snap_dia_grid_container, 5, SpinBox)
	editor_controls.dialog = find_child_with_name_containing(main_screen, "SnapDialog", ConfirmationDialog)

	if(null in editor_controls.values()):
		push_error("One or more Editor Controls could not be found.")
		return
	else:
		found_all_editor_controls = true

	editor_controls.snap_toggle.toggled.connect(func(tog: bool) -> void: snap_enabled = tog)
	editor_controls.dialog.get_ok_button().pressed.connect(update_values_from_editor)


func update_values_from_editor():
	if(!found_all_editor_controls):
		return
	snap_enabled = editor_controls.snap_toggle.button_pressed
	snap_offset.x = editor_controls.grid_offset_x.value
	snap_offset.y = editor_controls.grid_offset_y.value
	snap_step.x = editor_controls.grid_snap_x.value
	snap_step.y = editor_controls.grid_snap_y.value

func find_child_with_name_containing(parent, child_name, expected_type = null):
	var to_return = parent.find_child("*%s*" % child_name, true, false)

	if(to_return == null):
		push_error("Could not find child named ", child_name, " in ", parent)
	elif(expected_type != null and not is_instance_of(to_return, expected_type)):
		push_error("Expected ", to_return, ' to be an instance of ', expected_type)
		to_return = null

	return to_return
	
func safe_get_child(parent, index, expected_type = null):
	if(parent != null and index < parent.get_child_count()):
		var found = parent.get_child(index)
		if(expected_type != null and not is_instance_of(found, expected_type)):
			push_error("Expected ", found, ' to be an instance of ', expected_type)
			found = null
		return found
	else:
		push_error(str("Could not get child index ", index, " on ", parent, '.  ', parent, ' has ', parent.get_child_count(), ' children.'))
		return null
		
func safe_get_descendant(parent, names_and_or_indexes, expected_type = null):
	var here = parent
	var index = 0

	while(here != null and index < names_and_or_indexes.size()):
		var entry = names_and_or_indexes[index]
		if(typeof(entry) == TYPE_STRING):
			here = find_child_with_name_containing(here, entry)
		elif(typeof(entry) == TYPE_INT):
			here = safe_get_child(here, entry)
		index += 1

	if(here != null and expected_type != null and not is_instance_of(here, expected_type)):
		push_error("Expected ", here, ' to be an instance of ', expected_type)
		here = null

	return here
	
var descendant_props_to_print = ['text', 'value']

func print_descendants(parent, indent='', child_index = -1):
	var text = ""
	if(child_index != -1):
		text = str(indent, '[', child_index, '] ', parent)
	else:
		text = str(indent, parent)

	for ptp in descendant_props_to_print:
		if(parent.get(ptp) != null):
			text += str(' ', ptp, ' = ', parent.get(ptp))
	print(indent, text)

	for i in range(parent.get_child_count()):
		print_descendants(parent.get_child(i), indent + '  ', i)
