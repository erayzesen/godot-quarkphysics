class_name QMeshEditorTool extends RefCounted

var meshNode:QMeshNode
var selected_particle_indexes:PackedInt32Array=[]
#Input Variables
var mouse_drag_mode=false
var last_mouse_pressed_position=Vector2.ZERO
var last_mouse_released_position=Vector2.ZERO
var last_mouse_motion_position=Vector2.ZERO

#Plugin References
var radiusSpinBox:SpinBox
var internalCheckBox:CheckBox
var plugin:EditorPlugin

func _reset_tool() :
	mouse_drag_mode=false
	last_mouse_pressed_position=Vector2.ZERO
	last_mouse_released_position=Vector2.ZERO
	last_mouse_motion_position=Vector2.ZERO
	pass
func _handle_event(event:InputEvent)->bool:
	return false
	pass
	
func _handle_canvas_draw(overlay: Control):
	pass
	
func _internal_checkbox_toggled(toggled_on:bool) :
	pass

func _radius_bar_changed(value:float) :
	pass
	
func toScreen(point:Vector2) ->Vector2 :
	var viewport_transform=EditorInterface.get_editor_viewport_2d().get_final_transform()*EditorInterface.get_base_control().get_canvas_transform()
	return viewport_transform*point
	
func fromScreen(point:Vector2)->Vector2 :
	var viewport_transform = EditorInterface.get_editor_viewport_2d().get_final_transform() * EditorInterface.get_base_control().get_canvas_transform()
	var inverse_transform = viewport_transform.affine_inverse()
	return inverse_transform * point
	
func get_nearest_particle_index(targetMeshNode:QMeshNode,position:Vector2) ->int:
	for i in range(targetMeshNode.data_particle_positions.size() ) :
		var point=targetMeshNode.data_particle_positions[i]
		if ((point+targetMeshNode.global_position)-position).length()<5.0 :
			return i
	return -1;
	
func search_spring(targetMeshNode:QMeshNode,first_index:int,second_index:int,internal_springs:bool)->int:
	var spring_list=targetMeshNode.data_springs if internal_springs==false else targetMeshNode.data_internal_springs
	for i in range(spring_list.size()) :
		if targetMeshNode.data_springs[i][0]==first_index && targetMeshNode.data_springs[i][1]==second_index :
			return i
			
		if targetMeshNode.data_springs[i][1]==first_index && targetMeshNode.data_springs[i][0]==second_index :
			return i
	return -1
