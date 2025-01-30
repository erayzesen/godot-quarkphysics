extends QSoftBodyNode


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	var mesh=get_mesh_at(0)
	if mesh!=null :
		for i in range(mesh.get_particle_count()):
			var pp=mesh.get_particle_at((i-1+mesh.get_particle_count())%mesh.get_particle_count() )
			var p=mesh.get_particle_at(i)
			var np=mesh.get_particle_at( (i+1)%mesh.get_particle_count() )
			var angleConstraint=QAngleConstraintObject.new()
			angleConstraint.configure_with_angle_of_local_positions(pp,p,np,0.0)
			angleConstraint.set_rigidity(0.5)
			mesh.add_angle_constraint(angleConstraint)
	
		
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
