extends Node2D


func _ready() -> void:
	ExamplesController.call_deferred("on_example_selected",0)
	ExamplesController.option_button.selected=0
	pass # Replace with function body.
