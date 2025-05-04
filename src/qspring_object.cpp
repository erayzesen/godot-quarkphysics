#include "qspring_object.h"


void QSpringObject::_bind_methods() {
    //get
    
    ClassDB::bind_method(D_METHOD("get_particle_a"),&QSpringObject::get_particle_a );
    ClassDB::bind_method(D_METHOD("get_particle_b"),&QSpringObject::get_particle_b );
    ClassDB::bind_method(D_METHOD("get_length"),&QSpringObject::get_length );
    ClassDB::bind_method(D_METHOD("get_is_internal"),&QSpringObject::get_is_internal );
    ClassDB::bind_method(D_METHOD("get_rigidity"),&QSpringObject::get_rigidity );
    ClassDB::bind_method(D_METHOD("get_distance_limit_enabled"),&QSpringObject::get_distance_limit_enabled );
    ClassDB::bind_method(D_METHOD("get_minimum_distance_factor"),&QSpringObject::get_minimum_distance_factor );
    ClassDB::bind_method(D_METHOD("get_maximum_distance_factor"),&QSpringObject::get_maximum_distance_factor );
    ClassDB::bind_method(D_METHOD("get_enabled"),&QSpringObject::get_enabled );
    //set
    ClassDB::bind_method(D_METHOD("set_particle_a","particle_object"),&QSpringObject::set_particle_a );
    ClassDB::bind_method(D_METHOD("set_particle_b","particle_object"),&QSpringObject::set_particle_b );
    ClassDB::bind_method(D_METHOD("set_length","value"),&QSpringObject::set_length );
    ClassDB::bind_method(D_METHOD("set_is_internal","value"),&QSpringObject::set_is_internal );
    ClassDB::bind_method(D_METHOD("set_rigidity","value"),&QSpringObject::set_rigidity );
    ClassDB::bind_method(D_METHOD("set_distance_limit_enabled","value"),&QSpringObject::set_distance_limit_enabled );
    ClassDB::bind_method(D_METHOD("set_minimum_distance_factor","value"),&QSpringObject::set_minimum_distance_factor );
    ClassDB::bind_method(D_METHOD("set_maximum_distance_factor","value"),&QSpringObject::set_maximum_distance_factor );
    ClassDB::bind_method(D_METHOD("set_enabled","value"),&QSpringObject::set_enabled );

    ClassDB::bind_method(D_METHOD("configure","particle_object_a","particle_object_b","length","internal"),&QSpringObject::configure );
    ClassDB::bind_method(D_METHOD("configure_with_current_distance","particle_object_a","particle_object_b","internal"),&QSpringObject::configure_with_current_distance );
}


//GET
Ref<QParticleObject> QSpringObject::get_particle_a() {
	return pA;
}

Ref<QParticleObject> QSpringObject::get_particle_b() {
	return pB;
}

float QSpringObject::get_length() {
	return springObject->GetLength();
}

bool QSpringObject::get_is_internal(){
    return springObject->GetIsInternal();
}

float QSpringObject::get_rigidity() {
	return springObject->GetRigidity();
}

bool QSpringObject::get_distance_limit_enabled()
{
    return springObject->GetDistanceLimitEnabled();
}

float QSpringObject::get_minimum_distance_factor()
{
    return springObject->GetMinimumDistanceFactor();
}

float QSpringObject::get_maximum_distance_factor()
{
    return springObject->GetMaximumDistanceFactor();
}

bool QSpringObject::get_enabled() {
	return springObject->GetEnabled();
}

//SET

Ref<QSpringObject> QSpringObject::set_particle_a(Ref<QParticleObject> particle_object) {
    pA=particle_object;
    if(particle_object!=nullptr)
        springObject->SetParticleA(particle_object->particleObject);
    else 
        springObject->SetParticleA(nullptr);
	return Ref<QSpringObject>(this);
}
Ref<QSpringObject> QSpringObject::set_particle_b(Ref<QParticleObject> particle_object) {
    pB=particle_object;
    if(particle_object!=nullptr)
        springObject->SetParticleB(particle_object->particleObject);
    else 
        springObject->SetParticleB(nullptr);
	return Ref<QSpringObject>(this);
}

Ref<QSpringObject> QSpringObject::set_length(float value) {
    springObject->SetLength(value);
	return Ref<QSpringObject>(this);
}

Ref<QSpringObject> QSpringObject::set_is_internal(bool value) {
	springObject->SetIsInternal(value);
	return Ref<QSpringObject>(this);
}

Ref<QSpringObject> QSpringObject::set_rigidity(float value) {
	springObject->SetRigidity(value);
	return Ref<QSpringObject>(this);
}

Ref<QSpringObject> QSpringObject::set_distance_limit_enabled(bool value)
{
    springObject->SetDistanceLimitEnabled(value);
    return Ref<QSpringObject>(this);
}

Ref<QSpringObject> QSpringObject::set_minimum_distance_factor(float value)
{
    springObject->SetMinimumDistanceFactor(value);
    return Ref<QSpringObject>(this);
}

Ref<QSpringObject> QSpringObject::set_maximum_distance_factor(float value)
{
    springObject->SetMaximumDistanceFactor(value);
    return Ref<QSpringObject>(this);
}

Ref<QSpringObject> QSpringObject::set_enabled(bool value) {
	springObject->SetEnabled(value);
	return Ref<QSpringObject>(this);
}



