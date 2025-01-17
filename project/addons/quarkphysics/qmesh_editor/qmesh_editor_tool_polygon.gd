class_name QMeshEditorToolPolygon extends QMeshEditorTool

var repositioning_particles:bool=false



func _handle_event(event:InputEvent)->bool:
	if event is InputEventMouseButton :
		#Pressed Event
		if event.is_pressed() :
			#Handling mouse left button pressed
			var mPos=fromScreen(event.position)
			if operationOptionButton.selected==0 :	
				if event.button_index==MOUSE_BUTTON_LEFT :
					var nearest_particle_index=get_nearest_particle_index(meshNode,mPos)
					if nearest_particle_index==-1 :
						selected_particle_indexes.clear()
						make_mesh_graphics_active()
						plugin.update_overlays()
					else :
						var is_particle_exist=false
						for i in range(selected_particle_indexes.size()) :
							if nearest_particle_index==selected_particle_indexes[i] :
								is_particle_exist=true
						if is_particle_exist==true && selected_particle_indexes[0]==nearest_particle_index :
							#Setting new polygon
							var undo_redo=plugin.get_undo_redo()
							var new_polygon_indexes:PackedInt32Array=selected_particle_indexes.duplicate()
							undo_redo.create_action("Set a New Polygon Indexes: "+str(selected_particle_indexes)+"-"+str(nearest_particle_index) )
							undo_redo.add_undo_method(self,"command_set_polygon",plugin,meshNode,meshNode.data_polygon )
							undo_redo.add_do_method(self,"command_set_polygon",plugin,meshNode, new_polygon_indexes )
							undo_redo.commit_action(true)
							make_mesh_graphics_active()
							selected_particle_indexes.clear()
							meshNode.queue_redraw()
							plugin.update_overlays()
						else :
							if is_particle_exist==false:
								selected_particle_indexes.push_back(nearest_particle_index)
								make_mesh_graphics_passive()
								plugin.update_overlays()
							else :
								selected_particle_indexes.clear()
								plugin.update_overlays()
								make_mesh_graphics_active()
					last_mouse_pressed_position=mPos
					last_mouse_motion_position=mPos
					pass
			else:
				var polygon:PackedVector2Array 
				for i in range(meshNode.data_polygon.size() ):
					polygon.push_back( meshNode.data_particle_positions[meshNode.data_polygon[i] ].rotated(meshNode.global_rotation)+meshNode.global_position  )
				if Geometry2D.is_point_in_polygon(mPos,polygon) :
					var undo_redo=plugin.get_undo_redo()
					var polygon_points=meshNode.data_polygon.duplicate()
					undo_redo.create_action("Remove Polygon")
					undo_redo.add_undo_method(self,"command_set_polygon",plugin,meshNode,polygon_points )
					undo_redo.add_do_method(self,"command_set_polygon",plugin,meshNode, PackedInt32Array() )
					undo_redo.commit_action(true)
						
				
				
					
					
		elif event.is_released() :
			#Handling mouse left button released
			if event.button_index==MOUSE_BUTTON_LEFT :
				var mPos=fromScreen(event.position)
				last_mouse_released_position=mPos
				
				
	if event is InputEventMouseMotion :
		var mPos=fromScreen(event.position)
		plugin.update_overlays()
		#Mouse drag mode handling
		if mouse_drag_mode==true :
			pass
			
		#Saving last mouse position on the mouse drag mode
		last_mouse_motion_position=mPos	
	return true
	
func _handle_canvas_draw(overlay: Control):
	if selected_particle_indexes.size()>1 :
		for i in range(selected_particle_indexes.size()-1) :
			var pA=toScreen(meshNode.data_particle_positions[selected_particle_indexes[i] ].rotated(meshNode.global_rotation)+meshNode.global_position )
			var pB=toScreen(meshNode.data_particle_positions[ selected_particle_indexes[i+1] ].rotated(meshNode.global_rotation)+meshNode.global_position )
			overlay.draw_dashed_line(pA,pB,Color.GREEN,-1.0,10.0)

func _internal_checkbox_toggled(toggled_on:bool) :
	pass

func _radius_bar_changed(value:float) :
	pass

#Commands			
func command_set_polygon(targetPlugin:EditorPlugin,meshNode:QMeshNode,particleIndexes:PackedInt32Array):
	meshNode.data_polygon=particleIndexes
	meshNode.queue_redraw()
	targetPlugin.update_overlays()
	pass
