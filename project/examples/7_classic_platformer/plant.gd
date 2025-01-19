extends QMeshAdvancedNode


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	queue_redraw()
	
func _draw() -> void:
	var points:PackedVector2Array
	for i in range(get_particle_count()) :
		var p=get_particle_at(i).get_global_position()-global_position
		points.append(p)
	#draw_multiline(points,Color.BLACK,10,true)
	draw_polyline(points,Color.BLACK,3,true)
	#for i in range(points.size()-1) :
		#var pa=points[i]
		#var pb=points[i+1]
		#draw_line(pa,pb,Color.BLACK,4.0)
