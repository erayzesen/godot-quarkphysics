extends QSoftBodyNode

var ind=0

func _ready() -> void:
	var mesh=get_mesh_at(0) # mesh
	#Connecting flag to stick via springs
	var obody=get_parent() #other body
	var omesh=obody.get_mesh_at(0) #other body mesh
	
	var springA=QSpringObject.new()
	springA.configure(mesh.get_particle_at(0),omesh.get_particle_at(4),0.0,false)
	var springB=QSpringObject.new()
	springB.configure(mesh.get_particle_at(2),omesh.get_particle_at(5),0.0,false)
	
	
	get_world_node().add_spring(springA)
	get_world_node().add_spring(springB)
	pass # Replace with function body.
