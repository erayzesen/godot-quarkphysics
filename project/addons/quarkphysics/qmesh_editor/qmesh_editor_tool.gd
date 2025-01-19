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
var enabledCheckBox:CheckBox
var internalCheckBox:CheckBox
var lazyCheckBox:CheckBox
var operationOptionButton:OptionButton
var plugin:EditorPlugin

#Mesh Node Temp
var mesh_node_alpha_temp:float=1
var mesh_node_editing_alpha:float=0.3

#Helpers
var snapHelper:QMeshSnapHelper=QMeshSnapHelper.new()


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

func _enabled_checkbox_toggled(toggled_on:bool) :
	pass
	
func _internal_checkbox_toggled(toggled_on:bool) :
	pass
	
func _lazy_checkbox_toggled(toggled_on:bool) :
	pass

func _radius_bar_changed(value:float) :
	pass
	
func make_mesh_graphics_passive() :
	make_mesh_graphics_active()
	mesh_node_alpha_temp=meshNode.modulate.a
	meshNode.modulate.a=mesh_node_editing_alpha
	
func make_mesh_graphics_active() :
	meshNode.modulate.a=mesh_node_alpha_temp
	

func toScreen(point:Vector2) ->Vector2 :
	var viewport_transform=EditorInterface.get_editor_viewport_2d().get_final_transform()*EditorInterface.get_base_control().get_canvas_transform()
	return viewport_transform*point
	
func fromScreen(point:Vector2)->Vector2 :
	var viewport_transform = EditorInterface.get_editor_viewport_2d().get_final_transform() * EditorInterface.get_base_control().get_canvas_transform()
	var inverse_transform = viewport_transform.affine_inverse()
	return inverse_transform * point
	
func get_nearest_particle_index(targetMeshNode:QMeshNode,position:Vector2) ->int:
	var min_dist=INF
	var finded_index=-1
	for i in range(targetMeshNode.data_particle_positions.size() ) :
		var point=targetMeshNode.data_particle_positions[i].rotated(meshNode.global_rotation)
		var dist=((point+targetMeshNode.global_position)-position).length()
		if dist<plugin.particle_select_range && dist<min_dist:
			min_dist=dist
			finded_index=i
			
	return finded_index;
	
func get_nearest_spring_index(targetMeshNode:QMeshNode, position:Vector2, internal:bool)->int :
	var min_distance:float=INF
	var search_range=3.0
	var spring_index=-1
	var springs=targetMeshNode.data_internal_springs if internal else targetMeshNode.data_springs  
	for i in range(springs.size()) :
		var spring=springs[i]
		var pA=targetMeshNode.data_particle_positions[ spring[0] ].rotated(meshNode.global_rotation)+meshNode.global_position
		var pB=targetMeshNode.data_particle_positions[ spring[1] ].rotated(meshNode.global_rotation)+meshNode.global_position
		var a_vec=(pB-pA)
		var unit=a_vec.normalized()
		var normal=unit.orthogonal()
		var b_vec=position-pA
		var proj=b_vec.dot(unit)
		if proj>=0 && proj<=a_vec.length() :
			var distance=abs( b_vec.dot(normal) )
			if  distance<search_range :
				if distance<min_distance :
					spring_index=i
					min_distance=distance
				
	
	return spring_index
	
func snap_to_grid(point:Vector2,snap_step:Vector2,snap_offset:Vector2) ->Vector2:
	var snapped_x = roundf(point.x / snap_step.x) * snap_step.x
	var snapped_y = roundf(point.y / snap_step.y) * snap_step.y
	var res = Vector2(snapped_x, snapped_y) + snap_offset
	return res
		
	
func get_uv_map_index_at_position(targetMeshNode:QMeshNode, position:Vector2) -> int:
	for i in range(targetMeshNode.data_uv_maps.size()) :
		var map=targetMeshNode.data_uv_maps[i]
		var uv_poly:PackedVector2Array
		for j in range(map.size()):
			uv_poly.push_back( targetMeshNode.data_particle_positions[ map[j] ].rotated(targetMeshNode.global_rotation)+targetMeshNode.global_position )
		if Geometry2D.is_point_in_polygon(position,uv_poly) :
			return i
	return -1
		
	pass
	
	
func search_spring(targetMeshNode:QMeshNode,first_index:int,second_index:int,internal_springs:bool)->int:
	var spring_list=targetMeshNode.data_springs if internal_springs==false else targetMeshNode.data_internal_springs
	for i in range(spring_list.size()) :
		if spring_list[i][0]==first_index && spring_list[i][1]==second_index :
			return i
			
		if spring_list[i][1]==first_index && spring_list[i][0]==second_index :
			return i
	return -1

func is_particle_exist_in_springs(targetMeshNode:QMeshNode,particleIndex:int, internal:bool)->bool:
	var springs=targetMeshNode.data_springs if internal==false else targetMeshNode.data_internal_springs
	for i in range(springs.size()):
		var spring=springs[i]
		if spring[0]==particleIndex || spring[1]==particleIndex :
			return true
	return false
	
func is_particle_exist_in_polygon(targetMeshNode:QMeshNode,particleIndex:int)->bool:
	for i in range(targetMeshNode.data_polygon.size() ):
		if targetMeshNode.data_polygon[i]==particleIndex :
			return true
	return false
func is_particle_exist_in_uv(targetMeshNode:QMeshNode,particleIndex:int)->bool:
	for i in range(targetMeshNode.data_uv_maps.size() ):
		var map=targetMeshNode.data_uv_maps[i]
		for j in range(map.size()) :
			if map[j]==particleIndex :
				return true
	return false
	
static func solve_faulty_particle_data_issues(targetMeshNode:QMeshNode) :
	var particleCount=targetMeshNode.data_particle_positions.size()
	# SOLVING MISSING FEATURES
	#Check Particle Enabled Values
	if targetMeshNode.data_particle_is_enabled.size()<particleCount :
		var temp_collection=targetMeshNode.data_particle_is_enabled.duplicate()
		for i in range(temp_collection.size(),particleCount) :
			temp_collection.append(true)
		targetMeshNode.data_particle_is_enabled=temp_collection
	#Check Particle Internal Values
	if targetMeshNode.data_particle_is_internal.size()<particleCount :
		var temp_collection=targetMeshNode.data_particle_is_internal.duplicate()
		for i in range(temp_collection.size(),particleCount) :
			temp_collection.append(false)
		targetMeshNode.data_particle_is_internal=temp_collection
	
	#Check Particle Lazy Values
	if targetMeshNode.data_particle_is_lazy.size()<particleCount :
		var temp_collection=targetMeshNode.data_particle_is_lazy.duplicate()
		for i in range(temp_collection.size(),particleCount) :
			temp_collection.append(false)
		targetMeshNode.data_particle_is_lazy=temp_collection
		
	#Check Particle Radius Values
	if targetMeshNode.data_particle_radius.size()<particleCount :
		var temp_collection=targetMeshNode.data_particle_radius.duplicate()
		for i in range(temp_collection.size(),particleCount) :
			temp_collection.append(0.5)
		targetMeshNode.data_particle_radius=temp_collection
		
	#SOLVING OVERAGE FEATURES
	#Check Particle Enabled Values
	while targetMeshNode.data_particle_is_enabled.size()>targetMeshNode.data_particle_positions.size() :
		var temp=targetMeshNode.data_particle_is_enabled
		temp.remove_at(temp.size()-1)
		targetMeshNode.data_particle_is_enabled=temp
	#Check Particle Radius Values
	while targetMeshNode.data_particle_radius.size()>targetMeshNode.data_particle_positions.size() :
		var temp=targetMeshNode.data_particle_radius
		temp.remove_at(temp.size()-1)
		targetMeshNode.data_particle_radius=temp
	#Check Particle Internal Values
	while targetMeshNode.data_particle_is_internal.size()>targetMeshNode.data_particle_positions.size() :
		var temp=targetMeshNode.data_particle_is_internal
		temp.remove_at(temp.size()-1)
		targetMeshNode.data_particle_is_internal=temp
	#Check Particle Lazy Values
	while targetMeshNode.data_particle_is_lazy.size()>targetMeshNode.data_particle_positions.size() :
		var temp=targetMeshNode.data_particle_is_lazy
		temp.remove_at(temp.size()-1)
		targetMeshNode.data_particle_is_lazy=temp
	
