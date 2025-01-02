class_name Coin extends QAreaBodyNode

var a=0.0
var start_position:Vector2
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	start_position=global_position
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta: float) -> void:
	a+=0.03
	set_body_position( start_position + Vector2(0,sin(a)*8.0),true )
	pass
