#include "qbody_node.h"
#include "qworld_node.h"
#include "QuarkPhysics/qsoftbody.h"

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/variant/utility_functions.hpp>


void QBodyNode::_notification(int what) {
    switch (what){
        case NOTIFICATION_POST_ENTER_TREE:
            if(Engine::get_singleton()->is_editor_hint()==false)
                on_post_enter_tree();
            break;
        
        case NOTIFICATION_PREDELETE:
            if(Engine::get_singleton()->is_editor_hint()==false && bodyObject!=nullptr){
                if(worldNode!=nullptr){
                    worldNode->remove_body_node(this);
                }
            }
            break;    
        
        default:
            break;
    }
}

void QBodyNode::on_post_enter_tree() {
    if(isConfigured==false){
        //Check meshes
        set_body_position(get_global_position());
        set_body_rotation(get_global_rotation());
        //Check world
        if(worldNode==nullptr){
            Node *root=get_tree()->get_current_scene();
            if(root->has_node(NodePath("QWorldNode") )==true){
                Node *root_world_node=root->get_node<Node>(NodePath("QWorldNode"));
                if(root_world_node!=nullptr){
                    worldNode=(QWorldNode*)root_world_node;
                }
            }else{
                godot::UtilityFunctions::printerr("Quark Physics Error: Physics needs a world! Add a QWorldNode to the root node of the scene. ");
                
                return;
            }
        }
        
        if(bodyObject->GetBodyType()==QBody::BodyTypes::SOFT){
            QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
            //print_line("Resetting area preserving enabled.");
            if(sb->GetAreaPreservingEnabled())
                sb->SetAreaPreservingEnabled(true);
            if(sb->GetShapeMatchingFixedTransformEnabled()==true ){
                sb->SetShapeMatchingFixedPosition( QVector(get_global_position().x,get_global_position().y) );
                sb->SetShapeMatchingFixedRotation(get_global_rotation() );
            }
                
        }
        worldNode->add_body(this);
        isConfigured=true;

    }
}

void QBodyNode::_bind_methods() {
    //Get
     ClassDB::bind_method(D_METHOD("get_world_node"),&QBodyNode::get_world_node );
     ClassDB::bind_method(D_METHOD("get_body_position"),&QBodyNode::get_body_position );
     ClassDB::bind_method(D_METHOD("get_body_previous_position"),&QBodyNode::get_body_previous_position );
     ClassDB::bind_method(D_METHOD("get_body_rotation"),&QBodyNode::get_body_rotation );
     ClassDB::bind_method(D_METHOD("get_body_previous_rotation"),&QBodyNode::get_body_previous_rotation );
     ClassDB::bind_method(D_METHOD("get_body_rotation_degree"),&QBodyNode::get_body_rotation_degree );
     ClassDB::bind_method(D_METHOD("get_aabb"),&QBodyNode::get_aabb );
     ClassDB::bind_method(D_METHOD("get_total_initial_area"),&QBodyNode::get_total_initial_area );
     ClassDB::bind_method(D_METHOD("get_total_polygons_initial_area"),&QBodyNode::get_total_polygons_initial_area );
     ClassDB::bind_method(D_METHOD("get_total_area"),&QBodyNode::get_total_area );
     ClassDB::bind_method(D_METHOD("get_total_polygons_area"),&QBodyNode::get_total_polygons_area );
     ClassDB::bind_method(D_METHOD("get_mode"),&QBodyNode::get_mode );
     ClassDB::bind_method(D_METHOD("get_layers_bit"),&QBodyNode::get_layers_bit );
     ClassDB::bind_method(D_METHOD("get_collidable_layers_bit"),&QBodyNode::get_collidable_layers_bit );
     ClassDB::bind_method(D_METHOD("get_overlap_with_collidable_layers_bit"),&QBodyNode::get_overlap_with_collidable_layers_bit );
     ClassDB::bind_method(D_METHOD("get_overlap_with_layers_bit"),&QBodyNode::get_overlap_with_layers_bit );
     ClassDB::bind_method(D_METHOD("get_is_sleeping"),&QBodyNode::get_is_sleeping );
     ClassDB::bind_method(D_METHOD("get_can_sleep"),&QBodyNode::get_can_sleep );
     ClassDB::bind_method(D_METHOD("get_friction"),&QBodyNode::get_friction );
     ClassDB::bind_method(D_METHOD("get_static_friction"),&QBodyNode::get_static_friction );
     ClassDB::bind_method(D_METHOD("get_air_friction"),&QBodyNode::get_air_friction );
     ClassDB::bind_method(D_METHOD("get_mass"),&QBodyNode::get_mass );
     ClassDB::bind_method(D_METHOD("get_restitution"),&QBodyNode::get_restitution );
     ClassDB::bind_method(D_METHOD("get_body_specific_time_scale_enabled"),&QBodyNode::get_body_specific_time_scale_enabled );
     ClassDB::bind_method(D_METHOD("get_body_specific_time_scale"),&QBodyNode::get_body_specific_time_scale );
     ClassDB::bind_method(D_METHOD("get_enabled"),&QBodyNode::get_enabled );
     ClassDB::bind_method(D_METHOD("get_velocity_limit"),&QBodyNode::get_velocity_limit );
     ClassDB::bind_method(D_METHOD("get_integrated_velocities_enabled"),&QBodyNode::get_integrated_velocities_enabled );

     //Set
     ClassDB::bind_method(D_METHOD("set_body_position","value","with_previous_position"),&QBodyNode::set_body_position );
     ClassDB::bind_method(D_METHOD("add_body_position","value"),&QBodyNode::add_body_position );
     ClassDB::bind_method(D_METHOD("set_body_previous_position","value"),&QBodyNode::set_body_previous_position );
     ClassDB::bind_method(D_METHOD("add_body_previous_position","value"),&QBodyNode::add_body_previous_position );
     ClassDB::bind_method(D_METHOD("set_body_rotation","angle_radian","with_previous_rotation"),&QBodyNode::set_body_rotation );
     ClassDB::bind_method(D_METHOD("set_body_rotation_degree","angle_radian","with_previous_rotation"),&QBodyNode::set_body_rotation_degree );
     ClassDB::bind_method(D_METHOD("add_body_rotation","angle_radian"),&QBodyNode::add_body_rotation );
     ClassDB::bind_method(D_METHOD("set_body_previous_rotation","angle_radian"),&QBodyNode::set_body_previous_rotation );
     ClassDB::bind_method(D_METHOD("set_layers_bit","value"),&QBodyNode::set_layers_bit );
     ClassDB::bind_method(D_METHOD("set_collidable_layers_bit","value"),&QBodyNode::set_collidable_layers_bit );
     ClassDB::bind_method(D_METHOD("set_can_sleep","value"),&QBodyNode::set_can_sleep );
     ClassDB::bind_method(D_METHOD("set_mode","value"),&QBodyNode::set_mode );
     ClassDB::bind_method(D_METHOD("set_friction","value"),&QBodyNode::set_friction );
     ClassDB::bind_method(D_METHOD("set_static_friction","value"),&QBodyNode::set_static_friction );
     ClassDB::bind_method(D_METHOD("set_air_friction","value"),&QBodyNode::set_air_friction );
     ClassDB::bind_method(D_METHOD("set_mass","value"),&QBodyNode::set_mass );
     ClassDB::bind_method(D_METHOD("set_restitution","value"),&QBodyNode::set_restitution );
     ClassDB::bind_method(D_METHOD("set_body_spesific_time_scale_enabled","value"),&QBodyNode::set_body_spesific_time_scale_enabled );
     ClassDB::bind_method(D_METHOD("set_body_spesific_time_scale","value"),&QBodyNode::set_body_spesific_time_scale );
     ClassDB::bind_method(D_METHOD("set_enabled","value"),&QBodyNode::set_enabled );
     ClassDB::bind_method(D_METHOD("set_velocity_limit","value"),&QBodyNode::set_velocity_limit );
     ClassDB::bind_method(D_METHOD("set_integrated_velocities_enabled","value"),&QBodyNode::set_integrated_velocities_enabled );
     ClassDB::bind_method(D_METHOD("wake_up"),&QBodyNode::wake_up );

     ClassDB::bind_method(D_METHOD("add_mesh_node","mesh_node"),&QBodyNode::add_mesh_node );
     ClassDB::bind_method(D_METHOD("remove_mesh_node_at","index"),&QBodyNode::remove_mesh_node_at );
     ClassDB::bind_method(D_METHOD("get_mesh_at","index"),&QBodyNode::get_mesh_at );
     ClassDB::bind_method(D_METHOD("get_mesh_count"),&QBodyNode::get_mesh_count );

     

     //Export Features
     ADD_PROPERTY( PropertyInfo(Variant::BOOL, "enabled"),"set_enabled","get_enabled" );
     ADD_PROPERTY( PropertyInfo(Variant::INT, "mode", PROPERTY_HINT_ENUM, "Dynamic,Static"),"set_mode","get_mode" );
     ADD_PROPERTY( PropertyInfo(Variant::BOOL, "can_sleep"),"set_can_sleep","get_can_sleep" );
     ADD_PROPERTY( PropertyInfo(Variant::FLOAT, "velocity_limit"),"set_velocity_limit","get_velocity_limit" );

     ADD_GROUP("Physics Properties","");
     ADD_PROPERTY( PropertyInfo(Variant::FLOAT, "mass"),"set_mass","get_mass" );
     ADD_PROPERTY( PropertyInfo(Variant::FLOAT, "friction"),"set_friction","get_friction" );
     ADD_PROPERTY( PropertyInfo(Variant::FLOAT, "static_friction"),"set_static_friction","get_static_friction" );
     ADD_PROPERTY( PropertyInfo(Variant::FLOAT, "air_friction"),"set_air_friction","get_air_friction" );
     ADD_PROPERTY( PropertyInfo(Variant::FLOAT, "restitution"),"set_restitution","get_restitution" );

     ADD_GROUP("Collision Layers","");
     ADD_PROPERTY( PropertyInfo(Variant::INT, "layers",PROPERTY_HINT_LAYERS_2D_PHYSICS),"set_layers_bit","get_layers_bit" );
     ADD_PROPERTY( PropertyInfo(Variant::INT, "collidable_layers",PROPERTY_HINT_LAYERS_2D_PHYSICS),"set_collidable_layers_bit","get_collidable_layers_bit" );


     ADD_SIGNAL(MethodInfo("collision",PropertyInfo(Variant::OBJECT,"body_node"),PropertyInfo(Variant::DICTIONARY,"info")  ) );
     ADD_SIGNAL(MethodInfo("pre_step" ));
     ADD_SIGNAL(MethodInfo("step" ));

     BIND_CONSTANT(DYNAMIC);
     BIND_CONSTANT(STATIC);

     


}


//GET METHODS
QWorldNode *QBodyNode::get_world_node(){
    return worldNode;
}

Vector2 QBodyNode::get_body_position() {
	QVector value=bodyObject->GetPosition();
    return Vector2(value.x,value.y);
}

Vector2 QBodyNode::get_body_previous_position() {
	QVector value=bodyObject->GetPreviousPosition();
    return Vector2(value.x,value.y);
}

float QBodyNode::get_body_rotation() {
	return bodyObject->GetRotation();
}

float QBodyNode::get_body_previous_rotation() {
	return bodyObject->GetPreviousRotation();
}

float QBodyNode::get_body_rotation_degree() {
	return bodyObject->GetRotationDegree();
}

Rect2 QBodyNode::get_aabb() {
	QAABB aabb=bodyObject->GetAABB();
    QVector center=aabb.GetCenterPosition();
    QVector size=aabb.GetSize();
    Rect2 res(Vector2(center.x,center.y),Vector2(size.x,size.y) );
    return res;
}

float QBodyNode::get_total_initial_area() {
	return bodyObject->GetTotalInitialArea();
}

float QBodyNode::get_total_polygons_initial_area() {
	return bodyObject->GetTotalPolygonsInitialArea();
}

float QBodyNode::get_total_area() {
	return bodyObject->GetTotalArea();
}

float QBodyNode::get_total_polygons_area() {
	return bodyObject->GetTotalPolygonsArea();
}

QBodyNode::Modes QBodyNode::get_mode() {
	if(bodyObject->GetMode()==QBody::Modes::DYNAMIC){
        return Modes::DYNAMIC;
    }else{
        return Modes::STATIC;
    }
}

int QBodyNode::get_layers_bit() {
	return bodyObject->GetLayersBit();
}

int QBodyNode::get_collidable_layers_bit() {
	return bodyObject->GetCollidableLayersBit(); 
}

bool QBodyNode::get_overlap_with_collidable_layers_bit(int layersBit) {
	return bodyObject->GetOverlapWithCollidableLayersBit(layersBit);
}

bool QBodyNode::get_overlap_with_layers_bit(int layersBit) {
	return bodyObject->GetOverlapWithLayersBit(layersBit);
}

bool QBodyNode::get_is_sleeping() {
	return bodyObject->GetIsSleeping();
}

bool QBodyNode::get_can_sleep() {
	return bodyObject->GetCanSleep();
}

float QBodyNode::get_friction() {
	return bodyObject->GetFriction();
}

float QBodyNode::get_static_friction() {
	return bodyObject->GetStaticFriction();
}

float QBodyNode::get_air_friction() {
	return bodyObject->GetAirFriction();
}

float QBodyNode::get_mass() {
	return bodyObject->GetMass();
}

float QBodyNode::get_restitution() {
	return bodyObject->GetRestitution();
}



bool QBodyNode::get_body_specific_time_scale_enabled() {
	return bodyObject->GetBodySpecificTimeScaleEnabled();
}

float QBodyNode::get_body_specific_time_scale() {
	return bodyObject->GetBodySpesificTimeScale();
}

bool QBodyNode::get_enabled() {
    return enabled;
	
}

float QBodyNode::get_velocity_limit() {
	return bodyObject->GetVelocityLimit();
}

bool QBodyNode::get_integrated_velocities_enabled() {
	return bodyObject->GetIntegratedVelocitiesEnabled();
}

//SET METHODS

QBodyNode *QBodyNode::set_body_position(Vector2 value, bool with_previous_position) {
    bodyObject->SetPosition(QVector(value.x,value.y),with_previous_position );
	return this;
}

QBodyNode *QBodyNode::add_body_position(Vector2 value) {
	bodyObject->AddPosition(QVector(value.x,value.y) );
	return this;
}

QBodyNode *QBodyNode::set_body_previous_position(Vector2 value) {
	bodyObject->SetPreviousPosition(QVector(value.x,value.y) );
	return this;
}

QBodyNode *QBodyNode::add_body_previous_position(Vector2 value) {
	bodyObject->AddPreviousPosition(QVector(value.x,value.y) );
	return this;
}

QBodyNode *QBodyNode::set_body_rotation(float angle_radian, bool with_previous_rotation) {
	bodyObject->SetRotation(angle_radian,with_previous_rotation );
	return this;
}

QBodyNode *QBodyNode::set_body_rotation_degree(float degree, bool with_previous_rotation) {
	bodyObject->SetRotationDegree(degree,with_previous_rotation );
	return this;
}

QBodyNode *QBodyNode::add_body_rotation(float angle_radian) {
    bodyObject->AddRotation(angle_radian);
	return this;
}

QBodyNode *QBodyNode::set_body_previous_rotation(float angle_radian) {
	bodyObject->SetPreviousRotation(angle_radian);
	return this;
}

QBodyNode *QBodyNode::add_body_previous_rotation(float angle_radian) {
	bodyObject->AddPreviousRotation(angle_radian);
	return this;
}



QBodyNode *QBodyNode::set_layers_bit(int value) {
	bodyObject->SetLayersBit(value);
	return this;
}

QBodyNode *QBodyNode::set_collidable_layers_bit(int value) {
	bodyObject->SetCollidableLayersBit(value);
	return this;
}

QBodyNode *QBodyNode::set_can_sleep(bool value) {
	bodyObject->SetCanSleep(value);
	return this;
}

QBodyNode *QBodyNode::set_mode(Modes value) {
	if(value==Modes::DYNAMIC){
        bodyObject->SetMode(QBody::Modes::DYNAMIC);
    }else{
        bodyObject->SetMode(QBody::Modes::STATIC);
    }
    update_meshes_draw();
    return this;
}

QBodyNode *QBodyNode::set_friction(float value) {
	bodyObject->SetFriction(value);
	return this;
}

QBodyNode *QBodyNode::set_static_friction(float value) {
	bodyObject->SetStaticFriction(value);
	return this;
}

QBodyNode *QBodyNode::set_air_friction(float value) {
    bodyObject->SetAirFriction(value);
	return this;
}

QBodyNode *QBodyNode::set_mass(float value) {
	bodyObject->SetMass(value);
	return this;
}

QBodyNode *QBodyNode::set_restitution(float value) {
	bodyObject->SetRestitution(value);
	return this;
}

QBodyNode *QBodyNode::set_body_spesific_time_scale_enabled(bool value) {
	bodyObject->SetBodySpecificTimeScaleEnabled(value);
	return this;
}

QBodyNode *QBodyNode::set_body_spesific_time_scale(float value) {
	bodyObject->SetBodySpecificTimeScale(value);
	return this;
}

QBodyNode *QBodyNode::set_enabled(bool value) {
    enabled=value;

    bodyObject->SetEnabled(value);
    update_meshes_draw();
	return this;
}

QBodyNode *QBodyNode::set_velocity_limit(float value) {
    bodyObject->SetVelocityLimit(value);
	return this;
}

QBodyNode *QBodyNode::set_integrated_velocities_enabled(bool value) {
    bodyObject->SetIntegratedVelocitiesEnabled(value);
	return this;
}

QBodyNode *QBodyNode::wake_up() {
    bodyObject->WakeUp();
	return this;
}

QBodyNode *QBodyNode::add_mesh_node(Object *mesh_node) {
    QMeshNode *qmesh_node=QMeshNode::type_cast(mesh_node);
	if(qmesh_node==nullptr){
		godot::UtilityFunctions::printerr("Quark Physics Error: Not valid mesh node type! | QWorldNode.add_mesh_node() ");
		return this;
	}
    meshNodes.push_back(qmesh_node);
    if(qmesh_node->meshObject==nullptr){
        return this;
    }
    bodyObject->AddMesh(qmesh_node->meshObject);
	return this;
}

QBodyNode *QBodyNode::remove_mesh_node_at(int index) {
    bodyObject->RemoveMeshAt(index);
	meshNodes.erase(meshNodes.begin()+index);
    return this;
}

QBodyNode *QBodyNode::remove_mesh_node(Object *mesh_node) {
    QMeshNode *qmesh_node=QMeshNode::type_cast(mesh_node);
    if(qmesh_node==nullptr){
		godot::UtilityFunctions::printerr("Quark Physics Error: Not valid mesh node type! | QWorldNode.remove_mesh_node() ");
		return this;
	}
    auto findedIt=find(meshNodes.begin(),meshNodes.end(),qmesh_node);
    if(findedIt!=meshNodes.end() ){
        meshNodes.erase(findedIt);
        bodyObject->RemoveMeshAt(findedIt-meshNodes.begin());
    }
        
	return this;
}

QMeshNode *QBodyNode::get_mesh_at(int index) {
	return meshNodes[index];
}

int QBodyNode::get_mesh_count() {
	return meshNodes.size();
}

QMeshNode *QBodyNode::get_mesh_node_with_object(QMesh *mesh_object) {
    for(auto meshNode:meshNodes){
        if(meshNode->meshObject==mesh_object){
            return meshNode;
        }
    }
	return nullptr;
}

QBodyNode *QBodyNode::type_cast(Object *obj) {
	Node2D *node=Object::cast_to<Node2D>(obj);
    if(node!=nullptr){
        if(node->get_class()=="QBodyNode" || node->get_class()=="QRigidBodyNode" || node->get_class()=="QSoftBodyNode" || node->get_class()=="QAreaBodyNode" || node->get_class()=="QPlatformerBodyNode" ){
            return static_cast<QBodyNode*>(node);
        }
    }
    return nullptr;
}

void QBodyNode::update_meshes_draw() {
    if( Engine::get_singleton()->is_editor_hint()==true ){
        for(auto meshNode:meshNodes){
            meshNode->queue_redraw();
        }
    }
}

bool QBodyNode::on_collision_callback(QBody *body, QBody::CollisionInfo info) {
    QBodyNode *body_as_node=worldNode->get_body_node_with_object(body);
    Dictionary infoData;
    infoData["position"]=Vector2(info.position.x,info.position.y);
    infoData["body"]=worldNode->get_body_node_with_object(info.body);
    infoData["normal"]=Vector2(info.normal.x,info.normal.y);
    infoData["penetration"]=info.penetration;
    emit_signal("collision",body_as_node,infoData);
    return true;
}

void QBodyNode::pre_step_callback(QBody *body) {
    emit_signal("pre_step");
}

void QBodyNode::step_callback(QBody *body) {
    emit_signal("step");
}
