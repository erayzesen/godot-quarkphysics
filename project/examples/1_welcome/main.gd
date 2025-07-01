extends Node2D

#With this script, we spawn rigid body objects in the scene using polygon, rectangle, and circle meshes.

enum SpawnBodTypes{
	RECT,
	POLYGON,
	CIRCLE
}


var body_count:int=32
var current_body_count:int=0

var spawn_duration=5
var step=0

var colors:PackedColorArray=[Color("ff8426"),Color("ff2674"),Color("007899"),Color("bfff3c") ]
	
func _ready() -> void:
	var myNode=QRigidBodyNode.new()
	$QWorldNode.add_body_node(myNode)
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	step+=1
	
	if current_body_count<body_count and step%spawn_duration==0 :
		current_body_count+=1
		
		#Spawn Position
		var xPos=randf_range(300,600)
		var yPos=randf_range(-500.0,-1000.0)
		
		
		#Defining a New Rigid Body
		var body=QRigidBodyNode.new()
		#Defining a New Mesh
		var mesh:QMeshNode
		
		#BodyType is a Random Integer
		var bodyType=randi_range(0,2)
		match bodyType :
			SpawnBodTypes.RECT:
				#Creating a Rectangle Mesh for a Rigid Body
				var rect_size=Vector2( randf_range(32.0,64),randf_range(32.0,64) )
				#body.mode=QBodyNode.STATIC
				mesh=QMeshRectNode.new()
				mesh.rectangle_size=rect_size
				
			SpawnBodTypes.POLYGON :
				#Creating a Polygon Mesh for a Rigid Body
				var side_count=randi_range(3,12)
				var radius=randf_range(16.0,32.0)
				mesh=QMeshPolygonNode.new()
				mesh.side_count=side_count
				mesh.polygon_radius=radius
				
			SpawnBodTypes.CIRCLE :
				#Creating a Circle Mesh for a Rigid Body
				var radius=randf_range(16.0,32.0)
				mesh=QMeshCircleNode.new()
				mesh.circle_radius=radius
				
		# Setting Vector Rendering Properties
		mesh.enable_vector_rendering=true
		mesh.enable_stroke=true
		mesh.fill_color=colors[ randi_range(0,colors.size()-1) ] 
		
		#Add Mesh To Body
		body.add_child(mesh)
		#Set Body Position
		body.global_position=Vector2(xPos,yPos)
		#Add Body as a Child
		add_child(body)
		
				
		
		
		
			
	pass
