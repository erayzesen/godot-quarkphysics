extends Node2D

var current_scene:Node2D
var option_button:OptionButton
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	z_index=100
	option_button=OptionButton.new()
	option_button.add_item("1-Welcome",0)
	option_button.add_item("2-Soft Body Textured",1)
	option_button.add_item("3-Joints",2)
	option_button.add_item("4-Frictions",3)
	option_button.add_item("5-So Soft World",4)
	option_button.add_item("6-Box Stack",5)
	option_button.add_item("7-Classic Platformer",6)
	option_button.add_item("8-Box Benchmark ",7)
	option_button.add_item("9-Box Benchmark Godot ",8)
	option_button.set_global_position(Vector2(32,32) )
	add_child(option_button)
	option_button.selected=-1
	option_button.allow_reselect=true
	option_button.connect("item_selected",on_example_selected)
	#call_deferred("on_example_selected",0)
	pass # Replace with function body.


func on_example_selected(value:int) :
	match value:
		0:
			get_tree().change_scene_to_file("res://examples/1_welcome/main.tscn")
		1:
			get_tree().change_scene_to_file("res://examples/2_softbody_show/main.tscn")
		2:
			get_tree().change_scene_to_file("res://examples/3_joints/main.tscn")
		3:
			get_tree().change_scene_to_file("res://examples/4_frictions/main.tscn")
		4:
			get_tree().change_scene_to_file("res://examples/5_so_soft_world/main.tscn")
		5:
			get_tree().change_scene_to_file("res://examples/6_box_stack/main.tscn")
		6:
			get_tree().change_scene_to_file("res://examples/7_classic_platformer/main.tscn")
		7:
			get_tree().change_scene_to_file("res://examples/8_box_benchmark/main.tscn")
		8:
			get_tree().change_scene_to_file("res://examples/9_box_benchmark_godot/main.tscn")
	
	
	
	pass
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
