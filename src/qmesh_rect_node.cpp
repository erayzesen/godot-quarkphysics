#include "qmesh_rect_node.h"
#include "qbody_node.h"
#include <godot_cpp/variant/utility_functions.hpp>



void QMeshRectNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_size"),&QMeshRectNode::get_size );
    ClassDB::bind_method(D_METHOD("get_center_position"),&QMeshRectNode::get_center_position );
    ClassDB::bind_method(D_METHOD("get_grid"),&QMeshRectNode::get_grid );
    ClassDB::bind_method(D_METHOD("get_particle_radius"),&QMeshRectNode::get_particle_radius );

    ClassDB::bind_method(D_METHOD("set_size","value"),&QMeshRectNode::set_size );
    ClassDB::bind_method(D_METHOD("set_center_position","value"),&QMeshRectNode::set_center_position );
    ClassDB::bind_method(D_METHOD("set_grid","value"),&QMeshRectNode::set_grid );
    ClassDB::bind_method(D_METHOD("set_particle_radius","value"),&QMeshRectNode::set_particle_radius );

    ADD_PROPERTY( PropertyInfo(Variant::FLOAT, "particle_radius"),"set_particle_radius","get_particle_radius" );
    ADD_PROPERTY( PropertyInfo(Variant::VECTOR2, "rectangle_size"),"set_size","get_size" );
    ADD_PROPERTY( PropertyInfo(Variant::VECTOR2, "center_position"),"set_center_position","get_center_position" );
    ADD_PROPERTY( PropertyInfo(Variant::VECTOR2, "grid"),"set_grid","get_grid" );
    
}

void QMeshRectNode::update_mesh_node_data() {
    QMesh::MeshData meshData=meshObject->GenerateRectangleMeshData(QVector(size.x,size.y),QVector(centerPosition.x,centerPosition.y),QVector(grid.x,grid.y),particleRadius );
    apply_mesh_data_to_mesh_node_data(meshData);

    queue_redraw();
}


void QMeshRectNode::set_size(Vector2 value) {
    size=value;
    update_mesh_node_data();
}

Vector2  QMeshRectNode::get_size() {
    return size;
}
void QMeshRectNode::set_center_position(Vector2 value) {
    centerPosition=value;
    update_mesh_node_data();
}

Vector2  QMeshRectNode::get_center_position() {
    return centerPosition;
}
void QMeshRectNode::set_grid(Vector2 value) {
    grid=value;
    update_mesh_node_data();
}

Vector2  QMeshRectNode::get_grid() {
    return grid;
}

QMeshRectNode *QMeshRectNode::set_particle_radius(float value){
    particleRadius=value>=0.5f ? value:0.5f;
    update_mesh_node_data();
    return this;
}

float QMeshRectNode::get_particle_radius(){
    return particleRadius;
}