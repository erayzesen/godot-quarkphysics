class_name QMeshEditorToolParticle extends QMeshEditorTool


func _handle_event(event:InputEvent)->bool:
	if event is InputEventMouseButton :
		#Pressed Event
		if event.is_pressed() :
			#Handling mouse left button pressed
			if event.button_index==MOUSE_BUTTON_LEFT :
				var mPos=fromScreen(event.position)
				if operationOptionButton.selected==0 :
					var new_particle_global_pos=mPos
					if snapHelper.snap_enabled :
						new_particle_global_pos=snap_to_grid(new_particle_global_pos,snapHelper.snap_step,snapHelper.snap_offset)
					var new_particle_local_pos=(new_particle_global_pos-meshNode.global_position).rotated(-meshNode.global_rotation)
					#Add a new particle
					var undo_redo=plugin.get_undo_redo()
					undo_redo.create_action("Add a Particle to Position: "+str(mPos) )
					undo_redo.add_do_method(self,"command_add_particle",plugin,meshNode,new_particle_local_pos,0.5,internalCheckBox.button_pressed )
					undo_redo.add_undo_method(self,"command_remove_particle",plugin,meshNode,meshNode.data_particle_positions.size() )
					undo_redo.commit_action(true)
					
				else :
					#Remove a particle
					var nearest_particle_index=get_nearest_particle_index(meshNode,mPos)
					if nearest_particle_index!=-1 :
						var old_springs=[]
						var old_internal_springs=[]
						var old_polygon=[]
						var old_uv_maps=[]
						if is_particle_exist_in_springs(meshNode,nearest_particle_index,false) :
							old_springs=meshNode.data_springs.duplicate(true)
						if is_particle_exist_in_springs(meshNode,nearest_particle_index,true) :
							old_internal_springs=meshNode.data_internal_springs.duplicate(true)
						if is_particle_exist_in_polygon(meshNode,nearest_particle_index):
							old_polygon=meshNode.data_polygon.duplicate()
						if is_particle_exist_in_uv(meshNode,nearest_particle_index) :
							old_uv_maps=meshNode.data_uv_maps.duplicate(true)
						var target_particle_position=meshNode.data_particle_positions[nearest_particle_index]
						var target_particle_radius=meshNode.data_particle_radius[nearest_particle_index]
						var target_particle_internal=meshNode.data_particle_is_internal[nearest_particle_index]
						var target_particle_enabled=meshNode.data_particle_is_enabled[nearest_particle_index]
						var target_particle_lazy=meshNode.data_particle_is_lazy[nearest_particle_index]
						var undo_redo=plugin.get_undo_redo()
						
						undo_redo.create_action("Remove Particle Indexed: "+str(nearest_particle_index) )
						undo_redo.add_do_method(self,"command_remove_particle",plugin,meshNode,nearest_particle_index )
						undo_redo.add_undo_method(self,"command_add_particle",plugin,meshNode,target_particle_position,target_particle_radius,target_particle_internal,target_particle_enabled,target_particle_lazy,nearest_particle_index, old_springs,old_internal_springs,old_polygon,old_uv_maps )
						undo_redo.commit_action(true)
						pass
					pass
				
				
				last_mouse_pressed_position=mPos
				last_mouse_motion_position=mPos
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

func command_add_particle(targetPlugin:EditorPlugin,targetMeshNode:QMeshNode, position:Vector2,radius:float,is_internal:bool=false,is_enabled:bool=true,is_lazy:bool=false,array_position:int=-1, setter_springs:Array=[], setter_internal_springs:Array=[],setter_polygon:PackedInt32Array=[],setter_uv_maps:Array=[]):
	var particles=targetMeshNode.data_particle_positions.duplicate()
	if array_position==-1 :
		particles.push_back(position)
	else :
		particles.insert(array_position,position)
		#Update All Data Indexes 
		#Update Springs
		for i in range(targetMeshNode.data_springs.size()) :
			if targetMeshNode.data_springs[i][0]>=array_position :
				targetMeshNode.data_springs[i][0]+=1
			if targetMeshNode.data_springs[i][1]>=array_position :
				targetMeshNode.data_springs[i][1]+=1
		#Update Internal Springs
		for i in range(targetMeshNode.data_internal_springs.size()) :
			if targetMeshNode.data_internal_springs[i][0]>=array_position :
				targetMeshNode.data_internal_springs[i][0]+=1
			if targetMeshNode.data_internal_springs[i][1]>=array_position :
				targetMeshNode.data_internal_springs[i][1]+=1
		#Update Polygon
		for i in range(targetMeshNode.data_polygon.size()) :
			if targetMeshNode.data_polygon[i]>=array_position :
				targetMeshNode.data_polygon[i]+=1
		#Update UV Maps
		for i in range(targetMeshNode.data_uv_maps.size()) :
			var map:PackedInt32Array=targetMeshNode.data_uv_maps[i]
			for j in range(map.size()):
				if map[j]>=array_position :
					targetMeshNode.data_uv_maps[i][j]+=1
				
	targetMeshNode.data_particle_positions=particles
	#radius
	var particle_radius=targetMeshNode.data_particle_radius
	if array_position==-1 :
		particle_radius.append(radius)
	else :
		particle_radius.insert(array_position,radius)
	targetMeshNode.data_particle_radius=particle_radius
	#internal
	var particles_is_internal=targetMeshNode.data_particle_is_internal
	if array_position==-1 :
		particles_is_internal.append(is_internal)
	else :
		particles_is_internal.insert(array_position,is_internal)
	targetMeshNode.data_particle_is_internal=particles_is_internal
	#enabled
	var particles_is_enabled=targetMeshNode.data_particle_is_enabled
	if array_position==-1 :
		particles_is_enabled.append(is_enabled)
	else :
		particles_is_enabled.insert(array_position,is_enabled)
	targetMeshNode.data_particle_is_enabled=particles_is_enabled
	#lazy
	var particles_is_lazy=targetMeshNode.data_particle_is_lazy
	if array_position==-1 :
		particles_is_lazy.append(is_lazy)
	else :
		particles_is_lazy.insert(array_position,is_lazy)
	targetMeshNode.data_particle_is_lazy=particles_is_lazy
	
	if setter_springs.size()>0 :
		targetMeshNode.data_springs=setter_springs
	if setter_internal_springs.size()>0 :
		targetMeshNode.data_internal_springs=setter_internal_springs
	if setter_polygon.size()>0 :
		targetMeshNode.data_polygon=setter_polygon
	if setter_uv_maps.size()>0 :
		targetMeshNode.data_uv_maps=setter_uv_maps
	
	targetMeshNode.queue_redraw()
	targetPlugin.update_overlays()
	pass
	
func command_remove_particle(targetPlugin:EditorPlugin,targetMeshNode:QMeshNode, particle_index:int):
	#Checking Springs 
	var i=0
	var modified=false
	var new_spring_list=targetMeshNode.data_springs.duplicate(true)
	while(i<new_spring_list.size() ):
		var spring=new_spring_list[i]
		if spring[0]==particle_index || spring[1]==particle_index:
			modified=true
			new_spring_list.remove_at(i)
			print("deleted spring:" +str(i) )
		else :
			if spring[0]>particle_index :
				new_spring_list[i][0]-=1
				modified=true
			if spring[1]>particle_index :
				new_spring_list[i][1]-=1
				modified=true
			i+=1
	if modified :
		targetMeshNode.data_springs=new_spring_list
	
	#Checking Internal Springs 
	i=0
	modified=false
	var new_internal_spring_list=targetMeshNode.data_internal_springs.duplicate(true)
	
	while(i<new_internal_spring_list.size() ):
		var spring=new_internal_spring_list[i]
		if spring[0]==particle_index || spring[1]==particle_index:
			modified=true
			new_internal_spring_list.remove_at(i)
			print("deleted internal spring:" +str(i) )
		else :
			if spring[0]>particle_index :
				new_internal_spring_list[i][0]-=1
				modified=true
			if spring[1]>particle_index :
				new_internal_spring_list[i][1]-=1
				modified=true
			i+=1
	if modified :
		targetMeshNode.data_internal_springs=new_internal_spring_list
	
	#Checking Polygon 
	i=0
	modified=false
	var new_polygon=targetMeshNode.data_polygon.duplicate()
	
	while(i<new_polygon.size() ):
		var p=new_polygon[i]
		if p==particle_index:
			modified=true
			new_polygon.remove_at(i)
			print("deleted polygon point:" +str(i) )
		else :
			if p>particle_index :
				new_polygon[i]-=1
				modified=true
			i+=1
	if modified :
		targetMeshNode.data_polygon=new_polygon
		
	#Checking UV Maps 
	i=0
	modified=false
	var new_uv_maps=targetMeshNode.data_uv_maps.duplicate(true)
	
	while(i<new_uv_maps.size() ):
		var map=new_uv_maps[i]
		var j=0
		while(j<map.size() ):	
			if map[j]==particle_index :
				modified=true
				map.remove_at(j)
				print("deleted uv map point:" +str(j) )
			else :
				if map[j]>particle_index :
					new_uv_maps[i][j]-=1
					modified=true
				j+=1
		if map.size()<3 :
			modified=true
			new_uv_maps.remove_at(i)
			print("deleted uv map:" +str(j) )
		else :
			i+=1
			
			
	if modified :
		targetMeshNode.data_uv_maps=new_uv_maps
		
	targetMeshNode.queue_redraw()
	
			
	#position
	var particles=targetMeshNode.data_particle_positions
	particles.remove_at(particle_index)
	targetMeshNode.data_particle_positions=particles
	#radius
	var particle_radius=targetMeshNode.data_particle_radius
	particle_radius.remove_at(particle_index)
	targetMeshNode.data_particle_radius=particle_radius
	#internal
	var particles_is_internal=targetMeshNode.data_particle_is_internal
	particles_is_internal.remove_at(particle_index)
	targetMeshNode.data_particle_is_internal=particles_is_internal
	#enabled
	var particles_is_enabled=targetMeshNode.data_particle_is_enabled
	particles_is_enabled.remove_at(particle_index)
	targetMeshNode.data_particle_is_enabled=particles_is_enabled
	#lazy
	var particles_is_lazy=targetMeshNode.data_particle_is_lazy
	particles_is_lazy.remove_at(particle_index)
	targetMeshNode.data_particle_is_lazy=particles_is_lazy
	
	targetMeshNode.queue_redraw()
	targetPlugin.update_overlays()
	pass
