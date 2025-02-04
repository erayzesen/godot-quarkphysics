#include "qparticle_object.h"
#include "qmesh_node.h"

void QParticleObject::_bind_methods() {
    //Configure 
    ClassDB::bind_method(D_METHOD( "configure","position","radius" ),&QParticleObject::configure );
    //Get
    ClassDB::bind_method(D_METHOD( "get_global_position" ),&QParticleObject::get_global_position );
    ClassDB::bind_method(D_METHOD( "get_previous_global_position" ),&QParticleObject::get_previous_global_position );
    ClassDB::bind_method(D_METHOD( "get_position" ),&QParticleObject::get_position );
    ClassDB::bind_method(D_METHOD( "get_mass" ),&QParticleObject::get_mass );
    ClassDB::bind_method(D_METHOD( "get_owner_mesh_node" ),&QParticleObject::get_owner_mesh_node );
    ClassDB::bind_method(D_METHOD( "get_radius" ),&QParticleObject::get_radius );
    ClassDB::bind_method(D_METHOD( "get_is_internal" ),&QParticleObject::get_is_internal );
    ClassDB::bind_method(D_METHOD( "get_force" ),&QParticleObject::get_force );
    ClassDB::bind_method(D_METHOD( "get_enabled" ),&QParticleObject::get_enabled );
    ClassDB::bind_method(D_METHOD( "get_is_lazy" ),&QParticleObject::get_is_lazy );

    //Set
    ClassDB::bind_method(D_METHOD( "set_global_position","value" ),&QParticleObject::set_global_position );
    ClassDB::bind_method(D_METHOD( "add_global_position","value" ),&QParticleObject::add_global_position );
    ClassDB::bind_method(D_METHOD( "set_previous_global_position","value" ),&QParticleObject::set_previous_global_position );
    ClassDB::bind_method(D_METHOD( "add_previous_global_position","value" ),&QParticleObject::add_previous_global_position );
    ClassDB::bind_method(D_METHOD( "set_position","value" ),&QParticleObject::set_position );
    ClassDB::bind_method(D_METHOD( "add_position","value" ),&QParticleObject::add_position );
    ClassDB::bind_method(D_METHOD( "set_mass","value" ),&QParticleObject::set_mass );
    ClassDB::bind_method(D_METHOD( "set_radius","value" ),&QParticleObject::set_radius );
    ClassDB::bind_method(D_METHOD( "set_is_internal","value" ),&QParticleObject::set_is_internal );
    ClassDB::bind_method(D_METHOD( "apply_force","value" ),&QParticleObject::apply_force );
    ClassDB::bind_method(D_METHOD( "set_force","value" ),&QParticleObject::set_force );
    ClassDB::bind_method(D_METHOD( "add_force","value" ),&QParticleObject::add_force );
    ClassDB::bind_method(D_METHOD( "set_enabled","value" ),&QParticleObject::set_enabled );
    ClassDB::bind_method(D_METHOD( "set_is_lazy","value" ),&QParticleObject::set_is_lazy );


}

//GET METHODS
Vector2 QParticleObject::get_global_position() {
    QVector value=particleObject->GetGlobalPosition();
	return Vector2(value.x,value.y);
}

Vector2 QParticleObject::get_previous_global_position() {
	QVector value=particleObject->GetPreviousGlobalPosition();
	return Vector2(value.x,value.y);
}

Vector2 QParticleObject::get_position() {
	QVector value=particleObject->GetPosition();
	return Vector2(value.x,value.y);
}

float QParticleObject::get_mass() {
	return particleObject->GetMass();
}

QMeshNode *QParticleObject::get_owner_mesh_node() {
	return ownerMeshNode;
}

float QParticleObject::get_radius() {
	return particleObject->GetRadius();
}

bool QParticleObject::get_is_internal() {
	return particleObject->GetIsInternal();
}

Vector2 QParticleObject::get_force() {
	QVector value=particleObject->GetForce();
	return Vector2(value.x,value.y);
}

bool QParticleObject::get_enabled()
{
    return particleObject->GetEnabled();
}

bool QParticleObject::get_is_lazy()
{
    return particleObject->GetIsLazy();
}

//SET METHODS
Ref<QParticleObject> QParticleObject::set_global_position(Vector2 value) {
	particleObject->SetGlobalPosition(QVector(value.x,value.y) );
    return Ref<QParticleObject>(this);
}

Ref<QParticleObject> QParticleObject::add_global_position(Vector2 value) {
	particleObject->AddGlobalPosition(QVector(value.x,value.y) );
    return Ref<QParticleObject>(this);
}

Ref<QParticleObject> QParticleObject::set_previous_global_position(Vector2 value) {
	particleObject->SetPreviousGlobalPosition(QVector(value.x,value.y) );
    return Ref<QParticleObject>(this);
}

Ref<QParticleObject> QParticleObject::add_previous_global_position(Vector2 value) {
	particleObject->AddPreviousGlobalPosition(QVector(value.x,value.y) );
    return Ref<QParticleObject>(this);
}

Ref<QParticleObject> QParticleObject::set_position(Vector2 value) {
	particleObject->SetPosition(QVector(value.x,value.y) );
    return Ref<QParticleObject>(this);
}

Ref<QParticleObject> QParticleObject::add_position(Vector2 value) {
	particleObject->AddPosition(QVector(value.x,value.y) );
    return Ref<QParticleObject>(this);
}

Ref<QParticleObject> QParticleObject::set_mass(float value) {
	particleObject->SetMass(value);
    return Ref<QParticleObject>(this);
}

Ref<QParticleObject> QParticleObject::set_radius(float value) {
	particleObject->SetRadius(value);
    return Ref<QParticleObject>(this);
}

Ref<QParticleObject> QParticleObject::set_is_internal(bool value) {
	particleObject->SetIsInternal(value);
    return Ref<QParticleObject>(this);
}

Ref<QParticleObject> QParticleObject::apply_force(Vector2 value) {
	particleObject->ApplyForce(QVector(value.x,value.y) );
    return Ref<QParticleObject>(this);
}

Ref<QParticleObject> QParticleObject::set_force(Vector2 value) {
	particleObject->ApplyForce(QVector(value.x,value.y) );
    return Ref<QParticleObject>(this);
}

Ref<QParticleObject> QParticleObject::add_force(Vector2 value) {
	particleObject->AddForce(QVector(value.x,value.y) );
    return Ref<QParticleObject>(this);
}

Ref<QParticleObject> QParticleObject::set_enabled(bool value)
{
    particleObject->SetEnabled(value);
    return Ref<QParticleObject>(this);
}

Ref<QParticleObject> QParticleObject::set_is_lazy(bool value)
{
    particleObject->SetIsLazy(value);
    return Ref<QParticleObject>(this);
}
