extends OptionButton


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	set_physics_iteration(16)
	call_deferred("set_option_bar_to_current_iteration")	
	connect("item_selected",on_item_selected)
	
	pass # Replace with function body.

func set_option_bar_to_current_iteration() :
	var current_it:int=get_physics_iteration()
	print(current_it)
	match current_it :
		16 :
			selected=0
		100 :
			selected=1
		200 :
			selected=2
		300 :
			selected=3

func on_item_selected(index:int) :
	
	match index :
		0 :
			set_physics_iteration(16)
		1 :
			set_physics_iteration(100)
		2 :
			set_physics_iteration(200)
		3 :
			set_physics_iteration(300)
	get_parent().get_parent().clear()
	
			
func set_physics_iteration(value:int ):
	PhysicsServer2D.space_set_param(get_world_2d().space,PhysicsServer2D.SPACE_PARAM_SOLVER_ITERATIONS,value)
func get_physics_iteration( ):
	return PhysicsServer2D.space_get_param(get_world_2d().space,PhysicsServer2D.SPACE_PARAM_SOLVER_ITERATIONS)
