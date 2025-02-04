extends OptionButton
@export_node_path("QWorldNode") var targetWorld
var world:QWorldNode
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	world=get_node(targetWorld)
	call_deferred("set_option_bar_to_current_iteration")	
	connect("item_selected",on_item_selected)
	
	pass # Replace with function body.

func set_option_bar_to_current_iteration() :
	var current_it:int=get_physics_iteration()
	print(current_it)
	match current_it :
		2 :
			selected=0
		3 :
			selected=1
		4 :
			selected=2
		8 :
			selected=3
		16 :
			selected=4

func on_item_selected(index:int) :
	
	match index :
		0 :
			set_physics_iteration(2)
		1 :
			set_physics_iteration(4)
		2 :
			set_physics_iteration(8)
		3 :
			set_physics_iteration(16)
	get_parent().get_parent().clear()
	
	
			
func set_physics_iteration(value:int ):
	world.iteration_count=value
	pass
	
func get_physics_iteration( )->int:
	return world.iteration_count
