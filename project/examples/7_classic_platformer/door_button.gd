extends QRigidBodyNode

@export var door:QRigidBodyNode
var start_position:Vector2
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	start_position=global_position
	
	var joint_a:QJointObject=QJointObject.new()
	joint_a.configure_with_common_anchor_position(self,global_position-Vector2(-16,0),null)
	joint_a.set_rigidity(0.01)
	get_world_node().add_joint(joint_a)
	
	var joint_b:QJointObject=QJointObject.new()
	joint_b.configure_with_common_anchor_position(self,global_position-Vector2(16,0),null)
	joint_b.set_rigidity(0.01)
	get_world_node().add_joint(joint_b)
	


func _physics_process(delta: float) -> void:
	set_body_position_and_collide(Vector2 (start_position.x,get_body_position().y) ,true)
	
	var diff_y=global_position.y-start_position.y
	if diff_y>8:
		door.visible=false
		door.enabled=false
	else :
		door.visible=true
		door.enabled=true
	pass
	
