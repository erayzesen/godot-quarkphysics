class_name QMeshEditorToolSelect extends QMeshEditorTool

var repositioning_particles:bool=false
var selected_particle_previous_positions=[]

func _handle_event(event:InputEvent)->bool:
	if event is InputEventMouseButton :
		#Pressed Event
		if event.is_pressed() :
			#Handling mouse left button pressed
			if event.button_index==MOUSE_BUTTON_LEFT :
				repositioning_particles=false
				#Testing solo particle selection
				var mPos=fromScreen(event.position)
				last_mouse_pressed_position=mPos
				last_mouse_motion_position=mPos
				var soloSelectedParticle:int=-1
				for i in range(meshNode.data_particle_positions.size() ) :
					var point=meshNode.data_particle_positions[i].rotated(meshNode.global_rotation)
					if ((point+meshNode.global_position)-mPos).length()<5.0 :
						soloSelectedParticle=i
						break
				if soloSelectedParticle==-1 :
					#There is no solo particle selection, start the mouse drag mode
					mouse_drag_mode=true
					selected_particle_indexes.clear()
					
					
				else :
					#There is a solo particle selection, start the repositioning mode
					if selected_particle_indexes.has(soloSelectedParticle)==false:
						selected_particle_indexes.clear()
						selected_particle_indexes.append(soloSelectedParticle)
					selected_particle_previous_positions.clear()
					for i in range(selected_particle_indexes.size()) :
						selected_particle_previous_positions.append( meshNode.data_particle_positions[selected_particle_indexes[i] ] )
					repositioning_particles=true
					mouse_drag_mode=true
					
				plugin.update_overlays()
				
					
					
		elif event.is_released() :
			#Handling mouse left button released
			if event.button_index==MOUSE_BUTTON_LEFT :
				var mPos=fromScreen(event.position)
				if repositioning_particles==true && selected_particle_indexes.size()>0 :
					var delta=selected_particle_previous_positions[0]-meshNode.data_particle_positions[selected_particle_indexes[0] ]
					var undo_redo=plugin.get_undo_redo()
					var particle_indexes_copy:PackedInt32Array=selected_particle_indexes.duplicate()
					undo_redo.create_action("Move Particle Positions by Delta: "+str(delta) )
					undo_redo.add_do_method(self,"command_move_particle_positions",plugin,meshNode,particle_indexes_copy,-delta )
					undo_redo.add_undo_method(self,"command_move_particle_positions",plugin,meshNode,particle_indexes_copy,delta )
					undo_redo.commit_action(false)
				
				#Update Radius SpinBox 	
				update_radius_spin_box()
				update_internal_check_box()
				update_enabled_check_box()
				update_lazy_check_box()
					
						
				#Clearing the selection rect and mouse drag&repositioning values	
				mouse_drag_mode=false
				repositioning_particles=false
				plugin.selection_rect.size=Vector2.ZERO
				last_mouse_released_position=mPos
				selected_particle_previous_positions.clear()
				
				
	if event is InputEventMouseMotion :
		var mPos=fromScreen(event.position)
		
		
		
		#Mouse drag mode handling
		if mouse_drag_mode==true :
			#There are selected particles and the repositioning mode is active
			if selected_particle_indexes.size()>0 && repositioning_particles :
				var delta=mPos-last_mouse_pressed_position
				if snapHelper.snap_enabled and selected_particle_indexes.size()>1 :
					delta=snap_to_grid(delta,snapHelper.snap_step,snapHelper.snap_offset)
				
				for i in range(selected_particle_indexes.size()) :
					var new_pos=selected_particle_previous_positions[i]+delta
					if snapHelper.snap_enabled and selected_particle_indexes.size()==1 :
						new_pos=snap_to_grid(new_pos,snapHelper.snap_step,snapHelper.snap_offset)
					meshNode.data_particle_positions[ selected_particle_indexes[i] ]=new_pos
					
					 
				meshNode.queue_redraw()
				plugin.update_overlays()
			#The repositioning mode is passive, mouse drag mode is using only for the creating a selection rectangle.
			if repositioning_particles==false :
				selected_particle_indexes.clear()
				#Creating mouse rect
				var rectStartX=min(last_mouse_pressed_position.x,mPos.x)
				var rectStartY=min(last_mouse_pressed_position.y,mPos.y)
				var rectEndX=max(last_mouse_pressed_position.x,mPos.x)
				var rectEndY=max(last_mouse_pressed_position.y,mPos.y)
				var rect=Rect2(Vector2(rectStartX,rectStartY),Vector2(rectEndX-rectStartX,rectEndY-rectStartY) )
				
				#Sending rect to the plugin to rendering rect 
				var plugin_rect=Rect2( toScreen(last_mouse_pressed_position),event.position-toScreen(last_mouse_pressed_position) )
				plugin.selection_rect=plugin_rect
				
				#Checking which particles is inside of the rect 
				for i in range(meshNode.data_particle_positions.size() ):
					var particle_position=meshNode.data_particle_positions[i].rotated(meshNode.global_rotation)+meshNode.global_position
					if rect.has_point(particle_position):
						selected_particle_indexes.append(i)
				plugin.update_overlays()
		#Saving last mouse position on the mouse drag mode
		last_mouse_motion_position=mPos	
	return true

func _internal_checkbox_toggled(toggled_on:bool) :
	var value:int=1 if toggled_on else 0
	if selected_particle_indexes.size()>0 :
		var particle_indexes:PackedInt32Array=selected_particle_indexes.duplicate()
		var new_values:PackedByteArray
		var old_values:PackedByteArray
		for i in range(selected_particle_indexes.size() ) :
			var p_index=particle_indexes[i]
			old_values.append(meshNode.data_particle_is_internal[p_index])
			new_values.append(value)
		var undo_redo=plugin.get_undo_redo()
		undo_redo.create_action("Set Internal Values of Particles: "+str(value) )
		undo_redo.add_undo_method(self,"command_set_particle_internal_values",plugin,meshNode,particle_indexes,old_values )
		undo_redo.add_do_method(self,"command_set_particle_internal_values",plugin,meshNode,particle_indexes,new_values )
		undo_redo.commit_action(true)
		
func _enabled_checkbox_toggled(toggled_on:bool) :
	var value:int=1 if toggled_on else 0
	if selected_particle_indexes.size()>0 :
		var particle_indexes:PackedInt32Array=selected_particle_indexes.duplicate()
		var new_values:PackedByteArray
		var old_values:PackedByteArray
		for i in range(selected_particle_indexes.size() ) :
			var p_index=particle_indexes[i]
			old_values.append(meshNode.data_particle_is_enabled[p_index])
			new_values.append(value)
		var undo_redo=plugin.get_undo_redo()
		undo_redo.create_action("Set Enabled Values of Particles: "+str(value) )
		undo_redo.add_undo_method(self,"command_set_particle_enabled_values",plugin,meshNode,particle_indexes,old_values )
		undo_redo.add_do_method(self,"command_set_particle_enabled_values",plugin,meshNode,particle_indexes,new_values )
		undo_redo.commit_action(true)
		
func _lazy_checkbox_toggled(toggled_on:bool) :
	var value:int=1 if toggled_on else 0
	if selected_particle_indexes.size()>0 :
		var particle_indexes:PackedInt32Array=selected_particle_indexes.duplicate()
		var new_values:PackedByteArray
		var old_values:PackedByteArray
		for i in range(selected_particle_indexes.size() ) :
			var p_index=particle_indexes[i]
			old_values.append(meshNode.data_particle_is_lazy[p_index])
			new_values.append(value)
		var undo_redo=plugin.get_undo_redo()
		undo_redo.create_action("Set Lazy Values of Particles: "+str(value) )
		undo_redo.add_undo_method(self,"command_set_particle_lazy_values",plugin,meshNode,particle_indexes,old_values )
		undo_redo.add_do_method(self,"command_set_particle_lazy_values",plugin,meshNode,particle_indexes,new_values )
		undo_redo.commit_action(true)
		

func _radius_bar_changed(value:float) :
	
	if selected_particle_indexes.size()>0 :
		var new_values:PackedFloat32Array
		var old_values:PackedFloat32Array
		var particle_indexes:PackedInt32Array=selected_particle_indexes.duplicate()
		for i in range(selected_particle_indexes.size() ) :
			var p_index=particle_indexes[i]
			old_values.append( meshNode.data_particle_radius[p_index] )
			new_values.append( value)
		var undo_redo=plugin.get_undo_redo()
		undo_redo.create_action("Set Radius Values of Particles: "+str(value) )
		undo_redo.add_undo_method(self,"command_set_particle_radius_values",plugin,meshNode,particle_indexes,old_values )
		undo_redo.add_do_method(self,"command_set_particle_radius_values",plugin,meshNode,particle_indexes,new_values )
		undo_redo.commit_action(true)
		
			
	pass
	
func get_common_radius_value_of_particles(particle_indexes:PackedInt32Array) ->float:
	var common_value=0.5
	if particle_indexes.size()>1 :
		for i in range(particle_indexes.size()-1) :
			var cpr=meshNode.data_particle_radius[particle_indexes[i]]
			var npr=meshNode.data_particle_radius[particle_indexes[i+1]]
			common_value=cpr
			if cpr!=npr :
				break
	elif particle_indexes.size()==1 :
		common_value=meshNode.data_particle_radius[particle_indexes[0]]
		
	return common_value
	
func get_common_internal_value_of_particles(particle_indexes:PackedInt32Array) ->bool:
	var common_value:bool=false
	if particle_indexes.size()>1 :
		for i in range(particle_indexes.size()-1) :
			var cpr=meshNode.data_particle_is_internal[particle_indexes[i]]
			var npr=meshNode.data_particle_is_internal[particle_indexes[i+1]]
			common_value=cpr
			if cpr!=npr :
				break
	elif particle_indexes.size()==1 :
		common_value=meshNode.data_particle_is_internal[particle_indexes[0]]
		
	return common_value
	
func get_common_enabled_value_of_particles(particle_indexes:PackedInt32Array) ->bool:
	var common_value:bool=false
	if particle_indexes.size()>1 :
		for i in range(particle_indexes.size()-1) :
			var cpr=meshNode.data_particle_is_enabled[particle_indexes[i]]
			var npr=meshNode.data_particle_is_enabled[particle_indexes[i+1]]
			common_value=cpr
			if cpr!=npr :
				break
	elif particle_indexes.size()==1 :
		common_value=meshNode.data_particle_is_enabled[particle_indexes[0]]
		
	return common_value
	
func get_common_lazy_value_of_particles(particle_indexes:PackedInt32Array) ->bool:
	var common_value:bool=false
	if particle_indexes.size()>1 :
		for i in range(particle_indexes.size()-1) :
			var cpr=meshNode.data_particle_is_lazy[particle_indexes[i]]
			var npr=meshNode.data_particle_is_lazy[particle_indexes[i+1]]
			common_value=cpr
			if cpr!=npr :
				break
	elif particle_indexes.size()==1 :
		common_value=meshNode.data_particle_is_lazy[particle_indexes[0]]
		
	return common_value
	
func update_radius_spin_box():
	if selected_particle_indexes.size()==0:
		radiusSpinBox.set_value_no_signal(0.5)
		radiusSpinBox.editable=false
	else :
		var common_radius_value=get_common_radius_value_of_particles(selected_particle_indexes)
		radiusSpinBox.set_value_no_signal(common_radius_value)
		radiusSpinBox.editable=true
	pass
	
func update_internal_check_box():
	if selected_particle_indexes.size()==0:
		internalCheckBox.set_pressed_no_signal(false)
		internalCheckBox.disabled=true
	else :
		var common_value=get_common_internal_value_of_particles(selected_particle_indexes)
		internalCheckBox.set_pressed_no_signal(common_value)
		internalCheckBox.disabled=false
		
func update_enabled_check_box():
	if selected_particle_indexes.size()==0:
		enabledCheckBox.set_pressed_no_signal(false)
		enabledCheckBox.disabled=true
	else :
		var common_value=get_common_enabled_value_of_particles(selected_particle_indexes)
		enabledCheckBox.set_pressed_no_signal(common_value)
		enabledCheckBox.disabled=false
		
func update_lazy_check_box():
	if selected_particle_indexes.size()==0:
		lazyCheckBox.set_pressed_no_signal(false)
		lazyCheckBox.disabled=true
	else :
		var common_value=get_common_lazy_value_of_particles(selected_particle_indexes)
		lazyCheckBox.set_pressed_no_signal(common_value)
		lazyCheckBox.disabled=false

#Tool Commands
func command_set_particle_radius_values(targetPlugin:EditorPlugin, targetMeshNode:QMeshNode, particle_indexes:PackedInt32Array, values:PackedFloat32Array) :
	for i in range(particle_indexes.size() ):
		var p_index=particle_indexes[i]
		var p_value=values[i]
		targetMeshNode.data_particle_radius[p_index]=p_value
	targetMeshNode.queue_redraw()
	targetPlugin.update_overlays()
	update_radius_spin_box()
	
func command_set_particle_internal_values(targetPlugin:EditorPlugin, targetMeshNode:QMeshNode, particle_indexes:PackedInt32Array, values:PackedByteArray) :
	for i in range(particle_indexes.size() ):
		var p_index=particle_indexes[i]
		var p_value=values[i]
		targetMeshNode.data_particle_is_internal[p_index]=p_value
	targetMeshNode.queue_redraw()
	targetPlugin.update_overlays()
	update_internal_check_box()
	
func command_set_particle_enabled_values(targetPlugin:EditorPlugin, targetMeshNode:QMeshNode, particle_indexes:PackedInt32Array, values:PackedByteArray) :
	for i in range(particle_indexes.size() ):
		var p_index=particle_indexes[i]
		var p_value=values[i]
		targetMeshNode.data_particle_is_enabled[p_index]=p_value
	targetMeshNode.queue_redraw()
	targetPlugin.update_overlays()
	update_enabled_check_box()
	
func command_set_particle_lazy_values(targetPlugin:EditorPlugin, targetMeshNode:QMeshNode, particle_indexes:PackedInt32Array, values:PackedByteArray) :
	for i in range(particle_indexes.size() ):
		var p_index=particle_indexes[i]
		var p_value=values[i]
		targetMeshNode.data_particle_is_lazy[p_index]=p_value
	targetMeshNode.queue_redraw()
	targetPlugin.update_overlays()
	update_lazy_check_box()
	

func command_move_particle_positions(targetPlugin:EditorPlugin, targetMeshNode:QMeshNode, particle_indexes:PackedInt32Array, delta:Vector2) :
	for i in range(particle_indexes.size() ):
		targetMeshNode.data_particle_positions[particle_indexes[i]]+=delta
	targetMeshNode.queue_redraw()
	targetPlugin.update_overlays()
	
