extends Node2D
var box_instance:PackedScene=preload("res://examples/8_box_benchmark/qp_box.tscn")
var colors=[Color.ORANGE_RED,Color.ORANGE,Color.ROYAL_BLUE,Color.SEA_GREEN]
var box_counter:int=0

var spawn_mode=false

var boxes:Node2D=Node2D.new()
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	add_child(boxes)
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	if(spawn_mode==true) :
		spawn_box(get_global_mouse_position())
	$Control/Label.text="Box Count: "+ str(box_counter) + " FPS:" + str(Engine.get_frames_per_second() )
	pass
	
func _unhandled_input(event: InputEvent) -> void:
	if event is InputEventMouseButton :
		if event.button_index==MOUSE_BUTTON_LEFT :
			if event.is_pressed():
				spawn_mode=true
			else :
				spawn_mode=false
		else :
			spawn_mode=false
			
func spawn_box(pos:Vector2) :
	var box=box_instance.instantiate()
	box.global_position=pos
	boxes.add_child(box)
	box.get_node("ColorRect").color=colors[(box_counter)%colors.size()]
	box_counter+=1
	
func clear( ) :
	for box in boxes.get_children() :
		box.queue_free()
	box_counter=0
	
