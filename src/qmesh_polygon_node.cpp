#include "qmesh_polygon_node.h"
#include "qbody_node.h"



void QMeshPolygonNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_polygon_radius"),&QMeshPolygonNode::get_polygon_radius );
	ClassDB::bind_method(D_METHOD("get_side_count"),&QMeshPolygonNode::get_side_count );
    ClassDB::bind_method(D_METHOD("get_center_position"),&QMeshPolygonNode::get_center_position );
    ClassDB::bind_method(D_METHOD("get_polar_grid"),&QMeshPolygonNode::get_polar_grid );
    ClassDB::bind_method(D_METHOD("get_particle_radius"),&QMeshPolygonNode::get_particle_radius );

    ClassDB::bind_method(D_METHOD("set_polygon_radius","value"),&QMeshPolygonNode::set_polygon_radius );
    ClassDB::bind_method(D_METHOD("set_side_count","value"),&QMeshPolygonNode::set_side_count );
    ClassDB::bind_method(D_METHOD("set_center_position","value"),&QMeshPolygonNode::set_center_position );
    ClassDB::bind_method(D_METHOD("set_polar_grid","value"),&QMeshPolygonNode::set_polar_grid );
    ClassDB::bind_method(D_METHOD("set_particle_radius","value"),&QMeshPolygonNode::set_particle_radius );

    ADD_PROPERTY( PropertyInfo(Variant::FLOAT, "particle_radius"),"set_particle_radius","get_particle_radius" );
    ADD_PROPERTY( PropertyInfo(Variant::FLOAT, "polygon_radius", PROPERTY_HINT_RANGE,"0,99999,0.5"),"set_polygon_radius","get_polygon_radius" );
    ADD_PROPERTY( PropertyInfo(Variant::INT, "side_count"),"set_side_count","get_side_count" );
    ADD_PROPERTY( PropertyInfo(Variant::VECTOR2, "center_position"),"set_center_position","get_center_position" );
    ADD_PROPERTY( PropertyInfo(Variant::INT, "polar_grid"),"set_polar_grid","get_polar_grid" );
    
}

void QMeshPolygonNode::update_mesh_node_data() {
    QMesh::MeshData meshData=meshObject->GeneratePolygonMeshData(polygonRadius,sideCount,QVector(centerPosition.x,centerPosition.y),polarGrid,particleRadius);
    apply_mesh_data_to_mesh_node_data(meshData);
    queue_redraw();
}


void QMeshPolygonNode::set_polygon_radius(float value) {
    polygonRadius=value;
    update_mesh_node_data();
}

float  QMeshPolygonNode::get_polygon_radius() {
    return polygonRadius;
}

void QMeshPolygonNode::set_side_count(int value) {
    sideCount=value;
    update_mesh_node_data();
}

int  QMeshPolygonNode::get_side_count() {
    return sideCount;
}

void QMeshPolygonNode::set_center_position(Vector2 value) {
    centerPosition=value;
    update_mesh_node_data();
}

Vector2  QMeshPolygonNode::get_center_position() {
    return centerPosition;
}
void QMeshPolygonNode::set_polar_grid(int value) {
    polarGrid=value;
    update_mesh_node_data();
}

int  QMeshPolygonNode::get_polar_grid() {
    return polarGrid;
}

QMeshPolygonNode *QMeshPolygonNode::set_particle_radius(float value){
    particleRadius=value>=0.5f ? value:0.5f;
    update_mesh_node_data();
    return this;
}

float QMeshPolygonNode::get_particle_radius(){
    return particleRadius;
}