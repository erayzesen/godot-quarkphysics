class_name QTileMapCollider extends Node2D

enum MergeModes{
	DISABLE,
	HORIZONTAL,
	BOTH,
}

@export var merge_mode:MergeModes=MergeModes.BOTH

var tile_map:TileMapLayer
@onready var bodyNodes:Node2D=Node2D.new()
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	parse_tile_map()
				
func parse_tile_map() :
	if get_parent() is TileMapLayer :
		tile_map=get_parent()
		
	if tile_map==null :
		printerr("QTileMapCollider::The Parent node must be a TileMapLayer!")
		return
	
	#Creating and Adding Body Node Container
	
	add_child(bodyNodes)
	
	var physics_layer_count=tile_map.tile_set.get_physics_layers_count()
	var cells_rect=tile_map.get_used_rect()
	var collider_layers:Array
	for l in range(physics_layer_count) :
		var collision_layers_bit=tile_map.tile_set.get_physics_layer_collision_layer(l)
		var collidable_layers_bit=tile_map.tile_set.get_physics_layer_collision_mask(l)
		
		var collider_layer:Dictionary
		collider_layer["body_layers_bit"]=collision_layers_bit
		collider_layer["body_collidable_layers_bit"]=collidable_layers_bit
		
		#*** START OF HORIZONTAL MERGED POLYGONS ***
		var polygon_rows:Array
		for cy in range(cells_rect.position.y,cells_rect.position.y+cells_rect.size.y,1) :
			var polygon_list:Array[PackedVector2Array]
			var prev_polygon_support_points:PackedVector2Array
			for cx in range(cells_rect.position.x,cells_rect.position.x+cells_rect.size.x,1) :
				var cell_coord=Vector2(cx,cy)
				var cell_data=tile_map.get_cell_tile_data(cell_coord)
				var cell_pos=tile_map.map_to_local(cell_coord)
				#polygon_pos=tile_map.to_global(polygon_pos)
				if cell_data!=null :
					if cell_data.get_collision_polygons_count(l)!=0 :
						var polygon=LocalPolygonToGlobal( cell_data.get_collision_polygon_points(l,0),cell_pos )
						var is_cw=Geometry2D.is_polygon_clockwise(polygon)
						if is_cw==true :
							polygon.reverse()
						if (merge_mode==MergeModes.DISABLE) :
							polygon_list.append(polygon)
							continue
						if prev_polygon_support_points.size()<2 :
							polygon_list.append(polygon)
						else :
							if is_points_exist_in_polygon(prev_polygon_support_points,polygon) :
								#Get Previous polygon
								var prev_polygon=polygon_list[polygon_list.size()-1]
								#Get Previous Polygon Support Point Indexes
								var merged_polygon=Geometry2D.merge_polygons(prev_polygon,polygon)
								if merged_polygon.size()>0:
									polygon_list[polygon_list.size()-1]=merged_polygon[0]
							else :
								polygon_list.append(polygon)
								
				
							
						prev_polygon_support_points.clear()
						prev_polygon_support_points=get_support_points(polygon,Vector2.RIGHT)
					else :
						prev_polygon_support_points.clear()
				else :
					prev_polygon_support_points.clear()
			
			#Create Meshes via Polygon List
			polygon_rows.append(polygon_list)
			
		#*** END OF HORIZONTAL MERGED POLYGONS ***
		
		if merge_mode==MergeModes.HORIZONTAL || merge_mode==MergeModes.DISABLE :
			var collider_polygons:Array[PackedVector2Array]
			for row in polygon_rows :
				for poly in row :
					collider_polygons.append(poly)
			collider_layer["polygons"]=collider_polygons
		elif merge_mode==MergeModes.BOTH :
			# *** START OF VERTICAL MERGED POLYGONS ***
			var collider_polygons:Array[PackedVector2Array]
			for r in range(polygon_rows.size() ) :
				var row=polygon_rows[r]
				for polygon in row :
					var final_polygon=polygon.duplicate()
					
					
					var nr=r+1
					
					while nr<polygon_rows.size() :
						var not_matching=true
						var pi=0
						
						while pi<polygon_rows[nr].size() :
							
							var merged_polygon=Geometry2D.merge_polygons(final_polygon,polygon_rows[nr][pi])
							if merged_polygon.size()==1:
								if merged_polygon[0].size()>0 :
									final_polygon=merged_polygon[0]
									polygon_rows[nr].remove_at(pi)
									not_matching=false
								else :
									pi+=1
							else :
								pi+=1
								
						if not_matching==true :
							break
						nr+=1
					collider_polygons.append(final_polygon)
			
			#Merging All Collider Polygons - last process
			if merge_mode==MergeModes.BOTH :
				var ia=0
				while ia<collider_polygons.size() :
					var polygon_a=collider_polygons[ia]
					var ib=ia+1
					while ib<collider_polygons.size() :
						var polygon_b=collider_polygons[ib]
						var merged_polygon=Geometry2D.merge_polygons(polygon_a,polygon_b)
						if merged_polygon.size()==1  :
							if merged_polygon[0].size()>0 :
								collider_polygons[ia]=merged_polygon[0]
								collider_polygons.remove_at(ib)
								continue
						ib+=1
					ia+=1
				
						
			collider_layer["polygons"]=collider_polygons	
			# *** END OF VERTICAL MERGED POLYGONS ***	
				
			pass
			
		
		#Adding Collider Layer to Container
		collider_layers.append(collider_layer)
		
	#Creating Body And Colliders
	for collider_layer in collider_layers :
		for polygon in collider_layer["polygons"] :
			var bodyNode=QRigidBodyNode.new()
			bodyNode.mode=QBodyNode.STATIC
			bodyNode.layers=collider_layer["body_layers_bit"]
			bodyNode.collidable_layers=collider_layer["body_collidable_layers_bit"]
			bodyNodes.add_child(bodyNode)
	
			create_and_add_mesh_with_cell_data(bodyNode,polygon)
		
	pass
	

func LocalPolygonToGlobal(polygon :PackedVector2Array, pos:Vector2) :
	var res:PackedVector2Array
	for i in range(polygon.size()):
		res.append( polygon[ i ]+pos )
	return res
				
func get_support_points(points:PackedVector2Array, axis:Vector2)->PackedVector2Array:
	var result_indexes=get_support_point_indexes(points,axis)
	var result_points:PackedVector2Array
	for i in range (result_indexes.size()):
		result_points.append(points[result_indexes[i]])			
			
	return result_points
	
func get_support_point_indexes(points:PackedVector2Array, axis:Vector2)->PackedInt32Array:
	var max_distance=-INF
	var distances:PackedFloat32Array
	for i in range (points.size()):
		var dist=points[i].dot(axis)
		if dist>max_distance :
			max_distance=dist
		distances.append(dist)
	var result_indexes:PackedInt32Array
	for i in range (points.size()):
		if distances[i]==max_distance :
			result_indexes.append(i)
			
	return result_indexes
	
func is_points_exist_in_polygon(points:PackedVector2Array,polygon:PackedVector2Array) :
	var finded_count=0
	var matched=false
	for ppoint in polygon :
		for p in points :
			if p.round()==ppoint.round() :
				finded_count+=1
			if finded_count==points.size() :
				matched=true
				break
	return matched

func create_and_add_mesh_with_cell_data(bodyNode:QRigidBodyNode, polygon:PackedVector2Array)->QMeshNode :
	var mesh=QMeshNode.new() 
	bodyNode.add_child(mesh)
	#Adding Particles
	for i in range(polygon.size()) :
		
		var particle=QParticleObject.new()
		particle.set_position(polygon[i])
		particle.set_is_internal(false)
		particle.set_radius(0.5)
		mesh.add_particle(particle)
	
	#Adding Collider Polygon
	var polygonParticles:Array
	for i in range(mesh.get_particle_count() ) :
		polygonParticles.append(mesh.get_particle_at(i))
	mesh.set_polygon(polygonParticles)
	return mesh
