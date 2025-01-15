@tool
extends QRigidBodyNode

var start_location:Vector2
@export var next_location=Vector2.ZERO :set = set_next_location
@export var move_speed:float=1
var move_side=1

func _ready() -> void:
	set_process(true)
	start_location=global_position
	if Engine.is_editor_hint() :
		queue_redraw()
	
			
		
func _enter_tree() -> void:
	if Engine.is_editor_hint()==false :
		return
	set_notify_transform(true)
	
	
func set_next_location(value):
	next_location=value
	queue_redraw()
	
func _notification(what: int) -> void:
	if Engine.is_editor_hint()==false :
		return
	if what==NOTIFICATION_TRANSFORM_CHANGED :
		queue_redraw()
	
func _draw() -> void:
	#Drawing to See Next Location
	if Engine.is_editor_hint() :
		var guide_color=Color.DIM_GRAY
		var next_location_local=next_location-global_position
		draw_dashed_line(Vector2.ZERO,next_location-global_position,guide_color,-1.0,10.0)
		
		var mesh:QMeshNode=get_child(0)
		if mesh!=null:
			for i in range(mesh.data_polygon.size()) :
				var p_i=mesh.data_polygon[i] #particle index
				var np_i=mesh.data_polygon[ (i+1)%mesh.data_polygon.size() ] #next particle index
				var p=mesh.data_particle_positions[ p_i ]+next_location_local #particle position
				var np=mesh.data_particle_positions[ np_i ]+next_location_local #next particle position
				draw_dashed_line(p,np,guide_color,-1.0,10.0)
		
		
func _physics_process(delta: float) -> void:
	if Engine.is_editor_hint():
		return
	#Moving Platform Codes
	var target_location=next_location if move_side==1 else start_location
	var diff_vec=target_location-global_position
	var diff_unit=diff_vec.normalized()
	if diff_vec.length()<=move_speed :
		add_force(diff_vec)	
		move_side*=-1
	var move_vel=diff_unit*move_speed
	add_force(move_vel)
	
	
	pass
