#include "qmesh_circle_node.h"
#include "qbody_node.h"



void QMeshCircleNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_circle_radius"),&QMeshCircleNode::get_circle_radius );
    ClassDB::bind_method(D_METHOD("get_center_position"),&QMeshCircleNode::get_center_position );
    

    ClassDB::bind_method(D_METHOD("set_circle_radius","value"),&QMeshCircleNode::set_circle_radius );
    ClassDB::bind_method(D_METHOD("set_center_position","value"),&QMeshCircleNode::set_center_position );

    ADD_PROPERTY( PropertyInfo(Variant::FLOAT, "circle_radius"),"set_circle_radius","get_circle_radius" );
    ADD_PROPERTY( PropertyInfo(Variant::VECTOR2, "center_position"),"set_center_position","get_center_position" );
    
}

void QMeshCircleNode::update_mesh_node_data() {
    QMesh::MeshData meshData;

    meshData.particlePositions.push_back(QVector(centerPosition.x,centerPosition.y));
    meshData.particleRadValues.push_back(circleRadius);
    meshData.particleInternalValues.push_back(false);

    apply_mesh_data_to_mesh_node_data(meshData);
    
    queue_redraw();
}


void QMeshCircleNode::set_circle_radius(float value) {
    circleRadius=value;
    update_mesh_node_data();
}

float  QMeshCircleNode::get_circle_radius() {
    return circleRadius;
}


void QMeshCircleNode::set_center_position(Vector2 value) {
    centerPosition=value;
    update_mesh_node_data();
}

Vector2  QMeshCircleNode::get_center_position() {
    return centerPosition;
}
