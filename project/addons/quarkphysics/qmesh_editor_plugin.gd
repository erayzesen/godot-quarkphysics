@tool
extends EditorPlugin

var meshNode:QMeshNode

var editor_tool:QMeshEditorTool

var selection_rect:Rect2

var edit_mode=false

var tool_bar
	
func _edit(object: Object) -> void:
	meshNode=object
	update_overlays()
	if object is QMeshNode :
		reset_plugin()
		
		#Adding a tool bar to the top panel
		tool_bar=preload("res://addons/quarkphysics/qmesh_editor/qmesh_editor_tool_bar.tscn").instantiate()
		tool_bar.get_node("EditButton").connect("toggled",on_tool_bar_edit_pressed)
		var button_group:ButtonGroup=tool_bar.get_node("ParticleButton").button_group
		button_group.connect("pressed",on_tool_button_pressed)
		add_control_to_container(EditorPlugin.CONTAINER_CANVAS_EDITOR_MENU,tool_bar)

	else :
		editor_tool=null
		#print("deleted tool bar")
		reset_plugin()
	pass
	

	
func _handles(object: Object) -> bool:
	return object is QMeshAdvancedNode
	
func _make_visible(visible: bool) -> void:
	update_overlays()
	
func _forward_canvas_draw_over_viewport(overlay: Control) -> void:
	if meshNode is not QMeshNode || meshNode.is_inside_tree()==false :
		return
		
	if edit_mode==false:
		return
	
	for i in range(meshNode.data_particle_positions.size()):
		var circlePos=toScreen( meshNode.global_position+meshNode.data_particle_positions[i] )
		var circleRadius=5
		var color=Color.GREEN
		if editor_tool.selected_particle_indexes.has(i) :
			color=Color.WHITE
		overlay.draw_circle(circlePos,circleRadius,color,false )
	if selection_rect.size!=Vector2.ZERO :
		overlay.draw_rect(selection_rect,Color.WHITE,false)
		
	editor_tool._handle_canvas_draw(overlay)
		
func _forward_canvas_gui_input(event: InputEvent) -> bool:
	if meshNode==null || meshNode is not QMeshNode:
		return false
	if edit_mode==false:
		return false
		
	if event is not InputEventMouse :
		return false
		
	var res=editor_tool._handle_event(event)
	return res

	
func _enter_tree() -> void:
	# Initialization of the plugin goes here.
	pass
	
func on_tool_bar_edit_pressed(toggled_on:bool):
	if toggled_on :
		edit_mode=true
		tool_bar.get_node("SelectButton").visible=true
		tool_bar.get_node("ParticleButton").visible=true
		tool_bar.get_node("SpringButton").visible=true
		tool_bar.get_node("PolygonButton").visible=true
		tool_bar.get_node("UVButton").visible=true
		on_tool_button_pressed(tool_bar.get_node("SelectButton"))
		var select_button:Button=tool_bar.get_node("SelectButton")
		select_button.button_pressed=true
		update_overlays()
	else :
		edit_mode=false
		tool_bar.get_node("SelectButton").visible=false
		tool_bar.get_node("ParticleButton").visible=false
		tool_bar.get_node("SpringButton").visible=false
		tool_bar.get_node("PolygonButton").visible=false
		tool_bar.get_node("UVButton").visible=false
		tool_bar.get_node("InternalCheckBox").visible=false
		tool_bar.get_node("RadiusBar").visible=false
		tool_bar.get_node("OperationOptionButton").visible=false
		update_overlays()
	pass
	
func on_tool_button_pressed(button: BaseButton) :
	var select_button=tool_bar.get_node("SelectButton")
	var particle_button=tool_bar.get_node("ParticleButton")
	var spring_button=tool_bar.get_node("SpringButton")
	var polygon_button=tool_bar.get_node("PolygonButton")
	var uv_button=tool_bar.get_node("UVButton")
	match button:
		select_button:
			editor_tool=QMeshEditorToolSelect.new()
			tool_bar.get_node("InternalCheckBox").visible=true
			tool_bar.get_node("RadiusBar").visible=true
			tool_bar.get_node("OperationOptionButton").visible=false
			tool_bar.get_node("InternalCheckBox").connect("toggled",editor_tool._internal_checkbox_toggled)
			tool_bar.get_node("RadiusBar/SpinBox").connect("value_changed",editor_tool._radius_bar_changed)
			tool_bar.get_node("InternalCheckBox").disabled=true
		particle_button:
			editor_tool=QMeshEditorToolParticle.new()
			tool_bar.get_node("InternalCheckBox").visible=true
			tool_bar.get_node("RadiusBar").visible=false
			tool_bar.get_node("OperationOptionButton").visible=true
			tool_bar.get_node("OperationOptionButton").selected=0
			tool_bar.get_node("InternalCheckBox").disabled=false
		spring_button:
			editor_tool=QMeshEditorToolSpring.new()
			tool_bar.get_node("InternalCheckBox").visible=true
			tool_bar.get_node("RadiusBar").visible=false
			tool_bar.get_node("OperationOptionButton").visible=true
			tool_bar.get_node("OperationOptionButton").selected=0
			tool_bar.get_node("InternalCheckBox").disabled=false
		polygon_button:
			editor_tool=QMeshEditorToolPolygon.new()
			tool_bar.get_node("InternalCheckBox").visible=false
			tool_bar.get_node("RadiusBar").visible=false
			tool_bar.get_node("OperationOptionButton").visible=true
			tool_bar.get_node("OperationOptionButton").selected=0
		uv_button:
			editor_tool=QMeshEditorToolUV.new()
			tool_bar.get_node("InternalCheckBox").visible=false
			tool_bar.get_node("RadiusBar").visible=false
			tool_bar.get_node("OperationOptionButton").visible=true
			tool_bar.get_node("OperationOptionButton").selected=0
	editor_tool.plugin=self
	editor_tool.meshNode=meshNode
	editor_tool.radiusSpinBox=tool_bar.get_node("RadiusBar/SpinBox")
	editor_tool.internalCheckBox=tool_bar.get_node("InternalCheckBox")
	editor_tool.operationOptionButton=tool_bar.get_node("OperationOptionButton")
	editor_tool.radiusSpinBox.editable=false
	
	
	#print("changed editor tool button with:"+button.name)
	update_overlays()
	
func reset_plugin() :
	if tool_bar :
		remove_control_from_container(EditorPlugin.CONTAINER_CANVAS_EDITOR_MENU,tool_bar)
		tool_bar=null
	edit_mode=false
	update_overlays()
	
func toScreen(point:Vector2) ->Vector2 :
	var viewport_transform=EditorInterface.get_editor_viewport_2d().get_final_transform()*EditorInterface.get_base_control().get_canvas_transform()
	return viewport_transform*point
	
func fromScreen(point:Vector2)->Vector2 :
	var viewport_transform = EditorInterface.get_editor_viewport_2d().get_final_transform() * EditorInterface.get_base_control().get_canvas_transform()
	var inverse_transform = viewport_transform.affine_inverse()
	return inverse_transform * point


func _exit_tree() -> void:
	if tool_bar :
		remove_control_from_container(EditorPlugin.CONTAINER_CANVAS_EDITOR_MENU,tool_bar)
		tool_bar=null
	pass
