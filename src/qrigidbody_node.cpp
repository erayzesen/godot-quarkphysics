#include "qrigidbody_node.h"
#include "qworld_node.h"


void QRigidBodyNode::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_fixed_rotation_enabled"), &QRigidBodyNode::get_fixed_rotation_enabled );
    ClassDB::bind_method(D_METHOD("get_kinematic_enabled"), &QRigidBodyNode::get_kinematic_enabled );
    ClassDB::bind_method(D_METHOD("get_kinematic_collisions_enabled"), &QRigidBodyNode::get_kinematic_collisions_enabled );
    ClassDB::bind_method(D_METHOD("get_force"),&QRigidBodyNode::get_force );
    ClassDB::bind_method(D_METHOD("get_angular_force"),&QRigidBodyNode::get_angular_force );

    ClassDB::bind_method(D_METHOD("set_fixed_rotation_enabled","value"), &QRigidBodyNode::set_fixed_rotation_enabled );
    ClassDB::bind_method(D_METHOD("set_kinematic_enabled","value"), &QRigidBodyNode::set_kinematic_enabled );
    ClassDB::bind_method(D_METHOD("set_kinematic_collisions_enabled","value"), &QRigidBodyNode::set_kinematic_collisions_enabled );
    ClassDB::bind_method(D_METHOD("set_body_position_and_collide","value","with_previous_position"), &QRigidBodyNode::set_body_position_and_collide );
    ClassDB::bind_method(D_METHOD("apply_force","value","r","update_mesh_transforms"), &QRigidBodyNode::apply_force );
    ClassDB::bind_method(D_METHOD("apply_impulse","value","r"), &QRigidBodyNode::apply_impulse );
    ClassDB::bind_method(D_METHOD("set_force","value"),&QRigidBodyNode::set_force );
    ClassDB::bind_method(D_METHOD("add_force","value"),&QRigidBodyNode::add_force );
    ClassDB::bind_method(D_METHOD("set_angular_force","value"),&QRigidBodyNode::set_angular_force );
    ClassDB::bind_method(D_METHOD("add_angular_force","value"),&QRigidBodyNode::add_angular_force );

    ADD_PROPERTY( PropertyInfo(Variant::BOOL,"fixed_rotation"), "set_fixed_rotation_enabled","get_fixed_rotation_enabled" );
    ADD_PROPERTY( PropertyInfo(Variant::BOOL,"kinematic"), "set_kinematic_enabled","get_kinematic_enabled" );
    ADD_PROPERTY( PropertyInfo(Variant::BOOL,"kinematic_collisions"), "set_kinematic_collisions_enabled","get_kinematic_collisions_enabled" );

}



bool QRigidBodyNode::get_fixed_rotation_enabled() {
	auto body=(QRigidBody*)bodyObject;
    return body->GetFixedRotationEnabled();
}

bool QRigidBodyNode::get_kinematic_enabled() {
	auto body=(QRigidBody*)bodyObject;
    return body->GetKinematicEnabled();
}

bool QRigidBodyNode::get_kinematic_collisions_enabled() {
	auto body=(QRigidBody*)bodyObject;
    return body->GetKinematicCollisionsEnabled();
}

Vector2 QRigidBodyNode::get_force() {
    auto body=(QRigidBody*)bodyObject;
    QVector value=body->GetForce();
	return Vector2(value.x,value.y);
}

float QRigidBodyNode::get_angular_force() {
    auto body=(QRigidBody*)bodyObject;
	return body->GetAngularForce();
}

QRigidBodyNode *QRigidBodyNode::set_fixed_rotation_enabled(bool value) {
	auto body=(QRigidBody*)bodyObject;
    body->SetFixedRotationEnabled(value);
    return this;
}

QRigidBodyNode *QRigidBodyNode::set_kinematic_enabled(bool value) {
	auto body=(QRigidBody*)bodyObject;
    body->SetKinematicEnabled(value);
    update_meshes_draw();
    return this;
}

QRigidBodyNode *QRigidBodyNode::set_kinematic_collisions_enabled(bool value) {
	auto body=(QRigidBody*)bodyObject;
    body->SetKinematicCollisionsEnabled(value);
    return this;
}

QRigidBodyNode *QRigidBodyNode::set_body_position_and_collide(Vector2 value, bool with_previous_position) {
	auto body=(QRigidBody*)bodyObject;
    body->SetPositionAndCollide(QVector(value.x,value.y),with_previous_position);
    return this;
}


QRigidBodyNode * QRigidBodyNode::apply_force(Vector2 value, Vector2 r, bool update_mesh_transforms)
{
    auto body=(QRigidBody*)bodyObject;
    body->ApplyForce(QVector(value.x,value.y), QVector(r.x,r.y),update_mesh_transforms);
    return this;
}

QRigidBodyNode *QRigidBodyNode::apply_impulse(Vector2 value, Vector2 r) {
	auto body=(QRigidBody*)bodyObject;
    body->ApplyImpulse(QVector(value.x,value.y), QVector(r.x,r.y));
    return this;
}

QRigidBodyNode *QRigidBodyNode::set_force(Vector2 value) {
    auto body=(QRigidBody*)bodyObject;
	body->SetForce( QVector(value.x,value.y) );
	return this;
}

QRigidBodyNode *QRigidBodyNode::add_force(Vector2 value) {
    auto body=(QRigidBody*)bodyObject;
	body->AddForce( QVector(value.x,value.y) );
	return this;
}



QRigidBodyNode *QRigidBodyNode::set_angular_force(float value) {
    auto body=(QRigidBody*)bodyObject;
	body->SetAngularForce(value);
	return this;
}

QRigidBodyNode *QRigidBodyNode::add_angular_force(float value) {
    auto body=(QRigidBody*)bodyObject;
	body->AddAngularForce(value);
	return this;
}




