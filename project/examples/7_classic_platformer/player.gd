extends QPlatformerBodyNode

#The QPlatformerBody object is essentially
# a character class that handles fundamental platformer behaviors 
#(walking, acceleration, jumping, double jumping, sloped surfaces, etc.)
# out of the box. However, it is also flexible for
# adding different mechanics and features, 
#providing methods and properties to help you do more.
# For instance, in this example, 
#we are adding a wall-jumping feature to our character.


func _ready() -> void:
	#Using "collision" signal to handle collisions
	connect("collision",on_collision)
	pass 

	
func _physics_process(delta: float) -> void:
	#Walk 
	var walk_side=int(Input.is_action_pressed("ui_right"))-int(Input.is_action_pressed("ui_left"))
	walk(walk_side)
	#Implementing Wall Friction via the Gravity Multiplier Feature
	var wall_mode:bool=false 
	var wall_offset=3.0
	#Checking Walls according to the offset
	
	var wall_side= int( get_right_wall(3.0)["body"] !=null ) - int( get_left_wall(3.0)["body"]!=null )
	if get_is_on_floor()==false and wall_side!=0 :
		wall_mode=true
		if get_is_falling() :
			set_gravity_multiplier(0.3)
		else :
			set_gravity_multiplier(1.0)
	else :
		set_gravity_multiplier(1.0)
		
	#Jump 
	if Input.is_action_pressed("ui_up") :
		# If the player is on the Wall
		if wall_mode==true :
			if get_is_jump_released() :
				jump(4.0,true)
				set_controller_horizontal_velocity(Vector2.RIGHT*10.0*-wall_side)
		else :
			#Default Jump
			jump(4.0,false)
		
	if Input.is_action_just_released("ui_up") :
		release_jump()
		
func on_collision(body,info) :
	var collided_body=info["body"]
	if collided_body is Coin :
		collided_body.queue_free()
	
		
		
	
