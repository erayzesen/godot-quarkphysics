extends Node2D


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	var start_joint=QJointObject.new()
	var first_body:QRigidBodyNode=get_child(0)
	var world_node=first_body.get_world_node()
	start_joint.configure_with_common_anchor_position( first_body,global_position,null )
	world_node.add_joint(start_joint)
	
	
	for i in range(get_child_count()-1 ):
		var body_a:QRigidBodyNode=get_child(i)
		var body_b:QRigidBodyNode=get_child(i+1)
		var size_body_a=body_a.get_aabb().size
		var joint=QJointObject.new()
		joint.configure_with_common_anchor_position(body_a,body_a.global_position+Vector2(0,size_body_a.y),body_b )
		world_node.add_joint(joint)
		joint.set_collision_enabled(false)
		
		
		
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
