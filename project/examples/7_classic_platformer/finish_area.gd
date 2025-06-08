extends QAreaBodyNode

var level_finished=false

func _ready() -> void:
	connect("collision_enter",on_collision_enter)
	
func on_collision_enter(area_body_node,collided_body_node) :
	if level_finished==false :
		if collided_body_node==get_parent().get_node("QPlatformerBodyNode") :
			level_finished=true
			var message_label:Label=get_parent().get_node("Label")
			message_label.text="LEVEL FINISHED!"
			$GPUParticles2D.emitting=true
