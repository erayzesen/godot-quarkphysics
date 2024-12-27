class_name QMeshEditorToolParticle extends QMeshEditorTool


func _handle_event(event:InputEvent)->bool:
	if event is InputEventMouseButton :
		#Pressed Event
		if event.is_pressed() :
			#Handling mouse left button pressed
			if event.button_index==MOUSE_BUTTON_LEFT :
				var mPos=fromScreen(event.position)
				last_mouse_pressed_position=mPos
				last_mouse_motion_position=mPos
				var undo_redo=plugin.get_undo_redo()
				undo_redo.create_action("Add a Particle to Position: "+str(mPos) )
				undo_redo.add_do_method(self,"command_add_particle",plugin,meshNode,mPos-meshNode.global_position,false )
				undo_redo.add_undo_method(self,"command_remove_particle",plugin,meshNode,meshNode.data_particle_positions.size() )
				undo_redo.commit_action(false)
				command_add_particle(plugin,meshNode,mPos-meshNode.global_position,false)
				pass
				
				
					
					
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
			
		#Saving last mouse position on the mouse drag mode
		last_mouse_motion_position=mPos	
	return true
	
func _internal_checkbox_toggled(toggled_on:bool) :
	pass

func _radius_bar_changed(value:float) :
	pass
	
#Commands

func command_add_particle(targetPlugin:EditorPlugin,targetMeshNode:QMeshNode, position:Vector2,is_internal:bool):
	var particles=targetMeshNode.data_particle_positions
	particles.push_back(position)
	targetMeshNode.data_particle_positions=particles
	var particle_radius=targetMeshNode.data_particle_radius
	particle_radius.append(0.5)
	targetMeshNode.data_particle_radius=particle_radius
	var particles_is_internal=targetMeshNode.data_particle_is_internal
	particles_is_internal.push_back(is_internal)
	targetMeshNode.data_particle_is_internal=particles_is_internal
	targetMeshNode.queue_redraw()
	targetPlugin.update_overlays()
	pass
	
func command_remove_particle(targetPlugin:EditorPlugin,targetMeshNode:QMeshNode, particle_index:int):
	var particles=targetMeshNode.data_particle_positions
	particles.remove_at(particle_index)
	targetMeshNode.data_particle_positions=particles
	var particle_radius=targetMeshNode.data_particle_radius
	particle_radius.remove_at(particle_index)
	targetMeshNode.data_particle_radius=particle_radius
	var particles_is_internal=targetMeshNode.data_particle_is_internal
	particles_is_internal.remove_at(particle_index)
	targetMeshNode.data_particle_is_internal=particles_is_internal
	targetMeshNode.queue_redraw()
	targetPlugin.update_overlays()
	pass
