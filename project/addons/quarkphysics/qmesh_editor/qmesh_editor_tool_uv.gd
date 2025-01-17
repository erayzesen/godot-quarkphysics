class_name QMeshEditorToolUV extends QMeshEditorTool

var repositioning_particles:bool=false

var uv_debug_colors:PackedColorArray=[Color.YELLOW,Color.ORANGE_RED,Color.CADET_BLUE,Color.GREEN_YELLOW,Color.MEDIUM_ORCHID,Color.MEDIUM_PURPLE]

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
							#Adding new UV
							var undo_redo=plugin.get_undo_redo()
							var new_uv_indexes:PackedInt32Array=selected_particle_indexes.duplicate()
							var uv_maps_size= meshNode.data_uv_maps.size()
							undo_redo.create_action("Add a New UV Map: "+str(selected_particle_indexes) )
							undo_redo.add_undo_method(self,"command_remove_uv_at",plugin,meshNode,uv_maps_size )
							undo_redo.add_do_method(self,"command_add_uv",plugin,meshNode, new_uv_indexes )
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
			else :
				var target_uv_map_index:int=get_uv_map_index_at_position(meshNode,mPos)	
				if target_uv_map_index!=-1 :
					var uv_map_backup=meshNode.data_uv_maps[target_uv_map_index].duplicate()
					var undo_redo=plugin.get_undo_redo()
					undo_redo.create_action("Remove UV Map Indexed: "+str(target_uv_map_index) )
					undo_redo.add_undo_method(self,"command_add_uv",plugin,meshNode, uv_map_backup )
					undo_redo.add_do_method(self,"command_remove_uv_at",plugin,meshNode,target_uv_map_index )
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
	#Draw UV Maps
	for i in range(meshNode.data_uv_maps.size()) :
		var uv_map=meshNode.data_uv_maps[i]
		var polygon:PackedVector2Array
		for n in range(uv_map.size() ) :
			var pos=meshNode.data_particle_positions[ uv_map[n] ].rotated(meshNode.global_rotation)+meshNode.global_position
			pos=toScreen(pos)
			polygon.push_back( pos )
		var color=uv_debug_colors[i%uv_debug_colors.size()]
		color.a=0.3
		overlay.draw_polygon(polygon,[color])

	var uv_line_color=uv_debug_colors[ (meshNode.data_uv_maps.size())%uv_debug_colors.size() ]
	
	if selected_particle_indexes.size()>1 :
		for i in range(selected_particle_indexes.size()-1) :
			var pA=toScreen(meshNode.data_particle_positions[selected_particle_indexes[i] ].rotated(meshNode.global_rotation)+meshNode.global_position )
			var pB=toScreen(meshNode.data_particle_positions[ selected_particle_indexes[i+1] ].rotated(meshNode.global_rotation)+meshNode.global_position )
			overlay.draw_dashed_line(pA,pB,uv_line_color,-1.0,10.0)
	if selected_particle_indexes.size()>0:
		var first_pos:Vector2=meshNode.data_particle_positions[selected_particle_indexes[selected_particle_indexes.size()-1]].rotated(meshNode.global_rotation)+meshNode.global_position
		first_pos=toScreen(first_pos)
		var second_pos:Vector2=toScreen( last_mouse_motion_position )
		overlay.draw_dashed_line(first_pos,second_pos,uv_line_color,-1.0,10.0)
	
	
func _internal_checkbox_toggled(toggled_on:bool) :
	pass

func _radius_bar_changed(value:float) :
	pass
#Commands 
func command_add_uv(targetPlugin:EditorPlugin,targetMeshNode:QMeshNode,particleIndexes:PackedInt32Array):
	var uv_maps=targetMeshNode.data_uv_maps.duplicate()
	uv_maps.push_back(particleIndexes)
	targetMeshNode.data_uv_maps=uv_maps
	targetMeshNode.queue_redraw()
	targetPlugin.update_overlays()
	pass
	
func command_remove_uv_at(targetPlugin:EditorPlugin,targetMeshNode:QMeshNode,UVIndex:int):
	var uv_maps=targetMeshNode.data_uv_maps.duplicate()
	print("uv index:"+str(UVIndex))
	uv_maps.remove_at(UVIndex)
	targetMeshNode.data_uv_maps=uv_maps
	targetMeshNode.queue_redraw()
	targetPlugin.update_overlays()
	pass
