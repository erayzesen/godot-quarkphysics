#include "qrigidbody_node.h"
#include "qworld_node.h"
#include "qplatformerbody_node.h"

#include <godot_cpp/classes/engine.hpp>>

void QPlatformerBodyNode::_bind_methods() {
    //Floor
    ClassDB::bind_method(D_METHOD("set_moving_floor_snap_offset","value"), &QPlatformerBodyNode::set_moving_floor_snap_offset );
    ClassDB::bind_method(D_METHOD("get_moving_floor_snap_offset"), &QPlatformerBodyNode::get_moving_floor_snap_offset );

    ClassDB::bind_method(D_METHOD("set_floor_max_angle","value"), &QPlatformerBodyNode::set_floor_max_angle );
    ClassDB::bind_method(D_METHOD("get_floor_max_angle"), &QPlatformerBodyNode::get_floor_max_angle );

    ClassDB::bind_method(D_METHOD("set_floor_max_angle_degree","value"), &QPlatformerBodyNode::set_floor_max_angle_degree );
    ClassDB::bind_method(D_METHOD("get_floor_max_angle_degree"), &QPlatformerBodyNode::get_floor_max_angle_degree );

    ClassDB::bind_method(D_METHOD("get_is_on_floor"), &QPlatformerBodyNode::get_is_on_floor );

    ClassDB::bind_method(D_METHOD("get_is_on_ceiling"), &QPlatformerBodyNode::get_is_on_ceiling );

    //Gravity 
    
    ClassDB::bind_method(D_METHOD("set_gravity","value"), &QPlatformerBodyNode::set_gravity );
    ClassDB::bind_method(D_METHOD("get_gravity"), &QPlatformerBodyNode::get_gravity );

    ClassDB::bind_method(D_METHOD("set_gravity_multiplier","value"), &QPlatformerBodyNode::set_gravity_multiplier );
    ClassDB::bind_method(D_METHOD("get_gravity_multiplier"), &QPlatformerBodyNode::get_gravity_multiplier );

    //Walk

    ClassDB::bind_method(D_METHOD("set_walk_speed","value"), &QPlatformerBodyNode::set_walk_speed );
    ClassDB::bind_method(D_METHOD("get_walk_speed"), &QPlatformerBodyNode::get_walk_speed );

    ClassDB::bind_method(D_METHOD("set_walk_aceleration_rate","value"), &QPlatformerBodyNode::set_walk_aceleration_rate );
    ClassDB::bind_method(D_METHOD("get_walk_aceleration_rate"), &QPlatformerBodyNode::get_walk_aceleration_rate );

    ClassDB::bind_method(D_METHOD("set_walk_deceleration_rate","value"), &QPlatformerBodyNode::set_walk_deceleration_rate );
    ClassDB::bind_method(D_METHOD("get_walk_deceleration_rate"), &QPlatformerBodyNode::get_walk_deceleration_rate );

    ClassDB::bind_method(D_METHOD("walk","side"), &QPlatformerBodyNode::walk );

    //Controller Velocities

    ClassDB::bind_method(D_METHOD("set_controller_horizontal_velocity","value"), &QPlatformerBodyNode::set_controller_horizontal_velocity );
    ClassDB::bind_method(D_METHOD("get_controller_horizontal_velocity"), &QPlatformerBodyNode::get_controller_horizontal_velocity );

    ClassDB::bind_method(D_METHOD("set_controller_vertical_velocity","value"), &QPlatformerBodyNode::set_controller_vertical_velocity );
    ClassDB::bind_method(D_METHOD("get_controller_vertical_velocity"), &QPlatformerBodyNode::get_controller_vertical_velocity );

    ClassDB::bind_method(D_METHOD("get_is_falling"), &QPlatformerBodyNode::get_is_falling );

    ClassDB::bind_method(D_METHOD("get_is_rising"), &QPlatformerBodyNode::get_is_rising );

    //Jump

    ClassDB::bind_method(D_METHOD("set_jump_duration_frame_count","value"), &QPlatformerBodyNode::set_jump_duration_frame_count);
    ClassDB::bind_method(D_METHOD("get_jump_duration_frame_count"), &QPlatformerBodyNode::get_jump_duration_frame_count );

    ClassDB::bind_method(D_METHOD("set_jump_gravity_multiplier","value"), &QPlatformerBodyNode::set_jump_gravity_multiplier);
    ClassDB::bind_method(D_METHOD("get_jump_gravity_multiplier"), &QPlatformerBodyNode::get_jump_gravity_multiplier );

    ClassDB::bind_method(D_METHOD("set_jump_fall_gravity_multiplier","value"), &QPlatformerBodyNode::set_jump_fall_gravity_multiplier);
    ClassDB::bind_method(D_METHOD("get_jump_fall_gravity_multiplier"), &QPlatformerBodyNode::get_jump_fall_gravity_multiplier );

    ClassDB::bind_method(D_METHOD("set_max_jump_count","value"), &QPlatformerBodyNode::set_max_jump_count);
    ClassDB::bind_method(D_METHOD("get_max_jump_count"), &QPlatformerBodyNode::get_max_jump_count );

    ClassDB::bind_method(D_METHOD("jump","force","unconditional"), &QPlatformerBodyNode::jump);
    
    ClassDB::bind_method(D_METHOD("release_jump"), &QPlatformerBodyNode::release_jump );

    ClassDB::bind_method(D_METHOD("get_is_jumping"), &QPlatformerBodyNode::get_is_jumping );

    ClassDB::bind_method(D_METHOD("get_is_jump_released"), &QPlatformerBodyNode::get_is_jump_released );

    ClassDB::bind_method(D_METHOD("set_specific_platform_layers","value"), &QPlatformerBodyNode::set_specific_platform_layers);
    ClassDB::bind_method(D_METHOD("get_specific_platform_layers"), &QPlatformerBodyNode::get_specific_platform_layers );

    ClassDB::bind_method(D_METHOD("get_platform_collisions","test_position","nearest_on_axis"), &QPlatformerBodyNode::get_platform_collisions );

    ClassDB::bind_method(D_METHOD("get_right_wall","offset"), &QPlatformerBodyNode::get_right_wall );

    ClassDB::bind_method(D_METHOD("get_left_wall","offset"), &QPlatformerBodyNode::get_left_wall );

    ClassDB::bind_method(D_METHOD("get_floor","offset"), &QPlatformerBodyNode::get_floor );

    ClassDB::bind_method(D_METHOD("get_ceiling","offset"), &QPlatformerBodyNode::get_ceiling );



    

    ADD_PROPERTY( PropertyInfo(Variant::VECTOR2,"gravity"), "set_gravity","get_gravity" );
    ADD_PROPERTY( PropertyInfo(Variant::INT,"specific_platform_layers",PROPERTY_HINT_LAYERS_2D_PHYSICS), "set_specific_platform_layers","get_specific_platform_layers" );
    ADD_GROUP("Walking Properties","");
    ADD_PROPERTY( PropertyInfo(Variant::FLOAT,"walk_speed"), "set_walk_speed","get_walk_speed" );
    ADD_PROPERTY( PropertyInfo(Variant::FLOAT,"walk_aceleration_rate"), "set_walk_aceleration_rate","get_walk_aceleration_rate" );
    ADD_PROPERTY( PropertyInfo(Variant::FLOAT,"walk_deceleration_rate"), "set_walk_deceleration_rate","get_walk_deceleration_rate" );
    ADD_GROUP("Jumping Properties","");
    ADD_PROPERTY( PropertyInfo(Variant::INT,"max_jump_count"), "set_max_jump_count","get_max_jump_count" );
    ADD_PROPERTY( PropertyInfo(Variant::INT,"jump_duration_frame_count"), "set_jump_duration_frame_count","get_jump_duration_frame_count" );
    ADD_PROPERTY( PropertyInfo(Variant::FLOAT,"jump_gravity_multiplier"), "set_jump_gravity_multiplier","get_jump_gravity_multiplier" );
    ADD_PROPERTY( PropertyInfo(Variant::FLOAT,"jump_fall_gravity_multiplier"), "set_jump_fall_gravity_multiplier","get_jump_fall_gravity_multiplier" );
    
    ADD_GROUP("Floor Properties","");
    ADD_PROPERTY( PropertyInfo(Variant::FLOAT,"moving_floor_snap_offset"), "set_moving_floor_snap_offset","get_moving_floor_snap_offset" );
    ADD_PROPERTY( PropertyInfo(Variant::FLOAT,"floor_max_angle_degree"), "set_floor_max_angle_degree","get_floor_max_angle_degree" );
    
    
    

}





//QPLATFORMBODYNODE METHODS
QPlatformerBodyNode *QPlatformerBodyNode::set_moving_floor_snap_offset(float value) {
    QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    body->SetMovingFloorSnapOffset(value);
	return this;
}

float QPlatformerBodyNode::get_moving_floor_snap_offset() {
    QPlatformerBody *body=(QPlatformerBody*)bodyObject;
	return body->GetMovingFloorSnapOffset();
}

QPlatformerBodyNode *QPlatformerBodyNode::set_floor_max_angle(float value) {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    body->SetFloorMaxAngle(value);
	return this;
}

float QPlatformerBodyNode::get_floor_max_angle() {
    QPlatformerBody *body=(QPlatformerBody*)bodyObject;
	return body->GetFloorMaxAngle();
}

QPlatformerBodyNode *QPlatformerBodyNode::set_floor_max_angle_degree(float value) {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    body->SetFloorMaxAngleDegree(value);
	return this;
}

float QPlatformerBodyNode::get_floor_max_angle_degree() {
    QPlatformerBody *body=(QPlatformerBody*)bodyObject;
	return body->GetFloorMaxAngleDegree();
}

bool QPlatformerBodyNode::get_is_on_floor() {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
	return body->GetIsOnFloor();
}

bool QPlatformerBodyNode::get_is_on_ceiling() {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
	return body->GetIsOnCeiling();
}

QPlatformerBodyNode *QPlatformerBodyNode::set_gravity(Vector2 value) {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    body->SetGravity(QVector(value.x,value.y));
	return this;
}

Vector2 QPlatformerBodyNode::get_gravity() {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    QVector qvec=body->GetGravity();
    return Vector2(qvec.x,qvec.y);
}

QPlatformerBodyNode *QPlatformerBodyNode::set_gravity_multiplier(float value) {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    body->SetGravityMultiplier(value);
	return this;
}

float QPlatformerBodyNode::get_gravity_multiplier() {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
	return body->GetGravityMultiplier();
}

QPlatformerBodyNode *QPlatformerBodyNode::set_walk_speed(float value) {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    body->SetWalkSpeed(value);
	return this;
}

float QPlatformerBodyNode::get_walk_speed() {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
	return body->GetWalkSpeed();
}

QPlatformerBodyNode *QPlatformerBodyNode::set_walk_aceleration_rate(float value) {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    body->SetWalkAcelerationRate(value);
	return this;
}

float QPlatformerBodyNode::get_walk_aceleration_rate() {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
	return body->GetWalkAcelerationRate();
}

QPlatformerBodyNode *QPlatformerBodyNode::set_walk_deceleration_rate(float value) {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    body->SetWalkDecelerationRate(value);
	return this;
}

float QPlatformerBodyNode::get_walk_deceleration_rate() {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
	return body->GetWalkDecelerationRate();
}

QPlatformerBodyNode *QPlatformerBodyNode::walk(int side) {
    QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    body->Walk(side);
	return this;
}

QPlatformerBodyNode *QPlatformerBodyNode::set_controller_horizontal_velocity(Vector2 value) {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    body->SetControllerHorizontalVelocity(QVector(value.x,value.y));
	return this;
}

Vector2 QPlatformerBodyNode::get_controller_horizontal_velocity() {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    QVector qvec=body->GetControllerHorizontalVelocity();
    return Vector2(qvec.x,qvec.y);
}

QPlatformerBodyNode *QPlatformerBodyNode::set_controller_vertical_velocity(Vector2 value) {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    body->SetControllerVerticalVelocity(QVector(value.x,value.y));
	return this;
}

Vector2 QPlatformerBodyNode::get_controller_vertical_velocity() {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    QVector qvec=body->GetControllerVerticalVelocity();
    return Vector2(qvec.x,qvec.y);
}

bool QPlatformerBodyNode::get_is_falling() {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
	return body->GetIsFalling();
}

bool QPlatformerBodyNode::get_is_rising() {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
	return body->GetIsRising();
}

QPlatformerBodyNode *QPlatformerBodyNode::set_jump_duration_frame_count(int value) {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    body->SetJumpDurationFrameCount(value);
	return this;
}

int QPlatformerBodyNode::get_jump_duration_frame_count() {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
	return body->GetJumpDurationFrameCount();
}

QPlatformerBodyNode *QPlatformerBodyNode::set_jump_gravity_multiplier(float value) {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    body->SetJumpGravityMultiplier(value);
	return this;
}

float QPlatformerBodyNode::get_jump_gravity_multiplier() {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
	return body->GetJumpGravityMultiplier();
}

QPlatformerBodyNode *QPlatformerBodyNode::set_jump_fall_gravity_multiplier(float value) {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    body->SetJumpFallGravityMultiplier(value);
	return this;
}

float QPlatformerBodyNode::get_jump_fall_gravity_multiplier() {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
	return body->GetJumpFallGravityMultiplier();
}

QPlatformerBodyNode *QPlatformerBodyNode::set_max_jump_count(int value) {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    body->SetMaxJumpCount(value);
	return this;
}

int QPlatformerBodyNode::get_max_jump_count() {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
	return body->GetMaxJumpCount();
}

QPlatformerBodyNode *QPlatformerBodyNode::jump(float force, bool unconditional) {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    body->Jump(force,unconditional);
	return this;
}

QPlatformerBodyNode *QPlatformerBodyNode::release_jump() {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    body->ReleaseJump();
	return this;
}

bool QPlatformerBodyNode::get_is_jumping() {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
	return body->GetIsJumping();
}

bool QPlatformerBodyNode::get_is_jump_released() {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
	return body->GetIsJumpReleased();
}

QPlatformerBodyNode *QPlatformerBodyNode::set_specific_platform_layers(int layersBit) {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    body->SetSpecificPlatformLayers(layersBit);
	return this;
}

int QPlatformerBodyNode::get_specific_platform_layers() {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
	return body->GetSpecificPlatformLayers();
}

Dictionary QPlatformerBodyNode::get_platform_collisions(Vector2 test_position, Vector2 nearest_on_axis) {
    QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    QPlatformerBody::CollisionTestInfo collInfo=body->GetPlatformCollisions(QVector(test_position.x,test_position.y),QVector(nearest_on_axis.x,nearest_on_axis.y) );

    Dictionary sum={};
    sum["body"]=worldNode->get_body_node_with_object( collInfo.body);
    sum["position"]=Vector2(collInfo.position.x,collInfo.position.y);
    sum["penetration"]=collInfo.penetration;
    sum["normal"]=Vector2(collInfo.normal.x,collInfo.normal.y);

	return sum;
}

Dictionary QPlatformerBodyNode::get_right_wall(float offset) {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    QPlatformerBody::CollisionTestInfo collInfo=body->GetRightWall(offset);

    Dictionary sum={};
    sum["body"]=worldNode->get_body_node_with_object( collInfo.body);
    sum["position"]=Vector2(collInfo.position.x,collInfo.position.y);
    sum["penetration"]=collInfo.penetration;
    sum["normal"]=Vector2(collInfo.normal.x,collInfo.normal.y);

	return sum;
}

Dictionary QPlatformerBodyNode::get_left_wall(float offset) {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    QPlatformerBody::CollisionTestInfo collInfo=body->GetLeftWall(offset);

    Dictionary sum={};
    sum["body"]=worldNode->get_body_node_with_object( collInfo.body);
    sum["position"]=Vector2(collInfo.position.x,collInfo.position.y);
    sum["penetration"]=collInfo.penetration;
    sum["normal"]=Vector2(collInfo.normal.x,collInfo.normal.y);

	return sum;
}

Dictionary QPlatformerBodyNode::get_floor(float offset) {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    QPlatformerBody::CollisionTestInfo collInfo=body->GetFloor(offset);

    Dictionary sum={};
    sum["body"]=worldNode->get_body_node_with_object( collInfo.body);
    sum["position"]=Vector2(collInfo.position.x,collInfo.position.y);
    sum["penetration"]=collInfo.penetration;
    sum["normal"]=Vector2(collInfo.normal.x,collInfo.normal.y);

	return sum;
}

Dictionary QPlatformerBodyNode::get_ceiling(float offset) {
	QPlatformerBody *body=(QPlatformerBody*)bodyObject;
    QPlatformerBody::CollisionTestInfo collInfo=body->GetCeiling(offset);

    Dictionary sum={};
    sum["body"]=worldNode->get_body_node_with_object( collInfo.body);
    sum["position"]=Vector2(collInfo.position.x,collInfo.position.y);
    sum["penetration"]=collInfo.penetration;
    sum["normal"]=Vector2(collInfo.normal.x,collInfo.normal.y);

	return sum;
}
