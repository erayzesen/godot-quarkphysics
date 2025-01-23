extends QWorldNode


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	call_deferred("move_zero")
	
	var mesh:QMeshNode=QMeshNode.new()
	var particle:QParticleObject=QParticleObject.new()
	
	particle.configure(Vector2.ZERO,0.5)
	mesh.add_particle(particle)
	pass # Replace with function body.
	
func move_zero():
	get_parent().move_child(self,0)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
