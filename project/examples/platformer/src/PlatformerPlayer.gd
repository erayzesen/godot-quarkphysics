extends QPlatformerBodyNode


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	var side:int=int(Input.is_action_pressed("ui_right") )-int( Input.is_action_pressed("ui_left") )
	walk(side)
	
	if Input.is_action_pressed("ui_up"):
		jump(5.0,false)
	else :
		release_jump()
	pass
