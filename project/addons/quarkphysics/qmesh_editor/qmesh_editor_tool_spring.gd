class_name QMeshEditorToolSpring extends QMeshEditorTool

var repositioning_particles:bool=false

var first_selected_particle_index:int=-1

func _handle_event(event:InputEvent)->bool:
	if event is InputEventMouseButton :
		#Pressed Event
		if event.is_pressed() :
			#Handling mouse left button pressed
			if event.button_index==MOUSE_BUTTON_LEFT :
				var mPos=fromScreen(event.position)
				if operationOptionButton.selected==0 :
					if first_selected_particle_index!=-1 :
						var nearest_particle_index=get_nearest_particle_index(meshNode,mPos)
						if nearest_particle_index!=-1 && first_selected_particle_index!=-1:
							if nearest_particle_index!=first_selected_particle_index :
								#Checking particle pair of the spring whether QMeshNode exist
								var matched_spring=search_spring(meshNode,first_selected_particle_index,nearest_particle_index,false)
								if matched_spring==-1 :
									var matched_internal_spring=search_spring(meshNode,first_selected_particle_index,nearest_particle_index,true)
									if matched_internal_spring==-1:
										#The particle pair doesn't exist in QMeshNode
										var undo_redo=plugin.get_undo_redo()
										undo_redo.create_action("Add a New Spring Between Particles: "+str(first_selected_particle_index)+"-"+str(nearest_particle_index) )
										undo_redo.add_do_method(self,"command_add_spring",plugin,meshNode,first_selected_particle_index,nearest_particle_index,internalCheckBox.button_pressed )
										undo_redo.add_undo_method(self,"command_remove_spring",plugin,meshNode,first_selected_particle_index,nearest_particle_index )
										undo_redo.commit_action(true)
						make_mesh_graphics_active()
						
						first_selected_particle_index=-1
						
					else :
						var nearest_particle_index=get_nearest_particle_index(meshNode, mPos)
						first_selected_particle_index=nearest_particle_index
						last_mouse_pressed_position=mPos
						last_mouse_motion_position=mPos
						if nearest_particle_index!=-1 :
							make_mesh_graphics_passive()
						else :
							make_mesh_graphics_active()
					
				else :
					var nearest_spring_index=get_nearest_spring_index(meshNode,mPos,internalCheckBox.button_pressed)
					if nearest_spring_index!=-1:
						var springs=meshNode.data_internal_springs if internalCheckBox.button_pressed else meshNode.data_springs
						var pa_index=springs[nearest_spring_index][0]
						var pb_index=springs[nearest_spring_index][1]
						var undo_redo=plugin.get_undo_redo()
						undo_redo.create_action("Remove a Spring Indexed: "+str(nearest_spring_index) )
						undo_redo.add_do_method(self,"command_remove_spring_at",plugin,meshNode,nearest_spring_index,internalCheckBox.button_pressed )
						undo_redo.add_undo_method(self,"command_add_spring",plugin,meshNode,pa_index,pb_index,internalCheckBox.button_pressed )
						undo_redo.commit_action(true)
				
				
					
					
		elif event.is_released() :
			#Handling mouse left button released
			if event.button_index==MOUSE_BUTTON_LEFT :
				var mPos=fromScreen(event.position)
				last_mouse_released_position=mPos
				
				
	if event is InputEventMouseMotion :
		var mPos=fromScreen(event.position)
		#Mouse drag mode handling
		if mouse_drag_mode==true :
			pass
		plugin.update_overlays()
			
		#Saving last mouse position on the mouse drag mode
		last_mouse_motion_position=mPos	
	return true
	
func _handle_canvas_draw(overlay: Control):
	if first_selected_particle_index!=-1 :
		var first_pos:Vector2=toScreen( meshNode.data_particle_positions[first_selected_particle_index].rotated(meshNode.global_rotation)+meshNode.global_position )
		var second_pos:Vector2=toScreen( last_mouse_motion_position )
		overlay.draw_dashed_line(first_pos,second_pos,Color.GRAY,-1.0,10.0)
	pass

func _internal_checkbox_toggled(toggled_on:bool) :
	pass

func _radius_bar_changed(value:float) :
	pass
#Commands
func command_add_spring(targetPlugin:EditorPlugin,targetMeshNode:QMeshNode,first_particle_index:int,second_particle_index:int,is_internal:bool):
	#Adding new spring to QMeshNode
	var spring:PackedInt32Array=[first_particle_index,second_particle_index]
	if is_internal :
		var springs=targetMeshNode.data_internal_springs
		springs.push_back( spring )
		targetMeshNode.data_internal_springs=springs
	else :
		var springs=targetMeshNode.data_springs
		springs.push_back( spring )
		targetMeshNode.data_springs=springs
		
	targetMeshNode.queue_redraw()
	targetPlugin.update_overlays()
	
	
func command_remove_spring(targetPlugin:EditorPlugin,targetMeshNode:QMeshNode,first_particle_index:int,second_particle_index:int):
	#Checking particle pair of the spring whether QMeshNode exist
	var matched_spring=search_spring(targetMeshNode,first_particle_index,second_particle_index,false)
	if matched_spring!=-1 :
		var springs=targetMeshNode.data_springs.duplicate()
		springs.remove_at(matched_spring)
		targetMeshNode.data_springs=springs
	var matched_internal_spring=search_spring(targetMeshNode,first_particle_index,second_particle_index,true)
	if matched_internal_spring!=-1 :
		var springs=targetMeshNode.data_internal_springs.duplicate()
		springs.remove_at(matched_internal_spring)
		targetMeshNode.data_internal_springs=springs
		
	print ("matched_spring:"+str(matched_spring))
	print ("matched_internal_spring:"+str(matched_internal_spring))
	
	targetMeshNode.queue_redraw()
	targetPlugin.update_overlays()
	
func command_remove_spring_at(targetPlugin:EditorPlugin,targetMeshNode:QMeshNode,spring_index:int,internal:bool):
	var springs=targetMeshNode.data_internal_springs.duplicate(true) if internal else targetMeshNode.data_springs.duplicate(true)
	springs.remove_at(spring_index)
	if internal :
		targetMeshNode.data_internal_springs=springs
	else :
		targetMeshNode.data_springs=springs
	
	
	targetMeshNode.queue_redraw()
	targetPlugin.update_overlays()
