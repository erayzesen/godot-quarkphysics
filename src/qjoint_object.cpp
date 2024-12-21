#include "qjoint_object.h"
#include <godot_cpp/variant/utility_functions.hpp>

void QJointObject::_bind_methods() {
	//Get
	ClassDB::bind_method(D_METHOD("get_body_node_a"),&QJointObject::get_body_node_a );
	ClassDB::bind_method(D_METHOD("get_body_node_b"),&QJointObject::get_body_node_b );
	ClassDB::bind_method(D_METHOD("get_anchor_position_a"),&QJointObject::get_anchor_position_a );
	ClassDB::bind_method(D_METHOD("get_anchor_position_b"),&QJointObject::get_anchor_position_b );
	ClassDB::bind_method(D_METHOD("get_anchor_global_position_a"),&QJointObject::get_anchor_global_position_a );
	ClassDB::bind_method(D_METHOD("get_anchor_global_position_b"),&QJointObject::get_anchor_global_position_b );
	ClassDB::bind_method(D_METHOD("get_collisions_enabled"),&QJointObject::get_collisions_enabled );
	ClassDB::bind_method(D_METHOD("get_groove_enabled"),&QJointObject::get_groove_enabled );
	ClassDB::bind_method(D_METHOD("get_length"),&QJointObject::get_length );
	ClassDB::bind_method(D_METHOD("get_balance"),&QJointObject::get_balance );
	ClassDB::bind_method(D_METHOD("get_rigidity"),&QJointObject::get_rigidity );
	ClassDB::bind_method(D_METHOD("get_enabled"),&QJointObject::get_enabled );

	//Set
	ClassDB::bind_method(D_METHOD("set_body_node_a","rigid_body_node"),&QJointObject::set_body_node_a );
	ClassDB::bind_method(D_METHOD("set_body_node_b","rigid_body_node"),&QJointObject::set_body_node_b );
	ClassDB::bind_method(D_METHOD("set_anchor_position_a","value"),&QJointObject::set_anchor_position_a );
	ClassDB::bind_method(D_METHOD("set_anchor_position_b","value"),&QJointObject::set_anchor_position_b );
	ClassDB::bind_method(D_METHOD("set_collision_enabled","value"),&QJointObject::set_collision_enabled );
	ClassDB::bind_method(D_METHOD("set_groove_enabled","value"),&QJointObject::set_groove_enabled );
	ClassDB::bind_method(D_METHOD("set_length","value"),&QJointObject::set_length );
	ClassDB::bind_method(D_METHOD("set_balance","value"),&QJointObject::set_balance );
	ClassDB::bind_method(D_METHOD("set_rigidity","value"),&QJointObject::set_rigidity );
	ClassDB::bind_method(D_METHOD("set_enabled","value"),&QJointObject::set_enabled );

	//Configuration
	ClassDB::bind_method(D_METHOD("configure","rigid_body_node_a","anchor_world_pos_a","anchor_world_pos_b","rigid_body_node_b","length"),&QJointObject::configure );
	ClassDB::bind_method(D_METHOD("configure_with_current_distance","rigid_body_node_a","anchor_world_pos_a","anchor_world_pos_b","rigid_body_node_b"),&QJointObject::configure_with_current_distance );
	ClassDB::bind_method(D_METHOD("configure_with_common_anchor_position","rigid_body_node_a","common_anchor_world_pos","rigid_body_node_b"),&QJointObject::configure_with_common_anchor_position );

}


//GET
QRigidBodyNode *QJointObject::get_body_node_a() {
	return bodyA;
}

QRigidBodyNode *QJointObject::get_body_node_b() {
	return bodyB;
}

Vector2 QJointObject::get_anchor_position_a() {
	QVector res=jointObject->GetAnchorAPosition();
	return Vector2(res.x,res.y);
}

Vector2 QJointObject::get_anchor_position_b() {
	QVector res=jointObject->GetAnchorBPosition();
	return Vector2(res.x,res.y);
}

Vector2 QJointObject::get_anchor_global_position_a() {
	QVector res=jointObject->GetAnchorAGlobalPosition();
	return Vector2(res.x,res.y);
}

Vector2 QJointObject::get_anchor_global_position_b() {
	QVector res=jointObject->GetAnchorBGlobalPosition();
	return Vector2(res.x,res.y);
}

bool QJointObject::get_collisions_enabled() {
	return jointObject->GetCollisionEnabled();
}

bool QJointObject::get_groove_enabled() {
	return jointObject->GetGrooveEnabled();
}

float QJointObject::get_length() {
	return jointObject->GetLength();
}

float QJointObject::get_balance() {
	return jointObject->GetBalance();
}

float QJointObject::get_rigidity() {
	return jointObject->GetRigidity();
}

bool QJointObject::get_enabled() {
	return jointObject->GetEnabled();
}

//SET
Ref<QJointObject> QJointObject::set_body_node_a(Object *rigid_body_node) {
	
	if(rigid_body_node==nullptr){
		jointObject->SetBodyA(nullptr);
		bodyA=nullptr;
		return Ref<QJointObject>(this);
	}
	if(rigid_body_node->get_class()!="QRigidBodyNode"){
		godot::UtilityFunctions::printerr("Quark Physics Error: Invalid rigid body object type! | QJointObject.set_body_node_a() ");
		return Ref<QJointObject>(this);
	}
	bodyA=static_cast<QRigidBodyNode*>(rigid_body_node);
	QRigidBody *bodyObject=static_cast<QRigidBody*>(bodyA->bodyObject);

	jointObject->SetBodyA(bodyObject);
	
	return Ref<QJointObject>(this);
}

Ref<QJointObject> QJointObject::set_body_node_b(Object *rigid_body_node) {
	if(rigid_body_node==nullptr){
		jointObject->SetBodyB(nullptr);
		bodyB=nullptr;
		return Ref<QJointObject>(this);
	}
	
	if(rigid_body_node->get_class()!="QRigidBodyNode"){
		godot::UtilityFunctions::printerr("Quark Physics Error: Invalid rigid body object type! | QJointObject.set_body_node_b() ");
		return Ref<QJointObject>(this);
	}
	bodyB=static_cast<QRigidBodyNode*>(rigid_body_node);
	QRigidBody *bodyObject=static_cast<QRigidBody*>(bodyB->bodyObject);

	jointObject->SetBodyB(bodyObject);
	return Ref<QJointObject>(this);
}

Ref<QJointObject> QJointObject::set_anchor_position_a(Vector2 value) {
	jointObject->SetAnchorAPosition(QVector(value.x,value.y));
	return Ref<QJointObject>(this);
}

Ref<QJointObject> QJointObject::set_anchor_position_b(Vector2 value) {
	jointObject->SetAnchorBPosition(QVector(value.x,value.y));
	return Ref<QJointObject>(this);
}

Ref<QJointObject> QJointObject::set_collision_enabled(bool value) {
	jointObject->SetCollisionEnabled(value);
	return Ref<QJointObject>(this);
}

Ref<QJointObject> QJointObject::set_groove_enabled(bool value) {
	jointObject->SetGrooveEnabled(true);
	return Ref<QJointObject>(this);
}

Ref<QJointObject> QJointObject::set_length(float value) {
	jointObject->SetLength(value);
	return Ref<QJointObject>(this);
}

Ref<QJointObject> QJointObject::set_balance(float value) {
	jointObject->SetBalance(value);
	return Ref<QJointObject>(this);
}

Ref<QJointObject> QJointObject::set_rigidity(float value) {
	jointObject->SetRigidity(value);
	return Ref<QJointObject>(this);
}

Ref<QJointObject> QJointObject::set_enabled(bool value) {
	jointObject->SetEnabled(value);
	return Ref<QJointObject>(this);
}
