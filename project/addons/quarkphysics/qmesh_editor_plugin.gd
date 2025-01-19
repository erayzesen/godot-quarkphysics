@tool
extends EditorPlugin

var meshNode:QMeshNode

var editor_tool:QMeshEditorTool

var selection_rect:Rect2

var edit_mode=false

var tool_bar

var hovered_particle_index:int=-1

var particle_select_range:float=15.0

#Colors
var PARTICLE_DEFAULT=Color.LIME_GREEN
var PARTICLE_INTERNAL=Color.DARK_SEA_GREEN

	
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
		if editor_tool!=null :
			editor_tool.make_mesh_graphics_active()
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
		
	var screen_rect=overlay.get_rect()
	var edit_bg_color=Color.BLACK
	edit_bg_color.a=0.3
	overlay.draw_rect(screen_rect,edit_bg_color)
	
	
	for i in range(meshNode.data_particle_positions.size()):
		var particle_pos=meshNode.data_particle_positions[i]
		particle_pos=particle_pos.rotated(meshNode.global_rotation)
		var circlePos=toScreen( meshNode.global_position+particle_pos )
		var circleRadius=5
		var color=PARTICLE_DEFAULT
		var particle_is_disabled=false
		var particle_is_lazy=false
		var particle_is_internal=false
		if i<meshNode.data_particle_is_lazy.size() :
			if meshNode.data_particle_is_lazy[i]==1 :
				particle_is_lazy=true
		if i<meshNode.data_particle_is_enabled.size() :
			if meshNode.data_particle_is_enabled[i]==0 :
				particle_is_disabled=true
		if i<meshNode.data_particle_is_internal.size() :
			if meshNode.data_particle_is_internal[i]==1 :
				particle_is_internal=true
				color=PARTICLE_INTERNAL
		var outline_width=-1
		if editor_tool.selected_particle_indexes.has(i) :
			overlay.draw_circle(circlePos,circleRadius,color,true )
			color=Color.WHITE
		if i==hovered_particle_index :
			overlay.draw_circle(circlePos,circleRadius,color,true )
			outline_width=2
			
		overlay.draw_circle(circlePos,circleRadius,color,false,outline_width )
		if particle_is_disabled :
			overlay.draw_circle(circlePos,2.0,color,true )
		elif particle_is_lazy:
			overlay.draw_circle(circlePos,2.0,color,false )
		
	if selection_rect.size!=Vector2.ZERO :
		overlay.draw_rect(selection_rect,Color.WHITE,false)
		
	if hovered_particle_index!=-1 :
		if hovered_particle_index<meshNode.data_particle_positions.size() :
			var pos=meshNode.data_particle_positions[hovered_particle_index].rotated(meshNode.global_rotation)+meshNode.global_position
			
			pos=toScreen(pos)
			pos+=Vector2.UP*16
			pos+=Vector2.LEFT*12
			var font=ThemeDB.get_default_theme().default_font
			overlay.draw_string_outline(font,pos,str(hovered_particle_index),HORIZONTAL_ALIGNMENT_CENTER,24,16,8,Color.BLACK,TextServer.JUSTIFICATION_CONSTRAIN_ELLIPSIS)
			
			#overlay.draw_string(font,pos+Vector2.DOWN*1,str(hovered_particle_index),HORIZONTAL_ALIGNMENT_CENTER,-1,16,Color.BLACK )
			overlay.draw_string(font,pos,str(hovered_particle_index),HORIZONTAL_ALIGNMENT_CENTER,24,16,Color.WHITE,TextServer.JUSTIFICATION_CONSTRAIN_ELLIPSIS )
		else :
			hovered_particle_index=-1
		
		
	editor_tool._handle_canvas_draw(overlay)
		
func _forward_canvas_gui_input(event: InputEvent) -> bool:
	if meshNode==null || meshNode is not QMeshNode:
		return false
	if edit_mode==false:
		return false
		
	if event is not InputEventMouse :
		return false
	#Mouse Hovering 
	if event is InputEventMouseMotion :
		var mPos=fromScreen(event.position)
		check_hovered_particles(mPos,meshNode)
	
	var res=editor_tool._handle_event(event)
	return res

	
func _enter_tree() -> void:
	# Initialization of the plugin goes here.
	pass
	
func check_hovered_particles(mPos:Vector2, targetMeshNode:QMeshNode) :
	hovered_particle_index=-1
	var min_dist=INF
	for i in range(targetMeshNode.data_particle_positions.size()) :
		var p:Vector2=targetMeshNode.data_particle_positions[i]
		var distVec:Vector2=mPos-(p.rotated(targetMeshNode.global_rotation)+meshNode.global_position)
		var dist=distVec.length()
		if dist<particle_select_range && dist<min_dist :
			min_dist=dist
			hovered_particle_index=i
	update_overlays()
	
func on_tool_bar_edit_pressed(toggled_on:bool):
	if toggled_on :
		edit_mode=true
		QMeshEditorTool.solve_faulty_particle_data_issues(meshNode)
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
		if editor_tool!=null :
			editor_tool.make_mesh_graphics_active()
		tool_bar.get_node("SelectButton").visible=false
		tool_bar.get_node("ParticleButton").visible=false
		tool_bar.get_node("SpringButton").visible=false
		tool_bar.get_node("PolygonButton").visible=false
		tool_bar.get_node("UVButton").visible=false
		tool_bar.get_node("EnabledCheckBox").visible=false
		tool_bar.get_node("InternalCheckBox").visible=false
		tool_bar.get_node("LazyCheckBox").visible=false
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
	#Remove Connections and Set Default Feature Values 
	if editor_tool!=null :
		var control:CheckBox=tool_bar.get_node("EnabledCheckBox")
		remove_signal_connections(tool_bar.get_node("EnabledCheckBox"),"toggled")
		remove_signal_connections(tool_bar.get_node("InternalCheckBox"),"toggled")
		remove_signal_connections(tool_bar.get_node("LazyCheckBox"),"toggled")
		remove_signal_connections(tool_bar.get_node("RadiusBar/SpinBox"),"value_changed")
		tool_bar.get_node("EnabledCheckBox").button_pressed=false
		tool_bar.get_node("InternalCheckBox").button_pressed=false
		tool_bar.get_node("LazyCheckBox").button_pressed=false
		
		
	match button:
		select_button:
			editor_tool=QMeshEditorToolSelect.new()
			tool_bar.get_node("EnabledCheckBox").visible=true
			tool_bar.get_node("InternalCheckBox").visible=true
			tool_bar.get_node("LazyCheckBox").visible=true
			tool_bar.get_node("RadiusBar").visible=true
			tool_bar.get_node("OperationOptionButton").visible=false
			tool_bar.get_node("EnabledCheckBox").connect("toggled",editor_tool._enabled_checkbox_toggled)
			tool_bar.get_node("InternalCheckBox").connect("toggled",editor_tool._internal_checkbox_toggled)
			tool_bar.get_node("LazyCheckBox").connect("toggled",editor_tool._lazy_checkbox_toggled)
			tool_bar.get_node("RadiusBar/SpinBox").connect("value_changed",editor_tool._radius_bar_changed)
			tool_bar.get_node("InternalCheckBox").disabled=true
			tool_bar.get_node("EnabledCheckBox").disabled=true
			tool_bar.get_node("LazyCheckBox").disabled=true
		particle_button:
			editor_tool=QMeshEditorToolParticle.new()
			tool_bar.get_node("EnabledCheckBox").visible=false
			tool_bar.get_node("InternalCheckBox").visible=true
			tool_bar.get_node("LazyCheckBox").visible=false
			tool_bar.get_node("RadiusBar").visible=false
			tool_bar.get_node("OperationOptionButton").visible=true
			tool_bar.get_node("OperationOptionButton").selected=0
			tool_bar.get_node("InternalCheckBox").disabled=false
		spring_button:
			editor_tool=QMeshEditorToolSpring.new()
			tool_bar.get_node("EnabledCheckBox").visible=false
			tool_bar.get_node("InternalCheckBox").visible=true
			tool_bar.get_node("LazyCheckBox").visible=false
			tool_bar.get_node("RadiusBar").visible=false
			tool_bar.get_node("OperationOptionButton").visible=true
			tool_bar.get_node("OperationOptionButton").selected=0
			tool_bar.get_node("InternalCheckBox").disabled=false
		polygon_button:
			editor_tool=QMeshEditorToolPolygon.new()
			tool_bar.get_node("EnabledCheckBox").visible=false
			tool_bar.get_node("InternalCheckBox").visible=false
			tool_bar.get_node("LazyCheckBox").visible=false
			tool_bar.get_node("RadiusBar").visible=false
			tool_bar.get_node("OperationOptionButton").visible=true
			tool_bar.get_node("OperationOptionButton").selected=0
		uv_button:
			editor_tool=QMeshEditorToolUV.new()
			tool_bar.get_node("EnabledCheckBox").visible=false
			tool_bar.get_node("InternalCheckBox").visible=false
			tool_bar.get_node("LazyCheckBox").visible=false
			tool_bar.get_node("RadiusBar").visible=false
			tool_bar.get_node("OperationOptionButton").visible=true
			tool_bar.get_node("OperationOptionButton").selected=0
	editor_tool.plugin=self
	editor_tool.meshNode=meshNode
	editor_tool.radiusSpinBox=tool_bar.get_node("RadiusBar/SpinBox")
	editor_tool.enabledCheckBox=tool_bar.get_node("EnabledCheckBox")
	editor_tool.internalCheckBox=tool_bar.get_node("InternalCheckBox")
	editor_tool.lazyCheckBox=tool_bar.get_node("LazyCheckBox")
	editor_tool.operationOptionButton=tool_bar.get_node("OperationOptionButton")
	editor_tool.radiusSpinBox.editable=false
	editor_tool.selected_particle_indexes.clear()
	editor_tool.mesh_node_alpha_temp=meshNode.modulate.a
	
	
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
	
func remove_signal_connections(target:Control,signal_name:String):
	var connections=target.get_signal_connection_list(signal_name)
	for connection in connections :
		target.disconnect(signal_name,connection["callable"])
	pass


func _exit_tree() -> void:
	if tool_bar :
		remove_control_from_container(EditorPlugin.CONTAINER_CANVAS_EDITOR_MENU,tool_bar)
		tool_bar=null
	pass
